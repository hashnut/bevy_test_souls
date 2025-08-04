# 중요 코드 스니펫 모음

## 현재 작업 중인 캐릭터 로딩 시스템

### asset_loader.rs - 메인 로딩 함수
```rust
pub fn spawn_character_with_sword(
    mut commands: Commands,
    assets: Res<MyAssets>,
    gltf_assets: Res<Assets<Gltf>>,
    player_query: Query<(Entity, Option<&Mesh3d>), With<crate::components::player::Player>>,
    children_query: Query<&Children>,
) {
    for (player_entity, mesh) in player_query.iter() {
        // 이미 모델이 로드되었으면 스킵
        if children_query.get(player_entity).is_ok() {
            continue;
        }
        
        // scifi_full 모델 사용 (전체 캐릭터)
        if let Some(character_handle) = assets.gltf_files.get("scifi_full.glb") {
            if let Some(gltf) = gltf_assets.get(character_handle) {
                let scene = if !gltf.named_scenes.is_empty() {
                    gltf.named_scenes.get("Scene")
                        .or_else(|| gltf.named_scenes.values().next())
                        .unwrap()
                        .clone()
                } else if !gltf.scenes.is_empty() {
                    gltf.scenes[0].clone()
                } else {
                    continue;
                };

                // 캐릭터 모델 추가
                commands.entity(player_entity).with_children(|parent| {
                    parent.spawn((
                        SceneRoot(scene.clone()),
                        Transform::default(), // 기본 변환 사용
                        Name::new("PlayerModel"),
                    ));
                });

                // 칼 모델 추가
                if let Some(sword_handle) = assets.gltf_files.get("sword.glb") {
                    if let Some(sword_gltf) = gltf_assets.get(sword_handle) {
                        // 칼 씬 가져오기
                        let scene = if !sword_gltf.named_scenes.is_empty() {
                            sword_gltf.named_scenes.get("Scene")
                                .or_else(|| sword_gltf.named_scenes.values().next())
                                .unwrap()
                                .clone()
                        } else if !sword_gltf.scenes.is_empty() {
                            sword_gltf.scenes[0].clone()
                        } else {
                            continue;
                        };

                        // 칼을 플레이어에 부착
                        commands.entity(player_entity).with_children(|parent| {
                            parent.spawn((
                                SceneRoot(scene),
                                Transform::from_xyz(0.3, 0.5, 0.1) // 손 위치 근처
                                    .with_scale(Vec3::splat(0.1)) // 참고 프로젝트와 동일한 크기
                                    .with_rotation(Quat::from_euler(EulerRot::XYZ,
                                        -std::f32::consts::FRAC_PI_2,
                                        0.0,
                                        std::f32::consts::FRAC_PI_4)),
                                Name::new("PlayerSword"),
                            ));
                        });
                    }
                }
            }
        }
    }
}
```

### main.rs - 시스템 등록
```rust
fn main() {
    App::new()
        .add_plugins(DefaultPlugins.set(WindowPlugin {
            primary_window: Some(Window {
                title: "Dark Rust - Soulslike Game".into(),
                resolution: (1280., 720.).into(),
                ..default()
            }),
            ..default()
        }))
        .add_plugins(RapierPhysicsPlugin::<NoUserData>::default())
        .add_plugins(RapierDebugRenderPlugin::default())
        .init_state::<GameState>()
        .init_state::<AssetLoaderState>()
        .insert_resource(ClearColor(Color::srgb(0.1, 0.1, 0.1)))
        .insert_resource(AmbientLight {
            color: Color::WHITE,
            brightness: 300.0,
        })
        .init_resource::<InputMapping>()
        .init_resource::<MouseLook>()
        .init_resource::<MyAssets>()
        .add_systems(Startup, (setup_scene, setup_ui))
        .add_systems(
            Update,
            (
                player_movement,
                camera_follow,
                handle_input,
                combat_system,
                hitbox_system,
                weapon_system,
                enemy_ai_system,
                enemy_attack_system,
                attack_effect_system,
                update_health_ui,
                update_stamina_ui,
                update_souls_ui,
                // 애셋 로딩
                load_assets.run_if(in_state(AssetLoaderState::Loading)),
                // 디버깅
                debug_scene_hierarchy,
                debug_model_visibility,
            ).run_if(in_state(GameState::Playing))
        )
        .add_systems(
            OnEnter(AssetLoaderState::Done),
            spawn_character_with_sword
        )
        .run();
}
```

### debug.rs - 디버그 시스템
```rust
pub fn debug_scene_hierarchy(
    player_query: Query<Entity, With<Player>>,
    children_query: Query<&Children>,
    name_query: Query<&Name>,
    transform_query: Query<&Transform>,
    mesh_query: Query<&Mesh3d>,
) {
    for player_entity in player_query.iter() {
        info!("=== Player Entity Hierarchy ===");
        print_entity_tree(player_entity, &children_query, &name_query, &transform_query, &mesh_query, 0);
        info!("==============================");
    }
}
```

### 프로시저럴 캐릭터 (작동 확인됨)
```rust
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
            // 몸통, 머리, 팔, 다리 등 추가
        });
    }
}
```

## 참고 프로젝트 코드 (bevy-modular-characters-proof-of-concept)

### spawn_scenes.rs
```rust
for (name, gltf_handle) in &asset_pack.gltf_files {
    if let Some(gltf) = assets_gltf.get(gltf_handle) {
        let mut transform = Transform::from_xyz(0.0, 0.0, 0.0);
        
        if name == "sword.glb" {
            transform.scale = Vec3::splat(0.1)
        }
        
        let entity_commands = commands.spawn((
            SceneBundle {
                scene: gltf.named_scenes["Scene"].clone(),
                transform,
                ..Default::default()
            },
            SceneName(name.clone()),
        ));
    }
}
```

---
**마지막 업데이트**: 2025-08-01