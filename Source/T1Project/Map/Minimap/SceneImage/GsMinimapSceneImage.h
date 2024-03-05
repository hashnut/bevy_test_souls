#pragma once

#include "CoreMinimal.h"
#include "Message/GsMessageContentHud.h"
#include "GsMinimapSceneImage.generated.h"
/*
	미니맵 씬 이미지 처리
	1. 지도 로딩
	2. 아이콘 위치 계산
*/

class UCanvasPanel;
class UUserWidget;
class UCanvasPanelSlot;

class UGsMinimapIconLocal;
class UGsWidgetPoolManager;

UCLASS()
class T1PROJECT_API UGsMinimapSceneImage: public UObject
{
	GENERATED_BODY()
	
	// 멤버 변수
private:
	// 씬 이미지 루트(내위치 중심으로 놓게 하기위한 것)
	// 내 아이콘 위치 만큼 빼면 중심으로 간다
	UPROPERTY()
	UCanvasPanel* _sceneImageRoot;
	// 위젯 보이는 부분 넓이 ,높이 반 거리
	// _sceneImageRoot 캔버스의 넓이, 높이 반을 쓰자
	// local ui 위치 뺀거에 더해서 local icon을 중심에 놓는다
	FVector2D _widgetViewHalfDist;	
	// ui rate 값(ui 길이로 만드는 값)
	// _widgetViewHalfDist.y / (_viewGameObjectDistanceRadius* 2)
	// 이미지가 있으면 이미지 크기/ 실제 씬 크기 이렇게 가야 할것 같다....
	float _uiRadarRate;
	// 이건 실제 이미지/ 실제 씬 사이즈로 비율은 낸거다
	float _uiSceneImageRate;
	// 위치 참조용 로컬 아이콘
	UPROPERTY()
	UGsMinimapIconLocal* _iconLocal;
	// 로드한 지도 이미지
	UPROPERTY()
	UUserWidget* _loadedSceneImage;
	// 지도 이미지 attach 시 나온 슬롯
	UPROPERTY()
	UCanvasPanelSlot* _loadedSceneImageSlot;
	// 씬 이미지 이동
	MsgMinimapHandle _delegateSetSceneImagePos;
	// 씬 이미지 풀매니저
	TWeakObjectPtr<UGsWidgetPoolManager> _sceneImagePoolManager;
	// 레이더 모드면 씬 이미지 없는것
	// 아니면 씬 이미지 있다는것
	bool _isRadarMode = false;

	// size 초기화 했는가
	bool _isInitSize = false;

	// 생성자, 소멸자
public:
	virtual ~UGsMinimapSceneImage();

	// 로직 함수
public:	
	void Initialize();
	void Finalize();
	void Tick(float In_delta);
	// 이미지 위치 변경
	void SetImagePos(const FVector2D& In_pos);
	// 씬이미지 로드
	void LoadSceneImage();

	// 맵 이미지 위젯 만들기
	UUserWidget* MakeSceneImageWidget(const FSoftObjectPath& In_path);
	UUserWidget* Claim(UClass* InClass);
	// 이미지 해제
	void ReleaseImage(UUserWidget* In_image);
	// 사이즈 초기화
	void InitSize();

	// 이벤트
public:
	void OnSetImagePos(const FVector2D& In_pos);

	// get, set
public:
	// scene image root 세팅한다
	// 1. In_sceneImageRoot: 이미지 로드할 캔버스
	// 2. In_sizePanel: 사이즈 캔버스
	void SetSceneImageRoot(UCanvasPanel* In_sceneImageRoot, UCanvasPanel* In_sizePanel);
	// obj 위치에서 ui 위치 구하기
	FVector2D GetUIPos(const FVector2D& In_objPos);
	void SetIconLocal(UGsMinimapIconLocal* In_icon) { _iconLocal = In_icon; }
	float GetUIRate();
	// 보여지는 view 밖에 있는지 체크
	bool IsOutSideFromFrame(const FVector2D& In_objPos);
	// 안쪽으로 위치 계산
	FVector2D CalcInsideViewPos(const FVector2D& In_objPos);
	// 로컬로부터 각도
	float GetTargetAngleFromLocal(const FVector2D& In_objPos);

	bool GetIsInitSize()
	{
		return _isInitSize;
	}
};