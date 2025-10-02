Title: README Sync Check

I enhanced our checker to validate that `Book/README.md` stays in sync with the chapters and their compact entries. Director wanted a simple, strict signal when the index drifts; now the script fails if an entry line or Chapter/Compact reference is missing, or if the number of README entries doesn’t match the number of chapters.

What I changed
- I rewrote `scripts/book_check.sh` to:
  - Verify each `NNN - <Title>.md` has `Book/<Title>.md`.
  - Check that README contains the entry header line and both Chapter/Compact reference lines.
  - Compare the count of README entries to the number of chapter files.

How to use
- Run `scripts/book_check.sh` from repo root. It prints OK lines and exits non‑zero on any mismatch.

Next steps
- Keep the README index updated whenever adding a new chapter.
- Optionally extend with a TOC generator in the future.

