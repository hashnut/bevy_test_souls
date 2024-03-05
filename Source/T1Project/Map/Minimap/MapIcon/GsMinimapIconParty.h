#pragma once

#include "CoreMinimal.h"

#include "Map/Minimap/MapIcon/GsMinimapIconBase.h"
#include "Map/GsMapIconType.h"

#include "Engine/Classes/Engine/EngineTypes.h"

#include "GsMinimapIconParty.generated.h"

/*
	파티 아이콘 처리
	1. 스폰 반경 밖의 파티원(반경 안안은 UGsMinimapIconOther로 간다)
*/
class UUserWidget;

class UGsUIIconBase;
class UGsMinimapSceneImage;

class FGsPartyMemberInfo;

UCLASS()
class T1PROJECT_API UGsMinimapIconParty : public UGsMinimapIconBase
{
	GENERATED_BODY()

protected:
	// 방향 표시
	UPROPERTY()
	UUserWidget* _widgetArrow;
private:
	// 멤버 정보
	FGsPartyMemberInfo* _memberInfo;

	// 타이머
	FTimerHandle _timerHandle;
	// 이전 위치
	FVector _oldObjPos = FVector::ZeroVector;
	// 타겟 obj 의 위치 변경
	bool _targetObjPosDirty = false;
	// view 밖인지
	bool _isViewOutside = false;
	// 같은 채널인지
	bool _isSameChannel = false;


	// 가상 함수
public:
	// 위치 구하기
	virtual FVector2D GetPos() override;
	// 클리어
	virtual void Finalize() override;
	virtual void Tick(float In_delta) override;
	// ui 위치 구하기
	virtual FVector2D GetUIPos(const FVector2D& In_objectPos) override;

	// 로직 함수
public:
	void Initialize(
		FGsPartyMemberInfo* In_partyMemberInfo,
		UGsUIIconBase* In_iconWidget,
		EGsMapIconType In_iconType,
		float In_updateTime,
		UGsMinimapSceneImage* In_sceneImage,
		int In_depth, int64 In_gameId,
		UUserWidget* In_arrowWidget);

	// 타이머에의한 업데이트
	void UpdateTimmer();
	// 방향 표시 회전 갱신
	void UpdateArrowRotation(const FVector2D& In_objectPos);
	// 회전, 위치 갱신
	void UpdateObjectTransform();
};