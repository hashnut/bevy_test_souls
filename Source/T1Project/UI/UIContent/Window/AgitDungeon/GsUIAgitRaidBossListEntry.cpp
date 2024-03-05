// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIAgitRaidBossListEntry.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Classes/PaperSprite.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsToggleButton.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "DataSchema/Guild/Agit/GsSchemaAgitDungeonRaid.h"
#include "Guild/GsAgitDungeonRaidData.h"
#include "UTIL/GsLevelUtil.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Net/GsNetSendServiceGuild.h"


void UGsUIAgitRaidBossListEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnSlot->OnClicked.AddDynamic(this, &UGsUIAgitRaidBossListEntry::OnClickSlot);
	_toggleBtnBookmark->OnClicked.AddDynamic(this, &UGsUIAgitRaidBossListEntry::OnClickBookmark);
}

void UGsUIAgitRaidBossListEntry::NativeConstruct()
{
	Super::NativeConstruct();

	// 광클방지 타임 설정
	_toggleBtnBookmark->SetOnClickLockSec(0.5f);
}

void UGsUIAgitRaidBossListEntry::SetIsSelected(bool bInIsSelected)
{
	_bIsSelected = bInIsSelected;

	_imgSelected->SetVisibility(_bIsSelected ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

void UGsUIAgitRaidBossListEntry::SetData(TWeakPtr<FGsAgitDungeonRaidBossData> InData)
{
	if (false == InData.IsValid())
	{
		return;
	}

	FGsAgitDungeonRaidBossData* bossData = InData.Pin().Get();
	if (nullptr == bossData)
	{
		return;
	}

	const FGsSchemaAgitDungeonRaid* raidTable = bossData->GetTableData();
	if (nullptr == raidTable)
	{
		return;
	}

	_raidId = bossData->GetRaidId();

	// 보스 이미지
	int32 raidId = _raidId;
	TWeakObjectPtr<UGsUIAgitRaidBossListEntry> weakThis(this);
	FGsUIHelper::AsyncLoadSpriteFromPath(raidTable->portraitImg,
		FStreamableDelegateParam::CreateWeakLambda(this, [weakThis, raidId](UObject* InRes)
			{
				if (weakThis.IsValid())
				{
					if (UGsUIAgitRaidBossListEntry* strongThis = weakThis.Get())
					{
						strongThis->SetBossImage(raidId, Cast<UPaperSprite>(InRes));
					}
				}
			}
	));

	MapId mapId = INVALID_MAP_ID;

	// 맵이름
	if (const FGsSchemaMapData* mapTable = raidTable->mapId.GetRow())
	{
		mapId = mapTable->id;
		_textBlockMap->SetText(mapTable->MapStringId);
	}
	else
	{
		_textBlockMap->SetText(FText::GetEmpty());
	}

	// 보스이름 + 레벨
	if (const FGsSchemaNpcData* npcTable = raidTable->bossId.GetRow())
	{
		if (INVALID_MAP_ID == mapId)
		{
			_textBlockBossLevel->SetText(FText::GetEmpty());
		}
		else
		{
			int32 bossLevel = UGsLevelUtil::GetSpawnLevel(npcTable, mapId);

			// TEXT: Lv.{0}
			FText textFormat;
			if (FText::FindText(TEXT("FieldBossText"), TEXT("UI_Text_Level"), textFormat))
			{
				FText textLevel = FText::Format(textFormat, bossLevel);
				_textBlockBossLevel->SetText(textLevel);
			}
			else
			{
				_textBlockBossLevel->SetText(FText::AsNumber(bossLevel));
			}
		}

		_textBlockBossName->SetText(npcTable->nameText);
	}
	else
	{
		_textBlockBossLevel->SetText(FText::GetEmpty());
		_textBlockBossName->SetText(FText::GetEmpty());
	}

	// 레벨 제한
	// TEXT: 기사단 레벨 {0}이상
	FText textFormat;
	if (FText::FindText(TEXT("GuildText"), TEXT("Raid_GuildLevelLimit"), textFormat))
	{
		FText textLimit = FText::Format(textFormat, bossData->GetLevelLimit());
		_textBlockLevelLimit->SetText(textLimit);
	}
	else
	{
		_textBlockLevelLimit->SetText(FText::GetEmpty());
	}
	
	// 상태처리
	switch (bossData->GetRaidState())
	{
	case AgitDungeonRaidState::NONE:
	case AgitDungeonRaidState::CLOSED:
		_switcherState->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case AgitDungeonRaidState::OPEN:
		_switcherState->SetVisibility(ESlateVisibility::HitTestInvisible);
		_switcherState->SetActiveWidgetIndex(0);
		break;
	case AgitDungeonRaidState::CLEAR:
		_switcherState->SetVisibility(ESlateVisibility::HitTestInvisible);
		_switcherState->SetActiveWidgetIndex(1);
		break;
	}
}

void UGsUIAgitRaidBossListEntry::SetIsBookmark(bool bIsBookMark)
{
	_toggleBtnBookmark->SetIsSelected(bIsBookMark);
}

void UGsUIAgitRaidBossListEntry::SetBossImage(int32 InRaidId, UPaperSprite* InSprite)
{
	if (InRaidId != _raidId)
	{
		return;
	}

	if (InSprite)
	{
		_imgBoss->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_imgBoss->SetBrushFromAtlasInterface(InSprite);
	}
	else
	{
		_imgBoss->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UGsUIAgitRaidBossListEntry::OnClickSlot()
{
	if (_toggleCallback)
	{
		_toggleCallback(_indexInGroup, !_bIsSelected);
	}
}

void UGsUIAgitRaidBossListEntry::OnClickBookmark()
{
	if (0 < _raidId)
	{
		if (_toggleBtnBookmark->GetIsSelected())
		{
			FGsNetSendServiceGuild::SendReqAgitDungeonRaidBookmarkDelete(_raidId);
		}
		else
		{
			FGsNetSendServiceGuild::SendReqAgitDungeonRaidBookmarkAdd(_raidId);
		}
	}
}
