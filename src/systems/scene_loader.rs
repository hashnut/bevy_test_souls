use bevy::prelude::*;
use crate::{MyAssets, CheckpointMarker};

pub fn update_gltf_scene(
    mut commands: Commands,
    assets: Res<MyAssets>,
    gltf_assets: Res<Assets<Gltf>>,
    mut spawn_once: Local<bool>,
) {
    // 한 번만 실행
    if *spawn_once {
        return;
    }

    // 기본 씬 파일이 있는지 확인 (예: level.glb, scene.glb, world.glb)
    let scene_names = ["level.glb", "scene.glb", "world.glb", "environment.glb"];
    
    for scene_name in &scene_names {
        if let Some(scene_handle) = assets.gltf_files.get(*scene_name) {
            if let Some(gltf) = gltf_assets.get(scene_handle) {
                info!("Loading scene from: {}", scene_name);
                
                // 씬 가져오기
                let scene = if !gltf.named_scenes.is_empty() {
                    gltf.named_scenes.get("Scene")
                        .or_else(|| gltf.named_scenes.values().next())
                        .unwrap()
                        .clone()
                } else if !gltf.scenes.is_empty() {
                    gltf.scenes[0].clone()
                } else {
                    warn!("No scenes found in {}", scene_name);
                    continue;
                };

                // 씬 스폰
                commands.spawn((
                    SceneRoot(scene),
                    Transform::default(),
                    Name::new(format!("SceneRoot_{}", scene_name)),
                ));

                *spawn_once = true;
                info!("Scene loaded successfully from {}", scene_name);
                return;
            }
        }
    }
}

// 씬 내의 특정 오브젝트들을 찾아서 게임 컴포넌트 추가
pub fn process_scene_objects(
    mut commands: Commands,
    scene_query: Query<Entity, Added<SceneRoot>>,
    children_query: Query<&Children>,
    name_query: Query<&Name>,
    transform_query: Query<&Transform>,
    mut processed: Local<bool>,
) {
    if *processed {
        return;
    }

    for scene_entity in scene_query.iter() {
        info!("Processing scene objects...");
        process_entity_recursive(
            scene_entity,
            &mut commands,
            &children_query,
            &name_query,
            &transform_query,
            0,
        );
        *processed = true;
    }
}

fn process_entity_recursive(
    entity: Entity,
    commands: &mut Commands,
    children_query: &Query<&Children>,
    name_query: &Query<&Name>,
    transform_query: &Query<&Transform>,
    depth: usize,
) {
    // 엔티티 이름 확인
    if let Ok(name) = name_query.get(entity) {
        let name_str = name.as_str();
        
        // 이름에 따라 게임 컴포넌트 추가
        match name_str {
            n if n.contains("Player") || n.contains("player") => {
                info!("Found player spawn point: {}", n);
                if let Ok(transform) = transform_query.get(entity) {
                    // 플레이어 스폰 위치로 사용
                    commands.entity(entity).insert(PlayerSpawnPoint {
                        position: transform.translation,
                        rotation: transform.rotation,
                    });
                }
            }
            n if n.contains("Enemy") || n.contains("enemy") => {
                info!("Found enemy spawn point: {}", n);
                if let Ok(transform) = transform_query.get(entity) {
                    commands.entity(entity).insert(EnemySpawnPoint {
                        position: transform.translation,
                        rotation: transform.rotation,
                    });
                }
            }
            n if n.contains("Checkpoint") || n.contains("checkpoint") => {
                info!("Found checkpoint: {}", n);
                commands.entity(entity).insert(CheckpointMarker);
            }
            n if n.contains("Light") || n.contains("light") => {
                info!("Found light: {}", n);
                // 라이트는 이미 처리되어 있을 것
            }
            _ => {}
        }
    }

    // 자식들도 재귀적으로 처리
    if let Ok(children) = children_query.get(entity) {
        for &child in children.iter() {
            process_entity_recursive(
                child,
                commands,
                children_query,
                name_query,
                transform_query,
                depth + 1,
            );
        }
    }
}

// 마커 컴포넌트들
#[derive(Component)]
pub struct PlayerSpawnPoint {
    pub position: Vec3,
    pub rotation: Quat,
}

#[derive(Component)]
pub struct EnemySpawnPoint {
    pub position: Vec3,
    pub rotation: Quat,
}

