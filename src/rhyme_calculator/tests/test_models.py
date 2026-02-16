"""Unit tests for the data models."""

import unittest
from rhyme_calculator.models import (
    AlternativeLine, RhymingGroup, Line, SongPart, 
    Combination, Song, Project, RhymingType
)


class TestModels(unittest.TestCase):
    """Test cases for the data model classes."""
    
    def test_alternative_line_creation(self):
        """Test creating an AlternativeLine instance."""
        alt = AlternativeLine(id="test_id", text="test text", rhyming_score=0.5, is_final=True)
        self.assertEqual(alt.id, "test_id")
        self.assertEqual(alt.text, "test text")
        self.assertEqual(alt.rhyming_score, 0.5)
        self.assertTrue(alt.is_final)
    
    def test_rhyming_group_creation(self):
        """Test creating a RhymingGroup instance."""
        members = [{"part_name": "Verse 1", "line_position": 0}]
        group = RhymingGroup(group_id="test_group", group_type=RhymingType.END_RHYME, member_lines=members)
        self.assertEqual(group.group_id, "test_group")
        self.assertEqual(group.group_type, RhymingType.END_RHYME)
        self.assertEqual(group.member_lines, members)
    
    def test_line_creation(self):
        """Test creating a Line instance."""
        alternatives = [AlternativeLine(id="orig", text="original", rhyming_score=0.0, is_final=True)]
        line = Line(original_text="original", alternatives=alternatives, position=0)
        self.assertEqual(line.original_text, "original")
        self.assertEqual(len(line.alternatives), 1)  # Original is added as alternative if not present
        self.assertEqual(line.position, 0)
    
    def test_line_adds_original_as_alternative(self):
        """Test that Line automatically adds original text as an alternative if not present."""
        line = Line(original_text="original", alternatives=[], position=0)
        # The original text should be added as the first alternative
        self.assertEqual(len(line.alternatives), 1)
        self.assertEqual(line.alternatives[0].text, "original")
        self.assertTrue(line.alternatives[0].is_final)
    
    def test_song_part_creation(self):
        """Test creating a SongPart instance."""
        lines = [Line(original_text="line1", alternatives=[], position=0)]
        part = SongPart(name="Verse 1", lines=lines, attributes={"style": "singing"})
        self.assertEqual(part.name, "Verse 1")
        self.assertEqual(len(part.lines), 1)
        self.assertEqual(part.attributes["style"], "singing")
    
    def test_combination_creation(self):
        """Test creating a Combination instance."""
        selections = {"part1:0": "alt1"}
        combo = Combination(combination_id="combo1", line_selections=selections, total_rhyming_score=0.8)
        self.assertEqual(combo.combination_id, "combo1")
        self.assertEqual(combo.line_selections, selections)
        self.assertEqual(combo.total_rhyming_score, 0.8)
        self.assertFalse(combo.is_final)
    
    def test_song_creation(self):
        """Test creating a Song instance."""
        parts = [SongPart(name="Verse 1", lines=[], attributes={})]
        song = Song(title="Test Song", parts=parts)
        self.assertEqual(song.title, "Test Song")
        self.assertEqual(len(song.parts), 1)
    
    def test_project_creation(self):
        """Test creating a Project instance."""
        song = Song(title="Test Song", parts=[])
        project = Project(metadata={"title": "Test Project"}, song=song)
        self.assertEqual(project.metadata["title"], "Test Project")
        self.assertEqual(project.song.title, "Test Song")
        # Check that created timestamp was added
        self.assertIn("created", project.metadata)
        # Check that version was added
        self.assertIn("version", project.metadata)


if __name__ == '__main__':
    unittest.main()