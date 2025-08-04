# FBX 애니메이션을 Bevy에서 사용하는 방법

Bevy는 FBX 파일을 직접 지원하지 않습니다. GLTF/GLB 형식만 지원합니다.

## 방법 1: Aurora.glb 내장 애니메이션 사용 (권장)

Aurora.glb 파일에 이미 애니메이션이 포함되어 있을 가능성이 높습니다.

```rust
// 게임 실행 시 콘솔에서 애니메이션 목록 확인
// "=== Aurora.glb Animations ===" 로그 찾기

// 인덱스 기반 접근
asset_server.load("models/Aurora.glb#Animation0")  // 첫 번째 애니메이션
asset_server.load("models/Aurora.glb#Animation1")  // 두 번째 애니메이션

// 이름 기반 접근 (애니메이션에 이름이 있는 경우)
asset_server.load("models/Aurora.glb#Idle")
asset_server.load("models/Aurora.glb#Walk")
```

## 방법 2: FBX를 GLB로 변환

### Blender 사용 (무료, 권장)

1. **Blender 설치**: https://www.blender.org/download/

2. **FBX 임포트**:
   - File > Import > FBX
   - Aurora 모델과 애니메이션 FBX 파일들 선택

3. **애니메이션 병합**:
   - Aurora 모델 선택
   - 각 애니메이션 FBX 파일을 하나씩 임포트
   - Action Editor에서 각 애니메이션에 이름 지정
     - "Idle", "Walk", "Run", "Roll", "Jump", "Slash", "Parry", "Death"

4. **GLB 익스포트**:
   - File > Export > glTF 2.0
   - Format: glTF Binary (.glb)
   - Include > Animation 체크
   - Animation > Export All Actions 체크

### FBX2glTF 도구 사용

1. **설치**:
   ```bash
   npm install -g fbx2gltf
   ```

2. **변환**:
   ```bash
   # 단일 파일 변환
   fbx2gltf Aurora.FBX -o Aurora_with_anims.glb
   
   # 여러 애니메이션 병합 (스크립트 필요)
   ```

## 방법 3: 애니메이션별 GLB 파일 생성

각 애니메이션을 별도의 GLB 파일로 변환:

```bash
fbx2gltf Idle.FBX -o idle.glb
fbx2gltf Jog_Fwd.FBX -o walk.glb
fbx2gltf Primary_Attack_A.FBX -o attack.glb
```

그런 다음 코드에서:

```rust
let clips = AnimationClips {
    idle: Some(asset_server.load("animations/idle.glb#Animation0")),
    walk: Some(asset_server.load("animations/walk.glb#Animation0")),
    slash: Some(asset_server.load("animations/attack.glb#Animation0")),
    // ...
};
```

## 방법 4: Great Sword 애니메이션 사용 (임시)

Great Sword 폴더의 FBX들이 이미 있다면:

```rust
// 이 애니메이션들도 먼저 GLB로 변환 필요
let clips = AnimationClips {
    idle: Some(asset_server.load("models/great_sword_idle.glb#Animation0")),
    walk: Some(asset_server.load("models/great_sword_walk.glb#Animation0")),
    run: Some(asset_server.load("models/great_sword_run.glb#Animation0")),
    slash: Some(asset_server.load("models/great_sword_slash.glb#Animation0")),
    // ...
};
```

## 권장 워크플로우

1. **먼저 Aurora.glb의 내장 애니메이션 확인**
   - `cargo run` 실행 후 콘솔 로그 확인
   - 사용 가능한 애니메이션 목록 파악

2. **내장 애니메이션이 없거나 부족한 경우**
   - Blender로 FBX 애니메이션들을 Aurora 모델에 병합
   - 하나의 Aurora_animated.glb 파일로 익스포트

3. **코드 수정**:
   ```rust
   // 실제 애니메이션 이름/인덱스로 변경
   idle: Some(asset_server.load("models/Aurora_animated.glb#Idle")),
   walk: Some(asset_server.load("models/Aurora_animated.glb#Walk")),
   // ...
   ```

## 주의사항

- FBX 파일은 Bevy에서 직접 로드 불가
- 애니메이션은 반드시 GLTF/GLB 형식이어야 함
- 본(Bone) 구조가 모델과 애니메이션 간에 일치해야 함
- 애니메이션 이름은 대소문자 구분