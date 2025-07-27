# Dark Rust - 구현 플랜

## 프로젝트 구조

```
soulslike-test/
├── Cargo.toml                 # 프로젝트 설정 및 의존성
├── src/
│   ├── main.rs               # 메인 진입점
│   ├── lib.rs                # 라이브러리 루트
│   ├── components/           # ECS 컴포넌트들
│   │   ├── mod.rs
│   │   ├── player.rs         # 플레이어 관련 컴포넌트
│   │   ├── enemy.rs          # 적 관련 컴포넌트
│   │   ├── combat.rs         # 전투 관련 컴포넌트
│   │   ├── stats.rs          # 스탯 컴포넌트 (HP, 스테미나 등)
│   │   └── ui.rs             # UI 관련 컴포넌트
│   ├── systems/              # ECS 시스템들
│   │   ├── mod.rs
│   │   ├── player_movement.rs # 플레이어 이동 시스템
│   │   ├── camera.rs         # 카메라 시스템
│   │   ├── combat.rs         # 전투 시스템
│   │   ├── enemy_ai.rs       # 적 AI 시스템
│   │   ├── stats.rs          # 스탯 관리 시스템
│   │   ├── ui.rs             # UI 시스템
│   │   └── level.rs          # 레벨/환경 시스템
│   ├── resources/            # 전역 리소스들
│   │   ├── mod.rs
│   │   ├── input.rs          # 입력 매핑
│   │   ├── game_state.rs     # 게임 상태
│   │   └── assets.rs         # 에셋 관리
│   ├── states/               # 게임 상태들
│   │   ├── mod.rs
│   │   ├── menu.rs           # 메뉴 상태
│   │   ├── gameplay.rs       # 게임플레이 상태
│   │   └── death.rs          # 사망 상태
│   └── utils/                # 유틸리티 함수들
│       ├── mod.rs
│       └── math.rs           # 수학 관련 유틸
├── assets/                   # 게임 에셋들
│   ├── models/               # 3D 모델
│   ├── textures/             # 텍스처
│   ├── sounds/               # 사운드
│   └── fonts/                # 폰트
└── README.md                 # 프로젝트 설명
```

## 단계별 구현 계획

### Phase 1: 기본 시스템 구현

#### 1.1 프로젝트 초기 설정
- **파일**: `Cargo.toml`
- **의존성**: 
  - `bevy` (메인 엔진)
  - `bevy_rapier3d` (물리 엔진)
  - `bevy_asset_loader` (에셋 로딩)
- **구현 내용**: 기본 프로젝트 구조 및 의존성 설정

#### 1.2 기본 플레이어 캐릭터 및 이동
- **파일**: 
  - `src/components/player.rs`
  - `src/systems/player_movement.rs`
- **컴포넌트**:
  ```rust
  #[derive(Component)]
  pub struct Player {
      pub speed: f32,
      pub is_rolling: bool,
  }
  
  #[derive(Component)]
  pub struct MovementController {
      pub velocity: Vec3,
      pub is_grounded: bool,
  }
  ```
- **시스템**: WASD 키 입력으로 이동, 마우스 회전
- **물리**: Rapier3D를 이용한 충돌 처리

#### 1.3 3인칭 카메라 시스템
- **파일**: `src/systems/camera.rs`
- **컴포넌트**:
  ```rust
  #[derive(Component)]
  pub struct CameraController {
      pub distance: f32,
      pub sensitivity: f32,
      pub pitch: f32,
      pub yaw: f32,
  }
  ```
- **기능**: 플레이어 뒤를 따라가는 3인칭 카메라, 마우스로 회전

### Phase 2: 전투 시스템

#### 2.1 기본 공격 시스템
- **파일**: 
  - `src/components/combat.rs`
  - `src/systems/combat.rs`
- **컴포넌트**:
  ```rust
  #[derive(Component)]
  pub struct Weapon {
      pub damage: f32,
      pub attack_range: f32,
      pub attack_cooldown: f32,
  }
  
  #[derive(Component)]
  pub struct AttackState {
      pub is_attacking: bool,
      pub attack_timer: f32,
  }
  ```
- **기능**: 마우스 클릭으로 공격, 공격 범위 내 적에게 데미지

#### 2.2 체력/스테미나 시스템
- **파일**: 
  - `src/components/stats.rs`
  - `src/systems/stats.rs`
- **컴포넌트**:
  ```rust
  #[derive(Component)]
  pub struct Health {
      pub current: f32,
      pub max: f32,
  }
  
  #[derive(Component)]
  pub struct Stamina {
      pub current: f32,
      pub max: f32,
      pub regen_rate: f32,
  }
  ```
- **기능**: 체력 감소, 스테미나 소모 및 회복

### Phase 3: 적 AI 시스템

#### 3.1 기본 적 AI
- **파일**: 
  - `src/components/enemy.rs`
  - `src/systems/enemy_ai.rs`
- **컴포넌트**:
  ```rust
  #[derive(Component)]
  pub struct Enemy {
      pub ai_type: EnemyType,
      pub detection_range: f32,
      pub attack_range: f32,
  }
  
  #[derive(Component)]
  pub struct AIState {
      pub current_state: AIStateType,
      pub target: Option<Entity>,
  }
  ```
- **AI 상태**: Idle, Patrol, Chase, Attack
- **기능**: 플레이어 감지, 추적, 공격

### Phase 4: UI 시스템

#### 4.1 기본 UI
- **파일**: 
  - `src/components/ui.rs`
  - `src/systems/ui.rs`
- **컴포넌트**:
  ```rust
  #[derive(Component)]
  pub struct HealthBar;
  
  #[derive(Component)]
  pub struct StaminaBar;
  
  #[derive(Component)]
  pub struct SoulCounter;
  ```
- **기능**: 체력바, 스테미나바, 소울 카운터 표시

### Phase 5: 게임 상태 관리

#### 5.1 게임 상태 시스템
- **파일**: 
  - `src/states/gameplay.rs`
  - `src/states/death.rs`
  - `src/resources/game_state.rs`
- **상태**: Menu, Playing, Paused, Death
- **기능**: 상태 전환, 게임 저장/로드

#### 5.2 사망/부활 시스템
- **컴포넌트**:
  ```rust
  #[derive(Component)]
  pub struct DeathMarker {
      pub soul_amount: i32,
      pub position: Vec3,
  }
  
  #[derive(Component)]
  pub struct Checkpoint {
      pub is_active: bool,
  }
  ```
- **기능**: 사망 시 소울 드롭, 체크포인트에서 부활

### Phase 6: 레벨 및 환경

#### 6.1 기본 레벨
- **파일**: `src/systems/level.rs`
- **기능**: 기본 던전 환경, 벽, 바닥, 함정
- **에셋**: 기본 3D 모델들 (큐브, 구 등으로 프로토타입)

#### 6.2 환경 요소
- **컴포넌트**:
  ```rust
  #[derive(Component)]
  pub struct Checkpoint {
      pub is_activated: bool,
  }
  
  #[derive(Component)]
  pub struct Trap {
      pub damage: f32,
      pub trigger_area: f32,
  }
  ```

## 기술적 구현 세부사항

### ECS 아키텍처 활용
- **Entity**: 게임 오브젝트들 (플레이어, 적, 아이템 등)
- **Component**: 데이터 구조체들 (위치, 체력, AI 상태 등)
- **System**: 로직 처리 함수들 (이동, 공격, AI 등)

### 물리 시스템
- **Rapier3D**: 충돌 처리, 중력, 레이캐스팅
- **CollisionGroup**: 플레이어, 적, 환경, 공격 등으로 분류
- **Sensor**: 공격 범위, 감지 범위 등에 활용

### 입력 시스템
```rust
pub struct InputMapping {
    pub move_forward: KeyCode,
    pub move_backward: KeyCode,
    pub move_left: KeyCode,
    pub move_right: KeyCode,
    pub attack: MouseButton,
    pub roll: KeyCode,
    pub interact: KeyCode,
}
```

### 애니메이션 시스템
- **bevy_animation**: 캐릭터 애니메이션
- **AnimationState**: Idle, Walk, Run, Attack, Roll, Death

## 성능 최적화 계획

### 1. 렌더링 최적화
- LOD (Level of Detail) 시스템
- 프러스텀 컬링
- 인스턴싱 (반복 오브젝트들)

### 2. AI 최적화
- AI 업데이트 빈도 조절
- 거리 기반 AI 활성화/비활성화
- 상태 기반 최적화

### 3. 메모리 최적화
- 에셋 풀링
- 컴포넌트 최적화
- 불필요한 데이터 제거

## 테스트 계획

### 1. 단위 테스트
- 개별 시스템 테스트
- 컴포넌트 기능 테스트

### 2. 통합 테스트
- 시스템 간 상호작용 테스트
- 게임플레이 시나리오 테스트

### 3. 성능 테스트
- FPS 측정
- 메모리 사용량 모니터링
- 프로파일링

## 빌드 및 배포

### 1. 개발 빌드
```bash
cargo run --features dev
```

### 2. 릴리즈 빌드
```bash
cargo build --release
```

### 3. 에셋 최적화
- 텍스처 압축
- 모델 최적화
- 사운드 압축

이 구현 플랜을 따라 단계적으로 개발하여 완성도 높은 소울라이크 게임을 만들겠습니다.