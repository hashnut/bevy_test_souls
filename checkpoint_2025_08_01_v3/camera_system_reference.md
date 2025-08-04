# Third Person Camera 시스템 레퍼런스

## 🎮 조작법

### 마우스
- **이동**: 카메라 회전
- **휠**: 줌 인/아웃 (거리: 5.0 ~ 15.0)
- **우클릭**: 조준 모드 (홀드)
  - 캐릭터가 카메라 방향을 바라봄
  - 줌인 효과

### 키보드
- **E**: 좌/우 어깨 시점 전환
- **스페이스**: 커서 잠금/해제
- **WASD**: 캐릭터 이동 (기존)
- **좌클릭**: 공격 (기존)

## 📦 주요 컴포넌트

### ThirdPersonCamera
```rust
pub struct ThirdPersonCamera {
    pub aim_enabled: bool,              // 조준 기능 활성화
    pub aim_button: MouseButton,        // 조준 버튼 (우클릭)
    pub aim_speed: f32,                 // 조준 속도 (3.0)
    pub aim_zoom: f32,                  // 조준 줌 레벨 (0.7)
    pub cursor_lock_toggle_enabled: bool, // 커서 잠금 토글 가능
    pub cursor_lock_active: bool,       // 현재 커서 잠금 상태
    pub cursor_lock_key: KeyCode,       // 커서 잠금 키 (스페이스)
    pub sensitivity: Vec2,              // 마우스 민감도
    pub offset_enabled: bool,           // 어깨 너머 시점 활성화
    pub offset: Offset,                 // 카메라 오프셋
    pub offset_toggle_enabled: bool,    // 오프셋 토글 가능
    pub offset_toggle_key: KeyCode,     // 오프셋 토글 키 (E)
    pub zoom_enabled: bool,             // 줌 기능 활성화
    pub zoom: Zoom,                     // 줌 범위 설정
    pub zoom_sensitivity: f32,          // 줌 민감도
}
```

### ThirdPersonCameraTarget
```rust
#[derive(Component)]
pub struct ThirdPersonCameraTarget;  // 플레이어에 추가
```

## 🔧 시스템 함수

### 1. sync_player_camera
- **실행 시점**: PostUpdate
- **기능**: 카메라를 플레이어 위치에 동기화
- **특징**: TransformPropagate 전에 실행

### 2. orbit_mouse
- **실행 시점**: Update
- **기능**: 마우스로 카메라 회전
- **조건**: 커서가 잠긴 상태일 때만

### 3. zoom_mouse
- **실행 시점**: Update
- **기능**: 마우스 휠로 줌 조절
- **범위**: min ~ max (5.0 ~ 15.0)

### 4. aim
- **실행 시점**: Update
- **기능**: 조준 모드 처리
- **효과**: 줌인 + 캐릭터 회전

### 5. toggle_offset
- **실행 시점**: Update
- **기능**: 좌/우 어깨 시점 전환
- **키**: E

### 6. toggle_cursor
- **실행 시점**: Update
- **기능**: 커서 잠금/해제
- **키**: 스페이스

## 📝 사용 예시

### 플레이어 생성
```rust
commands.spawn((
    // ... 다른 컴포넌트들
    ThirdPersonCameraTarget,  // 필수!
));
```

### 카메라 생성
```rust
commands.spawn((
    Camera3d::default(),
    Transform::from_xyz(0.0, 8.0, 10.0).looking_at(Vec3::ZERO, Vec3::Y),
    ThirdPersonCamera {
        zoom: Zoom::new(5.0, 15.0),
        sensitivity: Vec2::new(0.5, 0.5),
        aim_enabled: true,
        offset_enabled: true,
        offset_toggle_enabled: true,
        ..default()
    },
));
```

## 🐛 트러블슈팅

1. **카메라가 플레이어를 따라가지 않음**
   - ThirdPersonCameraTarget이 플레이어에 있는지 확인
   - sync_player_camera가 PostUpdate에서 실행되는지 확인

2. **마우스가 작동하지 않음**
   - cursor_lock_active가 true인지 확인
   - 스페이스바로 커서 잠금 상태 확인

3. **줌이 작동하지 않음**
   - zoom_enabled가 true인지 확인
   - zoom.min과 zoom.max 값 확인