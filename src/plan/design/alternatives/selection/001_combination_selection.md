# Combination Selection System

## Objective
Track and manage multiple possible combinations of alternative lines and their overall rhyming scores.

## Core Concepts
- A combination is a selection of one alternative from each line across all parts
- Each combination has an overall rhyming score
- Users can mark combinations as final
- System tracks all possible combinations (potentially large number)

## Data Structure
- Unique ID for each combination
- Mapping of line references to selected alternative IDs
- Total rhyming score for the combination
- Flag indicating if this is the final selection

## Operations
1. Generate all possible combinations (with limitations)
2. Calculate total score for each combination
3. Select a combination as final
4. Export a combination as text
5. Compare different combinations

## Optimization Considerations
- For large numbers of alternatives, may need to limit combinations
- Possibly implement top-N scoring combinations only
- Allow users to focus on high-scoring combinations