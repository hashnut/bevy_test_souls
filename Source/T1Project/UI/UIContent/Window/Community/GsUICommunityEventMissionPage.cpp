// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/Community/GsUICommunityEventMissionPage.h"
#include "UI/UIContent/Window/Community/GsUICommunityEventMissionTab.h"
#include "UI/UIContent/Window/Community/GsUICommunityEventMissionEntry.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemReward.h"

#include "UserWidget.h"
#include "PanelWidget.h"
#include "WrapBox.h"

#include "DataSchema/CommunityGirl/GsSchemaCommunityEvent.h"
#include "DataSchema/CommunityGirl/Mission/GsSchemaCommunityEventMission.h"
#include "Management/ScopeGame/GsCommunityEventManager.h"
#include "CommunityEventMission/GsCommunityEventMissionData.h"
#include "UMG/Public/Components/HorizontalBoxSlot.h"
#include "UMG/Public/Components/PanelSlot.h"
#include "UMG/Public/Components/CanvasPanel.h"
#include "UMG/Public/Components/TextBlock.h"

#include "UI/UIContent/Common/Tooltip/GsUITooltipDesc.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UMG/Public/Blueprint/SlateBlueprintLibrary.h"
#include "../../Helper/GsUIHelper.h"


void UGsUICommunityEventMissionPage::BeginDestroy()
{
	if (nullptr != _tabScrollBoxHelper)
	{
		_tabScrollBoxHelper->OnCreateEntry.RemoveDynamic(this, &UGsUICommunityEventMissionPage::OnCreateTabButtonEntry);
		_tabScrollBoxHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUICommunityEventMissionPage::OnRefreshTabButtonEntry);
	}
	_tabToggleGroup.Clear();

	if (nullptr != _commonSlotHelper)
	{
		_commonSlotHelper->OnCreateEntry.RemoveDynamic(this, &UGsUICommunityEventMissionPage::OnCreateCommonEntry);
		_commonSlotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUICommunityEventMissionPage::OnRefreshCommonEntry);
	}
	_commonToggleGroup.Clear();

	Super::BeginDestroy();
}

void UGsUICommunityEventMissionPage::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TArray<UWidget*> childList = _tabScrollPanel->GetAllChildren();
	for (auto child : childList)
	{
		if (child->IsA<UGsUICommunityEventMissionTab>())
		{
			child->RemoveFromParent();
		}
	}
	childList.Empty();

	TArray<UWidget*> commnChildList = _missionCommonlist->GetAllChildren();
	for (auto commn : commnChildList)
	{
		if (commn->IsA<UGsUICommunityEventMissionEntry>())
		{
			commn->RemoveFromParent();
		}
	}
	commnChildList.Empty();

	_tabScrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_tabScrollBoxHelper->Initialize(_tabScrollBoxEntryClass, _tabScrollPanel);

	_tabScrollBoxHelper->OnCreateEntry.AddDynamic(this, &UGsUICommunityEventMissionPage::OnCreateTabButtonEntry);
	_tabScrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUICommunityEventMissionPage::OnRefreshTabButtonEntry);

	_tabToggleGroup.OnSelectChanged.BindUObject(this, &UGsUICommunityEventMissionPage::OnClickTab);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	_commonSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_commonSlotHelper->Initialize(_commonEntryWidgetClass, _missionCommonlist);
	_commonSlotHelper->OnCreateEntry.AddDynamic(this, &UGsUICommunityEventMissionPage::OnCreateCommonEntry);
	_commonSlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUICommunityEventMissionPage::OnRefreshCommonEntry);

	_commonToggleGroup.OnSelectChanged.BindUObject(this, &UGsUICommunityEventMissionPage::OnClickCommon);

	_tooltipButton->OnClicked.AddDynamic(this, &UGsUICommunityEventMissionPage::OnClickedTooltip);	

	FText findText;
	FText::FindText(TEXT("CommunityGirlText"), TEXT("ToolTip_Mission_Reward"), findText);
	_tooltipDesc->SetDescText(findText);

	SetToolTipDecPos();
	_tooltipDesc->OnInvalidatePosition.BindUObject(this, &UGsUICommunityEventMissionPage::SetToolTipDecPos);
}

void UGsUICommunityEventMissionPage::NativeDestruct()
{	
	_expire = false;
	_tabToggleGroup.Clear();
	_commonToggleGroup.Clear();

	_tabScrollBoxHelper->RefreshAll(0);
	_tabScrollBoxHelper->AllClearChildren();
	_commonSlotHelper->RefreshAll(0);
	_commonSlotHelper->AllClearChildren();

	if (_iconSelector)
	{
		if (auto icon = GetIcon())
		{
			icon->ReleaseToPool();
		}

		_iconSelector->RemoveAllChildren();
	}

	//타이머 해지	
	GetWorld()->GetTimerManager().ClearTimer(_updateTimerHandle);

	Super::NativeDestruct();
}

void UGsUICommunityEventMissionPage::NativeConstruct()
{
	Super::NativeDestruct();

	// 타이머 등록
	GetWorld()->GetTimerManager().SetTimer(_updateTimerHandle, this, &UGsUICommunityEventMissionPage::UpdateTimer, 1.0f, true);
}

void UGsUICommunityEventMissionPage::UpdateTimer()
{
	if (nullptr == _missionData)
		return;
	
	int tabCount = _missionData->GetTabCount();
	if (0 < tabCount)
	{
		const TMap<CommunityEventMissionTabId, TSharedPtr<FGsCommunityEventMissionTab>> mapMissionTab = _missionData->GetCommunityEventMissionTabDataList();

		TSharedPtr<FGsCommunityEventMissionTab> missionTab;
		int index = 0;
		for (auto It = mapMissionTab.CreateConstIterator(); It; ++It, ++index)
		{
			missionTab = It.Value();	
			if (!missionTab.IsValid())
				continue;;

			if (true == missionTab->GetIsTimeLock())
			{
				bool isLock = GSCommunityEvent()->IsCommunityEventMissionTabLock(_communityEventId, _missionId, missionTab->GetMissionTabId());
				if (false == isLock)
				{
					UWidget* widget = _tabScrollBoxHelper->GetEntry(index);
					if (nullptr != widget)
					{
						if (UGsUICommunityEventMissionTab* tab = Cast<UGsUICommunityEventMissionTab>(widget))
						{
							SetTabData(index, tab);
						}
					}
				}
			}
		}
	}
}

void UGsUICommunityEventMissionPage::OnCreateTabButtonEntry(UWidget* InEntry)
{
	if (auto tab = Cast<UGsUICommunityEventMissionTab>(InEntry))
	{
		_tabToggleGroup.AddToggleWidget(tab);
	}
}

// tab 스크롤 업데이트
void UGsUICommunityEventMissionPage::OnRefreshTabButtonEntry(int32 inIndex, class UWidget* InEntry)
{
	if (UGsUICommunityEventMissionTab* tab = Cast<UGsUICommunityEventMissionTab>(InEntry))
	{
		if (!_missionData.IsValid())
			return;

		if (inIndex < _missionData->GetTabCount())
		{
			const TMap<CommunityEventMissionTabId, TSharedPtr<FGsCommunityEventMissionTab>> mapMissionTab = _missionData->GetCommunityEventMissionTabDataList();

			TSharedPtr<FGsCommunityEventMissionTab> missionTab;
			int index = 0;
			for (auto It = mapMissionTab.CreateConstIterator(); It; ++It, ++index)
			{
				if (inIndex == index)
				{
					missionTab = It.Value();
					break;
				}
			}

			if (!missionTab.IsValid())
				return;
			
			// Text
			// Lock
			// redDot

			bool isLock = GSCommunityEvent()->IsCommunityEventMissionTabLock(_communityEventId, _missionId, missionTab->GetMissionTabId());
			tab->SetData(missionTab->GetTabNameText(), isLock, missionTab->IsRedDot(), missionTab->IsAllMissionCommonRewarded());
			tab->SetTabId(missionTab->GetMissionTabId());
		}

		if (inIndex == _missionData->GetTabCount() - 1)
		{
			const TArray<UPanelSlot*> slots = _tabScrollPanel->GetSlots();
			for (UPanelSlot* slot : slots)
			{
				if (nullptr == slot)
					continue;

				if (UHorizontalBoxSlot* boxSlot = Cast<UHorizontalBoxSlot>(slot))
				{
					boxSlot->SetSize(ESlateSizeRule::Fill);
				}
			}
		}
	}
}

void UGsUICommunityEventMissionPage::OnCreateCommonEntry(UWidget* InEntry)
{
	if (auto entry = Cast<UGsUICommunityEventMissionEntry>(InEntry))
	{
		_commonToggleGroup.AddToggleWidget(entry);
		entry->_delegateClickSlot.BindUObject(this, &UGsUICommunityEventMissionPage::OnClickCommonReward);
	}
}

void UGsUICommunityEventMissionPage::OnRefreshCommonEntry(int32 InIndex, class UWidget* InEntry)
{
	if (auto entry = Cast<UGsUICommunityEventMissionEntry>(InEntry))
	{
		if (!_missionTabData.IsValid())
			return;

		if (InIndex < _missionTabData->GetCommonCount())
		{
			const TMap<CommunityEventMissionCommonId, TSharedPtr<FGsCommunityEventMissionCommon>> mapMissionTab = _missionTabData->GetCommunityEventMissionCommonList();

			TSharedPtr<FGsCommunityEventMissionCommon> missionCommon;
			int index = 0;
			for (auto It = mapMissionTab.CreateConstIterator(); It; ++It, ++index)
			{
				if (InIndex == index)
				{
					missionCommon = It.Value();
					break;
				}
			}

			if (!missionCommon.IsValid())
				return;

			entry->SetData(missionCommon);
		}
	}
}

void UGsUICommunityEventMissionPage::OnClickTab(int32 inIndex)
{
	if (!_missionData.IsValid())
		return;

	if (inIndex < _missionData->GetTabCount())
	{
		const TMap<CommunityEventMissionTabId, TSharedPtr<FGsCommunityEventMissionTab>> mapMissionTab = _missionData->GetCommunityEventMissionTabDataList();
		
		int index = 0;
		for (auto It = mapMissionTab.CreateConstIterator(); It; ++It, ++index)
		{ 
			if (inIndex == index)
			{
				_missionTabData = It.Value();
				break;
			}
		}

		if (!_missionTabData.IsValid())
			return;

		int commonCount = _missionTabData->GetCommonCount();

		_tabIndex = inIndex;
		_missionTabId = _missionTabData->GetMissionTabId();
		_commonSlotHelper->AllClearChildren();
		_commonSlotHelper->RefreshAll(commonCount);
		_commonToggleGroup.SetSelectedIndex(0, true);

		_timeLockCanvasPanel->SetVisibility(_missionTabData->GetIsTimeLock()? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
}

void UGsUICommunityEventMissionPage::OnClickCommon(int32 InIndex)
{
	if (!_missionTabData.IsValid())
		return;

	if (InIndex >= _missionTabData->GetCommonCount())
		return;

	const TMap<CommunityEventMissionCommonId, TSharedPtr<FGsCommunityEventMissionCommon>> mapMissionCommon = _missionTabData->GetCommunityEventMissionCommonList();

	TSharedPtr<FGsCommunityEventMissionCommon> missionCommon;
	int index = 0;
	for (auto It = mapMissionCommon.CreateConstIterator(); It; ++It, ++index)
	{
		if (InIndex == index)
		{
			missionCommon = It.Value();
			break;
		}
	}

	if (!missionCommon.IsValid())
		return;

	if (false == missionCommon->IsRedDot())
		return;
}

void UGsUICommunityEventMissionPage::OnClickCommonReward(CommunityEventMissionCommonId InCommonId)
{
	if (!_missionTabData.IsValid())
		return;

	if (true == _expire)
		return;

	if (true == _missionTabData->GetIsTimeLock())
		return;

	const TMap<CommunityEventMissionCommonId, TSharedPtr<FGsCommunityEventMissionCommon>> mapMissionCommon = _missionTabData->GetCommunityEventMissionCommonList();
	TSharedPtr<FGsCommunityEventMissionCommon> missionCommon = mapMissionCommon.FindRef(InCommonId);
	if (!missionCommon.IsValid())
		return;

	if (false == missionCommon->IsRedDot())
		return;

	GSCommunityEvent()->SendReqMissionReward(_missionId, _missionTabId, missionCommon->GetMissionCommonId(), false);
}


void UGsUICommunityEventMissionPage::OnSelectMission(int index, const FGsSchemaCommunityEvent* inData)
{
	if (nullptr == inData)
	{
		return;
	}
	
	_tabIndex = -1;
	_schemaCommunityEvent = inData;
	_indexCommunityEvent = index;
	_communityEventId = _schemaCommunityEvent->id;

	const FGsSchemaCommunityEventMission* schemaMission
		= inData->typeInfoEventId.GetRowByType<FGsSchemaCommunityEventMission>();

	if (nullptr != schemaMission)
	{
		_SchemaCommunityEventMission = schemaMission;
		_missionId = schemaMission->id;
	}

	_missionData = GSCommunityEvent()->GetMissionData(_missionId);
	if (!_missionData.IsValid())
		return;
		
	bool isHasRewardBonus = _missionData->IsHasRewardBonus();
	_bonusCanvasPanel->SetVisibility(isHasRewardBonus ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	if (isHasRewardBonus)
	{
		FGsRewardUnboxingData bonusReward = _missionData->GetBonusRewardData();

		// 아이콘
		if (UGsUIIconItemReward* itemIcon = GetIcon())
		{
			itemIcon->SetIsAcceptable(_missionData->IsBonusRedDot());
			itemIcon->SetEnableDetailPopup(true);
			itemIcon->SetCenterPopup(true);
			itemIcon->SetItemNameToolTipFlag(true);

			itemIcon->SetIsRewardNoti(_missionData->IsBonusRedDot());
			itemIcon->SetIsRewarded(_missionData->IsRewardedBonus());

			if (false == itemIcon->OnClickSlot.IsBoundToObject(this))
			{
				itemIcon->OnClickSlot.AddUObject(this, &UGsUICommunityEventMissionPage::OnClickIconSlot);
			}
			if (false == itemIcon->OnFinishedLongPressEvent.IsBoundToObject(this))
			{
				itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUICommunityEventMissionPage::OnLongPressIcon);
			}

			if (bonusReward._itemDataList.IsValidIndex(0))
			{
				itemIcon->SetNotOwnItem(bonusReward._itemDataList[0]->itemId, bonusReward._itemDataList[0]->itemCountMin);
			}
		}

		FText findText;
		FText::FindText(TEXT("CommunityGirlText"), TEXT("UI_Screen_BonusReward"), findText);
		_bonusText->SetText(FText::Format(findText, _missionData->GetCompletedTabCount(), _missionData->GetTabCount()));
	}

	_tabToggleGroup.Clear();
	_commonToggleGroup.Clear();
	_tabScrollBoxHelper->AllClearChildren();
	_commonSlotHelper->AllClearChildren();

	UpdatePage(0);
}

void UGsUICommunityEventMissionPage::UpdatePage(int32 InIndex)
{
	if (!_missionData.IsValid())
		return;

	if (nullptr == _schemaCommunityEvent)
		return;

	if (nullptr == _SchemaCommunityEventMission)
		return;

	int tabCount = _missionData->GetTabCount();
	
	if (tabCount != _tabScrollBoxHelper->GetChildrenCount())
	{
		_tabScrollBoxHelper->RefreshAll(tabCount);
	}

	if (_tabIndex != InIndex)
	{
		_tabIndex = InIndex;
		_tabToggleGroup.SetSelectedIndex(_tabIndex, true);
	}

	_tabCanvasPanel->SetVisibility((tabCount < 2) ? ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible);
	_timeLockCanvasPanel->SetVisibility(_missionTabData->GetIsTimeLock() ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

#if UE_EDITOR
	_missionIdCanvasPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	FText textMissionId = FText::FromString(FString::Printf(TEXT("MissionId : %lld"), _missionId));
	_textMissionId->SetText(textMissionId);
#else
	_missionIdCanvasPanel->SetVisibility(ESlateVisibility::Collapsed);
#endif
}

void UGsUICommunityEventMissionPage::InvalidateAll()
{
	UpdatePage(_tabIndex);

	int tabCount = _tabScrollBoxHelper->GetItemCount();
	int commonCount = _commonSlotHelper->GetItemCount();

	for (int i = 0; i < tabCount; ++i)
	{
		UWidget* widget = _tabScrollBoxHelper->GetEntry(i);
		if (nullptr == widget)
		{
			continue;
		}

		if (UGsUICommunityEventMissionTab* tab = Cast<UGsUICommunityEventMissionTab>(widget))
		{
			SetTabData(i, tab);
		}
	}

	for (int j = 0; j < commonCount; ++j)
	{
		UWidget* widget = _commonSlotHelper->GetEntry(j);
		if (nullptr == widget)
		{
			continue;
		}

		if (UGsUICommunityEventMissionEntry* entry = Cast<UGsUICommunityEventMissionEntry>(widget))
		{
			SetCommonData(j, entry);
		}
	}

	if (nullptr == _missionData)
	{
		return;
	}

	bool isHasRewardBonus = _missionData->IsHasRewardBonus();
	if (isHasRewardBonus)
	{
		FGsRewardUnboxingData bonusReward = _missionData->GetBonusRewardData();

		// 아이콘
		if (UGsUIIconItemReward* itemIcon = GetIcon())
		{
			itemIcon->SetIsRewardNoti(_missionData->IsBonusRedDot());
			itemIcon->SetIsRewarded(_missionData->IsRewardedBonus());
		}

		FText findText;
		FText::FindText(TEXT("CommunityGirlText"), TEXT("UI_Screen_BonusReward"), findText);
		_bonusText->SetText(FText::Format(findText, _missionData->GetCompletedTabCount(), _missionData->GetTabCount()));
	}
}

UGsUIIconItemReward* UGsUICommunityEventMissionPage::GetIcon()
{
	if (0 == _iconSelector->GetChildrenCount())
	{
		UGsUIIconBase* newIcon = _iconSelector->CreateIcon();
		if (newIcon)
		{
			_iconSelector->AddChild(newIcon);
		}
	}

	// 아이콘을 하나만 생성하므로 0번째 아이콘 리턴
	return Cast<UGsUIIconItemReward>(_iconSelector->GetChildAt(0));
}

void UGsUICommunityEventMissionPage::OnClickIconSlot(class UGsUIIconItem& InIcon)
{
	UGsUIIconItemReward* itemIcon = Cast<UGsUIIconItemReward>(&InIcon);
	if (nullptr == itemIcon)
		return;

	if (true == _expire)
		return;

	GSCommunityEvent()->SendReqMissionReward(_missionId, 0, 0, true);
}

void UGsUICommunityEventMissionPage::OnLongPressIcon(UGsUIIconItem& InIcon)
{
	UGsUIIconItem::DefaultItemDetailOpener(InIcon, true);
}

void UGsUICommunityEventMissionPage::SetTabData(int InIndex, UGsUICommunityEventMissionTab* InEntryTab)
{
	if (nullptr == InEntryTab)
	{
		return;
	}

	const TMap<CommunityEventMissionTabId, TSharedPtr<FGsCommunityEventMissionTab>> mapMissionTab = _missionData->GetCommunityEventMissionTabDataList();

	TSharedPtr<FGsCommunityEventMissionTab> missionTab;
	int index = 0;
	for (auto It = mapMissionTab.CreateConstIterator(); It; ++It, ++index)
	{
		if (InIndex == index)
		{
			missionTab = It.Value();
			break;
		}
	}

	if (!missionTab.IsValid())
		return;

	// Text
	// Lock
	// redDot
	bool isLock = GSCommunityEvent()->IsCommunityEventMissionTabLock(_communityEventId, _missionId, missionTab->GetMissionTabId());
	InEntryTab->SetData(missionTab->GetTabNameText(), isLock, missionTab->IsRedDot(), missionTab->IsAllMissionCommonRewarded());
	InEntryTab->SetTabId(missionTab->GetMissionTabId());
	// 탭 이벤트 시간이 시작 여부
	_timeLockCanvasPanel->SetVisibility(_missionTabData->GetIsTimeLock() ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void UGsUICommunityEventMissionPage::SetCommonData(int InIndex, UGsUICommunityEventMissionEntry* InEntryCommon)
{
	if (nullptr == InEntryCommon)
	{
		return;
	}

	const TMap<CommunityEventMissionCommonId, TSharedPtr<FGsCommunityEventMissionCommon>> mapMissionTab = _missionTabData->GetCommunityEventMissionCommonList();

	TSharedPtr<FGsCommunityEventMissionCommon> missionCommon;
	int index = 0;
	for (auto It = mapMissionTab.CreateConstIterator(); It; ++It, ++index)
	{
		if (InIndex == index)
		{
			missionCommon = It.Value();
			break;
		}
	}

	if (!missionCommon.IsValid())
		return;

	InEntryCommon->SetData(missionCommon);
}

void UGsUICommunityEventMissionPage::OnClickedTooltip()
{
	if (_tooltipDesc->IsOpen() && _tooltipDesc->GetRenderOpacity() == 1.0f)
	{
		_tooltipDesc->Close();
		return;
	}

	SetToolTipDecPos();
	_tooltipDesc->Open();
	_tooltipDesc->SetRenderOpacity(1.0f);
	_tooltipDesc->SetIsNeedRefresh(true);
}

void UGsUICommunityEventMissionPage::SetToolTipDecPos()
{
	_tooltipDesc->SetRenderOpacity(1.0f);
	UWidget* targetWidget = _tooltipButton;

	FWidgetTransform panelTransform = _tooltipDesc->RenderTransform;
	if (targetWidget)
	{
		FVector2D targetTopLeftPixel;
		FVector2D targetTopLeftViewport;
		USlateBlueprintLibrary::LocalToViewport(GetParent(), targetWidget->GetCachedGeometry(), FVector2D::ZeroVector, targetTopLeftPixel, targetTopLeftViewport);

		FVector2D topLeftPixel;
		FVector2D topLeftViewport;
		USlateBlueprintLibrary::LocalToViewport(GetParent(), _tooltipDesc->GetCachedGeometry(), FVector2D::ZeroVector, topLeftPixel, topLeftViewport);

		FVector2D myTopRightViewport = targetTopLeftViewport;
		myTopRightViewport.X -= _tooltipDesc->GetCachedGeometry().GetLocalSize().X;

		panelTransform.Translation += -topLeftViewport + myTopRightViewport;	

		_tooltipDesc->SetRenderTransform(panelTransform);
	}
}

void UGsUICommunityEventMissionPage::SetExpire()
{

}