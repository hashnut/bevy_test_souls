# Aurora 애니메이션 매핑 가이드

## 소울라이크 게임 애니메이션 상태별 파일 매핑

### 1. **Idle (대기)**
- **추천 파일**: `Idle.FBX`
- **대안**: `Idle_Straight.FBX`, `Idle_Noise_A.FBX`, `Idle_Noise_B.FBX`
- **용도**: 캐릭터가 아무 행동도 하지 않을 때의 기본 자세

### 2. **Walk (걷기)**
- **추천 파일**: `Jog_Fwd.FBX`
- **대안**: 
  - 전진: `Jog_Fwd_Combat.FBX` (전투 중 걷기)
  - 후진: `Jog_Bwd.FBX`, `Jog_Bwd_Combat.FBX`
  - 좌측: `Jog_Left.FBX`, `Jog_Left_Combat.FBX`
  - 우측: `Jog_Right.FBX`, `Jog_Right_Combat.FBX`
- **용도**: 느린 속도로 이동할 때

### 3. **Run (달리기)**
- **추천 파일**: `Jog_Fwd.FBX` (속도를 높여서 사용)
- **시작**: `Jog_Fwd_Start.FBX`
- **정지**: `Jog_Fwd_Stop.FBX`
- **용도**: Shift 키를 눌러 빠르게 이동할 때

### 4. **Roll (회피/구르기)**
- **추천 파일**: `Ability_RMB_Fwd.FBX` (전방 회피)
- **방향별**:
  - 전방: `Ability_RMB_Fwd.FBX`
  - 후방: `Ability_RMB_Bwd.FBX`
  - 좌측: `Ability_RMB_Left.FBX`
  - 우측: `Ability_RMB_Right.FBX`
  - 대각선: `Ability_RMB_FwdLeft.FBX`, `Ability_RMB_FwdRight.FBX`, `Ability_RMB_BwdLeft.FBX`, `Ability_RMB_BwdRight.FBX`
- **용도**: Space 키로 긴급 회피

### 5. **Jump (점프)**
- **추천 시퀀스**:
  - 시작: `Jump_Start.FBX`
  - 공중: `Jump_Apex.FBX`
  - 착지: `Jump_Land.FBX`
  - 회복: `Jump_Recovery.FBX`
- **용도**: 점프 동작 전체 사이클

### 6. **Slash (공격)**
- **추천 콤보**:
  - 첫 번째: `Primary_Attack_A.FBX`
  - 두 번째: `Primary_Attack_B.FBX`
  - 세 번째: `Primary_Attack_C.FBX`
- **빠른 공격**: `Primary_Attack_Fast_A.FBX`, `Primary_Attack_Fast_B.FBX`, `Primary_Attack_Fast_C.FBX`, `Primary_Attack_Fast_D.FBX`
- **강공격**: `Primary_Attack_Slow_A.FBX`, `Primary_Attack_Slow_B.FBX`, `Primary_Attack_Slow_C.FBX`, `Primary_Attack_Slow_D.FBX`
- **공중 공격**: `Melee_Attack_Air.FBX`
- **용도**: 좌클릭 기본 공격

### 7. **Parry (패링/방어)**
- **추천 파일**: `HitReact_Front.FBX` (정면 방어 자세)
- **대안**: `Bound.FBX`, `Stun_Start.FBX`
- **용도**: Shift + 우클릭으로 공격 막기

### 8. **Death (사망)**
- **추천 파일**: `Death.FBX`
- **대안**: `Knockback_Fwd.FBX`, `Knockback_Bwd.FBX`
- **용도**: 체력이 0이 되었을 때

## 추가 애니메이션 (선택사항)

### 피격 반응
- 전방 피격: `HitReact_Front.FBX`
- 후방 피격: `HitReact_Back.FBX`
- 좌측 피격: `HitReact_Left.FBX`
- 우측 피격: `HitReact_Right.FBX`

### 회전 애니메이션
- 90도 좌회전: `Turn_Left_90.FBX`
- 90도 우회전: `Turn_Right_90.FBX`  
- 180도 좌회전: `Turn_Left_180.FBX`
- 180도 우회전: `Turn_Right_180.FBX`
- 제자리 회전: `Turn_In_Place_Fast.FBX`, `Turn_In_Place_Slow.FBX`

### 특수 동작
- 시전: `Cast.FBX`
- 기절: `Stun_Loop.FBX`
- 레벨 시작: `Level_Start.FBX`

## 구현 예시 코드

```rust
// src/systems/animation.rs에서 수정
let clips = AnimationClips {
    idle: Some(asset_server.load("Animations/Aurora/Idle.FBX")),
    walk: Some(asset_server.load("Animations/Aurora/Jog_Fwd.FBX")),
    run: Some(asset_server.load("Animations/Aurora/Jog_Fwd.FBX")), // 재생 속도 조절로 달리기 표현
    roll: Some(asset_server.load("Animations/Aurora/Ability_RMB_Fwd.FBX")),
    jump: Some(asset_server.load("Animations/Aurora/Jump_Start.FBX")),
    slash: Some(asset_server.load("Animations/Aurora/Primary_Attack_A.FBX")),
    parry: Some(asset_server.load("Animations/Aurora/HitReact_Front.FBX")),
    death: Some(asset_server.load("Animations/Aurora/Death.FBX")),
};
```

## 참고사항

1. **MSA 파일**: Motion Selective Animation의 약자로, 특정 부위만 애니메이션하는 파일
2. **InMotion 파일**: 이동 중에 사용하는 애니메이션 변형
3. **Combat 파일**: 전투 중 사용하는 애니메이션 변형
4. **Additive 파일**: 다른 애니메이션 위에 덧붙여 사용하는 보조 애니메이션

5. **Great_Sword 폴더**: 대검 무기 전용 애니메이션 (필요시 무기별 애니메이션 시스템 구현)