use bevy::prelude::*;

#[derive(Component)]
pub struct Weapon {
    pub damage: f32,
    pub attack_range: f32,
    pub attack_cooldown: f32,
    pub stamina_cost: f32,
}

impl Default for Weapon {
    fn default() -> Self {
        Self {
            damage: 25.0,
            attack_range: 2.0,
            attack_cooldown: 1.0,
            stamina_cost: 15.0,
        }
    }
}

#[derive(Component, Default)]
pub struct AttackState {
    pub is_attacking: bool,
    pub attack_timer: f32,
    pub can_attack: bool,
}

#[derive(Component)]
pub struct Hitbox {
    pub radius: f32,
    pub damage: f32,
    pub active: bool,
    pub lifetime: f32,
}

impl Default for Hitbox {
    fn default() -> Self {
        Self {
            radius: 1.5,
            damage: 25.0,
            active: false,
            lifetime: 0.2,
        }
    }
}