# 빠른 복구 가이드

## 1. 즉시 실행 가능한 상태로 복구

```bash
# 프로젝트 디렉토리로 이동
cd C:\Users\USER\Documents\Personal\Bevy_Projects\bevy_test_souls

# 게임 실행 (애니메이션 없이)
cargo run
```

## 2. 애니메이션 시스템 활성화

### Step 1: fbx2gltf 설치
```bash
# Node.js가 설치되어 있어야 함
npm install -g fbx2gltf

# 또는 직접 다운로드
# https://github.com/facebookincubator/FBX2glTF/releases
```

### Step 2: 애니메이션 변환
```powershell
# PowerShell에서 실행
.\convert_animations.ps1 -Binary

# 또는 Python으로
python convert_animations.py --binary
```

### Step 3: 코드 수정 (필요시)
```rust
// src/systems/animation.rs 195-203 라인 수정
let clips = AnimationClips {
    idle: Some(asset_server.load("Animations_GLTF/Aurora/Idle.glb#Animation0")),
    walk: Some(asset_server.load("Animations_GLTF/Aurora/Jog_Fwd_Combat.glb#Animation0")),
    run: Some(asset_server.load("Animations_GLTF/Aurora/Jog_Fwd.glb#Animation0")),
    roll: Some(asset_server.load("Animations_GLTF/Aurora/Ability_RMB_Fwd.glb#Animation0")),
    jump: Some(asset_server.load("Animations_GLTF/Aurora/Jump_Start.glb#Animation0")),
    slash: Some(asset_server.load("Animations_GLTF/Aurora/Primary_Attack_A.glb#Animation0")),
    parry: Some(asset_server.load("Animations_GLTF/Aurora/HitReact_Front.glb#Animation0")),
    death: Some(asset_server.load("Animations_GLTF/Aurora/Death.glb#Animation0")),
};
```

## 3. 현재 작동하는 기능 확인

- **이동**: WASD
- **달리기**: Shift + WASD (스테미나 소모)
- **회피**: Space (스테미나 20)
- **공격**: 좌클릭
- **카메라**: 마우스 회전, 휠 줌, E 시점 전환
- **일시정지**: ESC
- **커서**: Tab

## 4. 문제 해결

### "애니메이션이 안 보여요"
1. 콘솔에서 "Total animations: 0" 확인
2. Aurora.glb에 애니메이션이 없음
3. FBX 파일들을 변환해야 함

### "fbx2gltf가 없다고 나와요"
1. Node.js 설치 확인
2. `npm install -g fbx2gltf` 실행
3. 시스템 PATH에 추가

### "변환된 파일이 어디 있나요?"
- `assets/Animations_GLTF/Aurora/`
- `assets/Animations_GLTF/Great_Sword/`

## 5. 테스트 명령어

```bash
# 애니메이션 로그만 보기
cargo run 2>&1 | findstr /i "animation"

# Aurora.glb 정보 확인
cargo run 2>&1 | findstr /i "aurora"

# 전체 디버그 모드
RUST_LOG=debug cargo run
```