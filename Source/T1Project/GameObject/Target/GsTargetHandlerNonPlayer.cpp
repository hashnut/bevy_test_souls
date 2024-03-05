// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTargetHandlerNonPlayer.h"
#include "Data/GsDataContainManager.h"

#include "ActorEx/GsCharacterLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/ObjectClass/GsGameObjectNonPlayer.h"
#include "GameObject/Stat/GsStatBase.h"
#include "Animation/GsAnimInstanceState.h"

#include "AI/Function/GsAIFunc.h"

void FGsTargetHandlerNonPlayer::Initialize(UGsGameObjectBase* owner)
{
	Super::Initialize(owner);

	// NonPlayer 는 NonPlayer StatData에 별도 Data 있음
	//ClearRange = GData()->GetGlobalData()->LocalPlayerTargetclearRange;
}

void FGsTargetHandlerNonPlayer::Finalize()
{
	Super::Finalize();
}

bool FGsTargetHandlerNonPlayer::SetTarget(UGsGameObjectBase* target, bool isRotate, SelectTargetFilter selectFilter)
{
	if (false == Super::SetTarget(target, isRotate, selectFilter))
	{
		return false;
	}
	// 타겟 들어오면 전투상태

	// super 보다 먼저 처리하면
	// super 내에서 clear 호출 때문에 날라감...
	if (nullptr != _owner)
	{
		if (UGsGameObjectCreature* nonPlayer = _owner->CastGameObject<UGsGameObjectCreature>())
		{
			nonPlayer->ChangeIdleType(EGsAnimStateIdleType::Battle);
		}
	}

	//GSLOG(Log, TEXT("FGsTargetHandlerNonPlayer::SetTarget"));
	return  true;
}

void FGsTargetHandlerNonPlayer::ClearTarget(bool isAllClear)
{
	//GSLOG(Log, TEXT("FGsTargetHandlerNonPlayer::ClearTarget"));

	Super::ClearTarget(isAllClear);

	// 타겟이 없으면 평화상태
	if (nullptr != _owner)
	{
		if (UGsGameObjectCreature* nonPlayer = _owner->CastGameObject<UGsGameObjectCreature>())
		{
			nonPlayer->ChangeIdleType(EGsAnimStateIdleType::Normal);
		}
	}
}