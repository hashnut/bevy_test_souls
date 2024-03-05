#include "GsUIPageDungeonGroupArena.h"
#include "../UTIL/GsTimeSyncUtil.h"
#include "../Guild/GsGuildHelper.h"
#include "../Reward/GsRewardHelper.h"
#include "../../Helper/GsTimeStringHelper.h"
#include "../Map/WorldMap/GsWorldMapFunc.h"
#include "../Dungeon/GsDungeonCommonEnum.h"
#include "../Dungeon/GsDungeonHelper.h"
#include "../Dungeon/GsDungeonGroupBase.h"
#include "../Dungeon/GsDungeonGroupBattleArena.h"
#include "../Map/WorldMap/GsWorldMapDefine.h"
#include "../Management/GsMessageHolder.h"
#include "../Management/ScopeGlobal/GsUIManager.h"
#include "../Management/ScopeGlobal/GsLevelManager.h"
#include "../Management/ScopeGlobal/GsGameFlowManager.h"
#include "../Message/GsMessageContents.h"
#include "../ContentsServer/GsContentsServerHelper.h"
#include "../GameFlow/GsGameFlowGame.h"
#include "../GameFlow/Stage/StageGame/GsStageManagerGame.h"
#include "../GameFlow/Stage/StageGame/GsStageGameBase.h"
#include "../GameFlow/Stage/GsStageMode.h"
#include "../GameFlow/GameContents/GsContentsMode.h"
#include "../GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "../GameFlow/GameContents/ContentsGame/GsGameStateDungeon.h"
#include "../GameFlow/GameContents/ContentsGame/GsGameStateRegionMap.h"
#include "../GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "../GameFlow/GameContents/ContentsGame/Dungeon/GsDungeonStateHandler.h"
#include "../GameFlow/GameContents/ContentsGame/Dungeon/GsBattleArenaDungeonState.h"
#include "../GameFlow/GameContents/ContentsGame/Hud/BattleArena/GsBattleArenaData.h"
#include "../GameFlow/GameContents/ContentsGame/Hud/BattleArena/GsBattleArenaHelper.h"
#include "DataSchema/Reward/GsSchemaRewardData.h"
#include "DataSchema/Dungeon/GsSchemaDungeonData.h"
#include "DataSchema/Reward/GsSchemaRewardItemBagData.h"
#include "DataSchema/Dungeon/GsSchemaDungeonGroupData.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "DataSchema/BattleArena/GsSchemaGuildWarConfigData.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedPassivityEnum.h"
#include "UMG/Public/Components/Image.h"
#include "UMG/Public/Components/Button.h"
#include "UMG/Public/Components/ListView.h"
#include "UMG/Public/Components/TextBlock.h"
#include "../../Helper/GsUIHelper.h"
#include "../UI/UILib/Base/GsUIWidgetBase.h"
#include "../UI/UIContent/Common/Icon/GsUIIconAbnormality.h"
#include "../UI/UIContent/Popup/GsUIPopupAbnormalityDetailInfo.h"
#include "../UI/UIContent/Popup/BattleArena/GSUIPopupBattleArenaSeasonRankBoard.h"
#include "../UI/UIControlLib/ContentWidget/GsButton.h"
#include "../UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "../UI/UIContent/Common/Entry/GsUIRewardItemIconListViewEntry.h"
#include "../UI/UIContent/Popup/BattleArena/GsUIPopupBattleArenaGuildWarGuide.h"
#include "../UI/UIContent/Popup/Guild/GuildDungeon/GsUIPopupGuildDungeonReward.h"
#include "Engine/Texture2D.h"
#include "../Net/GsNetSendService.h"
#include "T1Project.h"



void UGsUIPageDungeonGroupArena::BeginDestroy()
{
	if (_listViewEntryPool)
	{
		_listViewEntryPool->RemovePool();
		_listViewEntryPool = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIPageDungeonGroupArena::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnRewardDetail->OnClicked.AddDynamic(this, &UGsUIPageDungeonGroupArena::OnClickedRewardDetail);
	_btnBattleArenaEntered->OnClicked.AddDynamic(this, &UGsUIPageDungeonGroupArena::OnClickedApplyArena);	
	_btnBattleArenaGuide->OnClicked.AddDynamic(this, &UGsUIPageDungeonGroupArena::OnClickedGuildWarGuide);
	_btnBattleArenaRank->OnClicked.AddDynamic(this, &UGsUIPageDungeonGroupArena::OnClickedSeasonRankOpen);

	const static int32 POOL_WARM_UP_SIZE = 10;
	_listViewEntryPool = NewObject<UGsPoolUObject>(this);
	_listViewEntryPool->WarmUp<UGsUIRewardIconListViewEntryData>(POOL_WARM_UP_SIZE);
}

void UGsUIPageDungeonGroupArena::NativeConstruct()
{
	Super::NativeConstruct();

	ClearTimer();	
}

void UGsUIPageDungeonGroupArena::NativeDestruct()
{
	ClearTimer();

	_listViewRewardSet->ClearListItems();
	_listViewEntryPool->ReleaseAll();
	
	Super::NativeDestruct();
}

void UGsUIPageDungeonGroupArena::OnClickedEnvironmentIcon(UGsUIIconAbnormality* InIcon)
{
	if (nullptr == InIcon)
	{
		GSLOG(Error, TEXT("nullptr == InIcon"));
		return;
	}

	TWeakObjectPtr<UGsUIWidgetBase>	widget = GUI()->OpenAndGetWidget(TEXT("PopupAbnormalityDetailInfo"));
	if (widget.IsValid())
	{
		if (UGsUIPopupAbnormalityDetailInfo* popup = Cast<UGsUIPopupAbnormalityDetailInfo>(widget.Get()))
		{
			const FGsSchemaPassivitySet* passivityData = _data->GetEnvPassivity();
			if (passivityData)
			{
				popup->SetPassivitySet(passivityData, PassivityOwnerType::MAP);
			}
		}
	}
}

void UGsUIPageDungeonGroupArena::OnClickedApplyArena()
{
	const static int32 DUNGEON_COUNT_ONE_ONLY = 0;
	const FGsDungeonData* dungeonData = _data->FindDungeonByIndex(DUNGEON_COUNT_ONE_ONLY);
	if (nullptr == dungeonData)
	{
		GSLOG(Error, TEXT("nullptr == dungeonData"));
		return;
	}
	const MapId targetMapId = dungeonData->GetSchema()->mapId.GetRow()->id;

	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	FGsContentsManagerGame* contents = (gameFlow) ? gameFlow->GetContentsManagerGame() : nullptr;
	if (nullptr == contents)
	{
		GSLOG(Error, TEXT("nullptr == contents"));
		return;
	}

	FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
	if (nullptr == hud)
	{
		GSLOG(Error, TEXT("nullptr == hud"));
		return;
	}

	if (FGsBattleArenaGuildWarHandler* battleArenaHandler = hud->GetBattleArenaGuildWarHandler())
	{
		if (targetMapId == battleArenaHandler->GetMapId())
		{
			const EBattleArenaGuildWarState state = battleArenaHandler->GetState();
			if (EBattleArenaGuildWarState::ERegisterPeriod == state || EBattleArenaGuildWarState::ERegisterCompleted == state)
			{	// 2023/11/21 PKT - 참여 신청을 보낸다.
				battleArenaHandler->TryToRegisterOrCancel();
			}
			else if (EBattleArenaGuildWarState::EAdmissionGameAvailable == state)
			{	// 2023/11/21 PKT - Arena 입장을 보낸다.
				battleArenaHandler->TryToEnterBattleArenaGuildWar();				
			}
		}		
	}
}

void UGsUIPageDungeonGroupArena::OnClickedRewardDetail()
{
	const static int32 DUNGEON_COUNT_ONE_ONLY = 0;
	const FGsDungeonData* dungeonData = _data->FindDungeonByIndex(DUNGEON_COUNT_ONE_ONLY);
	if (nullptr == dungeonData && nullptr == dungeonData->GetSchema()->mapId.GetRow())
	{
		GSLOG(Error, TEXT("nullptr == dungeonData"));
		return;
	}

	/**
	 * 보상 정보
	 */
	TArray<const FGsSchemaRewardItemBagData*> rewardItemBag;
	for (const FGsSchemaRewardDataRow& row : dungeonData->GetSchema()->mainRewardList)
	{
		FGsRewardUnboxingData rewardUnboxingData;
		if (row.GetRow() && true == FGsRewardHelper::UnboxingRewardData(row.GetRow()->id, rewardUnboxingData))
		{
			for (const auto& itemBagData : rewardUnboxingData._itemDataList)
			{
				rewardItemBag.Emplace(itemBagData);
			}
		}
	}

	TWeakObjectPtr<UGsUIPopupGuildDungeonReward> widget 
		= Cast<UGsUIPopupGuildDungeonReward>(GUI()->OpenAndGetWidget("PopupGuildDungeonReward"));
	if (widget.IsValid())
	{
		FText title;
		FGsDungeonHelper::FindText(TEXT("PartyDungeon_MainReward"), title);
		widget.Get()->SetData(title, rewardItemBag);
	}
}

void UGsUIPageDungeonGroupArena::OnClickedGuildWarGuide()
{
	TWeakObjectPtr<UGsUIPopupBattleArenaGuildWarGuide> popup
		= Cast<UGsUIPopupBattleArenaGuildWarGuide>(GUI()->OpenAndGetWidget(TEXT("PopupBattleArenaGuildWarGuide")));
	if (popup.IsValid())
	{
		// None
	}
}

void UGsUIPageDungeonGroupArena::OnClickedSeasonRankOpen()
{
	TWeakObjectPtr<UGSUIPopupBattleArenaSeasonRankBoard> popup
		= Cast<UGSUIPopupBattleArenaSeasonRankBoard>(GUI()->OpenAndGetWidget(TEXT("PopupBattleArenaSeasonRankBoard")));
	if (popup.IsValid())
	{
		popup->SetData(RankSearchType::KNIGHTS);
	}
}

void UGsUIPageDungeonGroupArena::OnTimer()
{
	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	FGsContentsManagerGame* contents = (gameFlow) ? gameFlow->GetContentsManagerGame() : nullptr;
	if (nullptr == contents)
	{
		GSLOG(Error, TEXT("nullptr == contents"));
		return;
	}

	FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
	if (nullptr == hud)
	{
		GSLOG(Error, TEXT("nullptr == hud"));
		return;
	}

	if (FGsBattleArenaGuildWarHandler* battleArenaHandler = hud->GetBattleArenaGuildWarHandler())
	{
		_textRemainingTime->SetText(battleArenaHandler->TextStateRemainingTime());
	}
}

void UGsUIPageDungeonGroupArena::ClearTimer()
{
	UWorld* world = GetWorld();
	if (world && _timerHandle.IsValid())
	{
		world->GetTimerManager().ClearTimer(_timerHandle);
	}
}

void UGsUIPageDungeonGroupArena::InvalidateApplyButtonState()
{
	const static int32 DUNGEON_COUNT_ONE_ONLY = 0;
	const FGsDungeonData* dungeonData = _data->FindDungeonByIndex(DUNGEON_COUNT_ONE_ONLY);
	if (nullptr == dungeonData && nullptr == dungeonData->GetSchema()->mapId.GetRow())
	{
		GSLOG(Error, TEXT("nullptr == dungeonData"));
		return;
	}

	const MapId targetMapId = dungeonData->GetSchema()->mapId.GetRow()->id;

	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	FGsContentsManagerGame* contents = (gameFlow) ? gameFlow->GetContentsManagerGame() : nullptr;
	if (nullptr == contents)
	{
		GSLOG(Error, TEXT("nullptr == contents"));
		return;
	}

	FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
	if (nullptr == hud)
	{
		GSLOG(Error, TEXT("nullptr == hud"));
		return;
	}

	bool isBtnEnable = false;
	bool isTimerOn = false;
	// 2023/11/20 PKT - 참여 신청(Default)
	FText textBtnName;
	FGsDungeonHelper::FindText(TEXT("Button_GuildArena_ApplyJoin"), textBtnName);

	if (FGsBattleArenaGuildWarHandler* battleArenaHandler = hud->GetBattleArenaGuildWarHandler())
	{
		if (dungeonData->IsUnLockState() && targetMapId == battleArenaHandler->GetMapId())
		{
			EBattleArenaGuildWarState currentState = battleArenaHandler->GetState();

			if (EBattleArenaGuildWarState::ERegisterPeriod == currentState)
			{
				isBtnEnable = true;
			}
			else if (EBattleArenaGuildWarState::ERegisterCompleted == currentState)
			{
				isBtnEnable = true;
				// 2023/11/21 PKT - 취 소
				FGsDungeonHelper::FindText(TEXT("Button_GuildArena_ApplyJoin_Cancel"), textBtnName);
			}
			else if (EBattleArenaGuildWarState::EMatchingWait == currentState)
			{
				isBtnEnable = false;
				isTimerOn = true;
				// 2023/11/21 PKT - 로비 입장
				FGsDungeonHelper::FindText(TEXT("Button_LobbyEntrance"), textBtnName);
			}
			else if (EBattleArenaGuildWarState::EAdmissionGameAvailable == currentState)
			{
				isBtnEnable = true;
				// 2023/11/21 PKT - 로비 입장
				FGsDungeonHelper::FindText(TEXT("Button_LobbyEntrance"), textBtnName);
			}
		}
	}

	_btnBattleArenaEntered->SetIsEnabled(isBtnEnable);
	_textEnteredBtnName->SetText(textBtnName);

	if (isTimerOn)
	{	// 2023/11/23 PKT - 남은 시간을 표기
		_panelEnterRemainingTime->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		if (false == _timerHandle.IsValid())
		{
			if (UWorld* world = GetWorld())
			{	// 2023/12/8 PKT - Timer를 사용하면 Text가 잠깐 튀는 현상이 발생. 미리 한번 불러 준다.
				OnTimer();
				world->GetTimerManager().SetTimer(_timerHandle, this, &UGsUIPageDungeonGroupArena::OnTimer, 0.5f, true);
			}
		}
	}
	else
	{	// 2023/12/7 PKT - Timer Off
		_panelEnterRemainingTime->SetVisibility(ESlateVisibility::Collapsed);

		ClearTimer();
	}
}

void UGsUIPageDungeonGroupArena::Enter(const FGsDungeonGroupBase* InData, const FGsDungeonUICaptureData& InCaptureData)
{
	Super::Enter(InData, InCaptureData);

	const FGsSchemaDungeonGroupData* schemaGroupData = _data->GetSchemaData();

	// 던전그룹 이름
	_textName->SetText(schemaGroupData->dungeonGroupName);

	// 2023/2/22 PKT - 환경 효과 아이콘
	if (_data->GetEnvPassivity())
	{
		_iconSelectorEnv->SetVisibility(ESlateVisibility::Visible);

		UGsUIIconAbnormality* icon = Cast<UGsUIIconAbnormality>(_iconSelectorEnv->GetChildAt(0));
		if (nullptr == icon)
		{
			icon = Cast<UGsUIIconAbnormality>(_iconSelectorEnv->CreateIcon());
			_iconSelectorEnv->AddChild(icon);
		}
		icon->SetData(_data->GetEnvPassivity());
		if (false == icon->OnClickIcon.IsBoundToObject(this))
		{
			icon->OnClickIcon.AddUObject(this, &UGsUIPageDungeonGroupArena::OnClickedEnvironmentIcon);
		}
	}
	else
	{
		_iconSelectorEnv->SetVisibility(ESlateVisibility::Collapsed);
	}

	// 던전그룹 설명
	_textDesc->SetText(schemaGroupData->dungeonGroupDesc);

	// 던전 대표 이미지
	UTexture2D* dungeonImg = FGsUIHelper::LoadTextureFromPath(schemaGroupData->dungeonGroupTexutreImg);
	if (dungeonImg)
	{
		_imgDungeonGroup->SetBrushFromTexture(dungeonImg);
	}

	{
		const static int32 DUNGEON_COUNT_ONE_ONLY = 0;
		const FGsDungeonData* dungeonData = _data->FindDungeonByIndex(DUNGEON_COUNT_ONE_ONLY);
		if (nullptr == dungeonData && nullptr == dungeonData->GetSchema()->mapId.GetRow())
		{
			GSLOG(Error, TEXT("nullptr == dungeonData"));
			return;
		}

		/**
		 * 보상 정보
		 */
		TSet<const FGsSchemaRewardItemBagData*> rewardItemBag;
		for (const FGsSchemaRewardDataRow& row : dungeonData->GetSchema()->mainRewardList)
		{
			FGsRewardUnboxingData rewardUnboxingData;
			if (row.GetRow() && true == FGsRewardHelper::UnboxingRewardData(row.GetRow()->id, rewardUnboxingData))
			{
				for (const auto& itemBagData : rewardUnboxingData._itemDataList)
				{
					rewardItemBag.Emplace(itemBagData);
				}
			}
		}

		_listViewEntryPool->ReleaseAll();

		TArray<UGsUIRewardIconListViewEntryData*> rewardEntrySet;
		FGsUIRewardIconData rewardIconData;
		for (const auto& itemBag : rewardItemBag)
		{
			UGsUIRewardIconListViewEntryData* viewEntryData = _listViewEntryPool->Claim<UGsUIRewardIconListViewEntryData>();
			viewEntryData->_rewardItemDataArray.Empty();
			rewardIconData.Init(itemBag, itemBag->currencyAmountMin);
			viewEntryData->_rewardItemDataArray.Add(rewardIconData);
			rewardEntrySet.Emplace(viewEntryData);
		}

		_listViewRewardSet->SetListItems(rewardEntrySet);
		_listViewRewardSet->RegenerateAllEntries();
		_listViewRewardSet->SetScrollbarVisibility(ESlateVisibility::Collapsed);
		_listViewRewardSet->ScrollIndexIntoView(0);
	}

	Invalidate();
}

void UGsUIPageDungeonGroupArena::Invalidate()
{
	Super::Invalidate();

	const static int32 DUNGEON_COUNT_ONE_ONLY = 0;
	const FGsDungeonData* dungeonData = _data->FindDungeonByIndex(DUNGEON_COUNT_ONE_ONLY);
	if (nullptr == dungeonData)
	{
		GSLOG(Error, TEXT("nullptr == dungeonData"));
		return;
	}

	const FGsDungeonGroupBattleArena* battleArenaGroupData = static_cast<const FGsDungeonGroupBattleArena*>(_data);
	if (nullptr == battleArenaGroupData)
	{
		GSLOG(Error, TEXT("nullptr == battleArenaGroupData"));
		return;
	}

	{	// 2023/11/20 PKT - 제한 조건 정보(레벨/기사단 레벨)
		FText textFormat;
		FGsDungeonHelper::FindText(TEXT("UI_Dungeon_LevelLimit"), textFormat);
		_textLevelLimit->SetText(FText::Format(textFormat, { dungeonData->GetSchema()->unlockLevel }));

		FGsDungeonHelper::FindText(TEXT("UI_Dungeon_GuilArenaLimit"), textFormat);
		_textGuildLevelLimit->SetText(FText::Format(textFormat, { battleArenaGroupData->ApplyLimitGuildLevel(dungeonData) }));
	}
	
	{	// 2023/11/20 PKT - 시즌 정보
		FText textFormat;

		if (battleArenaGroupData->IsFreeSeason(dungeonData))
		{	// 2023/11/23 PKT - 프리 시즌
			FGsDungeonHelper::FindText(TEXT("UI_BattleArena_FreeSeason_Schedule"), textFormat);
			_textSeasonCount->SetText(textFormat);
			_textSeasonDate->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{	// 2023/11/23 PKT - 시즌 {0} 기간
			FGsDungeonHelper::FindText(TEXT("UI_GuildArena_Season_Schedule"), textFormat);
			_textSeasonCount->SetText(FText::Format(textFormat, { battleArenaGroupData->SeasonNum(dungeonData) }));

			// 2023/11/20 PKT - {1}까지
			FText textSeasonEndDate = battleArenaGroupData->SeasonDateText(dungeonData);
			if (textSeasonEndDate.IsEmpty())
			{
				_textSeasonDate->SetVisibility(ESlateVisibility::Collapsed);
			}
			else
			{
				FGsDungeonHelper::FindText(TEXT("UI_GuildArena_Season_EndSchdule"), textFormat);
				_textSeasonDate->SetText(FText::Format(textFormat, { battleArenaGroupData->SeasonDateText(dungeonData) }));
				_textSeasonDate->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
			
		}
	}
	
	{	// 2023/11/20 PKT - 플레이 시간
		FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
		FGsContentsManagerGame* contents = (gameFlow) ? gameFlow->GetContentsManagerGame() : nullptr;
		
		if (FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud))
		{
			if (FGsBattleArenaGuildWarHandler* battleArenaHandler = hud->GetBattleArenaGuildWarHandler())
			{
				if (const FGsSchemaMapData* mapData = dungeonData->GetSchema()->mapId.GetRow())
				{
					const FGsSchemaGuildWarConfigData* guildWarData = battleArenaHandler->GetConfigData(mapData->id);
					_textPlayCycle->SetText(guildWarData ? guildWarData->playTimeText : FText::GetEmpty());
				}				
			}
		}
	}	

	{
		FText textFormat;
		FGsDungeonHelper::FindText(TEXT("UI_GuildArena_Gradeupper"), textFormat);

		FText textGuildGrade;
		FGsGuildHelper::GetMemberGradeText(battleArenaGroupData->ApplyLimitGuildGrade(dungeonData), textGuildGrade);
		
		_textRegisterLimitGuildGrade->SetText(FText::Format(textFormat, { textGuildGrade }));
	}

	InvalidateApplyButtonState();
}

void UGsUIPageDungeonGroupArena::Capture(FGsDungeonUICaptureData* OutCaptherData) const
{
	Super::Capture(OutCaptherData);
}