# Dark Rust - Animation System 체크포인트
**날짜**: 2025-08-02
**세션 주제**: Aurora 캐릭터 전환 및 애니메이션 FSM 시스템 구현

## 🎯 세션 목표 및 달성 상황

### ✅ 완료된 작업

1. **Aurora 캐릭터로 변경**
   - Vanguard → Aurora 모델 전환
   - Y축 오프셋 조정 (-1.0)
   - 스케일 조정 (1.0)

2. **애니메이션 FSM 시스템 구현**
   - 8가지 상태: Idle, Walk, Run, Roll, Jump, Slash, Parry, Death
   - AnimationController 컴포넌트로 상태 관리
   - 인터럽트 가능/불가능 애니메이션 구분
   - 상태별 지속 시간 설정

3. **입력 시스템 개선**
   - 스페이스 키 충돌 해결 (커서 토글 → Tab)
   - Shift + 우클릭으로 패링
   - 카메라 전환 속도 1.5배 증가
   - block 키 InputMapping에 추가

4. **컴파일 에러 수정**
   - 튜플 크기 제한 문제 해결 (시스템 그룹화)
   - import 경로 수정
   - 플레이어 엔티티 컴포넌트 분리

5. **FBX → GLTF 변환 스크립트 생성**
   - Batch 버전 (convert_animations.bat)
   - PowerShell 버전 (convert_animations.ps1)
   - Python 버전 (convert_animations.py)

## 🚨 발견된 이슈

1. **Aurora.glb 애니메이션 미포함**
   - 로그: "Total animations: 0"
   - FBX 파일들은 별도로 존재 (assets/Animations/Aurora/)
   - GLTF 변환 필요

2. **fbx2gltf 미설치**
   - 변환 도구가 시스템에 설치되지 않음
   - `npm install -g fbx2gltf` 필요

## 📁 주요 파일 변경사항

### 수정된 파일
1. **src/systems/animation.rs**
   - FSM 기반 애니메이션 시스템 완전 재작성
   - update_animation_state: 상태 전환 로직
   - play_animations: 애니메이션 재생
   - load_aurora_animations: Aurora 애니메이션 로드

2. **src/components/player.rs**
   - AnimationState enum 확장 (8가지 상태)
   - AnimationController 컴포넌트 추가
   - AnimationClips 컴포넌트 추가

3. **src/systems/asset_loader.rs**
   - Vanguard → Aurora 변경
   - 캐릭터 오프셋 조정

4. **src/resources/input.rs**
   - block: KeyCode 추가 (ShiftLeft)

5. **src/main.rs**
   - 시스템 그룹화로 튜플 크기 문제 해결
   - 애니메이션 시스템 추가

### 새로 생성된 파일
1. **ANIMATION_MAPPING.md** - 애니메이션 파일 매핑 가이드
2. **FBX_TO_GLTF_GUIDE.md** - FBX 변환 가이드
3. **convert_animations.bat/ps1/py** - 변환 스크립트

## 🎮 현재 게임 상태

### 작동하는 기능
- Aurora 캐릭터 모델 표시
- WASD 이동 + Shift 달리기
- Space 회피 (스테미나 20 소모)
- 좌클릭 공격
- ESC 일시정지/재개
- Tab 커서 잠금/해제
- E 카메라 좌/우 전환

### 작동하지 않는 기능
- 애니메이션 재생 (Aurora.glb에 애니메이션 없음)
- Shift + 우클릭 패링 (애니메이션 없음)

## 📋 다음 세션 TODO

1. **fbx2gltf 설치**
   ```bash
   npm install -g fbx2gltf
   ```

2. **애니메이션 변환 실행**
   ```powershell
   .\convert_animations.ps1 -Binary
   ```

3. **애니메이션 경로 업데이트**
   ```rust
   // src/systems/animation.rs
   asset_server.load("Animations_GLTF/Aurora/Idle.glb#Animation0")
   ```

4. **애니메이션 통합 테스트**
   - 각 상태별 애니메이션 확인
   - 전환 부드러움 체크

## 💡 참고사항

- Aurora FBX 애니메이션: 163개 파일
- Great Sword 애니메이션: 9개 파일
- 추천: GLB 형식 사용 (더 작은 파일 크기)
- 애니메이션 이름이 #Animation0 형식일 수 있음

## 🔧 빠른 복구 명령어

```bash
# 게임 실행
cargo run

# 애니메이션 변환 (fbx2gltf 설치 후)
.\convert_animations.ps1 -Binary

# 디버그 로그 확인
cargo run 2>&1 | findstr "animation"
```

---
**마지막 작업**: FBX → GLTF 변환 스크립트 생성 완료
**다음 작업**: fbx2gltf 설치 후 애니메이션 변환 및 통합