#!/usr/bin/env python3
"""
Remove empty MP3 template bullets from '## Meta' sections in escsrc/*/*.md:
  - lines like: "- ![ MP3]()" or "- ![MP3]()"
Also remove empty lines inside the Meta list so bullets are contiguous.

Edits only the Meta section; preserves all other content verbatim.
"""
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
ESC = ROOT / 'escsrc'

PLACEHOLDER_RE = re.compile(r"^\s*-\s*!\[\s*MP3\s*\]\(\s*\)\s*$", re.IGNORECASE)


def process_file(p: Path) -> bool:
    text = p.read_text(encoding='utf-8')
    lines = text.splitlines()
    # Find Meta section boundaries
    meta_start = None
    for i, ln in enumerate(lines):
        if ln.strip().lower() == '## meta':
            meta_start = i
            break
    if meta_start is None:
        return False

    meta_end = len(lines)
    for j in range(meta_start + 1, len(lines)):
        if lines[j].startswith('## '):
            meta_end = j
            break

    before = lines[:meta_start + 1]
    body = lines[meta_start + 1:meta_end]
    after = lines[meta_end:]

    changed = False
    new_body = []
    for ln in body:
        # drop placeholders
        if PLACEHOLDER_RE.match(ln):
            changed = True
            continue
        # skip empty lines inside Meta list
        if ln.strip() == '':
            changed = True
            continue
        new_body.append(ln)

    if not changed:
        return False

    # Reassemble, keep a single newline between sections by default
    out_lines = before + new_body + after
    p.write_text('\n'.join(out_lines) + ('\n' if text.endswith('\n') else ''), encoding='utf-8')
    return True


def main():
    count = 0
    for album_dir in ESC.iterdir():
        if not album_dir.is_dir():
            continue
        for p in album_dir.glob('*.md'):
            # Skip obvious non-song docs just in case
            low = p.name.lower()
            if low.startswith(('agents', 'eng archive', 'fin archive')):
                continue
            if process_file(p):
                count += 1
                print(f'Cleaned: {p}')
    print(f'Files cleaned: {count}')


if __name__ == '__main__':
    main()

