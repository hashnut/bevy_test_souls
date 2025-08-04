use bevy::prelude::*;

#[derive(Resource)]
pub struct InputMapping {
    pub move_forward: KeyCode,
    pub move_backward: KeyCode,
    pub move_left: KeyCode,
    pub move_right: KeyCode,
    pub attack: MouseButton,
    pub roll: KeyCode,
    pub interact: KeyCode,
    pub pause: KeyCode,
    pub block: KeyCode,  // 방어/패링
}

impl Default for InputMapping {
    fn default() -> Self {
        Self {
            move_forward: KeyCode::KeyW,
            move_backward: KeyCode::KeyS,
            move_left: KeyCode::KeyA,
            move_right: KeyCode::KeyD,
            attack: MouseButton::Left,
            roll: KeyCode::Space,
            interact: KeyCode::KeyE,
            pause: KeyCode::Escape,
            block: KeyCode::ShiftLeft,  // Shift 키로 방어
        }
    }
}

#[derive(Resource)]
pub struct MouseLook {
    pub sensitivity: f32,
    pub locked: bool,
}

impl Default for MouseLook {
    fn default() -> Self {
        Self {
            sensitivity: 2.0,
            locked: false,
        }
    }
}