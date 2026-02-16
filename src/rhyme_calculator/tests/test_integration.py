"""Integration tests for the rhyme calculator."""

import unittest
import tempfile
import os
from rhyme_calculator.models import *
from rhyme_calculator.engine import RhymingEngine
from rhyme_calculator.manager import ProjectManager


class TestIntegration(unittest.TestCase):
    """Integration tests for the rhyme calculator components."""
    
    def setUp(self):
        """Set up test fixtures before each test method."""
        self.engine = RhymingEngine()
        self.manager = ProjectManager()
        self.temp_dir = tempfile.mkdtemp()
    
    def tearDown(self):
        """Clean up after each test method."""
        # Clean up any temporary files created during tests
        for file in os.listdir(self.temp_dir):
            os.remove(os.path.join(self.temp_dir, file))
        os.rmdir(self.temp_dir)
    
    def test_full_workflow(self):
        """Test the full workflow: create project, add content, calculate scores, save/load."""
        # Create a new project
        project = self.manager.create_new_project("Test Song", description="Integration test")
        
        # Add parts
        self.manager.add_part_to_song(project, "Verse 1", {"style": "singing", "gender": "male"})
        self.manager.add_part_to_song(project, "Chorus", {"style": "singing", "gender": "female"})
        
        # Add lines to Verse 1
        self.manager.add_line_to_part(project, "Verse 1", "Walking down the sunny street")
        self.manager.add_line_to_part(project, "Verse 1", "Feeling so refreshingly neat")
        
        # Add lines to Chorus
        self.manager.add_line_to_part(project, "Chorus", "These summer days are here")
        
        # Add alternatives
        self.manager.add_alternative_to_line(project, "Verse 1", 0, "Strolling on the bright highway")
        self.manager.add_alternative_to_line(project, "Verse 1", 0, "Marching through the summer heat")
        self.manager.add_alternative_to_line(project, "Verse 1", 1, "Life feels so bittersweet")
        self.manager.add_alternative_to_line(project, "Verse 1", 1, "Nothing could feel complete")
        
        # Define a rhyming group for the end rhymes in Verse 1
        member_lines = [
            {"part_name": "Verse 1", "line_position": 0},
            {"part_name": "Verse 1", "line_position": 1}
        ]
        self.manager.define_rhyming_group(
            project,
            "verse1_end_rhyme",
            RhymingType.END_RHYME,
            member_lines
        )
        
        # Calculate scores for alternatives
        for part in project.song.parts:
            for line_idx, line in enumerate(part.lines):
                original_text = line.original_text
                for alt in line.alternatives:
                    if alt.text != original_text:  # Don't recalculate for original
                        # Calculate score based on rhyming groups this line belongs to
                        rhyme_types = []
                        for rg in project.rhyming_groups:
                            if any(member['part_name'] == part.name and 
                                   member['line_position'] == line_idx 
                                   for member in rg.member_lines):
                                rhyme_types.append(rg.group_type)
                        
                        if rhyme_types:
                            alt.rhyming_score = self.engine.calculate_total_score(
                                original_text, alt.text, rhyme_types
                            )
                        else:
                            alt.rhyming_score = self.engine.calculate_total_score(
                                original_text, alt.text, [RhymingType.END_RHYME, RhymingType.BEGIN_RHYME, RhymingType.INLINE_RHYME]
                            )
        
        # Save the project
        temp_file = os.path.join(self.temp_dir, "integration_test.json")
        self.manager.save_project(project, temp_file)
        
        # Load the project back
        loaded_project = self.manager.load_project(temp_file)
        
        # Verify the loaded project has the expected structure
        self.assertEqual(loaded_project.metadata["title"], "Test Song")
        self.assertEqual(len(loaded_project.song.parts), 2)
        self.assertEqual(loaded_project.song.parts[0].name, "Verse 1")
        self.assertEqual(loaded_project.song.parts[1].name, "Chorus")
        
        # Verify lines were loaded correctly
        self.assertEqual(len(loaded_project.song.parts[0].lines), 2)
        self.assertEqual(loaded_project.song.parts[0].lines[0].original_text, "Walking down the sunny street")
        
        # Verify alternatives were loaded with scores
        self.assertGreater(len(loaded_project.song.parts[0].lines[0].alternatives), 1)
        
        # Verify rhyming groups were loaded
        self.assertEqual(len(loaded_project.rhyming_groups), 1)
        self.assertEqual(loaded_project.rhyming_groups[0].group_id, "verse1_end_rhyme")
        
        print("Full workflow test passed!")


if __name__ == '__main__':
    unittest.main()