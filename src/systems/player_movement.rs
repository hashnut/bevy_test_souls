use bevy::prelude::*;
use bevy_rapier3d::prelude::*;
use crate::components::*;
use crate::resources::*;

pub fn player_movement(
    time: Res<Time>,
    keyboard_input: Res<ButtonInput<KeyCode>>,
    input_mapping: Res<InputMapping>,
    mut player_query: Query<(&mut Transform, &mut Velocity, &mut Player, &mut Stamina), With<Player>>,
) {
    for (mut transform, mut velocity, mut player, mut stamina) in player_query.iter_mut() {
        let mut direction = Vec3::ZERO;
        
        // 롤링 중이면 이동 제한
        if player.is_rolling {
            player.roll_timer -= time.delta_secs();
            if player.roll_timer <= 0.0 {
                player.is_rolling = false;
            }
            continue;
        }

        // 스테미나 회복
        stamina.current = (stamina.current + stamina.regen_rate * time.delta_secs()).min(stamina.max);

        // 입력 처리
        if keyboard_input.pressed(input_mapping.move_forward) {
            direction += *transform.forward();
        }
        if keyboard_input.pressed(input_mapping.move_backward) {
            direction -= *transform.forward();
        }
        if keyboard_input.pressed(input_mapping.move_left) {
            direction -= *transform.right();
        }
        if keyboard_input.pressed(input_mapping.move_right) {
            direction += *transform.right();
        }

        // 롤링 처리 - 스테미나 필요
        if keyboard_input.just_pressed(input_mapping.roll) && !player.is_rolling && stamina.current >= 20.0 {
            player.is_rolling = true;
            player.roll_timer = player.roll_duration;
            stamina.current -= 20.0; // 롤링 스테미나 소모
            
            // 롤링 방향 설정 (이동 방향이 없으면 앞쪽으로)
            let roll_direction = if direction.length() > 0.1 {
                direction.normalize()
            } else {
                *transform.forward()
            };
            
            velocity.linvel = roll_direction * player.speed * 2.0;
            continue;
        }

        // 일반 이동
        if direction.length() > 0.1 {
            direction = direction.normalize();
            velocity.linvel.x = direction.x * player.speed;
            velocity.linvel.z = direction.z * player.speed;
        } else {
            velocity.linvel.x = 0.0;
            velocity.linvel.z = 0.0;
        }
    }
}