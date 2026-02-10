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
            description="Find words that rhyme with a given word. Use this to improve the rhyming score of your alternatives. Supports end rhymes (perfect), begin rhymes (alliteration), and inline rhymes (assonance).",
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
        ),
        types.Tool(
            name="set_active_alternative",
            description="Set a specific alternative as the 'active' (final) choice for a line.",
            inputSchema={
                "type": "object",
                "properties": {
                    "filepath": {"type": "string", "description": "Path to the project file"},
                    "part": {"type": "string", "description": "Name of the part"},
                    "line_index": {"type": "integer", "description": "Index of the line (0-based)"},
                    "alt_index": {"type": "integer", "description": "Index of the alternative"}
                },
                "required": ["filepath", "part", "line_index", "alt_index"]
            }
        ),
        types.Tool(
            name="apply_combination",
            description="Apply a generated combination, making its selections the 'active' ones for the project.",
            inputSchema={
                "type": "object",
                "properties": {
                    "filepath": {"type": "string", "description": "Path to the project file"},
                    "combination_id": {"type": "string", "description": "ID of the combination to apply"}
                },
                "required": ["filepath", "combination_id"]
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
   - Use `get_rhymes` to find English words that fit your rhyme scheme and **improve your rhyming scores**.
   - Use `add_alternative` to translate/write lines in English.
   - Use `set_active_alternative` to pick which draft you currently want to use.
   - **Songwriting Rules**:
     - **Avoid Repetition**: Avoid using the same words twice in a song, unless it is a deliberate repetition for effect.
     - **Forbidden Words**: Avoid using the word "now".

4. **Analysis**:
   - Use `get_line_context` to see a line's peers in rhyming groups.
   - Use `calculate_scores` to evaluate alternatives against the **currently active** peers.
   - Use `evaluate_rhyme_quality` for a group-level report of the **active** song version.
   - Use `set_alternative_custom_score` to tag qualities (e.g., "roughness: 4.5").
   - Use `set_song_targets` to define goals for the song.

5. **Optimization**:
   - Use `generate_combination` to find the best set of alternatives that maximize Rhyme + Custom Scores.
   - Use `apply_combination` to make that set the new "active" version of the song.
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

        elif name == "set_active_alternative":
            project = manager.load_project(arguments["filepath"])
            manager.set_active_alternative(project, arguments["part"], arguments["line_index"], arguments["alt_index"])
            manager.save_project(project, arguments["filepath"])
            result = f"Set {arguments['part']}:{arguments['line_index']} Alt {arguments['alt_index']} as active."

        elif name == "apply_combination":
            project = manager.load_project(arguments["filepath"])
            combo = next((c for c in project.combinations if c.combination_id == arguments["combination_id"]), None)
            if not combo: return [types.TextContent(type="text", text="Combination not found")]
            
            for key, alt_id in combo.line_selections.items():
                p_name, l_idx = key.split(":")
                l_idx = int(l_idx)
                target_p = next((p for p in project.song.parts if p.name == p_name), None)
                if target_p and l_idx < len(target_p.lines):
                    line = target_p.lines[l_idx]
                    for alt in line.alternatives:
                        alt.is_final = (alt.id == alt_id)
            
            project.active_combination_id = combo.combination_id
            manager.save_project(project, arguments["filepath"])
            result = f"Applied combination {arguments['combination_id']} to all lines."

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
                            # Compare with ACTIVE peers in the group
                            peer_texts = []
                            for m in rg.member_lines:
                                if m['part_name'] == part.name and m['line_position'] == line_idx: continue
                                txt = manager.get_active_text(project, m['part_name'], m['line_position'])
                                if txt: peer_texts.append(txt)
                            
                            if not peer_texts: continue
                            
                            # Average rhyme with active peers
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
            result = "Calculated rhyming scores based on active group peers."

        elif name == "get_worst_lines":
            project = manager.load_project(arguments["filepath"])
            limit = arguments.get("limit", 5)
            all_lines = []
            for part in project.song.parts:
                for line_idx, line in enumerate(part.lines):
                    if line.skip_translation: continue
                    # Get the active alternative's score
                    active_alt = next((a for a in line.alternatives if a.is_final), line.alternatives[-1] if line.alternatives else None)
                    score = active_alt.rhyming_score if active_alt else 0.0
                    txt = active_alt.text if active_alt else line.original_text
                    all_lines.append({
                        "part": part.name,
                        "index": line_idx,
                        "text": txt,
                        "score": score
                    })
            
            worst = sorted(all_lines, key=lambda x: x["score"])[:limit]
            res = "Worst Rhyming Lines (Active Selection):\n"
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
                rhymes = pronouncing.rhymes(word)
            elif rhyme_type == "begin":
                first_phone = phones.split()[0]
                rhymes = pronouncing.search(f"^{first_phone}")
            elif rhyme_type == "inline":
                vowels = [p for p in phones.split() if any(char.isdigit() for char in p)]
                if vowels:
                    stressed_vowel = vowels[0]
                    rhymes = pronouncing.search(f"{stressed_vowel}")
                else:
                    return [types.TextContent(type="text", text=f"Could not identify stressed vowel in '{word}' for inline rhyme.")]
            
            if not rhymes:
                return [types.TextContent(type="text", text=f"No {rhyme_type} rhymes found for '{word}'")]
            
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
            context += f"Active Draft: {manager.get_active_text(project, part_name, idx)}\n"
            context += f"Groups: {', '.join(line.rhyming_groups)}\n"
            
            context += "Related Lines (Active Selections):\n"
            for rg in project.rhyming_groups:
                if any(m['part_name'] == part_name and m['line_position'] == idx for m in rg.member_lines):
                    context += f"  Group {rg.group_id} ({rg.group_type.value}):\n"
                    for m in rg.member_lines:
                        if m['part_name'] == part_name and m['line_position'] == idx: continue
                        txt = manager.get_active_text(project, m['part_name'], m['line_position'])
                        context += f"    - {m['part_name']}:{m['line_position']+1} [{txt}]\n"
            
            return [types.TextContent(type="text", text=context)]

        elif name == "evaluate_rhyme_quality":
            project = manager.load_project(arguments["filepath"])
            report = "Rhyme Group Quality Report (Active Selections):\n"
            
            total_group_score = 0
            group_count = 0
            
            for rg in project.rhyming_groups:
                lines_text = []
                for m in rg.member_lines:
                    txt = manager.get_active_text(project, m['part_name'], m['line_position'])
                    if txt: lines_text.append(txt)
                
                if len(lines_text) < 2: continue
                
                g_score = 0
                pairs = 0
                for i in range(len(lines_text)):
                    for j in range(i+1, len(lines_text)):
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
            
            from rhyme_calculator.models import Combination
            
            # Helper to get text for a genome
            def get_text_from_genome(genome, p_name, l_idx):
                key = f"{p_name}:{l_idx}"
                alt_idx = genome.get(key, 0)
                target_p = next((p for p in project.song.parts if p.name == p_name), None)
                if not target_p or l_idx >= len(target_p.lines): return ""
                line = target_p.lines[l_idx]
                if alt_idx >= len(line.alternatives): return ""
                return line.alternatives[alt_idx].text

            def get_fitness(genome):
                fit = 0
                # Rhyme
                if "rhyme" in weights:
                    r_tot, r_cnt = 0, 0
                    for rg in project.rhyming_groups:
                        txts = [get_text_from_genome(genome, m['part_name'], m['line_position']) for m in rg.member_lines]
                        if len(txts) < 2: continue
                        g_s, p_s = 0, 0
                        for i in range(len(txts)):
                            for j in range(i+1, len(txts)):
                                if rg.group_type == RhymingType.END_RHYME: g_s += engine.calculate_end_rhyme_score(txts[i], txts[j])
                                elif rg.group_type == RhymingType.BEGIN_RHYME: g_s += engine.calculate_begin_rhyme_score(txts[i], txts[j])
                                else: g_s += engine.calculate_inline_rhyme_score(txts[i], txts[j])
                                p_s += 1
                        if p_s: r_tot += g_s/p_s
                        r_cnt += 1
                    if r_cnt: fit += (r_tot/r_cnt) * weights["rhyme"]
                # Custom
                for k, w in weights.items():
                    if k == "rhyme": continue
                    c_tot, c_cnt = 0, 0
                    for part in project.song.parts:
                        for l_idx, line in enumerate(part.lines):
                            idx = genome.get(f"{part.name}:{l_idx}", 0)
                            if idx < len(line.alternatives):
                                c_tot += line.alternatives[idx].custom_scores.get(k, 0)
                                c_cnt += 1
                    if c_cnt: fit += (c_tot/c_cnt) * w
                return fit

            best_genome, best_score = {}, -1.0
            structure = {}
            for part in project.song.parts:
                for l_idx, line in enumerate(part.lines):
                    # Prefer picking from translations (index > 0) if they exist
                    if len(line.alternatives) > 1: structure[f"{part.name}:{l_idx}"] = range(1, len(line.alternatives))
                    else: structure[f"{part.name}:{l_idx}"] = [0]

            for _ in range(iterations):
                current = {k: random.choice(v) for k, v in structure.items()}
                f = get_fitness(current)
                if f > best_score: best_score, best_genome = f, current
            
            # Save as Combination object
            line_selections = {}
            for k, idx in best_genome.items():
                p_name, l_idx = k.split(":")
                l_idx = int(l_idx)
                target_p = next((p for p in project.song.parts if p.name == p_name), None)
                if target_p: line_selections[k] = target_p.lines[l_idx].alternatives[idx].id
            
            new_combo = Combination(
                combination_id=str(random.randint(1000, 9999)),
                line_selections=line_selections,
                total_rhyming_score=best_score
            )
            manager.add_combination(project, new_combo)
            manager.save_project(project, arguments["filepath"])
            
            res = f"Best Combination Found (ID: {new_combo.combination_id}, Score: {best_score:.2f}):\n"
            for k, idx in best_genome.items():
                res += f"  {k}: {get_text_from_genome(best_genome, k.split(':')[0], int(k.split(':')[1]))} (Alt {idx})\n"
            res += "\nUse apply_combination to make this the active version."
            return [types.TextContent(type="text", text=res)]

        elif name == "dump_project":
            project = manager.load_project(arguments["filepath"])
            part_name = arguments.get("part")
            
            if part_name:
                target_part = next((p for p in project.song.parts if p.name == part_name), None)
                if not target_part: return [types.TextContent(type="text", text=f"Part '{part_name}' not found")]
                result = f"[{target_part.name}]\n"
                for line_idx, line in enumerate(target_part.lines):
                    syl_str = f" [{line.syllable_count} syl]" if line.syllable_count is not None else ""
                    # Mark active with *
                    for alt_idx, alt in enumerate(line.alternatives):
                        prefix = "  *" if alt.is_final else "   "
                        result += f"{prefix} {line_idx+1}.{alt_idx} {alt.text}{syl_str if alt_idx==0 else ''} [Score: {alt.rhyming_score:.2f}]\n"
            else:
                result = f"Project: {project.metadata.get('title', 'Untitled')}\n"
                result += "=" * 50 + "\n"
                for p in project.song.parts:
                    result += f"\n  {p.name}:\n"
                    for line_idx, line in enumerate(p.lines):
                        # Show active one
                        active_txt = manager.get_active_text(project, p.name, line_idx)
                        syl_str = f" [{line.syllable_count} syl]" if line.syllable_count is not None else ""
                        result += f"    {line_idx+1}. {active_txt}{syl_str}\n"
                
                if project.combinations:
                    result += "\nCombinations: " + ", ".join([c.combination_id for c in project.combinations]) + "\n"

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
