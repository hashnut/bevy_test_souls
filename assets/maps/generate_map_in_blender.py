import bpy
import math

# Clear existing mesh objects
bpy.ops.object.select_all(action='SELECT')
bpy.ops.object.delete(use_global=False)

# Create a new collection for the map
map_collection = bpy.data.collections.new("DarkRustMap")
bpy.context.scene.collection.children.link(map_collection)

# Function to create materials
def create_material(name, color, roughness=0.8, metallic=0.1):
    mat = bpy.data.materials.new(name=name)
    mat.use_nodes = True
    bsdf = mat.node_tree.nodes["Principled BSDF"]
    bsdf.inputs[0].default_value = (*color, 1.0)  # Base Color
    bsdf.inputs[2].default_value = roughness      # Roughness
    bsdf.inputs[1].default_value = metallic       # Metallic
    return mat

# Create materials
floor_mat = create_material("Floor", (0.3, 0.3, 0.3), 0.8, 0.1)
wall_mat = create_material("Wall", (0.2, 0.2, 0.25), 0.9, 0.0)
pillar_mat = create_material("Pillar", (0.25, 0.25, 0.3), 0.7, 0.2)
checkpoint_mat = create_material("Checkpoint", (0.8, 0.6, 0.2), 0.3, 0.8)

# Map parameters
map_size = 30.0
tile_size = 5.0
wall_height = 4.0
wall_thickness = 0.5

# Create floor tiles
floor_parent = bpy.data.objects.new("Floors", None)
map_collection.objects.link(floor_parent)

for x in range(6):
    for z in range(6):
        x_pos = (x - 2.5) * tile_size
        z_pos = (z - 2.5) * tile_size
        
        # Create floor tile
        bpy.ops.mesh.primitive_cube_add(
            size=1,
            location=(x_pos, z_pos, -0.1)
        )
        floor = bpy.context.active_object
        floor.name = f"Floor_{x}_{z}"
        floor.scale = (tile_size/2, tile_size/2, 0.1)
        floor.data.materials.append(floor_mat)
        floor.parent = floor_parent
        
        # Move to collection
        bpy.context.scene.collection.objects.unlink(floor)
        map_collection.objects.link(floor)

# Create walls
walls_parent = bpy.data.objects.new("Walls", None)
map_collection.objects.link(walls_parent)

# North walls
for x in range(6):
    x_pos = (x - 2.5) * tile_size
    bpy.ops.mesh.primitive_cube_add(
        size=1,
        location=(x_pos, -map_size/2, wall_height/2)
    )
    wall = bpy.context.active_object
    wall.name = f"Wall_North_{x}"
    wall.scale = (tile_size/2, wall_thickness/2, wall_height/2)
    wall.data.materials.append(wall_mat)
    wall.parent = walls_parent
    bpy.context.scene.collection.objects.unlink(wall)
    map_collection.objects.link(wall)

# South walls
for x in range(6):
    x_pos = (x - 2.5) * tile_size
    bpy.ops.mesh.primitive_cube_add(
        size=1,
        location=(x_pos, map_size/2, wall_height/2)
    )
    wall = bpy.context.active_object
    wall.name = f"Wall_South_{x}"
    wall.scale = (tile_size/2, wall_thickness/2, wall_height/2)
    wall.data.materials.append(wall_mat)
    wall.parent = walls_parent
    bpy.context.scene.collection.objects.unlink(wall)
    map_collection.objects.link(wall)

# East walls
for z in range(6):
    z_pos = (z - 2.5) * tile_size
    bpy.ops.mesh.primitive_cube_add(
        size=1,
        location=(map_size/2, z_pos, wall_height/2)
    )
    wall = bpy.context.active_object
    wall.name = f"Wall_East_{z}"
    wall.scale = (wall_thickness/2, tile_size/2, wall_height/2)
    wall.data.materials.append(wall_mat)
    wall.parent = walls_parent
    bpy.context.scene.collection.objects.unlink(wall)
    map_collection.objects.link(wall)

# West walls
for z in range(6):
    z_pos = (z - 2.5) * tile_size
    bpy.ops.mesh.primitive_cube_add(
        size=1,
        location=(-map_size/2, z_pos, wall_height/2)
    )
    wall = bpy.context.active_object
    wall.name = f"Wall_West_{z}"
    wall.scale = (wall_thickness/2, tile_size/2, wall_height/2)
    wall.data.materials.append(wall_mat)
    wall.parent = walls_parent
    bpy.context.scene.collection.objects.unlink(wall)
    map_collection.objects.link(wall)

# Create pillars
pillars_parent = bpy.data.objects.new("Pillars", None)
map_collection.objects.link(pillars_parent)

pillar_positions = [
    (-10.0, -10.0),
    (10.0, -10.0),
    (-10.0, 10.0),
    (10.0, 10.0),
    (0.0, 0.0),
]

for i, (x, y) in enumerate(pillar_positions):
    bpy.ops.mesh.primitive_cylinder_add(
        radius=0.5,
        depth=wall_height,
        location=(x, y, wall_height/2)
    )
    pillar = bpy.context.active_object
    pillar.name = f"Pillar_{i}"
    pillar.data.materials.append(pillar_mat)
    pillar.parent = pillars_parent
    bpy.context.scene.collection.objects.unlink(pillar)
    map_collection.objects.link(pillar)

# Create obstacles
obstacles_parent = bpy.data.objects.new("Obstacles", None)
map_collection.objects.link(obstacles_parent)

obstacle_positions = [
    (-5.0, -5.0),
    (5.0, 5.0),
    (-7.5, 2.5),
    (7.5, -2.5),
]

for i, (x, y) in enumerate(obstacle_positions):
    bpy.ops.mesh.primitive_cube_add(
        size=1,
        location=(x, y, 0.75)
    )
    obstacle = bpy.context.active_object
    obstacle.name = f"Obstacle_{i}"
    obstacle.scale = (1.0, 1.0, 0.75)
    obstacle.data.materials.append(wall_mat)
    obstacle.parent = obstacles_parent
    bpy.context.scene.collection.objects.unlink(obstacle)
    map_collection.objects.link(obstacle)

# Create checkpoint
bpy.ops.mesh.primitive_cube_add(
    size=1,
    location=(-10.0, 0.0, 1.0)
)
checkpoint = bpy.context.active_object
checkpoint.name = "Checkpoint_1"
checkpoint.scale = (0.25, 0.25, 1.0)
checkpoint.data.materials.append(checkpoint_mat)
bpy.context.scene.collection.objects.unlink(checkpoint)
map_collection.objects.link(checkpoint)

# Add lights
lights_parent = bpy.data.objects.new("Lights", None)
map_collection.objects.link(lights_parent)

# Main light
bpy.ops.object.light_add(
    type='POINT',
    location=(0.0, 0.0, 5.0)
)
main_light = bpy.context.active_object
main_light.name = "MainLight"
main_light.data.energy = 500
main_light.data.color = (1.0, 0.8, 0.6)
main_light.parent = lights_parent
bpy.context.scene.collection.objects.unlink(main_light)
map_collection.objects.link(main_light)

# Checkpoint light
bpy.ops.object.light_add(
    type='POINT',
    location=(-10.0, 0.0, 3.0)
)
checkpoint_light = bpy.context.active_object
checkpoint_light.name = "CheckpointLight"
checkpoint_light.data.energy = 200
checkpoint_light.data.color = (0.8, 0.6, 0.2)
checkpoint_light.parent = lights_parent
bpy.context.scene.collection.objects.unlink(checkpoint_light)
map_collection.objects.link(checkpoint_light)

# Create empty objects for spawn points
spawn_points_parent = bpy.data.objects.new("SpawnPoints", None)
map_collection.objects.link(spawn_points_parent)

# Player spawn point
bpy.ops.object.empty_add(
    type='ARROWS',
    location=(0.0, 0.0, 0.5)
)
player_spawn = bpy.context.active_object
player_spawn.name = "PlayerSpawnPoint"
player_spawn.empty_display_size = 0.5
player_spawn.parent = spawn_points_parent
bpy.context.scene.collection.objects.unlink(player_spawn)
map_collection.objects.link(player_spawn)

# Enemy spawn point
bpy.ops.object.empty_add(
    type='ARROWS',
    location=(5.0, 0.0, 0.5)
)
enemy_spawn = bpy.context.active_object
enemy_spawn.name = "EnemySpawnPoint"
enemy_spawn.empty_display_size = 0.5
enemy_spawn.parent = spawn_points_parent
bpy.context.scene.collection.objects.unlink(enemy_spawn)
map_collection.objects.link(enemy_spawn)

# Select all objects in the map collection
for obj in map_collection.all_objects:
    obj.select_set(True)

# Export as GLB
import os
export_path = os.path.join(os.path.dirname(bpy.data.filepath), "dark_rust_map.glb")
bpy.ops.export_scene.gltf(
    filepath=export_path,
    export_format='GLB',
    use_selection=True,
    export_apply=True
)

print(f"Map exported successfully to: {export_path}")
print("Move the generated 'dark_rust_map.glb' file to your Bevy project's 'assets/models/' folder")