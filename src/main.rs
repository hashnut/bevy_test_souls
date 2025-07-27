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
        .insert_resource(ClearColor(Color::srgb(0.1, 0.1, 0.1)))
        .insert_resource(AmbientLight {
            color: Color::WHITE,
            brightness: 300.0,
        })
        .init_resource::<InputMapping>()
        .init_resource::<MouseLook>()
        .add_systems(Startup, (setup_scene, setup_ui))
        .add_systems(
            Update,
            (
                player_movement,
                camera_follow,
                handle_input,
                combat_system,
                hitbox_system,
                weapon_system,
                enemy_ai_system,
                enemy_attack_system,
                attack_effect_system,
                update_health_ui,
                update_stamina_ui,
                update_souls_ui,
                handle_death_ui,
            ).run_if(in_state(GameState::Playing))
        )
        .add_systems(OnEnter(GameState::Paused), setup_pause_menu)
        .add_systems(OnExit(GameState::Paused), cleanup_pause_menu)
        .add_systems(OnEnter(GameState::Death), setup_death_screen)
        .add_systems(OnExit(GameState::Death), cleanup_death_screen)
        .run();
}

fn setup_scene(
    mut commands: Commands,
    mut meshes: ResMut<Assets<Mesh>>,
    mut materials: ResMut<Assets<StandardMaterial>>,
) {
    // 바닥 생성
    commands.spawn((
        Mesh3d(meshes.add(Plane3d::default().mesh().size(20.0, 20.0))),
        MeshMaterial3d(materials.add(StandardMaterial {
            base_color: Color::srgb(0.5, 0.5, 0.5),
            ..default()
        })),
        Transform::from_xyz(0.0, 0.0, 0.0),
        RigidBody::Fixed,
        Collider::cuboid(10.0, 0.1, 10.0),
    ));

    // 플레이어 생성
    let player_entity = commands.spawn((
        Mesh3d(meshes.add(Capsule3d::default().mesh())),
        MeshMaterial3d(materials.add(StandardMaterial {
            base_color: Color::srgb(0.8, 0.3, 0.3),
            ..default()
        })),
        Transform::from_xyz(0.0, 1.0, 0.0),
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
    )).id();

    // 카메라 생성
    commands.spawn((
        Camera3d::default(),
        Transform::from_xyz(0.0, 5.0, 5.0).looking_at(Vec3::ZERO, Vec3::Y),
        CameraController { target: Some(player_entity), ..default() },
    ));

    // 테스트용 적 생성
    commands.spawn((
        Mesh3d(meshes.add(Capsule3d::default().mesh())),
        MeshMaterial3d(materials.add(StandardMaterial {
            base_color: Color::srgb(0.3, 0.8, 0.3),
            ..default()
        })),
        Transform::from_xyz(5.0, 1.0, 0.0),
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