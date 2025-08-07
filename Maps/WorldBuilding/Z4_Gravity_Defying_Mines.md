# Z4: 중력 이상 광산 (The Gravity-Defying Mines) 배치 플랜

## 구역 개요
- **위치:** 월드 북서쪽
- **고도:** Y: 100m ~ 600m (극단적인 수직 구조)
- **분위기:** 혼돈, 산업적 황폐함, 부유하는 섬, 뒤틀린 중력

## 상세 배치 데이터 (Placement Data)

### SubArea 1: 부유하는 채굴 현장 (Floating Quarry)
거대한 바위 덩어리들을 공중에 배치하고, 나무 비계와 끊어진 레일로 연결. 플랫포밍 핵심 구간.

```yaml
# Local Origin: (1000, 200, 3000)

# 주요 이동 경로가 되는 거대 부유석
FloatingIslands:
  - Asset: Environment/Rock/Floating_Island_Large_A
    Position: [0, 0, 0]
    Scale: [5, 3, 5]
    Movement: Slow_Bobbing # 천천히 위아래로 움직임 (Y축 ±2m)

  - Asset: Environment/Rock/Floating_Island_Medium_B
    Position: [100, 30, 50]
    Rotation: {X: 10, Y: 45, Z: 0}
    Scale: [3, 2, 3]
    Movement: Orbiting # 특정 지점을 중심으로 공전

  - Asset: Environment/Rock/Floating_Island_Large_B
    Position: [-100, 50, 100]
    Rotation: {X: 0, Y: 180, Z: 5}
    Scale: [4, 4, 4]
    Movement: Static

# 섬들을 연결하는 구조물
Connections:
  - Asset: Props/Structure/Scaffolding_Wood_Bridge
    Position: [50, 15, 25] # Island A와 B 연결 지점
    Rotation_Y: 45 # 연결 각도에 따라 조정 필요

  - Asset: Props/Structure/Minecart_Rail_Broken_Set
    Position: [0, 5, 0] # Island A 위에 배치
    Rotation_Y: 20
```

### SubArea 2: 중력 제어 장치 (Gravity Control Apparatus)
특정 구역의 중력 방향을 바꾸거나 부유석의 위치를 바꾸는 퍼즐 기믹 구간.

```yaml
# Local Origin: (1200, 350, 3500)

Machinery:
  - Asset: Machinery/Ancient/Gravity_Core_Generator
    Position: [0, 0, 0]
    Scale: [3, 3, 3]
    State: Active

  - Asset: Interactable/Lever_Heavy
    ID: Gravity_Control_Lever_1
    Position: [10, 0, 0]
    LinkedTo: Gravity_Zone_A

# 중력 변화 구역 정의
Gravity_Zone_A:
  BoundingBox: {Min: [50, -50, 0], Max: [150, 50, 100]}
  DefaultGravity: Y_Negative
  ToggledGravity: X_Positive # 레버 작동 시 중력이 X+ 방향으로 변경 (벽을 걸음)
```

### SubArea 3: 수직 갱도 (Vertical Shaft)
깊은 수직 구멍. 낙하를 이용한 빠른 하강(Fall Control) 및 상승 기류 활용 구간.

```yaml
# Local Origin: (800, 100, 3800)

# 모듈 반복 배치로 깊은 갱도 생성
ShaftStructure:
  - Asset: Environment/Mineshaft/Vertical_Module_A
    RepeatY: 40 # Y축으로 40회 반복
    Interval: 10 # 10m 간격 (총 400m 깊이)

# 낙하 중 멈출 수 있는 중간 플랫폼
Platforms:
  - Asset: Props/Platform/Wood_Small
    Position: [0, 80, 0]
  - Asset: Props/Platform/Wood_Broken
    Position: [5, 30, 5]
    Notes: 부서진 플랫폼. 착지 시 주의 필요.

# 최상층 출구 (Y=500 지점)
ExitPoint:
  - Asset: Environment/VFX/Gravity_Lift_Stream_Vertical
    Position: [0, 450, 0]
    Destination: Z6_Entrance
    Notes: Z6(왕관의 심장부) 하부로 연결되는 강력한 상승 기류.
```