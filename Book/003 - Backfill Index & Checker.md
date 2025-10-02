Title: Backfill Index & Checker

I backfilled a proper index for the Book and added a small verification script to keep chapters and compact entries paired correctly. Curator asked for a tidy overview and a checkable workflow; I delivered both and logged the change here in first person with a matching compact note.

What I did
- I created `Book/README.md` that lists each chapter and its title-based compact file.
- I wrote `scripts/book_check.sh` to verify that every `NNN - <Title>.md` has a matching `Book/<Title>.md` compact file.
- I kept filenames and conventions consistent with our earlier decisions.

How to use the checker
- Make sure the script is executable: `chmod +x scripts/book_check.sh` (already set in this repo).
- Run it from the repo root: `scripts/book_check.sh`
- It prints OK lines for each valid pair and exits non-zero if any compact file is missing.

Next steps
- Update the index whenever a new chapter is added.
- Extend the checker if we add more metadata (dates, tags) to the compact files.

