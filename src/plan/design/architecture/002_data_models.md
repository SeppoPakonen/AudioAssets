# Data Models for Rhyming Score Calculator

## Core Classes

### Song Class
- title: str
- parts: List[SongPart]
- metadata: Dict[str, Any] (style, genre, etc.)

### SongPart Class
- name: str (e.g. "Verse 1", "Chorus", "Bridge")
- attributes: Dict[str, str] (e.g. {"style": "singing", "gender": "female"})
- lines: List[Line]

### Line Class
- original_text: str (the source line)
- alternatives: List[AlternativeLine]
- rhyming_groups: List[RhymingGroup]
- position: int (position in part)

### AlternativeLine Class
- text: str
- rhyming_score: float (score for this alternative)
- is_final: bool (whether this is the selected final version)

### RhymingGroup Class
- group_id: str
- group_type: RhymingType (END_RHYME, INLINE_RHYME, BEGIN_RHYME)
- member_lines: List[Tuple[Line, int]] (line reference and position within line)

### RhymingType Enum
- END_RHYME
- INLINE_RHYME
- BEGIN_RHYME

### Project Class
- metadata: Dict[str, Any]
- song: Song
- combinations: List[Combination]
- active_combination_id: str

### Combination Class
- combination_id: str
- line_selections: Dict[str, str] (mapping line_id to selected alternative_id)
- total_rhyming_score: float
- is_final: bool

## Relationships
- A Song contains multiple SongParts
- A SongPart contains multiple Lines
- A Line has multiple AlternativeLines
- A RhymingGroup connects multiple Lines
- A Project contains a Song and multiple Combinations
- A Combination selects specific AlternativeLines from each Line