#include "GsBlueprintFunctionLibrarySummon.h"

#include "ActorEx/GsActorSummonFairy.h"
#include "ActorEx/GsActorSummonSceneObject.h"
#include "ActorEx/GsActorSummonCostume.h"

#include "GameMode/GsGameModeWorld.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Summon/GsSummonHandler.h"
#include "GameFlow/GameContents/GsContentsMode.h"

#include "Management/ScopeGlobal/GsGameFlowManager.h"

#include "Management/ScopeGame/GsSummonManager.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "Fairy/GsFairyData.h"

#include "Shared/Client/SharedEnums/SharedFairyEnum.h"
#include "Shared/Client/SharedEnums/SharedCostumeEnum.h"

// 선택된 뽑기용 페어리
AGsActorSummonFairy* UGsBlueprintFunctionLibrarySummon::GetSelectSummonFairy()
{
	AGsActorSummonSceneObject* summonSceneObject =
		GSSummon()->GetSummonSceneObject();

	if (summonSceneObject == nullptr)
	{
		return nullptr;
	}

	return summonSceneObject->GetSelectHighGradeSummonFairy();
}
// 선택된 뽑기용 페어리 등급
int UGsBlueprintFunctionLibrarySummon::GetSelectSummonFairyGradeInt()
{
	AGsActorSummonSceneObject* summonSceneObject =
		GSSummon()->GetSummonSceneObject();

	if (summonSceneObject == nullptr)
	{
		return 0;
	}

	AGsActorSummonFairy* fairy = summonSceneObject->GetSelectHighGradeSummonFairy();
	if (fairy == nullptr)
	{
		return 0;
	}

	FairyGrade fairyGrade = fairy->GetGrade();
	return (int)fairyGrade;
}

// 선택된 뽑기용 코스튬 등급
int UGsBlueprintFunctionLibrarySummon::GetSelectSummonCostumeGradeInt()
{
	AGsActorSummonSceneObject* summonSceneObject =
		GSSummon()->GetSummonSceneObject();

	if (summonSceneObject == nullptr)
	{
		return 0;
	}

	AGsActorSummonCostume* costume = summonSceneObject->GetSelectHighGradeSummonCostume();
	if (costume == nullptr)
	{
		return 0;
	}

	CostumeGrade costumeGrade =costume->GetGrade();
	return (int)costumeGrade;
}
// 선택된 뽑기용 코스튬 id
int UGsBlueprintFunctionLibrarySummon::GetSelectSummonCostumeId()
{
	AGsActorSummonSceneObject* summonSceneObject =
		GSSummon()->GetSummonSceneObject();

	if (summonSceneObject == nullptr)
	{
		return 0;
	}

	return static_cast<int>(summonSceneObject->GetSelectHighGradeSummonCostumeId());
}

// 뽑기 연출 기본 코스튬 id
int UGsBlueprintFunctionLibrarySummon::GetBaseSummonCostumeId()
{
	return GData()->GetGlobalData()->_summonHighGradeSequenceBaseCostumeId;
}

// sequence fairy by index
AGsActorSummonFairy* UGsBlueprintFunctionLibrarySummon::GetSequenceSummonFairyByIndex(int In_index)
{
	return GSSummon()->GetSummonFairyByIndex(In_index);
}

// sequence costume id by index
int UGsBlueprintFunctionLibrarySummon::GetSequenceSummonCostumeIdByIndex(int In_index)
{
	return GSSummon()->GetCostumeIdByIndex(In_index);
}

bool UGsBlueprintFunctionLibrarySummon::GetSequenceSummonCostumeHighValByIndex(int In_index)
{
	return GSSummon()->GetCostumeHighValByIndex(In_index);
}

// sequence fairy high get
AGsActorSummonFairy* UGsBlueprintFunctionLibrarySummon::GetHighGetSummonFairy()
{
	int dataIndex = GSSummon()->GetCurrentHighGetDataIndex();
	return GSSummon()->GetSummonFairyByIndex(dataIndex, false);
}

// sequence costume high get
int UGsBlueprintFunctionLibrarySummon::GetHighGetSummonCostumeId()
{
	int dataIndex = GSSummon()->GetCurrentHighGetDataIndex();
	return GSSummon()->GetCostumeIdByIndex(dataIndex);
}

bool UGsBlueprintFunctionLibrarySummon::GetSequenceSummonFairyHighValByIndex(int In_index)
{
	return GSSummon()->GetFairyHighValByIndex(In_index);
}

bool UGsBlueprintFunctionLibrarySummon::GetSummonFairysHighCardExist()
{
	return GSSummon()->GetFairysHighCardExist();
}

bool UGsBlueprintFunctionLibrarySummon::GetSummonCostumesHighCardExist()
{
	return GSSummon()->GetCostumesHighCardExist();
}