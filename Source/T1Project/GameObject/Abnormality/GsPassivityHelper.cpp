// Fill out your copyright notice in the Description page of Project Settings.

#include "GsPassivityHelper.h"

#include "DataSchema/Abnormality/GsSchemaAbnormalitySet.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivityEffectInvokeAbnormalitySet.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "DataSchema/XpBuff/GsSchemaArcas.h"
#include "DataSchema/XpBuff/GsSchemaArcasConfig.h"
#include "DataSchema/Item/GsSchemaItemConsumeDetail.h"

#include "Management/GsScopeGame.h"
#include "Management/GsScopeHolder.h"
#include "Management/ScopeGame/GsFairyManager.h"
#include "Management/ScopeGame/GsItemCollectionManager.h"
#include "Management/ScopeGame/GsMultiLevelRankManager.h"
#include "Management/ScopeGame/GsCostumeManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGame/GsMonsterCollectionManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsTitleManager.h"
#include "Management/ScopeGame/GsSpiritShotmanager.h"
#include "Management/ScopeGame/GsCodexmanager.h"

#include "Guild/GsGuildItemCollectionData.h"

#include "Title/GsTitleEquip.h"
#include "Title/GsTitleCollectionReward.h"
#include "UTIL/GsTableUtil.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"

#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Stat/GsStatInfo.h"


const IGsCollectionData* FGsPassivityHelper::GetCollectionData(PassivityOwnerType InType)
{
	switch (InType)
	{
	case PassivityOwnerType::GUILD_BUILDING:
	{
		return GGuild()->GetCollectionData();
	}
	case PassivityOwnerType::FAIRY_COLLECTION:
	{
		return GSFairy()->GetCollectionData();
	}
	case PassivityOwnerType::ITEM_COLLECTION:
	{
		return GSItemCollection()->PassivityCollectionData();
	}
	case PassivityOwnerType::COSTUME_COLLECTION:
	{
		return GSCostume()->GetCollectionData();
	}
	case PassivityOwnerType::MULTI_LEVEL_RANK:
	{
		return GSMultiLevelRank()->GetCollectionData();
	}
	case PassivityOwnerType::GUILD_ITEM_COLLECTION:
	{
		return GGuildItemCollection()->GetCollectionData();
	}
	case PassivityOwnerType::MONSTER_BOOK:
	{
		return GsMonsterCollection()->PassivityCollectionData();
	}
	case PassivityOwnerType::TITLE_COLLECTION:
	{
		return GTitle()->GetCollectionRewardData();
	}
	case PassivityOwnerType::TITLE_EQUIP:
	{
		return GTitle()->GetTitleEquipData();
	}
	case PassivityOwnerType::SPIRIT_SHOT_ENCHANT:
	{
		return GSSpiritShot()->GetPassivityCollectionData();
	}
	case PassivityOwnerType::SPIRIT_SHOT_ENCHANT_SUM_LEVEL:
	{
		return GSSpiritShot()->GetSumPassivityCollectionData();
	}
	case PassivityOwnerType::CODEX:
	{
		return GsCodex()->GetExplorePassivityCollectionData();
	}
	// TODO: 컬렉션 처리 추가
	}

	return nullptr;
}

const FGsSchemaCollectionData* FGsPassivityHelper::GetCollectionTableData(PassivityOwnerType InType)
{
	return UGsTableUtil::FindRowById<UGsTableCollectionData, FGsSchemaCollectionData, PassivityOwnerType>(InType);
}

const FGsSchemaAbnormalitySet* FGsPassivityHelper::GetInvokeAbnormality(const FGsSchemaPassivitySet* InPassivitySet)
{
	if (nullptr == InPassivitySet)
	{
		return nullptr;
	}

	if (PassivityType::INVOKE_ABNORMALITY != InPassivitySet->type)
	{
		return nullptr;
	}

	// 주의: 이렇게 하면 값이 안찾아짐...
	//const FGsSchemaPassivityEffectInvokeAbnormalitySet* invokeAbnormalityData = 
	//	UGsTableUtil::FindRow<const FGsSchemaPassivityEffectInvokeAbnormalitySet>(InPassivitySet->effectId);
	
	const UGsTablePassivityEffectInvokeAbnormalitySet* invokeAbnormalityTable = Cast<UGsTablePassivityEffectInvokeAbnormalitySet>(
		FGsSchemaPassivityEffectInvokeAbnormalitySet::GetStaticTable());
	if (nullptr == invokeAbnormalityTable)
	{
		return nullptr;
	}

	if (0 < InPassivitySet->effectInfoList.Num())
	{
		int effectId = InPassivitySet->effectInfoList[0].effectId;

		const FGsSchemaPassivityEffectInvokeAbnormalitySet* invokeAbnormalityData = nullptr;
		if (false == invokeAbnormalityTable->FindRowById(effectId, invokeAbnormalityData))
		{
			return nullptr;
		}

		return invokeAbnormalityData->param.abnormalityId.GetRow();
	}

	return nullptr;
}

const FGsSchemaAbnormalitySet* FGsPassivityHelper::GetInvokeAbnormality(PassivityId InId)
{
	return GetInvokeAbnormality(UGsTableUtil::FindRow<const FGsSchemaPassivitySet>(InId));
}

const int32 FGsPassivityHelper::GetArcasPoint()
{
	FGsGameDataManager* dataManager = GGameData();
	if (nullptr == dataManager)
		return 0;

	const FGsNetUserData* userData = dataManager->GetUserData();
	if (nullptr == userData)
		return 0;

	return userData->GetBlessPoint();
}

FSoftObjectPath FGsPassivityHelper::GetArcasIconPath()
{
	ArcasPoint arcasPoint = FGsPassivityHelper::GetArcasPoint();

	const FGsSchemaArcas* row = nullptr;
	if (const UGsTable* table = FGsSchemaArcas::GetStaticTable())
	{
		if (table->FindRow(TEXT("arcas"), row))
		{
			const TArray<FGsSchemaArcasBless>& arcasBlessList = row->arcasBlessList;
			int arcasBlessNum = arcasBlessList.Num();
			int currIndex = 0;
			for (int i= 0; i < arcasBlessNum; ++i)
			{
				if (arcasPoint >= arcasBlessList[i].minPoint &&
					arcasPoint <= arcasBlessList[i].maxPoint)
				{
					currIndex = i;
					break;
				}
			}

			FGsSchemaArcasConfig schemaArcasConfig = row->arcasConfig;
			int iconPathNum = schemaArcasConfig.iconPath.Num();
			if (iconPathNum > currIndex)
			{
				return schemaArcasConfig.iconPath[currIndex];
			}
		}
	}

	return FSoftObjectPath();
}

const int32 FGsPassivityHelper::GetArcasMaxArcasPoint()
{
	ArcasPoint arcasPoint = FGsPassivityHelper::GetArcasPoint();

	const FGsSchemaArcas* row = nullptr;
	if (const UGsTable* table = FGsSchemaArcas::GetStaticTable())
	{
		if (table->FindRow(TEXT("arcas"), row))
		{
			FGsSchemaArcasConfig schemaArcasConfig = row->arcasConfig;
			return schemaArcasConfig.maxArcasPoint;
		}
	}

	return 0;
}

bool FGsPassivityHelper::GetIsCharged(int32 In_DetailID)
{
	const UGsTableItemConsumeDetail* TableItemConsumeDetail = Cast<UGsTableItemConsumeDetail>(FGsSchemaItemConsumeDetail::GetStaticTable());
	if (nullptr == TableItemConsumeDetail)
	{
		GSLOG(Error, TEXT("nullptr == TableItemConsumeDetail"));
		return false;
	}

	const FGsSchemaItemConsumeDetail* ItemConsumeDetail = nullptr;
	if (!TableItemConsumeDetail->FindRowById(In_DetailID, ItemConsumeDetail))
	{
		GSLOG(Error, TEXT("!TableItemConsumeDetail->FindRowById(%d, ItemConsumeDetail)"), In_DetailID);
		return false;
	}

	UGsGameObjectBase* objectBase = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (nullptr == objectBase)
	{
		return false;
	}

	UGsGameObjectLocalPlayer* localPlayer = Cast<UGsGameObjectLocalPlayer>(objectBase);
	if (nullptr == localPlayer)
	{
		return false;
	}

	const FGsStatInfo* statArcasIncreaseRate = localPlayer->GetCreatureStatInfoByType(StatType::BONUS_ARCASPOINT_INCREASE_AMOUNT_RATE);
	if (nullptr == statArcasIncreaseRate)
	{
		return false;
	}

	//보유량 + 축복 충전량 > 축복 보유량 최대값
	int myAmount = FGsPassivityHelper::GetArcasPoint();
	int addAmount = ItemConsumeDetail->effectIDList[0] * (1 + (statArcasIncreaseRate->GetStatValue() / 10000));

	const FGsStatInfo* statArcasMaxIncrease = localPlayer->GetCreatureStatInfoByType(StatType::BONUS_BLESS_MAX_INCREASE_VALUE);
	if (nullptr == statArcasMaxIncrease)
	{
		return false;
	}

	// 축복 보유량 최대값 = arcasConfig.maxArcasPoint + BONUS_BLESS_MAX_INCREASE_VALUE
	int32 maxArcasPoint = GetArcasMaxArcasPoint() + (statArcasMaxIncrease->GetStatValue());
	if (myAmount + addAmount > maxArcasPoint)
	{
		return false;
	}

	return true;
}