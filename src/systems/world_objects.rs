use bevy::prelude::*;
use bevy_rapier3d::prelude::*;
use serde::{Deserialize, Serialize};
use rand::prelude::*;
use rand::rngs::ThreadRng;
use crate::systems::world_generation::{WorldZone, WorldObject, MaterialDatabase};

#[derive(Serialize, Deserialize, Clone)]
pub struct ObjectMetadata {
    pub object_id: String,
    pub display_name: String,
    pub object_type: ObjectType,
    pub material_id: String,
    pub scale: [f32; 3],
    pub physics: PhysicsMetadata,
    pub zone_specific: ZoneSpecificData,
}

#[derive(Serialize, Deserialize, Clone)]
pub enum ObjectType {
    // Environment
    Rock,
    Tree,
    Crystal,
    Building,
    Ruins,
    // Interactive
    Checkpoint,
    Chest,
    Door,
    Lever,
    // Decorative
    Statue,
    Debris,
    Foliage,
    // Zone specific
    FloatingRock,
    CrystalTree,
    SunkenDebris,
}

#[derive(Serialize, Deserialize, Clone)]
pub struct PhysicsMetadata {
    pub has_collision: bool,
    pub is_static: bool,
    pub collision_shape: CollisionShape,
}

#[derive(Serialize, Deserialize, Clone)]
pub enum CollisionShape {
    Box { half_extents: [f32; 3] },
    Sphere { radius: f32 },
    Capsule { height: f32, radius: f32 },
    Mesh, // Use mesh collider
}

#[derive(Serialize, Deserialize, Clone)]
pub struct ZoneSpecificData {
    pub emissive_strength: f32,
    pub float_height: Option<f32>, // For floating objects
    pub submerged_depth: Option<f32>, // For sunken objects
    pub crystal_corruption: f32, // 0.0 - 1.0
}

#[derive(Resource)]
pub struct ObjectDatabase {
    pub objects: Vec<ObjectMetadata>,
}

impl Default for ObjectDatabase {
    fn default() -> Self {
        Self {
            objects: create_object_database(),
        }
    }
}

fn create_object_database() -> Vec<ObjectMetadata> {
    let mut objects = Vec::new();
    
    // Z1 - Pilgrim's Ascent Objects
    objects.push(ObjectMetadata {
        object_id: "rock_large_01".to_string(),
        display_name: "Large Boulder".to_string(),
        object_type: ObjectType::Rock,
        material_id: "terrain_rock".to_string(),
        scale: [2.0, 2.5, 2.0],
        physics: PhysicsMetadata {
            has_collision: true,
            is_static: true,
            collision_shape: CollisionShape::Box { half_extents: [1.0, 1.25, 1.0] },
        },
        zone_specific: ZoneSpecificData {
            emissive_strength: 0.0,
            float_height: None,
            submerged_depth: None,
            crystal_corruption: 0.0,
        },
    });
    
    objects.push(ObjectMetadata {
        object_id: "rock_medium_01".to_string(),
        display_name: "Medium Rock".to_string(),
        object_type: ObjectType::Rock,
        material_id: "terrain_rock".to_string(),
        scale: [1.2, 1.5, 1.2],
        physics: PhysicsMetadata {
            has_collision: true,
            is_static: true,
            collision_shape: CollisionShape::Box { half_extents: [0.6, 0.75, 0.6] },
        },
        zone_specific: ZoneSpecificData {
            emissive_strength: 0.0,
            float_height: None,
            submerged_depth: None,
            crystal_corruption: 0.0,
        },
    });
    
    objects.push(ObjectMetadata {
        object_id: "rock_small_01".to_string(),
        display_name: "Small Stone".to_string(),
        object_type: ObjectType::Rock,
        material_id: "terrain_rock".to_string(),
        scale: [0.5, 0.6, 0.5],
        physics: PhysicsMetadata {
            has_collision: true,
            is_static: true,
            collision_shape: CollisionShape::Box { half_extents: [0.25, 0.3, 0.25] },
        },
        zone_specific: ZoneSpecificData {
            emissive_strength: 0.0,
            float_height: None,
            submerged_depth: None,
            crystal_corruption: 0.0,
        },
    });
    
    objects.push(ObjectMetadata {
        object_id: "wall_ruins_01".to_string(),
        display_name: "Ruined Wall Section".to_string(),
        object_type: ObjectType::Ruins,
        material_id: "stone_brick_weathered".to_string(),
        scale: [5.0, 3.0, 0.8],
        physics: PhysicsMetadata {
            has_collision: true,
            is_static: true,
            collision_shape: CollisionShape::Box { half_extents: [2.5, 1.5, 0.4] },
        },
        zone_specific: ZoneSpecificData {
            emissive_strength: 0.0,
            float_height: None,
            submerged_depth: None,
            crystal_corruption: 0.0,
        },
    });
    
    objects.push(ObjectMetadata {
        object_id: "pillar_broken_01".to_string(),
        display_name: "Broken Pillar".to_string(),
        object_type: ObjectType::Ruins,
        material_id: "stone_brick".to_string(),
        scale: [1.0, 2.5, 1.0],
        physics: PhysicsMetadata {
            has_collision: true,
            is_static: true,
            collision_shape: CollisionShape::Capsule { height: 2.5, radius: 0.5 },
        },
        zone_specific: ZoneSpecificData {
            emissive_strength: 0.0,
            float_height: None,
            submerged_depth: None,
            crystal_corruption: 0.0,
        },
    });
    
    objects.push(ObjectMetadata {
        object_id: "debris_pile_01".to_string(),
        display_name: "Stone Debris".to_string(),
        object_type: ObjectType::Debris,
        material_id: "terrain_rock".to_string(),
        scale: [1.5, 0.8, 1.5],
        physics: PhysicsMetadata {
            has_collision: true,
            is_static: true,
            collision_shape: CollisionShape::Box { half_extents: [0.75, 0.4, 0.75] },
        },
        zone_specific: ZoneSpecificData {
            emissive_strength: 0.0,
            float_height: None,
            submerged_depth: None,
            crystal_corruption: 0.0,
        },
    });
    
    objects.push(ObjectMetadata {
        object_id: "grass_patch_01".to_string(),
        display_name: "Dry Grass".to_string(),
        object_type: ObjectType::Foliage,
        material_id: "vegetation_dry".to_string(),
        scale: [1.0, 0.3, 1.0],
        physics: PhysicsMetadata {
            has_collision: false,
            is_static: true,
            collision_shape: CollisionShape::Box { half_extents: [0.5, 0.15, 0.5] },
        },
        zone_specific: ZoneSpecificData {
            emissive_strength: 0.0,
            float_height: None,
            submerged_depth: None,
            crystal_corruption: 0.0,
        },
    });
    
    // Additional environmental objects
    objects.push(ObjectMetadata {
        object_id: "bush_dead_01".to_string(),
        display_name: "Dead Bush".to_string(),
        object_type: ObjectType::Foliage,
        material_id: "vegetation_dead".to_string(),
        scale: [1.2, 1.0, 1.2],
        physics: PhysicsMetadata {
            has_collision: true,
            is_static: true,
            collision_shape: CollisionShape::Sphere { radius: 0.4 },
        },
        zone_specific: ZoneSpecificData {
            emissive_strength: 0.0,
            float_height: None,
            submerged_depth: None,
            crystal_corruption: 0.0,
        },
    });
    
    objects.push(ObjectMetadata {
        object_id: "wooden_crate_01".to_string(),
        display_name: "Broken Crate".to_string(),
        object_type: ObjectType::Debris,
        material_id: "wood_old".to_string(),
        scale: [1.0, 1.0, 1.0],
        physics: PhysicsMetadata {
            has_collision: true,
            is_static: true,
            collision_shape: CollisionShape::Box { half_extents: [0.5, 0.5, 0.5] },
        },
        zone_specific: ZoneSpecificData {
            emissive_strength: 0.0,
            float_height: None,
            submerged_depth: None,
            crystal_corruption: 0.0,
        },
    });
    
    objects.push(ObjectMetadata {
        object_id: "bone_pile_01".to_string(),
        display_name: "Bone Pile".to_string(),
        object_type: ObjectType::Debris,
        material_id: "bone_white".to_string(),
        scale: [0.8, 0.5, 0.8],
        physics: PhysicsMetadata {
            has_collision: false,
            is_static: true,
            collision_shape: CollisionShape::Box { half_extents: [0.4, 0.25, 0.4] },
        },
        zone_specific: ZoneSpecificData {
            emissive_strength: 0.0,
            float_height: None,
            submerged_depth: None,
            crystal_corruption: 0.0,
        },
    });
    
    objects.push(ObjectMetadata {
        object_id: "wooden_fence_01".to_string(),
        display_name: "Broken Fence".to_string(),
        object_type: ObjectType::Ruins,
        material_id: "wood_old".to_string(),
        scale: [3.0, 1.5, 0.3],
        physics: PhysicsMetadata {
            has_collision: true,
            is_static: true,
            collision_shape: CollisionShape::Box { half_extents: [1.5, 0.75, 0.15] },
        },
        zone_specific: ZoneSpecificData {
            emissive_strength: 0.0,
            float_height: None,
            submerged_depth: None,
            crystal_corruption: 0.0,
        },
    });
    
    objects.push(ObjectMetadata {
        object_id: "checkpoint_torch".to_string(),
        display_name: "Pilgrim's Torch".to_string(),
        object_type: ObjectType::Checkpoint,
        material_id: "stone_brick".to_string(),
        scale: [1.0, 3.0, 1.0],
        physics: PhysicsMetadata {
            has_collision: true,
            is_static: true,
            collision_shape: CollisionShape::Capsule { height: 3.0, radius: 0.5 },
        },
        zone_specific: ZoneSpecificData {
            emissive_strength: 5.0,
            float_height: None,
            submerged_depth: None,
            crystal_corruption: 0.0,
        },
    });
    
    objects.push(ObjectMetadata {
        object_id: "dead_tree_01".to_string(),
        display_name: "Withered Tree".to_string(),
        object_type: ObjectType::Tree,
        material_id: "wood_dead".to_string(),
        scale: [1.5, 4.0, 1.5],
        physics: PhysicsMetadata {
            has_collision: true,
            is_static: true,
            collision_shape: CollisionShape::Capsule { height: 4.0, radius: 0.3 },
        },
        zone_specific: ZoneSpecificData {
            emissive_strength: 0.0,
            float_height: None,
            submerged_depth: None,
            crystal_corruption: 0.0,
        },
    });
    
    // Z2 - Crystalized Forest Objects
    objects.push(ObjectMetadata {
        object_id: "crystal_tree_01".to_string(),
        display_name: "Crystalized Oak".to_string(),
        object_type: ObjectType::CrystalTree,
        material_id: "crystal_blue".to_string(),
        scale: [2.0, 6.0, 2.0],
        physics: PhysicsMetadata {
            has_collision: true,
            is_static: true,
            collision_shape: CollisionShape::Capsule { height: 6.0, radius: 0.5 },
        },
        zone_specific: ZoneSpecificData {
            emissive_strength: 2.0,
            float_height: None,
            submerged_depth: None,
            crystal_corruption: 0.8,
        },
    });
    
    objects.push(ObjectMetadata {
        object_id: "crystal_formation_01".to_string(),
        display_name: "Crystal Cluster".to_string(),
        object_type: ObjectType::Crystal,
        material_id: "crystal_purple".to_string(),
        scale: [1.0, 2.0, 1.0],
        physics: PhysicsMetadata {
            has_collision: true,
            is_static: true,
            collision_shape: CollisionShape::Box { half_extents: [0.5, 1.0, 0.5] },
        },
        zone_specific: ZoneSpecificData {
            emissive_strength: 3.0,
            float_height: None,
            submerged_depth: None,
            crystal_corruption: 1.0,
        },
    });
    
    // Z3 - Sunken City Objects
    objects.push(ObjectMetadata {
        object_id: "sunken_building_01".to_string(),
        display_name: "Collapsed Tower".to_string(),
        object_type: ObjectType::Building,
        material_id: "stone_brick_weathered".to_string(),
        scale: [8.0, 15.0, 8.0],
        physics: PhysicsMetadata {
            has_collision: true,
            is_static: true,
            collision_shape: CollisionShape::Mesh,
        },
        zone_specific: ZoneSpecificData {
            emissive_strength: 0.0,
            float_height: None,
            submerged_depth: Some(5.0),
            crystal_corruption: 0.2,
        },
    });
    
    objects.push(ObjectMetadata {
        object_id: "gargoyle_statue".to_string(),
        display_name: "Gargoyle Sentinel".to_string(),
        object_type: ObjectType::Statue,
        material_id: "stone_dark".to_string(),
        scale: [1.5, 2.0, 1.5],
        physics: PhysicsMetadata {
            has_collision: true,
            is_static: true,
            collision_shape: CollisionShape::Box { half_extents: [0.75, 1.0, 0.75] },
        },
        zone_specific: ZoneSpecificData {
            emissive_strength: 0.0,
            float_height: None,
            submerged_depth: None,
            crystal_corruption: 0.0,
        },
    });
    
    // Z4 - Gravity Mines Objects
    objects.push(ObjectMetadata {
        object_id: "floating_rock_01".to_string(),
        display_name: "Gravity-Defying Boulder".to_string(),
        object_type: ObjectType::FloatingRock,
        material_id: "terrain_rock".to_string(),
        scale: [3.0, 2.0, 3.0],
        physics: PhysicsMetadata {
            has_collision: true,
            is_static: false,
            collision_shape: CollisionShape::Box { half_extents: [1.5, 1.0, 1.5] },
        },
        zone_specific: ZoneSpecificData {
            emissive_strength: 0.5,
            float_height: Some(10.0),
            submerged_depth: None,
            crystal_corruption: 0.5,
        },
    });
    
    objects.push(ObjectMetadata {
        object_id: "mining_equipment_01".to_string(),
        display_name: "Abandoned Drill".to_string(),
        object_type: ObjectType::Debris,
        material_id: "metal_rusted".to_string(),
        scale: [2.0, 3.0, 2.0],
        physics: PhysicsMetadata {
            has_collision: true,
            is_static: true,
            collision_shape: CollisionShape::Mesh,
        },
        zone_specific: ZoneSpecificData {
            emissive_strength: 0.0,
            float_height: None,
            submerged_depth: None,
            crystal_corruption: 0.3,
        },
    });
    
    // Z5 - Gilded Fortress Objects
    objects.push(ObjectMetadata {
        object_id: "golden_statue_01".to_string(),
        display_name: "Knight Commander Statue".to_string(),
        object_type: ObjectType::Statue,
        material_id: "gold_ornate".to_string(),
        scale: [2.0, 5.0, 2.0],
        physics: PhysicsMetadata {
            has_collision: true,
            is_static: true,
            collision_shape: CollisionShape::Box { half_extents: [1.0, 2.5, 1.0] },
        },
        zone_specific: ZoneSpecificData {
            emissive_strength: 0.3,
            float_height: None,
            submerged_depth: None,
            crystal_corruption: 0.0,
        },
    });
    
    objects.push(ObjectMetadata {
        object_id: "fortress_wall_segment".to_string(),
        display_name: "Fortress Wall".to_string(),
        object_type: ObjectType::Building,
        material_id: "stone_fortress".to_string(),
        scale: [10.0, 8.0, 2.0],
        physics: PhysicsMetadata {
            has_collision: true,
            is_static: true,
            collision_shape: CollisionShape::Box { half_extents: [5.0, 4.0, 1.0] },
        },
        zone_specific: ZoneSpecificData {
            emissive_strength: 0.0,
            float_height: None,
            submerged_depth: None,
            crystal_corruption: 0.1,
        },
    });
    
    // Z6 - Crown's Heart Objects
    objects.push(ObjectMetadata {
        object_id: "crown_fragment_large".to_string(),
        display_name: "Crown Fragment".to_string(),
        object_type: ObjectType::Crystal,
        material_id: "crystal_crown".to_string(),
        scale: [5.0, 10.0, 5.0],
        physics: PhysicsMetadata {
            has_collision: false,
            is_static: false,
            collision_shape: CollisionShape::Mesh,
        },
        zone_specific: ZoneSpecificData {
            emissive_strength: 10.0,
            float_height: Some(50.0),
            submerged_depth: None,
            crystal_corruption: 1.0,
        },
    });
    
    objects
}

pub fn spawn_world_objects(
    mut commands: Commands,
    mut meshes: ResMut<Assets<Mesh>>,
    mut materials: ResMut<Assets<StandardMaterial>>,
    object_db: Res<ObjectDatabase>,
    material_db: Res<MaterialDatabase>,
) {
    // Define zone positions in the world
    // Each zone is positioned to create a connected world
    let zone_offsets = [
        Vec3::new(0.0, 0.0, 0.0),        // Z1 - Pilgrim's Ascent (center-south)
        Vec3::new(300.0, 20.0, -200.0),  // Z2 - Crystalized Forest (east)
        Vec3::new(-300.0, -30.0, -200.0),// Z3 - Sunken City (west, lower)
        Vec3::new(0.0, 50.0, -500.0),    // Z4 - Gravity Mines (north, higher)
        Vec3::new(0.0, 100.0, -800.0),   // Z5 - Gilded Fortress (far north, highest)
        Vec3::new(0.0, 200.0, -1100.0),  // Z6 - Crown's Heart (floating)
    ];
    
    // Spawn objects for all zones
    spawn_pilgrims_ascent_objects(
        &mut commands,
        &mut meshes,
        &mut materials,
        &object_db,
        &material_db,
        zone_offsets[0],
    );
    
    spawn_crystalized_forest_objects(
        &mut commands,
        &mut meshes,
        &mut materials,
        &object_db,
        &material_db,
        zone_offsets[1],
    );
    
    spawn_sunken_city_objects(
        &mut commands,
        &mut meshes,
        &mut materials,
        &object_db,
        &material_db,
        zone_offsets[2],
    );
    
    spawn_gravity_mines_objects(
        &mut commands,
        &mut meshes,
        &mut materials,
        &object_db,
        &material_db,
        zone_offsets[3],
    );
    
    spawn_gilded_fortress_objects(
        &mut commands,
        &mut meshes,
        &mut materials,
        &object_db,
        &material_db,
        zone_offsets[4],
    );
    
    spawn_crowns_heart_objects(
        &mut commands,
        &mut meshes,
        &mut materials,
        &object_db,
        &material_db,
        zone_offsets[5],
    );
}

fn spawn_pilgrims_ascent_objects(
    commands: &mut Commands,
    meshes: &mut Assets<Mesh>,
    materials: &mut Assets<StandardMaterial>,
    object_db: &ObjectDatabase,
    material_db: &MaterialDatabase,
    offset: Vec3,
) {
    let mut rng = rand::thread_rng();
    
    // Spawn checkpoint at entrance
    if let Some(checkpoint_meta) = object_db.objects.iter()
        .find(|obj| obj.object_id == "checkpoint_torch") {
        
        let mesh_handle = meshes.add(Cylinder::new(0.5, 3.0));
        let material = create_material_from_metadata(checkpoint_meta, material_db, materials);
        
        commands.spawn((
            Mesh3d(mesh_handle),
            MeshMaterial3d(material),
            Transform::from_xyz(offset.x + 0.0, offset.y + 1.5, offset.z - 100.0),
            WorldObject {
                object_type: checkpoint_meta.object_id.clone(),
                material_id: checkpoint_meta.material_id.clone(),
                zone: WorldZone::PilgrimsAscent,
            },
            create_collider(&checkpoint_meta.physics),
            RigidBody::Fixed,
        ))
        .with_children(|parent| {
            // Add flame light
            parent.spawn((
                PointLight {
                    intensity: 5000.0,
                    color: Color::srgb(1.0, 0.6, 0.2),
                    range: 20.0,
                    shadows_enabled: true,
                    ..default()
                },
                Transform::from_xyz(0.0, 2.0, 0.0),
            ));
        });
    }
    
    // Spawn large rocks strategically placed
    if let Some(rock_meta) = object_db.objects.iter()
        .find(|obj| obj.object_id == "rock_large_01") {
        
        let rock_positions = vec![
            Vec3::new(-15.0, 0.0, -80.0),
            Vec3::new(20.0, 0.0, -60.0),
            Vec3::new(-25.0, 0.0, -40.0),
            Vec3::new(30.0, 0.0, -20.0),
            Vec3::new(-35.0, 0.0, -90.0),
            Vec3::new(40.0, 0.0, -75.0),
            Vec3::new(-8.0, 0.0, -45.0),
            Vec3::new(12.0, 0.0, -25.0),
        ];
        
        for pos in rock_positions {
            let mesh_handle = meshes.add(Cuboid::new(2.0, 2.5, 2.0));
            let material = create_material_from_metadata(rock_meta, material_db, materials);
            
            commands.spawn((
                Mesh3d(mesh_handle),
                MeshMaterial3d(material),
                Transform::from_xyz(offset.x + pos.x, offset.y + pos.y + 1.25, offset.z + pos.z)
                    .with_rotation(Quat::from_rotation_y(rng.gen::<f32>() * std::f32::consts::TAU)),
                WorldObject {
                    object_type: rock_meta.object_id.clone(),
                    material_id: rock_meta.material_id.clone(),
                    zone: WorldZone::PilgrimsAscent,
                },
                create_collider(&rock_meta.physics),
                RigidBody::Fixed,
            ));
        }
    }
    
    // Spawn medium rocks scattered around - INCREASED DENSITY
    if let Some(rock_meta) = object_db.objects.iter()
        .find(|obj| obj.object_id == "rock_medium_01") {
        
        for _ in 0..60 {  // Increased from 20
            let x = rng.gen_range(-80.0..80.0);
            let z = rng.gen_range(-150.0..0.0);
            
            let mesh_handle = meshes.add(Cuboid::new(1.2, 1.5, 1.2));
            let material = create_material_from_metadata(rock_meta, material_db, materials);
            
            commands.spawn((
                Mesh3d(mesh_handle),
                MeshMaterial3d(material),
                Transform::from_xyz(offset.x + x, offset.y + 0.75, offset.z + z)
                    .with_rotation(Quat::from_rotation_y(rng.gen::<f32>() * std::f32::consts::TAU)),
                WorldObject {
                    object_type: rock_meta.object_id.clone(),
                    material_id: rock_meta.material_id.clone(),
                    zone: WorldZone::PilgrimsAscent,
                },
                create_collider(&rock_meta.physics),
                RigidBody::Fixed,
            ));
        }
    }
    
    // Spawn small stones abundantly
    if let Some(rock_meta) = object_db.objects.iter()
        .find(|obj| obj.object_id == "rock_small_01") {
        
        for _ in 0..40 {
            let x = rng.gen_range(-60.0..60.0);
            let z = rng.gen_range(-130.0..0.0);
            
            let mesh_handle = meshes.add(Cuboid::new(0.5, 0.6, 0.5));
            let material = create_material_from_metadata(rock_meta, material_db, materials);
            
            commands.spawn((
                Mesh3d(mesh_handle),
                MeshMaterial3d(material),
                Transform::from_xyz(offset.x + x, offset.y + 0.3, offset.z + z)
                    .with_rotation(Quat::from_rotation_y(rng.gen::<f32>() * std::f32::consts::TAU)),
                WorldObject {
                    object_type: rock_meta.object_id.clone(),
                    material_id: rock_meta.material_id.clone(),
                    zone: WorldZone::PilgrimsAscent,
                },
                create_collider(&rock_meta.physics),
                RigidBody::Fixed,
            ));
        }
    }
    
    // Spawn dead trees
    if let Some(tree_meta) = object_db.objects.iter()
        .find(|obj| obj.object_id == "dead_tree_01") {
        
        let tree_positions = vec![
            Vec3::new(-10.0, 0.0, -90.0),
            Vec3::new(15.0, 0.0, -70.0),
            Vec3::new(-20.0, 0.0, -50.0),
            Vec3::new(25.0, 0.0, -30.0),
            Vec3::new(-30.0, 0.0, -85.0),
            Vec3::new(35.0, 0.0, -65.0),
            Vec3::new(-5.0, 0.0, -110.0),
            Vec3::new(18.0, 0.0, -95.0),
            Vec3::new(-40.0, 0.0, -75.0),
            Vec3::new(45.0, 0.0, -55.0),
        ];
        
        for pos in tree_positions {
            let mesh_handle = meshes.add(Cylinder::new(0.3, 4.0));
            let material = materials.add(StandardMaterial {
                base_color: Color::srgb(0.3, 0.25, 0.2),
                perceptual_roughness: 0.9,
                ..default()
            });
            
            commands.spawn((
                Mesh3d(mesh_handle),
                MeshMaterial3d(material),
                Transform::from_xyz(offset.x + pos.x, offset.y + pos.y + 2.0, offset.z + pos.z),
                WorldObject {
                    object_type: tree_meta.object_id.clone(),
                    material_id: tree_meta.material_id.clone(),
                    zone: WorldZone::PilgrimsAscent,
                },
                create_collider(&tree_meta.physics),
                RigidBody::Fixed,
            ));
        }
    }
    
    // Spawn wall ruins
    if let Some(wall_meta) = object_db.objects.iter()
        .find(|obj| obj.object_id == "wall_ruins_01") {
        
        let wall_positions = vec![
            (Vec3::new(-25.0, 0.0, -100.0), 0.3),
            (Vec3::new(30.0, 0.0, -80.0), -0.5),
            (Vec3::new(-20.0, 0.0, -60.0), 1.2),
            (Vec3::new(35.0, 0.0, -40.0), -0.8),
        ];
        
        for (pos, rot) in wall_positions {
            let mesh_handle = meshes.add(Cuboid::new(5.0, 3.0, 0.8));
            let material = create_material_from_metadata(wall_meta, material_db, materials);
            
            commands.spawn((
                Mesh3d(mesh_handle),
                MeshMaterial3d(material),
                Transform::from_xyz(offset.x + pos.x, offset.y + pos.y + 1.5, offset.z + pos.z)
                    .with_rotation(Quat::from_rotation_y(rot)),
                WorldObject {
                    object_type: wall_meta.object_id.clone(),
                    material_id: wall_meta.material_id.clone(),
                    zone: WorldZone::PilgrimsAscent,
                },
                create_collider(&wall_meta.physics),
                RigidBody::Fixed,
            ));
        }
    }
    
    // Spawn broken pillars
    if let Some(pillar_meta) = object_db.objects.iter()
        .find(|obj| obj.object_id == "pillar_broken_01") {
        
        let pillar_positions = vec![
            Vec3::new(-15.0, 0.0, -95.0),
            Vec3::new(20.0, 0.0, -85.0),
            Vec3::new(-30.0, 0.0, -70.0),
            Vec3::new(25.0, 0.0, -50.0),
            Vec3::new(-10.0, 0.0, -35.0),
            Vec3::new(15.0, 0.0, -25.0),
        ];
        
        for pos in pillar_positions {
            let mesh_handle = meshes.add(Cylinder::new(0.5, 2.5));
            let material = create_material_from_metadata(pillar_meta, material_db, materials);
            
            commands.spawn((
                Mesh3d(mesh_handle),
                MeshMaterial3d(material),
                Transform::from_xyz(offset.x + pos.x, offset.y + pos.y + 1.25, offset.z + pos.z),
                WorldObject {
                    object_type: pillar_meta.object_id.clone(),
                    material_id: pillar_meta.material_id.clone(),
                    zone: WorldZone::PilgrimsAscent,
                },
                create_collider(&pillar_meta.physics),
                RigidBody::Fixed,
            ));
        }
    }
    
    // Spawn debris piles
    if let Some(debris_meta) = object_db.objects.iter()
        .find(|obj| obj.object_id == "debris_pile_01") {
        
        for _ in 0..15 {
            let x = rng.gen_range(-45.0..45.0);
            let z = rng.gen_range(-110.0..-20.0);
            
            let mesh_handle = meshes.add(Cuboid::new(1.5, 0.8, 1.5));
            let material = create_material_from_metadata(debris_meta, material_db, materials);
            
            commands.spawn((
                Mesh3d(mesh_handle),
                MeshMaterial3d(material),
                Transform::from_xyz(offset.x + x, offset.y + 0.4, offset.z + z)
                    .with_rotation(Quat::from_rotation_y(rng.gen::<f32>() * std::f32::consts::TAU)),
                WorldObject {
                    object_type: debris_meta.object_id.clone(),
                    material_id: debris_meta.material_id.clone(),
                    zone: WorldZone::PilgrimsAscent,
                },
                create_collider(&debris_meta.physics),
                RigidBody::Fixed,
            ));
        }
    }
    
    // Add path markers with smaller rocks
    if let Some(rock_meta) = object_db.objects.iter()
        .find(|obj| obj.object_id == "rock_small_01") {
        
        // Create a winding path
        for i in 0..50 {
            let t = i as f32 / 50.0;
            let z = -120.0 + t * 120.0;
            let x = (t * 8.0).sin() * 20.0 + rng.gen_range(-5.0..5.0);
            
            let mesh_handle = meshes.add(Cuboid::new(0.3, 0.4, 0.3));
            let material = create_material_from_metadata(rock_meta, material_db, materials);
            
            commands.spawn((
                Mesh3d(mesh_handle),
                MeshMaterial3d(material),
                Transform::from_xyz(offset.x + x, offset.y + 0.2, offset.z + z)
                    .with_rotation(Quat::from_rotation_y(rng.gen::<f32>() * std::f32::consts::TAU)),
                WorldObject {
                    object_type: rock_meta.object_id.clone(),
                    material_id: rock_meta.material_id.clone(),
                    zone: WorldZone::PilgrimsAscent,
                },
                create_collider(&rock_meta.physics),
                RigidBody::Fixed,
            ));
        }
    }
    
    // Add clustered rock formations
    for cluster_center in [(25.0, -90.0), (-30.0, -70.0), (40.0, -50.0), (-35.0, -30.0)] {
        spawn_rock_cluster(commands, meshes, materials, object_db, material_db, cluster_center, &mut rng, offset);
    }
    
    // Spawn grass patches - INCREASED DENSITY
    if let Some(grass_meta) = object_db.objects.iter()
        .find(|obj| obj.object_id == "grass_patch_01") {
        
        for _ in 0..80 {  // Increased from 30
            let x = rng.gen_range(-80.0..80.0);
            let z = rng.gen_range(-150.0..0.0);
            
            let mesh_handle = meshes.add(Plane3d::default().mesh().size(1.0, 1.0));
            let material = materials.add(StandardMaterial {
                base_color: Color::srgb(0.4, 0.35, 0.2),
                perceptual_roughness: 1.0,
                alpha_mode: AlphaMode::Blend,
                ..default()
            });
            
            commands.spawn((
                Mesh3d(mesh_handle),
                MeshMaterial3d(material),
                Transform::from_xyz(offset.x + x, offset.y + 0.01, offset.z + z)
                    .with_rotation(Quat::from_rotation_y(rng.gen::<f32>() * std::f32::consts::TAU)),
                WorldObject {
                    object_type: grass_meta.object_id.clone(),
                    material_id: grass_meta.material_id.clone(),
                    zone: WorldZone::PilgrimsAscent,
                },
            ));
        }
    }
    
    // Spawn dead bushes
    if let Some(bush_meta) = object_db.objects.iter()
        .find(|obj| obj.object_id == "bush_dead_01") {
        
        for _ in 0..40 {
            let x = rng.gen_range(-70.0..70.0);
            let z = rng.gen_range(-140.0..-10.0);
            
            let mesh_handle = meshes.add(Sphere::new(0.6));
            let material = materials.add(StandardMaterial {
                base_color: Color::srgb(0.35, 0.3, 0.25),
                perceptual_roughness: 0.9,
                ..default()
            });
            
            commands.spawn((
                Mesh3d(mesh_handle),
                MeshMaterial3d(material),
                Transform::from_xyz(offset.x + x, offset.y + 0.5, offset.z + z),
                WorldObject {
                    object_type: bush_meta.object_id.clone(),
                    material_id: bush_meta.material_id.clone(),
                    zone: WorldZone::PilgrimsAscent,
                },
                create_collider(&bush_meta.physics),
                RigidBody::Fixed,
            ));
        }
    }
    
    // Spawn wooden crates and barrels
    if let Some(crate_meta) = object_db.objects.iter()
        .find(|obj| obj.object_id == "wooden_crate_01") {
        
        for _ in 0..25 {
            let x = rng.gen_range(-60.0..60.0);
            let z = rng.gen_range(-130.0..-20.0);
            
            let mesh_handle = meshes.add(Cuboid::new(1.0, 1.0, 1.0));
            let material = materials.add(StandardMaterial {
                base_color: Color::srgb(0.4, 0.3, 0.2),
                perceptual_roughness: 0.85,
                ..default()
            });
            
            commands.spawn((
                Mesh3d(mesh_handle),
                MeshMaterial3d(material),
                Transform::from_xyz(offset.x + x, offset.y + 0.5, offset.z + z)
                    .with_rotation(Quat::from_rotation_y(rng.gen::<f32>() * std::f32::consts::TAU)),
                WorldObject {
                    object_type: crate_meta.object_id.clone(),
                    material_id: crate_meta.material_id.clone(),
                    zone: WorldZone::PilgrimsAscent,
                },
                create_collider(&crate_meta.physics),
                RigidBody::Fixed,
            ));
        }
    }
    
    // Spawn bone piles
    if let Some(bone_meta) = object_db.objects.iter()
        .find(|obj| obj.object_id == "bone_pile_01") {
        
        for _ in 0..20 {
            let x = rng.gen_range(-50.0..50.0);
            let z = rng.gen_range(-120.0..-30.0);
            
            let mesh_handle = meshes.add(Cuboid::new(0.8, 0.5, 0.8));
            let material = materials.add(StandardMaterial {
                base_color: Color::srgb(0.9, 0.85, 0.8),
                perceptual_roughness: 0.8,
                ..default()
            });
            
            commands.spawn((
                Mesh3d(mesh_handle),
                MeshMaterial3d(material),
                Transform::from_xyz(offset.x + x, offset.y + 0.25, offset.z + z)
                    .with_rotation(Quat::from_rotation_y(rng.gen::<f32>() * std::f32::consts::TAU)),
                WorldObject {
                    object_type: bone_meta.object_id.clone(),
                    material_id: bone_meta.material_id.clone(),
                    zone: WorldZone::PilgrimsAscent,
                },
            ));
        }
    }
    
    // Spawn broken fences
    if let Some(fence_meta) = object_db.objects.iter()
        .find(|obj| obj.object_id == "wooden_fence_01") {
        
        for _ in 0..15 {
            let x = rng.gen_range(-60.0..60.0);
            let z = rng.gen_range(-120.0..-20.0);
            let rotation = rng.gen::<f32>() * std::f32::consts::TAU;
            
            let mesh_handle = meshes.add(Cuboid::new(3.0, 1.5, 0.3));
            let material = materials.add(StandardMaterial {
                base_color: Color::srgb(0.35, 0.25, 0.15),
                perceptual_roughness: 0.9,
                ..default()
            });
            
            commands.spawn((
                Mesh3d(mesh_handle),
                MeshMaterial3d(material),
                Transform::from_xyz(offset.x + x, offset.y + 0.75, offset.z + z)
                    .with_rotation(Quat::from_rotation_y(rotation)),
                WorldObject {
                    object_type: fence_meta.object_id.clone(),
                    material_id: fence_meta.material_id.clone(),
                    zone: WorldZone::PilgrimsAscent,
                },
                create_collider(&fence_meta.physics),
                RigidBody::Fixed,
            ));
        }
    }
}

fn create_material_from_metadata(
    object_meta: &ObjectMetadata,
    material_db: &MaterialDatabase,
    materials: &mut Assets<StandardMaterial>,
) -> Handle<StandardMaterial> {
    if let Some(mat_meta) = material_db.materials.get(&object_meta.material_id) {
        let mut material = StandardMaterial {
            base_color: Color::srgba(
                mat_meta.base_color[0],
                mat_meta.base_color[1],
                mat_meta.base_color[2],
                mat_meta.base_color[3],
            ),
            metallic: mat_meta.metallic,
            perceptual_roughness: mat_meta.roughness,
            ..default()
        };
        
        // Add emissive for glowing objects
        if object_meta.zone_specific.emissive_strength > 0.0 {
            material.emissive = LinearRgba::new(
                mat_meta.base_color[0] * object_meta.zone_specific.emissive_strength,
                mat_meta.base_color[1] * object_meta.zone_specific.emissive_strength,
                mat_meta.base_color[2] * object_meta.zone_specific.emissive_strength,
                1.0,
            );
        }
        
        materials.add(material)
    } else {
        // Fallback material
        materials.add(StandardMaterial {
            base_color: Color::srgb(0.5, 0.5, 0.5),
            ..default()
        })
    }
}

fn spawn_rock_cluster(
    commands: &mut Commands,
    meshes: &mut Assets<Mesh>,
    materials: &mut Assets<StandardMaterial>,
    object_db: &ObjectDatabase,
    material_db: &MaterialDatabase,
    center: (f32, f32),
    rng: &mut ThreadRng,
    offset: Vec3,
) {
    let (center_x, center_z) = center;
    let cluster_radius = 8.0;
    
    // Spawn one large rock in the center
    if let Some(rock_meta) = object_db.objects.iter()
        .find(|obj| obj.object_id == "rock_large_01") {
        
        let mesh_handle = meshes.add(Cuboid::new(2.5, 3.0, 2.5));
        let material = create_material_from_metadata(rock_meta, material_db, materials);
        
        commands.spawn((
            Mesh3d(mesh_handle),
            MeshMaterial3d(material),
            Transform::from_xyz(offset.x + center_x, offset.y + 1.5, offset.z + center_z)
                .with_rotation(Quat::from_rotation_y(rng.gen::<f32>() * std::f32::consts::TAU)),
            WorldObject {
                object_type: rock_meta.object_id.clone(),
                material_id: rock_meta.material_id.clone(),
                zone: WorldZone::PilgrimsAscent,
            },
            create_collider(&rock_meta.physics),
            RigidBody::Fixed,
        ));
    }
    
    // Spawn medium rocks around it
    if let Some(rock_meta) = object_db.objects.iter()
        .find(|obj| obj.object_id == "rock_medium_01") {
        
        for _ in 0..rng.gen_range(3..6) {
            let angle = rng.gen::<f32>() * std::f32::consts::TAU;
            let distance = rng.gen_range(3.0..cluster_radius);
            let x = center_x + angle.cos() * distance;
            let z = center_z + angle.sin() * distance;
            
            let mesh_handle = meshes.add(Cuboid::new(1.2, 1.5, 1.2));
            let material = create_material_from_metadata(rock_meta, material_db, materials);
            
            commands.spawn((
                Mesh3d(mesh_handle),
                MeshMaterial3d(material),
                Transform::from_xyz(offset.x + x, offset.y + 0.75, offset.z + z)
                    .with_rotation(Quat::from_rotation_y(rng.gen::<f32>() * std::f32::consts::TAU)),
                WorldObject {
                    object_type: rock_meta.object_id.clone(),
                    material_id: rock_meta.material_id.clone(),
                    zone: WorldZone::PilgrimsAscent,
                },
                create_collider(&rock_meta.physics),
                RigidBody::Fixed,
            ));
        }
    }
    
    // Spawn small rocks as scatter
    if let Some(rock_meta) = object_db.objects.iter()
        .find(|obj| obj.object_id == "rock_small_01") {
        
        for _ in 0..rng.gen_range(8..15) {
            let angle = rng.gen::<f32>() * std::f32::consts::TAU;
            let distance = rng.gen_range(1.0..cluster_radius * 1.5);
            let x = center_x + angle.cos() * distance;
            let z = center_z + angle.sin() * distance;
            
            let mesh_handle = meshes.add(Cuboid::new(0.4, 0.5, 0.4));
            let material = create_material_from_metadata(rock_meta, material_db, materials);
            
            commands.spawn((
                Mesh3d(mesh_handle),
                MeshMaterial3d(material),
                Transform::from_xyz(offset.x + x, offset.y + 0.25, offset.z + z)
                    .with_rotation(Quat::from_rotation_y(rng.gen::<f32>() * std::f32::consts::TAU)),
                WorldObject {
                    object_type: rock_meta.object_id.clone(),
                    material_id: rock_meta.material_id.clone(),
                    zone: WorldZone::PilgrimsAscent,
                },
                create_collider(&rock_meta.physics),
                RigidBody::Fixed,
            ));
        }
    }
}

fn spawn_crystalized_forest_objects(
    commands: &mut Commands,
    meshes: &mut Assets<Mesh>,
    materials: &mut Assets<StandardMaterial>,
    object_db: &ObjectDatabase,
    material_db: &MaterialDatabase,
    offset: Vec3,
) {
    let mut rng = rand::thread_rng();
    
    // Spawn crystal trees - INCREASED DENSITY
    if let Some(tree_meta) = object_db.objects.iter()
        .find(|obj| obj.object_id == "crystal_tree_01") {
        
        for _ in 0..80 {  // Increased from 25
            let x = rng.gen_range(-150.0..150.0);
            let z = rng.gen_range(-150.0..150.0);
            
            let mesh_handle = meshes.add(Cylinder::new(0.5, 6.0));
            let material = create_material_from_metadata(tree_meta, material_db, materials);
            
            commands.spawn((
                Mesh3d(mesh_handle),
                MeshMaterial3d(material),
                Transform::from_xyz(offset.x + x, offset.y + 3.0, offset.z + z),
                WorldObject {
                    object_type: tree_meta.object_id.clone(),
                    material_id: tree_meta.material_id.clone(),
                    zone: WorldZone::CrystalizedForest,
                },
                create_collider(&tree_meta.physics),
                RigidBody::Fixed,
            ))
            .with_children(|parent| {
                parent.spawn((
                    PointLight {
                        intensity: 2000.0,
                        color: Color::srgb(0.4, 0.6, 1.0),
                        range: 15.0,
                        shadows_enabled: true,
                        ..default()
                    },
                    Transform::from_xyz(0.0, 3.0, 0.0),
                ));
            });
        }
    }
    
    // Spawn crystal formations - INCREASED DENSITY
    if let Some(crystal_meta) = object_db.objects.iter()
        .find(|obj| obj.object_id == "crystal_formation_01") {
        
        for _ in 0..120 {  // Increased from 40
            let x = rng.gen_range(-180.0..180.0);
            let z = rng.gen_range(-180.0..180.0);
            
            let size_variation = rng.gen_range(0.5..1.5);
            let height_variation = rng.gen_range(1.5..3.0);
            
            let mesh_handle = meshes.add(Cuboid::new(1.0 * size_variation, height_variation, 1.0 * size_variation));
            let material = create_material_from_metadata(crystal_meta, material_db, materials);
            
            commands.spawn((
                Mesh3d(mesh_handle),
                MeshMaterial3d(material),
                Transform::from_xyz(offset.x + x, offset.y + height_variation * 0.5, offset.z + z)
                    .with_rotation(Quat::from_rotation_y(rng.gen::<f32>() * std::f32::consts::TAU)),
                WorldObject {
                    object_type: crystal_meta.object_id.clone(),
                    material_id: crystal_meta.material_id.clone(),
                    zone: WorldZone::CrystalizedForest,
                },
                create_collider(&crystal_meta.physics),
                RigidBody::Fixed,
            ));
        }
    }
    
    // Add smaller crystal shards scattered on ground
    for _ in 0..200 {
        let x = rng.gen_range(-200.0..200.0);
        let z = rng.gen_range(-200.0..200.0);
        
        let mesh_handle = meshes.add(Cuboid::new(0.3, 0.6, 0.3));
        let material = materials.add(StandardMaterial {
            base_color: Color::srgb(0.5, 0.7, 1.0),
            emissive: LinearRgba::new(0.2, 0.3, 0.6, 1.0),
            perceptual_roughness: 0.2,
            metallic: 0.3,
            ..default()
        });
        
        commands.spawn((
            Mesh3d(mesh_handle),
            MeshMaterial3d(material),
            Transform::from_xyz(offset.x + x, offset.y + 0.3, offset.z + z)
                .with_rotation(Quat::from_rotation_y(rng.gen::<f32>() * std::f32::consts::TAU)),
            RigidBody::Fixed,
        ));
    }
    
    // Add corrupted rocks
    for _ in 0..60 {
        let x = rng.gen_range(-160.0..160.0);
        let z = rng.gen_range(-160.0..160.0);
        
        let mesh_handle = meshes.add(Cuboid::new(1.5, 2.0, 1.5));
        let material = materials.add(StandardMaterial {
            base_color: Color::srgb(0.3, 0.25, 0.4),
            perceptual_roughness: 0.8,
            ..default()
        });
        
        commands.spawn((
            Mesh3d(mesh_handle),
            MeshMaterial3d(material),
            Transform::from_xyz(offset.x + x, offset.y + 1.0, offset.z + z)
                .with_rotation(Quat::from_rotation_y(rng.gen::<f32>() * std::f32::consts::TAU)),
            Collider::cuboid(0.75, 1.0, 0.75),
            RigidBody::Fixed,
        ));
    }
}

fn spawn_sunken_city_objects(
    commands: &mut Commands,
    meshes: &mut Assets<Mesh>,
    materials: &mut Assets<StandardMaterial>,
    object_db: &ObjectDatabase,
    material_db: &MaterialDatabase,
    offset: Vec3,
) {
    let mut rng = rand::thread_rng();
    
    // Spawn sunken buildings - INCREASED DENSITY
    if let Some(building_meta) = object_db.objects.iter()
        .find(|obj| obj.object_id == "sunken_building_01") {
        
        for _ in 0..25 {  // Increased from 8
            let x = rng.gen_range(-200.0..200.0);
            let z = rng.gen_range(-200.0..200.0);
            
            // Vary building sizes
            let size_factor = rng.gen_range(0.6..1.4);
            let height_factor = rng.gen_range(0.8..1.5);
            
            let mesh_handle = meshes.add(Cuboid::new(8.0 * size_factor, 15.0 * height_factor, 8.0 * size_factor));
            let material = create_material_from_metadata(building_meta, material_db, materials);
            
            commands.spawn((
                Mesh3d(mesh_handle),
                MeshMaterial3d(material),
                Transform::from_xyz(offset.x + x, offset.y + 7.5 * height_factor - 10.0, offset.z + z)
                    .with_rotation(Quat::from_rotation_y(rng.gen::<f32>() * std::f32::consts::TAU)),
                WorldObject {
                    object_type: building_meta.object_id.clone(),
                    material_id: building_meta.material_id.clone(),
                    zone: WorldZone::SunkenCity,
                },
                create_collider(&building_meta.physics),
                RigidBody::Fixed,
            ));
        }
    }
    
    // Spawn gargoyle statues - INCREASED DENSITY
    if let Some(statue_meta) = object_db.objects.iter()
        .find(|obj| obj.object_id == "gargoyle_statue") {
        
        for _ in 0..60 {  // Increased from 15
            let x = rng.gen_range(-180.0..180.0);
            let z = rng.gen_range(-180.0..180.0);
            
            let mesh_handle = meshes.add(Cuboid::new(1.5, 2.0, 1.5));
            let material = create_material_from_metadata(statue_meta, material_db, materials);
            
            commands.spawn((
                Mesh3d(mesh_handle),
                MeshMaterial3d(material),
                Transform::from_xyz(offset.x + x, offset.y - 5.0 + rng.gen_range(0.0..3.0), offset.z + z),
                WorldObject {
                    object_type: statue_meta.object_id.clone(),
                    material_id: statue_meta.material_id.clone(),
                    zone: WorldZone::SunkenCity,
                },
                create_collider(&statue_meta.physics),
                RigidBody::Fixed,
            ));
        }
    }
    
    // Add sunken debris and rubble
    for _ in 0..150 {
        let x = rng.gen_range(-250.0..250.0);
        let z = rng.gen_range(-250.0..250.0);
        
        let size = rng.gen_range(0.5..2.0);
        let mesh_handle = meshes.add(Cuboid::new(size, size * 0.6, size));
        let material = materials.add(StandardMaterial {
            base_color: Color::srgb(0.3, 0.35, 0.4),
            perceptual_roughness: 0.9,
            ..default()
        });
        
        commands.spawn((
            Mesh3d(mesh_handle),
            MeshMaterial3d(material),
            Transform::from_xyz(offset.x + x, offset.y - 8.0 + rng.gen_range(0.0..2.0), offset.z + z)
                .with_rotation(Quat::from_euler(EulerRot::XYZ, 
                    rng.gen::<f32>() * 0.5, 
                    rng.gen::<f32>() * std::f32::consts::TAU, 
                    rng.gen::<f32>() * 0.5)),
            RigidBody::Fixed,
        ));
    }
    
    // Add broken columns and pillars
    for _ in 0..40 {
        let x = rng.gen_range(-200.0..200.0);
        let z = rng.gen_range(-200.0..200.0);
        
        let height = rng.gen_range(3.0..8.0);
        let mesh_handle = meshes.add(Cylinder::new(0.8, height));
        let material = materials.add(StandardMaterial {
            base_color: Color::srgb(0.45, 0.4, 0.35),
            perceptual_roughness: 0.8,
            ..default()
        });
        
        let tilt = rng.gen_range(0.0..0.3);
        commands.spawn((
            Mesh3d(mesh_handle),
            MeshMaterial3d(material),
            Transform::from_xyz(offset.x + x, offset.y - 5.0 + height * 0.5, offset.z + z)
                .with_rotation(Quat::from_euler(EulerRot::XYZ, tilt, rng.gen::<f32>() * std::f32::consts::TAU, tilt)),
            Collider::cylinder(height * 0.5, 0.8),
            RigidBody::Fixed,
        ));
    }
    
    // Add submerged vegetation (kelp, seaweed)
    for _ in 0..80 {
        let x = rng.gen_range(-220.0..220.0);
        let z = rng.gen_range(-220.0..220.0);
        
        let mesh_handle = meshes.add(Cuboid::new(0.3, 2.0, 0.3));
        let material = materials.add(StandardMaterial {
            base_color: Color::srgb(0.1, 0.3, 0.2),
            perceptual_roughness: 0.7,
            alpha_mode: AlphaMode::Blend,
            ..default()
        });
        
        commands.spawn((
            Mesh3d(mesh_handle),
            MeshMaterial3d(material),
            Transform::from_xyz(offset.x + x, offset.y - 7.0, offset.z + z),
        ));
    }
}

fn spawn_gravity_mines_objects(
    commands: &mut Commands,
    meshes: &mut Assets<Mesh>,
    materials: &mut Assets<StandardMaterial>,
    object_db: &ObjectDatabase,
    material_db: &MaterialDatabase,
    offset: Vec3,
) {
    let mut rng = rand::thread_rng();
    
    // Spawn floating rocks - INCREASED DENSITY
    if let Some(rock_meta) = object_db.objects.iter()
        .find(|obj| obj.object_id == "floating_rock_01") {
        
        for _ in 0..80 {  // Increased from 20
            let x = rng.gen_range(-200.0..200.0);
            let z = rng.gen_range(-200.0..200.0);
            let float_height = rng.gen_range(5.0..40.0);
            
            // Vary rock sizes
            let size_factor = rng.gen_range(0.3..2.0);
            
            let mesh_handle = meshes.add(Cuboid::new(3.0 * size_factor, 2.0 * size_factor, 3.0 * size_factor));
            let material = create_material_from_metadata(rock_meta, material_db, materials);
            
            commands.spawn((
                Mesh3d(mesh_handle),
                MeshMaterial3d(material),
                Transform::from_xyz(offset.x + x, offset.y + float_height, offset.z + z)
                    .with_rotation(Quat::from_rotation_y(rng.gen::<f32>() * std::f32::consts::TAU)),
                WorldObject {
                    object_type: rock_meta.object_id.clone(),
                    material_id: rock_meta.material_id.clone(),
                    zone: WorldZone::GravityMines,
                },
                create_collider(&rock_meta.physics),
                RigidBody::Dynamic,
                GravityScale(0.1),
            ))
            .with_children(|parent| {
                // Add faint glow to floating rocks
                if rng.gen_bool(0.3) {
                    parent.spawn((
                        PointLight {
                            intensity: 1000.0,
                            color: Color::srgb(0.6, 0.4, 0.8),
                            range: 10.0,
                            shadows_enabled: false,
                            ..default()
                        },
                        Transform::from_xyz(0.0, 0.0, 0.0),
                    ));
                }
            });
        }
    }
    
    // Spawn mining equipment - INCREASED DENSITY
    if let Some(equipment_meta) = object_db.objects.iter()
        .find(|obj| obj.object_id == "mining_equipment_01") {
        
        for _ in 0..40 {  // Increased from 10
            let x = rng.gen_range(-180.0..180.0);
            let z = rng.gen_range(-180.0..180.0);
            
            let mesh_handle = meshes.add(Cuboid::new(2.0, 3.0, 2.0));
            let material = create_material_from_metadata(equipment_meta, material_db, materials);
            
            commands.spawn((
                Mesh3d(mesh_handle),
                MeshMaterial3d(material),
                Transform::from_xyz(offset.x + x, offset.y + 1.5, offset.z + z)
                    .with_rotation(Quat::from_rotation_y(rng.gen::<f32>() * std::f32::consts::TAU)),
                WorldObject {
                    object_type: equipment_meta.object_id.clone(),
                    material_id: equipment_meta.material_id.clone(),
                    zone: WorldZone::GravityMines,
                },
                create_collider(&equipment_meta.physics),
                RigidBody::Fixed,
            ));
        }
    }
    
    // Add mine cart tracks
    for _ in 0..30 {
        let start_x = rng.gen_range(-150.0..150.0);
        let start_z = rng.gen_range(-150.0..150.0);
        let direction = Vec3::new(rng.gen_range(-1.0..1.0), 0.0, rng.gen_range(-1.0..1.0)).normalize();
        
        // Create track segments
        for i in 0..10 {
            let pos = Vec3::new(start_x, 0.0, start_z) + direction * (i as f32 * 3.0);
            
            let mesh_handle = meshes.add(Cuboid::new(3.0, 0.2, 1.5));
            let material = materials.add(StandardMaterial {
                base_color: Color::srgb(0.4, 0.3, 0.2),
                metallic: 0.7,
                perceptual_roughness: 0.4,
                ..default()
            });
            
            commands.spawn((
                Mesh3d(mesh_handle),
                MeshMaterial3d(material),
                Transform::from_xyz(offset.x + pos.x, offset.y + pos.y, offset.z + pos.z)
                    .with_rotation(Quat::from_rotation_y(direction.x.atan2(direction.z))),
                RigidBody::Fixed,
            ));
        }
    }
    
    // Add gravity anomaly zones (visual indicators)
    for _ in 0..50 {
        let x = rng.gen_range(-200.0..200.0);
        let z = rng.gen_range(-200.0..200.0);
        let y = rng.gen_range(0.0..30.0);
        
        let mesh_handle = meshes.add(Sphere::new(rng.gen_range(0.5..2.0)));
        let material = materials.add(StandardMaterial {
            base_color: Color::srgba(0.5, 0.3, 0.8, 0.3),
            alpha_mode: AlphaMode::Blend,
            unlit: true,
            ..default()
        });
        
        commands.spawn((
            Mesh3d(mesh_handle),
            MeshMaterial3d(material),
            Transform::from_xyz(offset.x + x, offset.y + y, offset.z + z),
        ));
    }
    
    // Add support beams and scaffolding
    for _ in 0..60 {
        let x = rng.gen_range(-180.0..180.0);
        let z = rng.gen_range(-180.0..180.0);
        
        let height = rng.gen_range(5.0..15.0);
        let mesh_handle = meshes.add(Cuboid::new(0.5, height, 0.5));
        let material = materials.add(StandardMaterial {
            base_color: Color::srgb(0.5, 0.4, 0.3),
            perceptual_roughness: 0.8,
            ..default()
        });
        
        commands.spawn((
            Mesh3d(mesh_handle),
            MeshMaterial3d(material),
            Transform::from_xyz(offset.x + x, offset.y + height * 0.5, offset.z + z),
            Collider::cuboid(0.25, height * 0.5, 0.25),
            RigidBody::Fixed,
        ));
    }
}

fn spawn_gilded_fortress_objects(
    commands: &mut Commands,
    meshes: &mut Assets<Mesh>,
    materials: &mut Assets<StandardMaterial>,
    object_db: &ObjectDatabase,
    material_db: &MaterialDatabase,
    offset: Vec3,
) {
    let mut rng = rand::thread_rng();
    
    // Spawn fortress walls - ENHANCED DESIGN
    if let Some(wall_meta) = object_db.objects.iter()
        .find(|obj| obj.object_id == "fortress_wall_segment") {
        
        // Create outer fortress perimeter
        for i in 0..16 {  // Increased segments
            let angle = i as f32 * std::f32::consts::TAU / 16.0;
            let x = angle.cos() * 150.0;
            let z = angle.sin() * 150.0;
            
            let mesh_handle = meshes.add(Cuboid::new(15.0, 12.0, 3.0));
            let material = create_material_from_metadata(wall_meta, material_db, materials);
            
            commands.spawn((
                Mesh3d(mesh_handle),
                MeshMaterial3d(material),
                Transform::from_xyz(offset.x + x, offset.y + 6.0, offset.z + z)
                    .with_rotation(Quat::from_rotation_y(angle)),
                WorldObject {
                    object_type: wall_meta.object_id.clone(),
                    material_id: wall_meta.material_id.clone(),
                    zone: WorldZone::GildedFortress,
                },
                create_collider(&wall_meta.physics),
                RigidBody::Fixed,
            ));
        }
        
        // Create inner walls
        for i in 0..8 {
            let angle = i as f32 * std::f32::consts::TAU / 8.0;
            let x = angle.cos() * 80.0;
            let z = angle.sin() * 80.0;
            
            let mesh_handle = meshes.add(Cuboid::new(12.0, 10.0, 2.5));
            let material = create_material_from_metadata(wall_meta, material_db, materials);
            
            commands.spawn((
                Mesh3d(mesh_handle),
                MeshMaterial3d(material),
                Transform::from_xyz(offset.x + x, offset.y + 5.0, offset.z + z)
                    .with_rotation(Quat::from_rotation_y(angle + std::f32::consts::PI / 8.0)),
                WorldObject {
                    object_type: wall_meta.object_id.clone(),
                    material_id: wall_meta.material_id.clone(),
                    zone: WorldZone::GildedFortress,
                },
                create_collider(&wall_meta.physics),
                RigidBody::Fixed,
            ));
        }
    }
    
    // Spawn golden statues - INCREASED DENSITY
    if let Some(statue_meta) = object_db.objects.iter()
        .find(|obj| obj.object_id == "golden_statue_01") {
        
        for _ in 0..50 {  // Increased from 12
            let x = rng.gen_range(-140.0..140.0);
            let z = rng.gen_range(-140.0..140.0);
            
            let size_factor = rng.gen_range(0.8..1.5);
            let mesh_handle = meshes.add(Cuboid::new(2.0 * size_factor, 5.0 * size_factor, 2.0 * size_factor));
            let material = create_material_from_metadata(statue_meta, material_db, materials);
            
            commands.spawn((
                Mesh3d(mesh_handle),
                MeshMaterial3d(material),
                Transform::from_xyz(offset.x + x, offset.y + 2.5 * size_factor, offset.z + z)
                    .with_rotation(Quat::from_rotation_y(rng.gen::<f32>() * std::f32::consts::TAU)),
                WorldObject {
                    object_type: statue_meta.object_id.clone(),
                    material_id: statue_meta.material_id.clone(),
                    zone: WorldZone::GildedFortress,
                },
                create_collider(&statue_meta.physics),
                RigidBody::Fixed,
            ))
            .with_children(|parent| {
                // Add golden glow
                if rng.gen_bool(0.4) {
                    parent.spawn((
                        PointLight {
                            intensity: 2000.0,
                            color: Color::srgb(1.0, 0.9, 0.6),
                            range: 15.0,
                            shadows_enabled: true,
                            ..default()
                        },
                        Transform::from_xyz(0.0, 3.0, 0.0),
                    ));
                }
            });
        }
    }
    
    // Add guard towers
    for i in 0..8 {
        let angle = i as f32 * std::f32::consts::TAU / 8.0;
        let x = angle.cos() * 120.0;
        let z = angle.sin() * 120.0;
        
        let mesh_handle = meshes.add(Cylinder::new(5.0, 20.0));
        let material = materials.add(StandardMaterial {
            base_color: Color::srgb(0.7, 0.65, 0.6),
            perceptual_roughness: 0.6,
            ..default()
        });
        
        commands.spawn((
            Mesh3d(mesh_handle),
            MeshMaterial3d(material),
            Transform::from_xyz(offset.x + x, offset.y + 10.0, offset.z + z),
            Collider::cylinder(10.0, 5.0),
            RigidBody::Fixed,
        ));
    }
    
    // Add banners and flags
    for _ in 0..40 {
        let x = rng.gen_range(-150.0..150.0);
        let z = rng.gen_range(-150.0..150.0);
        
        let mesh_handle = meshes.add(Cuboid::new(0.1, 4.0, 2.0));
        let material = materials.add(StandardMaterial {
            base_color: Color::srgb(0.8, 0.1, 0.2),
            perceptual_roughness: 0.7,
            double_sided: true,
            ..default()
        });
        
        commands.spawn((
            Mesh3d(mesh_handle),
            MeshMaterial3d(material),
            Transform::from_xyz(offset.x + x, offset.y + 8.0, offset.z + z)
                .with_rotation(Quat::from_rotation_y(rng.gen::<f32>() * std::f32::consts::TAU)),
        ));
    }
    
    // Add golden braziers
    for _ in 0..30 {
        let x = rng.gen_range(-130.0..130.0);
        let z = rng.gen_range(-130.0..130.0);
        
        let mesh_handle = meshes.add(Cylinder::new(1.5, 2.0));
        let material = materials.add(StandardMaterial {
            base_color: Color::srgb(0.9, 0.7, 0.3),
            metallic: 0.8,
            perceptual_roughness: 0.3,
            ..default()
        });
        
        commands.spawn((
            Mesh3d(mesh_handle),
            MeshMaterial3d(material),
            Transform::from_xyz(offset.x + x, offset.y + 1.0, offset.z + z),
            Collider::cylinder(1.0, 1.5),
            RigidBody::Fixed,
        ))
        .with_children(|parent| {
            // Add fire light
            parent.spawn((
                PointLight {
                    intensity: 4000.0,
                    color: Color::srgb(1.0, 0.7, 0.3),
                    range: 20.0,
                    shadows_enabled: true,
                    ..default()
                },
                Transform::from_xyz(0.0, 2.0, 0.0),
            ));
        });
    }
    
    // Add weapon racks and armor stands
    for _ in 0..60 {
        let x = rng.gen_range(-140.0..140.0);
        let z = rng.gen_range(-140.0..140.0);
        
        let mesh_handle = meshes.add(Cuboid::new(2.0, 3.0, 0.5));
        let material = materials.add(StandardMaterial {
            base_color: Color::srgb(0.4, 0.3, 0.2),
            perceptual_roughness: 0.8,
            ..default()
        });
        
        commands.spawn((
            Mesh3d(mesh_handle),
            MeshMaterial3d(material),
            Transform::from_xyz(offset.x + x, offset.y + 1.5, offset.z + z)
                .with_rotation(Quat::from_rotation_y(rng.gen::<f32>() * std::f32::consts::TAU)),
            Collider::cuboid(1.0, 1.5, 0.25),
            RigidBody::Fixed,
        ));
    }
}

fn spawn_crowns_heart_objects(
    commands: &mut Commands,
    meshes: &mut Assets<Mesh>,
    materials: &mut Assets<StandardMaterial>,
    object_db: &ObjectDatabase,
    material_db: &MaterialDatabase,
    offset: Vec3,
) {
    let mut rng = rand::thread_rng();
    
    // Spawn crown fragments - ENHANCED
    if let Some(crown_meta) = object_db.objects.iter()
        .find(|obj| obj.object_id == "crown_fragment_large") {
        
        // Large central fragments
        for i in 0..12 {
            let angle = i as f32 * std::f32::consts::TAU / 12.0;
            let radius = 80.0;
            let x = angle.cos() * radius;
            let z = angle.sin() * radius;
            let y = rng.gen_range(20.0..50.0);
            
            let size_factor = rng.gen_range(0.8..1.3);
            let mesh_handle = meshes.add(Cuboid::new(5.0 * size_factor, 10.0 * size_factor, 5.0 * size_factor));
            let material = create_material_from_metadata(crown_meta, material_db, materials);
            
            commands.spawn((
                Mesh3d(mesh_handle),
                MeshMaterial3d(material),
                Transform::from_xyz(offset.x + x, offset.y + y, offset.z + z)
                    .with_rotation(Quat::from_euler(EulerRot::XYZ, 
                        rng.gen::<f32>() * 0.3, 
                        angle, 
                        rng.gen::<f32>() * 0.3)),
                WorldObject {
                    object_type: crown_meta.object_id.clone(),
                    material_id: crown_meta.material_id.clone(),
                    zone: WorldZone::CrownsHeart,
                },
                RigidBody::KinematicVelocityBased,
                GravityScale(0.0),
            ))
            .with_children(|parent| {
                parent.spawn((
                    PointLight {
                        intensity: 10000.0,
                        color: Color::srgb(1.0, 0.9, 0.7),
                        range: 50.0,
                        shadows_enabled: true,
                        ..default()
                    },
                    Transform::from_xyz(0.0, 5.0, 0.0),
                ));
            });
        }
        
        // Smaller floating debris
        for _ in 0..60 {
            let x = rng.gen_range(-150.0..150.0);
            let z = rng.gen_range(-150.0..150.0);
            let y = rng.gen_range(10.0..80.0);
            
            let size = rng.gen_range(0.5..2.0);
            let mesh_handle = meshes.add(Cuboid::new(size, size * 1.5, size));
            let material = materials.add(StandardMaterial {
                base_color: Color::srgb(0.9, 0.85, 0.7),
                emissive: LinearRgba::new(0.8, 0.75, 0.6, 1.0),
                metallic: 0.7,
                perceptual_roughness: 0.3,
                ..default()
            });
            
            commands.spawn((
                Mesh3d(mesh_handle),
                MeshMaterial3d(material),
                Transform::from_xyz(offset.x + x, offset.y + y, offset.z + z)
                    .with_rotation(Quat::from_euler(EulerRot::XYZ, 
                        rng.gen::<f32>() * std::f32::consts::TAU,
                        rng.gen::<f32>() * std::f32::consts::TAU,
                        rng.gen::<f32>() * std::f32::consts::TAU)),
                RigidBody::KinematicVelocityBased,
                GravityScale(0.0),
            ));
        }
    }
    
    // Add energy streams between fragments
    for _ in 0..30 {
        let start_angle = rng.gen::<f32>() * std::f32::consts::TAU;
        let end_angle = rng.gen::<f32>() * std::f32::consts::TAU;
        let start_radius = rng.gen_range(60.0..100.0);
        let end_radius = rng.gen_range(60.0..100.0);
        
        let start_x = start_angle.cos() * start_radius;
        let start_z = start_angle.sin() * start_radius;
        let end_x = end_angle.cos() * end_radius;
        let end_z = end_angle.sin() * end_radius;
        
        let y = rng.gen_range(30.0..60.0);
        
        // Create energy beam visual
        let length = ((end_x - start_x).powi(2) + (end_z - start_z).powi(2)).sqrt();
        let angle = (end_z - start_z).atan2(end_x - start_x);
        
        let mesh_handle = meshes.add(Cuboid::new(length, 0.5, 0.5));
        let material = materials.add(StandardMaterial {
            base_color: Color::srgba(0.8, 0.7, 1.0, 0.6),
            emissive: LinearRgba::new(0.6, 0.5, 0.9, 1.0),
            alpha_mode: AlphaMode::Blend,
            unlit: true,
            ..default()
        });
        
        commands.spawn((
            Mesh3d(mesh_handle),
            MeshMaterial3d(material),
            Transform::from_xyz(
                offset.x + (start_x + end_x) / 2.0,
                offset.y + y,
                offset.z + (start_z + end_z) / 2.0
            ).with_rotation(Quat::from_rotation_y(angle)),
        ));
    }
    
    // Add central vortex effect
    let mesh_handle = meshes.add(Cylinder::new(15.0, 100.0));
    let material = materials.add(StandardMaterial {
        base_color: Color::srgba(0.7, 0.6, 0.9, 0.2),
        emissive: LinearRgba::new(0.5, 0.4, 0.8, 1.0),
        alpha_mode: AlphaMode::Blend,
        unlit: true,
        ..default()
    });
    
    commands.spawn((
        Mesh3d(mesh_handle),
        MeshMaterial3d(material),
        Transform::from_xyz(offset.x, offset.y + 50.0, offset.z),
    ))
    .with_children(|parent| {
        // Add powerful central light
        parent.spawn((
            PointLight {
                intensity: 50000.0,
                color: Color::srgb(0.9, 0.8, 1.0),
                range: 200.0,
                shadows_enabled: true,
                ..default()
            },
            Transform::from_xyz(0.0, 0.0, 0.0),
        ));
    });
    
    // Add floating platforms
    for _ in 0..20 {
        let x = rng.gen_range(-120.0..120.0);
        let z = rng.gen_range(-120.0..120.0);
        let y = rng.gen_range(5.0..40.0);
        
        let size_x = rng.gen_range(3.0..8.0);
        let size_z = rng.gen_range(3.0..8.0);
        
        let mesh_handle = meshes.add(Cuboid::new(size_x, 1.0, size_z));
        let material = materials.add(StandardMaterial {
            base_color: Color::srgb(0.7, 0.65, 0.8),
            metallic: 0.4,
            perceptual_roughness: 0.6,
            ..default()
        });
        
        commands.spawn((
            Mesh3d(mesh_handle),
            MeshMaterial3d(material),
            Transform::from_xyz(offset.x + x, offset.y + y, offset.z + z),
            Collider::cuboid(size_x * 0.5, 0.5, size_z * 0.5),
            RigidBody::KinematicVelocityBased,
            GravityScale(0.0),
        ));
    }
}

fn create_collider(physics: &PhysicsMetadata) -> Collider {
    if !physics.has_collision {
        return Collider::default();
    }
    
    match &physics.collision_shape {
        CollisionShape::Box { half_extents } => {
            Collider::cuboid(half_extents[0], half_extents[1], half_extents[2])
        }
        CollisionShape::Sphere { radius } => {
            Collider::ball(*radius)
        }
        CollisionShape::Capsule { height, radius } => {
            Collider::capsule_y(*height / 2.0, *radius)
        }
        CollisionShape::Mesh => {
            // For now, use a box as fallback
            // TODO: Implement actual mesh collider when mesh is available
            Collider::cuboid(1.0, 1.0, 1.0)
        }
    }
}