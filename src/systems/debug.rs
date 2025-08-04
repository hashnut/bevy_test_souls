use bevy::prelude::*;
use bevy::gltf::*;
use crate::components::player::Player;
use crate::MyAssets;

pub fn debug_scene_hierarchy(
    player_query: Query<Entity, With<Player>>,
    children_query: Query<&Children>,
    name_query: Query<&Name>,
    transform_query: Query<&Transform>,
    mesh_query: Query<&Mesh3d>,
    animation_player_query: Query<&AnimationPlayer>,
) {
    for player_entity in player_query.iter() {
        info!("=== Player Entity Hierarchy ===");
        print_entity_tree(player_entity, &children_query, &name_query, &transform_query, &mesh_query, &animation_player_query, 0);
        info!("==============================");
    }
}

fn print_entity_tree(
    entity: Entity,
    children_query: &Query<&Children>,
    name_query: &Query<&Name>,
    transform_query: &Query<&Transform>,
    mesh_query: &Query<&Mesh3d>,
    animation_player_query: &Query<&AnimationPlayer>,
    depth: usize,
) {
    let indent = "  ".repeat(depth);
    let name = name_query.get(entity)
        .map(|n| n.as_str())
        .unwrap_or("unnamed");
    
    let transform_info = transform_query.get(entity)
        .map(|t| format!("pos: {:?}, scale: {:?}", t.translation, t.scale))
        .unwrap_or_else(|_| "no transform".to_string());
    
    let has_mesh = if mesh_query.get(entity).is_ok() {
        " [HAS MESH]"
    } else {
        ""
    };
    
    let has_animation_player = if animation_player_query.get(entity).is_ok() {
        " [HAS ANIMATION PLAYER]"
    } else {
        ""
    };
    
    info!("{}Entity {:?} [{}] - {}{}{}", indent, entity, name, transform_info, has_mesh, has_animation_player);
    
    if let Ok(children) = children_query.get(entity) {
        for &child in children.iter() {
            print_entity_tree(child, children_query, name_query, transform_query, mesh_query, animation_player_query, depth + 1);
        }
    }
}

pub fn debug_model_visibility(
    query: Query<(Entity, &Name, &Visibility, &ViewVisibility), Changed<ViewVisibility>>,
) {
    for (entity, name, visibility, view_visibility) in query.iter() {
        info!(
            "Entity {:?} [{}] visibility: {:?}, view_visibility: {:?}",
            entity, name.as_str(), visibility, view_visibility.get()
        );
    }
}

// Aurora.glb의 애니메이션 목록 확인
pub fn debug_gltf_animations(
    assets: Res<MyAssets>,
    gltf_assets: Res<Assets<Gltf>>,
    mut checked: Local<bool>,
) {
    if *checked {
        return;
    }
    
    if let Some(aurora_handle) = assets.gltf_files.get("Aurora.glb") {
        if let Some(gltf) = gltf_assets.get(aurora_handle) {
            info!("=== Aurora.glb Animations ===");
            
            // 인덱스 기반 애니메이션
            for (index, animation) in gltf.animations.iter().enumerate() {
                info!("Animation{}: {:?}", index, animation);
            }
            
            // 이름 기반 애니메이션
            for (name, animation) in gltf.named_animations.iter() {
                info!("Named Animation [{}]: {:?}", name, animation);
            }
            
            info!("Total animations: {}", gltf.animations.len());
            info!("============================");
            
            *checked = true;
        }
    }
}