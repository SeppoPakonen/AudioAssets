#!/usr/bin/env python3
"""
Add MP3 links to the '## Meta' section of song Markdown files under escsrc/*/*.md.

The mapping is inferred from the MP3 URLs: the last path segment provides the
track title; the collection name maps to the album folder.

Rules:
- Search for a '## Meta' section; append bullet lines with image-style markdown:
  - ![<Title> MP3](<URL>)
- Avoid duplicates if the exact URL already appears anywhere in the file.
- If a file has no '## Meta', create one below the H1.

This script is idempotent and safe to re-run.
"""
from __future__ import annotations
import os
import sys
import re
import urllib.parse
import unicodedata
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
ESC = ROOT / 'escsrc'

URLS = [
    # 01 Freshman (EN)
    "https://archive.org/download/steve_chill_freshman/01%20-%20One%20out%20of%20four.mp3",
    "https://archive.org/download/steve_chill_freshman/02%20-%20To%20Victory%20or%20Ruin.mp3",
    "https://archive.org/download/steve_chill_freshman/03%20-%20Guitar%20soul.mp3",
    "https://archive.org/download/steve_chill_freshman/04%20-%20Pizza%20and%20Candy.mp3",
    "https://archive.org/download/steve_chill_freshman/05%20-%20Gotta%20Go%20in%20Summer.mp3",
    "https://archive.org/download/steve_chill_freshman/06%20-%20Summer%20is%20over.mp3",
    "https://archive.org/download/steve_chill_freshman/07%20-%20You%27re%20Just%20Awful.mp3",
    "https://archive.org/download/steve_chill_freshman/08%20-%20What%27s%20going%20on.mp3",
    "https://archive.org/download/steve_chill_freshman/09%20-%20Worse%20than%20the%20Worst.mp3",
    "https://archive.org/download/steve_chill_freshman/10%20-%20Girl%20I%20Never%20Touched.mp3",
    "https://archive.org/download/steve_chill_freshman/11%20-%20Invincible.mp3",
    "https://archive.org/download/steve_chill_freshman/12%20-%20What%27s%20up.mp3",
    "https://archive.org/download/steve_chill_freshman/13%20-%20Ship.mp3",
    # 02 Sophomore (EN)
    "https://archive.org/download/steve_chill_sophomore/01%20-%20Let%27s%20Go%20Somewhere%20Just%20Me%20And%20You.mp3",
    "https://archive.org/download/steve_chill_sophomore/02%20-%20Old%20tune.mp3",
    "https://archive.org/download/steve_chill_sophomore/03%20-%20Mornings.mp3",
    "https://archive.org/download/steve_chill_sophomore/04%20-%20Cougar.mp3",
    "https://archive.org/download/steve_chill_sophomore/05%20-%20I%20want%20you.mp3",
    "https://archive.org/download/steve_chill_sophomore/06%20-%20Thief%20of%20the%20Heart.mp3",
    "https://archive.org/download/steve_chill_sophomore/07%20-%20My%20jittering.mp3",
    "https://archive.org/download/steve_chill_sophomore/08%20-%20Peepin%27%20people.mp3",
    "https://archive.org/download/steve_chill_sophomore/09%20-%20Winter%20night.mp3",
    "https://archive.org/download/steve_chill_sophomore/10%20-%20I%20no%20longer.mp3",
    "https://archive.org/download/steve_chill_sophomore/11%20-%20Great%20Escape.mp3",
    "https://archive.org/download/steve_chill_sophomore/12%20-%20Tentacles.mp3",
    "https://archive.org/download/steve_chill_sophomore/13%20-%20We%E2%80%99re%20the%20One%20True%20Band.mp3",
    "https://archive.org/download/steve_chill_sophomore/14%20-%20Late.mp3",
    "https://archive.org/download/steve_chill_sophomore/15%20-%20Truth%20or%20Dare.mp3",
    # 03 The Band (EN)
    "https://archive.org/download/steve_chill_to_the_band/01%20-%20I%20don%27t%20know.mp3",
    "https://archive.org/download/steve_chill_to_the_band/02%20-%20Come%20to%20Bed%20Now.mp3",
    "https://archive.org/download/steve_chill_to_the_band/03%20-%20Girlfriend%20from%20Canada.mp3",
    "https://archive.org/download/steve_chill_to_the_band/04%20-%20I%20Bear%20Thee%20a%20Message.mp3",
    "https://archive.org/download/steve_chill_to_the_band/05%20-%20Shitty%20City.mp3",
    "https://archive.org/download/steve_chill_to_the_band/06%20-%20The%20Treehouse.mp3",
    "https://archive.org/download/steve_chill_to_the_band/07%20-%20She.mp3",
    "https://archive.org/download/steve_chill_to_the_band/08%20-%20Into%20Space.mp3",
    "https://archive.org/download/steve_chill_to_the_band/09%20-%20Eat%20Shit.mp3",
    "https://archive.org/download/steve_chill_to_the_band/10%20-%20Is%20This%20Any%20Real%20Help.mp3",
    # 01 Freshman (FI)
    "https://archive.org/download/oulupoko_ylaaste/04%20-%20Pitsaa%20ja%20karkkia.mp3",
    "https://archive.org/download/oulupoko_ylaaste/05%20-%20Kes%C3%A4l%20pakko%20on.mp3",
    "https://archive.org/download/oulupoko_ylaaste/06%20-%20Kes%C3%A4%20on%20ohi.mp3",
    "https://archive.org/download/oulupoko_ylaaste/07%20-%20S%C3%A4%20oot%20kamala.mp3",
    "https://archive.org/download/oulupoko_ylaaste/08%20-%20Mit%C3%A4%20tapahtuu.mp3",
    "https://archive.org/download/oulupoko_ylaaste/09%20-%20Pahuutta%20pahempaa.mp3",
    "https://archive.org/download/oulupoko_ylaaste/10%20-%20Girl%20I%20never%20had.mp3",
    "https://archive.org/download/oulupoko_ylaaste/11%20-%20Invincible.mp3",
    "https://archive.org/download/oulupoko_ylaaste/12%20-%20Mik%C3%A4%20on.mp3",
    "https://archive.org/download/oulupoko_ylaaste/13%20-%20Laiva.mp3",
    # 02 Sophomore (FI)
    "https://archive.org/download/oulupoko_lukioon/01%20-%20Menn%C3%A4%C3%A4n%20jonnekkin%20kahestaan.mp3",
    "https://archive.org/download/oulupoko_lukioon/02%20-%20Vanha%20s%C3%A4vel.mp3",
    "https://archive.org/download/oulupoko_lukioon/03%20-%20Aamuisin.mp3",
    "https://archive.org/download/oulupoko_lukioon/04%20-%20Puuma.mp3",
    "https://archive.org/download/oulupoko_lukioon/05%20-%20Tahdon%20sut.mp3",
    "https://archive.org/download/oulupoko_lukioon/06%20-%20Syd%C3%A4men%20varas.mp3",
    "https://archive.org/download/oulupoko_lukioon/07%20-%20T%C3%A4risen.mp3",
    "https://archive.org/download/oulupoko_lukioon/08%20-%20Ujostus.mp3",
    "https://archive.org/download/oulupoko_lukioon/09%20-%20Talviy%C3%B6.mp3",
    "https://archive.org/download/oulupoko_lukioon/10%20-%20M%C3%A4%20en%20en%C3%A4%C3%A4.mp3",
    "https://archive.org/download/oulupoko_lukioon/11%20-%20Great%20Escape.mp3",
    "https://archive.org/download/oulupoko_lukioon/12%20-%20Lonkerot.mp3",
    "https://archive.org/download/oulupoko_lukioon/13%20-%20The%20b%C3%A4ndi.mp3",
    "https://archive.org/download/oulupoko_lukioon/14%20-%20My%C3%B6h%C3%A4ss%C3%A4.mp3",
    "https://archive.org/download/oulupoko_lukioon/15%20-%20Totuus%20vai%20teht%C3%A4v%C3%A4.mp3",
    # 03 The Band (FI)
    "https://archive.org/download/oulupoko_bandiin/01%20-%20M%C3%A4%20tied%C3%A4%20en.mp3",
    "https://archive.org/download/oulupoko_bandiin/02%20-%20Nolo%20polo.mp3",
    "https://archive.org/download/oulupoko_bandiin/03%20-%20Tuu%20jo%20s%C3%A4nkyyn.mp3",
    "https://archive.org/download/oulupoko_bandiin/04%20-%20Girlfriend%20from%20Canada.mp3",
    "https://archive.org/download/oulupoko_bandiin/05%20-%20Viestin%20sulle%20tuon.mp3",
    "https://archive.org/download/oulupoko_bandiin/06%20-%20Paska%20kaupunni.mp3",
    "https://archive.org/download/oulupoko_bandiin/07%20-%20Puumaja.mp3",
    "https://archive.org/download/oulupoko_bandiin/08%20-%20H%C3%A4n.mp3",
    "https://archive.org/download/oulupoko_bandiin/09%20-%20Avaruuteen.mp3",
    "https://archive.org/download/oulupoko_bandiin/10%20-%20Sy%C3%B6%20paskaa.mp3",
    "https://archive.org/download/oulupoko_bandiin/11%20-%20Onko%20t%C3%A4%C3%A4%20apua.mp3",
]


def norm(s: str) -> str:
    return unicodedata.normalize('NFKC', s).casefold()


def parse_url_title(url: str) -> tuple[str, str, str | None]:
    """Return (collection, title, track_no) from archive.org URL.
    Title is the part after 'NN - ' and before '.mp3', percent-decoded.
    track_no is the leading NN if present, else None.
    """
    parts = urllib.parse.urlparse(url)
    seg = parts.path.rsplit('/', 1)[-1]
    seg = urllib.parse.unquote(seg)
    if seg.lower().endswith('.mp3'):
        seg = seg[:-4]
    # Drop leading track number and optional dash
    m = re.match(r'\s*(\d+)\s*-\s*(.*)$', seg)
    if m:
        track = m.group(1)
        title = m.group(2)
    else:
        track = None
        title = seg
    # Collection name: last folder in path under /download/
    col = parts.path.strip('/').split('/')[1] if '/download/' in parts.path else ''
    if not col:
        # handle other mirrors like /0/items/<collection>/file
        p = parts.path.strip('/').split('/')
        if 'items' in p:
            idx = p.index('items')
            if idx + 1 < len(p):
                col = p[idx + 1]
    return col, title, track


def find_md_files() -> list[Path]:
    files = []
    for album_dir in ESC.iterdir():
        if not album_dir.is_dir():
            continue
        for p in album_dir.glob('*.md'):
            # Skip archives/agents
            base = p.name.lower()
            if base.startswith(('agents', 'eng archive', 'fin archive')):
                continue
            if base.endswith('.md'):
                files.append(p)
    return files


def title_from_md_path(p: Path) -> str:
    base = p.stem
    # Remove track number prefix "NN " if present
    m = re.match(r'^\s*\d+\s+(.*)$', base)
    return m.group(1) if m else base


def choose_album_folder(col: str) -> str | None:
    col = col.lower()
    if 'freshman' in col or 'ylaaste' in col:
        return '01 Freshman'
    if 'sophomore' in col or 'lukioon' in col:
        return '02 Sophomore'
    if 'to_the_band' in col or 'bandiin' in col or 'to the band' in col:
        return '03 The Band'
    return None


def add_link_to_meta(md_path: Path, label: str, url: str) -> bool:
    text = md_path.read_text(encoding='utf-8')
    if url in text:
        return False
    lines = text.splitlines()
    # Find '## Meta' start/end
    meta_start = None
    for i, ln in enumerate(lines):
        if ln.strip().lower() == '## meta':
            meta_start = i
            break
    insert_at = None
    if meta_start is not None:
        # find end of meta (next '## ' heading or EOF)
        for j in range(meta_start + 1, len(lines)):
            if lines[j].startswith('## '):
                insert_at = j
                break
        if insert_at is None:
            insert_at = len(lines)
        new_line = f"- ![{label} MP3]({url})"
        lines.insert(insert_at, new_line)
        md_path.write_text('\n'.join(lines) + ('\n' if text.endswith('\n') else ''), encoding='utf-8')
        return True
    else:
        # Create a Meta section below the H1 if present, else at top
        h1_index = None
        for i, ln in enumerate(lines):
            if ln.startswith('# '):
                h1_index = i
                break
        new_block = [
            '## Meta',
            f'- ![{label} MP3]({url})',
            '',
        ]
        if h1_index is not None:
            insert_at = h1_index + 1
        else:
            insert_at = 0
        lines[insert_at:insert_at] = new_block
        md_path.write_text('\n'.join(lines) + ('\n' if text.endswith('\n') else ''), encoding='utf-8')
        return True


def main():
    # Build index: album -> title -> md_path
    md_files = find_md_files()
    index = {}
    for p in md_files:
        album = p.parent.name
        title = title_from_md_path(p)
        index.setdefault(album, {})[norm(title)] = p

    added_total = 0
    skipped = 0
    for url in URLS:
        col, url_title, track_no = parse_url_title(url)
        album = choose_album_folder(col)
        if not album:
            skipped += 1
            continue
        album_map = index.get(album, {})
        # Try direct title match first
        p = album_map.get(norm(url_title))
        if not p:
            # Fallback: try slight normalization of punctuation differences (ASCII vs curly quotes)
            # Replace ASCII apostrophe with curly and vice versa
            alt1 = url_title.replace("'", "’")
            alt2 = url_title.replace("’", "'")
            p = album_map.get(norm(alt1)) or album_map.get(norm(alt2))
        if not p:
            # As last resort, try startswith/contains matches ignoring case
            for tnorm, mp in album_map.items():
                if norm(url_title) in tnorm or tnorm in norm(url_title):
                    p = mp
                    break
        # For FI collections, if still not found, try track number mapping
        if not p and (col.startswith('oulupoko_')) and track_no:
            # Find file in album whose stem starts with f"{track_no} "
            for mp in (ESC / album).glob('*.md'):
                base = mp.stem
                if base.startswith(f'{track_no} '):
                    # Skip non-song docs
                    lower = base.lower()
                    if lower.startswith(('agents', 'eng archive', 'fin archive')):
                        continue
                    p = mp
                    break
        if not p:
            print(f'WARN: No match for URL title {url_title!r} in album {album} ({url})')
            skipped += 1
            continue
        label = url_title
        if add_link_to_meta(p, label, url):
            added_total += 1
            print(f'Added -> {p}: {url}')
        else:
            print(f'Exists -> {p}: {url}')

    print(f'Links added: {added_total}; skipped: {skipped}')


if __name__ == '__main__':
    main()
