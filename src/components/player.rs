use bevy::prelude::*;

#[derive(Component)]
pub struct Player {
    pub speed: f32,
    pub is_rolling: bool,
    pub roll_timer: f32,
    pub roll_duration: f32,
}

impl Default for Player {
    fn default() -> Self {
        Self {
            speed: 5.0,
            is_rolling: false,
            roll_timer: 0.0,
            roll_duration: 0.5,
        }
    }
}

#[derive(Component)]
pub struct CameraController {
    pub distance: f32,
    pub sensitivity: f32,
    pub pitch: f32,
    pub yaw: f32,
    pub target: Option<Entity>,
}

impl Default for CameraController {
    fn default() -> Self {
        Self {
            distance: 8.0,
            sensitivity: 2.0,
            pitch: 0.3,
            yaw: 0.0,
            target: None,
        }
    }
}