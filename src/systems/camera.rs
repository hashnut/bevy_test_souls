use bevy::prelude::*;
use bevy::input::mouse::MouseMotion;
use crate::components::*;
use crate::resources::*;

pub fn camera_follow(
    mut camera_query: Query<(&mut Transform, &mut CameraController), (With<Camera3d>, Without<Player>)>,
    player_query: Query<&Transform, (With<Player>, Without<Camera3d>)>,
    mut mouse_motion: EventReader<MouseMotion>,
    mouse_look: Res<MouseLook>,
    time: Res<Time>,
) {
    let Ok(player_transform) = player_query.get_single() else {
        return;
    };

    for (mut camera_transform, mut controller) in camera_query.iter_mut() {
        // 마우스 입력으로 카메라 회전 (마우스가 잠겨있지 않을 때만)
        if !mouse_look.locked {
            for motion in mouse_motion.read() {
                controller.yaw -= motion.delta.x * mouse_look.sensitivity * time.delta_secs();
                controller.pitch -= motion.delta.y * mouse_look.sensitivity * time.delta_secs();
            }
        }
        
        // 피치 제한
        controller.pitch = controller.pitch.clamp(-1.5, 1.5);

        // 카메라 위치 계산
        let rotation = Quat::from_euler(EulerRot::YXZ, controller.yaw, controller.pitch, 0.0);
        let offset = rotation * Vec3::new(0.0, 2.0, controller.distance);
        
        camera_transform.translation = player_transform.translation + offset;
        camera_transform.look_at(player_transform.translation + Vec3::Y * 1.5, Vec3::Y);
    }
}