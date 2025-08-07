# Z5: 황금빛 요새 (The Gilded Fortress) 배치 플랜 - 레거시 던전

## 구역 개요
- **위치:** 월드 북동쪽 고지대
- **고도:** Y: 300m ~ 500m (고밀도 다층 구조)
- **분위기:** 압도적, 위협적, 몰락한 영광. 황금빛과 수정의 조화

## 상세 배치 데이터 (Placement Data)

### SubArea 1: 외성벽 및 진입로 (Outer Walls and Approach)
강력한 방어선 구축. 진입 자체가 도전이 되도록 설계. 원거리 공격 다수 배치.

```yaml
# Local Origin: (3500, 300, 3000)

MainStructure:
  - Asset: Architecture/Fortress/Gilded_Wall_Gatehouse
    Position: [0, 0, 0]
    Scale: [1.5, 1.5, 1.5]

  # 모듈러 성벽 배치
  - Asset: Architecture/Fortress/Gilded_Wall_Segment
    Position: [50, 0, 0]
    Scale: [1.5, 1.5, 1.5]
  - Asset: Architecture/Fortress/Gilded_Wall_Segment
    Position: [-50, 0, 0]
    Scale: [1.5, 1.5, 1.5]

# 성벽 위 자동 방어 포탑
Defenses:
  - Asset: Machinery/Turret/Crystal_Auto_Cannon
    Position: [20, 30, 0]
    Rotation_Y: 180
  - Asset: Machinery/Turret/Crystal_Auto_Cannon
    Position: [-20, 30, 0]
    Rotation_Y: 180

# 진입 다리 (중력 이상으로 일부 부서지고 떠 있음)
Entrance:
  - Asset: Architecture/Bridge/Grand_Segment_A
    Position: [0, 0, -50]

  - Asset: Architecture/Bridge/Grand_Segment_B_Floating
    Position: [0, 5, -100]
    Rotation: {X: 5, Y: 0, Z: 0}
    Notes: 약간 떠 있는 구간. 점프 또는 숨겨진 기믹으로 건너야 함.
```

### SubArea 2: 병영 및 훈련소 (Barracks and Training Grounds) - 내부
좁은 복도와 넓은 공간이 반복. 강력한 정예 적 배치. 전략적 엄폐물과 함정 활용.

```yaml
# 내부 씬 데이터 (Internal Scene Data)

Layout:
  # 훈련소 (넓은 공간, 중앙 아레나)
  - Asset: Architecture/Room/Training_Hall_Large
    Position: [Internal_Origin]

  # 병영 (좁은 복도와 다수의 방)
  - Asset: Architecture/Corridor/Barracks_Set
    Position: [Internal_Origin + (50, 0, 0)]

# 훈련소 내부 프롭 (파괴 가능 오브젝트)
Props_TrainingHall:
  - Asset: Props/Training/Dummy_Knight
    Position: [Internal_Origin + (10, 0, 5)]
  - Asset: Props/Training/Weapon_Rack_Gilded
    Position: [Internal_Origin + (15, 0, 0)]

# 병영 내부 프롭 (길막기 및 분위기 조성) - 프로시저럴 배치
Props_Barracks:
  - Asset: Props/Furniture/BunkBed_Metal
    Density: 0.2
    PlacementArea: Barracks_Rooms
  - Asset: Props/Furniture/Locker_Rusted
    Density: 0.1
    PlacementArea: Barracks_Rooms
```

### SubArea 3: 왕좌로 가는 길 (Path to the Throne)
복잡한 미로 구조 극대화. 숏컷, 환영 벽(Illusory Walls), 비밀 통로 강조.

```yaml
# Local Origin: (3500, 400, 3500)

Structures:
  # 메인 승강기 (처음에는 작동 안 함, 숏컷 역할)
  - Asset: Interactable/Elevator_Grand_Royal
    ID: Z5_Main_Elevator
    Position: [0, 0, 0]
    State: Disabled_Top # 상층부에 고정된 상태

  # 우회 경로 (나선형 계단 타워)
  - Asset: Architecture/Tower/Staircase_Spiral_Large
    Position: [50, 0, 0]

# 비밀 통로 및 함정
Secrets:
  - Asset: Environment/Wall/Illusory_Wall_Trigger
    Position: [50, 10, 20] # 계단 중간의 특정 벽
    Disguise: Wall_Tapestry_Royal # 화려한 태피스트리로 위장

  - Asset: Prefab/Trap/Floor_Spike
    Position: [0, 0.1, 20]

# 숏컷 장치
Shortcuts:
  - Asset: Interactable/Ladder_Deployable_Long
    Position: [-50, 50, 0]
    State: Hidden
    Notes: 활성화 시 하층부(Y:0)로 바로 연결되는 사다리.
```