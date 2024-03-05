#include "GsUIWindowSeal.h"

#include "Runtime/UMG/Public/Animation/UMGSequencePlayer.h"
#include "Runtime/UMG/Public/Animation/WidgetAnimation.h"
#include "Runtime/UMG/Public/Components/ListView.h"
#include "Runtime/UMG/Public/Components/RichTextBlock.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Runtime/UMG/Public/Components/Widget.h"
#include "Runtime/UMG/Public/Components/WidgetSwitcher.h"
#include "Runtime/UMG/Public/Components/ScrollBox.h"
#include "Runtime/Core/Public/Math/UnrealMath.h"

#include "Currency/GsCostPackage.h"
#include "Currency/GsCurrencyHelper.h"
#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"
#include "DataSchema/Seal/GsSchemaSealNodeLevel.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsGameBase.h"
#include "GameFlow/GameContents/GsContentsMode.h"
#include "GameObject/Stat/GsStatHelper.h"
#include "Management/GsMessageHolder.h"
#include "Management/GsScopeGame.h"
#include "Management/GsScopeGlobal.h"
#include "Management/ScopeGame/GsSealManager.h"
#include "Management/ScopeGame/GsStatChangeNotifyManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Message/MessageParam/GsSealMessageParam.h"
#include "Net/GsNetSendServiceSeal.h"
#include "Seal/EGsSealState.h"
#include "Seal/GsSealNode.h"
#include "Seal/GsSealPack.h"
#include "Seal/GsSealSlot.h"
#include "Seal/GsUISealLevelStatListEntry.h"
#include "Seal/GsUISealNodeLine.h"
#include "Seal/GsUISealPack.h"
#include "Seal/GsUISealPackGainListEntry.h"
#include "Seal/GsUISealSlotListEntry.h"
#include "Seal/GsUISealMaterialListitem.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "T1Project.h"
#include "UI/UIContent/Common/GsUICurrencyButton.h"
#include "UI/UIContent/Common/GsUIProgressBar.h"
#include "UI/UIContent/Common/Icon/GsUIIconBase.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIContent/Common/Tooltip/GsUITooltipDesc.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsUIStringHelper.h"
#include "UI/UIContent/Helper/GsWidgetAnimationPlayCallbackProxy.h"
#include "UI/UIContent/Popup/GsUIPopupCommonPay.h"
#include "UI/UIContent/Popup/GsUIPopupSealNodeGainPool.h"
#include "UI/UIContent/Window/Seal/GsUISealNode.h"
#include "UI/UIContent/Window/Seal/GsUISealSlot.h"
#include "UI/UIContent/Common/GsUITabButtonCommon.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/ContentWidget/GsToggleButton.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UILib/Base/GsUIVFX.h"
#include "UI/UILib/Base/GsUIWidgetBase.h"

const FName UGsUIWindowSeal::WINDOW_ASSET_NAME = TEXT("WindowSeal");

static constexpr int32 SEAL_MAIN_TAB_OTHERS = 0;
static constexpr int32 SEAL_MAIN_TAB_SEALPACK = 1;

static constexpr int32 SEAL_SUB_TAB_SEALNODE = 0;
static constexpr int32 SEAL_SUB_TAB_SEALSLOT = 1;
static constexpr int32 SEAL_SUB_TAB_SEALPACK = 2;

static constexpr int32 SEAL_SUB_TAB_SEALSLOT_LEVEL_DESC_ENABLED = 0;
static constexpr int32 SEAL_SUB_TAB_SEALSLOT_LEVEL_DESC_DISABLED = 1;

void UGsUIWindowSeal::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr != _expProgressbar)
	{
		// 봉인에서는 이 기능을 비활성화 함.
		_expProgressbar->SetDecoWidgetVisibilityByAlpha(false);
	}

	_sequencePlayer = NewObject<UGsUISequencePlayer>();

	_btnBack->OnClicked.AddUniqueDynamic(this, &UGsUIWindowSeal::OnClickBack);
	_btnClose->OnClicked.AddUniqueDynamic(this, &UGsUIWindowSeal::OnClickCloseAllStack);

	// 카테고리 탭 초기화
	_categoryBtnSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_categoryBtnSlotHelper->Initialize(_subClassOfCategoryBtn, _scrollCategoryBtn);
	_categoryBtnSlotHelper->OnRefreshEntry.AddUniqueDynamic(this, &UGsUIWindowSeal::OnRefreshEntryCategoryTabBtn);
	_toggleGroupCategoryTabBtns.OnSelectChanged.BindUObject(this, &UGsUIWindowSeal::OnSelectChangedCategoryTabBtn);

	// 봉인팩 위젯 초기화
	{
		for (int32 i = 0; i < _sealPackSwitcher->GetNumWidgets(); ++i)
		{
			UWidget* SealPackWidget = _sealPackSwitcher->GetWidgetAtIndex(i);
			if (nullptr == SealPackWidget)
			{
				GSLOG(Error, TEXT("nullptr == SealPackWidget at Index : %d"), i);
				continue;
			}

			_sealPackWidgetList.Emplace(MakeWeakObjectPtr(Cast<UGsUISealPack>(SealPackWidget)));
		}

		for (TWeakObjectPtr<UGsUISealPack> WeakPackWidget : _sealPackWidgetList)
		{
			UGsUISealPack* PackWidget = WeakPackWidget.Get();
			if (nullptr == PackWidget)
			{
				GSLOG(Error, TEXT("nullptr == PackWidget"));
				continue;
			}

			PackWidget->_onSelectChangedNode.BindUObject(this, &UGsUIWindowSeal::OnSelectChangedNode);

			_onUiEvent.AddUObject(PackWidget, &UGsUISealPack::OnUIEvent);
		}
	}
	
	// 봉인획득표
	_sealPackGainToggleGroup.AddToggleWidget(_sealPackGain0TabBtn);
	_sealPackGainToggleGroup.AddToggleWidget(_sealPackGain1TabBtn);
	_sealPackGainToggleGroup.AddToggleWidget(_sealPackGain2TabBtn);
	_sealPackGainToggleGroup.OnSelectChanged.BindUObject(this, &UGsUIWindowSeal::OnSelectChangedSealPackGainTabBtn);

	// 봉인슬롯
	_uiSlotListEntries.Empty();
	_uiSlotListEntries.Append({
		MakeWeakObjectPtr(_sealSlotListEntry0),
		MakeWeakObjectPtr(_sealSlotListEntry1),
		MakeWeakObjectPtr(_sealSlotListEntry2),
	});

	_sealSlotListItems.Empty();
	for (int32 i = 0; i < _uiSlotListEntries.Num(); i++)
	{
		UGsUISealSlotListItem* SealSlotListItem = NewObject<UGsUISealSlotListItem>();
		_sealSlotListItems.Add(SealSlotListItem);
		_uiSlotListEntries[i]->SetListItem(SealSlotListItem);
		_onUiEvent.AddUObject(_uiSlotListEntries[i].Get(), &UGsUISealSlotListEntry::OnUIEvent);
	}

	// 서브탭버튼 초기화
	_subTabToggleGroup.AddToggleWidget(_subTab0Btn);
	_subTabToggleGroup.AddToggleWidget(_subTab1Btn);
	_subTabToggleGroup.AddToggleWidget(_subTab2Btn);
	_subTabToggleGroup.OnSelectChanged.BindUObject(this, &UGsUIWindowSeal::OnSelectChangedSubTabBtn);

	// 봉인슬롯획득 팝업 버튼
	_showStatBtn->OnClicked.AddUniqueDynamic(this, &UGsUIWindowSeal::OnClickShowSealSlotGainBtn);

	// 비용
	_enchantMaterialSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_enchantMaterialSlotHelper->Initialize(_subClassOfEnchantMaterial, _scrollEnchantMaterial);
	_enchantMaterialSlotHelper->OnRefreshEntry.AddUniqueDynamic(this, &UGsUIWindowSeal::OnRefreshEntryEnchantMaterialList);

	// 스킵 토글
	_skipToggleSwitcher->SetActiveWidgetIndex(1);
	if (nullptr != _skipToggleBtn)
	{
		_skipToggleBtn->OnClicked.AddUniqueDynamic(this, &UGsUIWindowSeal::OnClickSkipButton);
	}

	// 시도 버튼
	if (nullptr != _tryBtn)
	{
		float TryButtonLockSecs = 0.5f;
		UGsDataContainManager* DataContainManager = GData();
		if (nullptr != DataContainManager)
		{
			const UGsGlobalConstant* GlobalConstant = DataContainManager->GetGlobalData();
			if (nullptr != GlobalConstant)
			{
				TryButtonLockSecs = GlobalConstant->_sealWindowTryButtonLockSecs;
			}
		}

		_tryBtn->SetOnClickLockTime(TryButtonLockSecs);
		_tryBtn->OnClickCurrencyButton.BindUObject(this, &UGsUIWindowSeal::OnClickTryButton);
	}

	if (nullptr != _expProgressbar)
	{
		_expProgressbar->_onArrivedFullProgressOnAnimation.BindUObject(this, &UGsUIWindowSeal::OnArrivedFullProgressOnAnimation);
	}
}

void UGsUIWindowSeal::NativeConstruct()
{
	Super::NativeConstruct();

	GSStatChangeNotify()->Save(StatUpdateReason::SEAL_UPDATE);
	
	_selectedSealPackGainTabIndex = 0;
	_sequencePlayer->Reset();
}

bool UGsUIWindowSeal::SyncContentState()
{
	FGsMessageHolder* MessageHolder = GMessage();
	if (nullptr == MessageHolder)
	{
		GSLOG(Error, TEXT("nullptr == MessageHolder"));
		return false;
	}

	if (false == IsSameContentState(FGsContentsMode::InGame::ContentsSeal))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_SEAL);
		return true;
	}

	return false;
}

void UGsUIWindowSeal::NativeDestruct()
{
	_sequencePlayer->Reset();

	if (FGsStatChangeNotifyManager* statChangeNotifyMgr = GSStatChangeNotify())
	{
		statChangeNotifyMgr->CheckNotify(StatUpdateReason::SEAL_UPDATE);
	}

	Super::NativeDestruct();
}

TWeakObjectPtr<UGsUIWindowSeal> UGsUIWindowSeal::OpenWindow(const FGsContentsGameBase* InContentsGameBase)
{
	//GSLOG(Log, TEXT("UGsUIWindowSeal::OpenWindow(%p)"), InContentsGameBase);

	check(!!InContentsGameBase);
	check(InContentsGameBase->GetType() == FGsContentsMode::InGame::ContentsSeal);

	UGsUIManager* UiManager = GUI();
	if (nullptr == UiManager)
	{
		GSLOG(Error, TEXT("nullptr == UiManager"));
		return nullptr;
	}

	TWeakObjectPtr<UGsUIWidgetBase> Widget = UiManager->OpenAndGetWidget(UGsUIWindowSeal::WINDOW_ASSET_NAME);
	if (!Widget.IsValid())
	{
		GSLOG(Error, TEXT("!Widget.IsValid()"));
		return nullptr;
	}

	TWeakObjectPtr<UGsUIWindowSeal> Window = Cast<UGsUIWindowSeal>(Widget);
	if (!Window.IsValid())
	{
		GSLOG(Error, TEXT("!Window.IsValid()"));
		return nullptr;
	}

	return Window;
}

void UGsUIWindowSeal::ResetNodeSelections()
{
	FGsSealManager* SealManager = GSSeal();
	if (nullptr == SealManager)
	{
		GSLOG(Error, TEXT("nullptr == SealManager"));
		return;
	}

	// 서버에서 리롤 잠금 여부를 저장하고 불러오게 되어, 잠금 상태 리셋하지 않도록 변경됨
	//SealManager->TempResetAllSealNodeSelections();
}

void UGsUIWindowSeal::ResetTabs()
{
	ResetNodeSelections();

	// 메인탭 리셋
	_mainTabSwitcher->SetActiveWidgetIndex(SEAL_MAIN_TAB_OTHERS);

	UGsUISealPack* SealPackUI = GetSelectedPackWidget();
	if (nullptr != SealPackUI)
	{
		SealPackUI->ResetSelections();
	}

	_subTabToggleGroup.SetSelectedIndexNoEvent(-1);
	_subTabToggleGroup.SetSelectedIndex(0);
}

void UGsUIWindowSeal::ResetRerollUnlockSelection()
{
	for (UGsUISealSlotListItem* SealSlotListItem : _sealSlotListItems)
	{
		if (nullptr == SealSlotListItem)
		{
			GSLOG(Error, TEXT("nullptr == SealSlotListItem"));
			continue;
		}
		SealSlotListItem->ResetData();
	}
}

void UGsUIWindowSeal::InvalidateCategoryTab()
{
	FGsSealManager* SealManager = GSSeal();
	if (nullptr == SealManager)
	{
		GSLOG(Error, TEXT("nullptr == SealManager"));
		return;
	}

	// 카테고리 탭 재생성
	_categoryBtnSlotHelper->RefreshAll(SealManager->GetTotalSealPackCount());

	// 카테고리 탭 선택
	int32 TabIndex = SealManager->GetSealPackIndexWithId(_parameters._selectedPackId);
	_toggleGroupCategoryTabBtns.SetSelectedIndex(TabIndex == INDEX_NONE ? 0 : TabIndex);
}

void UGsUIWindowSeal::InvalidateSealPack()
{
	TWeakPtr<const FGsSealPack> SealPackPtr = GetSelectedPack();
	if (!SealPackPtr.IsValid())
	{
		GSLOG(Error, TEXT("!SealPackPtr.IsValid()"));
		return;
	}
	TSharedPtr<const FGsSealPack> SealPack = SealPackPtr.Pin();

	// 타이틀
	//_sealPackNameTxt->SetText(SealPack->GetSealPackTitle());

	UGsUISealPack* SealPackUI = GetSelectedPackWidget();
	if (nullptr == SealPackUI)
	{
		GSLOG(Error, TEXT("nullptr == SealPackUI"));
	}
	else
	{
		SealPackUI->SetParameters({
			SealPack->GetSealPackId()
		});
	}
}

void UGsUIWindowSeal::InvalidateTab(const bool InWithAnimation /*= false*/)
{
	// 메인
	const EGsSealMode SelectedMode = GetSelectedSealMode();

	// 매인탭
	if (EGsSealMode::SealPack == SelectedMode)
	{
		// 봉인노드 선택을 제거하고, 봉인팩 탭으로 전환
		_mainTabSwitcher->SetActiveWidgetIndex(SEAL_MAIN_TAB_SEALPACK);
	}
	else
	{
		_mainTabSwitcher->SetActiveWidgetIndex(SEAL_MAIN_TAB_OTHERS);
	}

	// 서브탭
	if (nullptr != _subTab0BtnSwitcher)
	{
		_subTab0BtnSwitcher->SetActiveWidgetIndex(EGsSealMode::SealNode == SelectedMode ? 0 : 1);
	}
	if (nullptr != _subTab1BtnSwitcher)
	{
		_subTab1BtnSwitcher->SetActiveWidgetIndex(EGsSealMode::SealSlot == SelectedMode ? 0 : 1);
	}
	if (nullptr != _subTab2BtnSwitcher)
	{
		_subTab2BtnSwitcher->SetActiveWidgetIndex(EGsSealMode::SealPack == SelectedMode ? 0 : 1);
	}

	if (EGsSealMode::SealPack == SelectedMode)
	{
		// 봉인노드 레벨 정보
		InvalidateSealPackStatus();
	}
	else
	{
		// 서브탭 공통 노드 정보
		const FGsSealNode* SealNode = GetSelectedNode();
		if (nullptr == SealNode)
		{
			GSLOG(Error, TEXT("nullptr == SealNode"));
		}
		else
		{
			FText SealNodeTitle = SealNode->GetTitle();
			if (nullptr != _titlePrefixTxt)
			{
				_titlePrefixTxt->SetText(SealNodeTitle);
			}

			if (nullptr != _titleSuffixTxt)
			{
				_titleSuffixTxt->SetText(FGsUIStringHelper::MakeFormatedText(FName("Lv.{0}"), SealNode->GetLevel()));
			}
		}

		if (EGsSealMode::SealNode == SelectedMode)
		{
			// 봉인노드 레벨 정보
			_subTabSwitcher->SetActiveWidgetIndex(SEAL_SUB_TAB_SEALNODE);
			InvalidateLevelInfo(InWithAnimation);
		}
		else if (EGsSealMode::SealSlot == SelectedMode)
		{
			// 봉인슬롯 리스트
			_subTabSwitcher->SetActiveWidgetIndex(SEAL_SUB_TAB_SEALSLOT);
			InvalidateSealSlotListArea();
		}

		InvalidateCost();
	}
}

static SealSlotColor GetByIndex(const int32 InIndex)
{
	switch (InIndex)
	{
	case 0: return SealSlotColor::RARE;
	case 1: return SealSlotColor::EPIC;
	case 2: return SealSlotColor::UNIQUE;
	}
	return SealSlotColor::NONE;
}

void UGsUIWindowSeal::InvalidateSealPackStatus()
{
	TWeakPtr<const FGsSealPack> SealPackPtr = GetSelectedPack();
	if (!SealPackPtr.IsValid())
	{
		GSLOG(Error, TEXT("!SealPackPtr.IsValid()"));
		return;
	}

	TSharedPtr<const FGsSealPack> SealPack = SealPackPtr.Pin();

	// 탭버튼 최신화
	_sealPackGainToggleGroup.SetSelectedIndex(_selectedSealPackGainTabIndex);

	// 봉인색상획득표
	const TMap<SealSlotColor, MatchCount>& SealLineColorCountMap = SealPack->GetSealLineColorCount();
	const SealSlotColor SelectedSealPackGainColor = GetByIndex(_sealPackGainToggleGroup.GetSelectedIndex());
	const MatchCount* MatchedCount = SealLineColorCountMap.Find(SelectedSealPackGainColor);
	if (nullptr == MatchedCount)
	{
		GSLOG(Error, TEXT("nullptr == MatchedCount"));
		return;
	}

	const TMap<MatchCount, TArray<TPair<StatType, StatValue>>>* GainsInColor = SealPack->GetSealPackAllGainsInColor(SelectedSealPackGainColor);
	if (nullptr == GainsInColor)
	{
		GSLOG(Error, TEXT("nullptr == GainsInColor"));
		return;
	}

	TSet<MatchCount> MatchCountSet;
	GainsInColor->GetKeys(MatchCountSet);
	TArray<MatchCount> MatchCounts = MatchCountSet.Array();
	MatchCounts.Sort();

	for (int32 i = 0; i < MatchCounts.Num(); ++i)
	{
		const MatchCount CurrentMatchCount = MatchCounts[i];

		const TArray<TPair<StatType, StatValue>>& GainStatInfos = GainsInColor->FindRef(CurrentMatchCount);
		if (0 >= GainStatInfos.Num())
		{
			GSLOG(Error, TEXT("0 >= GainStatInfos.Num()"));
			continue;
		}

		if (!_sealPackGainListItems.IsValidIndex(i))
		{
			_sealPackGainListItems.Add(NewObject<UGsUISealPackGainListItem>());
		}
		UGsUISealPackGainListItem* ListItem = _sealPackGainListItems[i];
		if (nullptr == ListItem)
		{
			GSLOG(Error, TEXT("nullptr == ListItem"));
			continue;
		}

		ListItem->_count = CurrentMatchCount;
		ListItem->_activated = CurrentMatchCount == (*MatchedCount);
		ListItem->_statInfos = GainStatInfos;
	}

	_sealPackGainListItems.SetNum(MatchCounts.Num());
	
	if (nullptr != _sealPackGainStatListView)
	{
		_sealPackGainStatListView->CancelScrollIntoView();
		_sealPackGainStatListView->SetListItems(_sealPackGainListItems);
		_sealPackGainStatListView->SetScrollbarVisibility(ESlateVisibility::Collapsed);
		_sealPackGainStatListView->RegenerateAllEntries();

		// 가장 근접한 갯수로 자동 스크롤
		{
			int32 NavigateIndex = -1;
			for (int32 i = 0; i < _sealPackGainListItems.Num(); i++)
			{
				check(_sealPackGainListItems.IsValidIndex(i));

				const UGsUISealPackGainListItem* SealPackGainListItem = _sealPackGainListItems[i];
				check(nullptr != SealPackGainListItem);

				if (SealPackGainListItem->_count == *MatchedCount)
				{
					NavigateIndex = i;
					break;
				}
			}

			if (0 <= NavigateIndex)
			{
				_sealPackGainStatListView->NavigateToIndex(NavigateIndex);
			}
		}
	}
}

void UGsUIWindowSeal::InvalidateLevelInfo(const bool InWithAnimation /*= false*/)
{
	const FGsSealNode* SealNode = GetSelectedNode();
	if (nullptr == SealNode)
	{
		GSLOG(Error, TEXT("nullptr == SealNode"));
		return;
	}

	// 노드 레벨별 스탯
	if (nullptr != _levelStatListView)
	{
		UpdateSealLevelStatListItems(*SealNode, InWithAnimation);
		_levelStatListView->CancelScrollIntoView();
		_levelStatListView->SetListItems(_sealLevelStatListItems);
		_levelStatListView->SetScrollbarVisibility(ESlateVisibility::Collapsed);
		_levelStatListView->RegenerateAllEntries();
		// 가장 근접한 레벨을 찾아 자동 스크롤
		{
			const Level CurrentSealNodeLevel = SealNode->GetLevel();
			int32 NavigateIndex = 0;
			for (int32 i = 0; i < _sealLevelStatListItems.Num(); i++)
			{
				check(_sealLevelStatListItems.IsValidIndex(i));

				const UGsUISealLevelStatListItem* SealLevelStatListItem = _sealLevelStatListItems[i];
				check(nullptr != SealLevelStatListItem);

				if (SealLevelStatListItem->_level <= CurrentSealNodeLevel)
				{
					NavigateIndex = i;
				}
			}
			if (0 <= NavigateIndex)
			{
				_levelStatListView->NavigateToIndex(NavigateIndex);
			}
		}
	}

	if (nullptr != _expProgressbar)
	{
		const float CurrentLevel = StaticCast<float>(SealNode->GetLevel());
		const float ExpPercent = (0 != SealNode->GetTotalExp()) ? (SealNode->GetCurrentExp() / StaticCast<float>(SealNode->GetTotalExp())) : 0.f;
		const float LevelAndExpPercent = CurrentLevel + ExpPercent;
		const float MaxLevelExp = FMath::Max(1, StaticCast<int32>(SealNode->GetLevelMax()));

		_expProgressbar->SetMaxPercent(MaxLevelExp);
		_expProgressbar->SetProgress(SealNode->IsLevelMax() ? MaxLevelExp : LevelAndExpPercent, 0.f, InWithAnimation);

		FText ProgressText;
		if (SealNode->IsLevelMax())
		{
			FText::FindText(TEXT("SealText"), TEXT("UI_Common_LevelMax"), ProgressText);
		}
		else
		{
			FGsUIStringHelper::GetTwoValueSlashedText(SealNode->GetCurrentExp(), SealNode->GetTotalExp(), ProgressText);
		}
		_expProgressbar->SetProgressText(ProgressText);
	}

	if (nullptr != _expDescRichTxt)
	{
		FText ExpDescText = FText::GetEmpty();
		if (!SealNode->IsLevelMax())
		{
			FText::FindText(TEXT("SealText"), TEXT("UI_Node_ExpGain"), ExpDescText);
			ExpDescText = FText::Format(ExpDescText, SealNode->GetRandomExpMin(), SealNode->GetRandomExpMax());
		}
		_expDescRichTxt->SetText(ExpDescText);
	}
}

void UGsUIWindowSeal::InvalidateSealSlotListArea()
{
	const FGsSealNode* SealNode = GetSelectedNode();
	if (nullptr == SealNode)
	{
		GSLOG(Error, TEXT("nullptr == SealNode"));
		return;
	}

	if (SealNode->CanReroll())
	{
		_levelDescSwitcher->SetActiveWidgetIndex(SEAL_SUB_TAB_SEALSLOT_LEVEL_DESC_ENABLED);
	}
	else
	{
		_levelDescSwitcher->SetActiveWidgetIndex(SEAL_SUB_TAB_SEALSLOT_LEVEL_DESC_DISABLED);

		const FText LevelText = FGsUIStringHelper::MakeFormatedText(TEXT("Lv.{0}"), SealNode->GetRerollAvailableLevel());
		FText Text;
		FText::FindText(TEXT("SealText"), TEXT("UI_Slot_NeedNodeLevel"), Text);
		const FText LevelDescText = FText::Format(Text, LevelText);
		_levelDescTxt->SetText(LevelDescText);
	}

	for (int32 i = 0; i < _uiSlotListEntries.Num(); i++)
	{
		InvalidateSealSlotListEntry(i, true);
	}
}

void UGsUIWindowSeal::InvalidateSealSlotListEntry(const int32 InSlotIndex, const bool InClearEffect)
{
	UpdateSealSlotListItem(InSlotIndex);

	if (_uiSlotListEntries.IsValidIndex(InSlotIndex))
	{
		TWeakObjectPtr<UGsUISealSlotListEntry>& SealSlotListEntry = _uiSlotListEntries[InSlotIndex];
		if (!SealSlotListEntry.IsValid())
		{
			GSLOG(Error, TEXT("!SealSlotListEntry.IsValid()"));
			return;
		}

		SealSlotListEntry->Invalidate(InClearEffect);
	}
}

void UGsUIWindowSeal::InvalidateCost()
{
	InvalidateItemCost();
	InvalidateCurrencyCost();
}

void UGsUIWindowSeal::InvalidateItemCost()
{
	const FGsSealNode* SealNode = GetSelectedNode();
	if (nullptr == SealNode)
	{
		GSLOG(Error, TEXT("nullptr == SealNode"));
		return;
	}

	const EGsSealMode SelectedTab = GetSelectedSealMode();
	if (EGsSealMode::SealNode == SelectedTab)
	{
		// 봉인노드 레벨업 비용
		_itemCostPackage = SealNode->CreateLevelUpCostPackage();
		_currencyCostPackage = SealNode->CreateLevelUpCostPackage(true);
		FText Text;
		FText::FindText(TEXT("SealText"), TEXT("UI_Node_Grow"), Text);
		_tryBtn->SetButtonName(Text);
	}
	else if (EGsSealMode::SealSlot == SelectedTab)
	{
		// 봉인슬롯 리롤 비용
		_itemCostPackage = SealNode->CreateRerollCostPackage(GetCurrentRerollLockCount());
		_currencyCostPackage = SealNode->CreateRerollCostPackage(GetCurrentRerollLockCount(), true);
		FText Text;
		FText::FindText(TEXT("SealText"), TEXT("UI_Slot_Reroll"), Text);
		_tryBtn->SetButtonName(Text);
	}

	_tryBtn->SetIsEnabled(SealNode->CanReroll());

	_enchantMaterialSlotHelper->RefreshAll(_itemCostPackage != nullptr ?  _itemCostPackage->GetCostElementsCount() : 1);
}

void UGsUIWindowSeal::InvalidateCurrencyCost()
{
	// 재화
	if (_currencyCostPackage.IsValid() && _currencyCostPackage->IsValidCostAtLeast())
	{
		const FGsCostElement* CostElement = _currencyCostPackage->FindFirstCostElementBy(CostType::CURRENCY);
		if (nullptr == CostElement)
		{
			_tryBtn->SetData(CurrencyType::NONE, NSLOCTEXT("SealText", "dash", "-"));
		}
		else
		{
			_tryBtn->SetData(*CostElement);
			_tryBtn->UpdateAmountTextColorLocalPlayer();
			_tryBtn->SetIsEnabled(true);
		}
	}
	else
	{
		_tryBtn->SetData(CurrencyType::NONE, NSLOCTEXT("SealText", "dash", "-"));
		_tryBtn->SetIsEnabled(false);
	}
}

void UGsUIWindowSeal::UpdateSealLevelStatListItems(const FGsSealNode& InSealNode, const bool InWithAnimation)
{
	const TMap<Level, TArray<TPair<StatType, StatValue>>> SealNodeStatInfoAllLevel = FGsSealManager::CollectSealNodeStatInfoAllLevel(InSealNode.GetSealNodeLevelId());

	TSet<Level> AllLevelsSet;
	SealNodeStatInfoAllLevel.GetKeys(AllLevelsSet);
	TArray<Level> AllLevels = AllLevelsSet.Array();
	AllLevels.Sort();

	const Level CurrentNodeLevel = InSealNode.GetLevel();

	int32 LevelIndex = 0;
	for (; LevelIndex < AllLevels.Num(); LevelIndex++)
	{
		UGsUISealLevelStatListItem* ListItem = (LevelIndex < _sealLevelStatListItems.Num()) ? _sealLevelStatListItems[LevelIndex] : _sealLevelStatListItems.Emplace_GetRef(NewObject<UGsUISealLevelStatListItem>());
		if (nullptr == ListItem)
		{
			GSLOG(Error, TEXT("nullptr == ListItem"));
			continue;
		}

		const Level SealNodeLevel = AllLevels[LevelIndex];
		ListItem->_level = SealNodeLevel;
		ListItem->_activated = SealNodeLevel <= CurrentNodeLevel;
		ListItem->_statInfos = SealNodeStatInfoAllLevel.FindRef(SealNodeLevel);
		ListItem->_justActivated = InWithAnimation && (ListItem->_activated) && _sealNodeLevelLast < SealNodeLevel;
	}

	_sealNodeLevelLast = CurrentNodeLevel;

	// 오버된 목록 삭제
	_sealLevelStatListItems.RemoveAt(LevelIndex, _sealLevelStatListItems.Num() - LevelIndex);
}

void UGsUIWindowSeal::UpdateSealSlotListItem(const int32 InSlotIndex)
{
	TWeakPtr<const FGsSealPack> SealPack = GetSelectedPack();
	if (!SealPack.IsValid())
	{
		GSLOG(Error, TEXT("!SealPack.IsValid()"));
		return;
	}

	const FGsSealNode* SealNode = GetSelectedNode();
	if (nullptr == SealNode)
	{
		GSLOG(Error, TEXT("nullptr == SealNode"));
		return;
	}

	TWeakPtr<const FGsSealSlot> SealSlotPtr = SealNode->GetSealSlotByIndex(InSlotIndex);
	if (!SealSlotPtr.IsValid())
	{
		GSLOG(Error, TEXT("!SealSlotPtr.IsValid()"));
		return;
	}

	TSharedPtr<const FGsSealSlot> SealSlot = SealSlotPtr.Pin();
	check(SealSlot.IsValid());

	if (!_sealSlotListItems.IsValidIndex(InSlotIndex))
	{
		GSLOG(Error, TEXT("!_sealSlotListItems.IsValidIndex(%d)"), InSlotIndex);
		return;
	}

	UGsUISealSlotListItem* ListItem = _sealSlotListItems[InSlotIndex];
	ListItem->SetSlotIndex(InSlotIndex);
	//ListItem._activated = InSealNode.
	const bool CanRerollLock = SealSlot->CanRerollLock();
	const bool ResetRerollLock = (ListItem->_sealSlotId != SealSlot->GetSealSlotId()) || !CanRerollLock;

	FString TextKey;
	switch (InSlotIndex)
	{
	case 0: TextKey = TEXT("UI_Slot_Slot1"); break;
	case 1: TextKey = TEXT("UI_Slot_Slot2"); break;
	case 2: TextKey = TEXT("UI_Slot_Slot3"); break;
	}
	FText SlotHeaderText;
	FText::FindText(TEXT("SealText"), TextKey, SlotHeaderText);

	ListItem->_sealNodeId = SealNode->GetSealNodeId();
	ListItem->_sealSlotId = SealSlot->GetSealSlotId();
	ListItem->_header = SlotHeaderText;
	ListItem->_title = SealSlot->GetStatText();
	ListItem->_sealColor = SealSlot->GetSealColor();
	ListItem->_unlockStat = SealSlot->IsRerollUnlock(EGsSealRerollUnlockTarget::Stat);
	ListItem->_unlockColor = SealSlot->IsRerollUnlock(EGsSealRerollUnlockTarget::Color);
	ListItem->_canRerollLock = CanRerollLock && (GetCurrentRerollLockCount() < SealPack.Pin()->GetSealRerollLockCountMax());
	ListItem->_onClickUnlockStat.BindUObject(this, &UGsUIWindowSeal::OnClickSealRerollUnlockTarget);
	ListItem->_onClickUnlockColor.BindUObject(this, &UGsUIWindowSeal::OnClickSealRerollUnlockTarget);
}

void UGsUIWindowSeal::TryToLevelup() const
{
	const FGsSealNode* SealNode = GetSelectedNode();
	if (nullptr == SealNode)
	{
		GSLOG(Error, TEXT("nullptr == SealNode"));
		return;
	}

	{
		TSharedPtr<FGsCostPackage> CostPackage = SealNode->CreateLevelUpCostPackage();
		if (!CostPackage.IsValid())
		{
			GSLOG(Error, TEXT("!CostPackage.IsValid()"));
		}
		else
		{
			const SealPackId TargetSealPackId = _parameters._selectedPackId;
			const SealId TargetSealNodeId = SealNode->GetSealNodeId();

			if (IsEnableSkip())
			{
				if (!CostPackage->PlayerHasEnough())
				{
					FGsCurrencyHelper::OpenLackCostPackagePopup(*CostPackage);  
				}
				else
				{
					GSLOG(Log, TEXT("FGsSealMessageParamReqSealLevelUp(%d)"), TargetSealNodeId);
					FGsUIHelper::ShowBlockUI();
					FGsSealMessageParamReqSealLevelUp(TargetSealPackId, TargetSealNodeId).Send();
				}
			}
			else
			{
				UGsUIPopupCommonPay* PopupCommonPay = Cast<UGsUIPopupCommonPay>(GUI()->OpenAndGetWidget(TEXT("PopupCommonPay")));
				FText Text;
				FText::FindText(TEXT("SealText"), TEXT("UI_Node_Popup_Desc"), Text);
				PopupCommonPay->SetData(Text, CostPackage.ToSharedRef(),
					[TargetSealPackId, TargetSealNodeId](bool InOk) {
						if (InOk)
						{
							GSLOG(Log, TEXT("FGsSealMessageParamReqSealLevelUp(%d)"), TargetSealNodeId);
							FGsUIHelper::ShowBlockUI();
							FGsSealMessageParamReqSealLevelUp(TargetSealPackId, TargetSealNodeId).Send();
						}
					});
			}
		}
	}
}

void UGsUIWindowSeal::TryToRerollFirst()
{
	const FGsSealNode* SealNode = GetSelectedNode();
	if (nullptr == SealNode)
	{
		GSLOG(Error, TEXT("nullptr == SealNode"));
		return;
	}

	const SealPackId PackId = _parameters._selectedPackId;
	const SealId SealNodeId = SealNode->GetSealNodeId();
	TSet<SealSlotId> LockStatSealSlotIds;
	TSet<SealSlotId> LockColorSealSlotIds;
	for (const UGsUISealSlotListItem* ListItem : _sealSlotListItems)
	{
		if (nullptr == ListItem)
		{
			GSLOG(Error, TEXT("nullptr == ListItem"));
			continue;
		}

		if (!ListItem->_unlockStat)
		{
			LockStatSealSlotIds.Add(ListItem->_sealSlotId);
		}

		if (!ListItem->_unlockColor)
		{
			LockColorSealSlotIds.Add(ListItem->_sealSlotId);
		}
	}

	TSharedPtr<FGsCostPackage> CostPackage = SealNode->CreateRerollCostPackage(GetCurrentRerollLockCount());
	if (!CostPackage.IsValid())
	{
		GSLOG(Error, TEXT("!CostPackage.IsValid()"));
	}
	else
	{
		if (IsEnableSkip())
		{
			if (!CostPackage->PlayerHasEnough())
			{
				FGsCurrencyHelper::OpenLackCostPackagePopup(*CostPackage);
			}
			else
			{
				UGsUIWindowSeal::TryToRerollLast(PackId, SealNodeId, MoveTemp(LockStatSealSlotIds), MoveTemp(LockColorSealSlotIds));
			}
		}
		else
		{
			UGsUIPopupCommonPay* PopupCommonPay = Cast<UGsUIPopupCommonPay>(GUI()->OpenAndGetWidget(TEXT("PopupCommonPay")));
			FText MessageText;
			FText::FindText(TEXT("SealText"), TEXT("UI_Slot_Popup_Desc"), MessageText);
			PopupCommonPay->SetData(MessageText, CostPackage.ToSharedRef(),
				[PackId, SealNodeId, InLockStatSealSlotIds = MoveTemp(LockStatSealSlotIds), InLockColorSealSlotIds = MoveTemp(LockColorSealSlotIds)](bool InOk) {
					if (InOk)
					{
						UGsUIWindowSeal::TryToRerollLast(PackId, SealNodeId, InLockStatSealSlotIds, InLockColorSealSlotIds);
					}
				});
		}
	}
}

void UGsUIWindowSeal::TryToRerollLast(const SealPackId InSealPackId, const SealId InSealNodeId, TSet<SealSlotId> InLockStatSealSlotIds, TSet<SealSlotId> InLockColorSealSlotIds)
{
	const FGsSealManager* SealManager = GSSeal();
	if (nullptr == SealManager)
	{
		GSLOG(Error, TEXT("nullptr == SealManager"));
		return;
	}

	TWeakPtr<const FGsSealNode> SealNodePtr = SealManager->GetSealNode(InSealNodeId);
	if (!SealNodePtr.IsValid())
	{
		GSLOG(Error, TEXT("!SealNodePtr.IsValid()"));
		return;
	}

	TSharedPtr<const FGsSealNode> SealNode = SealNodePtr.Pin();

	bool IsTryToRerollUniqueColor = false;
	bool IsTryToRerollUniqueStat = false;

	for (int32 i = SealNode->GetSealSlotCount() - 1; 0 <= i; --i)
	{
		TWeakPtr<const FGsSealSlot> SealSlotWeak = SealNode->GetSealSlotByIndex(i);
		if (!SealSlotWeak.IsValid())
		{
			GSLOG(Error, TEXT("!SealSlotWeak.IsValid()"));
			return;
		}

		TSharedPtr<const FGsSealSlot> SealSlot = SealSlotWeak.Pin();
		if (!SealSlot.IsValid())
		{
			GSLOG(Error, TEXT("!SealSlotWeak.IsValid()"));
			return;
		}

		if (SealSlotColor::UNIQUE == SealSlot->GetSealColor() && !InLockColorSealSlotIds.Contains(SealSlot->GetSealSlotId()))
		{
			IsTryToRerollUniqueColor = true;
			break;
		}
		
		if (1 == SealSlot->GetSealStatTier() && !InLockColorSealSlotIds.Contains(SealSlot->GetSealSlotId()))
		{
			IsTryToRerollUniqueStat = true;
		}
	}

	if (IsTryToRerollUniqueColor)
	{
		// 유니크 색상을 리롤하려고 시도할 때, 메세지박스 경고를 출력
		FText MessageText;
		FText::FindText(TEXT("SealText"), TEXT("UI_Popup_RerollCheck"), MessageText);
		FGsUIHelper::PopupYesNo(MessageText, [InSealPackId, InSealNodeId, InInLockStatSealSlotIds = MoveTemp(InLockStatSealSlotIds), InInLockColorSealSlotIds = MoveTemp(InLockColorSealSlotIds)](bool InYesNo)
		{
			if (InYesNo)
			{
				FGsUIHelper::ShowBlockUI();
				FGsSealMessageParamReqSealSlotChangeAll(InSealPackId, InSealNodeId, &InInLockStatSealSlotIds, &InInLockColorSealSlotIds).Send();
			}
		});
	}
	else if (IsTryToRerollUniqueStat)
	{
		// 유니크 스탯을 리롤하려고 시도할 때, 메세지박스 경고를 출력
		FText MessageText;
		FText::FindText(TEXT("SealText"), TEXT("UI_Popup_RerollCheck_Stat"), MessageText);
		FGsUIHelper::PopupYesNo(MessageText, [InSealPackId, InSealNodeId, InInLockStatSealSlotIds = MoveTemp(InLockStatSealSlotIds), InInLockColorSealSlotIds = MoveTemp(InLockColorSealSlotIds)](bool InYesNo)
			{
				if (InYesNo)
				{
					FGsUIHelper::ShowBlockUI();
					FGsSealMessageParamReqSealSlotChangeAll(InSealPackId, InSealNodeId, &InInLockStatSealSlotIds, &InInLockColorSealSlotIds).Send();
				}
			});
	}
	else
	{
		FGsUIHelper::ShowBlockUI();
		FGsSealMessageParamReqSealSlotChangeAll(InSealPackId, InSealNodeId, &InLockStatSealSlotIds, &InLockColorSealSlotIds).Send();
	}
}

TWeakPtr<const FGsSealPack> UGsUIWindowSeal::GetSelectedPack() const
{
	const FGsSealManager* SealManager = GSSeal();
	if (nullptr == SealManager)
	{
		GSLOG(Error, TEXT("nullptr == SealManager"));
		return nullptr;
	}

	return SealManager->GetSealPack(_parameters._selectedPackId);
}

TWeakPtr<const FGsSealPack> UGsUIWindowSeal::GetPackWithIndex(const uint8 InIndex) const
{
	const FGsSealManager* SealManager = GSSeal();
	if (nullptr == SealManager)
	{
		GSLOG(Error, TEXT("nullptr == SealManager"));
		return nullptr;
	}

	return SealManager->GetSealPackWithIndex(InIndex);
}

UGsUISealPack* UGsUIWindowSeal::GetSelectedPackWidget()
{
	int32 TabIndex = _toggleGroupCategoryTabBtns.GetSelectedIndex();

	if (!_sealPackWidgetList.IsValidIndex(TabIndex))
	{
		GSLOG(Error, TEXT("!_sealPackWidgetList.IsValidIndex(%d)"), TabIndex);
		return nullptr;
	}

	return _sealPackWidgetList[TabIndex].Get();
}

const UGsUISealPack* UGsUIWindowSeal::GetSelectedPackWidget() const
{
	int32 TabIndex = _toggleGroupCategoryTabBtns.GetSelectedIndex();

	if (!_sealPackWidgetList.IsValidIndex(TabIndex))
	{
		GSLOG(Error, TEXT("!_sealPackWidgetList.IsValidIndex(%d)"), TabIndex);
		return nullptr;
	}

	return _sealPackWidgetList[TabIndex].Get();
}

const FGsSealNode* UGsUIWindowSeal::GetSelectedNode() const
{
	TWeakPtr<const FGsSealPack> SealPack = GetSelectedPack();
	const UGsUISealPack* PackWidget = GetSelectedPackWidget();
	return (!SealPack.IsValid() || nullptr == PackWidget) ? nullptr : SealPack.Pin()->GetSealNodeByIndex(PackWidget->GetSelectedNodeIndex());
}

int32 UGsUIWindowSeal::GetCurrentRerollLockCount() const
{
	int32 Count = 0;
	for (const UGsUISealSlotListItem* SealSlotListItem : _sealSlotListItems)
	{
		if (nullptr == SealSlotListItem)
		{
			GSLOG(Error, TEXT("nullptr == SealSlotListItem"));
			continue;
		}

		if (!SealSlotListItem->_unlockStat)
		{
			++Count;
		}

		if (!SealSlotListItem->_unlockColor)
		{
			++Count;
		}
	}
	return Count;
}

UGsUIWindowSeal::EGsSealMode UGsUIWindowSeal::GetSelectedSealMode() const
{
	const int32 SubTabIndex = _subTabToggleGroup.GetSelectedIndex();
	if (SubTabIndex == SEAL_SUB_TAB_SEALNODE)
	{
		return UGsUIWindowSeal::EGsSealMode::SealNode;
	}
	else if (SubTabIndex == SEAL_SUB_TAB_SEALSLOT)
	{
		return UGsUIWindowSeal::EGsSealMode::SealSlot;
	}
	else if (SubTabIndex == SEAL_SUB_TAB_SEALPACK)
	{
		return UGsUIWindowSeal::EGsSealMode::SealPack;
	}

	GSLOG(Error, TEXT("Unknown SealMode"));
	return UGsUIWindowSeal::EGsSealMode::SealNode;
}

bool UGsUIWindowSeal::IsEnableSkip() const
{
	if (nullptr == _skipToggleSwitcher)
	{
		return false;
	}

	return 0 == _skipToggleSwitcher->GetActiveWidgetIndex();
}

const FGsSealNode* UGsUIWindowSeal::GetNodeAt(const int32 InIndex) const
{
	const FGsSealNode* SealNode = nullptr;

	const FGsSealManager* SealManager = GSSeal();
	if (nullptr == SealManager)
	{
		GSLOG(Error, TEXT("nullptr == SealManager"));
	}
	else
	{
		TWeakPtr<const FGsSealPack> SealPack = GetSelectedPack();
		if (!SealPack.IsValid())
		{
			GSLOG(Error, TEXT("!SealPack.IsValid()"));
		}
		else
		{
			SealNode = SealPack.Pin()->GetSealNodeByIndex(InIndex);
			if (nullptr == SealNode)
			{
				GSLOG(Error, TEXT("nullptr == SealNode"));
			}
		}
	}

	return SealNode;
}

TWeakPtr<const FGsSealSlot> UGsUIWindowSeal::GetSlotAt(const int32 InNodeIndex, const int32 InSlotIndex) const
{
	const FGsSealNode* SealNode = GetNodeAt(InNodeIndex);
	if (nullptr == SealNode)
	{
		GSLOG(Error, TEXT("nullptr == SealNode"));
		return nullptr;
	}

	return SealNode->GetSealSlotByIndex(InSlotIndex);
}

void UGsUIWindowSeal::SetParameters(Parameters InParameters, const bool InResetCategory /*= false*/, const bool InAddCurrentHistory /*= true*/)
{
	_parameters = MoveTemp(InParameters);

	if (InResetCategory)
		InvalidateCategoryTab();

	ResetTabs();
	ResetRerollUnlockSelection();
	InvalidateSealPack();
	InvalidateTab();
}

void UGsUIWindowSeal::SetRerollUnlockTargetUnlocked(const int32 InSlotIndex, const EGsSealRerollUnlockTarget InUnlockTarget, const bool InUnlocked)
{
	if (!_sealSlotListItems.IsValidIndex(InSlotIndex))
	{
		GSLOG(Error, TEXT("!_sealSlotListItems.IsValidIndex(%d)"), InSlotIndex);
		return;
	}

	UGsUISealSlotListItem* SealSlotListItem = _sealSlotListItems[InSlotIndex];
	if (nullptr == SealSlotListItem)
	{
		GSLOG(Error, TEXT("!_sealSlotListItems.IsValidIndex(%d)"), InSlotIndex);
		return;
	}

	if (EGsSealRerollUnlockTarget::Stat == InUnlockTarget)
	{
		SealSlotListItem->_unlockStat = InUnlocked;
	}

	if (EGsSealRerollUnlockTarget::Color == InUnlockTarget)
	{
		SealSlotListItem->_unlockColor = InUnlocked;
	}
}

void UGsUIWindowSeal::OnSealLevelUp(const SealPackId InSealPackId, const SealId InSealNodeId)
{
	_sealPackIdJustUpdated = InSealPackId;
	_sealNodeIdJustUpdated = InSealNodeId;

	InvalidateSealPack();
	InvalidateTab(true);
	FGsUIHelper::HideBlockUI();
}

void UGsUIWindowSeal::OnSealSlotChangeAll(const SealPackId InSealPackId, const SealId InSealNodeId)
{
	FGsUIHelper::HideBlockUI();
	
	_sealPackIdJustUpdated = InSealPackId;
	_sealNodeIdJustUpdated = InSealNodeId;

	if (nullptr == _sequencePlayer || !_sequencePlayer->IsValidLowLevel())
	{
		return;
	}

	UWidgetAnimation* RerollAni = IsEnableSkip() ? _rerollSkipAni : _rerollAni;
	_sequencePlayer->Play(this, RerollAni, FGsUISequencePlayerFinished::CreateUObject(this, &UGsUIWindowSeal::OnFinishedSealSlotChangeAllAni));
}

void UGsUIWindowSeal::OnSealSlotLockSave()
{
	_canChangeRerollLockState = true;
}

void UGsUIWindowSeal::OnFinishedSealSlotChangeAllAni(const UWidgetAnimation* InAnimation)
{
	OnUIEvent(EGsUISealWindowEvent::Reset);

	InvalidateSealPack();
	InvalidateTab();
}

void UGsUIWindowSeal::OnChangeCurrency(const CurrencyType InCurrencyType)
{
	InvalidateCost();
}

void UGsUIWindowSeal::OnChangeItem(const ItemId InItemId)
{
	InvalidateCost();
}

void UGsUIWindowSeal::OnUIEvent(const EGsUISealWindowEvent InUIEvent)
{
	GSLOG(Log, TEXT("InUIEvent: %d"), InUIEvent);

	FGsUIWindowSealEventParameters CurrentEventParameters;
	CurrentEventParameters._packId = _sealPackIdJustUpdated;
	CurrentEventParameters._nodeId = _sealNodeIdJustUpdated;
	CurrentEventParameters._event = InUIEvent;
	CurrentEventParameters._isSkip = IsEnableSkip();

	switch (InUIEvent)
	{
	case EGsUISealWindowEvent::Reset:
		break;

	case EGsUISealWindowEvent::SealNode:
		break;

	case EGsUISealWindowEvent::SealNodeValue:
		break;

	case EGsUISealWindowEvent::SealSlot0:
	{
		CurrentEventParameters._slotIndex = 0;
		break;
	}

	case EGsUISealWindowEvent::SealSlot1:
	{
		CurrentEventParameters._slotIndex = 1;
		break;
	}

	case EGsUISealWindowEvent::SealSlot2:
	{
		CurrentEventParameters._slotIndex = 2;
		break;
	}

	case EGsUISealWindowEvent::SealSlotValue0:
	{
		CurrentEventParameters._slotIndex = 0;
		InvalidateSealSlotListEntry(CurrentEventParameters._slotIndex, false);
		break;
	}

	case EGsUISealWindowEvent::SealSlotValue1:
	{
		CurrentEventParameters._slotIndex = 1;
		InvalidateSealSlotListEntry(CurrentEventParameters._slotIndex, false);
		break;
	}

	case EGsUISealWindowEvent::SealSlotValue2:
	{
		CurrentEventParameters._slotIndex = 2;
		InvalidateSealSlotListEntry(CurrentEventParameters._slotIndex, false);
		break;
	}

	case EGsUISealWindowEvent::SealLines:
		break;

	case EGsUISealWindowEvent::SealLinesValue:
	{
		InvalidateSealPack();
		break;
	}
	}

	_onUiEvent.Broadcast(CurrentEventParameters);
}

void UGsUIWindowSeal::OnSelectChangedNode()
{
	ResetNodeSelections();

	if (SEAL_SUB_TAB_SEALPACK == _subTabToggleGroup.GetSelectedIndex())
	{
		_subTabToggleGroup.SetSelectedIndex(SEAL_SUB_TAB_SEALNODE);
	}

	InvalidateSealPack();
	InvalidateTab();
}

void UGsUIWindowSeal::OnSelectChangedSealPackGainTabBtn(int32 InIndex)
{
	_selectedSealPackGainTabIndex = InIndex;

	InvalidateTab();
}

void UGsUIWindowSeal::OnSelectChangedSubTabBtn(int32 InIndex)
{
	InvalidateTab();
}

void UGsUIWindowSeal::OnFinishedLongPressEventMaterialItem(UGsUIIconItem& InIcon)
{
	UGsUIIconItem::DefaultItemDetailOpener(InIcon, true);
}

void UGsUIWindowSeal::OnRefreshEntryEnchantMaterialList(int32 InIndex, UWidget* InEntry)
{
	UGsUISealMaterialListitem* ListItem = Cast<UGsUISealMaterialListitem>(InEntry);
	if (nullptr == ListItem)
	{
		GSLOG(Error, TEXT("nullptr == ListItem, InIndex:%d"), InIndex);
		return;
	}

	if (nullptr == _itemCostPackage || !_itemCostPackage.IsValid())
	{
		ListItem->SetParameters(UGsUISealMaterialListitem::Parameters(
			{
				INVALID_ITEM_ID,
				0,
				0,
				true
			}));
	}
	else
	{
		const FGsCostElement* costElement = _itemCostPackage->GetCostElementAt(InIndex);
		if (nullptr == costElement)
		{
			GSLOG(Error, TEXT("nullptr == costElement, InIndex:%d"), InIndex);
			return;
		}

		ListItem->SetParameters(UGsUISealMaterialListitem::Parameters(
			{
				costElement->GetItemId(),
				costElement->GetCostAmount(),
				costElement->PlayerHasAmountMax(),
				costElement->PlayerHasEnough()
			}));
	}
}

void UGsUIWindowSeal::OnClickShowSealSlotGainBtn()
{
	const FGsSealNode* SealNode = GetSelectedNode();
	if (nullptr == SealNode)
	{
		GSLOG(Error, TEXT("nullptr == SealNode"));
		return;
	}

	UGsUIPopupSealNodeGainPool::OpenThis({ SealNode->GetSealNodeId(), SealNode->GetLevel(), SealNode->GetLevel() });
}

void UGsUIWindowSeal::OnClickSkipButton()
{
	_skipToggleSwitcher->SetActiveWidgetIndex(IsEnableSkip() ? 1 : 0);
}

void UGsUIWindowSeal::OnClickTryButton()
{
	const EGsSealMode SealMode = GetSelectedSealMode();
	if (EGsSealMode::SealNode == SealMode)
	{
		TryToLevelup();
	}
	else if (EGsSealMode::SealSlot == SealMode)
	{
		TryToRerollFirst();
	}
	else
	{
		GSLOG(Warning, TEXT("Unknow Seal Mode"));
	}
}

void UGsUIWindowSeal::OnRefreshEntryCategoryTabBtn(int32 InIndex, UWidget* InEntry)
{
	UGsUITabButtonCommon* tabButtonCommon = Cast<UGsUITabButtonCommon>(InEntry);
	if (nullptr == tabButtonCommon)
	{
		GSLOG(Error, TEXT("nullptr == tabButtonCommon, *InEntry->GetName():%s"), *InEntry->GetName());
		return;
	}

	const FGsSealPack* PackData = GetPackWithIndex(InIndex).Pin().Get();
	if (nullptr == PackData)
	{
		GSLOG(Error, TEXT("nullptr == PackData"));
		return;
	}

	// 잠금 상태 갱신
	if (PackData->GetSealState() == EGsSealState::Unknown || PackData->GetSealState() == EGsSealState::Deactivated)
	{
		tabButtonCommon->SetTitleText(FText::GetEmpty());
		tabButtonCommon->SetLockedText(PackData->GetSealPackTitle());
		tabButtonCommon->SetIsEnabled(false);
	}
	else
	{
		tabButtonCommon->SetTitleText(PackData->GetSealPackTitle());
		tabButtonCommon->SetLockedText(FText::GetEmpty());
		tabButtonCommon->SetIsEnabled(true);
	}

	IGsToggleGroupEntry* toggleGroupEntry = tabButtonCommon->GetToggleGroupEntry();
	if (nullptr == toggleGroupEntry)
	{
		GSLOG(Error, TEXT("nullptr == toggleGroupEntry"));
		return;
	}
	else
	{
		toggleGroupEntry->SetIsSelected(false);
		_toggleGroupCategoryTabBtns.AddToggleWidget(tabButtonCommon->GetToggleGroupEntry());
	}
}

void UGsUIWindowSeal::OnSelectChangedCategoryTabBtn(int32 InIndex)
{
	if (InIndex < 0)
	{
		return;
	}

	_sealPackSwitcher->SetActiveWidgetIndex(InIndex);

	const FGsSealManager* SealManager = GSSeal();
	if (nullptr == SealManager)
	{
		GSLOG(Error, TEXT("nullptr == SealManager"));
		return;
	}

	const FGsSealPack* PackData = SealManager->GetSealPackWithIndex(InIndex).Pin().Get();
	if (nullptr == PackData)
	{
		GSLOG(Error, TEXT("nullptr == PackData"));
		return;
	}

	SealPackId PackId = PackData->GetSealPackId();
	_parameters._selectedPackId = PackId;

	SetParameters(UGsUIWindowSeal::Parameters(PackId));
}

void UGsUIWindowSeal::OnClickSealRerollUnlockTarget(const int32 InSlotIndex, const EGsSealRerollUnlockTarget InUnlockTarget)
{
	TWeakPtr<const FGsSealPack> SealPackPtr = GetSelectedPack();
	if (!SealPackPtr.IsValid())
	{
		GSLOG(Error, TEXT("!SealPackPtr.IsValid()"));
		return;
	}

	TSharedPtr<const FGsSealPack> SealPack = SealPackPtr.Pin();

	const FGsSealNode* SealNode = GetSelectedNode();
	if (nullptr == SealNode)
	{
		GSLOG(Error, TEXT("nullptr == SealNode"));
		return;
	}

	TWeakPtr<const FGsSealSlot> SealSlotPtr = SealNode->GetSealSlotByIndex(InSlotIndex);
	if (!SealSlotPtr.IsValid())
	{
		GSLOG(Error, TEXT("!SealSlotPtr.IsValid()"));
		return;
	}

	TSharedPtr<const FGsSealSlot> SealSlot = SealSlotPtr.Pin();

	const bool IsTargetUnlocked = SealSlot->IsRerollUnlock(InUnlockTarget);

	if (!SealSlot->CanRerollLock())
	{
		// 리롤 잠금 기능을 아직 사용할 수 없음.
		FText TickerMessage;
		FText::FindText(TEXT("SealText"), TEXT("UI_Slot_Reroll_NotYet"), TickerMessage);
		FGsUIHelper::TrayTickerLauncher(TickerMessage);
	}
	else if (SealPack->GetSealRerollLockCountMax() <= GetCurrentRerollLockCount() && IsTargetUnlocked)
	{
		// 리롤 잠금 최대 갯수 초과하여 시도
		FText TickerMessage;
		FText::FindText(TEXT("SealText"), TEXT("UI_Slot_Reroll_Lock_Max"), TickerMessage);
		FGsUIHelper::TrayTickerLauncher(TickerMessage);
	}
	else
	{
		// 정상 시도 가능
		if (!_canChangeRerollLockState)
			return;

		FGsSealMessageParamTrySetRerollSlotUnlock(SealPack->GetSealPackId(), SealNode->GetSealNodeId(), SealSlot->GetSealSlotId(), InUnlockTarget, !IsTargetUnlocked).Send();

		// 서버로 잠금 데이터 저장 요청
		bool StatUnlocked = GSSeal()->GetSealSlotData(SealSlot->GetSealSlotId())->IsRerollUnlock(EGsSealRerollUnlockTarget::Stat);
		bool ColorUnlocked = GSSeal()->GetSealSlotData(SealSlot->GetSealSlotId())->IsRerollUnlock(EGsSealRerollUnlockTarget::Color);

		if (InUnlockTarget == EGsSealRerollUnlockTarget::Stat)
			FGsSealMessageParamReqSealSlotLockSave(SealSlot->GetSealSlotId(), !IsTargetUnlocked, ColorUnlocked).Send();
		else if (InUnlockTarget == EGsSealRerollUnlockTarget::Color)
			FGsSealMessageParamReqSealSlotLockSave(SealSlot->GetSealSlotId(), StatUnlocked, !IsTargetUnlocked).Send();

		_canChangeRerollLockState = false;

		// 사운드
		const FString SoundName = SealSlot->IsRerollUnlock(InUnlockTarget) ? TEXT("UI_Common_Unlock") : TEXT("UI_Common_Lock");
		FGsUIHelper::Play2DSound(SoundName);
	}

	InvalidateSealSlotListArea();
	InvalidateCost();
}

void UGsUIWindowSeal::OnArrivedFullProgressOnAnimation()
{
	//GSLOG(Log, TEXT("UGsUIWindowSeal::OnArrivedFullProgressOnAnimation()"));
	FGsUIHelper::Play2DSound(TEXT("UI_Seal_LevelUp"));
}

void UGsUIWindowSeal::OnUnlockCategory()
{
	InvalidateCategoryTab();
}
