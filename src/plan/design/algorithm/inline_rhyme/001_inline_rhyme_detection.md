# Inline Rhyme Algorithm

## Objective
Detect and score rhyming patterns within lines, not just at the beginning or end.

## Process
1. Tokenize each line into individual words
2. Identify potential rhyming candidates within the line
3. Convert words to phonetic representation
4. Compare phonetic patterns of internal words
5. Calculate similarity score based on internal patterns

## Implementation Steps
1. Split line into individual words
2. For each word, get phonetic transcription using `pronouncing.phones_for_word()`
3. Identify words that could form internal rhymes
4. Compare phonetic patterns using the same methods as end-rhyme detection
5. Score based on number and quality of internal rhymes

## Scoring Formula
- Multiple strong internal rhymes: 0.8-1.0
- One strong internal rhyme: 0.6-0.7
- Multiple weak internal rhymes: 0.4-0.5
- One weak internal rhyme: 0.2-0.3
- No internal rhymes: 0.0

## Challenges
- Determining which words to compare within a line
- Handling different line lengths
- Identifying meaningful internal rhyme positions
- Distinguishing between accidental and intentional internal rhymes