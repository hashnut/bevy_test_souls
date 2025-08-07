# Z1: 순례자의 오르막길 (The Pilgrim's Ascent) 배치 플랜

## 구역 개요
- **위치:** 월드 남쪽 경계 부근
- **고도 변화:** 점진적인 상승 구조 (Y: 0m ~ 150m)
- **분위기:** 황량, 안개, 몰락의 서막

## 상세 배치 데이터 (Placement Data)

### SubArea 1: 각성의 동굴 (Cave of Awakening)
시작 지점. 동굴을 나서며 월드의 거대함을 조망.

```yaml
# Local Origin: (0, 0, 0) - 튜토리얼 시작점 기준 상대 좌표
Objects:
  - Asset: Environment/Cave/Entrance_Set_Large
    Position: [0, 0, 0]
    Rotation_Y: 180 # 남쪽 방향에서 시작하여 북쪽(월드 중심)을 바라보도록 설정
    Scale: [1.5, 1.5, 1.5]

  - Asset: Props/Statue/Pilgrim_Broken_A
    Position: [-5, 0.1, 10]
    Rotation_Y: 45
    Scale: [1.2, 1.2, 1.2]

  - Asset: Lighting/Torch_Flickering_Wall
    Position: [3, 2.5, 5]
    Scale: [1, 1, 1]

  # 동굴 출구 (Vista Point)
  - Asset: VFX/Fog_Volume_Medium
    Position: [0, 1, 30]
    Scale: [20, 10, 10]
    Notes: 출구 밖의 안개 볼륨. 시야를 극적으로 연출.

  - Asset: Trigger/Vista_Reveal_Z6
    Position: [0, 1, 35]
    Notes: 플레이어가 도달 시 Z6(왕관의 심장부) 랜드마크 강조 연출 트리거.
```

### SubArea 2: 위태로운 절벽길 (Precarious Cliffs)
낙사 위험과 기본 전투를 학습하는 선형 경로.

```yaml
# 경로 정의 (Path Definition for Procedural Placement)
PathWaypoints:
  - [0, 1, 40]
  - [20, 5, 60]
  - [15, 10, 100]
  - [50, 15, 130]

# 절벽길 주변 식생 (Procedural Scattering)
Scattering_Vegetation:
  Assets: ["Vegetation/Tree_Dead_Twisted_A", "Vegetation/Tree_Dead_Pine_B"]
  Density: 0.1 # 평방미터 당 밀도
  PlacementRange: 8 # 경로 좌우 8m 이내
  ScaleRange: [0.8, 1.5]
  AlignToNormal: true # 지형 경사에 맞춰 배치

Scattering_Rocks:
  Assets: ["Environment/Rock/Rock_Sharp_Small", "Environment/Rock/Rock_Medium_A"]
  Density: 0.3
  PlacementRange: 10
  ScaleRange: [0.5, 2.0]

# 특정 위치 오브젝트 배치
SpecificObjects:
  - Asset: Props/Corpse/Pilgrim_Dried_A
    Position: [22, 5.1, 65]
    Rotation_X: 20 # 앞으로 기울어짐
    Notes: 절벽 가장자리에 배치하여 아이템 루팅 유도 및 위험 강조.
```

### SubArea 3: 검문소 폐허 (Checkpoint Ruins)
첫 번째 체크포인트 및 소규모 전투 거점. Z2, Z3 갈림길.

```yaml
# Local Origin: (100, 20, 250) - 월드 좌표계 기준 추정 위치
Structures:
  - Asset: Architecture/Ruins/Watchtower_Stone_Broken
    Position: [0, 0, 5]
    Rotation_Y: 210
    Scale: [1.2, 1.2, 1.2]

  - Asset: Architecture/Gate/Iron_Rusted_Large
    ID: Z1_MainGate
    Position: [15, 0, 0]
    Rotation_Y: 90
    Scale: [1, 1, 1]
    State: Closed

  - Asset: Interactable/Lever_Rusted
    Position: [5, 2, 5] # 망루 내부
    LinkedTo: Z1_MainGate # 레버와 문 연결

# 엄폐물 배치
Barricades:
  - Asset: Props/Barricade/Wooden_Spiked
    Position: [10, 0, -10]
    Rotation_Y: 100
  - Asset: Props/Crate/Crate_Broken_Set
    Position: [13, 0, -8]
    Rotation_Y: 10

# 체크포인트
Checkpoint:
  - Asset: Soulslike/Checkpoint_CrownRemnant
    Position: [0, 0, 15] # 망루 뒤 안전 구역
```