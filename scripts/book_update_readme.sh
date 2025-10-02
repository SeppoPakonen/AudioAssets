#!/usr/bin/env bash
set -euo pipefail

# Auto-generate Book/README.md entries from existing chapters.

ROOT_DIR="$(cd "$(dirname "$0")"/.. && pwd)"
BOOK_DIR="$ROOT_DIR/Book"
OUT="$BOOK_DIR/README.md"

shopt -s nullglob

echo "Book — Index" > "$OUT"
echo >> "$OUT"
echo "Purpose" >> "$OUT"
echo "- A quick index mapping narrative chapters to their compact agent notes." >> "$OUT"
echo "- Keep this updated whenever a new chapter is added." >> "$OUT"
echo >> "$OUT"
echo "Entries" >> "$OUT"

for chapter in "$BOOK_DIR"/[0-9][0-9][0-9]\ -\ *.md; do
  base="$(basename "$chapter")"
  nnn="${base%% *}"
  title_md="${base#??? - }"
  title_no_md="${title_md%.md}"
  echo "- $nnn — $title_no_md" >> "$OUT"
  echo "  - Chapter: \`$base\`" >> "$OUT"
  echo "  - Compact: \`$title_md\`" >> "$OUT"
done

echo >> "$OUT"
echo "Conventions" >> "$OUT"
echo "- Chapters: \`Book/%03d - <Title>.md\`" >> "$OUT"
echo "- Compact: \`Book/<Title>.md\`" >> "$OUT"
echo "- Always create/update both together." >> "$OUT"
echo >> "$OUT"
echo "Maintenance" >> "$OUT"
echo "- Run \`scripts/book_check.sh\` to verify chapter↔compact pairing." >> "$OUT"

echo "Generated Book/README.md with $(ls -1 "$BOOK_DIR"/[0-9][0-9][0-9]\ -\ *.md 2>/dev/null | wc -l | tr -d ' ') entries." >&2

