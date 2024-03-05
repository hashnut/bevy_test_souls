// Fill out your copyright notice in the Description page of Project Settings.


#include "GsItemConsumable.h"
#include "T1Project.h"
#include "DataSchema/Item/GsSchemaItemConsumeDetail.h"
#include "DataSchema/Stat/GsSchemaItemEffectRestoreStat.h"
#include "DataSchema/Item/GsSchemaItemEffectEnchantScroll.h"
#include "DataSchema/Item/GsSchemaItemEffectSkillBook.h"
#include "DataSchema/Reward/GsSchemaRewardData.h"
#include "GameObject/Abnormality/GsAbnormalityHandlerBase.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Skill/GsSkillHelper.h"
#include "Management/ScopeGame/GsInterServerManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"


FGsItemConsumable::FGsItemConsumable(const int32 In_ItemDetailID)
{
	const FGsSchemaItemConsumeDetail* consumDetailData = GetDetailConsumeTableData(In_ItemDetailID);
	if (nullptr == consumDetailData)
	{
		check(consumDetailData);
		GSLOG(Warning, TEXT("GetDetailConsumeTableData() is nullptr !!! "));
		return;
	}

	_consumDetailData = consumDetailData;

	switch (_consumDetailData->effectType)
	{
	case ItemEffectType::RESTORE_STAT:
		SetReStoreEffectDetailData(consumDetailData->effectIDList);
		break;
	case ItemEffectType::SKILL_BOOK:
		SetSkillBookEffectDetailData(consumDetailData->effectIDList);
		break;
	case ItemEffectType::GET_REWARD:
		SetRewardEffectDetailData(consumDetailData->effectIDList);
		break;
	}
}

void FGsItemConsumable::SetReStoreEffectDetailData(const TArray<int32>& InEffectIDList)
{
	_effectDetailDataBaseList.Empty();

	const UGsTableItemEffectRestoreStat* table = Cast<UGsTableItemEffectRestoreStat>(FGsSchemaItemEffectRestoreStat::GetStaticTable());
	if (nullptr == table)
	{
		return;
	}

	for (uint16 i = 0; i < InEffectIDList.Num(); ++i)
	{
		int32 id = InEffectIDList[i];
		const FGsSchemaItemEffectRestoreStat* row;
		if (false == table->FindRowById(id, row))
			continue;

		_effectDetailDataBaseList.Emplace(row);
	}
}

void FGsItemConsumable::SetSkillBookEffectDetailData(const TArray<int32>& InEffectIDList)
{
	_effectDetailDataBaseList.Empty();

	const UGsTableItemEffectSkillBook* table = Cast<UGsTableItemEffectSkillBook>(FGsSchemaItemEffectSkillBook::GetStaticTable());
	if (nullptr == table)
	{
		return;
	}

	for (uint16 i = 0; i < InEffectIDList.Num(); ++i)
	{
		const FGsSchemaItemEffectSkillBook* row = nullptr;
		if (table->FindRowById(InEffectIDList[i], row))
		{
			_effectDetailDataBaseList.Emplace(row);
		}
	}
}

void FGsItemConsumable::SetRewardEffectDetailData(const TArray<int32>& InEffectIDList)
{
	_effectDetailDataBaseList.Empty();

	const UGsTableRewardData* table = Cast<UGsTableRewardData>(FGsSchemaRewardData::GetStaticTable());
	if (nullptr == table)
	{
		return;
	}

	for (uint16 i = 0; i < InEffectIDList.Num(); ++i)
	{
		const FGsSchemaRewardData* row = nullptr;
		if (table->FindRowById(InEffectIDList[i], row))
		{
			_effectDetailDataBaseList.Emplace(row);
		}
	}
}

void FGsItemConsumable::UpdateBy(const FGsItem* In_Item)
{
	Super::UpdateBy(In_Item);

	int32 detailID = In_Item->GetDetailID();
	const FGsSchemaItemConsumeDetail* data = GetDetailConsumeTableData(detailID);
	if (nullptr != data)
	{
		_consumDetailData = data;
	}
}

const FGsSchemaItemConsumeDetail* FGsItemConsumable::GetDetailConsumeTableData(const int32 In_ID) const
{
	const UGsTableItemConsumeDetail* table = Cast<UGsTableItemConsumeDetail>(FGsSchemaItemConsumeDetail::GetStaticTable());
	if (nullptr == table)
	{
		return nullptr;
	}

	const FGsSchemaItemConsumeDetail* row = nullptr;
	if (false == table->FindRowById(In_ID, row))
	{
		return nullptr;
	}

	return row;
}

void FGsItemConsumable::FindReStoreStatData(OUT TArray<FGsItemRestoreStatData>& outDataList) const
{
	if (_effectDetailDataBaseList.Num() <= 0)
		return;

	for (int32 i = 0; i < _effectDetailDataBaseList.Num(); ++i)
	{
		FGsSchemaItemEffectRestoreStat* restoreStatData;
		restoreStatData = (FGsSchemaItemEffectRestoreStat*)_effectDetailDataBaseList[i];
		if (nullptr == restoreStatData)
			continue;

		FGsItemRestoreStatData itemRestoreStatData;
		itemRestoreStatData.type = restoreStatData->damageStatType;
		itemRestoreStatData.value= restoreStatData->value;
		outDataList.Emplace(itemRestoreStatData);
	}
}

bool FGsItemConsumable::IsOverlabUseBuffItem(class FGsAbnormalityHandlerBase* InhandlerBase)
{
	if (ItemEffectType::ADD_ABNORMALITY_STAT == _consumDetailData->effectType)
	{
		for (uint8 i = 0; i < _consumDetailData->effectIDList.Num(); ++i)
		{
			if (true == InhandlerBase->IsAbnormalityId(_consumDetailData->effectIDList[i]))
			{
				return true;
			}
		}
	}

	return false;
}

bool FGsItemConsumable::GetUserSkillSetIdList(OUT TArray<int32>& OutList) const
{
	if (ItemEffectType::SKILL_BOOK != _consumDetailData->effectType)
	{
		return false;
	}

	if (0 == _effectDetailDataBaseList.Num())
	{
		return false;
	}

	for (const FGsSchemaBase* iter : _effectDetailDataBaseList)
	{
		const FGsSchemaItemEffectSkillBook* skillBookEffect = static_cast<const FGsSchemaItemEffectSkillBook*>(iter);
		if (nullptr == skillBookEffect)
		{
			// 하나라도 없으면 false 리턴
			return false;
		}

		OutList.Emplace(skillBookEffect->userSkillSetId);
	}

	return true;
}

int32 FGsItemConsumable::GetPickOneTypeRewardID() const
{
	if (ItemEffectType::GET_REWARD != _consumDetailData->effectType)
	{
		return 0;
	}

	if (0 >= _effectDetailDataBaseList.Num())
	{
		return 0;
	}

	const FGsSchemaRewardData* rewardData =
		static_cast<const FGsSchemaRewardData*>(_effectDetailDataBaseList[0]);

	if (nullptr == rewardData)
	{
		return 0;
	}

	if (RewardType::PICK_ONE != rewardData->rewardType)
	{
		return 0;
	}

	return rewardData->id;
}

int32 FGsItemConsumable::GetRewardId() const
{
	if (ItemEffectType::GET_REWARD != _consumDetailData->effectType)
	{
		return 0;
	}

	if (0 >= _effectDetailDataBaseList.Num())
	{
		return 0;
	}

	const FGsSchemaRewardData* rewardData =
		static_cast<const FGsSchemaRewardData*>(_effectDetailDataBaseList[0]);

	if (nullptr == rewardData)
	{
		return 0;
	}

	return rewardData->id;
}

bool FGsItemConsumable::IsAllowedAutoUse() const
{
	return _consumDetailData->isAllowedAutoUse;
}

bool FGsItemConsumable::IsSpawnInvadePortal() const
{
	return ItemEffectType::SPAWN_INVADE_PORTAL == _consumDetailData->effectType;
}

bool FGsItemConsumable::IsLearnedSkillBookItem() const
{
	TArray<int32> userSkillSetIdList;
	if (false == GetUserSkillSetIdList(userSkillSetIdList))
	{
		return false;
	}

	// 하나라도 안배운게 있으면 false 리턴
	for (int32 i = 0; i < userSkillSetIdList.Num(); ++i)
	{
		if (false == FGsSkillHelper::IsLearnedSkill(userSkillSetIdList[i]))
		{
			return false;
		}
	}

	return true;
}

void FGsItemConsumable::FindEffectIdList(OUT TArray<int32>& outEffectIdList)
{
	outEffectIdList.Empty();
	if (nullptr == _consumDetailData)
		return;

	for (int32 i = 0; i < _consumDetailData->effectIDList.Num(); ++i)
	{
		outEffectIdList.Emplace(_consumDetailData->effectIDList[i]);
	}
}

ItemEffectType FGsItemConsumable::GetEffectType() const
{
	if (nullptr == _consumDetailData)
		return ItemEffectType::NONE;

	return _consumDetailData->effectType;
}

bool FGsItemConsumable::IsCooldownTimeItem() const
{
	if (nullptr == _consumDetailData)
		return false;

	return 0.0f < _consumDetailData->cooldownTime;
}

bool FGsItemConsumable::IsUseItem() const
{
	if (nullptr == _consumDetailData)
		return false;

	return true;
}

float FGsItemConsumable::GetMaxCoolTime() const
{
	if (nullptr == _consumDetailData)
		return 0.f;

	return _consumDetailData->cooldownTime;
}

int32 FGsItemConsumable::GetCooldownGroupID() const
{
	if (nullptr == _consumDetailData)
		return 0;

	return _consumDetailData->cooldownGroupId;
}

int32 FGsItemConsumable::GetTooltipDurationTime() const
{
	if (nullptr == _consumDetailData)
		return 0;

	int32 tableData = _consumDetailData->tooltipDurationTime;
	if (0 >= tableData)
	{
		return 0;
	}

	return (tableData / 10000) * 100;
}

bool FGsItemConsumable::IsItemUseLock(OUT CostumItemUseErrorType& errorNum) const
{	
	errorNum = CostumItemUseErrorType::NoError;

	if (nullptr == _consumDetailData)
		return false;

	if (_consumDetailData->effectType != ItemEffectType::SPAWN_INVADE_PORTAL)
	{
		return false;
	}

	//침공 포털 생성하는 일을 하는 consume item 일때
	if (auto interserverManager = GSInterServer())
	{
		int mapId = GLevel()->GetCurrentLevelId();

		if (interserverManager->IsValidAirRaidState() == false)
			errorNum = CostumItemUseErrorType::InvdeInvalidTime; 
		else if (interserverManager->IsValidInvasionMap(mapId) == false)
			errorNum = CostumItemUseErrorType::InvadeInvalidMap; 

		return errorNum != CostumItemUseErrorType::NoError;
	}
	return false;
}