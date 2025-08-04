use bevy::prelude::*;
use bevy_rapier3d::prelude::*;

mod components;
mod systems;
mod resources;
mod states;
mod utils;

use components::*;
use systems::*;
use resources::*;
use states::*;

fn main() {
    App::new()
        .add_plugins(DefaultPlugins.set(WindowPlugin {
            primary_window: Some(Window {
                title: "Dark Rust - Soulslike Game".into(),
                resolution: (1280.0, 720.0).into(),
                resizable: true,
                ..default()
            }),
            ..default()
        }))
        .add_plugins(RapierPhysicsPlugin::<NoUserData>::default())
        .add_plugins(RapierDebugRenderPlugin::default())
        .init_state::<GameState>()
        .init_state::<AssetLoaderState>()
        .insert_resource(ClearColor(Color::srgb(0.1, 0.1, 0.1)))
        .insert_resource(AmbientLight {
            color: Color::WHITE,
            brightness: 300.0,
        })
        .init_resource::<InputMapping>()
        .init_resource::<MouseLook>()
        .init_resource::<MyAssets>()
        .add_systems(Startup, (setup_scene, setup_ui, generate_procedural_map))
        // 플레이어 관련 시스템
        .add_systems(
            Update,
            (
                player_movement,
                handle_input,
                combat_system,
                hitbox_system,
                weapon_system,
                attack_effect_system,
            ).run_if(in_state(GameState::Playing))
        )
        // 카메라 시스템
        .add_systems(
            Update,
            (
                orbit_mouse,
                zoom_mouse,
                aim,
                toggle_offset,
                toggle_cursor,
            ).run_if(in_state(GameState::Playing))
        )
        // 적 AI 시스템
        .add_systems(
            Update,
            (
                enemy_ai_system,
                enemy_attack_system,
            ).run_if(in_state(GameState::Playing))
        )
        // UI 시스템
        .add_systems(
            Update,
            (
                update_health_ui,
                update_stamina_ui,
                update_souls_ui,
            ).run_if(in_state(GameState::Playing))
        )
        // 애니메이션 시스템
        .add_systems(
            Update,
            (
                update_animation_state,
                play_animations,
                load_aurora_animations,
                setup_animation_player,
            ).run_if(in_state(GameState::Playing))
        )
        // 애셋 로딩
        .add_systems(
            Update,
            load_assets.run_if(in_state(AssetLoaderState::Loading))
        )
        // 디버깅 시스템
        .add_systems(
            Update,
            (
                debug_scene_hierarchy,
                debug_model_visibility,
            ).run_if(in_state(GameState::Playing))
        )
        .add_systems(
            Update,
            debug_gltf_animations.run_if(in_state(AssetLoaderState::Done))
        )
        .add_systems(
            PostUpdate,
            sync_player_camera
                .run_if(in_state(GameState::Playing))
                .before(TransformSystem::TransformPropagate),
        )
        .add_systems(OnEnter(GameState::Paused), setup_pause_menu)
        .add_systems(OnExit(GameState::Paused), cleanup_pause_menu)
        .add_systems(OnEnter(GameState::Death), setup_death_screen)
        .add_systems(OnExit(GameState::Death), cleanup_death_screen)
        .add_systems(
            Update,
            handle_death_ui.run_if(in_state(GameState::Death))
        )
        .add_systems(
            OnEnter(AssetLoaderState::Done),
            update_gltf_scene
        )
        .add_systems(
            Update,
            (
                spawn_character_with_sword.run_if(in_state(AssetLoaderState::Done)),
                update_gltf_scene.run_if(in_state(AssetLoaderState::Done)),
                process_scene_objects.run_if(in_state(AssetLoaderState::Done)),
            )
        )
        .run();
}

fn setup_scene(
    mut commands: Commands,
    mut meshes: ResMut<Assets<Mesh>>,
    mut materials: ResMut<Assets<StandardMaterial>>,
) {
    // 프로시저럴 맵을 사용하므로 기본 바닥은 생성하지 않음
    // generate_procedural_map 시스템에서 바닥과 벽을 생성함

    // 플레이어 생성 - Y 오프셋을 낮춰서 지면에 더 가깝게
    let player_entity = commands.spawn((
        Transform::from_xyz(2.5, 0.5, 2.5),  // 중앙에서 약간 벗어난 안전한 위치
        Player::default(),
        Health { current: 100.0, max: 100.0 },
        Stamina { current: 100.0, max: 100.0, regen_rate: 20.0 },
        Souls::default(),
        Weapon::default(),
        AttackState::default(),
        RigidBody::Dynamic,
        Collider::capsule_y(1.0, 0.5),
        LockedAxes::ROTATION_LOCKED,
        Velocity::default(),
        GlobalTransform::default(),
        Visibility::default(),
    )).id();
    
    // 추가 컴포넌트들은 별도로 추가
    commands.entity(player_entity).insert((
        ThirdPersonCameraTarget,  // Third person camera target
        AnimationController::default(),  // 애니메이션 FSM 컨트롤러
        AnimationClips::default(),  // 애니메이션 클립들 (나중에 로드)
    ));

    // 임시 캡슐 메시는 제거 - Aurora 모델만 표시

    // 카메라 생성 - Third Person Camera
    commands.spawn((
        Camera3d::default(),
        Transform::from_xyz(0.0, 8.0, 10.0).looking_at(Vec3::ZERO, Vec3::Y),
        ThirdPersonCamera {
            zoom: Zoom::new(5.0, 15.0),  // 소울라이크 스타일 거리
            sensitivity: Vec2::new(0.5, 0.5),  // 민감도 조정
            aim_enabled: true,  // 조준 기능 활성화
            offset_enabled: true,  // 어깨 너머 시점
            offset_toggle_enabled: true,  // E키로 좌/우 전환
            ..default()
        },
    ));

    // 테스트용 적 생성 - Y 위치도 조정
    commands.spawn((
        Mesh3d(meshes.add(Capsule3d::default().mesh())),
        MeshMaterial3d(materials.add(StandardMaterial {
            base_color: Color::srgb(0.3, 0.8, 0.3),
            ..default()
        })),
        Transform::from_xyz(5.0, 0.5, 0.0),  // 1.0에서 0.5로 낮춤
        Enemy::default(),
        Health { current: 50.0, max: 50.0 },
        AIState::default(),
        RigidBody::Dynamic,
        Collider::capsule_y(1.0, 0.5),
        LockedAxes::ROTATION_LOCKED,
        Velocity::default(),
    ));

    // 조명 생성
    commands.spawn((
        DirectionalLight {
            shadows_enabled: true,
            ..default()
        },
        Transform::from_rotation(Quat::from_euler(EulerRot::XYZ, -0.5, -0.5, 0.0)),
    ));
}