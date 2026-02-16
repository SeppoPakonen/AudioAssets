# Rhyming Algorithm Plan

## Overview
The rhyming algorithm will calculate similarity between lines based on phonetic analysis. Different types of rhymes require different approaches.

## Required Libraries
- `pronouncing` - for phonetic transcription of English words
- `phoneme` - alternative phonetic analysis library
- `difflib` - for string similarity comparison
- Standard library for text processing

## Algorithm Types

### 1. End Rhyme Detection
- Extract the last stressed vowel sound and all subsequent sounds
- Compare phonetic endings of lines
- Use phonetic similarity scoring

### 2. Beginning Rhyme Detection (Alliteration)
- Compare initial consonant sounds of lines
- Focus on first few words of each line
- Account for common phonetic patterns

### 3. Inline Rhyme Detection
- Identify rhyming words within the line
- May involve identifying internal patterns
- More complex due to variable positions

## Scoring Methodology
- Normalize scores to 0-1 range
- Weight different rhyme types appropriately
- Consider syllable stress patterns
- Factor in semantic similarity as secondary measure