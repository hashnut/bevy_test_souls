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
        
        // 피치 제한 (위에서 아래로 보는 각도 제한)
        controller.pitch = controller.pitch.clamp(-0.3, 0.3); // 약간의 각도 조절 가능
        
        // 30도 탑다운 뷰를 위한 기본 각도 (라디안으로 약 0.52 = 30도)
        let base_pitch = 0.52; // 30도
        let actual_pitch = controller.pitch + base_pitch;

        // 카메라 위치 계산
        // 수평 거리와 수직 거리를 따로 계산
        let horizontal_distance = controller.distance * actual_pitch.cos();
        let vertical_distance = controller.distance * actual_pitch.sin();
        
        // 카메라 위치 설정
        let offset = Vec3::new(
            horizontal_distance * controller.yaw.sin(),
            vertical_distance,
            horizontal_distance * controller.yaw.cos()
        );
        
        camera_transform.translation = player_transform.translation + offset + Vec3::Y * 1.0; // 캐릭터 중심에서 약간 위
        
        // 카메라가 플레이어를 바라보도록 설정
        let look_target = player_transform.translation + Vec3::Y * 1.2; // 캐릭터 상체를 바라봄
        camera_transform.look_at(look_target, Vec3::Y);
    }
}