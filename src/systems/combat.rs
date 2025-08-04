use bevy::prelude::*;
use bevy_rapier3d::prelude::*;
use crate::components::*;
use crate::resources::*;

pub fn combat_system(
    time: Res<Time>,
    mouse_input: Res<ButtonInput<MouseButton>>,
    input_mapping: Res<InputMapping>,
    mut commands: Commands,
    mut player_query: Query<(Entity, &Transform, &mut AttackState, &mut Stamina, &Weapon), With<Player>>,
    mut enemy_query: Query<(Entity, &Transform, &mut Health), (With<Enemy>, Without<Player>)>,
    mut meshes: ResMut<Assets<Mesh>>,
    mut materials: ResMut<Assets<StandardMaterial>>,
) {
    for (player_entity, player_transform, mut attack_state, mut stamina, weapon) in player_query.iter_mut() {
        // 공격 쿨다운 타이머 감소
        if attack_state.attack_timer > 0.0 {
            attack_state.attack_timer -= time.delta_secs();
            if attack_state.attack_timer <= 0.0 {
                attack_state.can_attack = true;
                attack_state.is_attacking = false;
            }
        }

        // 공격 입력 처리
        if mouse_input.just_pressed(input_mapping.attack) 
            && attack_state.can_attack 
            && stamina.current >= weapon.stamina_cost 
            && !attack_state.is_attacking 
        {
            // 공격 시작
            attack_state.is_attacking = true;
            attack_state.can_attack = false;
            attack_state.attack_timer = weapon.attack_cooldown;
            stamina.current -= weapon.stamina_cost;

            // 히트박스 생성
            spawn_attack_hitbox(
                &mut commands,
                player_entity,
                player_transform,
                weapon,
                &mut meshes,
                &mut materials,
            );
        }
    }
}

fn spawn_attack_hitbox(
    commands: &mut Commands,
    player_entity: Entity,
    player_transform: &Transform,
    weapon: &Weapon,
    meshes: &mut ResMut<Assets<Mesh>>,
    materials: &mut ResMut<Assets<StandardMaterial>>,
) {
    // 공격 방향 (플레이어 앞쪽)
    let attack_position = player_transform.translation + player_transform.forward() * (weapon.attack_range / 2.0);
    
    // 히트박스 엔티티 생성
    commands.spawn((
        Mesh3d(meshes.add(Sphere::new(weapon.attack_range / 2.0).mesh().ico(2).unwrap())),
        MeshMaterial3d(materials.add(StandardMaterial {
            base_color: Color::srgba(1.0, 0.0, 0.0, 0.3), // 반투명 빨간색
            alpha_mode: AlphaMode::Blend,
            ..default()
        })),
        Transform::from_translation(attack_position),
        Hitbox {
            radius: weapon.attack_range,
            damage: weapon.damage,
            active: true,
            lifetime: 0.2,
        },
        // 물리 센서로 설정 (충돌은 하지만 물리적 반응은 없음)
        Collider::ball(weapon.attack_range / 2.0),
        Sensor,
    ));
}

pub fn hitbox_system(
    time: Res<Time>,
    mut commands: Commands,
    mut hitbox_query: Query<(Entity, &Transform, &mut Hitbox)>,
    mut enemy_query: Query<(Entity, &Transform, &mut Health), With<Enemy>>,
) {
    for (hitbox_entity, hitbox_transform, mut hitbox) in hitbox_query.iter_mut() {
        // 히트박스 생명주기 관리
        hitbox.lifetime -= time.delta_secs();
        
        if hitbox.lifetime <= 0.0 {
            commands.entity(hitbox_entity).despawn();
            continue;
        }

        // 활성화된 히트박스만 처리
        if !hitbox.active {
            continue;
        }

        // 적과의 충돌 검사
        for (enemy_entity, enemy_transform, mut enemy_health) in enemy_query.iter_mut() {
            let distance = hitbox_transform.translation.distance(enemy_transform.translation);
            
            if distance <= hitbox.radius {
                // 데미지 적용
                enemy_health.current -= hitbox.damage;
                
                // 히트박스 비활성화 (한 번만 적용)
                hitbox.active = false;
                
                // 적이 죽었는지 확인
                if enemy_health.current <= 0.0 {
                    commands.entity(enemy_entity).despawn();
                    info!("Enemy defeated!");
                }
                
                info!("Hit enemy for {} damage! Enemy health: {}", hitbox.damage, enemy_health.current);
                break;
            }
        }
    }
}

pub fn weapon_system(
    mut player_query: Query<(&mut AttackState, &mut Weapon), With<Player>>,
) {
    // 무기 관련 로직 (내구도, 업그레이드 등)
    for (mut attack_state, mut weapon) in player_query.iter_mut() {
        // 현재는 기본 무기만 사용
        // 나중에 무기 교체, 업그레이드 등 구현 예정
    }
}