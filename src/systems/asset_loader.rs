use bevy::prelude::*;
use bevy::gltf::Gltf;
use bevy::utils::HashMap;

#[derive(States, Clone, Eq, PartialEq, Default, Hash, Debug)]
pub enum AssetLoaderState {
    #[default]
    Loading,
    Done,
}

#[derive(Resource)]
pub struct MyAssets {
    pub gltf_files: HashMap<String, Handle<Gltf>>,
    pub loaded: bool,
}

impl Default for MyAssets {
    fn default() -> Self {
        Self {
            gltf_files: HashMap::new(),
            loaded: false,
        }
    }
}

pub fn load_assets(
    mut assets: ResMut<MyAssets>,
    asset_server: Res<AssetServer>,
    mut next_state: ResMut<NextState<AssetLoaderState>>,
) {
    if !assets.loaded {
        // GLB 파일들 로드
        assets.gltf_files.insert(
            "Aurora.glb".to_string(),
            asset_server.load("models/Aurora.glb"),
        );
        assets.gltf_files.insert(
            "main_skeleton.glb".to_string(),
            asset_server.load("models/main_skeleton.glb"),
        );
        assets.gltf_files.insert(
            "scifi_full.glb".to_string(),
            asset_server.load("models/scifi_full.glb"),
        );
        assets.gltf_files.insert(
            "scifi_torso.glb".to_string(),
            asset_server.load("models/scifi_torso.glb"),
        );
        assets.gltf_files.insert(
            "sword.glb".to_string(),
            asset_server.load("models/sword.glb"),
        );
        assets.gltf_files.insert(
            "witch_legs.glb".to_string(),
            asset_server.load("models/witch_legs.glb"),
        );
        
        assets.loaded = true;
    }
    
    // 모든 애셋이 로드되었는지 확인
    let mut all_loaded = true;
    for handle in assets.gltf_files.values() {
        if !asset_server.is_loaded_with_dependencies(handle) {
            all_loaded = false;
            break;
        }
    }
    
    if all_loaded && assets.loaded {
        info!("All assets loaded, transitioning to Done state");
        next_state.set(AssetLoaderState::Done);
    }
}

pub fn spawn_character_with_sword(
    mut commands: Commands,
    assets: Res<MyAssets>,
    gltf_assets: Res<Assets<Gltf>>,
    player_query: Query<Entity, With<crate::components::player::Player>>,
    children_query: Query<&Children>,
) {
    info!("spawn_character_with_sword called, found {} players", player_query.iter().len());
    
    for player_entity in player_query.iter() {
        // 이미 모델이 로드되었으면 스킵
        if children_query.get(player_entity).is_ok() {
            info!("Player already has children, skipping");
            continue;
        }
        
        info!("Attempting to load Aurora model for player");
        
        // Aurora 모델 사용 (전체 캐릭터)
        if let Some(character_handle) = assets.gltf_files.get("Aurora.glb") {
            info!("Found Aurora.glb handle");
            if let Some(gltf) = gltf_assets.get(character_handle) {
                info!("Aurora GLTF loaded successfully");
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

                // 기존 캡슐 메시는 제거하지 않음 - 이제 처음부터 추가하지 않음

                // 캐릭터 모델 추가
                commands.entity(player_entity).with_children(|parent| {
                    parent.spawn((
                        SceneRoot(scene.clone()),
                        Transform::from_translation(Vec3::new(0.0, -1.25, 0.0))  // Y축으로 -1.25 이동하여 발이 지면에 닿도록
                            .with_scale(Vec3::splat(1.0)),
                        Name::new("PlayerModel"),
                    ));
                });

                info!("Aurora character model spawned for player entity {:?}", player_entity);

                // Aurora 모델은 이미 무기를 포함하고 있을 수 있으므로 별도 무기는 추가하지 않음
                
                break; // 첫 번째 플레이어만 처리
            } else {
                warn!("Aurora.glb GLTF asset not loaded yet");
            }
        } else {
            warn!("Aurora.glb handle not found in assets");
        }
    }
}