# 빠른 참조 가이드

## 🚀 프로젝트 실행
```bash
cd C:\Users\USER\Documents\Personal\Bevy_Projects\bevy_test_souls
cargo run
```

## 🎮 게임 조작법
- **WASD**: 캐릭터 이동
- **마우스**: 카메라 회전
- **좌클릭**: 공격
- **스페이스**: 회피 (스테미나 20 소모)
- **Shift**: 방어 (미구현)

## 📁 주요 파일 위치
```
src/
├── systems/
│   ├── map_generator.rs     # 맵 생성
│   ├── asset_loader.rs      # 모델 로딩
│   ├── camera.rs           # 카메라 컨트롤
│   └── player_movement.rs  # 플레이어 이동
├── components/
│   ├── player.rs           # 플레이어 컴포넌트
│   └── map.rs             # 맵 컴포넌트
└── main.rs                # 엔트리 포인트

assets/
├── models/
│   ├── Vanguard.glb       # 현재 플레이어 모델
│   └── sword.glb          # 무기 모델
└── maps/
    └── generate_map_in_blender.py  # Blender 스크립트
```

## 🔧 주요 설정값

### 카메라 설정
- **각도**: 30도 (0.52 라디안)
- **거리**: 8.0
- **민감도**: 2.0

### 플레이어 설정
- **Y 위치**: 0.5 (지면 근처)
- **이동 속도**: 5.0
- **체력**: 100
- **스테미나**: 100

### 맵 설정
- **맵 크기**: 30x30
- **타일 크기**: 5x5
- **그리드**: 6x6
- **벽 높이**: 4.0

## 🐛 현재 이슈
1. **캐릭터 가시성**: Vanguard 모델이 화면에 표시되지 않을 수 있음
2. **경고**: 많은 unused 경고 (cargo fix로 정리 가능)

## 💡 다음 작업 제안
1. 캐릭터 모델 가시성 확인
2. 애니메이션 시스템 구현
3. 회피 시스템 완성
4. 체크포인트 상호작용

## 🛠️ 유용한 명령어
```bash
# 경고 수정
cargo fix --bin "dark-rust"

# 릴리즈 빌드
cargo build --release

# Blender에서 맵 생성
# 1. Blender 실행
# 2. Script Editor에서 generate_map_in_blender.py 실행
# 3. dark_rust_map.glb를 assets/models/로 이동
```

---
**마지막 업데이트**: 2025-08-01