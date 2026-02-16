"""
Rhyming engine for calculating rhyming scores between lines.
Implements end rhyme, begin rhyme, and inline rhyme detection.
"""

import re
from typing import List, Tuple, Dict
import pronouncing

# Handle both direct execution and module import
try:
    from .models import RhymingType, AlternativeLine
except ImportError:
    # When running as a script directly
    from models import RhymingType, AlternativeLine


class RhymingEngine:
    """Engine for calculating different types of rhymes and their scores."""
    
    def __init__(self):
        """Initialize the rhyming engine."""
        pass
    
    def calculate_end_rhyme_score(self, text1: str, text2: str) -> float:
        """
        Calculate the end rhyme score between two text lines.
        
        Args:
            text1: First line of text
            text2: Second line of text
            
        Returns:
            Float between 0 and 1 representing the rhyme score
        """
        # Clean the texts by removing punctuation and converting to lowercase
        clean_text1 = re.sub(r'[^\w\s]', '', text1.lower()).strip()
        clean_text2 = re.sub(r'[^\w\s]', '', text2.lower()).strip()
        
        # Get the last word of each line
        words1 = clean_text1.split()
        words2 = clean_text2.split()
        
        if not words1 or not words2:
            return 0.0
        
        last_word1 = words1[-1]
        last_word2 = words2[-1]
        
        # Get phonetic representations
        phones1 = pronouncing.phones_for_word(last_word1)
        phones2 = pronouncing.phones_for_word(last_word2)
        
        if not phones1 or not phones2:
            return 0.0
        
        # Calculate rhyme score based on phonetic similarity
        max_score = 0.0
        for phone1 in phones1:
            for phone2 in phones2:
                score = self._compare_phonetic_endings(phone1, phone2)
                max_score = max(max_score, score)
        
        return max_score
    
    def _compare_phonetic_endings(self, phone1: str, phone2: str) -> float:
        """
        Compare the ending sounds of two phonetic representations.
        
        Args:
            phone1: Phonetic representation of first word
            phone2: Phonetic representation of second word
            
        Returns:
            Float between 0 and 1 representing the rhyme score
        """
        # Find the last stressed vowel and everything after it
        vowels = ['AA', 'AE', 'AH', 'AO', 'AW', 'AY', 'EH', 'ER', 'EY', 'IH', 'IY', 'OW', 'OY', 'UH', 'UW']
        
        # Split phones into individual phonemes
        phonemes1 = phone1.split()
        phonemes2 = phone2.split()
        
        # Find the last stressed vowel in each sequence
        last_stressed1 = self._find_last_stressed_vowel_index(phonemes1)
        last_stressed2 = self._find_last_stressed_vowel_index(phonemes2)
        
        if last_stressed1 == -1 or last_stressed2 == -1:
            # If no stressed vowel found, fall back to comparing the whole ending
            return 1.0 if phone1 == phone2 else 0.0
        
        # Extract the ending from the last stressed vowel onwards
        ending1 = ' '.join(phonemes1[last_stressed1:])
        ending2 = ' '.join(phonemes2[last_stressed2:])
        
        # Calculate similarity score
        if ending1 == ending2:
            return 1.0
        elif self._phonemes_similar(ending1, ending2):
            return 0.7
        else:
            return 0.2
    
    def _find_last_stressed_vowel_index(self, phonemes: List[str]) -> int:
        """
        Find the index of the last stressed vowel in a sequence of phonemes.
        
        Args:
            phonemes: List of phonemes
            
        Returns:
            Index of the last stressed vowel, or -1 if none found
        """
        vowels = ['AA', 'AE', 'AH', 'AO', 'AW', 'AY', 'EH', 'ER', 'EY', 'IH', 'IY', 'OW', 'OY', 'UH', 'UW']
        # Stressed vowels typically end with '1' or '2'
        stressed_vowels = [v + '1' for v in vowels] + [v + '2' for v in vowels]
        
        # Look for stressed vowels from the end
        for i in range(len(phonemes) - 1, -1, -1):
            if phonemes[i] in stressed_vowels:
                return i
        
        # If no stressed vowel found, look for any vowel
        for i in range(len(phonemes) - 1, -1, -1):
            # Remove stress markers to check if it's a vowel
            base_phoneme = phonemes[i].rstrip('012')
            if base_phoneme in vowels:
                return i
        
        return -1
    
    def _phonemes_similar(self, ending1: str, ending2: str) -> bool:
        """
        Determine if two phonetic endings are similar enough to be considered a rhyme.
        
        Args:
            ending1: First phonetic ending
            ending2: Second phonetic ending
            
        Returns:
            Boolean indicating if the endings are similar
        """
        # Simple heuristic: if the last phoneme is the same, consider it similar
        p1_list = ending1.split()
        p2_list = ending2.split()
        
        if not p1_list or not p2_list:
            return False
        
        # Check if the last phoneme is the same
        return p1_list[-1] == p2_list[-1]
    
    def calculate_begin_rhyme_score(self, text1: str, text2: str) -> float:
        """
        Calculate the begin rhyme (alliteration) score between two text lines.
        
        Args:
            text1: First line of text
            text2: Second line of text
            
        Returns:
            Float between 0 and 1 representing the alliteration score
        """
        # Clean the texts by removing punctuation and converting to lowercase
        clean_text1 = re.sub(r'[^\w\s]', '', text1.lower()).strip()
        clean_text2 = re.sub(r'[^\w\s]', '', text2.lower()).strip()
        
        # Get the first word of each line
        words1 = clean_text1.split()
        words2 = clean_text2.split()
        
        if not words1 or not words2:
            return 0.0
        
        first_word1 = words1[0]
        first_word2 = words2[0]
        
        # Get phonetic representations
        phones1 = pronouncing.phones_for_word(first_word1)
        phones2 = pronouncing.phones_for_word(first_word2)
        
        if not phones1 or not phones2:
            return 0.0
        
        # Calculate alliteration score based on initial consonant sounds
        max_score = 0.0
        for phone1 in phones1:
            for phone2 in phones2:
                score = self._compare_initial_sounds(phone1, phone2)
                max_score = max(max_score, score)
        
        return max_score
    
    def _compare_initial_sounds(self, phone1: str, phone2: str) -> float:
        """
        Compare the initial consonant sounds of two phonetic representations.
        
        Args:
            phone1: Phonetic representation of first word
            phone2: Phonetic representation of second word
            
        Returns:
            Float between 0 and 1 representing the alliteration score
        """
        phonemes1 = phone1.split()
        phonemes2 = phone2.split()
        
        if not phonemes1 or not phonemes2:
            return 0.0
        
        # Get the first phoneme (consonant or vowel)
        first_phoneme1 = phonemes1[0]
        first_phoneme2 = phonemes2[0]
        
        # Check for exact match
        if first_phoneme1 == first_phoneme2:
            return 1.0
        
        # Check for similar consonant sounds (same consonant family)
        if self._consonants_similar(first_phoneme1, first_phoneme2):
            return 0.6
        
        return 0.0
    
    def _consonants_similar(self, phoneme1: str, phoneme2: str) -> bool:
        """
        Determine if two consonant phonemes are similar.
        
        Args:
            phoneme1: First phoneme
            phoneme2: Second phoneme
            
        Returns:
            Boolean indicating if the consonants are similar
        """
        # Define consonant families
        consonant_families = [
            {'B', 'P', 'M'},  # Bilabial
            {'F', 'V'},       # Labiodental
            {'T', 'D', 'N', 'L', 'S', 'Z', 'TH', 'DH'},  # Dental/Alveolar
            {'K', 'G', 'NG'}, # Velar
            {'CH', 'JH', 'SH', 'ZH'},  # Palatal
            {'R', 'W', 'Y', 'HH'}  # Liquids/Glides
        ]
        
        # Remove stress markers for comparison
        base_phoneme1 = phoneme1.rstrip('012')
        base_phoneme2 = phoneme2.rstrip('012')
        
        # Check if both phonemes belong to the same family
        for family in consonant_families:
            if base_phoneme1 in family and base_phoneme2 in family:
                return True
        
        return False
    
    def calculate_inline_rhyme_score(self, text1: str, text2: str) -> float:
        """
        Calculate the inline rhyme score between two text lines.
        This is a simplified implementation focusing on internal word similarities.
        
        Args:
            text1: First line of text
            text2: Second line of text
            
        Returns:
            Float between 0 and 1 representing the inline rhyme score
        """
        # Clean the texts by removing punctuation and converting to lowercase
        clean_text1 = re.sub(r'[^\w\s]', '', text1.lower()).strip()
        clean_text2 = re.sub(r'[^\w\s]', '', text2.lower()).strip()
        
        words1 = clean_text1.split()
        words2 = clean_text2.split()
        
        if len(words1) < 2 or len(words2) < 2:
            return 0.0
        
        # For simplicity, we'll check if any internal words rhyme
        # This is a basic implementation - a more sophisticated one would
        # consider positional relationships and semantic meaning
        max_score = 0.0
        
        for i, word1 in enumerate(words1):
            for j, word2 in enumerate(words2):
                # Skip first and last words since we're looking for internal rhymes
                if (i == 0 or i == len(words1) - 1) and (j == 0 or j == len(words2) - 1):
                    continue
                
                phones1 = pronouncing.phones_for_word(word1)
                phones2 = pronouncing.phones_for_word(word2)
                
                if not phones1 or not phones2:
                    continue
                
                for phone1 in phones1:
                    for phone2 in phones2:
                        score = self._compare_phonetic_endings(phone1, phone2)
                        max_score = max(max_score, score)
        
        return max_score
    
    def calculate_total_score(self, text1: str, text2: str, rhyme_types: List[RhymingType]) -> float:
        """
        Calculate the combined score based on specified rhyme types.
        
        Args:
            text1: First line of text
            text2: Second line of text
            rhyme_types: List of rhyme types to consider
            
        Returns:
            Combined score based on the specified rhyme types
        """
        total_score = 0.0
        weight_sum = 0.0
        
        for rhyme_type in rhyme_types:
            if rhyme_type == RhymingType.END_RHYME:
                score = self.calculate_end_rhyme_score(text1, text2)
                total_score += score * 0.5  # Weight for end rhyme
                weight_sum += 0.5
            elif rhyme_type == RhymingType.BEGIN_RHYME:
                score = self.calculate_begin_rhyme_score(text1, text2)
                total_score += score * 0.2  # Weight for begin rhyme
                weight_sum += 0.2
            elif rhyme_type == RhymingType.INLINE_RHYME:
                score = self.calculate_inline_rhyme_score(text1, text2)
                total_score += score * 0.3  # Weight for inline rhyme
                weight_sum += 0.3
        
        # Normalize the score by the sum of weights
        if weight_sum > 0:
            return total_score / weight_sum
        else:
            return 0.0
    
    def calculate_alternative_scores(self, original_text: str, alternatives: List[str], rhyme_types: List[RhymingType]) -> List[float]:
        """
        Calculate scores for a list of alternatives compared to an original text.
        
        Args:
            original_text: The original line text
            alternatives: List of alternative texts
            rhyme_types: List of rhyme types to consider
            
        Returns:
            List of scores corresponding to each alternative
        """
        scores = []
        for alt_text in alternatives:
            score = self.calculate_total_score(original_text, alt_text, rhyme_types)
            scores.append(score)
        return scores