# System Architecture

## Components

### 1. Rhyming Engine
- Responsible for calculating rhyming scores
- Uses phonetic analysis libraries
- Handles different types of rhymes (end, inline, begin)
- Provides scoring algorithms

### 2. Data Manager
- Manages the data models
- Handles serialization/deserialization of project files
- Maintains relationships between objects

### 3. Project Manager
- Loads and saves project files in JSON format
- Manages multiple combinations
- Tracks final selections

### 4. CLI Interface
- Provides command-line interaction
- Allows creation, modification, and viewing of projects
- Enables selection of final combinations

### 5. Alternative Manager
- Manages alternative lines for each original line
- Calculates scores for each alternative
- Maintains original line references

## Flow Diagram
```
User Input → CLI Interface → Project Manager → Data Manager → Rhyming Engine
                                    ↓              ↓            ↓
                            JSON File ← Project Manager ← Combination Tracker
```

## Dependencies
- phonetic analysis library (e.g., pronouncing, pyphen, etc.)
- JSON parsing (standard library)
- CLI parsing (argparse or click)