# 기술적 세부사항 및 코드 스니펫

## 프로시저럴 맵 생성 시스템

### MapConfig 구조체
```rust
pub struct MapConfig {
    pub map_size: f32,      // 30.0
    pub tile_size: f32,     // 5.0
    pub wall_height: f32,   // 4.0
    pub wall_thickness: f32,// 0.5
    pub grid_size: usize,   // 6
}
```

### MapElements 구조체
```rust
pub struct MapElements {
    pub pillar_positions: Vec<(f32, f32)>,
    pub obstacle_positions: Vec<(f32, f32)>,
    pub checkpoint_positions: Vec<(f32, f32, f32)>,
    pub enemy_spawn_positions: Vec<(f32, f32, f32)>,
}
```

### 맵 생성 흐름
1. `generate_procedural_map` - 메인 생성 함수
2. `generate_floor` - 6x6 바닥 타일 생성
3. `generate_walls` - 4방향 외벽 생성
4. `generate_pillars` - 장식용 기둥 생성
5. `generate_obstacles` - 장애물 생성
6. `generate_checkpoints` - 체크포인트 생성
7. `generate_lights` - 조명 생성

## 카메라 시스템 설정

### 카메라 컨트롤러
```rust
pub struct CameraController {
    pub distance: f32,      // 8.0
    pub sensitivity: f32,   // 2.0
    pub pitch: f32,        // 0.0 (기본값)
    pub yaw: f32,          // 0.0 (기본값)
    pub target: Option<Entity>,
}
```

### 카메라 각도 계산
```rust
// 30도 탑다운 뷰
let base_pitch = 0.52; // 30도 in radians
let actual_pitch = controller.pitch + base_pitch;

// 피치 제한
controller.pitch = controller.pitch.clamp(-0.3, 0.3);
```

## 애셋 로딩 시스템

### Vanguard 모델 로딩
```rust
// asset_loader.rs
assets.gltf_files.insert(
    "Vanguard.glb".to_string(),
    asset_server.load("models/Vanguard.glb"),
);

// 캐릭터 스폰 시 스케일 조정
Transform::from_scale(Vec3::splat(0.01))
```

### 애셋 상태 관리
```rust
#[derive(States, Clone, Eq, PartialEq, Default, Hash, Debug)]
pub enum AssetLoaderState {
    #[default]
    Loading,
    Done,
}
```

## 컴포넌트 구조

### 맵 관련 컴포넌트
```rust
#[derive(Component)]
pub struct CheckpointMarker;

#[derive(Component)]
pub struct MapTile;

#[derive(Component)]
pub struct Wall;

#[derive(Component)]
pub struct Floor;

#[derive(Component)]
pub struct Pillar;
```

## Blender 스크립트 핵심 부분

```python
# 맵 크기 파라미터
map_size = 30.0
tile_size = 5.0
wall_height = 4.0
wall_thickness = 0.5

# 바닥 타일 생성
for x in range(6):
    for z in range(6):
        x_pos = (x - 2.5) * tile_size
        z_pos = (z - 2.5) * tile_size
        # Blender API로 큐브 생성...

# GLB 익스포트
bpy.ops.export_scene.gltf(
    filepath="dark_rust_map.glb",
    export_format='GLB',
    use_selection=True,
    export_apply=True
)
```

## 시스템 등록 (main.rs)

```rust
.add_systems(Startup, (setup_scene, setup_ui, generate_procedural_map))
.add_systems(
    OnEnter(AssetLoaderState::Done),
    (spawn_character_with_sword, update_gltf_scene)
)
.add_systems(
    Update,
    (
        update_gltf_scene.run_if(in_state(AssetLoaderState::Done)),
        process_scene_objects.run_if(in_state(AssetLoaderState::Done)),
    )
)
```

## 플레이어 위치 조정

```rust
// Y 오프셋 변경
Transform::from_xyz(0.0, 0.5, 0.0)  // 이전: 1.0
```

---
**참고**: 이 문서는 다음 세션에서 빠른 기술적 참조를 위한 것입니다.