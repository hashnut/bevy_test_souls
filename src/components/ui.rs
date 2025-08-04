use bevy::prelude::*;

// UI 컴포넌트들
#[derive(Component)]
pub struct HealthBar;

#[derive(Component)]
pub struct StaminaBar;

#[derive(Component)]
pub struct SoulCounter;

#[derive(Component)]
pub struct GameUI;

#[derive(Component)]
pub struct PauseMenu;

#[derive(Component)]
pub struct DeathScreen;

// UI 업데이트를 위한 이벤트들
#[derive(Event)]
pub struct UpdateHealthUI {
    pub current: f32,
    pub max: f32,
}

#[derive(Event)]
pub struct UpdateStaminaUI {
    pub current: f32,
    pub max: f32,
}

#[derive(Event)]
pub struct UpdateSoulsUI {
    pub count: i32,
}

// UI 스타일 헬퍼 함수들
pub fn create_bar_style(width: f32, height: f32) -> Node {
    Node {
        width: Val::Px(width),
        height: Val::Px(height),
        margin: UiRect::all(Val::Px(5.0)),
        border: UiRect::all(Val::Px(2.0)),
        ..default()
    }
}

// Text style 헬퍼 함수는 제거하고 인라인으로 사용