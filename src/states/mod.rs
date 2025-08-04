use bevy::prelude::*;

#[derive(States, Default, Debug, Clone, PartialEq, Eq, Hash)]
pub enum GameState {
    MainMenu,
    #[default]
    Playing,
    Paused,
    Death,
    Loading,
}