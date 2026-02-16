# API Reference

This document describes the internal API of the Rhyming Score Calculator.

## Modules

### models.py

Contains the data models for the application:

#### RhymingType
Enum for different types of rhymes:
- `END_RHYME`
- `INLINE_RHYME`
- `BEGIN_RHYME`

#### AlternativeLine
Represents an alternative version of a line:
- `id`: Unique identifier
- `text`: The alternative text
- `rhyming_score`: Score for this alternative
- `is_final`: Whether this is the selected final version

#### RhymingGroup
Defines a group of lines that should rhyme together:
- `group_id`: Unique identifier for the group
- `group_type`: Type of rhyme (from RhymingType)
- `member_lines`: List of lines that belong to this group

#### Line
Represents a single line in a song part:
- `original_text`: The source line
- `alternatives`: List of AlternativeLine objects
- `position`: Position in the part
- `rhyming_groups`: List of group IDs this line belongs to

#### SongPart
Represents a part of a song (verse, chorus, etc.):
- `name`: Name of the part
- `lines`: List of Line objects
- `attributes`: Dictionary of attributes (style, gender, etc.)

#### Combination
Represents a combination of selected alternatives:
- `combination_id`: Unique identifier
- `line_selections`: Mapping of line references to selected alternative IDs
- `total_rhyming_score`: Overall score for this combination
- `is_final`: Whether this is a final combination

#### Song
Represents a complete song:
- `title`: Song title
- `parts`: List of SongPart objects

#### Project
Main project class:
- `metadata`: Project metadata
- `song`: The Song object
- `rhyming_groups`: List of RhymingGroup objects
- `combinations`: List of Combination objects
- `active_combination_id`: ID of the active combination

### engine.py

Contains the RhymingEngine class for calculating rhyming scores:

#### RhymingEngine
Main class for calculating different types of rhymes:

- `calculate_end_rhyme_score(text1, text2)`: Calculate end rhyme score
- `calculate_begin_rhyme_score(text1, text2)`: Calculate begin rhyme score
- `calculate_inline_rhyme_score(text1, text2)`: Calculate inline rhyme score
- `calculate_total_score(text1, text2, rhyme_types)`: Calculate combined score
- `calculate_alternative_scores(original_text, alternatives, rhyme_types)`: Calculate scores for alternatives

### manager.py

Contains the ProjectManager class for handling project files:

#### ProjectManager
Class for managing project files:

- `save_project(project, filepath)`: Save a project to JSON
- `load_project(filepath)`: Load a project from JSON
- `create_new_project(title, **metadata)`: Create a new project
- `add_part_to_song(project, part_name, attributes)`: Add a part to the song
- `add_line_to_part(project, part_name, original_text)`: Add a line to a part
- `add_alternative_to_line(project, part_name, line_index, text)`: Add an alternative to a line
- `define_rhyming_group(project, group_id, group_type, member_lines)`: Define a rhyming group

### cli.py

Contains the command-line interface using Click.