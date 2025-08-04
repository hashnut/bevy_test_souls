use bevy::prelude::*;
use bevy_rapier3d::prelude::*;
use crate::components::*;
use crate::resources::*;

pub fn player_movement(
    time: Res<Time>,
    keyboard_input: Res<ButtonInput<KeyCode>>,
    input_mapping: Res<InputMapping>,
    mut player_query: Query<(&mut Transform, &mut Velocity, &mut Player, &mut Stamina), With<Player>>,
    camera_query: Query<&Transform, (With<Camera3d>, Without<Player>)>,
) {
    let Ok(camera_transform) = camera_query.get_single() else {
        return;
    };
    
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

        // 카메라 기준으로 이동 방향 계산
        let camera_forward = camera_transform.forward().normalize();
        let camera_right = camera_transform.right().normalize();
        
        // Y축 성분을 제거하여 평면 이동만 허용
        let forward = Vec3::new(camera_forward.x, 0.0, camera_forward.z).normalize();
        let right = Vec3::new(camera_right.x, 0.0, camera_right.z).normalize();
        
        // 입력 처리
        if keyboard_input.pressed(input_mapping.move_forward) {
            direction += forward;
        }
        if keyboard_input.pressed(input_mapping.move_backward) {
            direction -= forward;
        }
        if keyboard_input.pressed(input_mapping.move_left) {
            direction -= right;
        }
        if keyboard_input.pressed(input_mapping.move_right) {
            direction += right;
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
            
            // 달리기 확인 (Shift 키)
            let is_running = keyboard_input.pressed(KeyCode::ShiftLeft);
            let current_speed = if is_running && stamina.current > 0.0 {
                // 달리기 중 스테미나 소모
                stamina.current = (stamina.current - 10.0 * time.delta_secs()).max(0.0);
                player.speed * 1.5  // 달리기 속도 (1.5배)
            } else {
                player.speed
            };
            
            velocity.linvel.x = direction.x * current_speed;
            velocity.linvel.z = direction.z * current_speed;
            
            // 캐릭터를 이동 방향으로 회전
            let target_rotation = Quat::from_rotation_arc(Vec3::Z, direction);
            transform.rotation = transform.rotation.slerp(target_rotation, 10.0 * time.delta_secs());
        } else {
            velocity.linvel.x = 0.0;
            velocity.linvel.z = 0.0;
            
            // 이동하지 않을 때는 카메라 방향을 바라봄
            let camera_dir = Vec3::new(camera_forward.x, 0.0, camera_forward.z).normalize();
            let target_rotation = Quat::from_rotation_arc(Vec3::Z, camera_dir);
            transform.rotation = transform.rotation.slerp(target_rotation, 5.0 * time.delta_secs());
        }
    }
}