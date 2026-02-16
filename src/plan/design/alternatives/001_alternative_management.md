# Alternative Lines Management

## Objective
Manage multiple alternative versions of each original line while preserving the original source.

## Core Concepts
- Each original line has one or more alternative versions
- Original line is preserved as reference
- Each alternative has its own rhyming score
- Alternatives can be marked as final selection

## Data Structure
- Original line stored as reference
- List of alternative lines with scores
- Mechanism to track which alternative is selected as final

## Operations
1. Add new alternative to a line
2. Remove an alternative
3. Update alternative text or score
4. Mark an alternative as final
5. Calculate/update scores for alternatives

## Storage Considerations
- Efficient storage in JSON format
- Maintain relationship between original and alternatives
- Track scoring history if needed