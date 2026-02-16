# Combination Scoring Methodology

## Objective
Calculate overall rhyming scores for each possible combination of alternative lines.

## Approach
1. Calculate individual line scores based on selected alternatives
2. Evaluate rhyming group compliance for selected alternatives
3. Combine individual scores with rhyming group scores
4. Apply weighting factors for different rhyme types
5. Generate total score for the combination

## Scoring Factors
- Individual alternative scores (pre-calculated)
- Rhyming group adherence (how well selected alternatives satisfy rhyme groups)
- Penalty for inconsistent selections
- Bonus for satisfying multiple rhyme types

## Implementation Strategy
- Pre-calculate scores for individual alternatives
- Calculate cross-line rhyming scores when evaluating combinations
- Cache scores to avoid recalculation
- Allow for different scoring models based on song style

## Performance Considerations
- For large projects, may need to implement approximate scoring
- Possibly use heuristics to eliminate low-scoring combinations early
- Implement scoring in batches to handle large projects