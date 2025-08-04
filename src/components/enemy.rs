use bevy::prelude::*;

#[derive(Component)]
pub struct Enemy {
    pub ai_type: EnemyType,
    pub detection_range: f32,
    pub attack_range: f32,
    pub move_speed: f32,
    pub attack_damage: f32,
    pub attack_cooldown: f32,
}

impl Default for Enemy {
    fn default() -> Self {
        Self {
            ai_type: EnemyType::BasicMelee,
            detection_range: 8.0,
            attack_range: 2.0,
            move_speed: 3.0,
            attack_damage: 20.0,
            attack_cooldown: 2.0,
        }
    }
}

#[derive(Debug, Clone, Copy)]
pub enum EnemyType {
    BasicMelee,
    Archer,
    Boss,
}

#[derive(Component, Default)]
pub struct AIState {
    pub current_state: AIStateType,
    pub target: Option<Entity>,
    pub state_timer: f32,
    pub attack_timer: f32,
    pub last_known_player_position: Vec3,
}

#[derive(Debug, Clone, Copy, Default)]
pub enum AIStateType {
    #[default]
    Idle,
    Patrol,
    Chase,
    Attack,
    SearchLastKnown,
    Stunned,
}

#[derive(Component)]
pub struct PatrolPath {
    pub points: Vec<Vec3>,
    pub current_index: usize,
    pub patrol_speed: f32,
}

impl Default for PatrolPath {
    fn default() -> Self {
        Self {
            points: vec![],
            current_index: 0,
            patrol_speed: 1.5,
        }
    }
}