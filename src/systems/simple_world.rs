use bevy::prelude::*;
use bevy_rapier3d::prelude::*;

pub fn setup_simple_world(
    mut commands: Commands,
    mut meshes: ResMut<Assets<Mesh>>,
    mut materials: ResMut<Assets<StandardMaterial>>,
) {
    // Add directional light (sun)
    commands.spawn((
        DirectionalLight {
            illuminance: 10000.0,
            shadows_enabled: true,
            ..default()
        },
        Transform::from_xyz(100.0, 1000.0, 100.0)
            .looking_at(Vec3::ZERO, Vec3::Y),
    ));
    
    // Create a large ground plane with better texture
    commands.spawn((
        Mesh3d(meshes.add(Plane3d::default().mesh().size(500.0, 500.0))),
        MeshMaterial3d(materials.add(StandardMaterial {
            base_color: Color::srgb(0.35, 0.3, 0.25),
            perceptual_roughness: 0.95,
            ..default()
        })),
        Transform::from_xyz(0.0, 0.0, 0.0),
        RigidBody::Fixed,
        Collider::cuboid(250.0, 0.1, 250.0),
    ));
}

