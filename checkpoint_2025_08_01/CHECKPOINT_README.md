# Dark Rust - 프로젝트 체크포인트 (2025-08-01)

## 🎮 프로젝트 개요
**Dark Rust**: Rust + Bevy 엔진으로 개발하는 3D 소울라이크 액션 게임

- **개발 시작일**: 2025-07-26
- **체크포인트 날짜**: 2025-08-01
- **개발 환경**: Windows 11, Rust 2021, Bevy 0.15
- **주요 목표**: 다크소울 스타일의 3D 액션 RPG 개발

## 📊 현재 진행 상황

### Phase 1 MVP ✅ 완료
1. **기본 프로젝트 구조**
   - Bevy 0.15 + Rapier3D 물리엔진
   - ECS 아키텍처 기반 모듈화 구조
   - 개발/릴리즈 빌드 최적화

2. **플레이어 시스템**
   - WASD 이동 + 마우스 시점
   - 체력 100, 스테미나 100
   - 캡슐 콜라이더 물리

3. **전투 시스템**
   - 좌클릭 기본 공격
   - 무기 데미지/쿨다운
   - 히트박스 시스템

4. **적 AI 시스템**
   - 플레이어 추적 AI
   - 적 공격 시스템
   - 체력 50

5. **카메라 시스템**
   - 3인칭 추적 카메라
   - 플레이어 뒤 5유닛

6. **UI 시스템**
   - 체력/스테미나바 (좌상단)
   - 소울 카운터 (우상단)

7. **게임 상태 관리**
   - Playing/Paused/Death 상태
   - 상태별 시스템 분리

### 현재 작업 중 🚧
**3D 캐릭터 모델 시스템 구현**
- 프로시저럴 캐릭터에서 GLB 모델로 전환 작업
- assets/models 폴더의 모델들 활용
  - main_skeleton.glb (스켈레톤만)
  - scifi_full.glb (전체 캐릭터)
  - sword.glb (칼)
- 참고 프로젝트: bevy-modular-characters-proof-of-concept

## 🔧 기술 스택 및 의존성

```toml
[dependencies]
bevy = { version = "0.15", features = ["default", "animation", "bevy_gltf"] }
bevy_rapier3d = "0.28"
rand = "0.8"
```

## 📁 프로젝트 구조

```
bevy_test_souls/
├── src/
│   ├── main.rs              # 게임 진입점
│   ├── components/          # ECS 컴포넌트
│   │   ├── combat.rs        # 무기, 공격 상태
│   │   ├── enemy.rs         # 적 AI 컴포넌트
│   │   ├── player.rs        # 플레이어 컴포넌트
│   │   ├── stats.rs         # 체력, 스테미나, 소울
│   │   └── ui.rs            # UI 컴포넌트
│   ├── systems/             # 게임 시스템
│   │   ├── animation.rs     # 애니메이션 시스템
│   │   ├── asset_loader.rs  # 애셋 로딩 시스템
│   │   ├── camera.rs        # 카메라 추적
│   │   ├── combat.rs        # 전투 로직
│   │   ├── debug.rs         # 디버그 시스템
│   │   ├── enemy_ai.rs      # 적 AI
│   │   ├── input.rs         # 입력 처리
│   │   ├── model_loader.rs  # 모델 로더 (deprecated)
│   │   ├── player_movement.rs # 플레이어 이동
│   │   ├── procedural_character.rs # 프로시저럴 캐릭터
│   │   └── ui.rs            # UI 업데이트
│   ├── resources/           # 글로벌 리소스
│   │   └── input.rs         # 입력 매핑
│   ├── states/              # 게임 상태
│   │   └── mod.rs           # GameState 정의
│   └── utils/               # 유틸리티
│       └── math.rs          # 수학 헬퍼
├── assets/
│   └── models/              # 3D 모델
│       ├── main_skeleton.glb
│       ├── scifi_full.glb
│       ├── scifi_torso.glb
│       ├── sword.glb
│       └── witch_legs.glb
└── checkpoint_2025_08_01/   # 이 체크포인트
```

## 🐛 현재 이슈 및 해결 과정

### 1. 캐릭터 모델 표시 문제
- **문제**: GLB 모델이 로드되지만 화면에 표시되지 않음
- **시도한 해결책**:
  1. 프로시저럴 캐릭터로 테스트 → 정상 표시
  2. main_skeleton.glb → scifi_full.glb로 변경
  3. 스케일 조정 (0.01 → 1.0)
  4. 디버그 시스템으로 씬 트리 확인
- **발견**: 모델은 로드되었지만 초기 view_visibility가 false
- **다음 시도**: 가시성 문제 해결 필요

### 2. 애셋 로딩 시스템
- AssetLoaderState 상태 기반 로딩 구현
- spawn_character_with_sword 함수로 캐릭터와 칼 동시 로드

## 🎯 다음 목표 (Phase 2)

### 우선순위 높음
1. **캐릭터 모델 표시 문제 해결**
2. **회피 시스템** - 스페이스바 롤링
3. **사망/부활 시스템** - 소울 드롭
4. **체크포인트 시스템** - 횃불 저장 포인트

### 우선순위 중간
5. **강공격 시스템** - 우클릭 차지 공격
6. **방어 시스템** - Shift 방패 방어
7. **소울 시스템** - 적 처치 시 획득

## 💻 개발 명령어

```bash
# 개발 실행
cargo run

# 릴리즈 빌드
cargo build --release

# 테스트
cargo test

# 포맷팅
cargo fmt

# 린트
cargo clippy
```

## 📝 중요 코드 스니펫

### 현재 애셋 로더 (asset_loader.rs)
```rust
pub fn spawn_character_with_sword(
    mut commands: Commands,
    assets: Res<MyAssets>,
    gltf_assets: Res<Assets<Gltf>>,
    player_query: Query<(Entity, Option<&Mesh3d>), With<Player>>,
    children_query: Query<&Children>,
) {
    // scifi_full.glb 모델 로드
    // sword.glb 부착
}
```

### 메인 시스템 등록 (main.rs)
```rust
.add_systems(
    Update,
    (
        player_movement,
        camera_follow,
        handle_input,
        combat_system,
        // ... 기타 시스템
        load_assets.run_if(in_state(AssetLoaderState::Loading)),
        debug_scene_hierarchy,
        debug_model_visibility,
    ).run_if(in_state(GameState::Playing))
)
```

## 🔍 디버깅 정보

- Rapier 물리 디버그: F12
- 씬 계층 구조: debug_scene_hierarchy 시스템
- 모델 가시성: debug_model_visibility 시스템

## 📌 세션 복구 가이드

1. 이 체크포인트 폴더의 모든 파일 읽기
2. 현재 이슈 확인: 캐릭터 모델 표시 문제
3. assets/models 폴더의 GLB 파일 확인
4. 참고 프로젝트 활용 가능

---
**작성일**: 2025-08-01
**작성자**: Claude (AI Assistant)
**프로젝트 상태**: 개발 진행 중