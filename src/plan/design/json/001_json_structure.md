# JSON Project File Format

## Overall Structure
```json
{
  "metadata": {
    "version": "1.0",
    "title": "Song Title",
    "created": "2023-01-01T00:00:00Z",
    "modified": "2023-01-01T00:00:00Z",
    "description": "Description of the song/project"
  },
  "song": {
    "title": "Song Title",
    "parts": [
      // Array of song parts
    ]
  },
  "rhyming_groups": [
    // Array of defined rhyming groups
  ],
  "combinations": [
    // Array of possible combinations
  ],
  "active_combination_id": "id_of_selected_combination"
}
```

## Song Parts Structure
Each part contains:
- name: identifier for the part (e.g. "Verse 1", "Chorus")
- attributes: additional properties like style and gender
- lines: array of line objects

## Rhyming Groups Structure
Groups that define which lines should rhyme together, with type specification.

## Combinations Structure
Different possible combinations of alternative lines with their scores.