"""
Data models for the rhyming score calculator.
Defines the core structures for songs, parts, lines, and alternatives.
"""

from dataclasses import dataclass, field
from enum import Enum
from typing import List, Dict, Optional
from uuid import uuid4


class RhymingType(Enum):
    """Enumeration of different rhyme types."""
    END_RHYME = "END_RHYME"
    INLINE_RHYME = "INLINE_RHYME"
    BEGIN_RHYME = "BEGIN_RHYME"


@dataclass
class AlternativeLine:
    """Represents an alternative version of a line."""
    id: str
    text: str
    rhyming_score: float = 0.0
    is_final: bool = False
    custom_scores: Dict[str, float] = field(default_factory=dict)  # Custom scores like joyfulness, etc.
    metadata: Dict[str, str] = field(default_factory=dict)  # Key-value pairs for notes, keywords, etc.
    
    def __post_init__(self):
        if not self.id:
            self.id = str(uuid4())


@dataclass
class RhymingGroup:
    """Defines a group of lines that should rhyme together."""
    group_id: str
    group_type: RhymingType
    member_lines: List[Dict[str, int]]  # List of {"part_name": str, "line_position": int}
    metadata: Dict[str, str] = field(default_factory=dict)  # Key-value pairs for notes, keywords, etc.
    
    def __post_init__(self):
        if not self.group_id:
            self.group_id = str(uuid4())


@dataclass
class Line:
    """Represents a single line in a song part."""
    original_text: str
    alternatives: List[AlternativeLine]
    position: int
    rhyming_groups: List[str] = field(default_factory=list)
    skip_translation: bool = False  # Flag to indicate if this line should skip translation
    syllable_count: Optional[int] = None  # Number of syllables in the line
    metadata: Dict[str, str] = field(default_factory=dict)  # Key-value pairs for notes, keywords, etc.
    id: str = ""
    
    def __post_init__(self):
        if not self.id:
            self.id = str(uuid4())
        
        # Ensure the original text is added as an alternative if not already present
        original_exists = any(alt.text == self.original_text for alt in self.alternatives)
        if not original_exists:
            self.alternatives.insert(0, AlternativeLine(
                id=str(uuid4()),
                text=self.original_text,
                rhyming_score=0.0,
                is_final=True
            ))


@dataclass
class SongPart:
    """Represents a part of a song (verse, chorus, etc.)."""
    name: str
    lines: List[Line]
    attributes: Dict[str, str] = field(default_factory=dict)
    id: str = ""
    
    def __post_init__(self):
        if not self.id:
            self.id = str(uuid4())


@dataclass
class Combination:
    """Represents a combination of selected alternatives across all lines."""
    combination_id: str
    line_selections: Dict[str, str]  # Maps "part_name:line_pos" to alternative_id
    total_rhyming_score: float
    custom_scores: Dict[str, float] = field(default_factory=dict)  # Custom scores for the combination
    metadata: Dict[str, str] = field(default_factory=dict)  # Key-value pairs for notes, keywords, etc.
    is_final: bool = False
    
    def __post_init__(self):
        if not self.combination_id:
            self.combination_id = str(uuid4())


@dataclass
class Song:
    """Represents a complete song with parts and lines."""
    title: str
    parts: List[SongPart]
    id: str = ""
    
    def __post_init__(self):
        if not self.id:
            self.id = str(uuid4())


@dataclass
class Project:
    """Main project class that holds the entire song structure."""
    metadata: Dict[str, str]
    song: Song
    rhyming_groups: List[RhymingGroup] = field(default_factory=list)
    combinations: List[Combination] = field(default_factory=list)
    active_combination_id: Optional[str] = None
    id: str = ""
    
    def __post_init__(self):
        if not self.id:
            self.id = str(uuid4())
        
        # Set creation timestamp if not already set
        if "created" not in self.metadata:
            from datetime import datetime
            self.metadata["created"] = datetime.now().isoformat()
        
        # Set version if not already set
        if "version" not in self.metadata:
            self.metadata["version"] = "1.0"