# 현재 이슈 및 해결 상황

## 🚨 메인 이슈: 캐릭터 모델이 보이지 않음

### 문제 상황
- GLB 모델 파일들은 정상적으로 로드됨
- 디버그 출력에서 모든 메시 컴포넌트 확인 (SciFi_Body, SciFi_Head 등)
- 하지만 게임 화면에서는 칼만 보이고 캐릭터는 보이지 않음

### 디버그 정보
```
Entity 41v1 [SciFi_Body] - [HAS MESH]
Entity 42v1 [SciFi_Feet] - [HAS MESH] 
Entity 43v1 [SciFi_Head] - [HAS MESH]
Entity 44v1 [SciFi_Legs] - [HAS MESH]
```
모든 부위가 메시를 가지고 있지만 view_visibility: false

### 시도한 해결책
1. **모델 변경**: main_skeleton.glb → scifi_full.glb
2. **스케일 조정**: 0.01 → 1.0 → default
3. **디버그 시스템 추가**: 씬 계층 구조 출력
4. **프로시저럴 캐릭터 테스트**: 정상 작동 확인

### 가능한 원인
1. **초기 프레임 문제**: 첫 프레임에서 아직 렌더링되지 않음
2. **Transform 계층 문제**: 부모-자식 Transform 누적
3. **카메라 위치**: 캐릭터가 카메라 범위 밖에 있을 수 있음
4. **Material 문제**: GLB의 머티리얼이 로드되지 않았을 수 있음

### 다음 시도할 방법
1. 카메라 거리 조정 (현재 5유닛 → 더 멀리)
2. 캐릭터 위치 직접 설정 (0, 0, 0)
3. Visibility 컴포넌트 강제 설정
4. 조명 밝기 증가
5. 간단한 큐브 메시로 테스트

## 📋 기타 마이너 이슈

### 1. 경고 메시지
- 사용하지 않는 import들
- 사용하지 않는 변수들
- → cargo clippy로 정리 필요

### 2. 프로젝트 구조
- model_loader.rs와 asset_loader.rs 중복
- → 하나로 통합 필요

### 3. 애니메이션 시스템
- 아직 구현되지 않음
- GLB 파일의 애니메이션 활용 필요

## ✅ 해결된 이슈

1. **컴파일 에러**: SceneBundle deprecated → SceneRoot 사용
2. **애셋 로딩**: 상태 기반 로딩 시스템 구현
3. **칼 부착**: 플레이어 엔티티에 성공적으로 부착

---
**마지막 업데이트**: 2025-08-01