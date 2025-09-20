Web Site Tasks (WIP)

Status
- Initial static site scaffold generated from `escsrc/Timeline`.
- Albums and Years pages list files under each separator.
- Progressive enhancement: subtle background animation; JS optional.

Open tasks
1) Song/album content pages
   - Render `.md` to clean HTML (no edits to source text).
   - Preserve code fences and background blocks.
2) Album metadata
   - Read snippets from `ENG Archive.txt`/`FIN Archive.txt` when available.
3) Covers
   - Add small PNG covers per album (avoid large binaries).
   - Show cover thumbnails on the Albums grid.
4) Links strategy
   - Decide: keep links pointing to `../escsrc/...` (works locally) or bundle copies under `/www` for standalone hosting.
5) Accessibility
   - Verify color contrast and keyboard navigation.

Notes
- Do not alter `.upp` files for website purposes.
- Keep the generator minimal and dependency-free.

