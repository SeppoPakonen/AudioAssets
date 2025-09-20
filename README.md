# AudioAssets

These are my music and soundfiles. I have composed allmost all of these songs. Files are in open source formats.

Usage is permitted under custom license.
Please contact me for commercial usage.

Songs are sorted to folders based on their original composition year. They show well my learning process. I was 11 years old in 2001, when I started, which should give the perspective.

## Start Here
- Open `escsrc/Timeline/Timeline.upp` in TheIDE to browse all active years and albums.
- Only packages and years referenced by Timeline are considered active. Unreferenced material lives in `escsrc/_attic/`.
- Obsolete non-music tooling and notes have been moved under `legacy/` to keep the focus on `escsrc/`.


## Instructions
### File extensions:
- .tg files can be opened with TuxGuitar

### Other
- Press F5 in TuxGuitar to see my comments for the songs

### TuxGuitar
Enable "FluidSynth output plugin" in plugins and add [FluidR3_GM.sf2](http://www.ronimusic.com/sf2/FluidR3_GM.sf2) as your soundfont.
Go to TuxGuitar sound settings (Tools -> Settings -> Sound) and in MIDI-port section select "TG Fluidsynth [FluidR3 GM]".

## Local Web Preview (/www)
Static HTML for browsing the timeline lives under `www/`. Generate it and serve it locally with a simple Node.js static server.

- Generate the site
  - `python3 scripts/generate_www.py`

- Serve `www/` as the site root (npm-based, no install needed via npx)
  - `npx http-server www -p 8080 -a 127.0.0.1 -c-1`
  - or: `npx serve www -l 8080`
  - open: `xdg-open http://127.0.0.1:8080/`
  - helpers: `scripts/serve_www.sh` (Linux/macOS) or `scripts/serve_www.bat` (Windows)

Notes
- Serving `www/` as the root only exposes the generated pages and `www/assets`. Links that point back to source files under `escsrc/` will not resolve in this mode. That’s expected.
  - If you want those source-file links to work, serve the repository root instead: `npx http-server . -p 8080` (then open `http://127.0.0.1:8080/www/`).
  - Alternatively, extend `scripts/generate_www.py` to copy needed sources into `www/` during generation.
  - Helpers support both: `scripts/serve_www.sh --repo-root` or `scripts/serve_www.bat repo-root`.
