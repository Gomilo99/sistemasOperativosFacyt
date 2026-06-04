#!/usr/bin/env python3
from __future__ import annotations

import argparse
import re
import sys
import textwrap
from dataclasses import dataclass
from pathlib import Path


DEFAULT_WRAP_WIDTH = 100


@dataclass(frozen=True)
class CleanupOptions:
    wrap_width: int
    fix_punctuation_spacing: bool
    normalize_hyphenation: bool
    promote_headings: bool
    join_wrapped_lines: bool


def _decode_with_bom(data: bytes) -> str:
    if data.startswith(b"\xff\xfe") or data.startswith(b"\xfe\xff"):
        return data.decode("utf-16", errors="replace")
    if data.startswith(b"\xef\xbb\xbf"):
        return data.decode("utf-8-sig", errors="replace")
    return data.decode("utf-8", errors="replace")


def _normalize_line_endings(text: str) -> str:
    return text.replace("\r\n", "\n").replace("\r", "\n")


def _toggle_fenced_code(line: str, in_code: bool) -> bool:
    stripped = line.lstrip()
    if stripped.startswith("```"):
        return not in_code
    return in_code


def _insert_breaks_before_tokens(text: str) -> str:
    # Insert paragraph breaks before tokens that often indicate structure.
    # We keep this conservative to avoid changing arbitrary in-line occurrences.
    tokens = [
        r"CHAPTER\s+\d+",
        r"Chapter\s+\d+",
        r"APPENDIX\s+[A-Z]",
        r"Appendix\s+[A-Z]",
        r"Bibliography\b",
        r"Index\b",
        r"References\b",
    ]
    pattern = re.compile(r"(?<!\n)\b(" + "|".join(tokens) + r")\b")
    return pattern.sub(r"\n\n\1", text)


def _fix_common_spacing(text: str) -> str:
    # Insert a space after punctuation when it is missing: "word.word" -> "word. word"
    # Avoid decimals like "3.14" or "v1.2".
    def repl(match: re.Match[str]) -> str:
        left, punct, right = match.group(1), match.group(2), match.group(3)
        return f"{left}{punct} {right}"

    # Letters around punctuation
    text = re.sub(r"([A-Za-z])([,;:!?])([A-Za-z])", repl, text)
    text = re.sub(r"([A-Za-z])(\.)([A-Za-z])", repl, text)

    # Close paren/bracket followed by a letter
    text = re.sub(r"([\)\]])([A-Za-z])", r"\1 \2", text)
    return text


def _normalize_hyphenation(text: str) -> str:
    # Common PDF extraction artifact: hyphen at line end that should be removed.
    # Example: "operat-\ning" -> "operating"
    return re.sub(r"-\n(?=[A-Za-z])", "", text)


def _promote_headings(lines: list[str]) -> list[str]:
    out: list[str] = []
    in_code = False

    chapter_re = re.compile(r"^\s*(CHAPTER|Chapter)\s+(\d+)\s*(.*)\s*$")
    appendix_re = re.compile(r"^\s*(APPENDIX|Appendix)\s+([A-Z])\s*(.*)\s*$")
    simple_headings = {
        "bibliography": "# Bibliography",
        "references": "# References",
        "index": "# Index",
    }

    for line in lines:
        in_code = _toggle_fenced_code(line, in_code)
        if in_code:
            out.append(line)
            continue

        stripped = line.strip()
        if not stripped:
            out.append(line)
            continue

        lowered = stripped.casefold()
        if lowered in simple_headings:
            out.append("")
            out.append(simple_headings[lowered])
            out.append("")
            continue

        m = chapter_re.match(line)
        if m:
            number = m.group(2)
            rest = m.group(3).strip()
            out.append("")
            out.append(f"# Chapter {number}")
            if rest:
                out.append(rest)
            out.append("")
            continue

        m = appendix_re.match(line)
        if m:
            letter = m.group(2)
            rest = m.group(3).strip()
            out.append("")
            out.append(f"# Appendix {letter}")
            if rest:
                out.append(rest)
            out.append("")
            continue

        out.append(line)

    return out


def _join_wrapped_lines_preserving_code(lines: list[str]) -> list[str]:
    # Many PDF->text conversions insert hard newlines mid-paragraph.
    # This joins single-newline wrapped lines into paragraphs.
    out: list[str] = []
    in_code = False
    buffer: list[str] = []

    def flush_paragraph() -> None:
        if not buffer:
            return
        paragraph = " ".join(s.strip() for s in buffer if s.strip())
        out.append(paragraph)
        buffer.clear()

    for line in lines:
        in_code = _toggle_fenced_code(line, in_code)
        if in_code:
            flush_paragraph()
            out.append(line)
            continue

        if not line.strip():
            flush_paragraph()
            out.append("")
            continue

        if line.lstrip().startswith("#"):
            flush_paragraph()
            out.append(line.strip())
            continue

        # Keep list items as separate paragraphs; joining them tends to mangle markdown.
        if re.match(r"^\s*([-*+]|\d+\.)\s+", line):
            flush_paragraph()
            out.append(line.rstrip())
            continue

        buffer.append(line)

    flush_paragraph()
    return out


def _wrap_paragraphs(lines: list[str], width: int) -> list[str]:
    out: list[str] = []
    in_code = False

    for line in lines:
        in_code = _toggle_fenced_code(line, in_code)
        if in_code or not line.strip() or line.lstrip().startswith("#"):
            out.append(line.rstrip())
            continue

        if re.match(r"^\s*([-*+]|\d+\.)\s+", line):
            out.append(line.rstrip())
            continue

        out.extend(textwrap.fill(line.strip(), width=width).splitlines())

    return out


def cleanup_markdown(text: str, options: CleanupOptions) -> str:
    text = _normalize_line_endings(text)

    if options.normalize_hyphenation:
        text = _normalize_hyphenation(text)

    if options.promote_headings:
        text = _insert_breaks_before_tokens(text)

    if options.fix_punctuation_spacing:
        text = _fix_common_spacing(text)

    lines = text.split("\n")
    if options.promote_headings:
        lines = _promote_headings(lines)

    if options.join_wrapped_lines:
        lines = _join_wrapped_lines_preserving_code(lines)

    if options.wrap_width > 0:
        lines = _wrap_paragraphs(lines, width=options.wrap_width)

    # Collapse excessive blank lines (keep max 2)
    cleaned: list[str] = []
    blank_run = 0
    for line in lines:
        if line.strip():
            blank_run = 0
            cleaned.append(line.rstrip())
            continue
        blank_run += 1
        if blank_run <= 2:
            cleaned.append("")

    return "\n".join(cleaned).rstrip() + "\n"


def _parse_args(argv: list[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description=(
            "Post-process a messy PDF-to-Markdown transcription (e.g. markitdown output). "
            "This script is designed to be content-agnostic and safe to run locally."
        )
    )
    parser.add_argument(
        "--input",
        "-i",
        required=True,
        help="Path to the input Markdown file (supports UTF-16 with BOM).",
    )
    parser.add_argument(
        "--output",
        "-o",
        required=True,
        help="Path to write cleaned Markdown (UTF-8).",
    )
    parser.add_argument(
        "--wrap-width",
        type=int,
        default=DEFAULT_WRAP_WIDTH,
        help="Reflow paragraphs to this width. Use 0 to disable wrapping.",
    )
    parser.add_argument(
        "--no-punct-spacing",
        action="store_true",
        help="Disable punctuation spacing fixes.",
    )
    parser.add_argument(
        "--no-hyphenation",
        action="store_true",
        help="Disable de-hyphenation across line breaks.",
    )
    parser.add_argument(
        "--no-headings",
        action="store_true",
        help="Disable basic CHAPTER/APPENDIX heading promotion.",
    )
    parser.add_argument(
        "--no-join-lines",
        action="store_true",
        help="Disable joining single-newline wrapped lines into paragraphs.",
    )
    return parser.parse_args(argv)


def main(argv: list[str]) -> int:
    args = _parse_args(argv)
    input_path = Path(args.input)
    output_path = Path(args.output)

    options = CleanupOptions(
        wrap_width=max(0, int(args.wrap_width)),
        fix_punctuation_spacing=not args.no_punct_spacing,
        normalize_hyphenation=not args.no_hyphenation,
        promote_headings=not args.no_headings,
        join_wrapped_lines=not args.no_join_lines,
    )

    data = input_path.read_bytes()
    text = _decode_with_bom(data)
    cleaned = cleanup_markdown(text, options)

    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text(cleaned, encoding="utf-8", newline="\n")

    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))

