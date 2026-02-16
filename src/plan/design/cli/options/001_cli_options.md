# CLI Options and Configuration

## Global Options
- `-v, --verbose`: Enable verbose output for debugging
- `-q, --quiet`: Suppress non-critical output
- `-c, --config FILE`: Specify configuration file path
- `-h, --help`: Show help information
- `-V, --version`: Display version information

## Command-Specific Options

### Project Commands
- `--template`: Use a template for new projects
- `--validate`: Validate project structure before loading

### Part Commands
- `--name NAME`: Specify part name (e.g., "Verse 1", "Chorus")
- `--attributes ATTR`: Specify attributes like style and gender
- `--position POS`: Insert at specific position

### Line Commands
- `--part PART_NAME`: Specify which part to add line to
- `--text TEXT`: The actual line text
- `--original ORIGINAL`: Mark as original source line
- `--index INDEX`: Position within the part

### Alternative Commands
- `--part PART_NAME`: Specify which part
- `--line-index INDEX`: Which line to add alternative for
- `--text TEXT`: The alternative line text
- `--final`: Mark as final selection immediately

### Rhyme Group Commands
- `--type TYPE`: Type of rhyme (end, begin, inline)
- `--lines LINES`: Comma-separated list of line references
- `--group-id ID`: Custom identifier for the rhyme group

### Export Commands
- `--output FILE`: Output file path
- `--format FORMAT`: Output format (text, json, etc.)
- `--combination-id ID`: Specific combination to export