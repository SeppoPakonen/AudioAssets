#!/usr/bin/env python3
"""
MCP Server for the Rhyming Score Calculator
This script creates an MCP server using the low-level Server class.
"""

import sys
import warnings
import asyncio
import random
from pathlib import Path
from typing import Any, Dict, List, Optional

# Suppress warnings that might corrupt the JSON-RPC stream
warnings.filterwarnings("ignore")

# Add the current directory (src) to the path so we can import our modules
sys.path.insert(0, str(Path(__file__).parent))

from mcp.server import Server
import mcp.types as types
import mcp.server.stdio

from rhyme_calculator.manager import ProjectManager
from rhyme_calculator.models import RhymingType
from rhyme_calculator.engine import RhymingEngine


# Initialize manager and engine
manager = ProjectManager()
engine = RhymingEngine()

# Create the server instance
app = Server("rhyme-calculator")

@app.list_tools()
async def list_tools() -> list[types.Tool]:
    """List all available tools."""
    return [
        types.Tool(
            name="tutorial",
            description="Get a tutorial on how to use the Rhyme Calculator workflow",
            inputSchema={
                "type": "object",
                "properties": {},
            }
        ),
        types.Tool(
            name="create_project",
            description="Create a new rhyme calculator project",
            inputSchema={
                "type": "object",
                "properties": {
                    "filepath": {"type": "string", "description": "Path to the project file"},
                    "title": {"type": "string", "description": "Title of the project"},
                    "metadata": {"type": "object", "description": "Optional metadata"}
                },
                "required": ["filepath", "title"]
            }
        ),
        types.Tool(
            name="add_part",
            description="Add a part to the project (e.g., verse, chorus)",
            inputSchema={
                "type": "object",
                "properties": {
                    "filepath": {"type": "string", "description": "Path to the project file"},
                    "name": {"type": "string", "description": "Name of the part"},
                    "attributes": {"type": "object", "description": "Attributes for the part"}
                },
                "required": ["filepath", "name"]
            }
        ),
        types.Tool(
            name="add_line",
            description="Add a line to a part",
            inputSchema={
                "type": "object",
                "properties": {
                    "filepath": {"type": "string", "description": "Path to the project file"},
                    "part": {"type": "string", "description": "Name of the part"},
                    "text": {"type": "string", "description": "Text of the line"},
                    "skip_translation": {"type": "boolean", "description": "Whether to skip translation"},
                    "syllable_count": {"type": "integer", "description": "Number of syllables"},
                    "metadata": {"type": "object", "description": "Metadata key-value pairs"}
                },
                "required": ["filepath", "part", "text"]
            }
        ),
        types.Tool(
            name="add_alternative",
            description="Add an alternative version of a line",
            inputSchema={
                "type": "object",
                "properties": {
                    "filepath": {"type": "string", "description": "Path to the project file"},
                    "part": {"type": "string", "description": "Name of the part"},
                    "line_index": {"type": "integer", "description": "Index of the line (0-based)"},
                    "text": {"type": "string", "description": "Text of the alternative line"}
                },
                "required": ["filepath", "part", "line_index", "text"]
            }
        ),
        types.Tool(
            name="define_rhyming_group",
            description="Define a group of lines that should rhyme together",
            inputSchema={
                "type": "object",
                "properties": {
                    "filepath": {"type": "string", "description": "Path to the project file"},
                    "group_id": {"type": "string", "description": "Unique identifier for the group"},
                    "group_type": {"type": "string", "description": "Type of rhyme (END_RHYME, BEGIN_RHYME, INLINE_RHYME)"},
                    "member_lines": {
                        "type": "array",
                        "items": {
                            "type": "object",
                            "properties": {
                                "part_name": {"type": "string"},
                                "line_position": {"type": "integer"}
                            },
                            "required": ["part_name", "line_position"]
                        }
                    },
                    "metadata": {"type": "object", "description": "Metadata key-value pairs"}
                },
                "required": ["filepath", "group_id", "group_type", "member_lines"]
            }
        ),
        types.Tool(
            name="set_song_targets",
            description="Set target values for song quality metrics (e.g. roughness: 4.5)",
            inputSchema={
                "type": "object",
                "properties": {
                    "filepath": {"type": "string", "description": "Path to the project file"},
                    "targets": {"type": "object", "description": "Target score values (e.g. {'roughness': 4.5})"}
                },
                "required": ["filepath", "targets"]
            }
        ),
        types.Tool(
            name="set_alternative_custom_score",
            description="Set a custom score for a specific alternative",
            inputSchema={
                "type": "object",
                "properties": {
                    "filepath": {"type": "string", "description": "Path to the project file"},
                    "part": {"type": "string", "description": "Name of the part"},
                    "line_index": {"type": "integer", "description": "Index of the line in the part"},
                    "alt_index": {"type": "integer", "description": "Index of the alternative in the line"},
                    "score_name": {"type": "string", "description": "Name of the custom score"},
                    "score_value": {"type": "number", "description": "Value of the custom score"}
                },
                "required": ["filepath", "part", "line_index", "alt_index", "score_name", "score_value"]
            }
        ),
        types.Tool(
            name="calculate_scores",
            description="Calculate rhyming scores for all alternatives",
            inputSchema={
                "type": "object",
                "properties": {
                    "filepath": {"type": "string", "description": "Path to the project file"}
                },
                "required": ["filepath"]
            }
        ),
        types.Tool(
            name="get_part_score",
            description="Get the average rhyming score for a specific part",
            inputSchema={
                "type": "object",
                "properties": {
                    "filepath": {"type": "string", "description": "Path to the project file"},
                    "part": {"type": "string", "description": "Name of the part"}
                },
                "required": ["filepath", "part"]
            }
        ),
        types.Tool(
            name="dump_project",
            description="Get a text dump of the project or a specific part",
            inputSchema={
                "type": "object",
                "properties": {
                    "filepath": {"type": "string", "description": "Path to the project file"},
                    "part": {"type": "string", "description": "Name of the part (optional)"}
                },
                "required": ["filepath"]
            }
        ),
        types.Tool(
            name="remove_line",
            description="Remove a specific line from a part",
            inputSchema={
                "type": "object",
                "properties": {
                    "filepath": {"type": "string", "description": "Path to the project file"},
                    "part": {"type": "string", "description": "Name of the part"},
                    "line_index": {"type": "integer", "description": "Index of the line to remove"}
                },
                "required": ["filepath", "part", "line_index"]
            }
        ),
        types.Tool(
            name="get_line_context",
            description="Get context for a line, including rhyming groups and related lines",
            inputSchema={
                "type": "object",
                "properties": {
                    "filepath": {"type": "string", "description": "Path to the project file"},
                    "part": {"type": "string", "description": "Name of the part"},
                    "line_index": {"type": "integer", "description": "Index of the line"}
                },
                "required": ["filepath", "part", "line_index"]
            }
        ),
        types.Tool(
            name="evaluate_rhyme_quality",
            description="Evaluate the actual rhyming quality between lines in groups based on selected alternatives",
            inputSchema={
                "type": "object",
                "properties": {
                    "filepath": {"type": "string", "description": "Path to the project file"}
                },
                "required": ["filepath"]
            }
        ),
        types.Tool(
            name="generate_combination",
            description="Generate a combination of alternatives aiming for high scores",
            inputSchema={
                "type": "object",
                "properties": {
                    "filepath": {"type": "string", "description": "Path to the project file"},
                    "weights": {"type": "object", "description": "Weights for scores (e.g. {'rhyme': 1.0, 'roughness': 0.5})"},
                    "count": {"type": "integer", "description": "Number of iterations (default 100)"}
                },
                "required": ["filepath"]
            }
        ),
        types.Tool(
            name="get_worst_lines",
            description="Get lines with the lowest rhyming scores",
            inputSchema={
                "type": "object",
                "properties": {
                    "filepath": {"type": "string", "description": "Path to the project file"},
                    "limit": {"type": "integer", "description": "Number of lines to return (default 5)"}
                },
                "required": ["filepath"]
            }
        ),
        types.Tool(
            name="get_rhymes",
            description="Find words that rhyme with a given word. Supports end rhymes (perfect), begin rhymes (alliteration), and inline rhymes (assonance).",
            inputSchema={
                "type": "object",
                "properties": {
                    "word": {"type": "string", "description": "The word to find rhymes for"},
                    "rhyme_type": {
                        "type": "string", 
                        "description": "Type of rhyme: 'end' (default), 'begin' (alliteration), 'inline' (assonance)",
                        "enum": ["end", "begin", "inline"]
                    },
                    "limit": {"type": "integer", "description": "Maximum number of rhymes to return (default 20)"}
                },
                "required": ["word"]
            }
        )
    ]

@app.call_tool()
async def call_tool(name: str, arguments: Any) -> list[types.TextContent]:
    """Execute a tool with the given arguments."""
    try:
        if name == "tutorial":
            tutorial_text = """# Rhyme Calculator Workflow Tutorial

1. **Setup**:
   - It is expected that you have already provided the original lyrics (e.g. in Finnish).
   - Use `create_project` to start.
   - Use `add_part` and `add_line` to input original lyrics.

2. **Structure**:
   - Analyze original lyrics to set `syllable_count` for each line.
   - Define `RhymingGroup`s (e.g., AABB) using `define_rhyming_group`.
     - A line can belong to multiple groups (End, Begin, Inline).

3. **Drafting**:
   - Use `get_rhymes` to find English words that fit your rhyme scheme.
     - `get_rhymes(word="cat", rhyme_type="end")` -> "bat", "hat"
     - `get_rhymes(word="cat", rhyme_type="begin")` -> "cab", "camera"
     - `get_rhymes(word="cat", rhyme_type="inline")` -> "black", "back" (Assonance)
   - Use `add_alternative` to translate/write lines in English.
   - Each line should have multiple alternatives to explore options.

4. **Analysis**:
   - Use `get_line_context` to see a line's peers in rhyming groups.
   - Use `calculate_scores` to evaluate how well each alternative rhymes with its peers in the same groups.
   - Use `evaluate_rhyme_quality` for a group-level report.
   - Use `set_alternative_custom_score` to tag qualities (e.g., "roughness: 4.5").
   - Use `set_song_targets` to define goals for the song.

5. **Optimization**:
   - Use `get_worst_lines` to identify which lines need improvement.
   - Use `generate_combination` to find the best set of alternatives that maximize Rhyme + Custom Scores based on your weights.
"""
            return [types.TextContent(type="text", text=tutorial_text)]

        elif name == "create_project":
            project = manager.create_new_project(arguments["title"], **arguments.get("metadata", {}))
            manager.save_project(project, arguments["filepath"])
            result = f"Created project: {arguments['filepath']}"

        elif name == "add_part":
            project = manager.load_project(arguments["filepath"])
            manager.add_part_to_song(project, arguments["name"], arguments.get("attributes", {}))
            manager.save_project(project, arguments["filepath"])
            result = f"Added part '{arguments['name']}' to project"

        elif name == "add_line":
            project = manager.load_project(arguments["filepath"])
            manager.add_line_to_part(
                project,
                arguments["part"],
                arguments["text"],
                arguments.get("skip_translation", False),
                arguments.get("syllable_count"),
                arguments.get("metadata", {})
            )
            manager.save_project(project, arguments["filepath"])
            result = f"Added line to part '{arguments['part']}'"

        elif name == "add_alternative":
            project = manager.load_project(arguments["filepath"])
            manager.add_alternative_to_line(
                project,
                arguments["part"],
                arguments["line_index"],
                arguments["text"]
            )
            manager.save_project(project, arguments["filepath"])
            result = f"Added alternative to line {arguments['line_index']} in part '{arguments['part']}'"

        elif name == "define_rhyming_group":
            project = manager.load_project(arguments["filepath"])
            manager.define_rhyming_group(
                project,
                arguments["group_id"],
                RhymingType(arguments["group_type"]),
                arguments["member_lines"],
                arguments.get("metadata", {})
            )
            manager.save_project(project, arguments["filepath"])
            result = f"Defined rhyming group '{arguments['group_id']}' with {len(arguments['member_lines'])} member lines"

        elif name == "set_song_targets":
            project = manager.load_project(arguments["filepath"])
            if "targets" not in project.metadata:
                project.metadata["targets"] = {}
            project.metadata["targets"].update(arguments["targets"])
            manager.save_project(project, arguments["filepath"])
            result = f"Updated song targets: {project.metadata['targets']}"

        elif name == "set_alternative_custom_score":
            project = manager.load_project(arguments["filepath"])
            target_part = next((p for p in project.song.parts if p.name == arguments["part"]), None)
            if not target_part: return [types.TextContent(type="text", text=f"Part '{arguments['part']}' not found")]
            if arguments["line_index"] >= len(target_part.lines): return [types.TextContent(type="text", text="Line index out of range")]
            line = target_part.lines[arguments["line_index"]]
            if arguments["alt_index"] >= len(line.alternatives): return [types.TextContent(type="text", text="Alt index out of range")]
            
            line.alternatives[arguments["alt_index"]].custom_scores[arguments["score_name"]] = arguments["score_value"]
            manager.save_project(project, arguments["filepath"])
            result = f"Set {arguments['score_name']}={arguments['score_value']} for {arguments['part']}:{arguments['line_index']} Alt {arguments['alt_index']}"

        elif name == "calculate_scores":
            project = manager.load_project(arguments["filepath"])
            # Calculate rhyming score for each alternative based on its groups
            for part in project.song.parts:
                for line_idx, line in enumerate(part.lines):
                    if line.skip_translation: continue
                    
                    # Find groups this line belongs to
                    line_groups = [rg for rg in project.rhyming_groups if any(m['part_name'] == part.name and m['line_position'] == line_idx for m in rg.member_lines)]
                    
                    for alt in line.alternatives:
                        if alt.text == line.original_text and alt.is_final and len(line.alternatives) > 1:
                            continue # Don't score original if alternatives exist
                        
                        group_scores = []
                        for rg in line_groups:
                            # Compare with peers in the group
                            peer_texts = []
                            for m in rg.member_lines:
                                if m['part_name'] == part.name and m['line_position'] == line_idx: continue
                                p_ref = next((p for p in project.song.parts if p.name == m['part_name']), None)
                                if p_ref and m['line_position'] < len(p_ref.lines):
                                    l_ref = p_ref.lines[m['line_position']]
                                    # Use latest alternative of peer
                                    peer_text = l_ref.alternatives[-1].text if l_ref.alternatives else l_ref.original_text
                                    peer_texts.append(peer_text)
                            
                            if not peer_texts: continue
                            
                            # Average rhyme with peers
                            s_sum = 0
                            for pt in peer_texts:
                                if rg.group_type == RhymingType.END_RHYME:
                                    s_sum += engine.calculate_end_rhyme_score(alt.text, pt)
                                elif rg.group_type == RhymingType.BEGIN_RHYME:
                                    s_sum += engine.calculate_begin_rhyme_score(alt.text, pt)
                                else:
                                    s_sum += engine.calculate_inline_rhyme_score(alt.text, pt)
                            group_scores.append(s_sum / len(peer_texts))
                        
                        alt.rhyming_score = sum(group_scores) / len(group_scores) if group_scores else 0.0
            
            manager.save_project(project, arguments["filepath"])
            result = "Calculated rhyming scores based on group peers."

        elif name == "get_worst_lines":
            project = manager.load_project(arguments["filepath"])
            limit = arguments.get("limit", 5)
            all_lines = []
            for part in project.song.parts:
                for line_idx, line in enumerate(part.lines):
                    if line.skip_translation: continue
                    # Use the rhyming score of the latest alternative
                    score = line.alternatives[-1].rhyming_score if line.alternatives else 0.0
                    all_lines.append({
                        "part": part.name,
                        "index": line_idx,
                        "text": line.alternatives[-1].text if line.alternatives else line.original_text,
                        "score": score
                    })
            
            worst = sorted(all_lines, key=lambda x: x["score"])[:limit]
            res = "Worst Rhyming Lines:\n"
            for w in worst:
                res += f"  {w['score']:.2f} - {w['part']}:{w['index']+1} [{w['text']}]\n"
            return [types.TextContent(type="text", text=res)]

        elif name == "get_rhymes":
            import pronouncing
            import re
            
            word = arguments["word"]
            limit = arguments.get("limit", 20)
            rhyme_type = arguments.get("rhyme_type", "end")
            
            phones_list = pronouncing.phones_for_word(word)
            if not phones_list:
                return [types.TextContent(type="text", text=f"Word '{word}' not found in dictionary.")]
            
            phones = phones_list[0] # Use first pronunciation
            rhymes = []
            
            if rhyme_type == "end":
                # Perfect rhymes
                rhymes = pronouncing.rhymes(word)
            
            elif rhyme_type == "begin":
                # Alliteration: Match start phones
                # Get first phoneme
                first_phone = phones.split()[0]
                # Search for words starting with this phoneme
                # Pattern: ^PHONE ...
                rhymes = pronouncing.search(f"^{first_phone}")
                
            elif rhyme_type == "inline":
                # Assonance: Match stressed vowel
                # Extract stressed vowel (has number 1 or 2 usually)
                vowels = [p for p in phones.split() if any(char.isdigit() for char in p)]
                if vowels:
                    stressed_vowel = vowels[0] # Primary
                    # Search for words containing this vowel
                    rhymes = pronouncing.search(f"{stressed_vowel}")
                else:
                    return [types.TextContent(type="text", text=f"Could not identify stressed vowel in '{word}' for inline rhyme.")]
            
            if not rhymes:
                return [types.TextContent(type="text", text=f"No {rhyme_type} rhymes found for '{word}'")]
            
            # Limit and format
            result = f"{rhyme_type.capitalize()} rhymes for '{word}':\n"
            result += ", ".join(rhymes[:limit])
            return [types.TextContent(type="text", text=result)]

        elif name == "get_line_context":
            project = manager.load_project(arguments["filepath"])
            part_name = arguments["part"]
            idx = arguments["line_index"]
            target_part = next((p for p in project.song.parts if p.name == part_name), None)
            if not target_part or idx >= len(target_part.lines):
                return [types.TextContent(type="text", text="Line not found")]
            
            line = target_part.lines[idx]
            context = f"Line Context: {part_name}:{idx+1}\n"
            context += f"Original: {line.original_text} (Syl: {line.syllable_count})\n"
            context += f"Groups: {', '.join(line.rhyming_groups)}\n"
            
            context += "Related Lines:\n"
            for rg in project.rhyming_groups:
                if any(m['part_name'] == part_name and m['line_position'] == idx for m in rg.member_lines):
                    context += f"  Group {rg.group_id} ({rg.group_type.value}):\n"
                    for m in rg.member_lines:
                        if m['part_name'] == part_name and m['line_position'] == idx: continue
                        p_ref = next((p for p in project.song.parts if p.name == m['part_name']), None)
                        if p_ref and m['line_position'] < len(p_ref.lines):
                            l_ref = p_ref.lines[m['line_position']]
                            context += f"    - {m['part_name']}:{m['line_position']+1} [{l_ref.original_text}]\n"
            
            return [types.TextContent(type="text", text=context)]

        elif name == "evaluate_rhyme_quality":
            project = manager.load_project(arguments["filepath"])
            report = "Rhyme Group Quality Report (Based on selected/first alternatives):\n"
            
            total_group_score = 0
            group_count = 0
            
            for rg in project.rhyming_groups:
                lines_text = []
                for m in rg.member_lines:
                    p = next((p for p in project.song.parts if p.name == m['part_name']), None)
                    if p and m['line_position'] < len(p.lines):
                        # Get first alternative (or 'selected' if we had that concept, defaulting to index 1 or 0)
                        # Typically index 0 is original, index 1 is first translation.
                        # We'll use the *last added* alternative or check 'is_final'? 
                        # Let's use the last one for "current draft" logic or index 1.
                        line = p.lines[m['line_position']]
                        if len(line.alternatives) > 1:
                            txt = line.alternatives[-1].text # Use latest draft
                        else:
                            txt = line.original_text
                        lines_text.append(txt)
                
                # Pairwise score
                if len(lines_text) < 2: continue
                
                g_score = 0
                pairs = 0
                for i in range(len(lines_text)):
                    for j in range(i+1, len(lines_text)):
                        # Use engine
                        if rg.group_type == RhymingType.END_RHYME:
                            s = engine.calculate_end_rhyme_score(lines_text[i], lines_text[j])
                        elif rg.group_type == RhymingType.BEGIN_RHYME:
                            s = engine.calculate_begin_rhyme_score(lines_text[i], lines_text[j])
                        else:
                            s = engine.calculate_inline_rhyme_score(lines_text[i], lines_text[j])
                        g_score += s
                        pairs += 1
                
                avg_g_score = g_score / pairs if pairs > 0 else 0
                report += f"  Group {rg.group_id} ({rg.group_type.value}): Score {avg_g_score:.2f} ({', '.join([t[:10] for t in lines_text])})\n"
                total_group_score += avg_g_score
                group_count += 1
                
            report += f"\nOverall Rhyme Quality: {total_group_score/group_count:.2f}" if group_count else "\nNo groups scored."
            return [types.TextContent(type="text", text=report)]

        elif name == "generate_combination":
            project = manager.load_project(arguments["filepath"])
            weights = arguments.get("weights", {"rhyme": 1.0})
            iterations = arguments.get("count", 100)
            
            # Helper to get active text for a "genome" (set of alt indices)
            def get_text(genome, p_name, l_idx):
                key = f"{p_name}:{l_idx}"
                alt_idx = genome.get(key, 0) # Default to 0 (Original)
                target_p = next((p for p in project.song.parts if p.name == p_name), None)
                if not target_p or l_idx >= len(target_p.lines): return ""
                if alt_idx >= len(target_p.lines[l_idx].alternatives): return ""
                return target_p.lines[l_idx].alternatives[alt_idx].text

            def get_custom_score(genome, key_score):
                total = 0
                count = 0
                for part in project.song.parts:
                    for l_idx, line in enumerate(part.lines):
                        idx = genome.get(f"{part.name}:{l_idx}", 0)
                        if idx < len(line.alternatives):
                            val = line.alternatives[idx].custom_scores.get(key_score, 0)
                            total += val
                            count += 1
                return total / count if count else 0

            def calculate_fitness(genome):
                score = 0
                # Rhyme Score
                if "rhyme" in weights:
                    r_total = 0
                    r_count = 0
                    for rg in project.rhyming_groups:
                        txts = [get_text(genome, m['part_name'], m['line_position']) for m in rg.member_lines]
                        if len(txts) < 2: continue
                        g_s = 0
                        p_s = 0
                        for i in range(len(txts)):
                            for j in range(i+1, len(txts)):
                                if rg.group_type == RhymingType.END_RHYME:
                                    g_s += engine.calculate_end_rhyme_score(txts[i], txts[j])
                                elif rg.group_type == RhymingType.BEGIN_RHYME:
                                    g_s += engine.calculate_begin_rhyme_score(txts[i], txts[j])
                                else:
                                    g_s += engine.calculate_inline_rhyme_score(txts[i], txts[j])
                                p_s += 1
                        if p_s: r_total += g_s/p_s
                        r_count += 1
                    if r_count: score += (r_total/r_count) * weights["rhyme"]

                # Custom Scores
                for k, w in weights.items():
                    if k != "rhyme":
                        score += get_custom_score(genome, k) * w
                return score

            # Simple Random Search (Monte Carlo)
            best_genome = {}
            best_score = -1.0
            
            # Initialize random genome
            structure = {}
            for part in project.song.parts:
                for l_idx, line in enumerate(part.lines):
                    # Only pick from created alternatives (skip 0/original usually if we have alts)
                    count_alts = len(line.alternatives)
                    if count_alts > 1:
                        structure[f"{part.name}:{l_idx}"] = range(1, count_alts) # Exclude original
                    else:
                        structure[f"{part.name}:{l_idx}"] = [0]

            for _ in range(iterations):
                current_genome = {k: random.choice(v) for k, v in structure.items()}
                fitness = calculate_fitness(current_genome)
                if fitness > best_score:
                    best_score = fitness
                    best_genome = current_genome
            
            result = f"Best Combination Found (Score: {best_score:.2f}):\n"
            for k, alt_idx in best_genome.items():
                p_name, l_idx = k.split(":")
                txt = get_text(best_genome, p_name, int(l_idx))
                result += f"  {k}: {txt} (Alt {alt_idx})\n"
            
            return [types.TextContent(type="text", text=result)]

        elif name == "get_part_score":
            project = manager.load_project(arguments["filepath"])
            target_part = next((p for p in project.song.parts if p.name == arguments["part"]), None)
            if not target_part: return [types.TextContent(type="text", text=f"Part '{arguments['part']}' not found")]
            scores = [alt.rhyming_score for line in target_part.lines for alt in line.alternatives if not (alt.text == line.original_text and alt.is_final)]
            avg_score = sum(scores) / len(scores) if scores else 0.0
            result = f"Average rhyming score for part '{arguments['part']}': {avg_score:.2f}"

        elif name == "dump_project":
            project = manager.load_project(arguments["filepath"])
            part_name = arguments.get("part")
            
            if part_name:
                target_part = next((p for p in project.song.parts if p.name == part_name), None)
                if not target_part:
                    return [types.TextContent(type="text", text=f"Part '{part_name}' not found in project")]

                result = f"[{target_part.name}]\n"
                for line_idx, line in enumerate(target_part.lines):
                    syl_str = f" [{line.syllable_count} syl]" if line.syllable_count is not None else ""
                    result += f"  {line_idx+1}. {line.original_text}{syl_str}\n"
                    if line.metadata:
                        result += f"      Metadata: {line.metadata}\n"
                    if len(line.alternatives) > 1:
                        result += "      Alternatives:\n"
                        for alt_idx, alt in enumerate(line.alternatives[1:], 1):  # Skip original
                            result += f"        {alt_idx}. {alt.text} [Score: {alt.rhyming_score:.2f}]\n"
                            if alt.custom_scores:
                                result += f"            Custom scores: {alt.custom_scores}\n"
            else:
                result = f"Project: {project.metadata.get('title', 'Untitled')}\n"
                result += "=" * 50 + "\n"
                result += f"\nParts ({len(project.song.parts)}):\n"
                for p in project.song.parts:
                    attrs = ', '.join([f"{k}:{v}" for k, v in p.attributes.items()])
                    attr_str = f" ({attrs})" if attrs else ""
                    result += f"\n  {p.name}{attr_str}:\n"
                    for line_idx, line in enumerate(p.lines):
                        syl_str = f" [{line.syllable_count} syl]" if line.syllable_count is not None else ""
                        result += f"    {line_idx+1}. {line.original_text}{syl_str}\n"
                        if len(line.alternatives) > 1:
                            for alt in line.alternatives[1:]:
                                result += f"       ~ {alt.text} [Score: {alt.rhyming_score:.2f}]\n"

                if project.rhyming_groups:
                    result += f"\nRhyming Groups ({len(project.rhyming_groups)}):\n"
                    for rg in project.rhyming_groups:
                        result += f"  {rg.group_id} ({rg.group_type.value}): {len(rg.member_lines)} lines\n"

        elif name == "remove_line":
            project = manager.load_project(arguments["filepath"])
            target_part = next((p for p in project.song.parts if p.name == arguments["part"]), None)
            if not target_part: return [types.TextContent(type="text", text=f"Part '{arguments['part']}' not found")]
            if arguments["line_index"] >= len(target_part.lines): return [types.TextContent(type="text", text="Line index out of range")]
            removed = target_part.lines.pop(arguments["line_index"])
            for i, line in enumerate(target_part.lines): line.position = i
            manager.save_project(project, arguments["filepath"])
            result = f"Removed line: {removed.original_text}"

        else:
            raise ValueError(f"Unknown tool: {name}")

        return [types.TextContent(type="text", text=result)]

    except Exception as e:
        return [types.TextContent(type="text", text=f"Error: {str(e)}")]

async def main():
    """Main entry point."""
    async with mcp.server.stdio.stdio_server() as (read_stream, write_stream):
        await app.run(
            read_stream,
            write_stream,
            app.create_initialization_options()
        )

if __name__ == "__main__":
    asyncio.run(main())
