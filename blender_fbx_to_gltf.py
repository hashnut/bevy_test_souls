#!/usr/bin/env python3
"""
Blender script to convert FBX files to GLTF/GLB format
Usage: blender --background --python blender_fbx_to_gltf.py -- --input_dir assets/Animations --output_dir assets/Animations_GLTF --format glb
"""

import bpy
import os
import sys
import argparse
from pathlib import Path

def clear_scene():
    """Clear all objects from the scene"""
    bpy.ops.object.select_all(action='SELECT')
    bpy.ops.object.delete(use_global=False)
    
    # Clear all collections except the main one
    for collection in bpy.data.collections:
        bpy.data.collections.remove(collection)
    
    # Clear orphan data
    for block in bpy.data.meshes:
        if block.users == 0:
            bpy.data.meshes.remove(block)
    
    for block in bpy.data.materials:
        if block.users == 0:
            bpy.data.materials.remove(block)
    
    for block in bpy.data.textures:
        if block.users == 0:
            bpy.data.textures.remove(block)
    
    for block in bpy.data.images:
        if block.users == 0:
            bpy.data.images.remove(block)

def convert_fbx_to_gltf(input_file, output_file, export_format='GLB'):
    """Convert a single FBX file to GLTF/GLB"""
    try:
        # Clear the scene
        clear_scene()
        
        # Import FBX
        bpy.ops.import_scene.fbx(filepath=str(input_file))
        
        # Set up export settings (updated for Blender 4.5)
        export_settings = {
            'filepath': str(output_file),
            'export_format': export_format,
            'export_animations': True,
            'export_apply': True,
            'export_texcoords': True,
            'export_normals': True,
            'export_materials': 'EXPORT',
            'export_image_format': 'AUTO',
            'export_extras': True,
            'export_yup': True,
            'export_skins': True,
            'export_morph': True,
            'export_lights': False,
            'export_cameras': False,
            'use_selection': False,
            'export_draco_mesh_compression_enable': False
        }
        
        # Export to GLTF/GLB
        bpy.ops.export_scene.gltf(**export_settings)
        
        return True, f"Converted: {input_file.name} -> {output_file.name}"
    
    except Exception as e:
        return False, f"Error converting {input_file.name}: {str(e)}"

def main():
    # Get command line arguments after --
    argv = sys.argv
    if "--" in argv:
        argv = argv[argv.index("--") + 1:]
    else:
        argv = []
    
    parser = argparse.ArgumentParser(description='Convert FBX files to GLTF/GLB using Blender')
    parser.add_argument('--input_dir', default='assets/Animations', help='Input directory containing FBX files')
    parser.add_argument('--output_dir', default='assets/Animations_GLTF', help='Output directory for GLTF/GLB files')
    parser.add_argument('--format', default='glb', choices=['gltf', 'glb'], help='Output format (gltf or glb)')
    parser.add_argument('--skip_existing', action='store_true', help='Skip files that already exist')
    
    args = parser.parse_args(argv)
    
    # Convert paths to Path objects
    input_dir = Path(args.input_dir)
    output_dir = Path(args.output_dir)
    export_format = 'GLB' if args.format.lower() == 'glb' else 'GLTF_SEPARATE'
    extension = '.glb' if args.format.lower() == 'glb' else '.gltf'
    
    # Create output directories
    output_dir.mkdir(parents=True, exist_ok=True)
    (output_dir / 'Aurora').mkdir(exist_ok=True)
    (output_dir / 'Great_Sword').mkdir(exist_ok=True)
    
    # Statistics
    converted = 0
    skipped = 0
    errors = 0
    results = []
    
    print("=" * 50)
    print("FBX to GLTF/GLB Converter (Blender)")
    print("=" * 50)
    print(f"Input directory: {input_dir}")
    print(f"Output directory: {output_dir}")
    print(f"Format: {args.format.upper()}")
    print("=" * 50)
    
    # Process Aurora animations
    aurora_dir = input_dir / 'Aurora'
    if aurora_dir.exists():
        print("\nConverting Aurora animations...")
        print("-" * 30)
        
        for fbx_file in aurora_dir.glob('*.FBX'):
            output_file = output_dir / 'Aurora' / (fbx_file.stem + extension)
            
            if args.skip_existing and output_file.exists():
                print(f"  SKIPPED: {fbx_file.name} (already exists)")
                skipped += 1
                results.append(f"Aurora/{fbx_file.stem} - Skipped (exists)")
                continue
            
            success, message = convert_fbx_to_gltf(fbx_file, output_file, export_format)
            
            if success:
                print(f"  SUCCESS: {message}")
                converted += 1
                results.append(f"Aurora/{fbx_file.stem} - Converted")
            else:
                print(f"  ERROR: {message}")
                errors += 1
                results.append(f"Aurora/{fbx_file.stem} - Failed")
    
    # Process Great Sword animations
    sword_dir = input_dir / 'Great_Sword'
    if sword_dir.exists():
        print("\nConverting Great Sword animations...")
        print("-" * 30)
        
        for fbx_file in sword_dir.glob('*.fbx'):
            output_file = output_dir / 'Great_Sword' / (fbx_file.stem + extension)
            
            if args.skip_existing and output_file.exists():
                print(f"  SKIPPED: {fbx_file.name} (already exists)")
                skipped += 1
                results.append(f"Great_Sword/{fbx_file.stem} - Skipped (exists)")
                continue
            
            success, message = convert_fbx_to_gltf(fbx_file, output_file, export_format)
            
            if success:
                print(f"  SUCCESS: {message}")
                converted += 1
                results.append(f"Great_Sword/{fbx_file.stem} - Converted")
            else:
                print(f"  ERROR: {message}")
                errors += 1
                results.append(f"Great_Sword/{fbx_file.stem} - Failed")
    
    # Summary
    print("\n" + "=" * 50)
    print("Conversion Complete!")
    print("=" * 50)
    print(f"Total files converted: {converted}")
    print(f"Total files skipped: {skipped}")
    print(f"Total errors: {errors}")
    
    # Create log file
    log_file = output_dir / 'conversion_log.txt'
    with open(log_file, 'w', encoding='utf-8') as f:
        f.write("FBX to GLTF/GLB Conversion Log (Blender)\n")
        f.write("=" * 50 + "\n\n")
        f.write(f"Settings:\n")
        f.write(f"- Input Directory: {input_dir}\n")
        f.write(f"- Output Directory: {output_dir}\n")
        f.write(f"- Format: {args.format.upper()}\n")
        f.write(f"- Skip Existing: {args.skip_existing}\n\n")
        f.write(f"Results:\n")
        f.write(f"- Total files converted: {converted}\n")
        f.write(f"- Total files skipped: {skipped}\n")
        f.write(f"- Total errors: {errors}\n\n")
        f.write("File Details:\n")
        f.write("\n".join(results))
    
    print(f"\nLog file created: {log_file}")
    
    # Create animation mapping JSON
    import json
    
    mapping_file = output_dir / 'animation_mapping.json'
    animation_mapping = {
        "Aurora": {
            "idle": f"Idle{extension}",
            "walk": f"Jog_Fwd{extension}",
            "run": f"Jog_Fwd{extension}",
            "walk_combat": f"Jog_Fwd_Combat{extension}",
            "roll_forward": f"Ability_RMB_Fwd{extension}",
            "roll_backward": f"Ability_RMB_Bwd{extension}",
            "roll_left": f"Ability_RMB_Left{extension}",
            "roll_right": f"Ability_RMB_Right{extension}",
            "jump_start": f"Jump_Start{extension}",
            "jump_apex": f"Jump_Apex{extension}",
            "jump_land": f"Jump_Land{extension}",
            "attack_1": f"Primary_Attack_A{extension}",
            "attack_2": f"Primary_Attack_B{extension}",
            "attack_3": f"Primary_Attack_C{extension}",
            "parry": f"HitReact_Front{extension}",
            "death": f"Death{extension}"
        },
        "GreatSword": {
            "idle": f"Great Sword Idle{extension}",
            "walk": f"Great Sword Walk{extension}",
            "run": f"Great Sword Run{extension}",
            "slash": f"Great Sword Slash{extension}"
        }
    }
    
    with open(mapping_file, 'w', encoding='utf-8') as f:
        json.dump(animation_mapping, f, indent=2)
    
    print(f"Animation mapping file created: {mapping_file}")

if __name__ == "__main__":
    main()