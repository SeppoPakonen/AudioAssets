"""
Command-line interface for the rhyming score calculator.
Provides commands for creating, loading, and manipulating projects.
"""

import click
import sys
from pathlib import Path
from typing import Dict, List

# Handle both direct execution and module import
try:
    from .models import RhymingType
    from .manager import ProjectManager
    from .engine import RhymingEngine
except ImportError:
    # When running as a script directly
    import os
    import sys
    sys.path.insert(0, os.path.dirname(__file__))
    from models import RhymingType
    from manager import ProjectManager
    from engine import RhymingEngine


@click.group()
@click.version_option(version='0.1.0')
def cli():
    """Rhyming Score Calculator - Calculate rhyming scores for English lyrics."""
    pass


@cli.command()
@click.argument('filepath', type=click.Path())
@click.option('--title', prompt='Project title', help='Title of the new project')
def create_project(filepath: str, title: str):
    """Create a new project file."""
    manager = ProjectManager()
    
    # Ensure the directory exists
    Path(filepath).parent.mkdir(parents=True, exist_ok=True)
    
    project = manager.create_new_project(title=title)
    manager.save_project(project, filepath)
    
    click.echo(f"Created new project: {filepath}")


@cli.command()
@click.argument('filepath', type=click.Path(exists=True))
def load_project(filepath: str):
    """Load and display information about a project."""
    manager = ProjectManager()
    project = manager.load_project(filepath)
    
    click.echo(f"Loaded project: {project.metadata['title']}")
    click.echo(f"Parts: {len(project.song.parts)}")
    click.echo(f"Rhyming groups: {len(project.rhyming_groups)}")
    click.echo(f"Combinations: {len(project.combinations)}")


@cli.command()
@click.argument('filepath', type=click.Path(exists=True))
@click.option('--name', prompt='Part name', help='Name of the part (e.g., "Verse 1", "Chorus")')
@click.option('--style', default='', help='Style attribute (e.g., singing, rapping)')
@click.option('--gender', default='', help='Gender attribute (e.g., male, female)')
def add_part(filepath: str, name: str, style: str, gender: str):
    """Add a new part to the project."""
    manager = ProjectManager()
    project = manager.load_project(filepath)
    
    attributes = {}
    if style:
        attributes['style'] = style
    if gender:
        attributes['gender'] = gender
    
    manager.add_part_to_song(project, name, attributes)
    manager.save_project(project, filepath)
    
    click.echo(f"Added part '{name}' to project")


@cli.command()
@click.argument('filepath', type=click.Path(exists=True))
@click.option('--part', prompt='Part name', help='Name of the part to add line to')
@click.option('--text', prompt='Line text', help='The original line text')
@click.option('--skip-translation', is_flag=True, help='Flag to skip translation for this line (e.g., for vocal sounds like "oo-oo")')
@click.option('--syllable-count', type=int, help='Number of syllables in the line')
@click.option('--metadata', multiple=True, help='Metadata in key=value format (can be used multiple times)')
def add_line(filepath: str, part: str, text: str, skip_translation: bool, syllable_count: int, metadata: tuple):
    """Add a new line to a specific part."""
    manager = ProjectManager()
    project = manager.load_project(filepath)
    
    # Convert metadata tuples to dictionary
    metadata_dict = {}
    for item in metadata:
        if '=' in item:
            key, value = item.split('=', 1)
            metadata_dict[key] = value
    
    manager.add_line_to_part(project, part, text, skip_translation=skip_translation, 
                             syllable_count=syllable_count, metadata=metadata_dict)
    manager.save_project(project, filepath)
    
    status_parts = []
    if skip_translation:
        status_parts.append("skipped translation")
    if syllable_count is not None:
        status_parts.append(f"{syllable_count} syllables")
    if metadata_dict:
        status_parts.append(f"metadata: {len(metadata_dict)} items")
    
    status = f" ({', '.join(status_parts)})" if status_parts else ""
    click.echo(f"Added line to part '{part}'{status}")


@cli.command()
@click.argument('filepath', type=click.Path(exists=True))
@click.option('--part', prompt='Part name', help='Name of the part containing the line')
@click.option('--line-index', prompt='Line index', type=int, help='Index of the line in the part')
@click.option('--text', prompt='Alternative text', help='The alternative line text')
def add_alternative(filepath: str, part: str, line_index: int, text: str):
    """Add an alternative version to a specific line."""
    manager = ProjectManager()
    project = manager.load_project(filepath)
    
    manager.add_alternative_to_line(project, part, line_index, text)
    manager.save_project(project, filepath)
    
    click.echo(f"Added alternative to line {line_index} in part '{part}'")


@cli.command()
@click.argument('filepath', type=click.Path(exists=True))
def calculate_scores(filepath: str):
    """Calculate rhyming scores for all alternatives in the project."""
    manager = ProjectManager()
    project = manager.load_project(filepath)
    engine = RhymingEngine()
    
    # Calculate scores for all alternatives
    for part in project.song.parts:
        for line_idx, line in enumerate(part.lines):
            # Skip scoring if this line has skip_translation flag set
            if line.skip_translation:
                # Set all alternative scores to 0 for skipped translation lines
                for alt in line.alternatives:
                    alt.rhyming_score = 0.0
                continue
            
            # Get the original text for comparison
            original_text = line.original_text
            
            # Calculate scores for each alternative
            for alt_idx, alt in enumerate(line.alternatives):
                # Skip if this is the original text (already marked as final)
                if alt.text == original_text and alt.is_final:
                    continue
                
                # Calculate score based on rhyming groups this line belongs to
                total_score = 0.0
                rhyme_types = []
                
                # Find all rhyming groups this line is part of
                for rg in project.rhyming_groups:
                    if any(member['part_name'] == part.name and 
                           member['line_position'] == line_idx 
                           for member in rg.member_lines):
                        rhyme_types.append(rg.group_type)
                
                # If no specific rhyme types from groups, use all types
                if not rhyme_types:
                    rhyme_types = [RhymingType.END_RHYME, RhymingType.BEGIN_RHYME, RhymingType.INLINE_RHYME]
                
                # Calculate the score
                if rhyme_types:
                    alt.rhyming_score = engine.calculate_total_score(original_text, alt.text, rhyme_types)
                else:
                    alt.rhyming_score = 0.0
    
    # Save the updated project
    manager.save_project(project, filepath)
    
    click.echo("Calculated rhyming scores for all alternatives")


@cli.command()
@click.argument('filepath', type=click.Path(exists=True))
@click.option('--part', prompt='Part name', help='Name of the part containing the line')
@click.option('--line-index', prompt='Line index', type=int, help='Index of the line in the part')
@click.option('--metadata', multiple=True, help='Metadata in key=value format (can be used multiple times)')
def set_line_metadata(filepath: str, part: str, line_index: int, metadata: tuple):
    """Set metadata for a specific line."""
    manager = ProjectManager()
    project = manager.load_project(filepath)
    
    # Find the part
    target_part = None
    for p in project.song.parts:
        if p.name == part:
            target_part = p
            break
    
    if target_part is None:
        click.echo(f"Part '{part}' not found in project")
        return
    
    if line_index >= len(target_part.lines) or line_index < 0:
        click.echo(f"Line index {line_index} out of range for part '{part}'")
        return
    
    # Convert metadata tuples to dictionary
    metadata_dict = {}
    for item in metadata:
        if '=' in item:
            key, value = item.split('=', 1)
            metadata_dict[key] = value
    
    # Update the line's metadata
    target_part.lines[line_index].metadata.update(metadata_dict)
    
    # Save the updated project
    manager.save_project(project, filepath)
    
    click.echo(f"Updated metadata for line {line_index} in part '{part}': {metadata_dict}")


@cli.command()
@click.argument('filepath', type=click.Path(exists=True))
@click.option('--part', prompt='Part name', help='Name of the part to remove')
def remove_part(filepath: str, part: str):
    """Remove a specific part from the project."""
    manager = ProjectManager()
    project = manager.load_project(filepath)
    
    # Find the part
    part_index = None
    for i, p in enumerate(project.song.parts):
        if p.name == part:
            part_index = i
            break
    
    if part_index is None:
        click.echo(f"Part '{part}' not found in project")
        return
    
    # Remove the part
    removed_part = project.song.parts.pop(part_index)
    
    # Save the updated project
    manager.save_project(project, filepath)
    
    click.echo(f"Removed part '{part}' with {len(removed_part.lines)} lines")


@cli.command()
@click.argument('filepath', type=click.Path(exists=True))
@click.option('--part', prompt='Part name', help='Name of the part containing the line')
@click.option('--line-index', prompt='Line index', type=int, help='Index of the line to remove')
def remove_line(filepath: str, part: str, line_index: int):
    """Remove a specific line from a part."""
    manager = ProjectManager()
    project = manager.load_project(filepath)
    
    # Find the part
    target_part = None
    for p in project.song.parts:
        if p.name == part:
            target_part = p
            break
    
    if target_part is None:
        click.echo(f"Part '{part}' not found in project")
        return
    
    if line_index >= len(target_part.lines) or line_index < 0:
        click.echo(f"Line index {line_index} out of range for part '{part}'")
        return
    
    # Remove the line
    removed_line = target_part.lines.pop(line_index)
    
    # Update positions of remaining lines
    for i, line in enumerate(target_part.lines):
        line.position = i
    
    # Save the updated project
    manager.save_project(project, filepath)
    
    click.echo(f"Removed line from part '{part}', original text: '{removed_line.original_text}'")


@cli.command()
@click.argument('filepath', type=click.Path(exists=True))
@click.option('--part', prompt='Part name', help='Name of the part containing the line')
@click.option('--line-index', prompt='Line index', type=int, help='Index of the line containing the alternative')
@click.option('--alt-index', prompt='Alternative index', type=int, help='Index of the alternative to remove (0 is original)')
def remove_alternative(filepath: str, part: str, line_index: int, alt_index: int):
    """Remove a specific alternative from a line."""
    manager = ProjectManager()
    project = manager.load_project(filepath)
    
    # Find the part
    target_part = None
    for p in project.song.parts:
        if p.name == part:
            target_part = p
            break
    
    if target_part is None:
        click.echo(f"Part '{part}' not found in project")
        return
    
    if line_index >= len(target_part.lines) or line_index < 0:
        click.echo(f"Line index {line_index} out of range for part '{part}'")
        return
    
    if alt_index >= len(target_part.lines[line_index].alternatives) or alt_index < 0:
        click.echo(f"Alternative index {alt_index} out of range for line {line_index}")
        return
    
    # Remove the alternative
    removed_alt = target_part.lines[line_index].alternatives.pop(alt_index)
    
    # Save the updated project
    manager.save_project(project, filepath)
    
    click.echo(f"Removed alternative from line {line_index} in part '{part}', text: '{removed_alt.text}'")


@cli.command()
@click.argument('filepath', type=click.Path(exists=True))
@click.option('--combination-id', prompt='Combination ID', help='ID of the combination to remove')
def remove_combination(filepath: str, combination_id: str):
    """Remove a specific combination from the project."""
    manager = ProjectManager()
    project = manager.load_project(filepath)
    
    # Find the combination
    combo_index = None
    for i, combo in enumerate(project.combinations):
        if combo.combination_id == combination_id:
            combo_index = i
            break
    
    if combo_index is None:
        click.echo(f"Combination '{combination_id}' not found in project")
        return
    
    # If this was the active combination, clear the active ID
    if project.active_combination_id == combination_id:
        project.active_combination_id = None
    
    # Remove the combination
    removed_combo = project.combinations.pop(combo_index)
    
    # Save the updated project
    manager.save_project(project, filepath)
    
    click.echo(f"Removed combination '{combination_id}' with score: {removed_combo.total_rhyming_score}")


@cli.command()
@click.argument('filepath', type=click.Path(exists=True))
@click.option('--part', prompt='Part name', help='Name of the part containing the line')
@click.option('--line-index', prompt='Line index', type=int, help='Index of the line in the part')
@click.option('--alt-index', prompt='Alternative index', type=int, help='Index of the alternative in the line')
@click.option('--metadata', multiple=True, help='Metadata in key=value format (can be used multiple times)')
def set_alternative_metadata(filepath: str, part: str, line_index: int, alt_index: int, metadata: tuple):
    """Set metadata for a specific alternative."""
    manager = ProjectManager()
    project = manager.load_project(filepath)
    
    # Find the part
    target_part = None
    for p in project.song.parts:
        if p.name == part:
            target_part = p
            break
    
    if target_part is None:
        click.echo(f"Part '{part}' not found in project")
        return
    
    if line_index >= len(target_part.lines) or line_index < 0:
        click.echo(f"Line index {line_index} out of range for part '{part}'")
        return
    
    if alt_index >= len(target_part.lines[line_index].alternatives) or alt_index < 0:
        click.echo(f"Alternative index {alt_index} out of range for line {line_index}")
        return
    
    # Convert metadata tuples to dictionary
    metadata_dict = {}
    for item in metadata:
        if '=' in item:
            key, value = item.split('=', 1)
            metadata_dict[key] = value
    
    # Update the alternative's metadata
    target_part.lines[line_index].alternatives[alt_index].metadata.update(metadata_dict)
    
    # Save the updated project
    manager.save_project(project, filepath)
    
    click.echo(f"Updated metadata for alternative {alt_index} in line {line_index} in part '{part}': {metadata_dict}")


@cli.command()
@click.argument('filepath', type=click.Path(exists=True))
def list_parts(filepath: str):
    """List all parts in the project."""
    manager = ProjectManager()
    project = manager.load_project(filepath)
    
    click.echo(f"Parts in project '{project.metadata['title']}':")
    for i, part in enumerate(project.song.parts):
        attrs = ', '.join([f"{k}:{v}" for k, v in part.attributes.items()])
        attr_str = f" ({attrs})" if attrs else ""
        click.echo(f"  {i+1}. {part.name}{attr_str} - {len(part.lines)} lines")


@cli.command()
@click.argument('filepath', type=click.Path(exists=True))
@click.option('--part', prompt='Part name', help='Name of the part containing the line')
@click.option('--line-index', prompt='Line index', type=int, help='Index of the line in the part')
@click.option('--alt-index', prompt='Alternative index', type=int, help='Index of the alternative in the line')
@click.option('--score-name', prompt='Score name', help='Name of the custom score (e.g., joyfulness)')
@click.option('--score-value', prompt='Score value', type=float, help='Value of the custom score')
def set_alternative_custom_score(filepath: str, part: str, line_index: int, alt_index: int, score_name: str, score_value: float):
    """Set a custom score for a specific alternative."""
    manager = ProjectManager()
    project = manager.load_project(filepath)
    
    # Find the part
    target_part = None
    for p in project.song.parts:
        if p.name == part:
            target_part = p
            break
    
    if target_part is None:
        click.echo(f"Part '{part}' not found in project")
        return
    
    if line_index >= len(target_part.lines) or line_index < 0:
        click.echo(f"Line index {line_index} out of range for part '{part}'")
        return
    
    if alt_index >= len(target_part.lines[line_index].alternatives) or alt_index < 0:
        click.echo(f"Alternative index {alt_index} out of range for line {line_index}")
        return
    
    # Update the alternative's custom scores
    target_part.lines[line_index].alternatives[alt_index].custom_scores[score_name] = score_value
    
    # Save the updated project
    manager.save_project(project, filepath)
    
    click.echo(f"Set custom score '{score_name}' to {score_value} for alternative {alt_index} in line {line_index} in part '{part}'")


@cli.command()
@click.argument('filepath', type=click.Path(exists=True))
@click.option('--combination-id', prompt='Combination ID', help='ID of the combination to update')
@click.option('--score-name', prompt='Score name', help='Name of the custom score')
@click.option('--score-value', prompt='Score value', type=float, help='Value of the custom score')
def set_combination_custom_score(filepath: str, combination_id: str, score_name: str, score_value: float):
    """Set a custom score for a specific combination."""
    manager = ProjectManager()
    project = manager.load_project(filepath)
    
    # Find the combination
    target_combo = None
    for combo in project.combinations:
        if combo.combination_id == combination_id:
            target_combo = combo
            break
    
    if target_combo is None:
        click.echo(f"Combination '{combination_id}' not found in project")
        return
    
    # Update the combination's custom scores
    target_combo.custom_scores[score_name] = score_value
    
    # Save the updated project
    manager.save_project(project, filepath)
    
    click.echo(f"Set custom score '{score_name}' to {score_value} for combination '{combination_id}'")


@cli.command()
@click.argument('filepath', type=click.Path(exists=True))
@click.option('--part', default=None, help='Name of the part to show lines for (show all if not specified)')
def list_lines(filepath: str, part: str):
    """List all lines in the project or in a specific part."""
    manager = ProjectManager()
    project = manager.load_project(filepath)
    
    if part:
        # Find the specific part
        target_part = None
        for p in project.song.parts:
            if p.name == part:
                target_part = p
                break
        
        if not target_part:
            click.echo(f"Part '{part}' not found in project")
            return
        
        click.echo(f"Lines in part '{target_part.name}':")
        for i, line in enumerate(target_part.lines):
            click.echo(f"  {i+1}. {line.original_text}")
            if line.alternatives:
                click.echo(f"     Alternatives: {len(line.alternatives)-1}")  # -1 to exclude original
    else:
        click.echo(f"Lines in project '{project.metadata['title']}':")
        for part in project.song.parts:
            click.echo(f"\n{part.name}:")
            for i, line in enumerate(part.lines):
                click.echo(f"  {i+1}. {line.original_text}")
                if line.alternatives:
                    click.echo(f"     Alternatives: {len(line.alternatives)-1}")  # -1 to exclude original


@cli.command()
@click.argument('filepath', type=click.Path(exists=True))
def view_project(filepath: str):
    """Display the complete project structure."""
    manager = ProjectManager()
    project = manager.load_project(filepath)
    
    click.echo(f"Project: {project.metadata['title']}")
    click.echo("=" * 50)
    
    # Display metadata
    click.echo("\nMetadata:")
    for key, value in project.metadata.items():
        click.echo(f"  {key}: {value}")
    
    # Display parts and lines
    click.echo(f"\nParts ({len(project.song.parts)}):")
    for part in project.song.parts:
        attrs = ', '.join([f"{k}:{v}" for k, v in part.attributes.items()])
        attr_str = f" ({attrs})" if attrs else ""
        click.echo(f"\n  {part.name}{attr_str}:")
        
        for i, line in enumerate(part.lines):
            click.echo(f"    {i+1}. {line.original_text}")
            if line.alternatives and len(line.alternatives) > 1:
                click.echo(f"        Alternatives:")
                for alt in line.alternatives[1:]:  # Skip the original
                    status = " (FINAL)" if alt.is_final else ""
                    click.echo(f"          - {alt.text} [Score: {alt.rhyming_score:.2f}]{status}")
    
    # Display rhyming groups
    click.echo(f"\nRhyming Groups ({len(project.rhyming_groups)}):")
    for rg in project.rhyming_groups:
        click.echo(f"  {rg.group_id} ({rg.group_type.value}):")
        for member in rg.member_lines:
            click.echo(f"    - {member['part_name']}:{member['line_position']}")
    
    # Display combinations
    click.echo(f"\nCombinations ({len(project.combinations)}):")
    for combo in project.combinations:
        status = " (ACTIVE)" if combo.combination_id == project.active_combination_id else ""
        status = status if combo.is_final else " (FINAL)" if combo.is_final else status
        click.echo(f"  {combo.combination_id} [Score: {combo.total_rhyming_score:.2f}]{status}")


@cli.command(name='define-rhyme-group')
@click.argument('filepath', type=click.Path(exists=True))
@click.option('--group-id', prompt='Group ID', help='Unique identifier for the rhyming group')
@click.option('--type', 'rtype', type=click.Choice(['end', 'begin', 'inline']), 
              prompt='Rhyme type', help='Type of rhyme (end, begin, inline)')
@click.option('--lines', prompt='Lines (format: part:pos,part:pos)', 
              help='Comma-separated list of lines in format "part_name:position"')
@click.option('--metadata', multiple=True, help='Metadata in key=value format (can be used multiple times)')
def define_rhyme_group(filepath: str, group_id: str, rtype: str, lines: str, metadata: tuple):
    """Define a rhyming group specifying which lines should rhyme together."""
    manager = ProjectManager()
    project = manager.load_project(filepath)
    
    # Parse the lines string
    member_lines = []
    for line_spec in lines.split(','):
        part_name, pos_str = line_spec.strip().split(':')
        position = int(pos_str)
        member_lines.append({
            "part_name": part_name.strip(),
            "line_position": position
        })
    
    # Map rhyme type string to enum
    type_map = {
        'end': RhymingType.END_RHYME,
        'begin': RhymingType.BEGIN_RHYME,
        'inline': RhymingType.INLINE_RHYME
    }
    
    group_type = type_map[rtype]
    
    # Convert metadata tuples to dictionary
    metadata_dict = {}
    for item in metadata:
        if '=' in item:
            key, value = item.split('=', 1)
            metadata_dict[key] = value
    
    manager.define_rhyming_group(project, group_id, group_type, member_lines, metadata_dict)
    manager.save_project(project, filepath)
    
    click.echo(f"Defined rhyming group '{group_id}' with {len(member_lines)} member lines")


@cli.command()
@click.argument('filepath', type=click.Path(exists=True))
@click.option('--output', '-o', type=click.Path(), help='Output file path (default: stdout)')
def export_text(filepath: str, output: str):
    """Export the active combination as plain text."""
    manager = ProjectManager()
    project = manager.load_project(filepath)
    
    # Find the active combination
    active_combo = None
    if project.active_combination_id:
        for combo in project.combinations:
            if combo.combination_id == project.active_combination_id:
                active_combo = combo
                break
    
    if not active_combo:
        click.echo("No active combination found. Using original lines.")
        # If no active combination, just use original lines
        result_lines = []
        for part in project.song.parts:
            result_lines.append(f"[{part.name}]")
            for line in part.lines:
                result_lines.append(line.original_text)
            result_lines.append("")  # Empty line between parts
    else:
        # Use the selected alternatives from the active combination
        result_lines = []
        for part in project.song.parts:
            result_lines.append(f"[{part.name}]")
            for line_idx, line in enumerate(part.lines):
                # Create a key for this line to look up in the combination
                line_key = f"{part.name}:{line_idx}"
                
                if line_key in active_combo.line_selections:
                    alt_id = active_combo.line_selections[line_key]
                    # Find the alternative with this ID
                    selected_alt = None
                    for alt in line.alternatives:
                        if alt.id == alt_id:
                            selected_alt = alt
                            break
                    
                    if selected_alt:
                        result_lines.append(selected_alt.text)
                    else:
                        # Fallback to original if alternative not found
                        result_lines.append(line.original_text)
                else:
                    # If this line isn't in the combination, use original
                    result_lines.append(line.original_text)
            result_lines.append("")  # Empty line between parts
    
    output_text = '\n'.join(result_lines)
    
    if output:
        with open(output, 'w', encoding='utf-8') as f:
            f.write(output_text)
        click.echo(f"Exported text to {output}")
    else:
        click.echo(output_text)


@cli.command()
@click.argument('filepath', type=click.Path(exists=True))
@click.option('--part', default=None, help='Name of the part to get score for (get overall if not specified)')
def get_part_score(filepath: str, part: str):
    """Get the average rhyming score for a specific part or the entire project."""
    manager = ProjectManager()
    project = manager.load_project(filepath)
    
    if part:
        # Calculate score for specific part
        target_part = None
        for p in project.song.parts:
            if p.name == part:
                target_part = p
                break
        
        if not target_part:
            click.echo(f"Part '{part}' not found in project")
            return
        
        total_score = 0.0
        count = 0
        
        for line in target_part.lines:
            for alt in line.alternatives:
                if not (alt.text == line.original_text and alt.is_final):  # Skip original if it's final
                    total_score += alt.rhyming_score
                    count += 1
        
        avg_score = total_score / count if count > 0 else 0.0
        click.echo(f"Average rhyming score for part '{part}': {avg_score:.2f} (based on {count} alternatives)")
    else:
        # Calculate overall project score
        total_score = 0.0
        count = 0
        
        for part in project.song.parts:
            for line in part.lines:
                for alt in line.alternatives:
                    if not (alt.text == line.original_text and alt.is_final):  # Skip original if it's final
                        total_score += alt.rhyming_score
                        count += 1
        
        avg_score = total_score / count if count > 0 else 0.0
        click.echo(f"Average rhyming score for project: {avg_score:.2f} (based on {count} alternatives)")


@cli.command()
@click.argument('filepath', type=click.Path(exists=True))
@click.option('--combination-id', default=None, help='ID of the combination to get score for (get active if not specified)')
def get_combination_score(filepath: str, combination_id: str):
    """Get the total rhyming score for a specific combination."""
    manager = ProjectManager()
    project = manager.load_project(filepath)
    
    target_combo = None
    
    if combination_id:
        # Find the specified combination
        for combo in project.combinations:
            if combo.combination_id == combination_id:
                target_combo = combo
                break
        
        if not target_combo:
            click.echo(f"Combination '{combination_id}' not found in project")
            return
    else:
        # Use the active combination
        if project.active_combination_id:
            for combo in project.combinations:
                if combo.combination_id == project.active_combination_id:
                    target_combo = combo
                    break
        
        if not target_combo:
            click.echo("No active combination found in project")
            return
    
    click.echo(f"Combination '{target_combo.combination_id}' total rhyming score: {target_combo.total_rhyming_score:.2f}")
    
    # Also show custom scores if any
    if target_combo.custom_scores:
        click.echo("Custom scores:")
        for score_name, score_value in target_combo.custom_scores.items():
            click.echo(f"  {score_name}: {score_value}")


@cli.command()
@click.argument('filepath', type=click.Path(exists=True))
@click.option('--group-id', prompt='Rhyming group ID', help='ID of the rhyming group to get score for')
def get_rhyming_group_score(filepath: str, group_id: str):
    """Get the rhyming score for a specific rhyming group."""
    manager = ProjectManager()
    project = manager.load_project(filepath)
    
    # Find the rhyming group
    target_group = None
    for rg in project.rhyming_groups:
        if rg.group_id == group_id:
            target_group = rg
            break
    
    if not target_group:
        click.echo(f"Rhyming group '{group_id}' not found in project")
        return
    
    click.echo(f"Rhyming group '{group_id}' ({target_group.group_type.value}):")
    for member in target_group.member_lines:
        part_name = member['part_name']
        line_pos = member['line_position']
        
        # Find the line
        target_part = None
        for part in project.song.parts:
            if part.name == part_name:
                target_part = part
                break
        
        if target_part and line_pos < len(target_part.lines):
            line = target_part.lines[line_pos]
            click.echo(f"  {part_name}:{line_pos} - '{line.original_text[:50]}{'...' if len(line.original_text) > 50 else ''}'")
        else:
            click.echo(f"  {part_name}:{line_pos} - (line not found)")
    
    # Note: Actual rhyming score calculation for the group would require more complex logic
    # that compares the lines in the group, which is not implemented here
    click.echo(f"  Note: Actual group rhyming score calculation would require comparing the lines in the group")


@cli.command()
@click.argument('filepath', type=click.Path(exists=True))
@click.option('--part', default=None, help='Name of the part to dump (dump entire project if not specified)')
def dump_project(filepath: str, part: str):
    """Get a text dump of the project or a specific part."""
    manager = ProjectManager()
    project = manager.load_project(filepath)
    
    if part:
        # Dump specific part
        target_part = None
        for p in project.song.parts:
            if p.name == part:
                target_part = p
                break
        
        if not target_part:
            click.echo(f"Part '{part}' not found in project")
            return
        
        result_lines = [f"[{target_part.name}]"]
        for line_idx, line in enumerate(target_part.lines):
            result_lines.append(f"  {line_idx+1}. {line.original_text}")
            if line.metadata:
                result_lines.append(f"      Metadata: {line.metadata}")
            if line.alternatives and len(line.alternatives) > 1:
                result_lines.append("      Alternatives:")
                for alt_idx, alt in enumerate(line.alternatives[1:], 1):  # Skip original
                    result_lines.append(f"        {alt_idx}. {alt.text} [Score: {alt.rhyming_score:.2f}]")
                    if alt.metadata:
                        result_lines.append(f"            Metadata: {alt.metadata}")
                    if alt.custom_scores:
                        result_lines.append(f"            Custom scores: {alt.custom_scores}")
    else:
        # Dump entire project
        result_lines = [f"Project: {project.metadata.get('title', 'Untitled')}"]
        result_lines.append("=" * 50)
        
        # Project metadata
        result_lines.append("\nProject Metadata:")
        for key, value in project.metadata.items():
            result_lines.append(f"  {key}: {value}")
        
        # Parts and lines
        result_lines.append(f"\nParts ({len(project.song.parts)}):")
        for part in project.song.parts:
            attrs = ', '.join([f"{k}:{v}" for k, v in part.attributes.items()])
            attr_str = f" ({attrs})" if attrs else ""
            result_lines.append(f"\n  {part.name}{attr_str}:")
            
            for line_idx, line in enumerate(part.lines):
                result_lines.append(f"    {line_idx+1}. {line.original_text}")
                if line.metadata:
                    result_lines.append(f"        Metadata: {line.metadata}")
                if line.syllable_count is not None:
                    result_lines.append(f"        Syllable count: {line.syllable_count}")
                if line.alternatives and len(line.alternatives) > 1:
                    result_lines.append("        Alternatives:")
                    for alt_idx, alt in enumerate(line.alternatives[1:], 1):  # Skip original
                        result_lines.append(f"          {alt_idx}. {alt.text} [Score: {alt.rhyming_score:.2f}]")
                        if alt.metadata:
                            result_lines.append(f"              Metadata: {alt.metadata}")
                        if alt.custom_scores:
                            result_lines.append(f"              Custom scores: {alt.custom_scores}")
        
        # Rhyming groups
        result_lines.append(f"\nRhyming Groups ({len(project.rhyming_groups)}):")
        for rg in project.rhyming_groups:
            result_lines.append(f"  {rg.group_id} ({rg.group_type.value}):")
            for member in rg.member_lines:
                result_lines.append(f"    - {member['part_name']}:{member['line_position']}")
            if rg.metadata:
                result_lines.append(f"    Metadata: {rg.metadata}")
        
        # Combinations
        result_lines.append(f"\nCombinations ({len(project.combinations)}):")
        for combo in project.combinations:
            status = " (ACTIVE)" if combo.combination_id == project.active_combination_id else ""
            status = status if combo.is_final else " (FINAL)" if combo.is_final else status
            result_lines.append(f"  {combo.combination_id} [Score: {combo.total_rhyming_score:.2f}]{status}")
            if combo.custom_scores:
                result_lines.append(f"    Custom scores: {combo.custom_scores}")
            if combo.metadata:
                result_lines.append(f"    Metadata: {combo.metadata}")
    
    # Print the dump
    for line in result_lines:
        click.echo(line)


if __name__ == '__main__':
    cli()