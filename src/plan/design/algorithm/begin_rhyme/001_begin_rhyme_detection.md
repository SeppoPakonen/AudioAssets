# Begin Rhyme Algorithm (Alliteration)

## Objective
Detect and score rhyming patterns at the beginning of lines based on initial consonant sounds.

## Process
1. Tokenize each line into words
2. Identify the first word of each line
3. Convert words to phonetic representation using ARPABET
4. Extract the beginning consonant sound pattern
5. Compare initial sounds between lines
6. Calculate similarity score

## Implementation Steps
1. Use `pronouncing.phones_for_word()` to get phonetic transcription
2. Parse ARPABET string to identify initial consonant clusters
3. Extract the beginning consonant sound
4. Compare extracted patterns using:
   - Exact match (highest score)
   - Similar initial consonants (medium score)
   - Partial similarity (lower score)
   - No similarity (zero score)

## Scoring Formula
- Exact consonant match: 1.0
- Same consonant cluster: 0.8
- Similar consonant sounds (e.g., 'B' and 'P'): 0.6
- Partial similarity: 0.3
- No similarity: 0.0

## Edge Cases
- Handle punctuation at the beginning
- Deal with silent letters (e.g., 'knight')
- Account for different starting sounds
- Handle lines starting with non-alphabetic characters