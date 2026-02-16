# End Rhyme Algorithm

## Objective
Detect and score rhyming patterns at the end of lines based on phonetic similarity.

## Process
1. Tokenize each line into words
2. Identify the last word of each line (or last stressed word)
3. Convert words to phonetic representation using ARPABET
4. Extract the ending sound pattern (from last stressed vowel to end)
5. Compare phonetic patterns between lines
6. Calculate similarity score

## Implementation Steps
1. Use `pronouncing.phones_for_word()` to get phonetic transcription
2. Parse ARPABET string to identify stressed vowels (marked with 1 or 2)
3. Extract the portion from the last stressed vowel to the end
4. Compare extracted patterns using:
   - Exact match (highest score)
   - Similar ending sounds (medium score)
   - Partial similarity (lower score)
   - No similarity (zero score)

## Scoring Formula
- Exact phonetic match: 1.0
- Same ending consonant + similar vowel: 0.7
- Similar ending sounds: 0.5
- Partial similarity: 0.2
- No similarity: 0.0

## Edge Cases
- Handle punctuation removal
- Deal with multi-syllable rhymes
- Account for imperfect rhymes
- Handle lines with no clear ending word