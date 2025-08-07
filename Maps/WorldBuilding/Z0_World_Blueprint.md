# 월드 청사진: 부서진 왕관과 중력 역병 (The Shattered Crown and Gravity Blight)

## 월드 개요
- **크기:** 4km x 4km (4000m x 4000m)
- **엔진:** Bevy
- **핵심 컨셉:** 중력 이상과 수정 역병으로 몰락한 고대 산악 왕국. 중앙 상공의 '왕관'이 최종 목표.
- **디자인 원칙:** 수직성(Verticality), 유기적 연결성(Interconnectivity), 랜드마크 기반 내비게이션.

## 구역 레이아웃 (Zone Layout)

월드 중심(2000, Y, 2000)을 기준으로 배치 가정.

| ID | 구역명 | 위치 | 크기 (근사치) | 주요 특징 |
| :-- | :--- | :--- | :--- | :--- |
| Z1 | 순례자의 오르막길 (The Pilgrim's Ascent) | 남쪽 (South) | 1km x 1km | 시작 지점, 튜토리얼, 험준한 산길 |
| Z2 | 수정화된 숲 (The Crystalized Forest) | 남동쪽 (SE) | 1.5km x 1km | 기괴한 변이, 시야 제한, 마법 위험 |
| Z3 | 가라앉은 도시 에델 (The Sunken City of Aethel) | 남서쪽 (SW) | 1.5km x 1.5km | 침수된 고딕 도시, 수직 탐험, 폐허 |
| Z4 | 중력 이상 광산 (The Gravity-Defying Mines) | 북서쪽 (NW) | 1km x 1.5km | 부유하는 바위, 산업 지대, 플랫포밍 |
| Z5 | 황금빛 요새 (The Gilded Fortress) | 북동쪽 (NE) | 1km x 1km | 레거시 던전, 고밀도, 고난이도 |
| Z6 | 왕관의 심장부 (The Crown's Heart) | 중앙 상공 (Center Sky) | 직경 1km | 최종 지역, 중력 혼돈, 보스 아레나 |

## 구역 연결성 (Interconnectivity)

```mermaid
graph TD
    A[Z1. 순례자의 오르막길] --> B[Z3. 가라앉은 도시 에델];
    A --> C[Z2. 수정화된 숲];
    B -- 지하 수로 --> D[Z4. 중력 이상 광산];
    C -- 협곡 통로 --> E[Z5. 황금빛 요새];
    D --> F(Z6. 왕관의 심장부);
    E --> F;
    B -- 대승강기 (숏컷) --> E;