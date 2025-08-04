use bevy::prelude::*;
use bevy_rapier3d::prelude::*;
use crate::components::{
    player::{Player, AnimationState, AnimationController, AnimationClips},
    combat::AttackState,
    stats::{Health, Stamina},
};
use crate::resources::InputMapping;

// 애니메이션 지속 시간
const ROLL_DURATION: f32 = 0.7;
const SLASH_DURATION: f32 = 0.8;
const PARRY_DURATION: f32 = 0.5;
const JUMP_DURATION: f32 = 1.0;
const DEATH_DURATION: f32 = 2.0;

// 애니메이션 FSM 업데이트
pub fn update_animation_state(
    mut player_query: Query<(
        &mut AnimationController,
        &Velocity,
        &Player,
        &Health,
        &Stamina,
        &AttackState,
    ), With<Player>>,
    keyboard_input: Res<ButtonInput<KeyCode>>,
    mouse_input: Res<ButtonInput<MouseButton>>,
    input_mapping: Res<InputMapping>,
    time: Res<Time>,
) {
    for (mut controller, velocity, player, health, stamina, attack_state) in player_query.iter_mut() {
        let previous_state = controller.current_state;
        
        // 타이머 업데이트
        controller.transition_timer.tick(time.delta());
        
        // 전환이 끝났으면 인터럽트 가능하도록
        if controller.transition_timer.finished() {
            controller.can_interrupt = true;
            controller.is_transitioning = false;
        }
        
        // 사망 상태 (최우선순위)
        if health.current <= 0.0 {
            if controller.current_state != AnimationState::Death {
                transition_to_state(&mut controller, AnimationState::Death, DEATH_DURATION, false);
            }
            continue;
        }
        
        // 인터럽트 불가능한 애니메이션 중이면 스킵
        if !controller.can_interrupt && !controller.transition_timer.finished() {
            continue;
        }
        
        // 구르기 (회피)
        if player.is_rolling {
            if controller.current_state != AnimationState::Roll {
                transition_to_state(&mut controller, AnimationState::Roll, ROLL_DURATION, false);
            }
            continue;
        }
        
        // 패링 (Shift + 우클릭)
        if keyboard_input.pressed(input_mapping.block) && mouse_input.just_pressed(MouseButton::Right) {
            if stamina.current >= 10.0 && controller.current_state != AnimationState::Parry {
                transition_to_state(&mut controller, AnimationState::Parry, PARRY_DURATION, false);
            }
            continue;
        }
        
        // 공격 (좌클릭)
        if attack_state.is_attacking {
            if controller.current_state != AnimationState::Slash {
                transition_to_state(&mut controller, AnimationState::Slash, SLASH_DURATION, false);
            }
            continue;
        }
        
        // 점프 (공중에 있을 때)
        if velocity.linvel.y > 1.0 {
            if controller.current_state != AnimationState::Jump {
                transition_to_state(&mut controller, AnimationState::Jump, JUMP_DURATION, true);
            }
            continue;
        }
        
        // 지상 이동 상태
        let horizontal_speed = Vec2::new(velocity.linvel.x, velocity.linvel.z).length();
        
        if horizontal_speed > 0.1 {
            // 달리기 (Shift + 이동)
            if keyboard_input.pressed(KeyCode::ShiftLeft) && horizontal_speed > 3.0 {
                if controller.current_state != AnimationState::Run {
                    transition_to_state(&mut controller, AnimationState::Run, 0.0, true);
                }
            } else {
                // 걷기
                if controller.current_state != AnimationState::Walk {
                    transition_to_state(&mut controller, AnimationState::Walk, 0.0, true);
                }
            }
        } else {
            // 대기 상태
            if controller.current_state != AnimationState::Idle {
                transition_to_state(&mut controller, AnimationState::Idle, 0.0, true);
            }
        }
        
        // 상태 변경 디버그
        if previous_state != controller.current_state {
            debug!("Animation state: {:?} -> {:?}", previous_state, controller.current_state);
        }
    }
}

// 상태 전환 헬퍼 함수
fn transition_to_state(
    controller: &mut AnimationController,
    new_state: AnimationState,
    duration: f32,
    interruptible: bool,
) {
    controller.previous_state = controller.current_state;
    controller.current_state = new_state;
    controller.is_transitioning = true;
    
    if duration > 0.0 {
        controller.transition_timer = Timer::from_seconds(duration, TimerMode::Once);
        controller.can_interrupt = interruptible;
    } else {
        controller.can_interrupt = true;
        controller.is_transitioning = false;
    }
}

// 애니메이션 재생 시스템
pub fn play_animations(
    player_query: Query<(
        Entity,
        &AnimationController,
        &AnimationClips,
    ), (With<Player>, Changed<AnimationController>)>,
    children_query: Query<&Children>,
    mut animation_players: Query<&mut AnimationPlayer>,
    mut animation_graphs: ResMut<Assets<AnimationGraph>>,
) {
    for (player_entity, controller, clips) in player_query.iter() {
        let clip_handle = match controller.current_state {
            AnimationState::Idle => clips.idle.clone(),
            AnimationState::Walk => clips.walk.clone(),
            AnimationState::Run => clips.run.clone(),
            AnimationState::Roll => clips.roll.clone(),
            AnimationState::Jump => clips.jump.clone(),
            AnimationState::Slash => clips.slash.clone(),
            AnimationState::Parry => clips.parry.clone(),
            AnimationState::Death => clips.death.clone(),
        };
        
        // 애니메이션 클립이 있을 때만 재생
        if let Some(clip) = clip_handle {
            // 플레이어의 자식들을 재귀적으로 탐색하여 AnimationPlayer 찾기
            if let Some(animation_player_entity) = find_animation_player_recursive(
                player_entity,
                &children_query,
                &animation_players,
            ) {
                if let Ok(mut animation_player) = animation_players.get_mut(animation_player_entity) {
                    let (graph, node_index) = AnimationGraph::from_clip(clip);
                    let _graph_handle = animation_graphs.add(graph);
                    
                    // 애니메이션 재생 설정
                    match controller.current_state {
                        // 반복 애니메이션
                        AnimationState::Idle | AnimationState::Walk | AnimationState::Run => {
                            animation_player.play(node_index).repeat();
                        },
                        // 한 번만 재생
                        _ => {
                            animation_player.play(node_index);
                        }
                    }
                    
                    info!("Playing animation: {:?} on entity {:?}", controller.current_state, animation_player_entity);
                }
            } else {
                warn!("No AnimationPlayer found for player entity {:?}", player_entity);
            }
        }
    }
}

// AnimationPlayer를 재귀적으로 찾는 헬퍼 함수
fn find_animation_player_recursive(
    entity: Entity,
    children_query: &Query<&Children>,
    animation_players: &Query<&mut AnimationPlayer>,
) -> Option<Entity> {
    // 현재 엔티티에 AnimationPlayer가 있는지 확인
    if animation_players.get(entity).is_ok() {
        return Some(entity);
    }
    
    // 자식들을 재귀적으로 확인
    if let Ok(children) = children_query.get(entity) {
        for &child in children.iter() {
            if let Some(player_entity) = find_animation_player_recursive(child, children_query, animation_players) {
                return Some(player_entity);
            }
        }
    }
    
    None
}

// Aurora 애니메이션 로드
pub fn load_aurora_animations(
    mut commands: Commands,
    asset_server: Res<AssetServer>,
    player_query: Query<Entity, (With<Player>, Without<AnimationClips>)>,
) {
    for player_entity in player_query.iter() {
        // 변환된 GLTF 애니메이션 파일 사용
        let clips = AnimationClips {
            idle: Some(asset_server.load("Animations_GLTF/Aurora/Idle.glb#Animation0")),                       // 대기
            walk: Some(asset_server.load("Animations_GLTF/Aurora/Jog_Fwd_Combat.glb#Animation0")),            // 걷기
            run: Some(asset_server.load("Animations_GLTF/Aurora/Jog_Fwd.glb#Animation0")),                    // 달리기
            roll: Some(asset_server.load("Animations_GLTF/Aurora/Ability_RMB_Fwd.glb#Animation0")),           // 회피
            jump: Some(asset_server.load("Animations_GLTF/Aurora/Jump_Start.glb#Animation0")),                // 점프
            slash: Some(asset_server.load("Animations_GLTF/Aurora/Primary_Attack_A.glb#Animation0")),         // 공격
            parry: Some(asset_server.load("Animations_GLTF/Aurora/HitReact_Front.glb#Animation0")),           // 패링
            death: Some(asset_server.load("Animations_GLTF/Aurora/Death.glb#Animation0")),                    // 사망
        };
        
        commands.entity(player_entity)
            .insert(clips)
            .insert(AnimationController::default());
            
        info!("Aurora animations loaded for player entity: {:?}", player_entity);
    }
}

// Scene에서 AnimationPlayer를 찾아서 연결하는 시스템
pub fn setup_animation_player(
    player_query: Query<Entity, (With<Player>, With<AnimationClips>)>,
    children_query: Query<&Children>,
    animation_player_query: Query<&mut AnimationPlayer>,
    name_query: Query<&Name>,
    mut processed: Local<std::collections::HashSet<Entity>>,
) {
    for player_entity in player_query.iter() {
        if processed.contains(&player_entity) {
            continue;
        }
        
        // 플레이어의 자식들 중에서 AnimationPlayer 찾기
        if let Some(animation_player_entity) = find_animation_player_recursive(
            player_entity,
            &children_query,
            &animation_player_query,
        ) {
            info!("Found AnimationPlayer {:?} for player {:?}", animation_player_entity, player_entity);
            processed.insert(player_entity);
        } else {
            // AnimationPlayer를 찾지 못했다면, Scene이 로드되길 기다려야 함
            // 디버그 정보 출력
            if let Ok(children) = children_query.get(player_entity) {
                if !children.is_empty() {
                    debug!("Player has {} children, but no AnimationPlayer found yet", children.len());
                    // 자식 엔티티들의 이름 확인
                    for &child in children.iter() {
                        if let Ok(name) = name_query.get(child) {
                            debug!("  Child: {} - {:?}", name.as_str(), child);
                        }
                    }
                }
            }
        }
    }
}