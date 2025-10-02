Title: Auto README Generator

I created a small generator to rebuild the Book index (`Book/README.md`) from the chapters on disk. Chief wanted a single command to keep the index authoritative without hand edits; this script writes the headers and enumerates every chapter with its matching compact entry.

What I did
- I added `scripts/book_update_readme.sh` that scans `Book/NNN - <Title>.md` files and regenerates the index.
- I kept the same index format so the checker remains compatible.

How to use
- Run from repo root: `scripts/book_update_readme.sh`
- Then verify: `scripts/book_check.sh`

Next steps
- Optionally wire this into a pre-commit or a simple make-like alias later.

