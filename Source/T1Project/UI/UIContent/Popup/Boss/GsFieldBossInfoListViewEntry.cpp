// Fill out your copyright notice in the Description page of Project Settings.


#include "GsFieldBossInfoListViewEntry.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/Image.h"
#include "UMG/Public/Components/Widget.h"
#include "UMG/Public/Components/Button.h"
#include "Paper2D/Classes/PaperSprite.h"
#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateBoss.h"
#include "GameObject/Boss/GsFieldBossHandler.h"
#include "GsTable.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGame/GsWorldMapManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGame/GsDungeonManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsBossMessageParam.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "UI/UIContent/Window/Boss/GsUIWindowBossInfo.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIControlLib/ContentWidget/GsToggleButton.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "Net/GsNetSendServiceBoss.h"
#include "DataSchema/Map/FieldBoss/GsSchemaFieldBossData.h"
#include "DataSchema/Dungeon/GsSchemaDungeonData.h"
#include "Dungeon/GsDungeonGroupBase.h"
#include "Dungeon/GsDungeonData.h"
#include "Dungeon/GsDungeonHelper.h"
#include "Engine/AssetManager.h"

void UGsFieldBossInfoListViewEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_button->OnClicked.AddDynamic(this, &UGsFieldBossInfoListViewEntry::OnClickListViewEntry);
	_bookmarkToggleButton->OnClicked.AddDynamic(this, &UGsFieldBossInfoListViewEntry::OnClickBookmark);
}

void UGsFieldBossInfoListViewEntry::NativeConstruct()
{
	Super::NativeConstruct();
	
	FText::FindText(FIELD_BOSS_TEXT_KEY, BOSS_SPAWN_AREA_TEXT_KEY, _levelLimitText);
	FText::FindText(TEXT("WorldMapText"), TEXT("MonLevel"), _levelFormat);

	if (FGsMessageHolder* msg = GMessage())
	{
		_uiDelegateArray.Emplace(msg->GetUI().AddUObject(MessageUI::INVALID_BOSS
		, this, &UGsFieldBossInfoListViewEntry::OnSelectBossInfo));
	}
}

void UGsFieldBossInfoListViewEntry::NativeDestruct()
{
	if (FGsMessageHolder* msg = GMessage())
	{
		if (_bossDelegateArray.Num() > 0)
		{
			for (auto iter : _bossDelegateArray)
			{
				msg->GetBoss().Remove(iter);
			}
			_bossDelegateArray.Empty();
		}

		if (_uiDelegateArray.Num() > 0)
		{
			for (auto iter : _uiDelegateArray)
			{
				msg->GetUI().Remove(iter);
			}
			_uiDelegateArray.Empty();
		}
	}

	Super::NativeDestruct();
}

void UGsFieldBossInfoListViewEntry::NativeOnListItemObjectSet(UObject* inListItemObject)
{
	UGsFieldBossInfoListViewEntryData* data = Cast<UGsFieldBossInfoListViewEntryData>(inListItemObject);
	if (nullptr == data)
	{
		return;
	}

	_bossData = data->_bossData;
	_mutantBossData = data->_mutantBossData;
	_parent = data->_parent;
	_level = data->_level;
	const FGsSchemaFieldBossData* selectedBossData = FGsGameStateBoss::GetInstance()->GetSelectedBossData();

	InvalidBossInfo();
	InvalidSelect(selectedBossData);
	InvalidBookmark();
}

void UGsFieldBossInfoListViewEntry::OnClickListViewEntry()
{
	FGsGameStateBoss::GetInstance()->SelectedFieldBoss(_bossData->bossId);
}

void UGsFieldBossInfoListViewEntry::InvalidBossInfo()
{
	//get boss npc id			
	const FGsSchemaNpcData* npcData = _bossData->bossNpcId.GetRow();
	if (npcData)
	{
		_bossNameTextBlock->SetText(npcData->nameText);
		_levelTextBlock->SetText(FText::Format(_levelFormat, _level));
		//set boss icon
				//set boss icon
		UPaperSprite* image = Cast<UPaperSprite>(UAssetManager::GetStreamableManager().LoadSynchronous(_bossData->portraitImg));
		_bossIcon->SetBrushFromAtlasInterface(image);
	}

	//get area name
	{
		if (const FGsSchemaMapData* mapData = _bossData->mapId.GetRow())
		{
			FString areaName = _bossData->mapId.GetRow()->MapStringId.ToString();
			_areaNameTextBlock->SetText(FText::FromString(areaName));
		}	
	}
}

void UGsFieldBossInfoListViewEntry::InvalidBookmark()
{
	int32 bossId = _bossData->bossId;
	bool isContain = GGameData()->GetUserData()->bossBookMarkArray.Contains(bossId);
	_bookmarkToggleButton->SetIsSelected(isContain);
}

void UGsFieldBossInfoListViewEntry::OnSelectBossInfo(const IGsMessageParam* inParam)
{
	if (nullptr == inParam)
	{
		return;
	}

	const FGsUIMsgParamSelectBoss* selectBoss = inParam->Cast<const FGsUIMsgParamSelectBoss>();
	if (selectBoss)
	{
		InvalidSelect(selectBoss->_fieldBoss);
	}
}

void UGsFieldBossInfoListViewEntry::InvalidSelect(const struct FGsSchemaFieldBossData* inData)
{
	if (inData == _bossData)
	{
		_selectImage->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		_selectImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UGsFieldBossInfoListViewEntry::OnClickBookmark()
{
	int32 bossId = _bossData->bossId;
	bool isContain = GGameData()->GetUserData()->bossBookMarkArray.Contains(bossId);	

	int32 newSelectBossId;
	if (false == TryGetNewSelectBossId(bossId, !isContain, newSelectBossId))
	{
		return;
	}	

	if (isContain)
	{
		GGameData()->RemoveBossBookmark(bossId);
	}
	else
	{
		GGameData()->AddBossBookmark(bossId);
	}	

	//북마크 추가해서 모든 북마크가 찰 경우 마지막 북마크 선택
	if (IsFullBookmark())
	{
		newSelectBossId = GetLastBossId();
	}

	if (IsEmptyBookmark())
	{
		newSelectBossId = GetFirstBossId();
	}
	
	InvalidBookmark();

	FGsNetSendServiceBoss::SendReqUpdateFieldBossBookmark(GGameData()->GetUserData()->bossBookMarkArray);

	FGsUIMsgFieldBossBookmarkParam param = FGsUIMsgFieldBossBookmarkParam(newSelectBossId, !isContain);
	GMessage()->GetUI().SendMessage(MessageUI::CHANGE_FIELD_BOSS_BOOKMARK, &param);
}

const FGsSchemaFieldBossData* UGsFieldBossInfoListViewEntry::GetBossData() const
{
	return _bossData;
}

const FGsSchemaNpcData* UGsFieldBossInfoListViewEntry::GetNpcData() const
{
	return _bossData->bossNpcId.GetRow();
}

bool UGsFieldBossInfoListViewEntry::TryGetNewSelectBossId(int32 inBossId, bool inAdd, OUT int32& outBossId)
{
	TArray<const struct FGsSchemaFieldBossData*> bossArray;	
	if (false == FGsGameStateBoss::GetInstance()->TryGetFieldBossArray(bossArray))
	{
		return false;

	}
	int32 index = bossArray.IndexOfByPredicate([&](const struct FGsSchemaFieldBossData* bossData)->bool {
		return bossData->bossId == inBossId;
		});

	int32 newSelectIndex = index + 1;
	if (false == bossArray.IsValidIndex(newSelectIndex))
	{				
		newSelectIndex = index - 1;
	}

	if (false == bossArray.IsValidIndex(newSelectIndex))
	{
		return false;
	}

	outBossId = bossArray[newSelectIndex]->bossId;

	return true;
}

bool UGsFieldBossInfoListViewEntry::IsFullBookmark()
{
	TArray<const struct FGsSchemaFieldBossData*> bossArray;
	if (false == FGsGameStateBoss::GetInstance()->TryGetFieldBossArray(bossArray))
	{
		return false;
	}

	//만약 최대 북마크 갯수라면 
	return GGameData()->GetUserData()->bossBookMarkArray.Num() == bossArray.Num();
}

bool UGsFieldBossInfoListViewEntry::IsEmptyBookmark()
{
	return  GGameData()->GetUserData()->bossBookMarkArray.Num() == 0;
}

int32 UGsFieldBossInfoListViewEntry::GetFirstBossId()
{
	TArray<const struct FGsSchemaFieldBossData*> bossArray;
	const UGsTable* fieldBossTable = FGsSchemaFieldBossData::GetStaticTable();
	if (fieldBossTable)
	{
		const UGsTableFieldBossData* castFieldBossTable = Cast<const UGsTableFieldBossData>(fieldBossTable);
		if (castFieldBossTable)
		{
			castFieldBossTable->GetAllRows(bossArray);
		}
	}

	if (0 == bossArray.Num())
	{
		return 0;
	}

	return bossArray[0]->bossId;
}

int32 UGsFieldBossInfoListViewEntry::GetLastBossId()
{
	const TArray<FieldBossId>& bossIdArray = GGameData()->GetUserData()->bossBookMarkArray;
	if (0 == bossIdArray.Num())
	{
		return 0;
	}

	return bossIdArray.Last();
}
