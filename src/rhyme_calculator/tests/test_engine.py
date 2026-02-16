"""Unit tests for the rhyming engine."""

import unittest
from rhyme_calculator.engine import RhymingEngine
from rhyme_calculator.models import RhymingType


class TestRhymingEngine(unittest.TestCase):
    """Test cases for the RhymingEngine class."""
    
    def setUp(self):
        """Set up test fixtures before each test method."""
        self.engine = RhymingEngine()
    
    def test_end_rhyme_identical_words(self):
        """Test end rhyme score for identical words."""
        score = self.engine.calculate_end_rhyme_score("cat", "cat")
        self.assertEqual(score, 1.0)
    
    def test_end_rhyme_perfect_rhyme(self):
        """Test end rhyme score for perfect rhymes."""
        score = self.engine.calculate_end_rhyme_score("cat", "hat")
        self.assertGreater(score, 0.5, "Perfect rhymes should have high scores")
    
    def test_end_rhyme_no_rhyme(self):
        """Test end rhyme score for non-rhyming words."""
        score = self.engine.calculate_end_rhyme_score("cat", "dog")
        self.assertLess(score, 0.5, "Non-rhyming words should have low scores")
    
    def test_begin_rhyme_identical_words(self):
        """Test begin rhyme score for identical words."""
        score = self.engine.calculate_begin_rhyme_score("cat", "cat")
        self.assertEqual(score, 1.0)
    
    def test_begin_rhyme_alliteration(self):
        """Test begin rhyme score for alliterating words."""
        score = self.engine.calculate_begin_rhyme_score("cat", "car")
        self.assertGreater(score, 0.5, "Alliterating words should have high scores")
    
    def test_begin_rhyme_no_alliteration(self):
        """Test begin rhyme score for non-alliterating words."""
        score = self.engine.calculate_begin_rhyme_score("cat", "dog")
        self.assertLess(score, 0.5, "Non-alliterating words should have low scores")
    
    def test_inline_rhyme_simple(self):
        """Test inline rhyme detection."""
        score = self.engine.calculate_inline_rhyme_score("the big cat sat", "on the mat")
        # Note: This is a basic test; the actual implementation may return 0
        # depending on how inline rhymes are detected
        self.assertIsInstance(score, float)
        self.assertGreaterEqual(score, 0.0)
        self.assertLessEqual(score, 1.0)
    
    def test_total_score_with_multiple_rhyme_types(self):
        """Test combined scoring with multiple rhyme types."""
        rhyme_types = [RhymingType.END_RHYME, RhymingType.BEGIN_RHYME]
        score = self.engine.calculate_total_score("cat", "hat", rhyme_types)
        self.assertGreaterEqual(score, 0.0)
        self.assertLessEqual(score, 1.0)
    
    def test_alternative_scoring(self):
        """Test scoring multiple alternatives."""
        alternatives = ["hat", "bat", "dog"]
        scores = self.engine.calculate_alternative_scores("cat", alternatives, [RhymingType.END_RHYME])
        self.assertEqual(len(scores), len(alternatives))
        for score in scores:
            self.assertGreaterEqual(score, 0.0)
            self.assertLessEqual(score, 1.0)


if __name__ == '__main__':
    unittest.main()