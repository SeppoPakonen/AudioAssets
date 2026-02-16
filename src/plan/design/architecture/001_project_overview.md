# Project Overview: Rhyming Score Calculator

## Purpose
Create a Python program that calculates rhyming scores for English lyrics, manages alternative lines, and supports song parts with customizable rhyming groups.

## Core Features
1. Calculate rhyming scores for English lyrics
2. Manage alternative lines for each original line
3. Support song parts (verse, chorus, etc.) with style/gender attributes
4. Define custom rhyming groups (end rhyme, inline rhyme, begin rhyme)
5. Track multiple translation combinations and their scores
6. Mark combinations as final
7. Export text versions of selected combinations

## Architecture Components
- Core engine for rhyming calculation
- Data models for lyrics structure
- Project manager for JSON serialization
- CLI interface for user interaction
- Alternative lines manager
- Combination tracker

## Technology Stack
- Python 3.8+
- External libraries for rhyming detection (to be determined)
- JSON for project persistence
- Standard library for CLI interface