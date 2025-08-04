# 애니메이션 FSM 시스템 상세 문서

## 구현된 애니메이션 상태

### 1. Idle (대기)
- **조건**: 속도 0, 다른 액션 없음
- **지속시간**: 무한 (반복)
- **인터럽트**: 가능
- **파일**: `Idle.FBX`

### 2. Walk (걷기)
- **조건**: 속도 > 0.1, Shift 미사용
- **지속시간**: 무한 (반복)
- **인터럽트**: 가능
- **파일**: `Jog_Fwd_Combat.FBX`

### 3. Run (달리기)
- **조건**: Shift + 이동, 속도 > 3.0
- **지속시간**: 무한 (반복)
- **인터럽트**: 가능
- **스테미나**: 초당 10 소모
- **파일**: `Jog_Fwd.FBX`

### 4. Roll (회피)
- **조건**: Space 키, 스테미나 >= 20
- **지속시간**: 0.7초
- **인터럽트**: 불가능
- **스테미나**: 20 소모
- **파일**: `Ability_RMB_Fwd.FBX`

### 5. Jump (점프)
- **조건**: Y 속도 > 1.0
- **지속시간**: 1.0초
- **인터럽트**: 가능
- **파일**: `Jump_Start.FBX`

### 6. Slash (공격)
- **조건**: 좌클릭, AttackState.is_attacking
- **지속시간**: 0.8초
- **인터럽트**: 불가능
- **파일**: `Primary_Attack_A.FBX`

### 7. Parry (패링)
- **조건**: Shift + 우클릭, 스테미나 >= 10
- **지속시간**: 0.5초
- **인터럽트**: 불가능
- **스테미나**: 10 소모
- **파일**: `HitReact_Front.FBX`

### 8. Death (사망)
- **조건**: 체력 <= 0
- **지속시간**: 2.0초
- **인터럽트**: 불가능
- **파일**: `Death.FBX`

## AnimationController 구조

```rust
pub struct AnimationController {
    pub current_state: AnimationState,
    pub previous_state: AnimationState,
    pub transition_timer: Timer,
    pub can_interrupt: bool,
    pub is_transitioning: bool,
}
```

## 상태 전환 우선순위

1. Death (최우선)
2. 인터럽트 불가능한 진행중 애니메이션
3. Roll
4. Parry
5. Slash
6. Jump
7. Run/Walk
8. Idle

## 시스템 실행 순서

1. `update_animation_state` - 상태 결정 (Update)
2. `play_animations` - 애니메이션 재생 (Update)
3. `load_aurora_animations` - 애니메이션 로드 (Update)

## 디버그 로그

- 상태 변경 시: `"Animation state: {:?} -> {:?}"`
- 애니메이션 재생 시: `"Playing animation: {:?}"`
- 애니메이션 로드 시: `"Aurora animations loaded"`