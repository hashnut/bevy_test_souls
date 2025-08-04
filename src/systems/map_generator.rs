use bevy::prelude::*;
use bevy_rapier3d::prelude::*;
use crate::components::map::*;
use super::map_data::*;


pub fn generate_procedural_map(
    mut commands: Commands,
    mut meshes: ResMut<Assets<Mesh>>,
    mut materials: ResMut<Assets<StandardMaterial>>,
    mut generated: Local<bool>,
) {
    if *generated {
        return;
    }
    *generated = true;

    info!("Generating procedural map...");

    // 맵 설정과 재질 로드
    let config = MapConfig::default();
    let elements = MapElements::default();
    let map_materials = MapMaterials::new(&mut materials);

    // 바닥 생성
    generate_floor(&mut commands, &mut meshes, &config, &map_materials);

    // 외벽 생성
    generate_walls(&mut commands, &mut meshes, &config, &map_materials);

    // 기둥과 장애물 생성
    generate_pillars(&mut commands, &mut meshes, &config, &elements, &map_materials);
    generate_obstacles(&mut commands, &mut meshes, &config, &elements, &map_materials);

    // 체크포인트 생성
    generate_checkpoints(&mut commands, &mut meshes, &mut materials, &elements);

    // 조명 생성
    generate_lights(&mut commands, &elements);

    info!("Procedural map generated successfully!");
}

// 바닥 생성 함수
fn generate_floor(
    commands: &mut Commands,
    meshes: &mut ResMut<Assets<Mesh>>,
    config: &MapConfig,
    materials: &MapMaterials,
) {
    for x in 0..config.grid_size {
        for z in 0..config.grid_size {
            let x_pos = (x as f32 - (config.grid_size as f32 / 2.0 - 0.5)) * config.tile_size;
            let z_pos = (z as f32 - (config.grid_size as f32 / 2.0 - 0.5)) * config.tile_size;
            
            commands.spawn((
                Mesh3d(meshes.add(Cuboid::new(config.tile_size, 0.2, config.tile_size))),
                MeshMaterial3d(materials.floor_material.clone()),
                Transform::from_xyz(x_pos, -0.1, z_pos),
                RigidBody::Fixed,
                Collider::cuboid(config.tile_size / 2.0, 0.1, config.tile_size / 2.0),
                Floor,
                MapTile,
                Name::new(format!("Floor_{}_{}", x, z)),
            ));
        }
    }
}

// 벽 생성 함수
fn generate_walls(
    commands: &mut Commands,
    meshes: &mut ResMut<Assets<Mesh>>,
    config: &MapConfig,
    materials: &MapMaterials,
) {
    // 북쪽 벽
    for x in 0..config.grid_size {
        let x_pos = (x as f32 - (config.grid_size as f32 / 2.0 - 0.5)) * config.tile_size;
        commands.spawn((
            Mesh3d(meshes.add(Cuboid::new(config.tile_size, config.wall_height, config.wall_thickness))),
            MeshMaterial3d(materials.wall_material.clone()),
            Transform::from_xyz(x_pos, config.wall_height / 2.0, -config.map_size / 2.0),
            RigidBody::Fixed,
            Collider::cuboid(config.tile_size / 2.0, config.wall_height / 2.0, config.wall_thickness / 2.0),
            Wall,
            MapTile,
            Name::new(format!("Wall_North_{}", x)),
        ));
    }

    // 남쪽 벽
    for x in 0..config.grid_size {
        let x_pos = (x as f32 - (config.grid_size as f32 / 2.0 - 0.5)) * config.tile_size;
        commands.spawn((
            Mesh3d(meshes.add(Cuboid::new(config.tile_size, config.wall_height, config.wall_thickness))),
            MeshMaterial3d(materials.wall_material.clone()),
            Transform::from_xyz(x_pos, config.wall_height / 2.0, config.map_size / 2.0),
            RigidBody::Fixed,
            Collider::cuboid(config.tile_size / 2.0, config.wall_height / 2.0, config.wall_thickness / 2.0),
            Wall,
            MapTile,
            Name::new(format!("Wall_South_{}", x)),
        ));
    }

    // 동쪽 벽
    for z in 0..config.grid_size {
        let z_pos = (z as f32 - (config.grid_size as f32 / 2.0 - 0.5)) * config.tile_size;
        commands.spawn((
            Mesh3d(meshes.add(Cuboid::new(config.wall_thickness, config.wall_height, config.tile_size))),
            MeshMaterial3d(materials.wall_material.clone()),
            Transform::from_xyz(config.map_size / 2.0, config.wall_height / 2.0, z_pos),
            RigidBody::Fixed,
            Collider::cuboid(config.wall_thickness / 2.0, config.wall_height / 2.0, config.tile_size / 2.0),
            Wall,
            MapTile,
            Name::new(format!("Wall_East_{}", z)),
        ));
    }

    // 서쪽 벽
    for z in 0..config.grid_size {
        let z_pos = (z as f32 - (config.grid_size as f32 / 2.0 - 0.5)) * config.tile_size;
        commands.spawn((
            Mesh3d(meshes.add(Cuboid::new(config.wall_thickness, config.wall_height, config.tile_size))),
            MeshMaterial3d(materials.wall_material.clone()),
            Transform::from_xyz(-config.map_size / 2.0, config.wall_height / 2.0, z_pos),
            RigidBody::Fixed,
            Collider::cuboid(config.wall_thickness / 2.0, config.wall_height / 2.0, config.tile_size / 2.0),
            Wall,
            MapTile,
            Name::new(format!("Wall_West_{}", z)),
        ));
    }
}

// 기둥 생성 함수
fn generate_pillars(
    commands: &mut Commands,
    meshes: &mut ResMut<Assets<Mesh>>,
    config: &MapConfig,
    elements: &MapElements,
    materials: &MapMaterials,
) {
    for (i, (x, z)) in elements.pillar_positions.iter().enumerate() {
        commands.spawn((
            Mesh3d(meshes.add(Cylinder::new(0.5, config.wall_height))),
            MeshMaterial3d(materials.pillar_material.clone()),
            Transform::from_xyz(*x, config.wall_height / 2.0, *z),
            RigidBody::Fixed,
            Collider::cylinder(config.wall_height / 2.0, 0.5),
            Pillar,
            MapTile,
            Name::new(format!("Pillar_{}", i)),
        ));
    }
}

// 장애물 생성 함수
fn generate_obstacles(
    commands: &mut Commands,
    meshes: &mut ResMut<Assets<Mesh>>,
    _config: &MapConfig,
    elements: &MapElements,
    materials: &MapMaterials,
) {
    for (i, (x, z)) in elements.obstacle_positions.iter().enumerate() {
        commands.spawn((
            Mesh3d(meshes.add(Cuboid::new(2.0, 1.5, 2.0))),
            MeshMaterial3d(materials.wall_material.clone()),
            Transform::from_xyz(*x, 0.75, *z),
            RigidBody::Fixed,
            Collider::cuboid(1.0, 0.75, 1.0),
            MapTile,
            Name::new(format!("Obstacle_{}", i)),
        ));
    }
}

// 체크포인트 생성 함수
fn generate_checkpoints(
    commands: &mut Commands,
    meshes: &mut ResMut<Assets<Mesh>>,
    materials: &mut ResMut<Assets<StandardMaterial>>,
    elements: &MapElements,
) {
    for (i, (x, y, z)) in elements.checkpoint_positions.iter().enumerate() {
        commands.spawn((
            Mesh3d(meshes.add(Cuboid::new(0.5, 2.0, 0.5))),
            MeshMaterial3d(materials.add(StandardMaterial {
                base_color: Color::srgb(0.8, 0.6, 0.2),
                emissive: LinearRgba::new(0.8, 0.6, 0.2, 1.0),
                ..default()
            })),
            Transform::from_xyz(*x, *y, *z),
            CheckpointMarker,
            Name::new(format!("Checkpoint_{}", i + 1)),
        ));
    }
}

// 조명 생성 함수
fn generate_lights(
    commands: &mut Commands,
    elements: &MapElements,
) {
    // 메인 조명
    commands.spawn((
        PointLight {
            color: Color::srgb(1.0, 0.8, 0.6),
            intensity: 500_000.0,
            radius: 20.0,
            shadows_enabled: true,
            ..default()
        },
        Transform::from_xyz(0.0, 5.0, 0.0),
        Name::new("MainLight"),
    ));

    // 체크포인트 조명
    for (i, (x, _y, z)) in elements.checkpoint_positions.iter().enumerate() {
        commands.spawn((
            PointLight {
                color: Color::srgb(0.8, 0.6, 0.2),
                intensity: 200_000.0,
                radius: 10.0,
                ..default()
            },
            Transform::from_xyz(*x, 3.0, *z),
            Name::new(format!("CheckpointLight_{}", i + 1)),
        ));
    }
}

// 맵을 정리하는 함수
pub fn cleanup_map(
    mut commands: Commands,
    map_query: Query<Entity, With<MapTile>>,
) {
    for entity in map_query.iter() {
        commands.entity(entity).despawn_recursive();
    }
}