use bevy::prelude::*;
use bevy_rapier3d::prelude::*;
use crate::components::*;

pub fn enemy_ai_system(
    time: Res<Time>,
    mut enemy_query: Query<(Entity, &Transform, &mut AIState, &Enemy), (With<Enemy>, Without<Player>)>,
    player_query: Query<&Transform, With<Player>>,
    mut velocity_query: Query<&mut Velocity>,
) {
    let Ok(player_transform) = player_query.get_single() else {
        return;
    };

    for (enemy_entity, enemy_transform, mut ai_state, enemy) in enemy_query.iter_mut() {
        // 상태 타이머 감소
        ai_state.state_timer -= time.delta_secs();
        ai_state.attack_timer -= time.delta_secs();

        // 플레이어와의 거리 계산
        let distance_to_player = enemy_transform.translation.distance(player_transform.translation);
        let direction_to_player = (player_transform.translation - enemy_transform.translation).normalize();

        // AI 상태 머신
        match ai_state.current_state {
            AIStateType::Idle => {
                // 플레이어가 감지 범위에 들어오면 추적 시작
                if distance_to_player <= enemy.detection_range {
                    ai_state.current_state = AIStateType::Chase;
                    ai_state.target = Some(enemy_entity);
                    ai_state.last_known_player_position = player_transform.translation;
                }
                
                // Idle 상태에서는 가만히 있거나 순찰
                if ai_state.state_timer <= 0.0 {
                    ai_state.current_state = AIStateType::Patrol;
                    ai_state.state_timer = 3.0; // 3초간 순찰
                }
            }

            AIStateType::Patrol => {
                // 간단한 무작위 이동
                if ai_state.state_timer <= 0.0 {
                    ai_state.current_state = AIStateType::Idle;
                    ai_state.state_timer = 2.0; // 2초간 정지
                }

                // 플레이어 감지
                if distance_to_player <= enemy.detection_range {
                    ai_state.current_state = AIStateType::Chase;
                    ai_state.target = Some(enemy_entity);
                    ai_state.last_known_player_position = player_transform.translation;
                }
            }

            AIStateType::Chase => {
                // 플레이어 위치 업데이트
                if distance_to_player <= enemy.detection_range * 1.5 {
                    ai_state.last_known_player_position = player_transform.translation;
                }

                // 공격 범위에 들어오면 공격
                if distance_to_player <= enemy.attack_range {
                    ai_state.current_state = AIStateType::Attack;
                    ai_state.attack_timer = enemy.attack_cooldown;
                } 
                // 너무 멀어지면 마지막 알려진 위치로 이동
                else if distance_to_player > enemy.detection_range * 2.0 {
                    ai_state.current_state = AIStateType::SearchLastKnown;
                    ai_state.state_timer = 5.0; // 5초간 탐색
                }
                // 플레이어를 향해 이동
                else {
                    if let Ok(mut velocity) = velocity_query.get_mut(enemy_entity) {
                        velocity.linvel = direction_to_player * enemy.move_speed;
                        velocity.linvel.y = velocity.linvel.y; // Y축 속도 유지
                    }
                }
            }

            AIStateType::Attack => {
                // 공격 실행
                if ai_state.attack_timer <= 0.0 {
                    // 실제 공격 로직은 별도 시스템에서 처리
                    info!("Enemy attacks player!");
                    
                    // 공격 후 잠시 대기
                    ai_state.current_state = AIStateType::Chase;
                    ai_state.attack_timer = enemy.attack_cooldown;
                }

                // 플레이어가 공격 범위를 벗어나면 추적
                if distance_to_player > enemy.attack_range * 1.2 {
                    ai_state.current_state = AIStateType::Chase;
                }

                // 공격 중에는 움직이지 않음
                if let Ok(mut velocity) = velocity_query.get_mut(enemy_entity) {
                    velocity.linvel.x = 0.0;
                    velocity.linvel.z = 0.0;
                }
            }

            AIStateType::SearchLastKnown => {
                let distance_to_last_known = enemy_transform.translation.distance(ai_state.last_known_player_position);
                
                // 마지막 알려진 위치에 도달했거나 시간이 지나면 Idle로
                if distance_to_last_known < 1.0 || ai_state.state_timer <= 0.0 {
                    ai_state.current_state = AIStateType::Idle;
                    ai_state.state_timer = 2.0;
                    
                    if let Ok(mut velocity) = velocity_query.get_mut(enemy_entity) {
                        velocity.linvel.x = 0.0;
                        velocity.linvel.z = 0.0;
                    }
                } else {
                    // 마지막 알려진 위치로 이동
                    let direction_to_last_known = (ai_state.last_known_player_position - enemy_transform.translation).normalize();
                    if let Ok(mut velocity) = velocity_query.get_mut(enemy_entity) {
                        velocity.linvel = direction_to_last_known * enemy.move_speed * 0.5; // 천천히 이동
                        velocity.linvel.y = velocity.linvel.y; // Y축 속도 유지
                    }
                }

                // 플레이어를 다시 발견하면 추적
                if distance_to_player <= enemy.detection_range {
                    ai_state.current_state = AIStateType::Chase;
                    ai_state.last_known_player_position = player_transform.translation;
                }
            }

            AIStateType::Stunned => {
                // 스턴 상태에서는 움직이지 않음
                if let Ok(mut velocity) = velocity_query.get_mut(enemy_entity) {
                    velocity.linvel.x = 0.0;
                    velocity.linvel.z = 0.0;
                }

                if ai_state.state_timer <= 0.0 {
                    ai_state.current_state = AIStateType::Idle;
                    ai_state.state_timer = 1.0;
                }
            }
        }
    }
}

pub fn enemy_attack_system(
    mut commands: Commands,
    mut enemy_query: Query<(Entity, &Transform, &mut AIState, &Enemy), With<Enemy>>,
    mut player_query: Query<(Entity, &Transform, &mut Health), With<Player>>,
    mut meshes: ResMut<Assets<Mesh>>,
    mut materials: ResMut<Assets<StandardMaterial>>,
) {
    let Ok((player_entity, player_transform, mut player_health)) = player_query.get_single_mut() else {
        return;
    };

    for (enemy_entity, enemy_transform, mut ai_state, enemy) in enemy_query.iter_mut() {
        if matches!(ai_state.current_state, AIStateType::Attack) && ai_state.attack_timer <= 0.0 {
            let distance_to_player = enemy_transform.translation.distance(player_transform.translation);
            
            if distance_to_player <= enemy.attack_range {
                // 플레이어에게 데미지
                player_health.current -= enemy.attack_damage;
                info!("Player takes {} damage! Health: {}", enemy.attack_damage, player_health.current);

                // 공격 이펙트 생성 (간단한 빨간 구체)
                commands.spawn((
                    Mesh3d(meshes.add(Sphere::new(0.5).mesh().ico(2).unwrap())),
                    MeshMaterial3d(materials.add(StandardMaterial {
                        base_color: Color::srgb(1.0, 0.2, 0.2),
                        ..default()
                    })),
                    Transform::from_translation(player_transform.translation + Vec3::Y * 1.0),
                    AttackEffect { lifetime: 0.3 },
                ));

                // 플레이어가 죽었는지 확인
                if player_health.current <= 0.0 {
                    info!("Player died!");
                    // 사망 처리는 별도 시스템에서
                }
            }
        }
    }
}

#[derive(Component)]
pub struct AttackEffect {
    pub lifetime: f32,
}

pub fn attack_effect_system(
    time: Res<Time>,
    mut commands: Commands,
    mut effect_query: Query<(Entity, &mut AttackEffect)>,
) {
    for (entity, mut effect) in effect_query.iter_mut() {
        effect.lifetime -= time.delta_secs();
        
        if effect.lifetime <= 0.0 {
            commands.entity(entity).despawn();
        }
    }
}