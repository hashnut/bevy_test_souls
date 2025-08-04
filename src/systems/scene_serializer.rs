use bevy::prelude::*;
use serde::{Serialize, Deserialize};
use std::fs;

// 씬 데이터를 저장하기 위한 구조체들
#[derive(Serialize, Deserialize, Debug)]
pub struct SerializedScene {
    pub name: String,
    pub entities: Vec<SerializedEntity>,
}

#[derive(Serialize, Deserialize, Debug)]
pub struct SerializedEntity {
    pub name: String,
    pub transform: SerializedTransform,
    pub entity_type: EntityType,
}

#[derive(Serialize, Deserialize, Debug)]
pub struct SerializedTransform {
    pub translation: [f32; 3],
    pub rotation: [f32; 4],
    pub scale: [f32; 3],
}

#[derive(Component, Serialize, Deserialize, Debug, Clone)]
pub enum EntityType {
    Floor { size: [f32; 3] },
    Wall { size: [f32; 3] },
    Pillar { radius: f32, height: f32 },
    Obstacle { size: [f32; 3] },
    Light { color: [f32; 3], intensity: f32 },
    SpawnPoint { spawn_type: String },
}

// 현재 씬을 JSON으로 저장
pub fn save_scene_to_json(
    entities: Query<(&Name, &Transform, &EntityType)>,
) {
    let mut serialized_entities = Vec::new();
    
    for (name, transform, entity_type) in entities.iter() {
        let serialized_entity = SerializedEntity {
            name: name.to_string(),
            transform: SerializedTransform {
                translation: transform.translation.to_array(),
                rotation: transform.rotation.to_array(),
                scale: transform.scale.to_array(),
            },
            entity_type: entity_type.clone(),
        };
        serialized_entities.push(serialized_entity);
    }
    
    let scene = SerializedScene {
        name: "procedural_map".to_string(),
        entities: serialized_entities,
    };
    
    // JSON으로 저장
    match serde_json::to_string_pretty(&scene) {
        Ok(json) => {
            if let Err(e) = fs::write("assets/maps/procedural_map.json", json) {
                error!("Failed to save scene: {}", e);
            } else {
                info!("Scene saved to assets/maps/procedural_map.json");
            }
        }
        Err(e) => error!("Failed to serialize scene: {}", e),
    }
}

// JSON에서 씬 로드
pub fn load_scene_from_json(
    mut commands: Commands,
    mut meshes: ResMut<Assets<Mesh>>,
    mut materials: ResMut<Assets<StandardMaterial>>,
) {
    let json_path = "assets/maps/procedural_map.json";
    
    match fs::read_to_string(json_path) {
        Ok(json) => {
            match serde_json::from_str::<SerializedScene>(&json) {
                Ok(scene) => {
                    info!("Loading scene: {}", scene.name);
                    spawn_entities_from_data(scene, &mut commands, &mut meshes, &mut materials);
                }
                Err(e) => error!("Failed to parse scene JSON: {}", e),
            }
        }
        Err(e) => error!("Failed to read scene file: {}", e),
    }
}

fn spawn_entities_from_data(
    scene: SerializedScene,
    commands: &mut Commands,
    meshes: &mut ResMut<Assets<Mesh>>,
    materials: &mut ResMut<Assets<StandardMaterial>>,
) {
    let default_material = materials.add(StandardMaterial {
        base_color: Color::srgb(0.5, 0.5, 0.5),
        ..default()
    });
    
    for entity_data in scene.entities {
        let transform = Transform {
            translation: Vec3::from_array(entity_data.transform.translation),
            rotation: Quat::from_array(entity_data.transform.rotation),
            scale: Vec3::from_array(entity_data.transform.scale),
        };
        
        match entity_data.entity_type {
            EntityType::Floor { size } => {
                commands.spawn((
                    Mesh3d(meshes.add(Cuboid::new(size[0], size[1], size[2]))),
                    MeshMaterial3d(default_material.clone()),
                    transform,
                    Name::new(entity_data.name),
                ));
            }
            EntityType::Wall { size } => {
                commands.spawn((
                    Mesh3d(meshes.add(Cuboid::new(size[0], size[1], size[2]))),
                    MeshMaterial3d(default_material.clone()),
                    transform,
                    Name::new(entity_data.name),
                ));
            }
            EntityType::Pillar { radius, height } => {
                commands.spawn((
                    Mesh3d(meshes.add(Cylinder::new(radius, height))),
                    MeshMaterial3d(default_material.clone()),
                    transform,
                    Name::new(entity_data.name),
                ));
            }
            EntityType::Light { color, intensity } => {
                commands.spawn((
                    PointLight {
                        color: Color::srgb(color[0], color[1], color[2]),
                        intensity,
                        ..default()
                    },
                    transform,
                    Name::new(entity_data.name),
                ));
            }
            _ => {}
        }
    }
}

// GLTF Export를 위한 대안: Blender Python 스크립트 생성
pub fn generate_blender_script(
    entities: Query<(&Name, &Transform, &EntityType)>,
) {
    let mut script = String::from(r#"import bpy
import math

# Clear existing mesh objects
bpy.ops.object.select_all(action='SELECT')
bpy.ops.object.delete(use_global=False)

# Function to create materials
def create_material(name, color):
    mat = bpy.data.materials.new(name=name)
    mat.use_nodes = True
    bsdf = mat.node_tree.nodes["Principled BSDF"]
    bsdf.inputs[0].default_value = (*color, 1.0)
    return mat

# Create materials
floor_mat = create_material("Floor", (0.3, 0.3, 0.3))
wall_mat = create_material("Wall", (0.2, 0.2, 0.25))
pillar_mat = create_material("Pillar", (0.25, 0.25, 0.3))

"#);

    for (name, transform, entity_type) in entities.iter() {
        let pos = transform.translation;
        let rot = transform.rotation.to_euler(EulerRot::XYZ);
        let scale = transform.scale;
        
        match entity_type {
            EntityType::Floor { size } => {
                script.push_str(&format!(r#"
# Create floor: {}
bpy.ops.mesh.primitive_cube_add(size=1, location=({}, {}, {}))
obj = bpy.context.active_object
obj.name = "{}"
obj.scale = ({}, {}, {})
obj.rotation_euler = ({}, {}, {})
obj.data.materials.append(floor_mat)
"#, name, pos.x, pos.z, -pos.y, name, size[0], size[2], size[1], rot.0, rot.2, -rot.1));
            }
            EntityType::Wall { size } => {
                script.push_str(&format!(r#"
# Create wall: {}
bpy.ops.mesh.primitive_cube_add(size=1, location=({}, {}, {}))
obj = bpy.context.active_object
obj.name = "{}"
obj.scale = ({}, {}, {})
obj.rotation_euler = ({}, {}, {})
obj.data.materials.append(wall_mat)
"#, name, pos.x, pos.z, -pos.y, name, size[0], size[2], size[1], rot.0, rot.2, -rot.1));
            }
            EntityType::Pillar { radius, height } => {
                script.push_str(&format!(r#"
# Create pillar: {}
bpy.ops.mesh.primitive_cylinder_add(radius={}, depth={}, location=({}, {}, {}))
obj = bpy.context.active_object
obj.name = "{}"
obj.rotation_euler = ({}, {}, {})
obj.data.materials.append(pillar_mat)
"#, name, radius, height, pos.x, pos.z, -pos.y, name, rot.0, rot.2, -rot.1));
            }
            _ => {}
        }
    }
    
    script.push_str(r#"
# Export as GLTF
bpy.ops.export_scene.gltf(filepath="procedural_map.glb", export_format='GLB')
print("Export complete!")
"#);

    // 스크립트 저장
    if let Err(e) = fs::write("assets/maps/generate_map_in_blender.py", script) {
        error!("Failed to save Blender script: {}", e);
    } else {
        info!("Blender script saved to assets/maps/generate_map_in_blender.py");
        info!("Run this script in Blender to generate a GLB file");
    }
}