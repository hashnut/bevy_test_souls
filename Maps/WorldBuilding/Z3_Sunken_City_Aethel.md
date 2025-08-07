# Z3: 가라앉은 도시 에델 (The Sunken City of Aethel) 배치 플랜

## 구역 개요
- **위치:** 월드 남서쪽
- **고도:** Y: 10m (수면) ~ 150m (첨탑 꼭대기). 극단적인 수직성
- **분위기:** 몰락한 웅장함, 습함, 고딕 폐허

## 상세 배치 데이터 (Placement Data)

### 환경 설정 (Global Environment)
```yaml
WaterPlane:
  - Asset: Environment/Water/Murky_Surface
    Y_Level: 10.0 # Y=10m 이하의 모든 구조물은 물에 잠김
    UnderwaterProperties:
      FogDensity: 0.1
      FogColor: Dark_Green
      MovementMultiplier: 0.6 # 물속 이동 속도 저하
```

### SubArea 1: 침수된 광장 (Flooded Plaza)
도시의 진입부. 수중 위험과 지붕 이동의 도입부.

```yaml
# Local Origin: (500, 0, 1000) - 수면 높이(Y=10) 고려하여 배치

Structures:
  # 중앙 거대 동상 (랜드마크)
  - Asset: Architecture/Statue/King_Giant_Headless
    Position: [0, -5, 0] # 하반신은 물에 잠김 (Y=10 수면 위로 상체만 보임)
    Scale: [3, 3, 3]

  # 주변 건물 (지붕 이동 경로 확보)
  - Asset: Architecture/Gothic/House_A_Ruined
    Position: [50, 0, 0]
    Rotation_Y: 90
    SinkLevel: 15 # 15m 가라앉음 (Y=0 기준)
  - Asset: Architecture/Gothic/House_B_Intact
    Position: [-50, 0, 0]
    Rotation_Y: -90
    SinkLevel: 10

# 물 위에 떠다니는 잔해 (동적 또는 정적 발판)
FloatingDebris:
  Assets: ["Props/Debris/Crate_Broken_Floating", "Props/Debris/Wood_Plank_Set_Floating"]
  Density: 0.2
  Area: {Center: [0, 10, 0], Radius: 80}
```

### SubArea 2: 기울어진 첨탑 지구 (Tilted Spires District)
건물들을 의도적으로 기울여 배치하여 불안정함 강조 및 독특한 수직 이동 경로 생성.

```yaml
# Local Origin: (700, 0, 1200)

Structures:
  - Asset: Architecture/Gothic/Tower_Clock_Gothic
    Position: [0, 20, 0]
    Rotation: {X: 15, Y: 0, Z: 5} # X, Z축으로 기울임
    Scale: [1.2, 1.2, 1.2]

  - Asset: Architecture/Gothic/Tower_Spire_A
    Position: [50, 10, 50]
    Rotation: {X: -10, Y: 45, Z: 0}

  # 두 타워를 연결하는 구조물
  - Asset: Architecture/Bridge/Stone_Broken_Medium
    Position: [25, 80, 25]
    Rotation_Y: 45
    Notes: 시계탑과 첨탑 A를 연결하는 부서진 다리. 점프 필요.

  # 가고일 석상 (일부는 적으로 활성화)
  - Asset: Props/Statue/Gargoyle_Perched
    Position: [5, 85, 5]
    Rotation_Y: 225
    IsEnemySpawner: false
  - Asset: Props/Statue/Gargoyle_Perched
    Position: [45, 75, 55]
    Rotation_Y: 225
    IsEnemySpawner: true
```

### SubArea 3: 대성당 (The Grand Cathedral)
구역의 핵심 랜드마크. 내부 던전 및 중요 숏컷 존재.

```yaml
# Local Origin: (1000, 12, 1500)

Exterior:
  - Asset: Architecture/Dungeon/Cathedral_Exterior_Set
    Position: [0, 0, 0]
    Rotation_Y: 180
    Scale: [2, 2, 2]

# 내부 씬 로드 시 배치될 주요 오브젝트 (Internal Scene Data)
InteriorFeatures:
  - Asset: Props/Lighting/Chandelier_Giant_Crashed
    Position: [Internal_Center]
    State: Crashed
    Notes: 바닥에 떨어져 장애물 및 경로 역할.

  - Asset: Interactable/Elevator_Ancient_GreatLift
    Position: [Internal_Basement_Center]
    Destination: Z5_Fortress_LowerLevel
    Notes: Z5(황금빛 요새) 하부로 연결되는 고대 대승강기 (거대 숏컷).
```