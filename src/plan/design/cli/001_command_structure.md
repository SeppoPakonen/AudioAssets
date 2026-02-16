# CLI Interface Plan

## Objective
Create a command-line interface that allows users to interact with the rhyming score calculator project.

## Main Commands

### 1. create-project
- Creates a new project file
- Initializes with basic structure
- Syntax: `rhyme_calc create-project <filename>`

### 2. load-project
- Loads an existing project file
- Syntax: `rhyme_calc load-project <filename>`

### 3. add-part
- Adds a new part to the song (verse, chorus, etc.)
- Syntax: `rhyme_calc add-part --name "Verse 1" --attributes "style:singing,gender:male"`

### 4. add-line
- Adds a line to a specific part
- Syntax: `rhyme_calc add-line --part "Verse 1" --text "This is the original line"`

### 5. add-alternative
- Adds an alternative version of a line
- Syntax: `rhyme_calc add-alternative --part "Verse 1" --line-index 0 --text "This is an alternative line"`

### 6. define-rhyme-group
- Defines which lines should rhyme together
- Syntax: `rhyme_calc define-rhyme-group --type end --lines "Verse 1:0,Verse 1:2"`

### 7. calculate-scores
- Calculates rhyming scores for all combinations
- Syntax: `rhyme_calc calculate-scores`

### 8. list-combinations
- Lists all possible combinations with scores
- Syntax: `rhyme_calc list-combinations`

### 9. select-combination
- Marks a combination as final
- Syntax: `rhyme_calc select-combination --id <combination_id>`

### 10. export-text
- Exports the selected combination as text
- Syntax: `rhyme_calc export-text --output <filename>`

### 11. view-project
- Displays the current project structure
- Syntax: `rhyme_calc view-project`

## Options
- `--verbose`: Show detailed output
- `--quiet`: Minimal output
- `--config`: Specify config file