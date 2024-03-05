#include "GsUseItemEffectHandler.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/Data/GsGameObjectDataLocalPlayer.h"
#include "GameObject/UseItemEffect/SpiritShot/GsSpiritShotDefine.h"

#include "ActorEx/GsCharacterBase.h"

#include "ActorComponentEx/GsEffectComponent.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "DataSchema/Item/GsSchemaItemEquipDetail.h"
#include "DataSchema/Item/GsSchemaItemConsumeParticleRes.h"
#include "DataSchema/Sound/GsSchemaSoundResData.h"

#include "Item/GsItemManager.h"
#include "Item/GsItem.h"

#include "Management/GsMessageHolder.h"

#include "Message/GsMessageContents.h"

#include "UTIL/GsTableUtil.h"

#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Management/ScopeGame/GsSpiritShotmanager.h"
#include "Sound/GsSoundPlayer.h"

#include "Engine/Classes/Particles/ParticleSystem.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Quest/Management/GsQuestActionManagement.h"

const FName NameHitEffectSocket = TEXT("HitEffectSocket");
const FName NameBattleWeaponLSocket = TEXT("SOC_BattleWeapon_L");
const FName NameBattleWeaponRSocket = TEXT("SOC_BattleWeapon_R");

void FGsUseItemEffectHandler::Initialize(UGsGameObjectBase* In_owner)
{
	_owner = In_owner;
}

// 아이템 사용 hit 이펙트
void FGsUseItemEffectHandler::OnUseItemHitEffect(ItemCategorySub In_category, Level InSumLevel)
{
	if (_owner == nullptr)
	{
		return;
	}

	if (!IsShowEffectPlay())
		return;

	// 천마석(정탄)
	if (In_category == ItemCategorySub::SPIRIT_SHOT)
	{		
		UParticleSystem* findParticle = 
			FindSpiritShotEffect(true, InSumLevel);

		if (findParticle == nullptr)
		{
			return;
		}

		AGsCharacterBase* character = _owner->GetCharacter();

		if (character == nullptr)
		{
			return;
		}

		FGsSpiritShotManager* spiritShotMgr = GSSpiritShot();
		if (nullptr == spiritShotMgr)
		{
			GSLOG(Error, TEXT("nullptr == spiritShotMgr"));
			return;
		}

		float effectScale = spiritShotMgr->GetSpiritShotEffectScale(InSumLevel);

		if (UGsEffectComponent* effComp = character->GetEffectComponent())
		{
			effComp->OnEffectSocketPos(findParticle, NameHitEffectSocket, effectScale);
		}
		// 사운드 처리
		const FGsSchemaSoundResData* findSoundRes =
			FindSpiritShotSoundRes(true);

		if (findSoundRes != nullptr)
		{
			if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
			{
				soundPlayer->PlaySoundTarget(findSoundRes, character->GetRootComponent());
			}
		}
	}
}
// 카테고리별 이펙트 출력 처리
void FGsUseItemEffectHandler::OnUseItemCategory(ItemCategorySub In_category)
{
	if (!IsShowEffectPlay())
		return;

	const FGsSchemaItemConsumeParticleRes* findTbl =
		UGsTableUtil::FindRowById<UGsTableItemConsumeParticleRes, FGsSchemaItemConsumeParticleRes>(In_category);

	if (findTbl == nullptr)
	{
		return;
	}

	FSoftObjectPath softPath = findTbl->particleRes;

	if (softPath.IsValid() == false)
	{
		return;
	}

	UObject* load = UAssetManager::GetStreamableManager().LoadSynchronous(softPath);
	if (load == nullptr)
	{
		return;
	}

	UParticleSystem* particle = Cast<UParticleSystem>(load);

	if (particle == nullptr)
	{
		return;
	}
	AGsCharacterBase* character = _owner->GetCharacter();
	if (character == nullptr)
	{
		return;
	}

	if (UGsEffectComponent* effComp = character->GetEffectComponent())
	{
		effComp->OnEffect(particle);
	}
}

// 아이템 사용 이펙트
void FGsUseItemEffectHandler::OnUseItemEffect(ItemCategorySub In_category, CreatureWeaponType In_weaponType, Level InSumLevel)
{
	if (!IsShowEffectPlay())
		return;

	// 천마석(정탄)
	if (In_category == ItemCategorySub::SPIRIT_SHOT)
	{
		UParticleSystem* findParticle =
			FindSpiritShotEffect(false, InSumLevel);


		if (findParticle == nullptr)
		{
			return;
		}


		TArray<FName> arrSocketName;
		// 양손: 쌍검, 맨손
		// 왼손: 활
		// 오른손: 지팡이, 대검
		if (In_weaponType == CreatureWeaponType::BOW)
		{
			arrSocketName.Add(NameBattleWeaponLSocket);
		}
		else if (In_weaponType == CreatureWeaponType::WAND ||
			In_weaponType == CreatureWeaponType::SWORD ||
			In_weaponType == CreatureWeaponType::ORB  ||
			In_weaponType == CreatureWeaponType::SHOTGUN)
		{
			arrSocketName.Add(NameBattleWeaponRSocket);
		}
		else if(In_weaponType == CreatureWeaponType::DAGGER ||
			In_weaponType == CreatureWeaponType::HAND)
		{
			arrSocketName.Add(NameBattleWeaponLSocket);
			arrSocketName.Add(NameBattleWeaponRSocket);
		}

		AGsCharacterBase* character = _owner->GetCharacter();
		if (character == nullptr)
		{
			return;
		}

		FGsSpiritShotManager* spiritShotMgr = GSSpiritShot();
		if (nullptr == spiritShotMgr)
		{
			GSLOG(Error, TEXT("nullptr == spiritShotMgr"));
			return;
		}

		float effectScale = spiritShotMgr->GetSpiritShotEffectScale(InSumLevel);

		if (UGsEffectComponent* effComp = character->GetEffectComponent())
		{
			for (FName& socketName: arrSocketName)
			{
				effComp->OnEffectSocketPos(findParticle, socketName, effectScale);
			}
		}

		// 사운드 처리
		const FGsSchemaSoundResData* findSoundRes =
			FindSpiritShotSoundRes(false);
		
		if (findSoundRes != nullptr)
		{
			if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
			{
				soundPlayer->PlaySoundTarget(findSoundRes, character->GetRootComponent());
			}
		}
	}
}



// 현재 장착 무기 아이템 천마석(정령탄) 이펙트 테이블 접근
UParticleSystem* FGsUseItemEffectHandler::FindSpiritShotEffect(
	bool In_isHit, Level InSumLevel)
{
	// 옛날에는 GlobalConstant 에 천마석 파티클 경로를 넣어 놓고 조회했다. 여름이었다.
	//FSoftObjectPath softPath =
	//	(In_isHit == true) ? GData()->GetGlobalData()->_spiritShotHitEffect :
	//	GData()->GetGlobalData()->_spiritShotUseEffect;


	// [U2] | ejrrb10 | 천마석 잠식 레벨에 따라 천마석 이펙트가 달라진다 
	FGsSpiritShotManager* spiritShotMgr = GSSpiritShot();
	if (nullptr == spiritShotMgr)
	{
		GSLOG(Error, TEXT("nullptr == spiritShotMgr"));
		return nullptr;
	}

	// 대상이 맞을 때(Hit 될 때)만 천마석 효과가 나와야 한다고 함
	FSoftObjectPath softPath;
	if (In_isHit)
	{
		softPath = spiritShotMgr->GetSpiritShotEffectPath(InSumLevel);
	}
	else
	{
		return nullptr;
	}


	if (softPath.IsValid() == false)
	{
		return nullptr;
	}

	UObject* load = UAssetManager::GetStreamableManager().LoadSynchronous(softPath);
	if (load == nullptr)
	{
		return nullptr;
	}

	return Cast<UParticleSystem>(load);
}
// 천마석(정령탄) 사운드 테이블 접근(아이템 등급에 따라)
const FGsSchemaSoundResData* FGsUseItemEffectHandler::FindSpiritShotSoundRes(bool In_isHit)
{
	const FGsSchemaSoundResData* schemaSoundResData = (In_isHit == true) ?
		GData()->GetGlobalData()->_spiritShotHitSound.GetRow() :
		GData()->GetGlobalData()->_spiritShotUseSound.GetRow();

	return schemaSoundResData;
}


bool FGsUseItemEffectHandler::IsShowEffectPlay()
{
	// 대사 연출 중이나 시네마틱 중이면 이펙트 출력 안한다
	if (UGsQuestManager* questManager = GSQuest())
	{
		if (FGsQuestActionManagement* action = questManager->GetQuestActionManagement())
		{
			return (action->IsActioning()) ? false : true;
		}
	}

	return true;
}