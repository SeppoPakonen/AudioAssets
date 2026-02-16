#!/usr/bin/env python3
"""
Basic test script to verify the rhyme calculator components work together.
"""

from rhyme_calculator.models import *
from rhyme_calculator.engine import RhymingEngine
from rhyme_calculator.manager import ProjectManager
from rhyme_calculator.cli import cli
import tempfile
import os


def test_basic_functionality():
    """Test basic functionality of the rhyme calculator."""
    print("Testing basic functionality...")
    
    # Test rhyming engine
    engine = RhymingEngine()
    score = engine.calculate_end_rhyme_score("cat", "hat")
    print(f"Rhyme score for 'cat' and 'hat': {score}")
    
    # Test project creation
    manager = ProjectManager()
    project = manager.create_new_project("Test Song", description="A test song")
    
    # Add a part
    manager.add_part_to_song(project, "Verse 1", {"style": "singing", "gender": "male"})
    
    # Add a line
    manager.add_line_to_part(project, "Verse 1", "This is the original line")
    
    # Add an alternative
    manager.add_alternative_to_line(project, "Verse 1", 0, "This is an alternative line")
    
    # Test saving/loading
    with tempfile.NamedTemporaryFile(mode='w', suffix='.json', delete=False) as tmp:
        manager.save_project(project, tmp.name)
        
        # Load the project back
        loaded_project = manager.load_project(tmp.name)
        print(f"Loaded project: {loaded_project.metadata['title']}")
        print(f"Parts: {len(loaded_project.song.parts)}")
        
        # Clean up
        os.unlink(tmp.name)
    
    print("Basic functionality test completed!")


if __name__ == "__main__":
    test_basic_functionality()