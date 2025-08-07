# Z6: 왕관의 심장부 (The Crown's Heart) 배치 플랜

## 구역 개요
- **위치:** 월드 중앙 상공 (X: 2000, Z: 2000 부근)
- **고도:** Y: 1000m ~ 1200m
- **분위기:** 초현실적, 코스믹 호러, 웅장함, 에너지의 소용돌이.

## 상세 배치 데이터 (Placement Data)

### 환경 설정 (Global Environment)
```yaml
Environment:
  - Asset: Skybox/Cosmic_Void_Nebula_Storm
  - Asset: VFX/Global/Energy_Vortex_Giant_Central
    Position: [2000, 1100, 2000]
    Scale: [20, 20, 20]
SubArea 1: 파편의 소용돌이 (The Shard Vortex)
최종 지역으로 향하는 길. 거대한 수정 파편들이 소용돌이치며 불안정한 발판을 형성. 고난도 플랫포밍.

YAML

# Local Origin: (2000, 1000, 2500) - Z5 연결부 기준 추정 위치

# 부유 플랫폼 (스크립트를 통한 동적 움직임 필요)
FloatingPlatforms:
  - Asset: Environment/Crystal/Crown_Shard_Platform_Large
    Position: [0, 0, 0] # 시작 지점
    Rotation: [0, 0, 0]
    Movement: Static

  - Asset: Environment/Crystal/Crown_Shard_Platform_Medium
    Position: [100, 20, -100]
    Rotation: {X: 10, Y: 0, Z: 15}
    Movement: Orbiting_Center_Slow # 중심부를 향해 천천히 공전

  - Asset: Environment/Crystal/Crown_Shard_Platform_Small
    Position: [150, 50, -200]
    Rotation_Y: 30
    Movement: Orbiting_Center_Fast # 빠르게 공전

  - Asset: Environment/Crystal/Crown_Shard_Platform_Tilted
    Position: [200, 70, -300]
    Rotation: {X: 60, Y: 0, Z: 0}
    Notes: 급경사 플랫폼. 미끄러짐 주의.

  # ... (중심부까지 경로를 따라 다수 배치)
SubArea 2: 공허의 제단 (Void Altar) - 최종 보스 아레나
모든 것의 중심. 시각적으로 가장 압도적인 공간. 보스전에 따라 환경이 변화.

YAML

# Local Origin: (2000, 1100, 2000)
# 형태: 직경 200m의 원형 플랫폼.

ArenaPlatform:
  - Asset: Environment/Altar/Platform_Central_Fractured
    Position: [0, 0, 0]
    Scale: [1, 1, 1]
    Notes: 보스 페이즈 전환 시 외곽 부분이 부서지거나 재구성되는 스크립트 연동 필요.

# 중심 핵
Centerpiece:
  - Asset: Environment/Crystal/Crown_Heart_Core_Pulsating
    Position: [0, 50, 0] # 플랫폼 중앙 상공
    RotationSpeed: [0, 5, 0] # 천천히 회전 (Y축 기준 분당 5도)
    VFX: Intense_Energy_Discharge

# 보스전 연동 동적 요소
DynamicElements:
  - EventTrigger: BossPhase2
    Action: Activate_Gravity_Inversion_Sequence # 페이즈 2 돌입 시 중력 역전 시퀀스 시작
  - EventTrigger: BossPhase3
    Action: Spawn_Floating_Platforms # 페이즈 3 돌입 시 추가 부유 플랫폼 스폰