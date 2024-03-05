#pragma once

#include "CoreMinimal.h"

#include "Engine/Classes/Engine/EngineTypes.h"

#include "GsRegionMapIconBase.generated.h"

/*
  지역맵 아이콘 기본
*/
class UGsUIIconBase;

class UCanvasPanelSlot;

UCLASS()
class T1PROJECT_API UGsRegionMapIconBase : public UObject
{
	GENERATED_BODY()
	
protected:
	// 아이콘 리소스 참조
	UPROPERTY()
	UGsUIIconBase* _icon = nullptr;
	// 로컬 위치 변경되었는지
	bool _localPosDirty = false;
	// 타이머
	FTimerHandle _timerHandle;
	// 이전 위치
	FVector _oldObjPos = FVector::ZeroVector;
	// 타겟 obj 의 위치 변경
	bool _targetObjPosDirty = false;

	// 가상함수
public:
	// 정리
	virtual void Finalize();

	// 위치 구하기
	virtual FVector2D GetPos() { return FVector2D::ZeroVector; }
	// 타이머에의한 업데이트
	virtual void UpdateTimmer() {}

	virtual void Tick(float In_delta) {}
	// 로직 함수
public:
	// 위치 갱신
	void UpdatePos();
	// 아이콘 위치 갱신
	void SetIconPos(const FVector2D& In_pos);
	// 아이콘 세팅
	void SetIcon(UGsUIIconBase* In_icon, UCanvasPanelSlot* In_panelSlot, float In_depth);
	// 타이머 세팅
	void SetTimmer(float In_updateTime);
	

	// get, set
public:
	void SetLocalPosDirty(bool In_val) { _localPosDirty = In_val; }
	UGsUIIconBase* GetIcon() { return _icon; }

	void SetIconVisibility(bool In_isVisible);
};