use bevy::prelude::*;
// use crate::components::*;
use crate::resources::*;
use crate::states::*;

pub fn handle_input(
    keyboard_input: Res<ButtonInput<KeyCode>>,
    _mouse_input: Res<ButtonInput<MouseButton>>,
    input_mapping: Res<InputMapping>,
    mut mouse_look: ResMut<MouseLook>,
    mut next_state: ResMut<NextState<GameState>>,
    current_state: Res<State<GameState>>,
) {
    // ESC 키로 게임 일시정지/재개
    if keyboard_input.just_pressed(input_mapping.pause) {
        match current_state.get() {
            GameState::Playing => next_state.set(GameState::Paused),
            GameState::Paused => next_state.set(GameState::Playing),
            _ => {}
        }
    }
    
    // 마우스 락/언락 (Alt 키)
    if keyboard_input.just_pressed(KeyCode::AltLeft) {
        mouse_look.locked = !mouse_look.locked;
    }
}