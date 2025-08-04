# 3D Model Setup

## Required Model

Place a humanoid character model in this directory with the filename `character.glb` or `character.gltf`.

## Free Model Resources

You can download free humanoid models from:

1. **Mixamo** (https://www.mixamo.com/)
   - Free animated characters
   - Download as FBX and convert to GLTF using Blender

2. **Quaternius** (https://quaternius.com/)
   - Low-poly character models
   - CC0 license (free for any use)

3. **OpenGameArt** (https://opengameart.org/)
   - Various free character models
   - Check individual licenses

## Model Requirements

- Format: GLTF 2.0 (.glb or .gltf)
- Should have a skeleton/armature
- Recommended: Include basic animations (idle, walk, run, attack)
- Scale: Approximately 1.8 units tall in Bevy coordinates

## Quick Setup with Quaternius Models

1. Download from: https://quaternius.com/packs/ultimateanimatedcharacter.html
2. Extract and find a .glb file
3. Rename to `character.glb`
4. Place in this directory (`assets/models/`)

## Converting FBX to GLTF

If you have an FBX model:
1. Open Blender
2. Import the FBX file
3. Export as GLTF 2.0 (.glb)