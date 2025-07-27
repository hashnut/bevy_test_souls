use bevy::prelude::*;

#[derive(Component)]
pub struct Health {
    pub current: f32,
    pub max: f32,
}

impl Default for Health {
    fn default() -> Self {
        Self {
            current: 100.0,
            max: 100.0,
        }
    }
}

#[derive(Component)]
pub struct Stamina {
    pub current: f32,
    pub max: f32,
    pub regen_rate: f32,
}

impl Default for Stamina {
    fn default() -> Self {
        Self {
            current: 100.0,
            max: 100.0,
            regen_rate: 20.0,
        }
    }
}

#[derive(Component, Default)]
pub struct Souls {
    pub count: i32,
}

#[derive(Component)]
pub struct DeathMarker {
    pub soul_amount: i32,
    pub position: Vec3,
}