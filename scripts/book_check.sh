#!/usr/bin/env bash
set -euo pipefail

# Verify that each chapter Book/%03d - <Title>.md has a matching compact Book/<Title>.md
# and that Book/README.md lists all chapters with proper Chapter/Compact references.

ROOT_DIR="$(cd "$(dirname "$0")"/.. && pwd)"
BOOK_DIR="$ROOT_DIR/Book"
README="$BOOK_DIR/README.md"

missing_compact=()
readme_misses=()
ok=0

shopt -s nullglob
for chapter in "$BOOK_DIR"/[0-9][0-9][0-9]\ -\ *.md; do
  base="$(basename "$chapter")"
  title="${base#??? - }"   # strip leading NNN and ' - '
  compact="$BOOK_DIR/$title"
  if [[ -f "$compact" ]]; then
    echo "OK  - $base ↔ $(basename "$compact")"
    ((ok++))
  else
    echo "MISS-Compact for chapter: $base"
    missing_compact+=("$compact")
  fi

  # README sync checks per chapter
  nnn="${base%% *}"
  title_no_md="${title%.md}"
  chapter_line="- $nnn — $title_no_md"
  chapter_ref_line="  - Chapter: \`$base\`"
  compact_ref_line="  - Compact: \`$title\`"
  if [[ -f "$README" ]]; then
    grep -Fq "$chapter_line" "$README" || readme_misses+=("Entry line: $chapter_line")
    grep -Fq "$chapter_ref_line" "$README" || readme_misses+=("Chapter ref: $base")
    grep -Fq "$compact_ref_line" "$README" || readme_misses+=("Compact ref: $title")
  else
    readme_misses+=("README missing: $README")
  fi
done

echo "-- Summary: $ok chapter(s) verified, ${#missing_compact[@]} missing compact file(s)."

status=0
if [[ ${#missing_compact[@]} -gt 0 ]]; then
  echo "Missing compact files:" >&2
  for m in "${missing_compact[@]}"; do echo "  - $m" >&2; done
  status=1
fi

# README entry count sanity check
if [[ -f "$README" ]]; then
  entries_count=$(grep -E '^- [0-9]{3} — ' "$README" | wc -l | tr -d ' ')
  chapters_count=$(ls -1 "$BOOK_DIR"/[0-9][0-9][0-9]\ -\ *.md 2>/dev/null | wc -l | tr -d ' ')
  if [[ "$entries_count" != "$chapters_count" ]]; then
    echo "README entry count mismatch: entries=$entries_count chapters=$chapters_count" >&2
    status=1
  fi
fi

if [[ ${#readme_misses[@]} -gt 0 ]]; then
  echo "README missing/mismatched lines:" >&2
  for r in "${readme_misses[@]}"; do echo "  - $r" >&2; done
  status=1
fi

exit $status

