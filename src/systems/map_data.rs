use bevy::prelude::*;

// 맵 설정 데이터
pub struct MapConfig {
    pub map_size: f32,
    pub tile_size: f32,
    pub wall_height: f32,
    pub wall_thickness: f32,
    pub grid_size: usize,
}

impl Default for MapConfig {
    fn default() -> Self {
        Self {
            map_size: 30.0,
            tile_size: 5.0,
            wall_height: 4.0,
            wall_thickness: 0.5,
            grid_size: 6,
        }
    }
}

// 맵 요소 위치 데이터
pub struct MapElements {
    pub pillar_positions: Vec<(f32, f32)>,
    pub obstacle_positions: Vec<(f32, f32)>,
    pub checkpoint_positions: Vec<(f32, f32, f32)>,
    pub enemy_spawn_positions: Vec<(f32, f32, f32)>,
}

impl Default for MapElements {
    fn default() -> Self {
        Self {
            pillar_positions: vec![
                (-10.0, 10.0),
                (10.0, 10.0),
                (-10.0, -10.0),
                (10.0, -10.0),
            ],
            obstacle_positions: vec![
                (-5.0, 5.0),
                (5.0, -5.0),
                (-7.5, -2.5),
                (7.5, 2.5),
            ],
            checkpoint_positions: vec![
                (-10.0, 0.0, 1.0),
            ],
            enemy_spawn_positions: vec![
                (5.0, 0.0, 0.5),
                (-5.0, 7.0, 0.5),
                (7.0, -5.0, 0.5),
            ],
        }
    }
}

// 머티리얼 설정
pub struct MapMaterials {
    pub floor_material: Handle<StandardMaterial>,
    pub wall_material: Handle<StandardMaterial>,
    pub pillar_material: Handle<StandardMaterial>,
    pub checkpoint_material: Handle<StandardMaterial>,
}

impl MapMaterials {
    pub fn new(materials: &mut ResMut<Assets<StandardMaterial>>) -> Self {
        Self {
            floor_material: materials.add(StandardMaterial {
                base_color: Color::srgb(0.3, 0.3, 0.3),
                perceptual_roughness: 0.8,
                metallic: 0.1,
                ..default()
            }),
            wall_material: materials.add(StandardMaterial {
                base_color: Color::srgb(0.2, 0.2, 0.25),
                perceptual_roughness: 0.9,
                metallic: 0.0,
                ..default()
            }),
            pillar_material: materials.add(StandardMaterial {
                base_color: Color::srgb(0.25, 0.25, 0.3),
                perceptual_roughness: 0.7,
                metallic: 0.2,
                ..default()
            }),
            checkpoint_material: materials.add(StandardMaterial {
                base_color: Color::srgb(0.8, 0.6, 0.2),
                emissive: LinearRgba::new(0.8, 0.6, 0.2, 1.0),
                ..default()
            }),
        }
    }
}