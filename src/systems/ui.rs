use bevy::prelude::*;
use crate::components::*;
use crate::states::*;

pub fn setup_ui(mut commands: Commands) {
    // 메인 UI 컨테이너
    commands.spawn((
        Node {
            width: Val::Percent(100.0),
            height: Val::Percent(100.0),
            flex_direction: FlexDirection::Column,
            justify_content: JustifyContent::SpaceBetween,
            align_items: AlignItems::Stretch,
            ..default()
        },
        GlobalZIndex(1),
        GameUI,
    )).with_children(|parent| {
        // 상단 UI (체력, 스테미나)
        parent.spawn((
            Node {
                width: Val::Percent(100.0),
                height: Val::Px(120.0),
                flex_direction: FlexDirection::Column,
                justify_content: JustifyContent::Start,
                align_items: AlignItems::Start,
                padding: UiRect::all(Val::Px(20.0)),
                ..default()
            },
            BackgroundColor(Color::NONE),
        )).with_children(|parent| {
            // 체력바 컨테이너
            parent.spawn((
                Node {
                    width: Val::Px(300.0),
                    height: Val::Px(30.0),
                    margin: UiRect::bottom(Val::Px(10.0)),
                    border: UiRect::all(Val::Px(2.0)),
                    ..default()
                },
                BorderColor(Color::WHITE),
                BackgroundColor(Color::srgb(0.2, 0.2, 0.2)),
            )).with_children(|parent| {
                // 체력바
                parent.spawn((
                    Node {
                        width: Val::Percent(100.0),
                        height: Val::Percent(100.0),
                        ..default()
                    },
                    BackgroundColor(Color::srgb(0.8, 0.2, 0.2)),
                    HealthBar,
                ));
            });

            // 스테미나바 컨테이너
            parent.spawn((
                Node {
                    width: Val::Px(300.0),
                    height: Val::Px(20.0),
                    border: UiRect::all(Val::Px(2.0)),
                    ..default()
                },
                BorderColor(Color::WHITE),
                BackgroundColor(Color::srgb(0.2, 0.2, 0.2)),
            )).with_children(|parent| {
                // 스테미나바
                parent.spawn((
                    Node {
                        width: Val::Percent(100.0),
                        height: Val::Percent(100.0),
                        ..default()
                    },
                    BackgroundColor(Color::srgb(0.2, 0.8, 0.2)),
                    StaminaBar,
                ));
            });
        });

        // 우상단 UI (소울 카운터)
        parent.spawn((
            Node {
                width: Val::Percent(100.0),
                height: Val::Px(60.0),
                flex_direction: FlexDirection::Row,
                justify_content: JustifyContent::End,
                align_items: AlignItems::Start,
                padding: UiRect::all(Val::Px(20.0)),
                ..default()
            },
            BackgroundColor(Color::NONE),
        )).with_children(|parent| {
            // 소울 카운터
            parent.spawn((
                Text::new("Souls: 0"),
                TextFont {
                    font_size: 24.0,
                    ..default()
                },
                TextColor(Color::srgb(1.0, 1.0, 0.0)),
                SoulCounter,
            ));
        });
    });
}

pub fn update_health_ui(
    player_query: Query<&Health, (With<Player>, Changed<Health>)>,
    mut health_bar_query: Query<&mut Node, With<HealthBar>>,
) {
    for health in player_query.iter() {
        for mut style in health_bar_query.iter_mut() {
            let health_percentage = (health.current / health.max).max(0.0);
            style.width = Val::Percent(health_percentage * 100.0);
        }
    }
}

pub fn update_stamina_ui(
    player_query: Query<&Stamina, (With<Player>, Changed<Stamina>)>,
    mut stamina_bar_query: Query<&mut Node, With<StaminaBar>>,
) {
    for stamina in player_query.iter() {
        for mut style in stamina_bar_query.iter_mut() {
            let stamina_percentage = (stamina.current / stamina.max).max(0.0);
            style.width = Val::Percent(stamina_percentage * 100.0);
        }
    }
}

pub fn update_souls_ui(
    player_query: Query<&Souls, (With<Player>, Changed<Souls>)>,
    mut soul_counter_query: Query<&mut Text, With<SoulCounter>>,
) {
    for souls in player_query.iter() {
        for mut text in soul_counter_query.iter_mut() {
            text.0 = format!("Souls: {}", souls.count);
        }
    }
}

pub fn setup_pause_menu(
    mut commands: Commands, 
    current_state: Res<State<GameState>>,
    mut window_q: Query<&mut Window, With<bevy::window::PrimaryWindow>>,
    mut cam_q: Query<&mut crate::systems::third_person_camera::ThirdPersonCamera>,
) {
    // 일시정지 시 커서 보이기
    if let Ok(mut window) = window_q.get_single_mut() {
        window.cursor_options.grab_mode = bevy::window::CursorGrabMode::None;
        window.cursor_options.visible = true;
    }
    
    // 카메라 커서 락 해제
    if let Ok(mut cam) = cam_q.get_single_mut() {
        cam.cursor_lock_active = false;
    }
    
    if *current_state.get() == GameState::Paused {
        commands.spawn((
            Node {
                width: Val::Percent(100.0),
                height: Val::Percent(100.0),
                justify_content: JustifyContent::Center,
                align_items: AlignItems::Center,
                flex_direction: FlexDirection::Column,
                ..default()
            },
            BackgroundColor(Color::srgba(0.0, 0.0, 0.0, 0.8)),
            GlobalZIndex(10),
            PauseMenu,
        )).with_children(|parent| {
                // 일시정지 텍스트
            parent.spawn((
                Text::new("PAUSED"),
                TextFont {
                    font_size: 48.0,
                    ..default()
                },
                TextColor(Color::WHITE),
            ));
            
            parent.spawn((
                Text::new("Press ESC to resume"),
                TextFont {
                    font_size: 24.0,
                    ..default()
                },
                TextColor(Color::srgb(0.8, 0.8, 0.8)),
            ));
        });
    }
}

pub fn cleanup_pause_menu(
    mut commands: Commands,
    pause_menu_query: Query<Entity, With<PauseMenu>>,
    mut window_q: Query<&mut Window, With<bevy::window::PrimaryWindow>>,
    mut cam_q: Query<&mut crate::systems::third_person_camera::ThirdPersonCamera>,
) {
    // 일시정지 해제 시 커서 다시 잠그기
    if let Ok(mut window) = window_q.get_single_mut() {
        window.cursor_options.grab_mode = bevy::window::CursorGrabMode::Locked;
        window.cursor_options.visible = false;
    }
    
    // 카메라 커서 락 활성화
    if let Ok(mut cam) = cam_q.get_single_mut() {
        cam.cursor_lock_active = true;
    }
    
    for entity in pause_menu_query.iter() {
        commands.entity(entity).despawn_recursive();
    }
}

pub fn setup_death_screen(mut commands: Commands) {
    commands.spawn((
        Node {
            width: Val::Percent(100.0),
            height: Val::Percent(100.0),
            justify_content: JustifyContent::Center,
            align_items: AlignItems::Center,
            flex_direction: FlexDirection::Column,
            ..default()
        },
        BackgroundColor(Color::srgba(0.5, 0.0, 0.0, 0.9)),
        GlobalZIndex(15),
        DeathScreen,
    )).with_children(|parent| {
        // YOU DIED 텍스트
        parent.spawn((
            Text::new("YOU DIED"),
            TextFont {
                font_size: 64.0,
                ..default()
            },
            TextColor(Color::srgb(1.0, 0.2, 0.2)),
        ));
        
        parent.spawn((
            Text::new("Press SPACE to respawn"),
            TextFont {
                font_size: 24.0,
                ..default()
            },
            TextColor(Color::WHITE),
        ));
    });
}

pub fn cleanup_death_screen(
    mut commands: Commands,
    death_screen_query: Query<Entity, With<DeathScreen>>,
) {
    for entity in death_screen_query.iter() {
        commands.entity(entity).despawn_recursive();
    }
}

pub fn handle_death_ui(
    keyboard_input: Res<ButtonInput<KeyCode>>,
    mut next_state: ResMut<NextState<GameState>>,
    current_state: Res<State<GameState>>,
    player_query: Query<&Health, With<Player>>,
) {
    // 플레이어가 죽었는지 확인
    if let Ok(health) = player_query.get_single() {
        if health.current <= 0.0 && *current_state.get() != GameState::Death {
            next_state.set(GameState::Death);
        }
    }

    // 사망 상태에서 스페이스바로 부활
    if *current_state.get() == GameState::Death && keyboard_input.just_pressed(KeyCode::Space) {
        next_state.set(GameState::Playing);
    }
}