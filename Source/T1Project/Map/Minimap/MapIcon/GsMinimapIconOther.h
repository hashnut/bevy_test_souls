#pragma once

#include "CoreMinimal.h"
#include "Map/Minimap/MapIcon/GsMinimapIconObject.h"
#include "Engine/Classes/Engine/EngineTypes.h"
#include "GsMinimapIconOther.generated.h"

/*
	나이외 아이콘 처리(remote, Nonplayer, party)
	1. 갱신 (타이머로 위치 갱신)
	2. 스폰 반경 안의 object(gameobject 클래스)
*/

UCLASS()
class T1PROJECT_API UGsMinimapIconOther : public UGsMinimapIconObject
{
	GENERATED_BODY()
private:
	// 타이머
	FTimerHandle _timerHandle;
	// 이전 위치
	FVector _oldObjPos = FVector::ZeroVector;
	// 타겟 obj 의 위치 변경
	bool _targetObjPosDirty = false;
	// 파티 원인지
	bool _isPartyMember = false;
public:
	virtual ~UGsMinimapIconOther() {}

	// 가상함수
public:
	// 초기화
	virtual void Initialize(UGsGameObjectBase* In_targetObj, 
		UGsUIIconBase* In_iconWidget,
		EGsMapIconType In_iconType, float In_updateTime, UGsMinimapSceneImage* In_sceneImage,
		int In_depth, int64 In_gameId) override;
	// 클리어
	virtual void Finalize() override;
	virtual void Tick(float In_delta) override;

	// 로직 함수
public:
	// 타이머에의한 업데이트
	void UpdateTimmer();

	// get, set
public:
	bool GetIsPartyMember()
	{
		return _isPartyMember;
	}
	void SetIsPartyMember(bool In_isPartyMember)
	{
		_isPartyMember = In_isPartyMember;
	}	
};