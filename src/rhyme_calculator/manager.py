"""
Project manager for loading, saving, and managing rhyme calculator projects.
Handles JSON serialization and deserialization of project data.
"""

import json
from typing import Dict, Any, List
from pathlib import Path

# Handle both direct execution and module import
try:
    from .models import Project, Song, SongPart, Line, AlternativeLine, RhymingGroup, Combination, RhymingType
except ImportError:
    # When running as a script directly
    from models import Project, Song, SongPart, Line, AlternativeLine, RhymingGroup, Combination, RhymingType


class ProjectManager:
    """Manages loading, saving, and manipulation of project files."""
    
    def __init__(self):
        """Initialize the project manager."""
        pass
    
    def save_project(self, project: Project, filepath: str) -> None:
        """
        Save a project to a JSON file.
        
        Args:
            project: The project to save
            filepath: Path to the file to save to
        """
        data = self._project_to_dict(project)
        with open(filepath, 'w', encoding='utf-8') as f:
            json.dump(data, f, indent=2, ensure_ascii=False)
    
    def load_project(self, filepath: str) -> Project:
        """
        Load a project from a JSON file.
        
        Args:
            filepath: Path to the file to load from
            
        Returns:
            Loaded project object
        """
        with open(filepath, 'r', encoding='utf-8') as f:
            data = json.load(f)
        return self._dict_to_project(data)
    
    def create_new_project(self, title: str, **metadata) -> Project:
        """
        Create a new project with the given title and metadata.
        
        Args:
            title: Title of the project
            **metadata: Additional metadata for the project
            
        Returns:
            New project object
        """
        project_metadata = {
            "title": title,
            "version": "1.0",
            **metadata
        }
        
        # Initialize with empty song
        song = Song(title=title, parts=[])
        
        return Project(
            metadata=project_metadata,
            song=song,
            rhyming_groups=[],
            combinations=[],
            active_combination_id=None
        )
    
    def set_active_alternative(self, project: Project, part_name: str, line_index: int, alt_index: int) -> None:
        """
        Set a specific alternative as the 'active' (final) one for a line.
        Unsets is_final for all other alternatives in that line.
        """
        target_part = next((p for p in project.song.parts if p.name == part_name), None)
        if not target_part:
            raise ValueError(f"Part '{part_name}' not found")
        if line_index >= len(target_part.lines):
            raise ValueError("Line index out of range")
        
        line = target_part.lines[line_index]
        if alt_index >= len(line.alternatives):
            raise ValueError("Alternative index out of range")
            
        for i, alt in enumerate(line.alternatives):
            alt.is_final = (i == alt_index)

    def add_combination(self, project: Project, combination: Combination) -> None:
        """Add a combination to the project."""
        project.combinations.append(combination)

    def get_active_text(self, project: Project, part_name: str, line_index: int) -> str:
        """
        Get the text of the active (final) alternative for a line.
        If none marked final, returns the latest alternative.
        """
        target_part = next((p for p in project.song.parts if p.name == part_name), None)
        if not target_part or line_index >= len(target_part.lines):
            return ""
        
        line = target_part.lines[line_index]
        # Look for is_final
        for alt in line.alternatives:
            if alt.is_final:
                return alt.text
        
        # Fallback to latest
        return line.alternatives[-1].text if line.alternatives else line.original_text

    def _project_to_dict(self, project: Project) -> Dict[str, Any]:
        """Convert a Project object to a dictionary."""
        return {
            "id": project.id,
            "metadata": project.metadata,
            "song": self._song_to_dict(project.song),
            "rhyming_groups": [self._rhyming_group_to_dict(rg) for rg in project.rhyming_groups],
            "combinations": [self._combination_to_dict(combo) for combo in project.combinations],
            "active_combination_id": project.active_combination_id
        }
    
    def _song_to_dict(self, song: Song) -> Dict[str, Any]:
        """Convert a Song object to a dictionary."""
        return {
            "id": song.id,
            "title": song.title,
            "parts": [self._part_to_dict(part) for part in song.parts]
        }
    
    def _part_to_dict(self, part: SongPart) -> Dict[str, Any]:
        """Convert a SongPart object to a dictionary."""
        return {
            "id": part.id,
            "name": part.name,
            "attributes": part.attributes,
            "lines": [self._line_to_dict(line) for line in part.lines]
        }
    
    def _line_to_dict(self, line: Line) -> Dict[str, Any]:
        """Convert a Line object to a dictionary."""
        result = {
            "id": line.id,
            "original_text": line.original_text,
            "position": line.position,
            "rhyming_groups": line.rhyming_groups,
            "alternatives": [self._alternative_to_dict(alt) for alt in line.alternatives]
        }
        # Only include skip_translation if it's True to keep JSON cleaner
        if line.skip_translation:
            result["skip_translation"] = True
        # Only include syllable_count if it's set to a value
        if line.syllable_count is not None:
            result["syllable_count"] = line.syllable_count
        # Only include metadata if it's not empty
        if line.metadata:
            result["metadata"] = line.metadata
        return result
    
    def _alternative_to_dict(self, alt: AlternativeLine) -> Dict[str, Any]:
        """Convert an AlternativeLine object to a dictionary."""
        result = {
            "id": alt.id,
            "text": alt.text,
            "rhyming_score": alt.rhyming_score,
            "is_final": alt.is_final
        }
        # Only include custom_scores if it's not empty
        if alt.custom_scores:
            result["custom_scores"] = alt.custom_scores
        # Only include metadata if it's not empty
        if alt.metadata:
            result["metadata"] = alt.metadata
        return result
    
    def _rhyming_group_to_dict(self, rg: RhymingGroup) -> Dict[str, Any]:
        """Convert a RhymingGroup object to a dictionary."""
        result = {
            "group_id": rg.group_id,
            "group_type": rg.group_type.value,
            "member_lines": rg.member_lines
        }
        # Only include metadata if it's not empty
        if rg.metadata:
            result["metadata"] = rg.metadata
        return result
    
    def _combination_to_dict(self, combo: Combination) -> Dict[str, Any]:
        """Convert a Combination object to a dictionary."""
        result = {
            "combination_id": combo.combination_id,
            "line_selections": combo.line_selections,
            "total_rhyming_score": combo.total_rhyming_score,
            "is_final": combo.is_final
        }
        # Only include custom_scores if it's not empty
        if combo.custom_scores:
            result["custom_scores"] = combo.custom_scores
        # Only include metadata if it's not empty
        if combo.metadata:
            result["metadata"] = combo.metadata
        return result
    
    def _dict_to_project(self, data: Dict[str, Any]) -> Project:
        """Convert a dictionary to a Project object."""
        return Project(
            id=data.get("id", ""),
            metadata=data["metadata"],
            song=self._dict_to_song(data["song"]),
            rhyming_groups=[self._dict_to_rhyming_group(rg) for rg in data.get("rhyming_groups", [])],
            combinations=[self._dict_to_combination(combo) for combo in data.get("combinations", [])],
            active_combination_id=data.get("active_combination_id")
        )
    
    def _dict_to_song(self, data: Dict[str, Any]) -> Song:
        """Convert a dictionary to a Song object."""
        return Song(
            id=data.get("id", ""),
            title=data["title"],
            parts=[self._dict_to_part(part) for part in data["parts"]]
        )
    
    def _dict_to_part(self, data: Dict[str, Any]) -> SongPart:
        """Convert a dictionary to a SongPart object."""
        return SongPart(
            id=data.get("id", ""),
            name=data["name"],
            attributes=data.get("attributes", {}),
            lines=[self._dict_to_line(line) for line in data["lines"]]
        )
    
    def _dict_to_line(self, data: Dict[str, Any]) -> Line:
        """Convert a dictionary to a Line object."""
        return Line(
            id=data.get("id", ""),
            original_text=data["original_text"],
            position=data["position"],
            rhyming_groups=data.get("rhyming_groups", []),
            skip_translation=data.get("skip_translation", False),
            syllable_count=data.get("syllable_count"),
            metadata=data.get("metadata", {}),
            alternatives=[self._dict_to_alternative(alt) for alt in data["alternatives"]]
        )
    
    def _dict_to_alternative(self, data: Dict[str, Any]) -> AlternativeLine:
        """Convert a dictionary to an AlternativeLine object."""
        return AlternativeLine(
            id=data["id"],
            text=data["text"],
            rhyming_score=data["rhyming_score"],
            is_final=data["is_final"],
            custom_scores=data.get("custom_scores", {}),
            metadata=data.get("metadata", {})
        )
    
    def _dict_to_rhyming_group(self, data: Dict[str, Any]) -> RhymingGroup:
        """Convert a dictionary to a RhymingGroup object."""
        return RhymingGroup(
            group_id=data["group_id"],
            group_type=RhymingType(data["group_type"]),
            member_lines=data["member_lines"],
            metadata=data.get("metadata", {})
        )
    
    def _dict_to_combination(self, data: Dict[str, Any]) -> Combination:
        """Convert a dictionary to a Combination object."""
        return Combination(
            combination_id=data["combination_id"],
            line_selections=data["line_selections"],
            total_rhyming_score=data["total_rhyming_score"],
            custom_scores=data.get("custom_scores", {}),
            metadata=data.get("metadata", {}),
            is_final=data["is_final"]
        )
    
    def add_part_to_song(self, project: Project, part_name: str, attributes: Dict[str, str] = None) -> None:
        """
        Add a new part to the song in the project.
        
        Args:
            project: The project containing the song
            part_name: Name of the part to add (e.g., "Verse 1", "Chorus")
            attributes: Optional attributes for the part (style, gender, etc.)
        """
        if attributes is None:
            attributes = {}
        
        new_part = SongPart(
            name=part_name,
            attributes=attributes,
            lines=[]
        )
        
        project.song.parts.append(new_part)
    
    def add_line_to_part(self, project: Project, part_name: str, original_text: str, skip_translation: bool = False, syllable_count: int = None, metadata: Dict[str, str] = None) -> None:
        """
        Add a new line to a specific part in the project.
        
        Args:
            project: The project containing the song
            part_name: Name of the part to add the line to
            original_text: The original line text
            skip_translation: Whether to skip translation for this line
            syllable_count: Number of syllables in the line (optional)
            metadata: Metadata key-value pairs for the line (optional)
        """
        if metadata is None:
            metadata = {}
        
        # Find the part
        target_part = None
        for part in project.song.parts:
            if part.name == part_name:
                target_part = part
                break
        
        if target_part is None:
            raise ValueError(f"Part '{part_name}' not found in project")
        
        # Create a new line with the original text as the first alternative
        new_line = Line(
            original_text=original_text,
            position=len(target_part.lines),
            skip_translation=skip_translation,
            syllable_count=syllable_count,
            metadata=metadata,
            alternatives=[
                AlternativeLine(
                    id="",  # Will be auto-generated
                    text=original_text,
                    rhyming_score=0.0,
                    is_final=True
                )
            ]
        )
        
        target_part.lines.append(new_line)
    
    def add_alternative_to_line(self, project: Project, part_name: str, line_index: int, text: str) -> None:
        """
        Add an alternative version to a specific line in the project.
        
        Args:
            project: The project containing the song
            part_name: Name of the part containing the line
            line_index: Index of the line in the part
            text: The alternative text
        """
        # Find the part
        target_part = None
        for part in project.song.parts:
            if part.name == part_name:
                target_part = part
                break
        
        if target_part is None:
            raise ValueError(f"Part '{part_name}' not found in project")
        
        if line_index >= len(target_part.lines) or line_index < 0:
            raise ValueError(f"Line index {line_index} out of range for part '{part_name}'")
        
        # Add the alternative to the line
        new_alternative = AlternativeLine(
            id="",  # Will be auto-generated
            text=text,
            rhyming_score=0.0,  # Will be calculated later
            is_final=False
        )
        
        target_part.lines[line_index].alternatives.append(new_alternative)
    
    def define_rhyming_group(self, project: Project, group_id: str, group_type: RhymingType, member_lines: List[Dict[str, int]], metadata: Dict[str, str] = None) -> None:
        """
        Define a rhyming group that specifies which lines should rhyme together.
        
        Args:
            project: The project to add the rhyming group to
            group_id: Unique identifier for the rhyming group
            group_type: Type of rhyme (end, begin, inline)
            member_lines: List of dictionaries with "part_name" and "line_position" keys
            metadata: Metadata key-value pairs for the rhyming group (optional)
        """
        if metadata is None:
            metadata = {}
        
        new_group = RhymingGroup(
            group_id=group_id,
            group_type=group_type,
            member_lines=member_lines,
            metadata=metadata
        )
        
        project.rhyming_groups.append(new_group)
        
        # Add the group_id to the rhyming_groups list of each participating line
        for member in member_lines:
            part_name = member["part_name"]
            line_position = member["line_position"]
            
            # Find the part
            target_part = None
            for part in project.song.parts:
                if part.name == part_name:
                    target_part = part
                    break
            
            if target_part is None:
                continue  # Skip if part doesn't exist
            
            if line_position >= len(target_part.lines) or line_position < 0:
                continue  # Skip if line doesn't exist
            
            # Add the group_id to the line's rhyming_groups if not already present
            if group_id not in target_part.lines[line_position].rhyming_groups:
                target_part.lines[line_position].rhyming_groups.append(group_id)