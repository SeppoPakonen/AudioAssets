AudioAssets — AGENTS Guide

Purpose
- This repository primarily stores music compositions, lyrics, album texts, and related project assets. A small amount of C++ (Ultimate++/U++) tooling lives under `uppsrc`, and some Bash helpers live under `scripts`.
- Agents and contributors should preserve creative intent, file layout, and naming conventions. Prefer suggestive drafts over destructive edits.

Scope
- This AGENTS.md applies to the whole repository.
- When editing code, follow the style and constraints in this document. When editing creative content, prefer non-invasive changes and drafts.

Per-Directory Guides & Tasks
- Prefer adding a small `AGENTS.md` in any directory where local context helps (album packages, key tools, etc.). Keep it short and specific to that folder’s workflow.
- Maintain `CURRENT_TASK.md` files alongside `AGENTS.md` to track active work, decisions, and next steps for that directory.
- If a directory contains a `.upp` file (U++ package), list both `AGENTS.md` and `CURRENT_TASK.md` at the top of that package’s file list in the `.upp` file so TheIDE shows them first.
- Root task tracker: see `/CURRENT_TASK.md` for the current top-level focus.

Repository Map (high level)
- `escsrc/` — Active source of songs, lyrics, and album packages. This is the main focus.
- `escsrc/Timeline/` — Aggregator TheIDE package listing years and albums; keep it up to date.
- `uppsrc/` — U++ tooling/utilities (C++). Minimal, follow U++ conventions.
- `scripts/` — Bash helpers for converting/exporting (e.g., TuxGuitar MIDI → WAV/MP3 join).
- `txtsrc/` — Older/legacy content; do not place new work here unless migrating out.
- `doc/` — Notes for human process and prompts.
- `Book/` — Human-readable running log (chapters) and compact agent notes. Keep this updated whenever meaningful work is completed.

Key Focus
- Day-to-day work centers on `escsrc/` packages and `escsrc/Timeline`.
- Latest topical work: the Prom-era package. TheIDE lists `Love is my text.md` in `escsrc/06 Prom/06 Prom.upp`. If that Markdown file is missing, consult `escsrc/06 Prom/Love is my song.txt` for the live draft and context.
  - Note: As of now `escsrc/06 Prom/Love is my song.md` is present; prefer that Markdown draft for edits and keep the `.upp` file in sync with actual filenames.
 - Active albums (work-in-progress): `04 Summer Break`, `05 Junior`, `06 Prom`.
   - 04/05: many English demo `.txt` files without Finnish. Clean and convert these into structured `.md` files modeled after `06 Prom/Love is my song.md` (sections: Inspiration, Structure, Lyrics with syllables, etc.).
   - 04/05/06: many `.tg` still carry old project names. Rename `.tg` files to match the English song titles; update `.upp` file lists accordingly without altering the year separators.
   - Create cover images for albums 04–06 and store them next to the album package (keep filenames stable for distribution).
   - Create `.md` song/album documents across albums 01–06 to standardize information structure.

Packages & TheIDE (U++)
- TheIDE treats any directory with a `.upp` file as a package. Do not rename `.upp` files casually; they are part of the project graph.
- Album packages live under `escsrc/NN Title/Title.upp` (e.g., `escsrc/01 Freshman/01 Freshman.upp`).
 - Timeline aggregator: `escsrc/Timeline/Timeline.upp` lists all year directories and albums in order. When adding a new year or album, update this `uses` list to include it in navigation.
 - For any `.upp` package, add `AGENTS.md` and `CURRENT_TASK.md` to the top of the package’s file list (if the package has a `file` section). If the package lacks a `file` section (e.g., only `uses`), you may add a minimal `file` section with those two entries.
  - Year separators in `.upp` (e.g., `2006 readonly separator`) are essential and must remain intact; compositions are anchored to their original year and honoring the original idea is a core rule.

Albums, Years, Naming
- Albums use a two-digit numeric prefix and Title Case: `01 Freshman`, `02 Sophomore`, etc.
- Year folders under `escsrc/` (e.g., `2001`, `2025`) indicate original composition year; they are referenced by Timeline.
- Inside album folders you will often find:
  - `.tg` — TuxGuitar project files (primary composition source)
  - `.txt` — Lyrics, notes, release copy in FI/EN
  - `.ecs` — Project metadata for a custom TheIDE extension; JSON-like, keep well-formed JSON
  - `ENG Archive.txt`, `FIN Archive.txt` — Archive.org release texts; maintain both where relevant
- Do not change album prefixes or year folder names without a compelling reason; many tools and references depend on them.

Languages & Releases
- Finnish (FI) is primary; English (EN, US-EN) is secondary.
- Aim to release FI first or in parallel with EN. Maintain FI/EN parity for album/archive texts and plainly note where translations are assisted by AI.
- Published bilingual albums: `01 Freshman`, `02 Sophomore`, `03 The Band` are already released in both FI and EN. Treat lyrics for these albums as final (not drafts). Prefer non‑invasive edits; propose substantial rewrites as clearly marked `-draft` variants rather than altering the published text.

AI Usage & Disclosure (Lyrics, Compositions, Video)
- Lyrics and compositions/arrangements: Must not be AI-generated. FI lyrics and `.tg` scores are human-authored only; this is a strict rule and part of the public commitment.
- Proof-of-authorship: Scores were composed with TuxGuitar prior to the ChatGPT era; do not imply they are AI work.
- Music videos and visual assets: You may use AI freely for storyboards, shot lists, prompts, and video-production planning. This freedom applies to video concepts/scripts and visuals. The AI limitation applies to lyrics and compositions/arrangements only.

AI Usage & Disclosure (Lyrics & Compositions)
- Compositions and original Finnish lyrics must not be AI-generated. All `.tg` scores and FI lyrics are human-authored only; this is a strict rule and critical PR commitment.
- Proof-of-authorship: scores were composed with TuxGuitar prior to the ChatGPT era. Do not label or imply these as AI work.
- Finnish inspiration texts are first-class creative artifacts—treat them as at least as important as the FI lyrics.
- Workflow policy:
  - Write Finnish lyrics by hand.
  - Optionally translate FI → EN with AI assistance and request opinions/phrasing; keep the English as a translation and helper context.
  - It is acceptable to turn Finnish inspiration texts into rough English lyric drafts to explore the song’s vibe; use that exploration to inform the handwritten Finnish lyrics.
- Allowed AI assistance:
  - Rhyme-book style help (rhymes, synonyms, meter) and broad brainstorming.
  - Present many diverse options, including wild-card ideas; do not narrow or steer the author.
- Prohibited AI assistance:
  - Any material contribution to Finnish lyrics or musical composition.
  - Steering the author toward particular stylistic/ideological frames (e.g., US political binaries) or low-trust societal norms; preserve the author’s intent and Finland’s high-trust perspective.
- Material change protocol:
  - If an AI suggestion would materially change lyrics, explicitly ask the author first and provide a wide, neutral option set without preference.
- AI Disclosure requirement:
  - If AI is used for any lyric-related task (including EN translation, brainstorming, or rhyme assistance), add an "AI Disclosure" section to the nearest `AGENTS.md` (album or song folder). You may include other notes, but you must include the specific lyric text(s) involved and a clear description of what AI did.
  - Keep disclosure consistent with Archive.org deliverables where AI assistance is already listed; ensure both stay in sync.
- PR note: Assume the audience is sensitive to AI usage. Emphasize that compositions and original Finnish lyrics are handmade by the author.

File Types & Conventions
- `.tg`: Authoritative composition source. TuxGuitar comments (F5) may include important notes.
- `.ecs`: JSON-like structures for a custom UI. Keep keys and arrays intact; do not reorder fields unless necessary.
- `.txt`: Lyrics and notes. Preserve line breaks and formatting—even when grammar fixing—unless explicitly asked to reflow.
- Avoid committing large binaries (WAV/FLAC/MP3). Generated media should live outside git or be uploaded to distribution targets (e.g., Archive.org). The `tmp/` folder is git-ignored and safe for intermediates.

Scripts (conversion/export)
- Location: `scripts/`
  - `make_single_mp3_part1.sh`: Gathers approved `.tg` into `tmp/all_tg`. It expects external folders `../music/Composed` and `../music/Release` (adjust as needed before use).
  - `make_single_mp3_part2.sh`: Converts `.mid` → `.wav` (Timidity) → `.mp3` (ffmpeg) and concatenates into `tmp/all.mp3`.
- Prerequisites: GNU/Linux, TuxGuitar (for batch `.tg`→`.mid`), `timidity`, `ffmpeg`.
- See `scripts/MAKE_SINGLE_MP3.md` for the step-by-step, including TuxGuitar’s batch converter. Do not commit anything under `tmp/`.
- TuxGuitar setup: enable FluidSynth output and use a GM soundfont (e.g., FluidR3 GM). See README for details.

C++ Code (U++/Ultimate++)
- Keep changes minimal and focused; this repo is not primarily code.
- Style to match existing U++ code:
  - Tabs for indentation; braces on the same line as declarations
  - Use `Upp::` types (`String`, `Vector`, `Date`, `Time`) and helpers
  - Keep functions small; avoid introducing new dependencies
  - Prefer free functions and small structs over deep inheritance
  - Follow existing naming patterns (PascalCase for functions/types; lower_with_caps for locals)
- Build/Run: Use TheIDE with the package’s `.upp`. Do not add build tooling outside the U++ ecosystem without discussion.

Archive.org Deliverables
- For each album, maintain `ENG Archive.txt` and `FIN Archive.txt` under the album folder. These are the canonical release descriptions for Archive.org.
- Include: artist names (artist and real name), album title, release date, UPC if any, ISRCs per track, IPI (e.g., 1201855096), license/usage notes, links, and disclosure about any AI assistance.
- Keep tone factual and consistent across languages; reflect the historical context (e.g., composition years and tools used then).

Licensing & Rights
- The repository uses a custom license (see `LICENSE`). Do not alter licensing text without explicit owner request.
- Respect ISRC/IPI fields and credit lines. Preserve explicit labels (e.g., `[Explicit lyrics]`) where used.

Do & Don’t
- Do:
  - Work inside `escsrc/` for new creative material and packages
  - Update `escsrc/Timeline/Timeline.upp` when adding a year/album
  - Keep `.ecs` JSON well-formed; validate before committing
  - Propose creative rewrites as new files or clearly marked drafts
  - Keep bilingual pairs in sync (FI/EN)
- Don’t:
  - Move/rename album or year folders casually
  - Commit generated audio, temporary renders, or `tmp/` outputs
  - Overwrite or mass-format lyrics without approval
  - Introduce non-U++ build systems for `uppsrc`

Common Tasks
- Add a new album
  - Create `escsrc/NN Title/` with `.upp` named `NN Title.upp`
  - Add `.tg` compositions and seed `ENG Archive.txt` / `FIN Archive.txt`
  - Update `escsrc/Timeline/Timeline.upp` to include the new album
- Add a new composition to a year
  - Place the `.tg` in the relevant `escsrc/YYYY/` folder
  - Add supportive `.txt` or `.ecs` files as needed; reference from relevant `.upp`
- Prepare a single MP3 containing many tracks
  - Follow `scripts/MAKE_SINGLE_MP3.md`; ensure dependencies are installed
  - Do not commit `tmp/` outputs

Album Cleanup (04–06 now; extend to 01–06)
- Convert legacy lyric `.txt` files to structured `.md` with clear sections (Inspiration, Structure, English/Finnish lyrics, syllable counts).
- Rename `.tg` to match official English song titles (retain year-based grouping in `.upp`).
- Add minimal `AGENTS.md` and `CURRENT_TASK.md` to album packages; list them first in `.upp`.
- Add/prepare cover images per album (keep consistent naming; avoid committing large binaries unless essential).

Notes for AI Agents
- We are at the beginning of using Codex/AI for songs. Be conservative with edits. Prefer creating `-draft` files or writing in `doc/notes/` when proposing changes.
- If a referenced file is missing (e.g., `Love is my text.md`), search the package `.upp` for context and link to the nearest source (e.g., `Love is my song.txt`). Ask before creating artifacts with canonical names inside album folders.
 - Track active work in `CURRENT_TASK.md` files. The root tracker (`/CURRENT_TASK.md`) should always reflect the primary repo focus and is referenced by this guide.

Book Logging (Process & Format)
- Purpose: Maintain a parallel narrative log for humans and a compact checklist for agents, reflecting everything done by the AI (“I”) together with the project lead (“he/him”). Refer to him using context-appropriate titles: Spearhead, Captain, Curator, Director, Chief, or Ringleader (use Ringleader only if the tone is intentionally sketchy). Use all of these over time; pick the best per situation.
- Location: `Book/`
  - Chapters: `Book/%03d - <Title>.md` — first-person narrative of actions and reasoning.
  - Compact: `Book/<Title>.md` — brief bullets (date-first), mirrors key actions from the chapter; the filename matches the chapter title without the numeric prefix.
- Updating: Whenever you finish a meaningful set of actions, add/append a chapter and its compact entry in the same change. Keep entries small and frequent.
- Tone: First-person for the AI. Refer to the user with the chosen role noun and he/him pronouns.
- Scope: Log repo-impacting actions (file additions, conversions, renames, structural changes), noteworthy decisions, and open follow-ups.
- Automation: It is acceptable for the AI to run `scripts/book_update_readme.sh` and `scripts/book_check.sh` opportunistically to keep the index in sync and verify pairing at any time.

Session Close Checklist
- Ensure the latest changes are reflected with a new Book chapter and matching compact entry.
- Regenerate the index: `scripts/book_update_readme.sh`.
- Verify pairing and README sync: `scripts/book_check.sh`.

Contact & Context
- Finnish is the author’s native language. Releases are prepared in both Finnish and English, with a tendency to publish Finnish first.
- Year folders roughly match the composer’s age and history (born 1989-12-04). Albums are numbered in release order: `01`, `02`, ...
Markdown Conversion Rules (Lyrics & Docs)
- Conversion: When converting a song `.txt` to `.md`, migrate all information (lyrics, style/notes, background, inspiration). After verifying completeness, remove the converted `.txt` from the repo.
 - Package listing: Update the album `.upp` file list to replace removed `.txt` entries with the new `.md` entries. Keep all readonly year separators intact. `AGENTS.md` and `CURRENT_TASK.md` should appear first in any `.upp` file list.
   - Important: when you convert a `.txt` to `.md`, also remove (or rename to `.md`) that exact `.txt` filename from the `.upp` file list so there are no dangling references.
 - Package listing (placement): When replacing a `.txt` with a `.md`, keep the new `.md` at the same position and under the same year separator where the `.txt` originally lived. Do not regroup converted files under a newer/current year separator.
- Sections: Use second‑level headings (`##`) for sections at minimum: `## Meta`, `## Inspiration`, `## Structure`, `## Lyrics (FI)`, `## Lyrics (EN)`, `## Background/Notes` (add `## Production/Style` when relevant).
- Lyrics fencing: Always wrap every lyrics block in fenced code blocks using triple backticks. Preserve line breaks and do not reflow unless explicitly requested.
- Background text preservation: Never modify any "background" text (context/provenance notes). Keep wording and line breaks verbatim. Only translate background texts on explicit request; when adding a translation, include it as a clearly labeled translated subsection alongside the original, not a replacement.
- Provenance markers (>>>> / <<<<): Some `.txt` files contain blocks starting with `>>>>` and `<<<<`. Carry these into `.md` unmodified and wrap them in code fences. Use simple section headers without marker suffixes:
  - `## Author` for `>>>>` blocks
  - `## AI` for `<<<<` blocks
  Keep the literal marker lines (`>>>>` / `<<<<`) inside the fenced blocks. Do not rewrite or summarize these blocks unless explicitly requested.
  - Optional consolidation: If the exchange reads naturally as a back‑and‑forth (e.g., question/answer), you may place both `>>>>` and `<<<<` blocks under a single `## Discussion` section, in chronological order, keeping each block’s literal marker lines inside the same or consecutive fenced blocks. Use this when it improves readability (e.g., 02 Sophomore/06 Thief of the Heart.md).
  - No omissions: Always copy the full contents of every `>>>>` and `<<<<` block into the `.md` (no ellipses, no truncation). You may add an informative section title above (e.g., `## Discussion`, `## Video Concept`, `## Social Post`), but the fenced content itself must be complete and verbatim.
- Separator lines (`---------`): Long lines of hyphens in `.txt` denote a separator between conceptual blocks. Treat each as its own section in `.md`. Add a concise `##` section title; if no explicit name exists, invent a clear, descriptive one (e.g., "Social Post", "Release Notes", "Day Log"). Keep the following content verbatim with original line breaks. Only translate on request.
 - Notes cue (`Muistiinpanot:`): When a `.txt` contains a `Muistiinpanot:` (notes) block, create a new `## Notes: <AI SUMMARY>` section in the `.md` with a short, descriptive summary you write. Keep the original `Muistiinpanot:` text verbatim elsewhere (inside a fenced block under Background/Notes) — do not modify it.
 - Song name cue (`Kappale "..."`): Normalize `Kappale "<name>"` to `Title: <name>` in your structured sections. You may add a brief one‑line AI heading or description under that title. Do not alter the original line inside verbatim blocks.
 - Title boundaries (`Title: ...`): Any `Title:` line in source marks a section boundary. Reflect this by splitting/labeling sections in `.md` (e.g., `## Title: <name>`). Preserve the literal `Title:` lines verbatim inside fenced blocks if present.
- Languages: Treat Finnish (FI) as canonical; clearly note if English (EN) translations are AI‑assisted.
- Archive files: Do not delete `ENG Archive.txt` or `FIN Archive.txt` when converting; they are canonical for Archive.org.
- TuxGuitar context: If background notes exist in `.tg` or were moved out, capture them under `## Background/Notes` in the corresponding `.md`.
