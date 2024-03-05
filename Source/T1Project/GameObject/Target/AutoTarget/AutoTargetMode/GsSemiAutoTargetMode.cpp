// Fill out your copyright notice in the Description page of Project Settings.
#include "GsSemiAutoTargetMode.h"

#include "GameObject/Target/GsTargetHandlerLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

#include "Management/ScopeGame/GsGameObjectManager.h"

void FGsSemiAutoTargetMode::SearchTarget(OUT TArray<class UGsGameObjectBase*>& OutResult)
{
	SearchDataInitailize();

	FGsTargetHandlerLocalPlayer* targetHandler = _local->GetCastTarget<FGsTargetHandlerLocalPlayer>();
	UGsGameObjectBase* target = targetHandler->GetTarget();
	// 기존 타겟이 존재할 경우 유지
	if (target)
	{
		OutResult.Emplace(target);
		return;
	}

	// 아레나 모드 확인
	if (_isArena)
	{
		TArray<UGsGameObjectBase*> playerTargetlist = GSGameObject()->FindObjectArray(EGsGameObjectType::RemotePlayer);
		if (playerTargetlist.Num() > 0)
		{
			// 더이상 검사는 의미없다.
			OutResult.Append(playerTargetlist);
			return;
		}
	}

	// 2차 타겟도 확인
	target = targetHandler->GetSecondTarget();
	if (target)
	{
		OutResult.Emplace(target);
		return;
	}

	CallSearchFunction(OutResult);
}

void FGsSemiAutoTargetMode::SearchDataInitailize()
{
	Super::SearchDataInitailize();

	// FGsAutoTargetMode를 상속 받아서 _searchCenterPos2D는 다시 재정의 해준다.
	_searchCenterPos2D = _local->GetLocation2D();
}
