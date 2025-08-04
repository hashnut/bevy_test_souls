# Dark Rust - 세션 체크포인트 v2
**작성일**: 2025-08-01 오후
**프로젝트**: Dark Rust - 3D 소울라이크 액션 게임 (Rust + Bevy)

## 🎯 이번 세션 주요 작업

### 1. 캐릭터 Y 오프셋 조정
- 플레이어와 적의 Y 위치를 1.0에서 0.5로 낮춤
- 캐릭터가 지면에 더 가깝게 표시되도록 수정

### 2. 프로시저럴 맵 생성 시스템 구현
- `map_generator.rs`: 동적 맵 생성 로직
- `map_data.rs`: 맵 설정 및 요소 데이터 구조체
- `scene_serializer.rs`: JSON 저장/로드 시스템
- `generate_map_in_blender.py`: Blender GLB 익스포트 스크립트
- 6x6 타일 바닥, 외벽, 기둥, 장애물, 체크포인트 자동 생성

### 3. 카메라 시스템 개선
- 초기: 30도 위에서 캐릭터를 바라보는 각도
- 중간: 80도 탑다운 뷰로 변경
- 최종: 30도 각도로 재조정 (사용자 요청)
- 카메라 거리 및 피치 제한 조정

### 4. 캐릭터 모델 변경
- 기존: scifi_full.glb
- 변경: Vanguard.glb
- 스케일 조정: 0.01 (Vanguard 모델이 크기 때문)
- 별도 무기 모델 제거 (Vanguard에 포함되어 있을 가능성)

## 📁 추가/수정된 주요 파일

### 새로 생성된 파일:
```
src/systems/
├── map_generator.rs      # 프로시저럴 맵 생성
├── map_data.rs          # 맵 설정 데이터
├── scene_loader.rs      # GLTF 씬 로딩 시스템
├── scene_serializer.rs  # 씬 직렬화
src/components/
└── map.rs               # 맵 관련 컴포넌트

assets/
├── maps/
│   └── generate_map_in_blender.py  # Blender 스크립트
```

### 수정된 파일:
- `src/main.rs`: 플레이어 Y 오프셋 조정, 맵 생성 시스템 추가
- `src/systems/camera.rs`: 카메라 각도 조정 (30도 → 80도 → 30도)
- `src/components/player.rs`: 카메라 거리 조정
- `src/systems/asset_loader.rs`: Vanguard.glb 모델 로드
- `Cargo.toml`: serde, serde_json 의존성 추가

## 🔧 기술적 변경사항

### 1. 의존성 추가
```toml
serde = { version = "1.0", features = ["derive"] }
serde_json = "1.0"
```

### 2. 맵 생성 설정
```rust
MapConfig {
    map_size: 30.0,
    tile_size: 5.0,
    wall_height: 4.0,
    wall_thickness: 0.5,
    grid_size: 6,
}
```

### 3. 카메라 설정
```rust
// 현재 설정
base_pitch = 0.52  // 30도
distance = 8.0
pitch_clamp = (-0.3, 0.3)
```

## 🎮 현재 게임 상태

### 구현된 기능:
- ✅ 프로시저럴 던전 맵 생성
- ✅ Vanguard 캐릭터 모델
- ✅ 30도 카메라 앵글
- ✅ 체크포인트 시스템 (시각적 마커)
- ✅ 동적 조명 시스템
- ✅ WASD 이동 + 마우스 시점
- ✅ 기본 전투 시스템
- ✅ 적 AI 추적

### 미구현 기능:
- ❌ 캐릭터 모델 가시성 문제 (여전히 해결 필요)
- ❌ 애니메이션 시스템
- ❌ 회피 메커니즘
- ❌ 사망/부활 시스템
- ❌ 실제 체크포인트 상호작용

## 💡 다음 세션 권장 작업

1. **캐릭터 가시성 문제 해결**
   - Vanguard 모델이 제대로 표시되는지 확인
   - 스케일 및 위치 조정

2. **애니메이션 시스템**
   - Vanguard 모델의 애니메이션 통합
   - 이동, 공격, 대기 애니메이션

3. **게임플레이 개선**
   - 회피 시스템 구현
   - 체크포인트 상호작용
   - 사망/부활 메커니즘

## 🐛 알려진 이슈

1. **모델 가시성**: GLB 모델들이 로드되지만 화면에 표시되지 않는 문제 지속
2. **경고 메시지**: 많은 unused 경고 (최적화 필요)

## 📝 사용자 요청 기록

1. "cargo run 을 하니까 에러가 발생하는 것 같은데"
2. "추가로, 게임의 기본 씬을 update_gltf_scene 로 열도록 만들 수 있을까"
3. "procedual 하게 생성한 씬을 왜 .glb 형식이나 .gltf 형식으로 저장할 수 없는거야?"
4. "procedural 하게 생성하는 로직을 map_generator.rs 에 분리해서..."
5. "캐릭터를 위에서 아래로 바라보도록 각도를 수정해줘. 그리고 캐릭터 모델은 Vanguard.glb 로 변경해줘"
6. "위에서 아래로 내려다보는 각도를 30도 정도로 조절해줘"
7. "현재까지 작업한 내용을 checkpoint 로 저장해서..."

---
**마지막 빌드 상태**: 성공 (경고 있음)
**다음 목표**: 캐릭터 가시성 문제 해결 및 애니메이션 시스템 구현