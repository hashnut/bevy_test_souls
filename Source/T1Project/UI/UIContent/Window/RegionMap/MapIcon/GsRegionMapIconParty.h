#pragma once
#include "CoreMinimal.h"

#include "UI/UIContent/Window/RegionMap/MapIcon/GsRegionMapIconBase.h"

#include "Engine/Classes/Engine/EngineTypes.h"
#include "SlateCore/Public/Layout/Geometry.h"

#include "GsRegionMapIconParty.generated.h"

/*
	지역맵 파티 아이콘 처리
*/
class FGsPartyMemberInfo;

UCLASS()
class T1PROJECT_API UGsRegionMapIconParty : public UGsRegionMapIconBase
{
	GENERATED_BODY()

private:
	// 멤버 정보
	FGsPartyMemberInfo* _memberInfo;


	// 가상 함수
public:
	virtual void Tick(float In_delta) override;
	// 타이머에의한 업데이트
	virtual void UpdateTimmer() override;
	virtual FVector2D GetPos() override;

	// get, set
public:
	void SetPartyMemberInfo(FGsPartyMemberInfo* In_info)
	{
		_memberInfo = In_info;
	}
};