Current Task — Repository

- Primary focus: maintain and refine `escsrc/Timeline` (Timeline package).
- Keep `Timeline.upp` `uses` list up to date with years and albums.
- Ensure key packages include `AGENTS.md` and `CURRENT_TASK.md` and list them at the top of their `.upp` file lists.

Album Work (active now)
- 04 Summer Break, 05 Junior, 06 Prom:
  - Convert unfinished English demo `.txt` lyrics to structured `.md` modeled after `06 Prom/Love is my song.md`.
  - Add missing Finnish lyrics where possible (maintain FI/EN parity).
  - Rename `.tg` files that still have old project names to official English song titles; update `.upp` lists accordingly.
  - Keep `.upp` year separators intact (they anchor compositions to original years).
  - Create cover images for albums 04–06.
- Extend `.md` song/album docs to albums 01–06 for consistent structure.

Notes
- Keep `escsrc/06 Prom/06 Prom.upp` synchronized with actual files (e.g., `Love is my song.md`).

Rename Plan (initial draft)
- Approach
  - Use `.txt` titles and album docs to define canonical English titles per track.
  - Preserve track numbers and version suffixes like ` - a/b/c` (used downstream for version display).
  - Keep year separators intact in `.upp`; update only the filenames and `.upp` entries.
  - Apply renames with `git mv` so history is retained; verify TheIDE package file list reflects the new names.
- 04 Summer Break (capitalization/punctuation-only changes)
  - `01 mr loser.tg` -> `01 Mr Loser.tg`
  - `02 wrong way.tg` -> `02 Wrong Way.tg`
  - `03 two - b.tg` -> `03 Two - b.tg`
  - `04 smoke and mirrors.tg` -> `04 Smoke and Mirrors.tg`
  - `05 back to the disco.tg` -> `05 Back to the Disco.tg`
  - `06 pirate song - a.tg` -> `06 Pirate Song - a.tg`
  - `07 morning.tg` -> `07 Morning.tg`
  - `08 bad competition entry.tg` -> `08 Bad Competition Entry.tg`
  - `09 dancing song.tg` -> `09 Dancing Song.tg`
  - `10 summer 2006 (attitude).tg` -> `10 Summer 2006 (Attitude).tg`
  - `11 bye bye baby.tg` -> `11 Bye Bye Baby.tg`
  - `12 worried about a girl.tg` -> `12 Worried About a Girl.tg`
  - `13 withdrawal symptoms.tg` -> `13 Withdrawal Symptoms.tg`
  - `14 trashcan.tg` -> `14 Trashcan.tg`
  - `15 comforting friend.tg` -> `15 Comforting Friend.tg`
  - `16 california tourist.tg` -> `16 California Tourist.tg`
  - `17 breakup song.tg` -> `17 Breakup Song.tg`
  - `18 waltz in the wilderness.tg` -> `18 Waltz in the Wilderness.tg`
- 05 Junior (tentative — confirm against lyrics and notes)
  - `01 heartbeat.tg` -> `01 Heartbeat.tg`
  - `02 it could be so.tg` -> `02 It Could Be So.tg`
  - `03 I lose on purpose.tg` -> `03 I Lose on Purpose.tg`
  - `04 timberland.tg` -> `04 Timberland.tg`
  - `xx autumn tone - d.tg` -> `Autumn Tone - d.tg` (drop `xx` if desired)
  - `xx black and white.tg` -> `Black and White.tg`
  - `xx feels like home - c.tg` -> `Feels Like Home - c.tg`
  - `xx nightmare - c.tg` -> `Nightmare - c.tg`
