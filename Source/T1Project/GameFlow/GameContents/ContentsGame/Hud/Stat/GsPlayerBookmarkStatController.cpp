// Fill out your copyright notice in the Description page of Project Settings.


#include "GsPlayerBookmarkStatController.h"
#include "Net/GsNetSendServiceWorld.h"
#include "DataSchema/Stat/GsSchemaStatFormatingData.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/Data/GsGameObjectDataLocalPlayer.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "DataSchema/Stat/GsSchemaStatConfig.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "GameObject/Stat/GsStatHelper.h"

FGsPlayerBookmarkStatController::FGsPlayerBookmarkStatController()
{
	_statArray.Empty();
}

FGsPlayerBookmarkStatController::~FGsPlayerBookmarkStatController()
{
	_statArray.Empty();
}

void FGsPlayerBookmarkStatController::Init()
{
	_statArray.Empty();
	_statFormatDataArray.Empty();	
	FGsSchemaStatFormatingData::GetStaticTable()->GetAllRows(_statFormatDataArray);

	const FGsSchemaStatConfig* config = UGsTableUtil::FindFirstRow<FGsSchemaStatConfig>();
	if (config)
	{
		_maxStatBookmarkCount = config->MaxUserStatBookmarkCount;
	}
}

void FGsPlayerBookmarkStatController::Close()
{

}

void FGsPlayerBookmarkStatController::SetBookmarkStat(PD::SC::PKT_SC_ACK_USER_STAT_BOOKMARK_LIST_READ* inPacket)
{
	_statArray.Empty();
	//save favorit stat	
	PD::SC::PKT_SC_ACK_USER_STAT_BOOKMARK_LIST_READ::StatListIterator iterEnd = inPacket->GetLastStatListIterator();
	for (PD::SC::PKT_SC_ACK_USER_STAT_BOOKMARK_LIST_READ::StatListIterator iter = inPacket->GetFirstStatListIterator();
		iter != iterEnd; ++iter)
	{
		_statArray.Add((StatType)iter->Stat());
	}
}

void FGsPlayerBookmarkStatController::AddBookmarkStat(PD::SC::PKT_SC_ACK_USER_STAT_BOOKMARK_ADD_READ* inPacket)
{
	_statArray.Emplace((StatType)inPacket->Stat());
}

void FGsPlayerBookmarkStatController::RemoveBookmarkStat(PD::SC::PKT_SC_ACK_USER_STAT_BOOKMARK_DELETE_READ* inPacket)
{
	_statArray.Remove((StatType)inPacket->Stat());
}

bool FGsPlayerBookmarkStatController::AddStatBookmark(StatType inStatType)
{
	if (_statArray.Num() >= _maxStatBookmarkCount)
	{
		//warning message
		return false;
	}

	FGsNetSendServiceWorld::SendReqUserStatBookmarkAdd(inStatType);

	return true;
}

void FGsPlayerBookmarkStatController::DeleteStatBookmark(StatType inStatType)
{
	FGsNetSendServiceWorld::SendReqUserStatBookmarkDelete(inStatType);
}

bool FGsPlayerBookmarkStatController::IsBookmarkStat(StatType inStat)
{
	return _statArray.Contains(inStat);
}

bool FGsPlayerBookmarkStatController::TryGetStat(OUT TArray<StatType>& outFavoritStatArray, OUT TMap<int32, TArray<StatType>>& outSectionStatMap)
{
	CreatureWeaponType weaponType = FGsStatHelper::GetPlayerWeapon();
	TFunction<bool(StatType, StatType)> sortFunc = [&](StatType sourceStatType, StatType destStatType)->bool {
		//section check
		const FGsSchemaStatFormatingData* sourceStatData = FGsStatHelper::FindStat(sourceStatType);
		const FGsSchemaStatFormatingData* destStatData = FGsStatHelper::FindStat(destStatType);
				
		//다음과 같은 순서로 스탯을 보여준다.
		//1. 공용 스탯
		//2. 무기 스탯
		//3. uiOrder
		if (sourceStatData && destStatData)
		{
			//wepon check
			bool isImportantSource = (0 == sourceStatData->correspondingWeapon.Num()) || sourceStatData->correspondingWeapon.Contains(weaponType);
			bool isImportantDest = (0 == destStatData->correspondingWeapon.Num()) || destStatData->correspondingWeapon.Contains(weaponType);

			if (isImportantSource && false == isImportantDest)
			{
				return true;
			}
			else if (false == isImportantSource && isImportantDest)
			{
				return false;
			}

			return sourceStatData->uiOrderNum < destStatData->uiOrderNum;
		}		

		return false;
	};

	outSectionStatMap.Empty();

	//get favorit stat
	outFavoritStatArray = _statArray;
	int32 num = (int32)StatType::MAX;
	const FGsSchemaStatFormatingData* stat = nullptr;
	StatType statType;
	for (int32 i = 0; i < num; ++i)
	{
		statType = (StatType)i;
		stat = FGsStatHelper::FindStat(statType);
		if (nullptr == stat)
		{
#if STAT_DEBUG
			FString statTypeText = FGsTextUtil::GetStringFromEnum(TEXT("StatType"), (StatType)i);
			GSLOG(Warning, TEXT("Stat Detail Panel - stat missing - stat type : %s"), *statTypeText);
#endif
			continue;
		}

		if (0 == stat->sector)
		{
#if STAT_DEBUG
			FString statTypeText = FGsTextUtil::GetStringFromEnum(TEXT("StatType"), (StatType)i);
			GSLOG(Warning, TEXT("Stat Detail Panel - stat section is 0 - stat type : %s"), *statTypeText);
#endif
			continue;
		}

		if (_statArray.Contains(statType))
		{
			continue;
		}

		if (false == outSectionStatMap.Contains(stat->sector))
		{
			outSectionStatMap.Emplace(stat->sector, TArray<StatType>());
		}
		outSectionStatMap[stat->sector].Emplace(statType);
	}

	//sort favorit stat
	num = outSectionStatMap.Num();
	TArray<int32> keyArray;
	outSectionStatMap.GenerateKeyArray(keyArray);	
	for (int32 i = 0; i < num; ++i)
	{
		outSectionStatMap[keyArray[i]].Sort(sortFunc);
	}

	return true;
}