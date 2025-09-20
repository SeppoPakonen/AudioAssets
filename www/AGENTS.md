AudioAssets — Web Site (/www)

Purpose
- Static site generated from `escsrc/Timeline` for quick browsing.
- Old-browser friendly HTML/CSS; progressive eye candy on modern browsers.

How it’s built
- Generator: `scripts/generate_www.py` (no external deps).
- Outputs under `/www`:
  - `index.html` (Years + Albums)
  - `albums/<album-slug>/index.html`
  - `albums/<album-slug>/<song-slug>/index.html` for each `.md` song/doc with:
    - embedded HTML5 audio players for any MP3 links found in the `.md`
    - raw Markdown shown read-only (preserves line breaks)
  - `years/<year>/index.html`
  - `assets/style.css`, `assets/script.js`

Run
- Basic: `python3 scripts/generate_www.py`
- Polished: `python3 scripts/generate_www.py --polished` (filters list to song `.md` files; renders Markdown to HTML)
- Re-run whenever `.upp` files change.

Links & scope
- File links on album/year pages point to the source files outside `/www` (e.g., `../escsrc/...`).
  - Works when browsing locally from repo root (file://) or serving the repo root as web root.
- If deploying `/www` as a standalone site, either:
    - adjust links to an absolute base (e.g., set `<base href="https://example.com/repo/">`), or
    - copy/download the referenced files into `/www` and update links.
  - In polished mode, album pages list only relevant `.md` songs (skips `.tg`, `XX*`, archive, and agent docs). Per‑song pages render Markdown to HTML.

Creative rules
- This site only surfaces existing files and their structure; it does not change lyrics or compositions.
- Keep FI/EN parity and provenance in the sources; the site is a read-only view.

Next steps (suggested)
- Render `.md` song docs to HTML pages (read-only) for easier web reading.
- Surface album cover images next to packages (place small PNGs next to `*.upp`).
- Read album descriptions from `ENG Archive.txt`/`FIN Archive.txt` when present.
- Optional: add a lightweight search (client-side, no build step).
