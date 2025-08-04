#!/usr/bin/env python3
"""
FBX to GLTF Animation Converter
Converts all FBX animation files to GLTF/GLB format
"""

import os
import sys
import subprocess
import json
import argparse
from pathlib import Path
from datetime import datetime

class AnimationConverter:
    def __init__(self, source_dir="assets/Animations", output_dir="assets/Animations_GLTF"):
        self.source_dir = Path(source_dir)
        self.output_dir = Path(output_dir)
        self.converted = 0
        self.skipped = 0
        self.errors = 0
        self.results = []
        
    def check_fbx2gltf(self):
        """Check if fbx2gltf is installed"""
        try:
            subprocess.run(["fbx2gltf", "--version"], capture_output=True, check=True)
            return True
        except (subprocess.CalledProcessError, FileNotFoundError):
            print("ERROR: fbx2gltf is not installed or not in PATH")
            print("Please install it using: npm install -g fbx2gltf")
            print("Or download from: https://github.com/facebookincubator/FBX2glTF")
            return False
    
    def create_directories(self):
        """Create output directory structure"""
        dirs = [
            self.output_dir,
            self.output_dir / "Aurora",
            self.output_dir / "Great_Sword"
        ]
        
        for dir_path in dirs:
            dir_path.mkdir(parents=True, exist_ok=True)
            print(f"Created directory: {dir_path}")
    
    def convert_file(self, input_file, output_file, binary=False, skip_existing=False):
        """Convert a single FBX file to GLTF/GLB"""
        # Check if output already exists
        if skip_existing and output_file.exists():
            print(f"  SKIPPED: {output_file.name} already exists")
            return "skipped"
        
        # Build command
        cmd = ["fbx2gltf", str(input_file), "-o", str(output_file), "--embed-images"]
        if binary:
            cmd.append("--binary")
        
        try:
            # Run conversion
            result = subprocess.run(cmd, capture_output=True, text=True)
            
            if result.returncode == 0:
                print(f"  SUCCESS: Created {output_file.name}")
                return "success"
            else:
                print(f"  ERROR: Failed to convert (Exit code: {result.returncode})")
                if result.stderr:
                    print(f"  Error details: {result.stderr.strip()}")
                return "error"
                
        except Exception as e:
            print(f"  ERROR: {str(e)}")
            return "error"
    
    def convert_directory(self, subdir, pattern, binary=False, skip_existing=False):
        """Convert all FBX files in a directory"""
        source_path = self.source_dir / subdir
        output_path = self.output_dir / subdir
        extension = "glb" if binary else "gltf"
        
        print(f"\nConverting {subdir} animations...")
        print("-" * 40)
        
        if not source_path.exists():
            print(f"  WARNING: Source directory {source_path} does not exist")
            return
        
        # Find all FBX files
        fbx_files = list(source_path.glob(pattern))
        
        if not fbx_files:
            print(f"  No {pattern} files found in {source_path}")
            return
        
        for fbx_file in fbx_files:
            output_file = output_path / f"{fbx_file.stem}.{extension}"
            print(f"Converting: {fbx_file.name}")
            
            result = self.convert_file(fbx_file, output_file, binary, skip_existing)
            
            if result == "success":
                self.converted += 1
                self.results.append(f"{subdir}/{fbx_file.stem} - Converted")
            elif result == "skipped":
                self.skipped += 1
                self.results.append(f"{subdir}/{fbx_file.stem} - Skipped (exists)")
            else:
                self.errors += 1
                self.results.append(f"{subdir}/{fbx_file.stem} - Failed")
            
            print()
    
    def create_animation_mapping(self, extension="gltf"):
        """Create a JSON mapping file for animations"""
        mapping = {
            "Aurora": {
                "idle": f"Idle.{extension}",
                "walk": f"Jog_Fwd.{extension}",
                "run": f"Jog_Fwd.{extension}",
                "walk_combat": f"Jog_Fwd_Combat.{extension}",
                "roll_forward": f"Ability_RMB_Fwd.{extension}",
                "roll_backward": f"Ability_RMB_Bwd.{extension}",
                "roll_left": f"Ability_RMB_Left.{extension}",
                "roll_right": f"Ability_RMB_Right.{extension}",
                "jump_start": f"Jump_Start.{extension}",
                "jump_apex": f"Jump_Apex.{extension}",
                "jump_land": f"Jump_Land.{extension}",
                "jump_recovery": f"Jump_Recovery.{extension}",
                "attack_1": f"Primary_Attack_A.{extension}",
                "attack_2": f"Primary_Attack_B.{extension}",
                "attack_3": f"Primary_Attack_C.{extension}",
                "attack_fast": f"Primary_Attack_Fast_A.{extension}",
                "attack_slow": f"Primary_Attack_Slow_A.{extension}",
                "parry": f"HitReact_Front.{extension}",
                "death": f"Death.{extension}",
                "hit_front": f"HitReact_Front.{extension}",
                "hit_back": f"HitReact_Back.{extension}",
                "hit_left": f"HitReact_Left.{extension}",
                "hit_right": f"HitReact_Right.{extension}"
            },
            "GreatSword": {
                "idle": f"Great Sword Idle.{extension}",
                "walk": f"Great Sword Walk.{extension}",
                "run": f"Great Sword Run.{extension}",
                "slash": f"Great Sword Slash.{extension}",
                "kick": f"Great Sword Kick.{extension}",
                "impact": f"Great Sword Impact.{extension}",
                "sheath": f"Great Sword Sheath.{extension}",
                "strafe": f"Great Sword Strafe.{extension}",
                "turn": f"Great Sword Turn.{extension}"
            }
        }
        
        mapping_file = self.output_dir / "animation_mapping.json"
        with open(mapping_file, 'w', encoding='utf-8') as f:
            json.dump(mapping, f, indent=2)
        
        print(f"Animation mapping file created: {mapping_file}")
        return mapping
    
    def create_log(self, binary=False, skip_existing=False):
        """Create a detailed log file"""
        extension = "glb" if binary else "gltf"
        log_content = f"""FBX to GLTF Conversion Log
Generated: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}
=======================================

Settings:
- Source Directory: {self.source_dir}
- Output Directory: {self.output_dir}
- Format: {extension.upper()}
- Skip Existing: {skip_existing}

Results:
- Total files converted: {self.converted}
- Total files skipped: {self.skipped}
- Total errors: {self.errors}

File Details:
{chr(10).join(self.results)}

Converted Files:
"""
        # List all converted files
        for ext in ['gltf', 'glb']:
            for file in self.output_dir.rglob(f"*.{ext}"):
                log_content += f"{file}\n"
        
        log_file = self.output_dir / "conversion_log.txt"
        with open(log_file, 'w', encoding='utf-8') as f:
            f.write(log_content)
        
        print(f"\nLog file created: {log_file}")
    
    def convert_all(self, binary=False, skip_existing=False):
        """Main conversion process"""
        print("=" * 50)
        print("FBX to GLTF Animation Converter")
        print("=" * 50)
        
        # Check fbx2gltf
        if not self.check_fbx2gltf():
            return False
        
        # Create directories
        self.create_directories()
        
        # Convert Aurora animations
        self.convert_directory("Aurora", "*.FBX", binary, skip_existing)
        
        # Convert Great Sword animations
        self.convert_directory("Great_Sword", "*.fbx", binary, skip_existing)
        
        # Summary
        print("=" * 50)
        print("Conversion Complete!")
        print("=" * 50)
        print(f"Total files converted: {self.converted}")
        print(f"Total files skipped: {self.skipped}")
        print(f"Total errors: {self.errors}")
        print(f"\nOutput directory: {self.output_dir}")
        
        # Create mapping and log
        extension = "glb" if binary else "gltf"
        self.create_animation_mapping(extension)
        self.create_log(binary, skip_existing)
        
        return self.errors == 0

def main():
    parser = argparse.ArgumentParser(description="Convert FBX animations to GLTF/GLB format")
    parser.add_argument("-s", "--source", default="assets/Animations", 
                        help="Source directory containing FBX files")
    parser.add_argument("-o", "--output", default="assets/Animations_GLTF", 
                        help="Output directory for GLTF/GLB files")
    parser.add_argument("-b", "--binary", action="store_true", 
                        help="Output binary GLB format instead of GLTF")
    parser.add_argument("--skip-existing", action="store_true", 
                        help="Skip files that already exist")
    
    args = parser.parse_args()
    
    converter = AnimationConverter(args.source, args.output)
    success = converter.convert_all(args.binary, args.skip_existing)
    
    sys.exit(0 if success else 1)

if __name__ == "__main__":
    main()