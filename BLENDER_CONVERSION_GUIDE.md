# Blender를 사용한 FBX → GLTF/GLB 변환 가이드

## 사전 준비사항

### 1. Blender 설치
- [Blender 공식 웹사이트](https://www.blender.org/download/)에서 최신 버전 다운로드
- 권장 버전: 3.6 LTS 이상 또는 4.0+
- 설치 시 "Add Blender to PATH" 옵션 선택 권장

### 2. Blender 설치 확인
```powershell
# PowerShell에서 실행
blender --version
```

## 변환 방법

### 방법 1: PowerShell 스크립트 사용 (권장)
```powershell
# 기본 사용법 (GLB 형식으로 변환)
.\convert_with_blender.ps1

# GLTF 형식으로 변환
.\convert_with_blender.ps1 -Format gltf

# 이미 존재하는 파일 건너뛰기
.\convert_with_blender.ps1 -SkipExisting

# Blender 경로 직접 지정
.\convert_with_blender.ps1 -BlenderPath "C:\Program Files\Blender Foundation\Blender 4.0\blender.exe"
```

### 방법 2: 배치 파일 사용
```batch
convert_with_blender.bat
```

### 방법 3: Python 스크립트 직접 실행
```bash
# Blender가 PATH에 있는 경우
blender --background --python blender_fbx_to_gltf.py -- --input_dir assets/Animations --output_dir assets/Animations_GLTF --format glb

# Blender 전체 경로 사용
"C:\Program Files\Blender Foundation\Blender 4.0\blender.exe" --background --python blender_fbx_to_gltf.py -- --input_dir assets/Animations --output_dir assets/Animations_GLTF --format glb
```

## 스크립트 옵션

- `--input_dir`: FBX 파일이 있는 디렉토리 (기본값: assets/Animations)
- `--output_dir`: GLTF/GLB 파일을 저장할 디렉토리 (기본값: assets/Animations_GLTF)
- `--format`: 출력 형식 - 'glb' 또는 'gltf' (기본값: glb)
- `--skip_existing`: 이미 존재하는 파일 건너뛰기

## GLB vs GLTF 형식

### GLB (권장)
- 바이너리 형식 (단일 파일)
- 더 작은 파일 크기
- 더 빠른 로딩
- Bevy에서 권장

### GLTF
- JSON + 바이너리 (여러 파일)
- 사람이 읽을 수 있는 형식
- 디버깅에 유용

## 변환 후 확인사항

1. **출력 디렉토리 확인**
   ```
   assets/Animations_GLTF/
   ├── Aurora/
   │   ├── Idle.glb
   │   ├── Jog_Fwd.glb
   │   ├── Primary_Attack_A.glb
   │   └── ...
   └── Great_Sword/
       ├── Great Sword Idle.glb
       └── ...
   ```

2. **로그 파일 확인**
   - `assets/Animations_GLTF/conversion_log.txt`

3. **애니메이션 매핑 파일**
   - `assets/Animations_GLTF/animation_mapping.json`

## 문제 해결

### Blender를 찾을 수 없음
1. Blender가 설치되어 있는지 확인
2. PowerShell 스크립트에서 `-BlenderPath` 옵션으로 경로 지정
3. 환경 변수 PATH에 Blender 경로 추가

### 변환 실패
1. FBX 파일이 손상되지 않았는지 확인
2. Blender에서 직접 FBX 파일 열어보기
3. `conversion_log.txt` 파일에서 에러 메시지 확인

### 애니메이션이 포함되지 않음
1. FBX 파일에 애니메이션이 있는지 확인
2. Blender에서 Timeline/Dope Sheet 창으로 확인

## Rust 코드 업데이트

변환 후 `src/systems/animation.rs`에서 경로 업데이트:

```rust
// 기존
asset_server.load("models/Aurora.glb#Animation0")

// 변경 후
asset_server.load("Animations_GLTF/Aurora/Idle.glb#Animation0")
```

## 참고사항

- Blender는 백그라운드 모드로 실행되므로 UI가 표시되지 않습니다
- 변환 중 콘솔에 진행 상황이 표시됩니다
- 대량의 파일 변환 시 시간이 걸릴 수 있습니다 (파일당 약 1-3초)