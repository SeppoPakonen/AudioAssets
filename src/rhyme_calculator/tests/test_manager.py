"""Unit tests for the project manager."""

import unittest
import tempfile
import os
from rhyme_calculator.models import *
from rhyme_calculator.manager import ProjectManager


class TestProjectManager(unittest.TestCase):
    """Test cases for the ProjectManager class."""
    
    def setUp(self):
        """Set up test fixtures before each test method."""
        self.manager = ProjectManager()
        self.temp_dir = tempfile.mkdtemp()
    
    def tearDown(self):
        """Clean up after each test method."""
        # Clean up any temporary files created during tests
        for file in os.listdir(self.temp_dir):
            os.remove(os.path.join(self.temp_dir, file))
        os.rmdir(self.temp_dir)
    
    def test_create_new_project(self):
        """Test creating a new project."""
        project = self.manager.create_new_project("Test Song", description="A test")
        self.assertEqual(project.metadata["title"], "Test Song")
        self.assertEqual(project.metadata["description"], "A test")
        self.assertEqual(project.song.title, "Test Song")
        self.assertIn("created", project.metadata)
        self.assertIn("version", project.metadata)
    
    def test_save_and_load_project(self):
        """Test saving and loading a project."""
        original_project = self.manager.create_new_project("Test Song")
        
        # Add a part to the project
        self.manager.add_part_to_song(original_project, "Verse 1", {"style": "singing"})
        
        # Add a line to the part
        self.manager.add_line_to_part(original_project, "Verse 1", "This is a test line")
        
        # Save the project
        temp_file = os.path.join(self.temp_dir, "test_project.json")
        self.manager.save_project(original_project, temp_file)
        
        # Load the project back
        loaded_project = self.manager.load_project(temp_file)
        
        # Verify the loaded project matches the original
        self.assertEqual(loaded_project.metadata["title"], original_project.metadata["title"])
        self.assertEqual(len(loaded_project.song.parts), len(original_project.song.parts))
        self.assertEqual(loaded_project.song.parts[0].name, original_project.song.parts[0].name)
        self.assertEqual(len(loaded_project.song.parts[0].lines), len(original_project.song.parts[0].lines))
        self.assertEqual(loaded_project.song.parts[0].lines[0].original_text, 
                         original_project.song.parts[0].lines[0].original_text)
    
    def test_add_part_to_song(self):
        """Test adding a part to a song."""
        project = self.manager.create_new_project("Test Song")
        
        self.manager.add_part_to_song(project, "Chorus", {"style": "singing", "gender": "female"})
        
        self.assertEqual(len(project.song.parts), 1)
        self.assertEqual(project.song.parts[0].name, "Chorus")
        self.assertEqual(project.song.parts[0].attributes["style"], "singing")
        self.assertEqual(project.song.parts[0].attributes["gender"], "female")
    
    def test_add_line_to_part(self):
        """Test adding a line to a part."""
        project = self.manager.create_new_project("Test Song")
        self.manager.add_part_to_song(project, "Verse 1", {})
        
        self.manager.add_line_to_part(project, "Verse 1", "This is the original line")
        
        self.assertEqual(len(project.song.parts[0].lines), 1)
        self.assertEqual(project.song.parts[0].lines[0].original_text, "This is the original line")
        # Check that the original text was added as an alternative
        self.assertEqual(len(project.song.parts[0].lines[0].alternatives), 1)
        self.assertEqual(project.song.parts[0].lines[0].alternatives[0].text, "This is the original line")
    
    def test_add_alternative_to_line(self):
        """Test adding an alternative to a line."""
        project = self.manager.create_new_project("Test Song")
        self.manager.add_part_to_song(project, "Verse 1", {})
        self.manager.add_line_to_part(project, "Verse 1", "Original line")
        
        self.manager.add_alternative_to_line(project, "Verse 1", 0, "Alternative line")
        
        self.assertEqual(len(project.song.parts[0].lines[0].alternatives), 2)
        self.assertEqual(project.song.parts[0].lines[0].alternatives[1].text, "Alternative line")
    
    def test_define_rhyming_group(self):
        """Test defining a rhyming group."""
        project = self.manager.create_new_project("Test Song")
        self.manager.add_part_to_song(project, "Verse 1", {})
        self.manager.add_line_to_part(project, "Verse 1", "Line 1")
        self.manager.add_line_to_part(project, "Verse 1", "Line 2")
        
        member_lines = [
            {"part_name": "Verse 1", "line_position": 0},
            {"part_name": "Verse 1", "line_position": 1}
        ]
        
        self.manager.define_rhyming_group(
            project, 
            "test_rhyme_group", 
            RhymingType.END_RHYME, 
            member_lines
        )
        
        self.assertEqual(len(project.rhyming_groups), 1)
        self.assertEqual(project.rhyming_groups[0].group_id, "test_rhyme_group")
        self.assertEqual(project.rhyming_groups[0].group_type, RhymingType.END_RHYME)
        self.assertEqual(len(project.rhyming_groups[0].member_lines), 2)
        
        # Check that both lines now belong to the rhyming group
        self.assertIn("test_rhyme_group", project.song.parts[0].lines[0].rhyming_groups)
        self.assertIn("test_rhyme_group", project.song.parts[0].lines[1].rhyming_groups)


if __name__ == '__main__':
    unittest.main()