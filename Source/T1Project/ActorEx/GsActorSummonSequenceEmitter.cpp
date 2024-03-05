#include "GsActorSummonSequenceEmitter.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Management/ScopeGame/GsSummonManager.h"

#include "GameFlow/GameContents/ContentsGAme/Hud/Summon/GsSummonHandler.h"

#include "Engine/AssetManager.h"
#include "Engine/Classes/Particles/ParticleSystem.h"

void AGsActorSummonSequenceEmitter::ApplyParticle(bool In_isHighCardExist, bool In_isFairySummon)
{
	FSoftObjectPath path;

	if(In_isFairySummon == true)
	{ 
		path =
			In_isHighCardExist ?
			GData()->GetGlobalData()->_effectHighStartSequenceSummonFairy :
			GData()->GetGlobalData()->_effectNormalStartSequenceSummonFairy;
	}
	else
	{
		path =
			In_isHighCardExist ?
			GData()->GetGlobalData()->_effectHighStartSequenceSummonCostume :
			GData()->GetGlobalData()->_effectNormalStartSequenceSummonCostume;
	}

	UObject* load = UAssetManager::GetStreamableManager().LoadSynchronous(path);
	if (load == nullptr)
	{
		return;
	}

	UParticleSystem* particle = Cast<UParticleSystem>(load);

	if (particle == nullptr)
	{
		return;
	}

	SetTemplate(particle);

	// 페어리 이펙트 사운드 처리
	if (In_isFairySummon == true)
	{
		FGsSummonHandler* summonHandler = UGsSummonManager::GetSummonHandler();
		if (summonHandler == nullptr)
		{
			return;
		}
		summonHandler->PlayFairyEffectSound(true, In_isHighCardExist);
	}
	
}

void AGsActorSummonSequenceEmitter::ApplyCostumeParticle(bool In_isHighCardExist, int In_effectNumber)
{
	FSoftObjectPath path;

	if (In_effectNumber == 0)
	{
		path =
			In_isHighCardExist ?
			GData()->GetGlobalData()->_effectHighStartSequenceSummonCostume_0 :
			GData()->GetGlobalData()->_effectNormalStartSequenceSummonCostume_0;
	}
	else if(In_effectNumber == 1)
	{
		path =
			In_isHighCardExist ?
			GData()->GetGlobalData()->_effectHighStartSequenceSummonCostume_1 :
			GData()->GetGlobalData()->_effectNormalStartSequenceSummonCostume_1;
	}

	UObject* load = UAssetManager::GetStreamableManager().LoadSynchronous(path);
	if (load == nullptr)
	{
		return;
	}

	UParticleSystem* particle = Cast<UParticleSystem>(load);

	if (particle == nullptr)
	{
		return;
	}

	SetTemplate(particle);
}

// 날라가는 이펙트(일반, 고등급)
void AGsActorSummonSequenceEmitter::ApplyFairyMoveLightParticle(bool In_isHighCardExist)
{
	FSoftObjectPath path =
			In_isHighCardExist ?
			GData()->GetGlobalData()->_effectHighMoveLightSequenceSummonFairy :
			GData()->GetGlobalData()->_effectNormalMoveLightSequenceSummonFairy;
	

	UObject* load = UAssetManager::GetStreamableManager().LoadSynchronous(path);
	if (load == nullptr)
	{
		return;
	}

	UParticleSystem* particle = Cast<UParticleSystem>(load);

	if (particle == nullptr)
	{
		return;
	}

	SetTemplate(particle);
}

// 테미르 쾅에서 등급 결정 이펙트 출력
void AGsActorSummonSequenceEmitter::ApplyHighGetPreGradeParticle()
{
	int highGetPreGradeEffectIndex = GSSummon()->GetCurrentHighGetPreEffectIndex();

	FSoftObjectPath path;
	switch (highGetPreGradeEffectIndex)
	{
	case 0:
	{
		path = GData()->GetGlobalData()->_effectHighPreGradeEpic;
		break;
	}
	case 1:
	{
		path = GData()->GetGlobalData()->_effectHighPreGradeUnique;
		break;
	}
	case 2:
	{
		path = GData()->GetGlobalData()->_effectHighPreGradeLegend;
		break;
	}
	}
	UObject* load = UAssetManager::GetStreamableManager().LoadSynchronous(path);
	if (load == nullptr)
	{
		return;
	}

	UParticleSystem* particle = Cast<UParticleSystem>(load);

	if (particle == nullptr)
	{
		return;
	}

	SetTemplate(particle);

}