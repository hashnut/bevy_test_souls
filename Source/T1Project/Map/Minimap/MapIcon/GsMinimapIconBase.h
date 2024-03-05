#pragma once
#include "CoreMinimal.h"
#include "Map/GsMapIconType.h"
#include "GsMinimapIconBase.generated.h"

/*
	미니맵 아이콘 기본
	고정형 아이콘이 추가 되어서
	제일 기본적인 base 클래스 추가됨
*/
class UGsUIIconBase;
class UGsMinimapSceneImage;
class UCanvasPanelSlot;

UCLASS()
class T1PROJECT_API UGsMinimapIconBase : public UObject
{
	GENERATED_BODY()
	
protected:
	// 아이콘 리소스 참조
	UPROPERTY()
	UGsUIIconBase* _icon = nullptr;
	// 위치 계산 참조
	UPROPERTY()
	UGsMinimapSceneImage* _sceneImage;
	// 위치 이동용 
	UPROPERTY()
	UCanvasPanelSlot* _canvasPanelSlot;

	// 아이콘 타입
	EGsMapIconType _iconType;
	// 아이콘 뎁스
	int _iconDepth;
	// game id
	int64 _gameId;
	// 로컬 위치 변경되었는지
	bool _localPosDirty = false;

	// 생성자, 소멸자
public:
	virtual ~UGsMinimapIconBase();

	// 가상함수
public:
	// 클리어
	virtual void Finalize();
	// 위치 구하기
	virtual FVector2D GetPos() { return FVector2D::ZeroVector; }
	virtual void Tick(float In_delta) {}
	// 테이블 아이디 구하기
	virtual int GetTableId() { return -1; }
	// ui 위치 구하기
	virtual FVector2D GetUIPos(const FVector2D& In_objectPos);
	// 아이콘 위치 세팅
	virtual void SetIconPos(FVector2D In_objectPos);

	// 로직 함수
public:
	// 위치 갱신
	void UpdatePos();
	// 해제
	void Release();
	
	// get, set
public:
	void SetCanvasPanelSlot(UCanvasPanelSlot* In_slot);

	UGsUIIconBase* GetIcon() { return _icon; }
	void SetIcon(UGsUIIconBase* In_icon) { _icon = In_icon; }

	EGsMapIconType GetIconType() { return _iconType; }
	void SetIconType(EGsMapIconType In_type) { _iconType = In_type; }

	int64 GetGameId() { return _gameId; }

	void SetLocalPosDirty(bool In_val) { _localPosDirty = In_val; }

	// 2022/09/30 PKT - visiblity
	void SetIconHidden(bool InIsHidden);

	bool IsIconHidden() const;
};