#pragma once


#include "GsDungeonGroupBase.h"
#include "GsDungeonGroupNormal.h"
#include "GsDungeonGroupParty.h"
#include "../Management/ScopeGame/GsDungeonManager.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "DataSchema/Dungeon/GsSchemaCoopPartyDungeonConfig.h"
#include "DataSchema/Dungeon/GsSchemaDungeonGroupData.h"
#include "../UTIL/GsTableUtil.h"
#include "../T1Project.h"
#include "Shared/Client/SharedEnums/SharedPartyDungeonEnum.h"
#include "GsDungeonGroupBattleArena.h"



class T1PROJECT_API FGsDungeonHelper
{
public:

	static FGsDungeonGroupBase* Factory(DungeonGroupId InDungeonGroupId)
	{
		auto FindTableData = [](DungeonGroupId InDungeonGroupId) -> const FGsSchemaDungeonGroupData*
		{
			return UGsTableUtil::FindRowById<UGsTableDungeonGroupData, FGsSchemaDungeonGroupData>(InDungeonGroupId);
		};

		const FGsSchemaDungeonGroupData* findData = FindTableData(InDungeonGroupId);
		if (nullptr == findData)
		{
			GSLOG(Error, TEXT("nullptr == findData : [%d]"), InDungeonGroupId);
			return nullptr;
		}

		FGsDungeonGroupBase* newData = nullptr;

		switch (findData->dungeonCategory)
		{
		case DungeonCategory::NORMAL:
		case DungeonCategory::SPECIAL:
		case DungeonCategory::ELITE:
			newData = new FGsDungeonGroupNormal(findData);
			break;		
		case DungeonCategory::PARTY:
		{
			const FGsSchemaCoopPartyDungeonConfig* config = GSDungeon()->GetPartyDungeonConfig();
			newData = new FGsDungeonGroupParty(findData, config->weeklyRewardCount);
		}break;
		case DungeonCategory::ARENA:
		{
			newData = new FGsDungeonGroupBattleArena(findData);
		}break;
		};

		return newData;
	};


	static bool FindText(const FTextKey& InKey, OUT FText& OutText)
	{
		if (false == FText::FindText(TEXT("DungeonText"), InKey, OutText))
		{
			OutText = FText::FromString(FString::Format(TEXT("UnKnow Message : {0}"), { InKey.GetChars() }));
			return false;
		}

		return true;
	}

	static bool DungeonNameByMapData(const FGsSchemaDungeonGroupData* InDungoneGroup, const FGsSchemaDungeonData* InDungeon, FText& OutText)
	{
		OutText = FText::GetEmpty();

		if (nullptr == InDungoneGroup || nullptr == InDungeon)
		{
			return false;
		}

		const FGsSchemaMapData* mapData = InDungeon->mapId.GetRow();
		if (nullptr == mapData)
		{
			return false;
		}

		/**
		 * 2023/3/2 PKT - 기존에는 던전 타입별로 약간씩 이름을 출력하는 방식이 달랐으나, 이젠 지역 이름으로 통합됨.
		 */
		OutText = mapData->MapStringId;

		return (false == OutText.IsEmpty());
	}


	static bool FindTextDifficulty(DungeonDifficulty InDifficulty, bool InTag, FText& OutText)
	{
		OutText = FText::GetEmpty();

		auto AddTag = [InDifficulty, InTag](OUT FText& OutText) -> bool
		{
			if (OutText.IsEmpty())
			{
				return false;
			}
	
			FString inTagText;
			switch (InDifficulty)
			{
			case DungeonDifficulty::EASY:
				inTagText = FString::Format(TEXT("<easy>{0}</>"), { OutText.ToString() });
				break;
			case DungeonDifficulty::NORMAL:
				inTagText = FString::Format(TEXT("<normal>{0}</>"), { OutText.ToString() });
				break;
			case DungeonDifficulty::DIFFICULT:
				inTagText = FString::Format(TEXT("<hard>{0}</>"), { OutText.ToString() });
				break;
			case DungeonDifficulty::NIGHTMARE:
				inTagText = FString::Format(TEXT("<nightmare>{0}</>"), { OutText.ToString() });
				break;
			};
	
			OutText = FText::FromString(inTagText);
			
			return true;
		};

		switch (InDifficulty)
		{
		case DungeonDifficulty::EASY:
			FGsDungeonHelper::FindText(TEXT("PartyDungeon_Easy"), OutText);
			break;
		case DungeonDifficulty::NORMAL:
			FGsDungeonHelper::FindText(TEXT("PartyDungeon_Normal"), OutText);
			break;
		case DungeonDifficulty::DIFFICULT:
			FGsDungeonHelper::FindText(TEXT("PartyDungeon_Difficult"), OutText);
			break;
		case DungeonDifficulty::NIGHTMARE:
			FGsDungeonHelper::FindText(TEXT("PartyDungeon_Nightmare"), OutText);
			break;
		};

		return (InTag) ? AddTag(OutText) : (false == OutText.IsEmpty());
	}

	static bool DungeonNameByDifficulty(const FGsDungeonGroupBase* InDungeonGroupData
		, const FGsDungeonData* InDungeonData, bool InTag, FText& OutText)
	{
		FText dungeonName;
		DungeonNameByMapData(InDungeonGroupData->GetSchemaData(), InDungeonData->GetSchema(), dungeonName);

		FText difficultyName;
		FindTextDifficulty(InDungeonData->GetSchema()->levelOfDifficulty, InTag, difficultyName);

		OutText = FText::FromString(FString::Format(TEXT("{0} {1}"), { dungeonName.ToString(), difficultyName.ToString() }));

		return true;
	}		
};

