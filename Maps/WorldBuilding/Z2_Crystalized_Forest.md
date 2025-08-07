# Z2: 수정화된 숲 (The Crystalized Forest) 배치 플랜

## 구역 개요
- **위치:** 월드 남동쪽
- **고도:** 완만한 구릉지 (Y: 50m ~ 100m)
- **분위기:** 기괴한 아름다움, 정적, 푸른/보라색 발광, 시야 제한

## 상세 배치 데이터 (Placement Data)

### SubArea 1: 수정 나무 군락 (Crystal Tree Grove)
맵 전역에 걸친 고밀도 식생 배치. 미로 형태 구성.

```yaml
# 주요 식생 (Procedural Scattering - 전체 구역 적용)
Vegetation_Primary:
  # 거대 수정 나무 (랜드마크 및 주요 장애물)
  Assets: ["Vegetation/Crystal/Tree_Large_Blue", "Vegetation/Crystal/Tree_Large_Purple"]
  Density: 0.05
  ScaleRange: [2.0, 4.5]
  EmissionStrength: 2.5 # 자체 발광 강도
  PlacementPattern: Clustered # 군집 형태로 배치

Vegetation_Secondary:
  # 중간 크기 수정 나무 (시야 차단 및 경로 형성)
  Assets: ["Vegetation/Crystal/Tree_Medium_A", "Vegetation/Crystal/Tree_Medium_B"]
  Density: 0.3
  ScaleRange: [1.0, 2.0]
  PlacementPattern: Dense_Random

Undergrowth:
  # 바닥 식생 (발광 효과 및 이동 속도 저하)
  Assets: ["Vegetation/Crystal/Grass_Glowing", "Vegetation/Crystal/Fern_Sharp"]
  Density: 1.5
  ScaleRange: [0.5, 1.2]
```

### SubArea 2: 소음 함정 지대 (Sound Trap Area)
특정 구간에 소음 유발 오브젝트를 집중 배치하여 은신 플레이 유도.

```yaml
# 함정 구역 정의 (Bounding Box 예시)
AreaDefinition:
  Min: [500, 55, 300]
  Max: [550, 56, 400]

# 해당 구역 바닥 오브젝트 (Procedural Scattering - 구역 한정)
GroundCover:
  Asset: Environment/Ground/Crystal_Shards_Sharp
  Density: 5.0 # 매우 빽빽하게
  ScaleRange: [0.5, 1.0]
  Properties:
    SoundTrigger: true # 밟으면 소음 발생
    SoundRadius: 30 # 소음 전파 범위 (m)

# 분위기 연출
Ambiance:
  - Asset: VFX/Crystal_Dust_Motes_Blue
    Position: [525, 60, 350]
    VolumeSize: [50, 10, 100]
    Notes: 공중에 떠다니는 수정 먼지 VFX.
```

### SubArea 3: 파편 추락지 (Shard Crash Site) - 미니 보스 아레나
중앙에 거대한 파편이 박힌 원형 분지. 환경 위험 요소 존재.

```yaml
# Local Origin: (800, 60, 700)
# 형태: 직경 100m 원형 아레나

Centerpiece:
  - Asset: Environment/Crown_Shard_Giant_Impacted
    Position: [0, -10, 0] # 땅에 비스듬히 박힘
    Rotation: {X: 45, Y: 20, Z: 10}
    Scale: [5, 5, 5]
    EmissionColor: Bright_Cyan

EnvironmentalHazards:
  - Asset: VFX/Energy_Pulse_Wave_Cyan
    Position: [0, 0, 0]
    TriggerInterval: 25s # 25초마다 파편에서 에너지 파동 발생

# 엄폐물 및 환경 스토리텔링
Debris:
  - Asset: Props/Corpse/Beast_Crystalized_Large
    Position: [20, 0, 30]
    Rotation_Y: 120
    Notes: 수정화가 진행 중인 거대 짐승 사체. 엄폐물 역할.
  - Asset: Props/Corpse/Beast_Crystalized_Large
    Position: [-30, 0, -20]
    Rotation_Y: -45
```