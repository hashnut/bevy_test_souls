use bevy::prelude::*;
use crate::components::player::Player;

#[derive(Component)]
pub struct HumanoidPart {
    pub part_type: BodyPart,
}

#[derive(Clone, Copy, PartialEq)]
pub enum BodyPart {
    Head,
    Torso,
    LeftArm,
    RightArm,
    LeftLeg,
    RightLeg,
}

pub fn create_procedural_humanoid(
    mut commands: Commands,
    mut meshes: ResMut<Assets<Mesh>>,
    mut materials: ResMut<Assets<StandardMaterial>>,
    player_query: Query<Entity, (With<Player>, Without<HumanoidPart>)>,
    children_query: Query<&Children>,
) {
    for player_entity in player_query.iter() {
        // 이미 자식이 있으면 스킵
        if children_query.get(player_entity).is_ok() {
            continue;
        }
        // 기존 캡슐 메시 제거
        commands.entity(player_entity).remove::<Mesh3d>();
        commands.entity(player_entity).remove::<MeshMaterial3d<StandardMaterial>>();

        // 인간형 파츠들을 자식으로 추가
        commands.entity(player_entity).with_children(|parent| {
            let skin_color = Color::srgb(0.96, 0.80, 0.69);
            let shirt_color = Color::srgb(0.2, 0.3, 0.7);
            let pants_color = Color::srgb(0.3, 0.3, 0.3);

            // 몸통 (Torso)
            parent.spawn((
                Mesh3d(meshes.add(Cuboid::new(0.8, 1.2, 0.4))),
                MeshMaterial3d(materials.add(StandardMaterial {
                    base_color: shirt_color,
                    ..default()
                })),
                Transform::from_xyz(0.0, 0.0, 0.0),
                HumanoidPart { part_type: BodyPart::Torso },
            ));

            // 머리 (Head)
            parent.spawn((
                Mesh3d(meshes.add(Sphere::new(0.3))),
                MeshMaterial3d(materials.add(StandardMaterial {
                    base_color: skin_color,
                    ..default()
                })),
                Transform::from_xyz(0.0, 0.9, 0.0),
                HumanoidPart { part_type: BodyPart::Head },
            ));

            // 왼팔 (Left Arm)
            parent.spawn((
                Mesh3d(meshes.add(Capsule3d::new(0.15, 0.8))),
                MeshMaterial3d(materials.add(StandardMaterial {
                    base_color: skin_color,
                    ..default()
                })),
                Transform::from_xyz(-0.6, 0.2, 0.0)
                    .with_rotation(Quat::from_rotation_z(0.2)),
                HumanoidPart { part_type: BodyPart::LeftArm },
            ));

            // 오른팔 (Right Arm)
            parent.spawn((
                Mesh3d(meshes.add(Capsule3d::new(0.15, 0.8))),
                MeshMaterial3d(materials.add(StandardMaterial {
                    base_color: skin_color,
                    ..default()
                })),
                Transform::from_xyz(0.6, 0.2, 0.0)
                    .with_rotation(Quat::from_rotation_z(-0.2)),
                HumanoidPart { part_type: BodyPart::RightArm },
            ));

            // 왼다리 (Left Leg)
            parent.spawn((
                Mesh3d(meshes.add(Capsule3d::new(0.2, 0.8))),
                MeshMaterial3d(materials.add(StandardMaterial {
                    base_color: pants_color,
                    ..default()
                })),
                Transform::from_xyz(-0.25, -1.0, 0.0),
                HumanoidPart { part_type: BodyPart::LeftLeg },
            ));

            // 오른다리 (Right Leg)
            parent.spawn((
                Mesh3d(meshes.add(Capsule3d::new(0.2, 0.8))),
                MeshMaterial3d(materials.add(StandardMaterial {
                    base_color: pants_color,
                    ..default()
                })),
                Transform::from_xyz(0.25, -1.0, 0.0),
                HumanoidPart { part_type: BodyPart::RightLeg },
            ));
        });

        info!("Procedural humanoid character created");
    }
}

pub fn animate_procedural_character(
    time: Res<Time>,
    player_query: Query<&Children, With<Player>>,
    mut part_query: Query<(&mut Transform, &HumanoidPart)>,
    velocity_query: Query<&bevy_rapier3d::prelude::Velocity, With<Player>>,
) {
    for children in player_query.iter() {
        if let Ok(velocity) = velocity_query.get_single() {
            let speed = velocity.linvel.length();
            
            for &child in children.iter() {
                if let Ok((mut transform, part)) = part_query.get_mut(child) {
                    match part.part_type {
                        BodyPart::LeftArm | BodyPart::RightArm => {
                            // 걷기/달리기 시 팔 스윙
                            if speed > 0.1 {
                                let swing = (time.elapsed_secs() * 5.0).sin() * 0.5;
                                let direction = if matches!(part.part_type, BodyPart::LeftArm) { 1.0 } else { -1.0 };
                                transform.rotation = Quat::from_rotation_x(swing * direction * (speed / 5.0).min(1.0));
                            }
                        }
                        BodyPart::LeftLeg | BodyPart::RightLeg => {
                            // 걷기/달리기 시 다리 움직임
                            if speed > 0.1 {
                                let stride = (time.elapsed_secs() * 5.0 + 
                                    if matches!(part.part_type, BodyPart::LeftLeg) { 0.0 } else { std::f32::consts::PI }
                                ).sin() * 0.6;
                                transform.rotation = Quat::from_rotation_x(stride * (speed / 5.0).min(1.0));
                            }
                        }
                        BodyPart::Head => {
                            // 머리는 약간의 흔들림만
                            let bob = (time.elapsed_secs() * 2.0).sin() * 0.05;
                            transform.translation.y = 0.9 + bob * (speed / 10.0).min(1.0);
                        }
                        _ => {}
                    }
                }
            }
        }
    }
}