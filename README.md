# sistemasOperativosFacyt
Repositorio recopilatorio de información y estudio para la materia Sistemas Operativos 

## Libro (transcripción a Markdown)
Por temas de copyright, este repo no debería versionar una “copia limpia” completa del libro. En su lugar, hay un script para reordenar/limpiar una transcripción ya generada (por ejemplo con `markitdown`) de forma local.

- Script: `tools/osc_markdown_cleanup.py`
- Ejemplo (genera salida local ignorada por git): `python tools/osc_markdown_cleanup.py -i "Recursos/Operating System Concepts 10th 2018.md" -o ".osc-generated/Operating System Concepts 10th 2018.cleaned.md"`
- Tip: si el resultado queda “demasiado refluido”, prueba `--wrap-width 0` para no envolver líneas.
