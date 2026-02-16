# Rhyming Score Calculator Tutorial

This tutorial will walk you through creating your first project with the Rhyming Score Calculator.

## Step 1: Create a New Project

First, create a new project file:

```bash
rhyme-calc create-project my_first_song.json --title "My First Song"
```

## Step 2: Add Song Parts

Add different parts to your song, such as verses and choruses:

```bash
rhyme-calc add-part my_first_song.json --name "Verse 1" --style singing --gender male
rhyme-calc add-part my_first_song.json --name "Chorus" --style singing --gender female
rhyme-calc add-part my_first_song.json --name "Verse 2" --style singing --gender male
```

## Step 3: Add Lines

Add the original lines to your song parts:

```bash
rhyme-calc add-line my_first_song.json --part "Verse 1" --text "Walking down the sunny street"
rhyme-calc add-line my_first_song.json --part "Verse 1" --text "Feeling so refreshingly neat"
rhyme-calc add-line my_first_song.json --part "Chorus" --text "These summer days are here"
rhyme-calc add-line my_first_song.json --part "Chorus" --text "With joy we hold so dear"
```

## Step 4: Add Alternative Lines

Add alternative versions of your lines to explore different options:

```bash
rhyme-calc add-alternative my_first_song.json --part "Verse 1" --line-index 0 --text "Strolling on the bright highway"
rhyme-calc add-alternative my_first_song.json --part "Verse 1" --line-index 0 --text "Marching through the summer heat"
rhyme-calc add-alternative my_first_song.json --part "Verse 1" --line-index 1 --text "Life feels so bittersweet"
```

## Step 5: Define Rhyming Groups

Specify which lines should rhyme together:

```bash
rhyme-calc define-rhyme-group my_first_song.json --group-id verse1_rhyme --type end --lines "Verse 1:0,Verse 1:1"
rhyme-calc define-rhyme-group my_first_song.json --group-id chorus_rhyme --type end --lines "Chorus:0,Chorus:1"
```

## Step 6: Calculate Scores

Calculate rhyming scores for all alternatives:

```bash
rhyme-calc calculate-scores my_first_song.json
```

## Step 7: View Your Project

Check the structure and scores of your project:

```bash
rhyme-calc view-project my_first_song.json
```

## Step 8: Export Your Lyrics

Once you're happy with your selections, export the final version:

```bash
rhyme-calc export-text my_first_song.json --output final_lyrics.txt
```

## Advanced Usage

### Working with Different Rhyme Types

You can define different types of rhyming groups:

- `--type end` for end rhymes (traditional rhymes at the end of lines)
- `--type begin` for begin rhymes (alliteration at the start of lines)
- `--type inline` for internal rhymes within lines

### Managing Combinations

While the current version focuses on individual line alternatives, future versions will allow you to save and compare different combinations of alternatives to find the best overall score for your song.

## Tips

1. Start with a clear structure in mind (verses, chorus, bridge, etc.)
2. Add original lines first before creating alternatives
3. Define rhyming groups after adding all relevant lines
4. Use the view-project command frequently to check your progress
5. Experiment with different alternatives to see how they affect the overall score