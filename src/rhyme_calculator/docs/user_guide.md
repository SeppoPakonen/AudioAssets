# Rhyming Score Calculator

A Python program that calculates rhyming scores for English lyrics, manages alternative lines, and supports song parts with customizable rhyming groups.

## Features

- Calculate rhyming scores for English lyrics using phonetic analysis
- Manage multiple alternative lines for each original line
- Support for song parts (verse, chorus, bridge, etc.) with style/gender attributes
- Define custom rhyming groups (end rhyme, inline rhyme, begin rhyme)
- Track multiple translation combinations and their scores
- Mark combinations as final
- Export text versions of selected combinations

## Installation

```bash
pip install rhyme-calculator
```

## Usage

### Creating a New Project

```bash
rhyme-calc create-project my_song.json --title "My Song"
```

### Adding Parts to Your Song

```bash
rhyme-calc add-part my_song.json --name "Verse 1" --style singing --gender male
rhyme-calc add-part my_song.json --name "Chorus" --style singing --gender female
```

### Adding Lines

```bash
rhyme-calc add-line my_song.json --part "Verse 1" --text "Walking down the sunny street"
rhyme-calc add-line my_song.json --part "Verse 1" --text "Feeling so refreshingly neat"
```

### Adding Alternative Lines

```bash
rhyme-calc add-alternative my_song.json --part "Verse 1" --line-index 0 --text "Strolling on the bright highway"
```

### Defining Rhyming Groups

```bash
rhyme-calc define-rhyme-group my_song.json --group-id verse1_rhyme --type end --lines "Verse 1:0,Verse 1:1"
```

### Calculating Scores

```bash
rhyme-calc calculate-scores my_song.json
```

### Viewing the Project

```bash
rhyme-calc view-project my_song.json
```

### Exporting the Final Version

```bash
rhyme-calc export-text my_song.json --output final_lyrics.txt
```

## Project Structure

The project file is stored in JSON format with the following structure:

- `metadata`: Information about the project (title, version, etc.)
- `song`: Contains the song structure with parts and lines
- `rhyming_groups`: Definitions of which lines should rhyme together
- `combinations`: Different possible combinations of alternatives
- `active_combination_id`: The currently selected combination

## Supported Rhyme Types

- **End Rhyme**: Rhymes at the end of lines
- **Begin Rhyme (Alliteration)**: Similar beginning sounds
- **Inline Rhyme**: Rhymes within the line

## Song Part Attributes

You can specify attributes for song parts such as:
- `style`: singing, rapping, etc.
- `gender`: male, female, etc.