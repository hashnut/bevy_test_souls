#include "GsUIPageDungeonGroupCommon.h"

#include "GsUIDungeonSlotEntry.h"

#include "Item/GsItem.h"
#include "Item/GsItemManager.h"
#include "Unlock/GsUnlockDefine.h"
#include "Dungeon/GsDungeonData.h"
#include "Dungeon/GsDungeonHelper.h"
#include "Dungeon/GsDungeonGroupBase.h"
#include "Dungeon/GsDungeonGroupNormal.h"
#include "Dungeon/GsDungeonUICaptureData.h"

#include "Message/GsMessageContents.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "Management/ScopeGame/GsDungeonManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"

#include "Message/MessageParam/GsMessageParam.h"

#include "UTIL/GsCronUtil.h"

#include "UI/UILib/Base/GsUIWidgetBase.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "UI/UIContent/Common/Tooltip/GsUITooltipDesc.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Common/Icon/GsUIIconAbnormality.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemReward.h"
#include "UI/UIContent/Common/GsUICurrencyButton.h"
#include "UI/UIContent/Common/Entry/GsUIRewardItemIconListViewEntry.h"
#include "UI/UIContent/Popup/GsUIPopupAbnormalityDetailInfo.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UMG/Public/Components/ListView.h"
#include "UMG/Public/Components/PanelWidget.h"

#include "../GameFlow/GsGameFlowGame.h"
#include "../GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "../GameFlow/GameContents/ContentsGame/GsGameStateRegionMap.h"
#include "../GameFlow/GameContents/GsContentsMode.h"

#include "UnrealString.h"
#include "UserWidget.h"
#include "ScrollBox.h"
#include "TextBlock.h"
#include "WidgetSwitcher.h"
#include "Components/Image.h"
#include "PaperSprite.h"
#include "VerticalBox.h"
#include "Reward/GsRewardHelper.h"

#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "DataSchema/GsSchemaResetTimeTicketConfig.h"
#include "DataSchema/Unlock/GsSchemaUnlockContents.h"
#include "DataSchema/Dungeon/GsSchemaDungeonGroupData.h"
#include "DataSchema/Dungeon/GsSchemaDungeonData.h"
#include "DataSchema/Reward/GsSchemaRewardItemBagData.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"

#include "../../Common/Tooltip/GsUITooltipDesc.h"
#include "../../Popup/GsUIPopupItemDetailMain.h"
#include "UMG/Public/Components/HorizontalBox.h"

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"

#include "T1Project.h"
#include "../Message/MessageParam/GsItemContentsMessageParam.h"

#include "Map/WorldMap/GsWorldMapFunc.h"




void UGsUIPageDungeonGroupCommon::BeginDestroy()
{
	if (_scrollBoxHelperDungeonSet)
	{
		_scrollBoxHelperDungeonSet->OnRefreshEntry.RemoveDynamic(this, &UGsUIPageDungeonGroupCommon::OnRefreshDungeonSetEntry);
	}
	_scrollBoxHelperDungeonSet = nullptr;

	if (_rewardListViewEntryPool)
	{
		_rewardListViewEntryPool->RemovePool();
		_rewardListViewEntryPool = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIPageDungeonGroupCommon::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnGoWorldMap->OnClicked.AddDynamic(this, &UGsUIPageDungeonGroupCommon::OnClickedGoWorldMap);

	_btnCharge->OnClicked.AddDynamic(this, &UGsUIPageDungeonGroupCommon::OnClickedCharge);

	_btnResetTooltip->OnClicked.AddDynamic(this, &UGsUIPageDungeonGroupCommon::OnClickedResetTooltip);

	_btnEliteRewardTooltip->OnClicked.AddDynamic(this, &UGsUIPageDungeonGroupCommon::OnClickedEliteRewardTooltip);

	_btnTryEntered->OnClickCurrencyButton.BindUObject(this, &UGsUIPageDungeonGroupCommon::OnClickedGoTryToDungeon);

	_toggleGroupDungeonSet.OnSelectChanged.BindUObject(this, &UGsUIPageDungeonGroupCommon::OnSelectedDungeon);

	// 스크롤 세팅
	_scrollBoxHelperDungeonSet = NewObject<UGsDynamicPanelSlotHelper>(this);
	_scrollBoxHelperDungeonSet->Initialize(_entryWidgetDungeonSetClass, _scrollBoxDungeonSet);
	_scrollBoxHelperDungeonSet->OnRefreshEntry.AddDynamic(this, &UGsUIPageDungeonGroupCommon::OnRefreshDungeonSetEntry);

	// 2023/4/11 PKT - Icon 생성
	

	const static int32 POOL_WARM_UP_SIZE = 20;
	_rewardListViewEntryPool = NewObject<UGsPoolUObject>(this);
	_rewardListViewEntryPool->WarmUp<UGsUIRewardIconListViewEntryData>(POOL_WARM_UP_SIZE);
}

void UGsUIPageDungeonGroupCommon::NativeConstruct()
{
	Super::NativeConstruct();

	FText message;
	FGsDungeonHelper::FindText(TEXT("UI_Tooltip_ResetTime"), message);
	_tooltipResetTimeInfo->SetDescText(message);
	_tooltipResetTimeInfo->Close();

	FGsDungeonHelper::FindText(TEXT("UI_Tooltip_EliteReward"), message);
	_tooltipEliteRewardInfo->SetDescText(message);
	_tooltipEliteRewardInfo->Close();

	_iconChargeItem.Reset();
	_iconChargeItem = Cast<UGsUIIconItemInventory>(_iconSelector->CreateIcon(EGsIconType::ICON_ITEM_INVENTORY_XL, 0));
	if (_iconChargeItem.IsValid())
	{
		_iconSelector->AddChild(_iconChargeItem.Get());
		_iconChargeItem->SetItemNameToolTipFlag(true);
		_iconChargeItem->SetOnChangeDisplayDetailWndType(UGsUIIconItemInventory::UseDisplayDetailWndType::LongPressType);

		if (false == _iconChargeItem->OnFinishedLongPressEvent.IsBoundToObject(this))
		{
			_iconChargeItem->OnFinishedLongPressEvent.AddUObject(this, &UGsUIPageDungeonGroupCommon::OnLongPressIconChargeItem);
		}
	}
}



void UGsUIPageDungeonGroupCommon::NativeDestruct()
{
	_listViewRewardSet->ClearListItems();
	_rewardListViewEntryPool->ReleaseAll();

	if (_iconSelector)
	{
		_iconSelector->RemoveAllChildren();
	}

	_iconChargeItem.Reset();

	Super::NativeDestruct();
}

void UGsUIPageDungeonGroupCommon::InvalidateDungeonDetailInfo()
{
	const int32 selectedIndex = _toggleGroupDungeonSet.GetSelectedIndex();
	TWeakObjectPtr<UGsUIDungeonSlotEntry> item = Cast<UGsUIDungeonSlotEntry>(_scrollBoxHelperDungeonSet->GetEntry(selectedIndex));
	if (item.IsValid())
	{
		const FGsDungeonData* dungeonData = item.Get()->GetData();
		const FGsSchemaDungeonData* schemaData = dungeonData->GetSchema();

		/**
		* 입장 정보
		*/
		if (DungeonCategory::NORMAL == _data->GetCategory() || DungeonCategory::SPECIAL == _data->GetCategory())
		{
			const FGsSchemaMapBalance* mapBalanceData
				= schemaData->mapId.GetRow() ? schemaData->mapId.GetRow()->balanceId.GetRow() : nullptr;

			if (mapBalanceData)
			{
				_btnTryEntered->SetData(dungeonData->GetEnterdCurrencyData().Get<0>(), dungeonData->GetEnterdCurrencyData().Get<1>());
			}

			_btnTryEntered->SetIsEnabled(dungeonData->IsUnLockState());

			/**
			* 입장 가능 레벨
			*/
			FText format;
			FGsDungeonHelper::FindText(TEXT("UI_Dungeon_LevelLimit"), format);
			_textLevelLimit->SetText(FText::Format(format, { schemaData->unlockLevel }));
		}
		else if (DungeonCategory::ELITE == _data->GetCategory())
		{
			const static int32 DEFAULT_LOBBY_INDEX = 0;
			const FGsDungeonData* lobbyDungeonData = _data->FindDungeonByIndex(DEFAULT_LOBBY_INDEX);

			if (lobbyDungeonData)
			{
				const FGsSchemaMapBalance* mapBalanceData
					= lobbyDungeonData->GetSchema()->mapId.GetRow() ? lobbyDungeonData->GetSchema()->mapId.GetRow()->balanceId.GetRow() : nullptr;

				_btnTryEntered->SetData(lobbyDungeonData->GetEnterdCurrencyData().Get<0>(), lobbyDungeonData->GetEnterdCurrencyData().Get<1>());
			}

			bool isUnlock = dungeonData->IsUnLockState() && GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::HALL_OF_TRIALS_CANNOT_ENTER);
			_btnTryEntered->SetIsEnabled(isUnlock);

			/**
			* 입장 가능 레벨
			*/
			FText format = FText::GetEmpty();			
			if (const FGsSchemaUnlockContents* unlockContentsData = _data->GetSchemaData()->unLockContentsId.GetRow())
			{
				if (UnlockCondition::LEVEL == unlockContentsData->lockType)
				{
					FGsDungeonHelper::FindText(TEXT("UI_Dungeon_LevelLimit"), format);

					format = FText::Format(format, { unlockContentsData->requireLevel});					
				}				
			}

			_textLevelLimit->SetText(format);
		}
	}	
}

void UGsUIPageDungeonGroupCommon::OnRefreshDungeonSetEntry(int32 InIndex, UWidget* InEntry)
{
	if (nullptr == _data)
	{
		GSLOG(Error, TEXT("nullptr == _data"));
		return;
	}

	TWeakObjectPtr<UGsUIDungeonSlotEntry> item = Cast<UGsUIDungeonSlotEntry>(InEntry);
	if (item.IsValid() && _data)
	{
		if (DungeonCategory::ELITE == _data->GetCategory())
		{	// 2023/4/5 PKT - 고레벨 던전은 로비층은 보이면 안된다.
			const auto& dungeonData = _data->FindDungeonByIndex(InIndex + 1);
			item.Get()->SetData(dungeonData);
		}
		else
		{
			const auto& dungeonData = _data->FindDungeonByIndex(InIndex);
			item.Get()->SetData(dungeonData);
		}
	}
}

void UGsUIPageDungeonGroupCommon::OnSelectedDungeon(int32 InIndex)
{
	TWeakObjectPtr<UGsUIDungeonSlotEntry> item = Cast<UGsUIDungeonSlotEntry>(_scrollBoxHelperDungeonSet->GetEntry(InIndex));
	if (item.IsValid())
	{
		const FGsDungeonData* dungeonData = item.Get()->GetData();
		const FGsSchemaDungeonData* schemaData = dungeonData->GetSchema();

		/**
		 * 보상 정보
		 */
		TSet<const FGsSchemaRewardItemBagData*> rewardItemBag;
		for (const FGsSchemaRewardDataRow& row : schemaData->mainRewardList)
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
		
		_rewardListViewEntryPool->ReleaseAll();

		TArray<UGsUIRewardIconListViewEntryData*> rewardEntrySet;
		FGsUIRewardIconData rewardIconData;
		for (const auto& itemBag : rewardItemBag)
		{
			UGsUIRewardIconListViewEntryData* viewEntryData = _rewardListViewEntryPool->Claim<UGsUIRewardIconListViewEntryData>();
			viewEntryData->_rewardItemDataArray.Empty();

			rewardIconData.Init(itemBag, itemBag->currencyAmountMin);

			viewEntryData->_rewardItemDataArray.Add(rewardIconData);

			rewardEntrySet.Emplace(viewEntryData);
		}

		_listViewRewardSet->SetListItems(rewardEntrySet);		
		_listViewRewardSet->RegenerateAllEntries();
		_listViewRewardSet->SetScrollbarVisibility(ESlateVisibility::Collapsed);
		_listViewRewardSet->ScrollIndexIntoView(0);

		InvalidateDungeonDetailInfo();

		if (DungeonCategory::ELITE == schemaData->dungeonCategory)
		{
			FText strTip = schemaData->isRandomRewardDesc;
			if (strTip.IsEmpty())
			{
				FGsDungeonHelper::FindText(TEXT("UI_Elite_Reward_Notify"), strTip);
			}		
			_textEliteRewardTip->SetText(strTip);
		}
	}

	_tooltipResetTimeInfo->Close();
	_tooltipEliteRewardInfo->Close();
}

void UGsUIPageDungeonGroupCommon::OnClickedGoTryToDungeon()
{
	if (DungeonCategory::NORMAL == _data->GetCategory() || DungeonCategory::SPECIAL == _data->GetCategory())
	{
		const int32 selectedIndex = _toggleGroupDungeonSet.GetSelectedIndex();
		TWeakObjectPtr<UGsUIDungeonSlotEntry> item = Cast<UGsUIDungeonSlotEntry>(_scrollBoxHelperDungeonSet->GetEntry(selectedIndex));
		if (item.IsValid() && item->GetData())
		{
			GSDungeon()->TryToEnterDungeon(item->GetData()->GetSchema()->mapId.GetRow()->id);
		}
	}
	else if (DungeonCategory::ELITE == _data->GetCategory())
	{
		const static int32 DEFAULT_LOBBY_INDEX = 0;
		const FGsDungeonData* data = _data->FindDungeonByIndex(DEFAULT_LOBBY_INDEX);
		if (data && data->GetSchema() && data->GetSchema()->mapId.GetRow())
		{
			GSDungeon()->TryToEnterDungeon(data->GetSchema()->mapId.GetRow()->id);
		}
	}
}

void UGsUIPageDungeonGroupCommon::OnClickedCharge()
{
	if (nullptr == _data)
	{
		GSLOG(Error, TEXT("nullptr == _data"));
		return;
	}

	const FGsSchemaItemCommon* schemaItemData = (_data->GetSchemaData()) ? _data->GetSchemaData()->chargeCostItemId.GetRow() : nullptr;
	if (schemaItemData)
	{
		const TArray<TWeakPtr<FGsItem>>& candinateItemList = GItem()->FindItem(ItemType::CONSUME, ItemCategoryMain::CHARGE, ItemCategorySub::DUNGEON_TIME);

		ItemDBId useItemDBId = INVALID_ITEM_DB_ID;

		for (const auto& candinateItem : candinateItemList)
		{
			TSharedPtr<FGsItem> item = candinateItem.Pin();

			if (nullptr == item->GetTableData())
			{
				continue;
			}

			if (item->GetTableData()->itemGroupId != schemaItemData->itemGroupId)
			{	// 2023/8/9 PKT -  그룹 아이디가 다르다면 같은 아이템이 아님.
				continue;
			}

			if (useItemDBId == INVALID_ITEM_DB_ID)
			{	// 2023/8/9 PKT - 제일 처음 아이템에 대한 정보는 기록
				useItemDBId = item->GetDBID();
			}

			if (true == item->GetTableData()->tradable)
			{	// 2023/8/9 PKT - 비귀속 아이템을 먼저 소비 한다.
				useItemDBId = item->GetDBID();

				// 2023/8/9 PKT - 비귀속 아이템을 찾았다면 빠져 나간다.
				break;
			}
		}

		if (INVALID_ITEM_DB_ID != useItemDBId)
		{
			const static ItemAmount DEFAULT_USE_COUNT = 0;
			GItem()->TryUseItem(useItemDBId, DEFAULT_USE_COUNT);
		}
	}
}

void UGsUIPageDungeonGroupCommon::OnClickedGoWorldMap()
{
	//if (false == GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::WORLDMAP))
	//{
	//	GSUnlock()->ShowLockTicker(EGsUnlockContentsType::WORLDMAP);
	//	return;
	//}

	// 언락 컨텐츠와 가능 타입 맵, 관전 모드 체크 한번에 하자
	if (FGsWorldMapFunc::IsRegionMapOpenPossible() == false)
	{
		return;
	}

	// 개인 이벤트 채널이면 티커출력 안넘어감
	// 싱글 필드면 안넘어감
	if (GLevel()->IsMapType(MapType::INSTANCE_SINGLE) == true || GLevel()->IsMapType(MapType::SINGLE_FIELD) == true)
	{
		FText findText;
		FText::FindText(TEXT("WorldMapText"), TEXT("ErrorInvalidRegion"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return;
	}

	if (GMode())
	{
		if (FGsGameFlowGame* gameFlow = GMode()->GetGameFlow())
		{
			if (FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame())
			{
				if (FGsGameStateRegionMap* regionMap =
					contents->GetContentsStateObject<FGsGameStateRegionMap>(FGsContentsMode::InGame::ContentsRegionMap))
				{
					const int32 selectedDungeon = _toggleGroupDungeonSet.GetSelectedIndex();

					TWeakObjectPtr<UGsUIDungeonSlotEntry> item = Cast<UGsUIDungeonSlotEntry>(_scrollBoxHelperDungeonSet->GetEntry(selectedDungeon));
					if (item.IsValid())
					{
						const FGsSchemaDungeonData* schemaData = item.Get()->GetData()->GetSchema();
						// 월드맵 개선
						// 던전에서 지역맵갈때
						// 전리품 탭으로 가게 처리
						// https://jira.com2us.com/jira/browse/CHR-23756
						regionMap->SetMapChangeType(EGsWorldMapChangeType::TO_REGIONMAP_DROP, schemaData->mapId.GetRow()->id);
					}					
				}
			}
		}
	}

	// 지역맵으로 이동
	GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_REGIONMAP);
}

void UGsUIPageDungeonGroupCommon::OnClickedEnvironmentIcon(class UGsUIIconAbnormality* InIcon)
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

void UGsUIPageDungeonGroupCommon::OnClickedResetTooltip()
{
	// 2023/2/22 PKT - 초기화 시간	
	_tooltipResetTimeInfo->ToggleOpenClose();
}

void UGsUIPageDungeonGroupCommon::OnClickedEliteRewardTooltip()
{	
	_tooltipEliteRewardInfo->ToggleOpenClose();
}

void UGsUIPageDungeonGroupCommon::OnLongPressIconChargeItem(UGsUIIconItem& InIcon)
{
	FGsItem* itemData = InIcon.GetItemIconData();
	if (itemData)
	{
		const FGsItemDetailOpenMessageParamTID param(itemData->GetTID(), true, false, UGsUIPopupItemDetailMain::CenterPosType::CenterPos);
		GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM_TID, &param);
	}
}

void UGsUIPageDungeonGroupCommon::Enter(const class FGsDungeonGroupBase* InData, const FGsDungeonUICaptureData& InCaptureData)
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
			icon->OnClickIcon.AddUObject(this, &UGsUIPageDungeonGroupCommon::OnClickedEnvironmentIcon);
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

	if (_iconChargeItem.IsValid())
	{
		if (const FGsSchemaItemCommon* itemCommonData = schemaGroupData->chargeCostItemId.GetRow())
		{
			_iconChargeItem->SetNotOwnItem(itemCommonData->id, 0);
			_iconChargeItem->SetItemNameVisibility(false);
			_iconChargeItem->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			_iconChargeItem->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	FText textBtnName;
	if (DungeonCategory::NORMAL == _data->GetCategory() || DungeonCategory::SPECIAL == _data->GetCategory())
	{
		FGsDungeonHelper::FindText(TEXT("UI_Button_Entered"), textBtnName);
	}
	else if (DungeonCategory::ELITE == _data->GetCategory())
	{
		FGsDungeonHelper::FindText(TEXT("UI_Button_Elite_Entered"), textBtnName);
	}

	_btnTryEntered->SetButtonName(textBtnName);

	/**
	 * Dungeon Set
	 */
	int32 maxCount = _data->DungeonCountAll();
	if (DungeonCategory::ELITE == _data->GetCategory())
	{	// 2023/2/27 PKT - 고레벨 같은 경우. 로비 층을 보여 주진 않는다.
		const static int32 LOBBY_COUNT = 1;
		maxCount -= LOBBY_COUNT;
	}
	// 2023/2/27 PKT - 던전 생성
	_scrollBoxHelperDungeonSet->RefreshAll(maxCount);

	/**
	 * Dungeon Set Toggle
	 */
	UWidget* focusWidget = nullptr;
	int32 selectedDungeonIndex = InCaptureData._lastDungeonIndex;
	if (0 != InCaptureData._mapId)
	{
		for (int32 idx = 0; idx < _scrollBoxHelperDungeonSet->GetItemCount(); ++idx)
		{
			TWeakObjectPtr<UGsUIDungeonSlotEntry> item = Cast<UGsUIDungeonSlotEntry>(_scrollBoxHelperDungeonSet->GetEntry(idx));
			if (item.IsValid())
			{
				const auto& dungeon = item.Get()->GetData();
				const FGsSchemaMapData* schemaMapData = (dungeon) ? dungeon->GetSchema()->mapId.GetRow() : nullptr;
				if (schemaMapData && schemaMapData->id == InCaptureData._mapId)
				{
					selectedDungeonIndex = idx;

					focusWidget = item.Get();

					break;
				}
			}
		}
	}

	_toggleGroupDungeonSet.Clear();
	_toggleGroupDungeonSet.AddToggleWidgetByParentPanel(_scrollBoxDungeonSet);
	_toggleGroupDungeonSet.SetSelectedIndex(selectedDungeonIndex, true);

	// 2023/2/21 PKT - Scoll Offset
	if (focusWidget)
	{
		_scrollBoxDungeonSet->ScrollWidgetIntoView(focusWidget, false, EDescendantScrollDestination::IntoView);
	}
	else
	{
		_scrollBoxDungeonSet->SetScrollOffset(InCaptureData._scrollBoxListOffset);
	}

	/**
	 * 초기화 시간
	 */
	FString strCron;
	FGsCronUtil::GetCronString(_data->GetSchemaData()->resetCycling.GetRow()->makeCron, strCron);	
	_textResetTime->SetText(FText::FromString(strCron));

	if (DungeonCategory::NORMAL == _data->GetCategory() || DungeonCategory::SPECIAL == _data->GetCategory())
	{
		_panelEliteRewardZone->SetVisibility(ESlateVisibility::Hidden);
	}
	else if (DungeonCategory::ELITE == _data->GetCategory())
	{
		_panelEliteRewardZone->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	Invalidate();
}

void UGsUIPageDungeonGroupCommon::Invalidate()
{
	Super::Invalidate();

	if (nullptr == _data)
	{
		return;
	}

	// 2023/2/27 PKT - 엘리트 던전 포함 노말 던전에 속한다.
	const FGsDungeonGroupNormal* normalGroupData = StaticCast<const FGsDungeonGroupNormal*>(_data);
	const FGsSchemaDungeonGroupData* schemaData = normalGroupData->GetSchemaData();

	/**
	 * 던전 기본 제공 시간
	 */
	if (-1 == normalGroupData->GetNormalTime())
	{
		FText findText;
		FText::FindText(TEXT("UICommonText"), TEXT("DungeonBaseTimeUnlimit"), findText);
		_textBaseTime->SetText(findText);
	}
	else
	{
		FString strBaseTime = FGsTimeStringHelper::GetMinuteString(normalGroupData->GetNormalTime() / 60);
		_textBaseTime->SetText(FText::FromString(strBaseTime));
	}

	bool isUnLockState = normalGroupData->IsUnLockContents(false);

	/**
	 * 충전 시간 / Cost 정보
	 */
	if (false == schemaData->disableTimeCharge)
	{
		_panelChargeRect->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		// 던전 추가 충전 시간
		if (-1 == normalGroupData->GetNormalTime())
		{
			_swicherChargeTime->SetActiveWidgetIndex(0);
		}
		else
		{
			int addTime = normalGroupData->GetChargeTime() / 60;
			_swicherChargeTime->SetActiveWidgetIndex((addTime > 0) ? 1 : 0);
			
			FText format;
			FGsDungeonHelper::FindText(TEXT("UI_ChargeTime_Min"), format);

			FText message = FText::Format(format, { FMath::Max(int32(addTime), 0) });			
			_textChargeTime->SetText(message);
		}

		// 2023/2/22 PKT - 충전 가능 여부( 언락 & Has Cost )		
		const ItemAmount costAmount = normalGroupData->CostAmount();
		_textChargeItemCount->SetText(FText::AsNumber(costAmount));
		_btnCharge->SetIsEnabled((isUnLockState && 0 < costAmount) ? true : false);
	}
	else
	{
		_btnCharge->SetIsEnabled(false);
		_panelChargeRect->SetVisibility(ESlateVisibility::Collapsed);
	}

	// 2023/2/27 PKT - 컨텐츠 언락에 따라 버튼 활성화
	InvalidateDungeonDetailInfo();

	// 2023/2/22 PKT - Dungeon Invalidate
	const int32 maxCount = _scrollBoxHelperDungeonSet->GetItemCount();
	for (int32 idx = 0; idx < maxCount; ++idx)
	{
		TWeakObjectPtr<UGsUIDungeonSlotEntry> item = Cast<UGsUIDungeonSlotEntry>(_scrollBoxHelperDungeonSet->GetEntry(idx));
		if (item.IsValid())
		{
			item.Get()->Invalidate();
		}
	}
}

void UGsUIPageDungeonGroupCommon::Capture(FGsDungeonUICaptureData* OutCaptherData) const
{
	Super::Capture(OutCaptherData);

	OutCaptherData->_scrollBoxListOffset = _scrollBoxDungeonSet->GetScrollOffset();
	OutCaptherData->_lastDungeonIndex = _toggleGroupDungeonSet.GetSelectedIndex();
}