use bevy::prelude::*;

#[derive(Component)]
pub struct Player {
    pub speed: f32,
    pub is_rolling: bool,
    pub roll_timer: f32,
    pub roll_duration: f32,
}

impl Default for Player {
    fn default() -> Self {
        Self {
            speed: 5.0,
            is_rolling: false,
            roll_timer: 0.0,
            roll_duration: 0.5,
        }
    }
}

// 애니메이션 FSM 상태
#[derive(Component, Debug, Clone, Copy, PartialEq)]
pub enum AnimationState {
    Idle,
    Walk,
    Run,
    Roll,
    Jump,
    Slash,
    Parry,
    Death,
}

impl Default for AnimationState {
    fn default() -> Self {
        AnimationState::Idle
    }
}

// 애니메이션 상태 전환 관리
#[derive(Component)]
pub struct AnimationController {
    pub current_state: AnimationState,
    pub previous_state: AnimationState,
    pub transition_timer: Timer,
    pub can_interrupt: bool,
    pub is_transitioning: bool,
}

impl Default for AnimationController {
    fn default() -> Self {
        Self {
            current_state: AnimationState::Idle,
            previous_state: AnimationState::Idle,
            transition_timer: Timer::from_seconds(0.1, TimerMode::Once),
            can_interrupt: true,
            is_transitioning: false,
        }
    }
}

// 애니메이션 클립들
#[derive(Component, Default)]
pub struct AnimationClips {
    pub idle: Option<Handle<AnimationClip>>,
    pub walk: Option<Handle<AnimationClip>>,
    pub run: Option<Handle<AnimationClip>>,
    pub roll: Option<Handle<AnimationClip>>,
    pub jump: Option<Handle<AnimationClip>>,
    pub slash: Option<Handle<AnimationClip>>,
    pub parry: Option<Handle<AnimationClip>>,
    pub death: Option<Handle<AnimationClip>>,
}

#[derive(Component)]
pub struct CameraController {
    pub distance: f32,
    pub sensitivity: f32,
    pub pitch: f32,
    pub yaw: f32,
    pub target: Option<Entity>,
}

impl Default for CameraController {
    fn default() -> Self {
        Self {
            distance: 8.0,  // 30도 각도에 적합한 거리
            sensitivity: 2.0,
            pitch: 0.0,     // 기본 피치를 0으로 (30도는 카메라 시스템에서 추가)
            yaw: 0.0,
            target: None,
        }
    }
}