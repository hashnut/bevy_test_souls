

#include "GsUIPageDungeonGroupParty.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGame/GsDungeonManager.h"
#include "Management/ScopeGame/GsPartyManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"

#include "Party/GsPartyFunc.h"

#include "Dungeon/GsDungeonData.h"
#include "Dungeon/GsDungeonHelper.h"
#include "Dungeon/GsDungeonGroupBase.h"
#include "Dungeon/GsDungeonGroupParty.h"
#include "Dungeon/CoopPartyDungeon/GsCoopPartyDungeonGroupInfo.h"
#include "Dungeon/GsDungeonUICaptureData.h"

#include "../Message/MessageParam/GsItemContentsMessageParam.h"
#include "../Message/GsMessageContents.h"

#include "Reward/GsRewardHelper.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/GsContentsMode.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/Stage/StageGame/GsStageManagerGame.h"
#include "GameFlow/Stage/StageGame/GsStageGameDungeon.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateDungeon.h"
#include "GameFlow/GameContents/ContentsGame/Dungeon/GsDungeonStateHandler.h"
#include "GameFlow/GameContents/ContentsGame/Dungeon/GsDungeonStateBase.h"

#include "UTIL/GsGameObjectUtil.h"

#include "UI/UILib/Base/GsUIWidgetBase.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Popup/Guild/GuildDungeon/GsUIPopupGuildDungeonReward.h"
#include "UI/UIContent/Popup/GsUIPopupOK.h"
#include "UI/UIContent/Popup/GsUIPopupYesNoTwoMessage.h"
#include "UI/UIContent/Common/RedDot/GsUIRedDotDefault.h"
#include "UI/UIContent/Common/Entry/GsUIRewardItemIconListViewEntry.h"
#include "UI/UIContent/Window/Dungeon/CoopPartyDungeon/GsUIPartyDungeonPartyListViewEntry.h"
#include "UI/UIContent/Common/Tooltip/GsUITooltipDesc.h"
#include "UI/UIContent/Popup/GsUIPopupItemDetailMain.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"

#include "../Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperSprite.h"

#include "TextBlock.h"
#include "Image.h"
#include "ListView.h"
#include "WidgetSwitcher.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Engine/Classes/Engine/Texture2D.h"

#include "DataSchema/Item/GsSchemaItemData.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "DataSchema/Dungeon/GsSchemaDungeonGroupData.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "DataSchema/Reward/GsSchemaRewardData.h"
#include "DataSchema/Reward/GsSchemaRewardItemBagData.h"

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"

#include "Net/GsNetSendService.h"






void UGsUIPageDungeonGroupParty::BeginDestroy()
{
	if (_partyListViewEntryPool)
	{
		_partyListViewEntryPool->RemovePool();
		_partyListViewEntryPool = nullptr;
	}

	if (_rewardListViewEntryPool)
	{
		_rewardListViewEntryPool->RemovePool();
		_rewardListViewEntryPool = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIPageDungeonGroupParty::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_toggleGroupButton.AddToggleWidget(_switcherButtonDifficultyEasy);
	_toggleGroupButton.AddToggleWidget(_switcherButtonDifficultyNormal);
	_toggleGroupButton.AddToggleWidget(_switcherButtonDifficultyHard);
	_toggleGroupButton.AddToggleWidget(_switcherButtonDifficultyManiac);
	_toggleGroupButton.OnSelectChanged.BindUObject(this, &UGsUIPageDungeonGroupParty::OnSelectedDifficultyButton);

	_btnRefresh->OnClicked.AddDynamic(this, &UGsUIPageDungeonGroupParty::OnClickRefreshPartyList);
	_btnAutoMatching->OnClicked.AddDynamic(this, &UGsUIPageDungeonGroupParty::OnClickAutoMatching);
	_btnAutoMatchingCancel->OnClicked.AddDynamic(this, &UGsUIPageDungeonGroupParty::OnClickMatchingCancel);
	_btnPartyCreate->OnClicked.AddDynamic(this, &UGsUIPageDungeonGroupParty::OnClickCreateParty);
	_btnPartyQuit->OnClicked.AddDynamic(this, &UGsUIPageDungeonGroupParty::OnClickPartyLeave);
	_btnToolTipPartyList->OnClicked.AddDynamic(this, &UGsUIPageDungeonGroupParty::OnClickedPartyListToolTip);

	_basicRewardButton->OnClicked.AddDynamic(this, &UGsUIPageDungeonGroupParty::OnClickedShowBasicReward);
	_mainRewardButton->OnClicked.AddDynamic(this, &UGsUIPageDungeonGroupParty::OnClickedShowMainReward);

	_listViewPartyList->OnListViewScrolled().AddUObject(this, &UGsUIPageDungeonGroupParty::OnScrollPartyListView);

	//FText textName;
	//// 2022/06/03 PKT - 쉬움 컬러
	//FGsDungeonHelper::FindTextDifficulty(DungeonDifficulty::EASY, false, textName);
	//_textBtnEasyEnable->SetText(textName);
	//_textBtnEasyEnable->SetColorAndOpacity(FGsUIColorHelper::GetDungeonDifficultyType(DungeonDifficulty::EASY));
	//_textBtnEasyDisable->SetText(textName);	
	//_textBtnEasyDisable->SetColorAndOpacity(FGsUIColorHelper::GetDungeonDifficultyType(DungeonDifficulty::EASY));
	//// 2022/06/03 PKT - 일반 컬러
	//FGsDungeonHelper::FindTextDifficulty(DungeonDifficulty::NORMAL, false, textName);
	//_textBtnNormalEnable->SetText(textName);
	//_textBtnNormalEnable->SetColorAndOpacity(FGsUIColorHelper::GetDungeonDifficultyType(DungeonDifficulty::NORMAL));
	//_textBtnNormalDisable->SetText(textName);	
	//_textBtnNormalDisable->SetColorAndOpacity(FGsUIColorHelper::GetDungeonDifficultyType(DungeonDifficulty::NORMAL));
	//// 2022/06/03 PKT - 어려움 컬러
	//FGsDungeonHelper::FindTextDifficulty(DungeonDifficulty::DIFFICULT, false, textName);
	//_textBtnHardEnable->SetText(textName);
	//_textBtnHardEnable->SetColorAndOpacity(FGsUIColorHelper::GetDungeonDifficultyType(DungeonDifficulty::DIFFICULT));
	//_textBtnHardDisable->SetText(textName);
	//_textBtnHardDisable->SetColorAndOpacity(FGsUIColorHelper::GetDungeonDifficultyType(DungeonDifficulty::DIFFICULT));
	//	// 2022/06/03 PKT - 극악 컬러
	//FGsDungeonHelper::FindTextDifficulty(DungeonDifficulty::NIGHTMARE, false, textName);
	//_textBtnNightmareEnable->SetText(textName);
	//_textBtnNightmareEnable->SetColorAndOpacity(FGsUIColorHelper::GetDungeonDifficultyType(DungeonDifficulty::NIGHTMARE));
	//_textBtnNightmareDisable->SetText(textName);
	//_textBtnNightmareDisable->SetColorAndOpacity(FGsUIColorHelper::GetDungeonDifficultyType(DungeonDifficulty::NIGHTMARE));

	const static int32 POOL_WARM_UP_SIZE = 20;
	_partyListViewEntryPool = NewObject<UGsPoolUObject>(this);
	_partyListViewEntryPool->WarmUp<UGsPartyDungeonPartyListViewEntryData>(POOL_WARM_UP_SIZE);

	
	_rewardListViewEntryPool = NewObject<UGsPoolUObject>(this);
	_rewardListViewEntryPool->WarmUp<UGsUIRewardIconListViewEntryData>(POOL_WARM_UP_SIZE);
}

void UGsUIPageDungeonGroupParty::NativeConstruct()
{
	Super::NativeConstruct();

	FText message;
	FGsDungeonHelper::FindText(TEXT("PartyDungeon_PartyListGuideText"), message);
	_tooltipPartyListInfo->SetDescText(message);
	_tooltipPartyListInfo->Close();

	// 2023/4/11 PKT - Icon 생성
	_iconDungeonKey.Reset();
	_iconDungeonKey = Cast<UGsUIIconItemInventory>(_iconSelector->CreateIcon(EGsIconType::ICON_ITEM_INVENTORY_XL, 0));
	if (_iconDungeonKey.IsValid())
	{
		_iconSelector->AddChild(_iconDungeonKey.Get());
		_iconDungeonKey->SetItemNameToolTipFlag(true);
		_iconDungeonKey->SetOnChangeDisplayDetailWndType(UGsUIIconItemInventory::UseDisplayDetailWndType::LongPressType);

		if (false == _iconDungeonKey->OnFinishedLongPressEvent.IsBoundToObject(this))
		{
			_iconDungeonKey->OnFinishedLongPressEvent.AddUObject(this, &UGsUIPageDungeonGroupParty::OnLongPressIconChargeItem);
		}
	}
}

void UGsUIPageDungeonGroupParty::NativeDestruct()
{
	_listViewPartyList->ClearListItems();
	_partyListViewEntryPool->ReleaseAll();

	_defaultRewardListView->ClearListItems();
	_mainRewardListView->ClearListItems();
	_rewardListViewEntryPool->ReleaseAll();

	if (_iconSelector)
	{
		_iconSelector->RemoveAllChildren();
	}

	_iconDungeonKey.Reset();

	Super::NativeDestruct();
}

void UGsUIPageDungeonGroupParty::SetStateSideTabEnable(const FGsDungeonGroupParty* InData)
{
	if (nullptr == InData)
	{
		return;
	}

	// 2022/08/18 PKT - 난이도 버튼 Enable 설정
	const FGsDungeonData* dungeonData = nullptr;
	dungeonData = InData->FindDungeonByDifficulty(DungeonDifficulty::EASY);
	if (dungeonData)
	{
		_imgLockDifficultyEasy->SetVisibility(dungeonData->IsUnLockState() ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
	}

	dungeonData = InData->FindDungeonByDifficulty(DungeonDifficulty::NORMAL);
	if (dungeonData)
	{
		_imgLockDifficultyNormal->SetVisibility(dungeonData->IsUnLockState() ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
	}

	dungeonData = InData->FindDungeonByDifficulty(DungeonDifficulty::DIFFICULT);
	if (dungeonData)
	{
		_imgLockDifficultyHard->SetVisibility(dungeonData->IsUnLockState() ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
	}

	dungeonData = InData->FindDungeonByDifficulty(DungeonDifficulty::NIGHTMARE);
	if (dungeonData)
	{
		_imgLockDifficultyNaghtmare->SetVisibility(dungeonData->IsUnLockState() ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
	}
}

void UGsUIPageDungeonGroupParty::SetStateControlButton(const FGsDungeonGroupParty* InData)
{
	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	FGsContentsManagerGame* contents = (gameFlow) ? gameFlow->GetContentsManagerGame() : nullptr;
	if (nullptr == contents)
	{
		GSLOG(Error, TEXT("nullptr == contents"));
		return;
	}

	FGsGameStateDungeon* stateDungeon = contents->GetContentsStateObject<FGsGameStateDungeon>(FGsContentsMode::InGame::ContentsDungeon);
	if (nullptr == stateDungeon)
	{
		GSLOG(Error, TEXT("nullptr == stateDungeon"));
		return;
	}

	const FGsDungeonStateHandler* dungeonStateHandler = stateDungeon->StateHandler();
	const FGsDungeonStateBase* dungeonState
		= (nullptr != dungeonStateHandler) ? dungeonStateHandler->GetState(DungeonCategory::PARTY) : nullptr;
	if (nullptr == dungeonState)
	{
		GSLOG(Error, TEXT("nullptr == dungeonState"));
		return;
	}
	
	const static int32 SWITCHER_INDEX_AUTO_MATCHING = 0;
	const static int32 SWITCHER_INDEX_MATCHING_CANCEL = 1;

	if (EDungeonProgressState::EMatching == dungeonState->GetProgressState())
	{	
		// 2022/06/14 PKT - 자동 매칭
		_swithcerMatchingBtn->SetActiveWidgetIndex(SWITCHER_INDEX_MATCHING_CANCEL);
	}
	else
	{
		// 2022/06/14 PKT - 자동 매칭
		_swithcerMatchingBtn->SetActiveWidgetIndex(SWITCHER_INDEX_AUTO_MATCHING);
	}

	const static int32 SWITCHER_INDEX_CREATE_PARTY = 0;
	const static int32 SWITCHER_INDEX_LEAVE_PARTY = 1;

	if (EDungeonProgressState::EJoinParty == dungeonState->GetProgressState()
		|| EDungeonProgressState::EReady == dungeonState->GetProgressState())
	{	
		// 2022/06/07 PKT - 파티 던전을 목적으로 형성된 파티라면 탈퇴를 활성화
		_switcherPartyButton->SetActiveWidgetIndex(SWITCHER_INDEX_LEAVE_PARTY);

		auto widget = _switcherPartyButton->GetActiveWidget();
		if (widget)
		{
			// 2023/2/15 PKT - 준비 상태 일때는 버튼 을 비활성화
			widget->SetIsEnabled(EDungeonProgressState::EReady != dungeonState->GetProgressState());
		}
	}
	else
	{	// 2022/06/19 PKT - 파티 던전을 목적으로 파티 상태가 아니라면 파티 생성 활성화
		_switcherPartyButton->SetActiveWidgetIndex(SWITCHER_INDEX_CREATE_PARTY);
	}

	DungeonDifficulty findDifficulty = StaticCast<DungeonDifficulty>(_toggleGroupButton.GetSelectedIndex() + 1);
	const FGsDungeonData* dungeonData = InData->FindDungeonByDifficulty(findDifficulty);
	bool isEnable = (dungeonData && true == dungeonData->IsUnLockState());

	_btnAutoMatching->SetIsEnabled(isEnable);
	_btnPartyCreate->SetIsEnabled(isEnable);
}

void UGsUIPageDungeonGroupParty::SetStateRewardInfo(const FGsDungeonGroupParty* InData)
{
	FText message;
	// 2022/06/10 PKT - "{0}회"
	FGsDungeonHelper::FindText(TEXT("rewardCountSign"), message);

	int32 weekRewardCount = InData->GetDefaultHaveRewardCount();
	// 2022/08/18 PKT - 주간 보상 가능 횟수
	_defaultRewardCount->SetText(FText::Format(message, weekRewardCount));
	// 2022/08/18 PKT - 보상 가능한 아이템 보유 갯수
	_itemRewardCount->SetText(FText::Format(message, InData->GetDungeonKeyAmount()));
	if (0 < InData->GetDungeonKeyAmount())
	{
		_itemRewardCount->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::DEFAULT_VALID));
	}
	else
	{
		_itemRewardCount->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::DEFAULT_LACK));
	}

	// 2022/06/02 PKT - RedDot	
	const static int32 ON_REDDOT_COUNT = 1;
	bool isRedDot = false;
	if (InData->IsUnLockContents() && ON_REDDOT_COUNT <= weekRewardCount)
	{	// 2022/08/18 PKT - 레드닷 조건이 성립 할때 난이도가 입장 가능 한 경우 레드닷 On
		DungeonDifficulty findDifficulty = StaticCast<DungeonDifficulty>(_toggleGroupButton.GetSelectedIndex() + 1);
		const FGsDungeonData* dungeonData = InData->FindDungeonByDifficulty(findDifficulty);
		if (dungeonData && true == dungeonData->IsUnLockState())
		{
			isRedDot = true;
		}
	}
	_rewardRedDot->SetRedDot(isRedDot, EGsIconRedDotType::NORMAL);
}

void UGsUIPageDungeonGroupParty::SetDungeonDataDisplay(const FGsDungeonGroupParty* InDungeonGroupParty, int32 InIndex)
{
	if (nullptr == InDungeonGroupParty)
	{
		return;
	}

	DungeonDifficulty findDifficultyType = StaticCast<DungeonDifficulty>(InIndex + 1);
	const FGsDungeonData* dungeonData = InDungeonGroupParty->FindDungeonByDifficulty(findDifficultyType);
	if (nullptr == dungeonData)
	{
		GSLOG(Error, TEXT("nullptr == dungeonData"));
		return;
	}

	const FGsSchemaDungeonData* schemaDungeonData = dungeonData->GetSchema();

	/**
	 * 1. 2022/06/01 PKT - 종족/속성/재질
	 */
	const FGsSchemaNpcData* bossData = schemaDungeonData->bossId.GetRow();
	// 2022/06/01 PKT - 종족
	_raceTextBlock->SetText(UGsGameObjectUtil::GetRaceText(bossData->race));
	_materialTextBlock->SetText(UGsGameObjectUtil::GetRaceWeaknessItemMaterialText(bossData->race));
	_traitTextBlock->SetText(UGsGameObjectUtil::GetMonsterElementalText(bossData->element));


	/**
	 * 2. 2022/06/01 PKT - 던전 제한 시간
	 */
	 // 2022/06/01 PKT - 
	FText message;
	FGsDungeonHelper::FindText(TEXT("PartyDungeon_TimeLimit"), message);
	_textTimeLimit->SetText(FText::Format(message, schemaDungeonData->timeLimitM));


	/**
	 * 3. 2022/06/01 PKT - Reward Data
	 */
	auto BuildRewardViewList
		= [this](const TArray<FGsSchemaRewardDataRow>& rewardDataRow
			, TArray<const FGsSchemaRewardItemBagData*>& OutRewardItemBagDataSet
			, UListView** rewardListView)
	{
		OutRewardItemBagDataSet.Empty();

		// 2023/2/15 PKT - 보상 정보가 복수로 들어가 있는 경우도 있음.
		for (const FGsSchemaRewardDataRow& row : rewardDataRow)
		{
			FGsRewardUnboxingData rewardUnboxingData;
			if (row.GetRow() && true == FGsRewardHelper::UnboxingRewardData(row.GetRow()->id, rewardUnboxingData))
			{
				for (const auto& itemBagData : rewardUnboxingData._itemDataList)
				{
					OutRewardItemBagDataSet.Emplace(itemBagData);
				}
			}
		}

		TArray<UGsUIRewardIconListViewEntryData*> rewardEntrySet;
		FGsUIRewardIconData rewardIconData;
		for (const auto& item : OutRewardItemBagDataSet)
		{
			UGsUIRewardIconListViewEntryData* viewEntryData = _rewardListViewEntryPool->Claim<UGsUIRewardIconListViewEntryData>();
			viewEntryData->_rewardItemDataArray.Empty();
			if (CurrencyType::NONE == item->currencyType)
			{
				rewardIconData.Init(item, item->itemCountMin, item->itemCountMax);
			}
			else
			{
				rewardIconData.Init(item, item->currencyAmountMin, item->currencyAmountMax);
			}
			
			viewEntryData->_rewardItemDataArray.Add(rewardIconData);

			// 2023/2/15 PKT - .. Add
			rewardEntrySet.Add(viewEntryData);
		}

		(*rewardListView)->SetListItems(rewardEntrySet);
		(*rewardListView)->RegenerateAllEntries();
		(*rewardListView)->SetScrollbarVisibility(ESlateVisibility::Collapsed);
		(*rewardListView)->ScrollIndexIntoView(0);
	};

	_rewardListViewEntryPool->ReleaseAll();

	// 2022/06/02 PKT - 기본 보상
	_basicRewardItemBagDataSet.Empty();
	BuildRewardViewList(schemaDungeonData->basicRewardList, _basicRewardItemBagDataSet, &_defaultRewardListView);

	// 2022/06/02 PKT - 메인 보상
	_mainRewardItemBagDataSet.Empty();
	BuildRewardViewList(schemaDungeonData->mainRewardList, _mainRewardItemBagDataSet, &_mainRewardListView);
}

bool UGsUIPageDungeonGroupParty::CheckInDungeonPossible()
{
	if (nullptr == _data)
	{
		return false;
	}

	const MapType mapType = GLevel()->GetCurrentMapType();
	if (MapType::INSTANCE_FIELD == mapType 
		|| MapType::PUBLIC_DUNGEON == mapType 
		|| MapType::GUILD_DUNGEON == mapType 
		|| MapType::ELITE_DUNGEON_1 == mapType
		|| MapType::PARTY_DUNGEON == mapType)
	{
		// 2022/06/10 PKT - "파티 던전에 입장할 수 없는 지역입니다."
		FText message;
		FGsDungeonHelper::FindText(TEXT("Notice_Cannot_Join_WrongArea"), message);
		FGsUIHelper::TrayMessageTicker(message);
		return false;
	}

	DungeonDifficulty currentDifficulty = StaticCast<DungeonDifficulty>(_toggleGroupButton.GetSelectedIndex() + 1);

	const FGsDungeonGroupParty* partyDungeonGroup = StaticCast<const FGsDungeonGroupParty*>(_data);
	if (const FGsDungeonData* data = partyDungeonGroup->FindDungeonByDifficulty(currentDifficulty))
	{
		if (true == data->IsUnLockState())
		{
			return true;
		}

		TWeakObjectPtr<UGsUIPopupOK> widget = Cast<UGsUIPopupOK>(GUI()->OpenAndGetWidget("PopupOK"));
		if (widget.IsValid())
		{
			// 2022/06/10 PKT - "해당 난이도의 던전을 이용할 수 없습니다."
			FText message;
			FGsDungeonHelper::FindText(TEXT("PartyDungeon_Notice_Cannot_ThisLevel"), message);
			widget.Get()->SetData(message);
		}
	}
	return false;
}

void UGsUIPageDungeonGroupParty::RequestRefreshPartyList()
{
	if (_data)
	{	// 2022/06/01 PKT - 서버에 파티 목록 리스트를 요청한다.
		FGsNetSendService::SendReqPartyDungeonpartyList(_data->GetGroupId());
	}
}

void UGsUIPageDungeonGroupParty::Enter(const FGsDungeonGroupBase* InData, const FGsDungeonUICaptureData& InCaptureData)
{
	Super::Enter(InData, InCaptureData);

	const FGsDungeonGroupParty* partyDungeonGroup = StaticCast<const FGsDungeonGroupParty*>(_data);
	const FGsSchemaDungeonGroupData* schemaData = _data->GetSchemaData();

	// 2022/06/01 PKT - Dungeon Title & Desc
	_textDungeonName->SetText(schemaData->dungeonGroupName);
	_textDesc->SetText(schemaData->dungeonGroupDesc);

	// 던전 대표 이미지
	UTexture2D* loadedTexture = FGsUIHelper::LoadTextureFromPath(schemaData->dungeonGroupTexutreImg);
	if (loadedTexture)
	{
		_imgDungeonGroup->SetBrushFromTexture(loadedTexture);
	}

	if (_iconDungeonKey.IsValid())
	{
		if (const FGsSchemaItemCommon* itemCommonData = schemaData->dungeonKey.GetRow())
		{
			_iconDungeonKey->SetNotOwnItem(itemCommonData->id, 0);
			_iconDungeonKey->SetItemNameVisibility(false);
			_iconDungeonKey->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			_iconDungeonKey->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	int32 currentIndex = InCaptureData._value0;
	if (0 != InCaptureData._mapId)
	{
		if (const FGsDungeonData* dungeonData = _data->FindDungeonByMapId(InCaptureData._mapId))
		{
			currentIndex = StaticCast<int32>(dungeonData->GetSchema()->levelOfDifficulty) - 1;
		}
	}
	else if (0 > currentIndex)
	{
		// 2023/2/22 PKT - None을 포함 하고 있기 때문에 -1
		currentIndex = FMath::Max(StaticCast<int32>(partyDungeonGroup->DifficultyToEnter()) - 1, 0);
	}
	// 2022/06/02 PKT - Seletected Dungeon Difficulty
	_toggleGroupButton.SetSelectedIndex(currentIndex, true);

	// 2022/06/07 PKT - 파티 목록 기본 상태 "파티 목록 없음"
	InvalidatePartyInfoSet(nullptr);

	// 2022/06/01 PKT - 서버에 파티 목록 리스트를 요청한다.
	RequestRefreshPartyList();

	Invalidate();
}

void UGsUIPageDungeonGroupParty::Invalidate()
{
	Super::Invalidate();

	if (nullptr == _data)
	{
		return;
	}

	const FGsDungeonGroupParty* partyDungeonGroup = StaticCast<const FGsDungeonGroupParty*>(_data);
	// 2023/2/22 PKT - 난이도 탭 활성화 여부
	SetStateSideTabEnable(partyDungeonGroup);
	// 2023/2/23 PKT - 하단 매칭/파티 생성 등 버튼 설정
	SetStateControlButton(partyDungeonGroup);
	// 2023/2/23 PKT - 보상 정보 및 레드닷
	SetStateRewardInfo(partyDungeonGroup);

	if (_iconDungeonKey.IsValid())
	{
		_iconDungeonKey->SetItemCount(partyDungeonGroup->GetDungeonKeyAmount());
	}
}

void UGsUIPageDungeonGroupParty::InvalidatePartyInfoSet(const TArray<FGsPartyDungeonPartyStatusList>* InPartyStatusList)
{
	if (nullptr == InPartyStatusList || 0 >= (*InPartyStatusList).Num())
	{
		const static int32 EMPTY_PARTY_LIST = 1;
		_switcherPartyList->SetActiveWidgetIndex(EMPTY_PARTY_LIST);

		_listViewPartyList->ClearListItems();
		_partyListViewEntryPool->ReleaseAll();
	}
	else
	{
		const static int32 SHOW_PARTY_LIST = 0;
		_switcherPartyList->SetActiveWidgetIndex(SHOW_PARTY_LIST);

		TArray<UGsPartyDungeonPartyListViewEntryData*> entryDataSet;
		for (const FGsPartyDungeonPartyStatusList& partyStatus : (*InPartyStatusList))
		{
			UGsPartyDungeonPartyListViewEntryData* data = _partyListViewEntryPool->Claim<UGsPartyDungeonPartyListViewEntryData>();
			data->SetData(partyStatus);
			data->OnJoin = [=](PartyId InPartyId) { this->OnGoToParty(InPartyId); };
			data->OnIntrude = [=](PartyId InPartyId) { this->OnIntrude(InPartyId); };
			entryDataSet.Add(data);
		}

		_listViewPartyList->SetListItems(entryDataSet);
		_listViewPartyList->RegenerateAllEntries();
		_listViewPartyList->SetScrollbarVisibility(ESlateVisibility::Collapsed);
		_listViewPartyList->SetScrollOffset(_lastPartyViewScrollOffset);
	}
}

void UGsUIPageDungeonGroupParty::Capture(FGsDungeonUICaptureData* OutCaptherData) const
{
	OutCaptherData->_scrollBoxListOffset = _lastPartyViewScrollOffset;
	OutCaptherData->_value0 = _toggleGroupButton.GetSelectedIndex();
}

void UGsUIPageDungeonGroupParty::OnSelectedDifficultyButton(int32 InIndex)
{	// 2022/06/01 PKT - 던전 난이도에 따른 화면 갱신
	if (nullptr == _data)
	{
		return;
	}

	const FGsDungeonGroupParty* partyDungeonGroup = StaticCast<const FGsDungeonGroupParty*>(_data);

	SetDungeonDataDisplay(partyDungeonGroup, InIndex);

	SetStateRewardInfo(partyDungeonGroup);

	SetStateControlButton(partyDungeonGroup);
}

void UGsUIPageDungeonGroupParty::OnClickRefreshPartyList()
{
	RequestRefreshPartyList();
}

void UGsUIPageDungeonGroupParty::OnClickAutoMatching()
{	
	// 2022/06/07 PKT - 자동 매칭
	if (nullptr == _data)
	{
		GSLOG(Error, TEXT("nullptr == _data"));
		return;
	}

	// 2022/06/10 PKT - 신청자(로컬 유저)가 입장 가능한 레벨인지 체크
	if (false == CheckInDungeonPossible())
	{
		return;
	}

	TWeakObjectPtr<UGsUIPopupYesNoTwoMessage> widget = Cast<UGsUIPopupYesNoTwoMessage>(GUI()->OpenAndGetWidget(TEXT("PopupYesNoTwoMessage")));
	if (widget.IsValid())
	{
		/**
		 * 현재 선택한 난이도는 {0}입니다. \r\n협동 파티 자동 찾기를 하시겠습니까?
		 */
		FText noticeMessage;
		FGsDungeonHelper::FindText(TEXT("PartyDungeon_PopUp_AutoMatching"), noticeMessage);

		DungeonDifficulty selectedDifficulty = StaticCast<DungeonDifficulty>(_toggleGroupButton.GetSelectedIndex() + 1);
			
		FText textDifficulty;
		FGsDungeonHelper::FindTextDifficulty(selectedDifficulty, true, textDifficulty);
		noticeMessage = FText::Format(noticeMessage, textDifficulty);

		const FGsDungeonGroupParty* partyDungeonGroup = StaticCast<const FGsDungeonGroupParty*>(_data);
		if (partyDungeonGroup && 0 >= (partyDungeonGroup->GetDefaultHaveRewardCount() + partyDungeonGroup->GetDungeonKeyAmount()))
		{
			/**
			* 보상 획득 가능 횟수가 부족하여 클리어 보상을 획득할 수 없습니다. \r\n입장 하시겠습니까?
			*/
			FText message;
			FGsDungeonHelper::FindText(TEXT("PartyDungeon_PopUp_CannotReward"), message);

			FString str = noticeMessage.ToString() + TEXT("\r\n") + message.ToString();
			noticeMessage = FText::FromString(str);
		}		

		/**
		 * 파티에 참여 중인 경우에는 기존 파티를 탈퇴합니다.
		 */
		FText commonMessage;
		FGsDungeonHelper::FindText(TEXT("PartyDungeon_PopUp_AutoMatching2"), commonMessage);

		DungeonGroupId InGroupId = _data->GetGroupId();
		DungeonDifficulty difficulty = selectedDifficulty;
		TFunction<void(bool)> InCallback = [InGroupId, difficulty](bool InIsOk)
		{
			if (false == InIsOk)
			{
				return;
			}

			// 2022/06/07 PKT - 오토 매칭 신청
			FGsNetSendService::SendPartyDungeonAutoMatching(InGroupId, difficulty);
		};
		widget.Get()->SetData(noticeMessage, commonMessage, InCallback);
	}
}

void UGsUIPageDungeonGroupParty::OnClickMatchingCancel()
{
	FGsNetSendService::SendPartyDungeonAutoMatchingCancel();
}

void UGsUIPageDungeonGroupParty::OnClickCreateParty()
{	// 2022/06/01 PKT - 파티 생성
	// 2022/06/07 PKT - 자동 매칭
	if (nullptr == _data)
	{
		GSLOG(Error, TEXT("nullptr == _data"));
		return;
	}

	// 2022/06/10 PKT - 신청자(로컬 유저)가 입장 가능한 레벨인지 체크
	if (false == CheckInDungeonPossible())
	{
		return;
	}

	TWeakObjectPtr<UGsUIPopupYesNoTwoMessage> widget = Cast<UGsUIPopupYesNoTwoMessage>(GUI()->OpenAndGetWidget(TEXT("PopupYesNoTwoMessage")));
	if (widget.IsValid())
	{
		DungeonDifficulty selectedDifficulty = StaticCast<DungeonDifficulty>(_toggleGroupButton.GetSelectedIndex() + 1);

		// 2022/06/07 PKT - "현재 선택한 난이도는 {0}입니다."
		FText textDifficulty;
		FGsDungeonHelper::FindTextDifficulty(selectedDifficulty, true, textDifficulty);

		FText findText;
		FGsDungeonHelper::FindText(TEXT("PartyDungeon_PopUp_PartyCreate"), findText);
		FText noticeMessage = FText::Format(findText, textDifficulty);

		const FGsDungeonGroupParty* partyDungeonGroup = StaticCast<const FGsDungeonGroupParty*>(_data);
		if (partyDungeonGroup && 0 >= (partyDungeonGroup->GetDefaultHaveRewardCount() + partyDungeonGroup->GetDungeonKeyAmount()))
		{
			/**
			* 보상 획득 가능 횟수가 부족하여 클리어 보상을 획득할 수 없습니다. \r\n입장 하시겠습니까?
			*/
			FText message;
			FGsDungeonHelper::FindText(TEXT("PartyDungeon_PopUp_CannotReward"), message);

			FString str = noticeMessage.ToString() + TEXT("\r\n") + message.ToString();
			noticeMessage = FText::FromString(str);
		}

		// 2022/06/07 PKT - "공개 파티를 생성하겠습니까?."
		FText commonMessage = FText::GetEmpty();
		FGsDungeonHelper::FindText(TEXT("PartyDungeon_PopUp_PartyCreate2"), commonMessage);

		DungeonGroupId InGroupId = _data->GetGroupId();
		DungeonDifficulty difficulty = selectedDifficulty;
		TFunction<void(bool)> InCallback = [InGroupId, difficulty](bool InIsOk)
		{
			if (false == InIsOk)
			{
				return;
			}
			// 2022/06/07 PKT - 방 생성
			FGsNetSendService::SendReqPartyDungeonCreateParty(InGroupId, difficulty);
		};

		widget->SetData(noticeMessage, commonMessage, InCallback);
	}
}

void UGsUIPageDungeonGroupParty::OnClickPartyLeave()
{
	// 2022/06/01 PKT - 파티 탈퇴
	auto refreshList = [&]()
	{
		this->RequestRefreshPartyList();
	};

	FGsPartyFunc::OpenLeavePopup(refreshList);
}

void UGsUIPageDungeonGroupParty::OnScrollPartyListView(float InOffsetInItems, float InDistanceRemaining)
{
	_lastPartyViewScrollOffset = InOffsetInItems;
}

void UGsUIPageDungeonGroupParty::OnClickedShowBasicReward()
{
	TWeakObjectPtr<UGsUIPopupGuildDungeonReward> widget = Cast<UGsUIPopupGuildDungeonReward>(GUI()->OpenAndGetWidget("PopupGuildDungeonReward"));
	if (widget.IsValid())
	{
		FText title;
		FGsDungeonHelper::FindText(TEXT("PartyDungeon_BasicReward"), title);	
		widget.Get()->SetData(title, _basicRewardItemBagDataSet);
	}
}

void UGsUIPageDungeonGroupParty::OnClickedShowMainReward()
{
	TWeakObjectPtr<UGsUIPopupGuildDungeonReward> widget = Cast<UGsUIPopupGuildDungeonReward>(GUI()->OpenAndGetWidget("PopupGuildDungeonReward"));
	if (widget.IsValid())
	{
		FText title;
		FGsDungeonHelper::FindText(TEXT("PartyDungeon_MainReward"), title);
		widget.Get()->SetData(title, _mainRewardItemBagDataSet);
	}
}

void UGsUIPageDungeonGroupParty::OnClickedPartyListToolTip()
{
	_tooltipPartyListInfo->ToggleOpenClose();	
}

void UGsUIPageDungeonGroupParty::OnLongPressIconChargeItem(UGsUIIconItem& InIcon)
{
	FGsItem* itemData = InIcon.GetItemIconData();
	if (itemData)
	{
		const FGsItemDetailOpenMessageParamTID param(itemData->GetTID(), true, false, UGsUIPopupItemDetailMain::CenterPosType::CenterPos);
		GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM_TID, &param);
	}
}

void UGsUIPageDungeonGroupParty::OnGoToParty(PartyId InPartyId)
{
	// 2022/06/10 PKT - 신청자(로컬 유저)가 입장 가능한 레벨인지 체크
	if (false == CheckInDungeonPossible())
	{
		return;
	}

	// 2022/06/07 PKT - 안내 팝업창을 띄운다.
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupYesNoTwoMessage"));
	if (!widget.IsValid())
	{
		GSLOG(Error, TEXT("!widget.IsValid()"));
		return;
	}

	if (UGsUIPopupYesNoTwoMessage* popup = Cast<UGsUIPopupYesNoTwoMessage>(widget.Get()))
	{	// 2022/06/07 PKT - "해당 협동 파티에 참여 하시겠습니까? 기존 속해 있는 파티는 탈퇴합니다."
		FText noticeMessage;
		FText::FindText(TEXT("PartyDungeonText"), TEXT("PopUp_Join_inParty"), noticeMessage);

		const FGsDungeonGroupParty* partyDungeonGroup = StaticCast<const FGsDungeonGroupParty*>(_data);
		if (partyDungeonGroup && 0 >= (partyDungeonGroup->GetDefaultHaveRewardCount() + partyDungeonGroup->GetDungeonKeyAmount()))
		{
			/**
			* 보상 획득 가능 횟수가 부족하여 클리어 보상을 획득할 수 없습니다. \r\n입장 하시겠습니까?
			*/
			FText message;
			FGsDungeonHelper::FindText(TEXT("PartyDungeon_PopUp_CannotReward"), message);

			FString str = noticeMessage.ToString() + TEXT("\r\n") + message.ToString();
			noticeMessage = FText::FromString(str);
		}

		FText commonMessage;
		FText::FindText(TEXT("PartyDungeonText"), TEXT("PopUp_Join_inParty2"), commonMessage);

		TFunction<void(bool)> InCallback = [InPartyId](bool InIsOk)
		{
			if (false == InIsOk)
			{
				return;
			}

			// send..
			if (INVALID_PARTY_ID != InPartyId)
			{	// 2022/06/07 PKT - 보내기..
				FGsNetSendService::SendPartyDungeonJoin(InPartyId);
			}
		};
		popup->SetData(noticeMessage, commonMessage, InCallback);
	}
}

void UGsUIPageDungeonGroupParty::OnIntrude(PartyId InPartyId)
{
	// 2022/06/10 PKT - 신청자(로컬 유저)가 입장 가능한 레벨인지 체크
	if (false == CheckInDungeonPossible())
	{
		return;
	}

	// 2022/06/07 PKT - 안내 팝업창을 띄운다.
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupYesNoTwoMessage"));
	if (!widget.IsValid())
	{
		GSLOG(Error, TEXT("!widget.IsValid()"));
		return;
	}

	if (UGsUIPopupYesNoTwoMessage* popup = Cast<UGsUIPopupYesNoTwoMessage>(widget.Get()))
	{	// 2022/06/07 PKT - "해당 협동 파티에 중도 참여 하시겠습니까?"
		FText noticeMessage;
		FText::FindText(TEXT("PartyDungeonText"), TEXT("PopUp_BreakInto"), noticeMessage);

		const FGsDungeonGroupParty* partyDungeonGroup = StaticCast<const FGsDungeonGroupParty*>(_data);
		if (partyDungeonGroup && 0 >= (partyDungeonGroup->GetDefaultHaveRewardCount() + partyDungeonGroup->GetDungeonKeyAmount()))
		{
			/**
			* 보상 획득 가능 횟수가 부족하여 클리어 보상을 획득할 수 없습니다. \r\n입장 하시겠습니까?
			*/
			FText message;
			FGsDungeonHelper::FindText(TEXT("PartyDungeon_PopUp_CannotReward"), message);

			FString str = noticeMessage.ToString() + TEXT("\r\n") + message.ToString();
			noticeMessage = FText::FromString(str);
		}

		FText commonMessage = FText::GetEmpty();

		TFunction<void(bool)> InCallback = [InPartyId](bool InIsOk)
		{
			if (false == InIsOk)
			{
				return;
			}

			// send..
			if (INVALID_PARTY_ID != InPartyId)
			{	// 2022/06/07 PKT - 보내기..
				FGsNetSendService::SendPartyDungeonBreakInto(InPartyId);
			}
		};

		popup->SetData(noticeMessage, commonMessage, InCallback);
	}	
}