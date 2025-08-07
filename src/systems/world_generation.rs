use bevy::prelude::*;
use bevy_rapier3d::prelude::*;
use noise::{NoiseFn, Perlin};
use serde::{Deserialize, Serialize};
use std::collections::HashMap;

#[derive(Component, Clone)]
pub struct TerrainChunk {
    pub chunk_x: i32,
    pub chunk_z: i32,
    pub size: f32,
}

#[derive(Component, Clone)]
pub struct WorldObject {
    pub object_type: String,
    pub material_id: String,
    pub zone: WorldZone,
}

#[derive(Clone, Debug, PartialEq)]
pub enum WorldZone {
    PilgrimsAscent,     // Z1
    CrystalizedForest,  // Z2
    SunkenCity,         // Z3
    GravityMines,       // Z4
    GildedFortress,     // Z5
    CrownsHeart,        // Z6
}

#[derive(Resource)]
pub struct WorldGenConfig {
    pub world_size: f32,        // 4000.0 meters (4km)
    pub chunk_size: f32,        // 256.0 meters per chunk
    pub height_scale: f32,      // 500.0 meters max height
    pub sea_level: f32,         // Base water level
}

impl Default for WorldGenConfig {
    fn default() -> Self {
        Self {
            world_size: 4000.0,
            chunk_size: 256.0,
            height_scale: 500.0,
            sea_level: 50.0,
        }
    }
}

#[derive(Resource)]
pub struct TerrainNoise {
    pub height_noise: Perlin,
    pub detail_noise: Perlin,
    pub forest_noise: Perlin,
}

impl Default for TerrainNoise {
    fn default() -> Self {
        Self {
            height_noise: Perlin::new(1337),
            detail_noise: Perlin::new(2468),
            forest_noise: Perlin::new(9876),
        }
    }
}

#[derive(Serialize, Deserialize, Clone)]
pub struct MaterialMetadata {
    pub material_id: String,
    pub base_color: [f32; 4],
    pub metallic: f32,
    pub roughness: f32,
    pub texture_paths: TexturePaths,
}

#[derive(Serialize, Deserialize, Clone)]
pub struct TexturePaths {
    pub diffuse: Option<String>,
    pub normal: Option<String>,
    pub metallic_roughness: Option<String>,
    pub ambient_occlusion: Option<String>,
    pub emissive: Option<String>,
}

#[derive(Resource, Default)]
pub struct MaterialDatabase {
    pub materials: HashMap<String, MaterialMetadata>,
}

pub fn setup_world_generation(
    mut commands: Commands,
    _meshes: ResMut<Assets<Mesh>>,
    _materials: ResMut<Assets<StandardMaterial>>,
) {
    // Initialize world generation resources
    commands.insert_resource(WorldGenConfig::default());
    commands.insert_resource(TerrainNoise::default());
    
    // Create material database
    let mut material_db = MaterialDatabase::default();
    populate_material_database(&mut material_db);
    commands.insert_resource(material_db);
    
    // Add directional light (sun)
    commands.spawn((
        DirectionalLight {
            illuminance: 10000.0,
            shadows_enabled: true,
            ..default()
        },
        Transform::from_xyz(100.0, 1000.0, 100.0)
            .looking_at(Vec3::ZERO, Vec3::Y),
    ));
    
    // Add fog for atmosphere
    commands.insert_resource(ClearColor(Color::srgb(0.4, 0.45, 0.5)));
}

pub fn generate_terrain_chunk(
    chunk_x: i32,
    chunk_z: i32,
    config: &WorldGenConfig,
    noise: &TerrainNoise,
    meshes: &mut Assets<Mesh>,
) -> Handle<Mesh> {
    let resolution = 64; // Vertices per side
    let chunk_world_x = chunk_x as f32 * config.chunk_size;
    let chunk_world_z = chunk_z as f32 * config.chunk_size;
    
    let mut positions = Vec::new();
    let mut normals: Vec<[f32; 3]> = Vec::new();
    let mut uvs = Vec::new();
    let mut indices = Vec::new();
    
    // Generate heightmap
    for z in 0..=resolution {
        for x in 0..=resolution {
            let world_x = chunk_world_x + (x as f32 / resolution as f32) * config.chunk_size;
            let world_z = chunk_world_z + (z as f32 / resolution as f32) * config.chunk_size;
            
            let height = calculate_terrain_height(world_x, world_z, config, noise);
            
            positions.push([
                world_x,
                height,
                world_z,
            ]);
            
            uvs.push([
                x as f32 / resolution as f32,
                z as f32 / resolution as f32,
            ]);
        }
    }
    
    // Generate indices
    for z in 0..resolution {
        for x in 0..resolution {
            let idx = z * (resolution + 1) + x;
            
            // Two triangles per quad
            indices.push(idx as u32);
            indices.push((idx + resolution + 1) as u32);
            indices.push((idx + 1) as u32);
            
            indices.push((idx + 1) as u32);
            indices.push((idx + resolution + 1) as u32);
            indices.push((idx + resolution + 2) as u32);
        }
    }
    
    // Calculate normals
    let normals = calculate_normals(&positions, &indices);
    
    let mut mesh = Mesh::new(bevy::render::mesh::PrimitiveTopology::TriangleList, bevy::render::render_asset::RenderAssetUsages::RENDER_WORLD | bevy::render::render_asset::RenderAssetUsages::MAIN_WORLD);
    mesh.insert_attribute(Mesh::ATTRIBUTE_POSITION, positions);
    mesh.insert_attribute(Mesh::ATTRIBUTE_NORMAL, normals);
    mesh.insert_attribute(Mesh::ATTRIBUTE_UV_0, uvs);
    mesh.insert_indices(bevy::render::mesh::Indices::U32(indices));
    
    meshes.add(mesh)
}

fn calculate_terrain_height(
    world_x: f32,
    world_z: f32,
    config: &WorldGenConfig,
    noise: &TerrainNoise,
) -> f32{
    let zone = get_zone_at_position(world_x, world_z, config.world_size);
    
    // Base terrain height
    let scale = 0.001;
    let mut height = noise.height_noise.get([world_x as f64 * scale, world_z as f64 * scale]) as f32;
    
    // Add detail
    let detail_scale = 0.01;
    height += noise.detail_noise.get([world_x as f64 * detail_scale, world_z as f64 * detail_scale]) as f32 * 0.1;
    
    // Zone-specific modifications
    match zone {
        WorldZone::PilgrimsAscent => {
            // Gradual slope upward
            height = height * 0.5 + (world_z / config.world_size) * 0.3;
            height *= config.height_scale * 0.6;
        }
        WorldZone::CrystalizedForest => {
            // Relatively flat with some variation
            height = height * 0.3 + 0.2;
            height *= config.height_scale * 0.4;
        }
        WorldZone::SunkenCity => {
            // Depression below sea level
            height = height * 0.2 - 0.3;
            height *= config.height_scale * 0.3;
            height = height.min(config.sea_level - 10.0);
        }
        WorldZone::GravityMines => {
            // Extreme height variations
            height = height.abs() * 2.0;
            height *= config.height_scale * 0.8;
        }
        WorldZone::GildedFortress => {
            // High plateau
            height = height * 0.2 + 0.7;
            height *= config.height_scale * 0.9;
        }
        WorldZone::CrownsHeart => {
            // Floating islands effect
            height = (height * 2.0).sin() * 0.5 + 0.8;
            height *= config.height_scale;
        }
    }
    
    height
}

fn get_zone_at_position(x: f32, z: f32, world_size: f32) -> WorldZone {
    let half_size = world_size / 2.0;
    let center_x = 0.0;
    let center_z = 0.0;
    
    // Normalize coordinates to -1 to 1
    let norm_x = (x - center_x) / half_size;
    let norm_z = (z - center_z) / half_size;
    
    // Distance from center
    let distance = (norm_x * norm_x + norm_z * norm_z).sqrt();
    
    // Center zone
    if distance < 0.25 {
        return WorldZone::CrownsHeart;
    }
    
    // Determine zone based on position
    if norm_z < -0.5 {
        // Southern regions
        if norm_x < -0.3 {
            WorldZone::SunkenCity
        } else if norm_x > 0.3 {
            WorldZone::CrystalizedForest
        } else {
            WorldZone::PilgrimsAscent
        }
    } else {
        // Northern regions
        if norm_x < 0.0 {
            WorldZone::GravityMines
        } else {
            WorldZone::GildedFortress
        }
    }
}

fn calculate_normals(positions: &[[f32; 3]], indices: &[u32]) -> Vec<[f32; 3]> {
    let mut normals = vec![[0.0, 0.0, 0.0]; positions.len()];
    
    // Calculate face normals and accumulate
    for chunk in indices.chunks(3) {
        let i0 = chunk[0] as usize;
        let i1 = chunk[1] as usize;
        let i2 = chunk[2] as usize;
        
        let v0 = Vec3::from(positions[i0]);
        let v1 = Vec3::from(positions[i1]);
        let v2 = Vec3::from(positions[i2]);
        
        let normal = (v1 - v0).cross(v2 - v0).normalize();
        
        normals[i0][0] += normal.x;
        normals[i0][1] += normal.y;
        normals[i0][2] += normal.z;
        
        normals[i1][0] += normal.x;
        normals[i1][1] += normal.y;
        normals[i1][2] += normal.z;
        
        normals[i2][0] += normal.x;
        normals[i2][1] += normal.y;
        normals[i2][2] += normal.z;
    }
    
    // Normalize
    for normal in &mut normals {
        let n = Vec3::from(*normal).normalize();
        *normal = [n.x, n.y, n.z];
    }
    
    normals
}

pub fn setup_material_database(mut commands: Commands) {
    let mut db = MaterialDatabase::default();
    populate_material_database(&mut db);
    commands.insert_resource(db);
}

fn populate_material_database(db: &mut MaterialDatabase) {
    // Terrain materials
    db.materials.insert("terrain_rock".to_string(), MaterialMetadata {
        material_id: "terrain_rock".to_string(),
        base_color: [0.4, 0.35, 0.3, 1.0],
        metallic: 0.0,
        roughness: 0.9,
        texture_paths: TexturePaths {
            diffuse: Some("textures/terrain/rock_diffuse.png".to_string()),
            normal: Some("textures/terrain/rock_normal.png".to_string()),
            metallic_roughness: Some("textures/terrain/rock_metallic_roughness.png".to_string()),
            ambient_occlusion: Some("textures/terrain/rock_ao.png".to_string()),
            emissive: None,
        },
    });
    
    db.materials.insert("terrain_grass".to_string(), MaterialMetadata {
        material_id: "terrain_grass".to_string(),
        base_color: [0.2, 0.4, 0.15, 1.0],
        metallic: 0.0,
        roughness: 0.8,
        texture_paths: TexturePaths {
            diffuse: Some("textures/terrain/grass_diffuse.png".to_string()),
            normal: Some("textures/terrain/grass_normal.png".to_string()),
            metallic_roughness: Some("textures/terrain/grass_metallic_roughness.png".to_string()),
            ambient_occlusion: None,
            emissive: None,
        },
    });
    
    // Crystal materials
    db.materials.insert("crystal_blue".to_string(), MaterialMetadata {
        material_id: "crystal_blue".to_string(),
        base_color: [0.2, 0.4, 0.8, 0.7],
        metallic: 0.0,
        roughness: 0.1,
        texture_paths: TexturePaths {
            diffuse: Some("textures/crystal/crystal_blue_diffuse.png".to_string()),
            normal: Some("textures/crystal/crystal_normal.png".to_string()),
            metallic_roughness: None,
            ambient_occlusion: None,
            emissive: Some("textures/crystal/crystal_blue_emissive.png".to_string()),
        },
    });
    
    // Building materials
    db.materials.insert("stone_brick".to_string(), MaterialMetadata {
        material_id: "stone_brick".to_string(),
        base_color: [0.5, 0.45, 0.4, 1.0],
        metallic: 0.0,
        roughness: 0.7,
        texture_paths: TexturePaths {
            diffuse: Some("textures/building/stone_brick_diffuse.png".to_string()),
            normal: Some("textures/building/stone_brick_normal.png".to_string()),
            metallic_roughness: Some("textures/building/stone_brick_metallic_roughness.png".to_string()),
            ambient_occlusion: Some("textures/building/stone_brick_ao.png".to_string()),
            emissive: None,
        },
    });
    
    db.materials.insert("gold_ornate".to_string(), MaterialMetadata {
        material_id: "gold_ornate".to_string(),
        base_color: [0.9, 0.7, 0.3, 1.0],
        metallic: 1.0,
        roughness: 0.3,
        texture_paths: TexturePaths {
            diffuse: Some("textures/building/gold_ornate_diffuse.png".to_string()),
            normal: Some("textures/building/gold_ornate_normal.png".to_string()),
            metallic_roughness: Some("textures/building/gold_ornate_metallic_roughness.png".to_string()),
            ambient_occlusion: None,
            emissive: None,
        },
    });
    
    // Water
    db.materials.insert("water_murky".to_string(), MaterialMetadata {
        material_id: "water_murky".to_string(),
        base_color: [0.1, 0.2, 0.25, 0.8],
        metallic: 0.0,
        roughness: 0.0,
        texture_paths: TexturePaths {
            diffuse: None,
            normal: Some("textures/water/water_normal.png".to_string()),
            metallic_roughness: None,
            ambient_occlusion: None,
            emissive: None,
        },
    });
}

pub fn spawn_terrain_chunks(
    mut commands: Commands,
    config: Res<WorldGenConfig>,
    noise: Res<TerrainNoise>,
    mut meshes: ResMut<Assets<Mesh>>,
    mut materials: ResMut<Assets<StandardMaterial>>,
    material_db: Res<MaterialDatabase>,
    chunks_query: Query<&TerrainChunk>,
) {
    let chunks_per_side = (config.world_size / config.chunk_size) as i32;
    let half_chunks = chunks_per_side / 2;
    
    // Check which chunks already exist
    let mut existing_chunks = std::collections::HashSet::new();
    for chunk in chunks_query.iter() {
        existing_chunks.insert((chunk.chunk_x, chunk.chunk_z));
    }
    
    // Spawn initial chunks around origin (for testing, normally based on player position)
    for chunk_z in -2..=2 {
        for chunk_x in -2..=2 {
            if existing_chunks.contains(&(chunk_x, chunk_z)) {
                continue;
            }
            
            let mesh_handle = generate_terrain_chunk(chunk_x, chunk_z, &config, &noise, &mut meshes);
            
            // Determine material based on zone
            let world_x = chunk_x as f32 * config.chunk_size;
            let world_z = chunk_z as f32 * config.chunk_size;
            let zone = get_zone_at_position(world_x, world_z, config.world_size);
            
            let material_id = match zone {
                WorldZone::PilgrimsAscent => "terrain_rock",
                WorldZone::CrystalizedForest => "terrain_grass",
                WorldZone::SunkenCity => "stone_brick",
                WorldZone::GravityMines => "terrain_rock",
                WorldZone::GildedFortress => "gold_ornate",
                WorldZone::CrownsHeart => "crystal_blue",
            };
            
            let material_meta = material_db.materials.get(material_id).unwrap();
            let material_handle = materials.add(StandardMaterial {
                base_color: Color::srgba(
                    material_meta.base_color[0],
                    material_meta.base_color[1],
                    material_meta.base_color[2],
                    material_meta.base_color[3],
                ),
                metallic: material_meta.metallic,
                perceptual_roughness: material_meta.roughness,
                ..default()
            });
            
            commands.spawn((
                Mesh3d(mesh_handle),
                MeshMaterial3d(material_handle),
                Transform::from_xyz(0.0, 0.0, 0.0),
                TerrainChunk {
                    chunk_x,
                    chunk_z,
                    size: config.chunk_size,
                },
                Collider::heightfield(
                    vec![0.0; 65 * 65], // TODO: Use actual heightmap data
                    65,
                    65,
                    Vec3::new(config.chunk_size, 1.0, config.chunk_size),
                ),
                RigidBody::Fixed,
            ));
        }
    }
}