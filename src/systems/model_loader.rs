use bevy::prelude::*;
use crate::components::player::Player;

#[derive(Resource)]
pub struct PlayerModelHandle {
    pub handle: Option<Handle<Gltf>>,
    pub loaded: bool,
}

impl Default for PlayerModelHandle {
    fn default() -> Self {
        Self {
            handle: None,
            loaded: false,
        }
    }
}

pub fn load_player_model(
    mut model_handle: ResMut<PlayerModelHandle>,
    asset_server: Res<AssetServer>,
) {
    if model_handle.handle.is_none() {
        // 메인 스켈레톤 모델 로드
        let handle = asset_server.load("models/main_skeleton.glb");
        model_handle.handle = Some(handle);
    }
}

pub fn setup_player_model(
    mut commands: Commands,
    mut model_handle: ResMut<PlayerModelHandle>,
    gltf_assets: Res<Assets<Gltf>>,
    player_query: Query<Entity, With<Player>>,
    children_query: Query<&Children>,
    mesh_query: Query<Entity, With<Mesh3d>>,
) {
    if model_handle.loaded {
        return;
    }

    let Some(handle) = &model_handle.handle else {
        return;
    };

    let Some(gltf) = gltf_assets.get(handle) else {
        return;
    };

    for player_entity in player_query.iter() {
        // 既存のカプセルメッシュを削除
        if let Ok(children) = children_query.get(player_entity) {
            for child in children.iter() {
                if mesh_query.get(*child).is_ok() {
                    commands.entity(*child).despawn_recursive();
                }
            }
        }

        // GLTF 씬을 플레이어 엔티티의 자식으로 추가
        let scene = if !gltf.named_scenes.is_empty() {
            gltf.named_scenes.get("Scene")
                .or_else(|| gltf.named_scenes.values().next())
                .unwrap()
                .clone()
        } else if !gltf.scenes.is_empty() {
            gltf.scenes[0].clone()
        } else {
            error!("No scenes found in GLTF file");
            return;
        };

        commands.entity(player_entity).with_children(|parent| {
            parent.spawn((
                SceneRoot(scene),
                Transform::from_scale(Vec3::splat(0.01)) // 모델 크기 조정
                    .with_rotation(Quat::from_rotation_y(std::f32::consts::PI)),
            ));
        });

        // 임시 캡슐 메시 제거
        commands.entity(player_entity).remove::<Mesh3d>();
        commands.entity(player_entity).remove::<MeshMaterial3d<StandardMaterial>>();

        model_handle.loaded = true;
        info!("Player model loaded successfully");
    }
}

#[derive(Resource)]
pub struct SwordModelHandle {
    pub handle: Option<Handle<Gltf>>,
    pub loaded: bool,
}

impl Default for SwordModelHandle {
    fn default() -> Self {
        Self {
            handle: None,
            loaded: false,
        }
    }
}

pub fn load_sword_model(
    mut sword_handle: ResMut<SwordModelHandle>,
    asset_server: Res<AssetServer>,
) {
    if sword_handle.handle.is_none() {
        let handle = asset_server.load("models/sword.glb");
        sword_handle.handle = Some(handle);
    }
}

pub fn setup_sword_model(
    mut commands: Commands,
    mut sword_handle: ResMut<SwordModelHandle>,
    gltf_assets: Res<Assets<Gltf>>,
    player_query: Query<Entity, With<Player>>,
    model_handle: Res<PlayerModelHandle>,
) {
    if sword_handle.loaded || !model_handle.loaded {
        return;
    }

    let Some(handle) = &sword_handle.handle else {
        return;
    };

    let Some(gltf) = gltf_assets.get(handle) else {
        return;
    };

    for player_entity in player_query.iter() {
        // 칼 씬 가져오기
        let scene = if !gltf.named_scenes.is_empty() {
            gltf.named_scenes.get("Scene")
                .or_else(|| gltf.named_scenes.values().next())
                .unwrap()
                .clone()
        } else if !gltf.scenes.is_empty() {
            gltf.scenes[0].clone()
        } else {
            error!("No scenes found in sword GLTF file");
            return;
        };

        // 칼을 플레이어의 오른쪽에 부착 (손 위치 근사)
        commands.entity(player_entity).with_children(|parent| {
            parent.spawn((
                SceneRoot(scene),
                Transform::from_xyz(0.3, 0.5, 0.1) // 오른손 근처 위치
                    .with_scale(Vec3::splat(0.05)) // 칼 크기 조정  
                    .with_rotation(Quat::from_euler(EulerRot::XYZ, 
                        -std::f32::consts::FRAC_PI_2, // X축 회전
                        0.0, 
                        std::f32::consts::FRAC_PI_4)), // Z축 회전
            ));
        });
        
        sword_handle.loaded = true;
        info!("Sword model attached to player");
        return;
    }
}

pub fn handle_missing_model(
    model_handle: Res<PlayerModelHandle>,
    asset_server: Res<AssetServer>,
    player_query: Query<Entity, (With<Player>, Without<Mesh3d>)>,
    mut commands: Commands,
    mut meshes: ResMut<Assets<Mesh>>,
    mut materials: ResMut<Assets<StandardMaterial>>,
) {
    // モデルが見つからない場合、デフォルトのカプセルを維持
    if let Some(handle) = &model_handle.handle {
        if asset_server.is_loaded_with_dependencies(handle) {
            return;
        }
        
        let load_state = asset_server.get_load_state(handle);
        if matches!(load_state, Some(bevy::asset::LoadState::Failed(_))) {
            warn!("Failed to load player model, using default capsule");
            
            // エラーメッセージを表示し、デフォルトカプセルを維持
            for player_entity in player_query.iter() {
                if !model_handle.loaded {
                    commands.entity(player_entity).insert((
                        Mesh3d(meshes.add(Capsule3d::default().mesh())),
                        MeshMaterial3d(materials.add(StandardMaterial {
                            base_color: Color::srgb(0.8, 0.3, 0.3),
                            ..default()
                        })),
                    ));
                }
            }
        }
    }
}