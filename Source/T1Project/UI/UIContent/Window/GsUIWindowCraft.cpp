#include "GsUIWindowCraft.h"

#include "Runtime/Core/Public/Containers/CircularQueue.h"
#include "Runtime/UMG/Public/Binding/VisibilityBinding.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/Components/Image.h"
#include "Runtime/UMG/Public/Components/ListView.h"
#include "Runtime/UMG/Public/Components/PanelWidget.h"
#include "Runtime/UMG/Public/Components/ProgressBar.h"
#include "Runtime/UMG/Public/Components/ScrollBox.h"
#include "Runtime/UMG/Public/Components/SlateWrapperTypes.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Runtime/UMG/Public/Components/Widget.h"
#include "Runtime/UMG/Public/Components/WidgetSwitcher.h"

#include "../Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperSprite.h"
#include "Classes/GsMessageHandler.h"
#include "Classes/GsPoolUObject.h"
#include "Craft/GsUICraftExpectedListItem.h"
#include "Craft/GsUICraftTargetListEntry.h"
#include "Currency/GsCostPackage.h"
#include "DataSchema/GsSchemaEnums.h"
#include "DataSchema/Item/Craft/GsSchemaItemCraftRecipe.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsGameBase.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateCraft.h"
#include "GameFlow/GameContents/GsContentsMode.h"
#include "GameFlow/GsGameFlowGame.h"
#include "Item/Craft/GsCraftTab.h"
#include "Item/Craft/GsCraftTarget.h"
#include "Item/Craft/GsCraftTargetObject.h"
#include "Item/GsItem.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsCraftManager.h"
#include "Management/ScopeGame/GsSupplyManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Market/GsUIMarketSubCategoryListEntry.h"
#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsCraftMessageParam.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "T1Project.h"
#include "UI/UIContent/Common/GsUITabButtonCommon.h"
#include "UI/UIContent/Common/Numpad/TGsCalculateHelper.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "UI/UIContent/Define/GsUIContentDefine.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Popup/GsUIPopupCalculator.h"
#include "UI/UIContent/Popup/GsUIPopupItemDetailMain.h"
#include "UI/UIContent/Popup/GsUIPopupSortCandidateGoods.h"
#include "UI/UIContent/Window/Craft/GsUICraftMaterialListItem.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "UI/UILib/Base/GsUIVFX.h"
#include "UI/UILib/Base/GsUIWidgetBase.h"
#include "UTIL/GsLocalizationUtil.h"
#include "UTIL/GsTimeSyncUtil.h"

static int32 GetIndexInList(const UListView* InListView, const UObject* InObject)
{
	if (nullptr != InListView || nullptr != InObject)
	{
		const TArray<UObject*>& ListItems = InListView->GetListItems();
		for (int32 i = ListItems.Num() - 1; 0 <= i; --i)
		{
			if (ListItems[i] == InObject)
			{
				return i;
			}
		}
	}
	return -1;
}

const FName UGsUIWindowCraft::WINDOW_ASSET_NAME = TEXT("WindowCraft");

const int32 UGsUIWindowCraft::CALCULATE_HELPER_MIN_VALUE = 1;

const int32 UGsUIWindowCraft::TAB_ROUTE_MIN_DEPTH = 3;

void UGsUIWindowCraft::FGsSimpleScenePlayer::Add(UWidget* InWidget)
{
	_widgets.AddUnique(InWidget);
}

void UGsUIWindowCraft::FGsSimpleScenePlayer::Start(FSimpleDelegate InOnFinishDelegate)
{
	_finished = false;

	_inOnFinishDelegate = MoveTemp(InOnFinishDelegate);

	for (TWeakObjectPtr<UWidget> WidgetPtr : _widgets)
	{
		if (!WidgetPtr.IsValid())
		{
			GSLOG(Warning, TEXT("!WidgetPtr.IsValid()"));
			continue;
		}

		if (UGsUIVFX* Vfx = Cast<UGsUIVFX>(WidgetPtr))
		{
			Vfx->ShowVFX();
		}
	}
}

void UGsUIWindowCraft::FGsSimpleScenePlayer::Rewind()
{
	_finished = false;

	_inOnFinishDelegate.Unbind();

	for (TWeakObjectPtr<UWidget> WidgetPtr : _widgets)
	{
		if (!WidgetPtr.IsValid())
		{
			GSLOG(Warning, TEXT("!WidgetPtr.IsValid()"));
			continue;
		}

		if (UGsUIVFX* Vfx = Cast<UGsUIVFX>(WidgetPtr))
		{
			Vfx->HideVFX();
		}
	}
}

void UGsUIWindowCraft::FGsSimpleScenePlayer::Finish()
{
	if (_inOnFinishDelegate.IsBound())
	{
		_inOnFinishDelegate.Execute();
		_inOnFinishDelegate.Unbind();
	}

	_finished = true;
}

bool UGsUIWindowCraft::FGsSimpleScenePlayer::IsFinished() const
{
	return _finished;
}

void UGsUIWindowCraft::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_craftTargetListItemsPool = NewObject<UGsPoolUObject>();
	_helpContentsKey = EGsHelpContents::Craft;

	GSLOG(Log, TEXT("UGsUIWindowCraft::NativeOnInitialized()"));

#if WITH_EDITOR
	if (nullptr == _subClassOfMainTabBtn || nullptr == _subClassOfSubTabBtn || nullptr == _subClassOfSub2ndTabBtn || nullptr == _subClassOfCraftTargetBtn || nullptr == _subClassOfCraftMaterialBtn)
	{
		GSLOG(Error, TEXT("필수 오브젝트 링크 오류"));
	}
#endif

	// 제작 연출
	_scenePlayer.Add(_vfxOnCraftIcon);

	// 상단 버튼
	_btnBack->OnClicked.AddUniqueDynamic(this, &UGsUIWindowCraft::OnClickBack);
	_btnClose->OnClicked.AddUniqueDynamic(this, &UGsUIWindowCraft::OnClickCloseAllStack);
	_btnCraftBack->OnClicked.AddUniqueDynamic(this, &UGsUIWindowCraft::OnClickCraftBack);
	
	// 메인탭
	_slotHelperMainTabBtns = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperMainTabBtns->Initialize(_subClassOfMainTabBtn, _scrollMainTabBtns);
	_slotHelperMainTabBtns->OnRefreshEntry.AddUniqueDynamic(this, &UGsUIWindowCraft::OnRefreshEntryMainTabBtn);
	_toggleGroupMainTabBtns.OnSelectChanged.BindUObject(this, &UGsUIWindowCraft::OnSelectChangedMainTabBtn);

	// 서브탭
	_slotHelperSubTabBtns = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperSubTabBtns->Initialize(_subClassOfSubTabBtn, _scrollSubTabBtns);
	_slotHelperSubTabBtns->OnRefreshEntry.AddUniqueDynamic(this, &UGsUIWindowCraft::OnRefreshEntrySubTabBtn);
	_toggleGroupSubTabBtns.OnSlotClicked.BindUObject(this, &UGsUIWindowCraft::OnSlotClickedSubTabBtn);
	_toggleGroupSubTabBtns.OnSelectChanged.BindUObject(this, &UGsUIWindowCraft::OnSelectChangedSubTabBtn);

	// 2nd서브탭
	_slotHelperSub2ndTabBtns = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperSub2ndTabBtns->Initialize(_subClassOfSub2ndTabBtn, _scrollSub2ndTabBtns);
	_slotHelperSub2ndTabBtns->OnRefreshEntry.AddUniqueDynamic(this, &UGsUIWindowCraft::OnRefreshEntrySub2ndTabBtn);
	_toggleGroupSub2ndTabBtns.OnSlotClicked.BindUObject(this, &UGsUIWindowCraft::OnSlotClickedSub2ndTabBtn);
	_toggleGroupSub2ndTabBtns.OnSelectChanged.BindUObject(this, &UGsUIWindowCraft::OnSelectChangedSub2ndTabBtn);

	// 제작 대상 목록
	//_slotHelperCraftTargetBtns = NewObject<UGsDynamicPanelSlotHelper>(this);
	//_slotHelperCraftTargetBtns->Initialize(_subClassOfCraftTargetBtn, _scrollCraftTargetBtns);
	//_slotHelperCraftTargetBtns->OnRefreshEntry.AddUniqueDynamic(this, &UGsUIWindowCraft::OnRefreshEntryCraftTargetBtn);
	//_toggleGroupCraftTargetBtns.OnSelectChanged.BindUObject(this, &UGsUIWindowCraft::OnSelectChangedCraftTargetBtn);
	_craftTargetList->OnItemSelectionChanged().AddUObject(this, &UGsUIWindowCraft::OnCraftTargetListItemSelectionChanged);
	_craftTargetList->OnItemClicked().AddUObject(this, &UGsUIWindowCraft::OnClickCraftTargetListItem);

	// 제작 대상 상세 정보
	_btnDetailSwitcher->OnClicked.AddUniqueDynamic(this, &UGsUIWindowCraft::OnClickedDetailSwitcher);
	_btnCraftTargetIcon->OnClicked.AddUniqueDynamic(this, &UGsUIWindowCraft::OnClickedCraftTargetIcon);
	_calculateHelper._onChanged.BindUObject(this, &UGsUIWindowCraft::OnChangedCalculateHelper);
	_calculateHelper.SetMin(UGsUIWindowCraft::CALCULATE_HELPER_MIN_VALUE);

	// 하단
	_btnCalcReset->OnClicked.AddUniqueDynamic(this, &UGsUIWindowCraft::OnClickedCalcReset);
	_btnCalcPlus10->OnClicked.AddUniqueDynamic(this, &UGsUIWindowCraft::OnClickedCalcPlus10);
	_btnCalcMax->OnClicked.AddUniqueDynamic(this, &UGsUIWindowCraft::OnClickedCalcMax);
	_btnCalcMinus1->OnClicked.AddUniqueDynamic(this, &UGsUIWindowCraft::OnClickedCalcMinus1);
	_btnCalcPlus1->OnClicked.AddUniqueDynamic(this, &UGsUIWindowCraft::OnClickedCalcPlus1);
	_btnCalcOpen->OnClicked.AddUniqueDynamic(this, &UGsUIWindowCraft::OnClickedCalcOpen);
	_btnCraft->OnClicked.AddUniqueDynamic(this, &UGsUIWindowCraft::OnClickedCraft);
	_btnCraftCancel->OnClicked.AddUniqueDynamic(this, &UGsUIWindowCraft::OnClickedCraftCancel);

	// 제작 예상 목록
	_craftExpectedListItems.Empty();

	if (nullptr == _craftExpectedListItem0)
	{
		GSLOG(Error, TEXT("nullptr == _craftExpectedListItem0"));
	}
	else
	{
		_craftExpectedListItems.Add(_craftExpectedListItem0);
	}

	if (nullptr == _craftExpectedListItem1)
	{
		GSLOG(Error, TEXT("nullptr == _craftExpectedListItem1"));
	}
	else
	{
		_craftExpectedListItems.Add(_craftExpectedListItem1);
	}

	if (nullptr == _craftExpectedListItem2)
	{
		GSLOG(Error, TEXT("nullptr == _craftExpectedListItem2"));
	}
	else
	{
		_craftExpectedListItems.Add(_craftExpectedListItem2);
	}

	// 제작 재료 목록
	_slotHelperCraftMaterialBtns = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperCraftMaterialBtns->Initialize(_subClassOfCraftMaterialBtn, _scrollCraftMaterialBtns);
	_slotHelperCraftMaterialBtns->OnRefreshEntry.AddUniqueDynamic(this, &UGsUIWindowCraft::OnRefreshEntryCraftMaterialBtn);

	// UI 출력에 필요한 텍스트 미리 세팅
	FText endTimeSuffixText;
	FText::FindText(TEXT("ItemCraftText"), TEXT("Craft_UI_displayTime"), endTimeSuffixText);
	_endTimeSuffix = endTimeSuffixText.ToString();
}

void UGsUIWindowCraft::NativeConstruct()
{
	GSLOG(Log, TEXT("UGsUIWindowCraft::NativeConstruct()"));

	Super::NativeConstruct();

	_isOpeningPopupCalculator = false;
	SetSpellingState(false);
	SetVisibilitySub2ndTabBtns(false);

	ResetAllDatas();
}

bool UGsUIWindowCraft::SyncContentState()
{
	FGsMessageHolder* MessageHolder = GMessage();
	if (nullptr == MessageHolder)
	{
		GSLOG(Error, TEXT("nullptr == MessageHolder"));
		return false;
	}

	if (false == IsSameContentState(FGsContentsMode::InGame::ContentsCraft))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_CRAFT);
		return true;
	}

	return false;
}

void UGsUIWindowCraft::NativeDestruct()
{
	GSLOG(Log, TEXT("UGsUIWindowCraft::NativeDestruct()"));

	if (IsStackRecoverFlag())
	{
		// 스태킹이벤트라면 히스토리에 저장
		AddToHistoryQueueThis();
	}
	else
	{
		// 아니라면 데이터 클리어
		FGsGameStateCraft* GameStateCraft = UGsUIWindowCraft::GetGameStateCraft();
		if (nullptr != GameStateCraft)
		{
			GameStateCraft->ClearHistory();
		}
	}

	_isOpeningPopupCalculator = false;
	_isBoxDetailResult = 0;
	SetSpellingState(false);

	ResetAllDatas();

	SetTutorial(false);
	SetIsTutorialInteraction(false);

	Super::NativeDestruct();
}

void UGsUIWindowCraft::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	InvalidateSpelling();
}

void UGsUIWindowCraft::InvalidateCraftTargetListEntries(const bool InResetFirst /*= false*/)
{
	if (InResetFirst)
	{
		// 제작리스트 최상위로 스크롤
		_craftTargetList->ScrollToTop();

		// 이전 선택 상태를 클리어하고, 기존 제작리스트 클리어
		_craftTargetList->ClearListItems();
		_craftTargetList->SetSelectedIndex(-1);
		//_toggleGroupCraftTargetBtns.SetSelectedIndex(-1);
		//_toggleGroupCraftTargetBtns.Clear();
	}

	_craftTargetList->RegenerateAllEntries();
	_craftTargetList->SetListItems(_craftTargetListItems);
	_craftTargetList->SetScrollbarVisibility(ESlateVisibility::Collapsed);
}

void UGsUIWindowCraft::InvalidateUIInterationState()
{
	const bool CanUIIneraction = !IsSpellingState() && !_isOpeningPopupCalculator;
	const ESlateVisibility CurrentVisibility = CanUIIneraction ? ESlateVisibility::Visible : ESlateVisibility::HitTestInvisible;

	_btnCraftBack->SetIsEnabled(CanUIIneraction);

	_scrollMainTabBtns->SetVisibility(CurrentVisibility);
	_scrollSubTabBtns->SetVisibility(CurrentVisibility);
	_scrollSub2ndTabBtns->SetVisibility(CurrentVisibility);
	_craftTargetList->SetVisibility(CurrentVisibility);
	_boxDetail->SetVisibility(CurrentVisibility);
	_boxCraftAmount->SetVisibility(CurrentVisibility);
}

void UGsUIWindowCraft::InvalidateMainTabBtns()
{
	_toggleGroupMainTabBtns.Clear();
	_toggleGroupMainTabBtns.SetSelectedIndex(-1);

	//GSLOG(Log, TEXT("UGsUIWindowCraft::InvalidateMainTabBtns(), _cachedCraftMainTabs.Num():%d"), _cachedCraftMainTabs.Num());
	_slotHelperMainTabBtns->RefreshAll(_cachedCraftMainTabs.Num());
}

void UGsUIWindowCraft::InvalidateSubTabBtns(const bool InResetScroll /*= false*/)
{
	if (InResetScroll)
	{
		_scrollSubTabBtns->EndInertialScrolling();
		_scrollSubTabBtns->ScrollToStart();
	}

	//GSLOG(Log, TEXT("UGsUIWindowCraft::InvalidateSubTabBtns(), _cachedCraftSubTabs.Num():%d"), _cachedCraftSubTabs.Num());
	_slotHelperSubTabBtns->RefreshAll(_cachedCraftSubTabs.Num());

	const UGsCraftManager* CraftManager = GSCraft();
	if (nullptr == CraftManager)
	{
		GSLOG(Error, TEXT("nullptr == CraftManager"));
		return;
	}

	const bool ShowEmptyFavoriteCraftTargetMsg = (0 == _craftTargetListItems.Num()) && (CraftManager->GetFavoriteTab() == GetSelectedTabTail());
	_panelEmptyFavoriteHelp->SetVisibility(ShowEmptyFavoriteCraftTargetMsg ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UGsUIWindowCraft::InvalidateSub2ndTabBtns()
{
	_scrollSub2ndTabBtns->EndInertialScrolling();
	_scrollSub2ndTabBtns->ScrollToStart();

	_toggleGroupSub2ndTabBtns.Clear();
	_toggleGroupSub2ndTabBtns.SetSelectedIndex(-1);

	//GSLOG(Log, TEXT("UGsUIWindowCraft::InvalidateSub2ndTabBtns(), _cachedCraftSub2ndTabs.Num():%d"), _cachedCraftSub2ndTabs.Num());
	_slotHelperSub2ndTabBtns->RefreshAll(_cachedCraftSub2ndTabs.Num());
}

void UGsUIWindowCraft::InvalidateCraftTargetListEntry(const int32 InIndex)
{
	UGsUICraftTargetListEntry* ListEntry = _craftTargetList->GetEntryWidgetFromItem<UGsUICraftTargetListEntry>(_craftTargetList->GetItemAt(InIndex));
	if (nullptr != ListEntry)
	{
		ListEntry->Invalidate();
	}
}

void UGsUIWindowCraft::InvalidateCraftTargetDetail()
{
	static constexpr int32 CRAFT_TARGET_DETAIL_DEFAULT = 0;
	static constexpr int32 CRAFT_TARGET_DETAIL_EMPTY = 1;

	const FGsCraftTarget* CraftTarget = GetSelectedCraftTarget();
	if (nullptr == CraftTarget)
	{
		_switcherDetail->SetActiveWidgetIndex(CRAFT_TARGET_DETAIL_EMPTY);

		const UGsCraftManager* CraftManager = GSCraft();
		if (nullptr == CraftManager)
		{
			GSLOG(Error, TEXT("nullptr == CraftManager"));
			return;
		}

		const bool ShowSelectCraftTargetMsg = (0 < _craftTargetListItems.Num()) || (CraftManager->GetFavoriteTab() != GetSelectedTabTail());
		_txtEmptySelectionHelp->SetVisibility(ShowSelectCraftTargetMsg ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

		return;
	}

	_switcherDetail->SetActiveWidgetIndex(CRAFT_TARGET_DETAIL_DEFAULT);

	// 이름
	{
		_txtCraftName->SetText(CraftTarget->GetCraftName());
		const FGsCraftTargetObject* CraftTargetObject = CraftTarget->GetDefaultCraftTargetObject();
		_txtCraftName->SetColorAndOpacity((nullptr != CraftTargetObject) ? CraftTargetObject->GetCommonColor() : FGsUIColorHelper::GetColorItemGrade(ItemGrade::NONE));
	}

	// 실패 공지
	_containerFailGuide->SetVisibility(CraftTarget->CanBeFail() ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	// icon
	{
		const FSoftObjectPath IconObjectPath = CraftTarget->GetDefaultCraftTargetObjectIconPath();
		if (IconObjectPath.IsValid())
		{
			UPaperSprite* IconPaperSprite = FGsUIHelper::LoadSpriteFromPath(IconObjectPath);
			if (nullptr == IconPaperSprite)
			{
				GSLOG(Error, TEXT("nullptr == IconPaperSprite, IconObjectPath:%s"), *IconObjectPath.ToString());
			}
			else
			{
				_imgCraftTargetIcon->SetBrushFromAtlasInterface(IconPaperSprite);
			}
		}
		else
		{
			GSLOG(Error, TEXT("NOT IconObjectPath.IsValid(), CraftTarget->_craftTargetId:%d"), CraftTarget->_craftTargetId);
		}
	}

	FText SuccessRatioTxt = FText::GetEmpty();
	if (CraftTarget->CanBeGreatSuccess())
	{
		// 대성공
		FText FormatTxt;
		if (FText::FindText(TEXT("ItemCraftText"), TEXT("Craft_UI_ResultInfo_GreatSuccessProb"), FormatTxt))
		{
			SuccessRatioTxt = FText::Format(FormatTxt, FText::AsPercent(CraftTarget->GetGreatSuccessRatio()), FText::AsPercent(CraftTarget->GetSuccessRatio()));
		}
		else
		{
			GSLOG(Error, TEXT("NOT FText::FindText(TEXT(\"ItemCraftText\"), TEXT(\"Craft_UI_ResultInfo_GreatSuccessProb\"))"));
		}
	}
	else if (CraftTarget->CanBeSuccess())
	{
		// 성공
		FText FormatTxt;
		if (FText::FindText(TEXT("ItemCraftText"), TEXT("Craft_UI_ResultInfo_SuccessProb"), FormatTxt))
		{
			SuccessRatioTxt = FText::Format(FormatTxt, FText::AsPercent(CraftTarget->GetSuccessRatio()));
		}
		else
		{
			GSLOG(Error, TEXT("NOT FText::FindText(TEXT(\"ItemCraftText\"), TEXT(\"Craft_UI_ResultInfo_SuccessProb\"))"));
		}
	}
	_txtSuccessRatio->SetText(SuccessRatioTxt);

	{
		// 서버별 제작 개수 제한
		const bool HasLimit = 0 <= CraftTarget->GetLimitCountServer();
		_containerLimitCountServer->SetVisibility(HasLimit ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		if (HasLimit)
		{
			_txtLimitCountServer->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), CraftTarget->GetLimitCountServerCrafted(), CraftTarget->GetLimitCountServer())));
		}
	}

	{
		// 계정 서버별 제작 개수 제한
		const bool HasLimit = 0 <= CraftTarget->GetLimitCountAccountByServer();
		_containerLimitCountAccountByServer->SetVisibility(HasLimit ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		if (HasLimit)
		{
			_txtLimitCountAccountByServer->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), CraftTarget->GetLimitCountAccountByServerCrafted(), CraftTarget->GetLimitCountAccountByServer())));
		}
	}

	{
		const bool HasReset = ResetTimeTicket::RTT_NONE != CraftTarget->GetLimitCountResetTimeTicket();
		_containerReset->SetVisibility(HasReset ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		if (HasReset)
		{
			_txtReset->SetText(FGsEnumLocalizedText::GetText(CraftTarget->GetLimitCountResetTimeTicket()));
		}
	}

	{
		// 만약 종료 시간이 '상시' 에 해당하는 매직 타임일 경우, 표시 하지 않는다... (하고 싶어서 이렇게 처리한 것이 아님)
		const bool HasDisplayTimeEnd = CraftTarget->HasDisplayUtcRange() && !CraftTarget->IsEndTimeMagicTime();

		_containerExpire->SetVisibility(HasDisplayTimeEnd ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		if (HasDisplayTimeEnd)
		{
			FString endUtcString = CraftTarget->GetDisplayEndLocalString();
			endUtcString.Append(_endTimeSuffix);
			_txtExpire->SetText(FText::FromString(endUtcString));
		}
	}

	// 제작 예상 목록
	{
		static const int8 CRAFT_EXPECTED_LIST_ITEM_MAX_COUNT = 3;
		static const CraftResult CRAFT_EXPECTED_LIST_ITEM_ORDER[CRAFT_EXPECTED_LIST_ITEM_MAX_COUNT] = {
			CraftResult::CRAFT_SUCCEEDED_GREAT, CraftResult::CRAFT_SUCCEEDED, CraftResult::CRAFT_FAIL
		};

		_craftExpectedListItemParameters.Empty();

		// 데이터 입력
		for (int32 i = 0; i < CRAFT_EXPECTED_LIST_ITEM_MAX_COUNT; ++i)
		{
			const CraftResult CurrentCraftResult = CRAFT_EXPECTED_LIST_ITEM_ORDER[i];
			const FGsCraftTargetObject* CraftTargetObject = CraftTarget->GetCraftTargetObjectByResult(CurrentCraftResult);

			if (nullptr != CraftTargetObject)
			{
				UGsUICraftExpectedListItem::Parameters CurrentParameters {
					CurrentCraftResult,
					*CraftTargetObject,
					CraftTargetObject->GetItemCount(),
					CraftTargetObject->GetItemLevel()
				};

				_craftExpectedListItemParameters.Add(CurrentParameters);
			}
		}

		// 목록 새로고침
		for (int32 i = 0; i < _craftExpectedListItems.Num(); ++i)
		{
			UGsUICraftExpectedListItem* CraftExpectedListItem = _craftExpectedListItems[i];
			check(!!CraftExpectedListItem);

			if (!_craftExpectedListItemParameters.IsValidIndex(i))
			{
				CraftExpectedListItem->SetVisibility(ESlateVisibility::Collapsed);
				continue;
			}

			CraftExpectedListItem->SetVisibility(ESlateVisibility::Visible);
			OnRefreshEntryCraftExpectedListItem(i, CraftExpectedListItem);
		}

		// 성공 확률
		_txtCraftExpectedSuccessRatio->SetText(SuccessRatioTxt);
	}

	InvalidateCalculateHelper();
	InvalidateCraftMaterialBtns();

	// 상세부 토글 리셋
	_boxDetailSwitcher->SetActiveWidgetIndex(_isBoxDetailResult);
}

void UGsUIWindowCraft::InvalidateSpelling()
{
	if (!IsSpellingState())
	{
		return;
	}

	if (0.f != _spellingDurationSecs)
	{
		const float Percent = (FDateTime::Now() - _spellingStartTime).GetTicks() / (float)(FTimespan::FromSeconds(_spellingDurationSecs).GetTicks());
		_progressCraft->SetPercent(FMath::Clamp(Percent, 0.f, 1.f));

		if (1.f <= Percent)
		{
			_scenePlayer.Finish();
		}
	}
}

void UGsUIWindowCraft::ResetCraftMaterialBtns()
{
	_scrollCraftMaterialBtns->EndInertialScrolling();
	_scrollCraftMaterialBtns->ScrollToStart();
	_toggleGroupCraftMaterialBtns.Clear();

	DeselectCraftMaterialBtns();
}

void UGsUIWindowCraft::DeselectCraftMaterialBtns()
{
	_toggleGroupCraftMaterialBtns.SetSelectedIndex(-1);
}

void UGsUIWindowCraft::InvalidateCraftMaterialBtns()
{
	TSharedPtr<FGsCostPackage> CostPackage = GetSelectedCraftTargetCostPackage();
	if (!CostPackage.IsValid())
	{
		return;
	}

	_slotHelperCraftMaterialBtns->RefreshAll(CostPackage->GetCostElementsCount());
}

void UGsUIWindowCraft::InvalidateCalculateHelper()
{
	const FGsCraftTarget* CraftTarget = GetSelectedCraftTarget();
	if (nullptr == CraftTarget)
	{
		return;
	}

	const int32 PlayerCanCraftMaxCount = CraftTarget->GetPlayerCanCraftMaxCount();

	const int32 MaxAmount = FMath::Clamp(PlayerCanCraftMaxCount, _calculateHelper.GetMin(), TNumericLimits<int32>::Max());
	_calculateHelper.SetMax(MaxAmount);

	const int32 CurrentValue = _calculateHelper.GetCurrent();

	_btnCalcReset->SetIsEnabled(!_calculateHelper.IsMin());
	_btnCalcPlus10->SetIsEnabled(!_calculateHelper.IsMax());
	_btnCalcMax->SetIsEnabled(!_calculateHelper.IsMax());
	_btnCalcMinus1->SetIsEnabled(!_calculateHelper.IsMin());

	_txtCalcCurrent->SetText(FText::AsNumber(CurrentValue));
	const EGsUIColorType TxtCalcCurrentColorType = (CurrentValue <= PlayerCanCraftMaxCount) ? EGsUIColorType::DEFAULT_ENOUGH : EGsUIColorType::DEFAULT_LACK;
	_txtCalcCurrent->SetColorAndOpacity(FGsUIColorHelper::GetColor(TxtCalcCurrentColorType));

	_btnCalcPlus1->SetIsEnabled(!_calculateHelper.IsMax());
	_btnCalcOpen->SetIsEnabled(_calculateHelper.GetMin() != _calculateHelper.GetMax());
}

TWeakObjectPtr<UGsUIWindowCraft> UGsUIWindowCraft::OpenWindow(const FGsContentsGameBase* InContentsGameBase)
{
	GSLOG(Log, TEXT("UGsUIWindowCraft::OpenWindow(%p)"), InContentsGameBase);

	check(!!InContentsGameBase);
	check(InContentsGameBase->GetType() == FGsContentsMode::InGame::ContentsCraft);

	UGsUIManager* UiManager = GUI();
	if (nullptr == UiManager)
	{
		GSLOG(Error, TEXT("nullptr == UiManager"));
		return nullptr;
	}

	TWeakObjectPtr<UGsUIWidgetBase> Widget = UiManager->OpenAndGetWidget(UGsUIWindowCraft::WINDOW_ASSET_NAME);
	if (!Widget.IsValid())
	{
		GSLOG(Error, TEXT("!Widget.IsValid()"));
		return nullptr;
	}

	TWeakObjectPtr<UGsUIWindowCraft> Window = Cast<UGsUIWindowCraft>(Widget);
	if (!Window.IsValid())
	{
		GSLOG(Error, TEXT("!Window.IsValid()"));
		return nullptr;
	}

	return Window;
}

void UGsUIWindowCraft::EndSpelling(const CraftId InCraftTargetId, const int32 InCraftCount)
{
	GSLOG(Log, TEXT("UGsUIWindowCraft::EndSpelling(%d, %d)"), InCraftTargetId, InCraftCount);

	if (INVALID_CRAFT_ID == InCraftTargetId)
	{
		GSLOG(Error, TEXT("INVALID_CRAFT_ID == InCraftTargetId"));
		return;
	}

	const UGsCraftManager* CraftManager = GSCraft();
	if (nullptr == CraftManager)
	{
		GSLOG(Error, TEXT("nullptr == CraftManager"));
		return;
	}

	TSharedPtr<FGsCostPackage> CostPackage = GetSelectedCraftTargetCostPackage();
	if (!CostPackage.IsValid())
	{
		GSLOG(Error, TEXT("!CostPackage.IsValid()"));
		return;
	}

	TSharedRef<FGsCostPackageMultiple> CostPackageMultiple = CostPackage->MakeCostPackageMultiple(InCraftCount);

	CraftManager->TryCraft(InCraftTargetId, *CostPackageMultiple);
}

void UGsUIWindowCraft::ResetAllDatas()
{
	_toggleGroupCraftMaterialBtns.Clear();

	//_toggleGroupCraftTargetBtns.Clear();
	_toggleGroupSub2ndTabBtns.Clear();
	_toggleGroupSubTabBtns.Clear();
	_toggleGroupMainTabBtns.Clear();

	_craftTargetListItems.Empty();
	if (nullptr != _craftTargetListItemsPool)
	{
		_craftTargetListItemsPool->ReleaseAll();
	}
	_cachedCraftSub2ndTabs.Empty();
	_cachedCraftSubTabs.Empty();
	_cachedCraftMainTabs.Empty();
}

bool UGsUIWindowCraft::ValidateCanCraftNow() const
{
	const UGsCraftManager* CraftManager = GSCraft();
	if (nullptr == CraftManager)
	{
		GSLOG(Error, TEXT("nullptr == CraftManager"));
		return false;
	}

	const FGsCraftTarget* CraftTarget = GetSelectedCraftTarget();
	if (nullptr == CraftTarget)
	{
		GSLOG(Warning, TEXT("nullptr == CraftTarget"));
		return false;
	}

	return CraftManager->CanCraftNow(CraftTarget->_craftTargetId, GetCalcCurrentValue(), true);
}

void UGsUIWindowCraft::AddToHistoryQueueThis()
{
	FGsGameStateCraft* GameStateCraft = UGsUIWindowCraft::GetGameStateCraft();
	if (nullptr == GameStateCraft)
	{
		return;
	}

	Parameters CurrentParameters;

	const FGsCraftTab* MainTab = GetSelectedMainTab();
	CurrentParameters._mainTabId = (nullptr != MainTab) ? MainTab->_craftTabId : INVALID_CRAFT_TAB_ID;

	const FGsCraftTab* SubTab = GetSelectedSubTab();
	CurrentParameters._subTabId = (nullptr != SubTab) ? SubTab->_craftTabId : INVALID_CRAFT_TAB_ID;

	const FGsCraftTab* Sub2ndTab = GetSelectedSub2ndTab();
	CurrentParameters._sub2ndTabId = (nullptr != Sub2ndTab) ? Sub2ndTab->_craftTabId : INVALID_CRAFT_TAB_ID;

	const FGsCraftTarget* CraftTarget = GetSelectedCraftTarget();
	CurrentParameters._craftId = (nullptr != CraftTarget) ? CraftTarget->_craftTargetId : INVALID_CRAFT_ID;
	// 현재 선택중인 요소가 없을 경우, CurrentParameters._craftId == INVALID_CRAFT_ID 일 수 있고, 이 경우라도 뒤로가기 했을 때 탭은 오픈되어야 함

	GameStateCraft->PushHistory(MoveTemp(CurrentParameters));
}

void UGsUIWindowCraft::OpenPopupCalculator()
{
	UGsUIManager* UiManager = GUI();
	if (nullptr == UiManager)
	{
		GSLOG(Error, TEXT("nullptr == UiManager"));
		return;
	}

	TWeakObjectPtr<UGsUIWidgetBase> WidgetBasePtr = UiManager->OpenAndGetWidget(TEXT("PopupCalculator"));
	if (!WidgetBasePtr.IsValid())
	{
		GSLOG(Error, TEXT("!WidgetBasePtr.IsValid()"));
		return;
	}

	UGsUIPopupCalculator* PopupCalculator = Cast<UGsUIPopupCalculator>(WidgetBasePtr.Get());
	if (nullptr == PopupCalculator)
	{
		GSLOG(Error, TEXT("nullptr == PopupCalculator"));
		return;
	}

	FText TitleText;
	if (FText::FindText(TEXT("ItemCraftText"), TEXT("Craft_UI_Calculator_BatchCraft"), TitleText))
	{
		PopupCalculator->SetTitleText(TitleText);
	}

	PopupCalculator->SetDimmed(true);
	PopupCalculator->SetResetCount();
	PopupCalculator->SetMinimumCount(0);
	PopupCalculator->SetMaximumCount(_calculateHelper.GetMax());
	PopupCalculator->SetDefaultCount(_calculateHelper.GetCurrent());
	PopupCalculator->InvalidateNumText();

	if (!PopupCalculator->OnFinishCalculateDelegate.IsBoundToObject(this))
	{
		PopupCalculator->OnFinishCalculateDelegate.AddUObject(this, &UGsUIWindowCraft::OnFinishCalculatePopupCalculator);
		PopupCalculator->OnNativeDestructDelegate.AddUObject(this, &UGsUIWindowCraft::OnNativeDestructPopupCalculator);
	}

	_isOpeningPopupCalculator = true;

	InvalidateUIInterationState();
}

void UGsUIWindowCraft::OpenPopupSortCandidateGoods(const uint8 InIndex)
{
	TSharedPtr<FGsCostPackage> CostPackage = GetSelectedCraftTargetCostPackage();
	if (!CostPackage.IsValid())
	{
		GSLOG(Error, TEXT("!CostPackage.IsValid()"));
		return;
	}

	// 대체아이템 없으면 무시
	if (2 > CostPackage->GetCostElementAt(InIndex)->GetCandidateCount())
	{
		return;
	}

	// [U1] | ejrrb10 | 제작 대체 아이템 정렬의 경우, 거래 가능 아이템을 먼저 보여준다 (CHR-21150)
	CostPackage->SortElementAtIndex(InIndex, EGsSortItemDisposal::CraftMaterial);

	UGsUIPopupSortCandidateGoods::OpenThis({
		TWeakPtr<const FGsCostPackage>(CostPackage),
		InIndex,
		FSimpleDelegate::CreateUObject(this, &UGsUIWindowCraft::OnOkPopupSortCandidateGoods)
	});
}

void UGsUIWindowCraft::CraftTargetUpdated(const CraftId InCraftId)
{
	UGsUICraftTargetListItem* ListItem = nullptr;
	int32 ListItemIndex = -1;
	const TArray<UObject*> ListItems = _craftTargetList->GetListItems();
	for (int32 i = ListItems.Num() - 1; 0 <= i; --i)
	{
		ListItem = Cast<UGsUICraftTargetListItem>(ListItems[i]);
		if (nullptr != ListItem && InCraftId == ListItem->_craftTargetId)
		{
			ListItemIndex = i;
			break;
		}
	}

	// 업데이트된 제작대상이 현재 제작대상리스트에 있는 경우에만 리프레시를 수행.
	if (nullptr != ListItem && 0 <= ListItemIndex)
	{
		UGsUIWindowCraft::UpdateCraftTargetListItem(*ListItem);
		InvalidateCraftTargetListEntry(ListItemIndex);
	}

	// 업데이트된 제작대상이 현재 상세정보에 출력중인 경우 리프레시를 수행.
	const FGsCraftTarget* CraftTarget = GetSelectedCraftTarget();
	if (nullptr != CraftTarget && InCraftId == CraftTarget->_craftTargetId)
	{
		InvalidateCraftTargetDetail();
	}
}

void UGsUIWindowCraft::SetParameters(const Parameters& InParameters, const bool InAddCurrentHistory /*= true*/)
{
	// 새로운 파라미터가 들어왔으므로, 히스토리 복원을 위해 현재 상태를 저장
	if (InAddCurrentHistory)
	{
		AddToHistoryQueueThis();
	}

	const FGsCraftTarget* CraftTarget = GetSelectedCraftTarget();
	const bool HasSelectedCraftTarget = nullptr != CraftTarget;
	const bool NeedSetParameters = InAddCurrentHistory || (INVALID_CRAFT_ID == InParameters._craftId) || (INVALID_CRAFT_ID != InParameters._craftId && (nullptr == CraftTarget || CraftTarget->_craftTargetId != InParameters._craftId));
	if (NeedSetParameters)
	{
		SetParametersPrivate(InParameters);
	}

	_isBoxDetailResult = 0;
}

void UGsUIWindowCraft::SetParametersPrivate(const Parameters& InParameters)
{
	GSLOG(Log, TEXT("UGsUIWindowCraft::SetParametersPrivate({_mainTabId:%d, _subTabId:%d, _sub2ndTabId:%d, _craftId:%d})"), InParameters._mainTabId, InParameters._subTabId, InParameters._sub2ndTabId, InParameters._craftId);

	ESlateVisibility backButtonVisibility = ESlateVisibility::Collapsed;
	FGsGameStateCraft* GameStateCraft = UGsUIWindowCraft::GetGameStateCraft();
	if (nullptr != GameStateCraft)
	{
		backButtonVisibility = GameStateCraft->HasHistory() ? ESlateVisibility::Visible : ESlateVisibility::Collapsed;
	}
	_btnCraftBack->SetVisibility(backButtonVisibility);

	const UGsCraftManager* CraftManager = GSCraft();
	if (nullptr == CraftManager)
	{
		GSLOG(Error, TEXT("nullptr == CraftManager"));
		return;
	}

	UGsUIWindowCraft::MakeDisplayableChildTabs(CraftManager->GetRootTab(), _cachedCraftMainTabs);
	InvalidateMainTabBtns();

	CraftTabId MainTabId = InParameters._mainTabId;
	CraftTabId SubTabId = InParameters._subTabId;
	CraftTabId Sub2ndTabId = InParameters._sub2ndTabId;

	if (!InParameters.HasValidTabs())
	{
		// 탭 지정이 없는 경우, 디폴트 탭을 지정함.
		// root tab 부터 최하위 tab 까지
		const TArray<const FGsCraftTab*> TabRoute = CraftManager->MakeTabRouteFromRoot(InParameters._craftId);
		if (UGsUIWindowCraft::TAB_ROUTE_MIN_DEPTH > TabRoute.Num())
		{
			GSLOG(Error, TEXT("UGsUIWindowCraft::TAB_ROUTE_MIN_DEPTH(%d) > %d"), UGsUIWindowCraft::TAB_ROUTE_MIN_DEPTH, TabRoute.Num());
			return;
		}

		{
			const FGsCraftTab* RootTab = TabRoute[0];
			if (nullptr == RootTab || CraftManager->GetRootTab() != RootTab)
			{
				GSLOG(Error, TEXT("nullptr == RootTab || CraftManager->GetRootTab() != RootTab, RootTab:0x%p"), RootTab);
				return;
			}
		}

		{
			const FGsCraftTab* MainTab = TabRoute[1];
			if (nullptr == MainTab)
			{
				GSLOG(Error, TEXT("nullptr == MainTab"));
				return;
			}
			MainTabId = MainTab->_craftTabId;
		}

		{
			const FGsCraftTab* SubTab = TabRoute[2];
			if (nullptr == SubTab)
			{
				GSLOG(Error, TEXT("nullptr == SubTab"));
				return;
			}
			SubTabId = SubTab->_craftTabId;
		}

		if (3 < TabRoute.Num())
		{
			const FGsCraftTab* Sub2ndTab = TabRoute[3];
			if (nullptr == Sub2ndTab)
			{
				GSLOG(Error, TEXT("nullptr == Sub2ndTab"));
				return;
			}
			Sub2ndTabId = Sub2ndTab->_craftTabId;
		}
		else
		{
			Sub2ndTabId = INVALID_CRAFT_TAB_ID;
		}
	}

	// 제작 대상을 외부에서 특정했을 때에만 자동 선택됨.
	NavigateTo(MainTabId, SubTabId, Sub2ndTabId, InParameters._craftId);
}

void UGsUIWindowCraft::SelectMainTabBtn(const CraftTabId InCraftTabId)
{
	GSLOG(Log, TEXT("UGsUIWindowCraft::SelectMainTabBtn(%d)"), InCraftTabId);

	for (int32 i = _cachedCraftMainTabs.Num() - 1; 0 <= i; --i)
	{
		if (_cachedCraftMainTabs[i]->_craftTabId == InCraftTabId)
		{
			_toggleGroupMainTabBtns.SetSelectedIndex(i);
			return;
		}
	}

	GSLOG(Error, TEXT("Failed to find InCraftTabId:%d"), InCraftTabId);
}

void UGsUIWindowCraft::SelectSubTabBtn(const CraftTabId InCraftTabId)
{
	GSLOG(Log, TEXT("UGsUIWindowCraft::SelectSubTabBtn(%d)"), InCraftTabId);

	for (int32 i = _cachedCraftSubTabs.Num() - 1; 0 <= i; --i)
	{
		if (_cachedCraftSubTabs[i]->_craftTabId == InCraftTabId)
		{
			_toggleGroupSubTabBtns.SetSelectedIndex(i);
			return;
		}
	}

	GSLOG(Error, TEXT("Failed to find InCraftTabId:%d"), InCraftTabId);
}

void UGsUIWindowCraft::SelectSub2ndTabBtn(const CraftTabId InCraftTabId)
{
	GSLOG(Log, TEXT("UGsUIWindowCraft::SelectSub2ndTabBtn(%d)"), InCraftTabId);
	
	for (int32 i = _cachedCraftSub2ndTabs.Num() - 1; 0 <= i; --i)
	{
		if (_cachedCraftSub2ndTabs[i]->_craftTabId == InCraftTabId)
		{
			_toggleGroupSub2ndTabBtns.SetSelectedIndex(i);
			return;
		}
	}

	GSLOG(Error, TEXT("Failed to find InCraftTabId:%d"), InCraftTabId);
}

void UGsUIWindowCraft::SetVisibilitySub2ndTabBtns(const bool InVisible)
{
	_panelSub2ndTabBtns->SetVisibility(InVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UGsUIWindowCraft::ToggleVisibilitySub2ndTabBtns()
{
	_panelSub2ndTabBtns->SetVisibility((_panelSub2ndTabBtns->GetVisibility() == ESlateVisibility::Collapsed) ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UGsUIWindowCraft::SelectCraftTargetBtn(const CraftId InCraftTargetId)
{
	for (const UGsUICraftTargetListItem* ListItem : _craftTargetListItems)
	{
#if WITH_EDITOR
		if (nullptr == ListItem)
		{
			GSLOG(Error, TEXT("nullptr == ListItem"));
			continue;
		}
#endif

		if (ListItem->_craftTargetId == InCraftTargetId)
		{
			_craftTargetList->SetSelectedItem(ListItem);
			return;
		}
	}
	
	GSLOG(Warning, TEXT("Failed to find ListItem has InCraftTargetId(%d)"), InCraftTargetId);
}

void UGsUIWindowCraft::ResetCraftTargetCostPackage()
{
	_copyedCostPackage.Reset();
}

void UGsUIWindowCraft::NavigateTo(const CraftTabId InMainTabId, const CraftTabId InSubTabId, const CraftTabId InSub2ndTabId, const CraftId InCraftId /*= INVALID_CRAFT_ID*/)
{
	GSLOG(Log, TEXT("UGsUIWindowCraft::NavigateTo(InMainTabId:%d, InSubTabId:%d, InSub2ndTabId:%d, InCraftId:%d)"), InMainTabId, InSubTabId, InSub2ndTabId, InCraftId);

	if (INVALID_CRAFT_TAB_ID != InMainTabId)
	{
		SelectMainTabBtn(InMainTabId);
	}

	if (INVALID_CRAFT_TAB_ID != InSubTabId)
	{
		SelectSubTabBtn(InSubTabId);
	}

	if (INVALID_CRAFT_TAB_ID != InSub2ndTabId)
	{
		SelectSub2ndTabBtn(InSub2ndTabId);
	}

	if (INVALID_CRAFT_ID != InCraftId)
	{
		SelectCraftTargetBtn(InCraftId);
	}
}

void UGsUIWindowCraft::SetSpellingState(const bool InSpelling)
{
	GSLOG(Log, TEXT("UGsUIWindowCraft::SetSpellingState(%d)"), (int32)InSpelling);

	if (InSpelling)
	{
		const FGsCraftTarget* CraftTarget = GetSelectedCraftTarget();
		if (nullptr == CraftTarget)
		{
			GSLOG(Error, TEXT("nullptr == CraftTarget"));
			return;
		}

		_spellingStartTime = FDateTime::Now();
		_switcherBottom->SetActiveWidgetIndex(1);

		const CraftId CurrentCraftId = CraftTarget->_craftTargetId;
		const int32 CraftCount = GetCalcCurrentValue();
		_scenePlayer.Start(FSimpleDelegate::CreateLambda([CurrentCraftId, CraftCount]() {
			// 딜레이 호출 및 인스턴스 확인 등이 필요하여 메세지로 발송함.
			FGsCraftMessageParamOnEndSpelling(CurrentCraftId, CraftCount).Send();
		}));
	}
	else
	{
		_scenePlayer.Rewind();
		_switcherBottom->SetActiveWidgetIndex(0);
	}

	InvalidateUIInterationState();
}

FGsGameStateCraft* UGsUIWindowCraft::GetGameStateCraft()
{
	FGsGameFlowManager* GameFlowManager = GMode();
	if (nullptr == GameFlowManager)
	{
		GSLOG(Warning, TEXT("nullptr == GameFlowManager"));
		return nullptr;
	}

	FGsGameFlowGame* GameFlowGame = GameFlowManager->GetGameFlow();
	if (nullptr == GameFlowGame)
	{
		GSLOG(Warning, TEXT("nullptr == GameFlowGame"));
		return nullptr;
	}

	FGsContentsManagerGame* ContentsManagerGame = GameFlowGame->GetContentsManagerGame();
	if (nullptr == ContentsManagerGame)
	{
		GSLOG(Warning, TEXT("nullptr == ContentsManagerGame"));
		return nullptr;
	}

	FGsGameStateCraft* GameStateCraft = ContentsManagerGame->GetContentsStateObject<FGsGameStateCraft>(FGsContentsMode::InGame::ContentsCraft);
	if (nullptr == GameStateCraft)
	{
		GSLOG(Warning, TEXT("nullptr == ContentsManagerGame"));
	}

	return GameStateCraft;
}

void UGsUIWindowCraft::MakeDisplayableChildTabs(const FGsCraftTab* InParentTab, OUT TArray<const FGsCraftTab*>& OutCraftTabs)
{
	OutCraftTabs.Empty();

	if (nullptr == InParentTab)
	{
		// GSLOG(Error, TEXT("nullptr == InParentTab"));
		return;
	}

	const UGsCraftManager* CraftManager = GSCraft();
	if (nullptr == CraftManager)
	{
		GSLOG(Error, TEXT("nullptr == CraftManager"));
		return;
	}

	OutCraftTabs.Append(InParentTab->MakeDisplaybleChildTabs());

	GSLOG(Log, TEXT("UGsUIWindowCraft::MakeDisplayableChildTabs(), InParentTab->_craftTabId:%d, OutCraftTabs.Num():%d"), InParentTab->_craftTabId, OutCraftTabs.Num());
}

const FGsCraftTab* UGsUIWindowCraft::GetSelectedMainTab() const
{
	const int32 SelectedIndex = _toggleGroupMainTabBtns.GetSelectedIndex();
	GSLOG(Log, TEXT("UGsUIWindowCraft::GetSelectedMainTab(), SelectedIndex:%d"), SelectedIndex);

	if (!_cachedCraftMainTabs.IsValidIndex(SelectedIndex))
	{
		return nullptr;
	}

	return _cachedCraftMainTabs[SelectedIndex];
}

const FGsCraftTab* UGsUIWindowCraft::GetSelectedSubTab() const
{
	const int32 SelectedIndex = _toggleGroupSubTabBtns.GetSelectedIndex();
	GSLOG(Log, TEXT("UGsUIWindowCraft::GetSelectedSubTab(), SelectedIndex:%d"), SelectedIndex);

	if (!_cachedCraftSubTabs.IsValidIndex(SelectedIndex))
	{
		return nullptr;
	}

	return _cachedCraftSubTabs[SelectedIndex];
}

const FGsCraftTab* UGsUIWindowCraft::GetSelectedSub2ndTab() const
{
	const int32 SelectedIndex = _toggleGroupSub2ndTabBtns.GetSelectedIndex();
	//GSLOG(Log, TEXT("UGsUIWindowCraft::GetSelectedSub2ndTab(), SelectedIndex:%d"), SelectedIndex);

	if (!_cachedCraftSub2ndTabs.IsValidIndex(SelectedIndex))
	{
		return nullptr;
	}

	return _cachedCraftSub2ndTabs[SelectedIndex];
}

const FGsCraftTab* UGsUIWindowCraft::GetSelectedTabTail() const
{
	TArray<const FGsCraftTab*> Tabs({ GetSelectedMainTab(), GetSelectedSubTab(), GetSelectedSub2ndTab() });
	while (0 < Tabs.Num())
	{
		const FGsCraftTab* CraftTab = Tabs.Pop(false);
		if (nullptr != CraftTab && CraftTab->CanDisplay())
		{
			return CraftTab;
		}
	}

	return nullptr;
}

const FGsCraftTarget* UGsUIWindowCraft::GetSelectedCraftTarget() const
{
	const UGsUICraftTargetListItem* SelectedListItem = _craftTargetList->GetSelectedItem<UGsUICraftTargetListItem>();
	if (nullptr == SelectedListItem)
	{
		return nullptr;
	}

	const UGsCraftManager* CraftManager = GSCraft();
	if (nullptr == CraftManager)
	{
		GSLOG(Error, TEXT("nullptr == CraftManager"));
		return nullptr;
	}

	return CraftManager->GetCraftTarget(SelectedListItem->_craftTargetId);
}

TSharedPtr<FGsCostPackage> UGsUIWindowCraft::GetSelectedCraftTargetCostPackage()
{
	if (!_copyedCostPackage.IsValid())
	{
		const FGsCraftTarget* CraftTarget = GetSelectedCraftTarget();
		if (nullptr != CraftTarget)
		{
			// copy cost package
			_copyedCostPackage = CraftTarget->CreateCostPackage();
		}
	}

	return _copyedCostPackage;
}

int32 UGsUIWindowCraft::GetCalcCurrentValue() const
{
	return _calculateHelper.GetCurrent();
}

bool UGsUIWindowCraft::IsSpellingState() const
{
	return (1 == _switcherBottom->GetActiveWidgetIndex()) && !_scenePlayer.IsFinished();
}

void UGsUIWindowCraft::OnRefreshEntryMainTabBtn(int32 InIndex, UWidget* InEntry)
{
	if (!_cachedCraftMainTabs.IsValidIndex(InIndex))
	{
		GSLOG(Error, TEXT("!_cachedCraftMainTabs.IsValidIndex(InIndex), InIndex:%d"), InIndex);
		return;
	}

	const FGsCraftTab* Tab = _cachedCraftMainTabs[InIndex];
	if (nullptr == Tab)
	{
		GSLOG(Error, TEXT("nullptr == Tab, InIndex:%d"), InIndex);
		return;
	}

	UGsUITabButtonCommon* CommonTabBtn = Cast<UGsUITabButtonCommon>(InEntry);
	if (nullptr == CommonTabBtn)
	{
		GSLOG(Error, TEXT("nullptr == CommonTabBtn, *InEntry->GetName():%s"), *InEntry->GetName());
		return;
	}

	CommonTabBtn->SetTitleText(Tab->GetTabTitle());

	IGsToggleGroupEntry* ToggleGroupEntry = CommonTabBtn->GetToggleGroupEntry();
	if (nullptr == ToggleGroupEntry)
	{
		GSLOG(Error, TEXT("nullptr == ToggleGroupEntry"));
	}
	else
	{
		ToggleGroupEntry->SetIsSelected(false);
	}

	_toggleGroupMainTabBtns.AddToggleWidget(ToggleGroupEntry);
}

void UGsUIWindowCraft::OnSelectChangedMainTabBtn(int32 InIndex)
{
	if (0 > InIndex)
	{
		return;
	}

	UGsUIWindowCraft::MakeDisplayableChildTabs(GetSelectedMainTab(), _cachedCraftSubTabs);

	// 기본 선택인덱스가 0 이라서, 아래 0번 셀렉션을 시도해도 refresh 가 발생하지 않으므로, 강제로 0 으로 설정함.
	_toggleGroupSubTabBtns.SetSelectedIndex(-1);

	InvalidateSubTabBtns(true);

	// 0번 요소 자동 선택
	_toggleGroupSubTabBtns.SetSelectedIndex(0);
}

void UGsUIWindowCraft::OnRefreshEntrySubTabBtn(int32 InIndex, UWidget* InEntry)
{
	//GSLOG(Log, TEXT("UGsUIWindowCraft::OnRefreshEntrySubTabBtn(%d, %p)"), InIndex, InEntry);

	if (!_cachedCraftSubTabs.IsValidIndex(InIndex))
	{
		GSLOG(Error, TEXT("!_cachedCraftSubTabs.IsValidIndex(InIndex), InIndex:%d"), InIndex);
		return;
	}

	const FGsCraftTab* Tab = _cachedCraftSubTabs[InIndex];
	if (nullptr == Tab)
	{
		GSLOG(Error, TEXT("nullptr == Tab, InIndex:%d"), InIndex);
		return;
	}

	UGsUITabButtonCommon* CommonTabBtn = Cast<UGsUITabButtonCommon>(InEntry);
	if (nullptr == CommonTabBtn)
	{
		GSLOG(Error, TEXT("nullptr == CommonTabBtn, *InEntry->GetName():%s"), *InEntry->GetName());
		return;
	}

	CommonTabBtn->SetTitleText(Tab->GetTabTitle());

	const bool IsSelected = _toggleGroupSubTabBtns.GetSelectedIndex() == InIndex;

	IGsToggleGroupEntry* ToggleGroupEntry = CommonTabBtn->GetToggleGroupEntry();
	if (nullptr == ToggleGroupEntry)
	{
		GSLOG(Error, TEXT("nullptr == ToggleGroupEntry"));
		return;
	}

	ToggleGroupEntry->SetIsSelected(IsSelected);

	{
		FText SubText = FText::GetEmpty();

		const FGsCraftTab* SelectedSub2ndTab = GetSelectedSub2ndTab();
		if (IsSelected && nullptr != SelectedSub2ndTab)
		{
			SubText = SelectedSub2ndTab->GetTabTitle();
		}

		CommonTabBtn->SetSubText(SubText);
	}

	_toggleGroupSubTabBtns.AddToggleWidget(ToggleGroupEntry);
}

void UGsUIWindowCraft::OnSlotClickedSubTabBtn(int32 InIndex, bool bIsSelected)
{
	if (!_cachedCraftSubTabs.IsValidIndex(InIndex))
	{
		GSLOG(Log, TEXT("!_cachedCraftSubTabs.IsValidIndex(%d)"), InIndex);
		return;
	}

	const FGsCraftTab* CraftTab = _cachedCraftSubTabs[InIndex];
	check(nullptr != CraftTab);

	if (CraftTab->HasChildTab() && nullptr != GetSelectedSub2ndTab())
	{
		// 하위탭 있다면 하위탭 출력 토글
		ToggleVisibilitySub2ndTabBtns();
	}
}

void UGsUIWindowCraft::OnSelectChangedSubTabBtn(int32 InIndex)
{
	if (!_cachedCraftSubTabs.IsValidIndex(InIndex))
	{
		GSLOG(Log, TEXT("!_cachedCraftSubTabs.IsValidIndex(%d)"), InIndex);
		return;
	}

	// 선택 객체로 자동 스크롤
	{
		UWidget* SelectedWidget = _slotHelperSubTabBtns->GetEntry(InIndex);
		if (nullptr != SelectedWidget)
		{
			_scrollSubTabBtns->ScrollWidgetIntoView(SelectedWidget);
		}
	}

	const FGsCraftTab* CraftTab = _cachedCraftSubTabs[InIndex];
	check(nullptr != CraftTab);

	UGsUIWindowCraft::MakeDisplayableChildTabs(CraftTab, _cachedCraftSub2ndTabs);

	if (!CraftTab->HasChildTab())
	{
		// 하위탭 없다면 리스트 업데이트
		SetVisibilitySub2ndTabBtns(false);

		UpdateCraftTargetListItems(true);
		InvalidateCraftTargetListEntries(true);
		ResetCraftMaterialBtns();
		InvalidateCraftTargetDetail();
	}
	else
	{
		// 하위탭 있다면 하위탭 출력
		InvalidateSub2ndTabBtns();
		SetVisibilitySub2ndTabBtns(true);

		//UpdateCraftTargetListItems(true);
		//InvalidateCraftTargetListEntries(true);
		//ResetCraftMaterialBtns();
		//InvalidateCraftTargetDetail();
	}

	// 동일 그룹 내의 다른 버튼들도 새로고침해야 함
	InvalidateSubTabBtns();
}

void UGsUIWindowCraft::OnRefreshEntrySub2ndTabBtn(int32 InIndex, UWidget* InEntry)
{
	//GSLOG(Log, TEXT("UGsUIWindowCraft::OnRefreshEntrySub2ndTabBtn(%d, %p)"), InIndex, InEntry);

	if (!_cachedCraftSub2ndTabs.IsValidIndex(InIndex))
	{
		GSLOG(Error, TEXT("!_cachedCraftSub2ndTabs.IsValidIndex(InIndex), InIndex:%d"), InIndex);
		return;
	}

	const FGsCraftTab* Tab = _cachedCraftSub2ndTabs[InIndex];
	if (nullptr == Tab)
	{
		GSLOG(Error, TEXT("nullptr == Tab, InIndex:%d"), InIndex);
		return;
	}

	UGsUIMarketSubCategoryListEntry* CommonTabBtn = Cast<UGsUIMarketSubCategoryListEntry>(InEntry);
	if (nullptr == CommonTabBtn)
	{
		GSLOG(Error, TEXT("nullptr == CommonTabBtn, *InEntry->GetName():%s"), *InEntry->GetName());
		return;
	}

	CommonTabBtn->SetUIName(Tab->GetTabTitle());

	CommonTabBtn->SetIsSelected(false);

	_toggleGroupSub2ndTabBtns.AddToggleWidget(CommonTabBtn);
}

void UGsUIWindowCraft::OnSlotClickedSub2ndTabBtn(int32 InIndex, bool bIsSelected)
{
	SetVisibilitySub2ndTabBtns(false);
}

void UGsUIWindowCraft::OnSelectChangedSub2ndTabBtn(int32 InIndex)
{
	InvalidateSubTabBtns();

	SetVisibilitySub2ndTabBtns(false);

	if (0 > InIndex)
	{
		return;
	}

	// 선택 객체로 자동 스크롤
	{
		UWidget* SelectedWidget = _slotHelperSub2ndTabBtns->GetEntry(InIndex);
		if (nullptr != SelectedWidget)
		{
			_scrollSub2ndTabBtns->ScrollWidgetIntoView(SelectedWidget);
		}
	}

	UpdateCraftTargetListItems(true);
	InvalidateCraftTargetListEntries(true);
	ResetCraftMaterialBtns();
	InvalidateCraftTargetDetail();
}

void UGsUIWindowCraft::UpdateCraftTargetListItems(const bool InResetFirst /*= false*/)
{
	if (nullptr == _craftTargetListItemsPool)
	{
		GSLOG(Error, TEXT("nullptr == _craftTargetListItemsPool"));
		return;
	}

	const UGsCraftManager* CraftManager = GSCraft();
	if (nullptr == CraftManager)
	{
		GSLOG(Error, TEXT("nullptr == CraftManager"));
		return;
	}

	// 리셋 요청한 경우에는 새로운 ID목록을 생성하고, 리셋 요청이 아닌 경우에는 기존 리스트아이템을 그대로 사용.
	TArray<CraftId> CraftTargetIds;
	if (InResetFirst)
	{
		const FGsCraftTab* SelectedTabTail = GetSelectedTabTail();
		if (nullptr != SelectedTabTail)
		{
			// 특정 탭이 열릴 때, 필요한 경우 서버에 추가 정보를 요청함.
			FGsCraftMessageParamOnOpenTab(SelectedTabTail->_craftTabId).Send();

			// 하위탭이 있는 경우 해당 하위탭 아래에 있는 모든 녀석들을 가져옴 (C2URWQ-3735)
			//if (!SelectedTabTail->HasChildTab())
			{
				const bool IsFavoriteTab = CraftManager->GetFavoriteTab() == SelectedTabTail;
				// 임시 제작대상목록
				TArray<const FGsCraftTarget*> CraftTargets;
				SelectedTabTail->MakeDisplaybleCraftTargets(CraftTargets, !IsFavoriteTab);

				_craftTargetListItems.Empty(CraftTargets.Num());
				_craftTargetListItemsPool->ReleaseAll();

				for (const FGsCraftTarget* CraftTarget : CraftTargets)
				{
					CraftTargetIds.Add(CraftTarget->_craftTargetId);
				}
			}
		}
	}
	else
	{
		for (UGsUICraftTargetListItem* ListItem : _craftTargetListItems)
		{
			if (nullptr == ListItem || INVALID_CRAFT_ID == ListItem->_craftTargetId)
			{
				GSLOG(Error, TEXT("nullptr == ListItem || INVALID_CRAFT_ID == ListItem->_craftTargetId"));
				continue;
			}

			CraftTargetIds.Add(ListItem->_craftTargetId);
		}
	}

	for (int32 i = 0; i < CraftTargetIds.Num(); ++i)
	{
		const CraftId CurrentCraftId = CraftTargetIds[i];
		const FGsCraftTarget* CraftTarget = CraftManager->GetCraftTarget(CurrentCraftId);
		if (nullptr == CraftTarget)
		{
			GSLOG(Error, TEXT("nullptr == CraftTarget, i:%d"), i);
			continue;
		}

		if (!_craftTargetListItems.IsValidIndex(i))
		{
			_craftTargetListItems.Emplace(_craftTargetListItemsPool->Claim<UGsUICraftTargetListItem>());
		}

		UGsUICraftTargetListItem* ListItem = _craftTargetListItems[i];

#ifdef WITH_EDITOR
		if (nullptr == ListItem)
		{
			GSLOG(Error, TEXT("nullptr == ListItem, i:%d"), i);
			continue;
		}
#endif // WITH_EDITOR

		ListItem->_craftTargetId = CraftTarget->_craftTargetId;
		UGsUIWindowCraft::UpdateCraftTargetListItem(*ListItem);
	}
}

void UGsUIWindowCraft::UpdateCraftTargetListItem(UGsUICraftTargetListItem& InListItem)
{
	if (INVALID_CRAFT_ID == InListItem._craftTargetId)
	{
		GSLOG(Error, TEXT("INVALID_CRAFT_ID == InListItem._craftTargetId(%d)"), InListItem._craftTargetId);
		return;
	}

	const UGsCraftManager* CraftManager = GSCraft();
	if (nullptr == CraftManager)
	{
		GSLOG(Error, TEXT("nullptr == CraftManager"));
		return;
	}

	const FGsCraftTarget* CraftTarget = CraftManager->GetCraftTarget(InListItem._craftTargetId);
	if (nullptr == CraftTarget)
	{
		GSLOG(Error, TEXT("nullptr == CraftTarget"));
		return;
	}

	const FGsCraftTargetObject* DefaultCraftTargetObject = CraftTarget->GetDefaultCraftTargetObject();
	if (nullptr == DefaultCraftTargetObject)
	{
		GSLOG(Error, TEXT("nullptr != DefaultCraftTargetObject"));
	}

	InListItem._craftTargetObject = (nullptr != DefaultCraftTargetObject) ? *DefaultCraftTargetObject : FGsCraftTargetObject();
}

void UGsUIWindowCraft::OnCraftTargetListItemSelectionChanged(UObject* InListItem)
{
	if (nullptr == InListItem)
	{
		return;
	}

	ResetCraftTargetCostPackage();

	// 선택 객체로 자동 스크롤
	_craftTargetList->RequestNavigateToItem(InListItem);
	_calculateHelper.ToMin();

	ResetCraftMaterialBtns();
	InvalidateCraftTargetDetail();
}

void UGsUIWindowCraft::OnClickCraftTargetListItem(UObject* InListItem)
{
	// ListView는 버튼이 안먹기 때문에 별도 사운드 부착 
	if (UGsUIManager* uiMgr = GUI())
	{
		uiMgr->PlayPressSound();
	}
}

void UGsUIWindowCraft::OnClickedDetailSwitcher()
{
	// toggle
	const int32 NumWidgets = _boxDetailSwitcher->GetNumWidgets();
	const int32 CurrentWidgetIndex = _boxDetailSwitcher->GetActiveWidgetIndex();
	_boxDetailSwitcher->SetActiveWidgetIndex((CurrentWidgetIndex + 1) % NumWidgets);
	_isBoxDetailResult = (_isBoxDetailResult + 1) % NumWidgets;
}

void UGsUIWindowCraft::OnClickedCraftTargetIcon()
{
	const FGsCraftTarget* CraftTarget = GetSelectedCraftTarget();
	if (nullptr == CraftTarget)
	{
		GSLOG(Warning, TEXT("nullptr == CraftTarget"));
		return;
	}

	const FGsCraftTargetObject* CraftTargetObject = CraftTarget->GetDefaultCraftTargetObject();
	if (nullptr == CraftTargetObject)
	{
		GSLOG(Error, TEXT("nullptr == CraftTargetObject"));
		return;
	}

	FGsMessageHolder* MessageHolder = GMessage();
	if (nullptr == MessageHolder)
	{
		GSLOG(Error, TEXT("nullptr == MessageHolder"));
		return;
	}

	if (CostType::ITEM == CraftTargetObject->GetCostType())
	{
		FGsItemDetailOpenMessageParamTID Message(CraftTargetObject->GetItemId(), false, false, UGsUIPopupItemDetailMain::CenterPosType::CenterPos);
		MessageHolder->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM_TID, &Message);
	}
	else if (CostType::CURRENCY == CraftTargetObject->GetCostType())
	{
		FGsCurrencyDetailOpenMessageParam Message(CraftTargetObject->GetCurrencyType(), 1, false, false, UGsUIPopupItemDetailMain::CenterPosType::CenterPos);
		MessageHolder->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_CURRENCY, &Message);
	}
	else
	{
		GSLOG(Error, TEXT("Unknown CostType"));
	}
}

void UGsUIWindowCraft::OnRefreshEntryCraftExpectedListItem(int32 InIndex, UWidget* InEntry)
{
	if (!_craftExpectedListItemParameters.IsValidIndex(InIndex))
	{
		GSLOG(Error, TEXT("!_craftExpectedListItemParameters.IsValidIndex(%d)"), InIndex);
		return;
	}

	UGsUICraftExpectedListItem* ListItem = Cast<UGsUICraftExpectedListItem>(InEntry);
	if (nullptr == ListItem)
	{
		GSLOG(Error, TEXT("nullptr == ListItem, InIndex:%d"), InIndex);
		return;
	}

	ListItem->SetParameters(_craftExpectedListItemParameters[InIndex]);
}

void UGsUIWindowCraft::OnRefreshEntryCraftMaterialBtn(int32 InIndex, UWidget* InEntry)
{
	const FGsCraftTarget* CraftTarget = GetSelectedCraftTarget();
	if (nullptr == CraftTarget)
	{
		GSLOG(Error, TEXT("nullptr == CraftTarget"));
		return;
	}

	TSharedPtr<FGsCostPackage> CostPackage = GetSelectedCraftTargetCostPackage();
	if (!CostPackage.IsValid())
	{
		GSLOG(Error, TEXT("!CostPackage.IsValid()"));
		return;
	}

	const FGsCostElement* CostElement = CostPackage->GetCostElementAt(InIndex);
	if (nullptr == CostElement)
	{
		GSLOG(Error, TEXT("nullptr == CostElement, CraftTarget->_craftTargetId:%d, InIndex:%d"), CraftTarget->_craftTargetId, InIndex);
		return;
	}

	UGsUICraftMaterialListItem* ListItem = Cast<UGsUICraftMaterialListItem>(InEntry);
	if (nullptr == ListItem)
	{
		GSLOG(Error, TEXT("nullptr == ListItem, CraftTarget->_craftTargetId:%d, InIndex:%d"), CraftTarget->_craftTargetId, InIndex);
		return;
	}

	const UGsSupplyManager* SupplyManager = GSSupply();
	if (nullptr == SupplyManager)
	{
		GSLOG(Error, TEXT("nullptr == SupplyManager"));
		return;
	}
	const bool HasNavigateToCraft = UGsSupplyManager::EGsNavigationStatus::Movable == SupplyManager->GetNavigationStatus(EGsUnlockContentsType::CRAFT, CostElement->GetItemId());

	const int32 CalcCurrentValue = GetCalcCurrentValue();
	ListItem->SetParameters({
		InIndex,
		CostElement->_costType,
		CostElement->GetCurrencyType(),
		CostElement->GetItemId(),
		CostElement->GetItemLevel(),
		CostElement->GetCostAmount() * CalcCurrentValue,
		CostElement->PlayerHasAmountMax(),
		CostElement->PlayerHasEnough(CalcCurrentValue),
		CostElement->PlayerHasEnoughJustFirstCandidate(CalcCurrentValue),
		HasNavigateToCraft,
		(1 < CostElement->GetCandidateCount()),
		UGsUICraftMaterialListItem::FGsOnClickCraftMaterialListItem::CreateUObject(this, &UGsUIWindowCraft::OnClickedCraftMaterialCraft),
		UGsUICraftMaterialListItem::FGsOnClickCraftMaterialListItem::CreateUObject(this, &UGsUIWindowCraft::OnClickedCraftMaterialShowCandidate)
	});

	{
		IGsToggleGroupEntry* ToggleGroupEntry = ListItem->GetToggleGroupEntry();
		if (nullptr == ToggleGroupEntry)
		{
			GSLOG(Error, TEXT("nullptr == ToggleGroupEntry"));
		}
		else
		{
			_toggleGroupCraftMaterialBtns.AddToggleWidget(ToggleGroupEntry);
		}
		ToggleGroupEntry->SetIsSelected(_toggleGroupCraftMaterialBtns.GetSelectedIndex() == InIndex);
	}
}

void UGsUIWindowCraft::OnClickedCraftMaterialCraft(const int32 InIndex)
{
	DeselectCraftMaterialBtns();

	if (0 > InIndex)
	{
		GSLOG(Error, TEXT("0 > InIndex"));
		return;
	}

	TSharedPtr<FGsCostPackage> CostPackage = GetSelectedCraftTargetCostPackage();
	if (!CostPackage.IsValid())
	{
		GSLOG(Error, TEXT("!CostPackage.IsValid()"));
		return;
	}

	const FGsCostElement* CostElement = CostPackage->GetCostElementAt(InIndex);
	if (nullptr == CostElement)
	{
		GSLOG(Error, TEXT("nullptr == CostElement"));
		return;
	}

	const ItemId CurrentItemId = CostElement->GetItemId();

	const UGsSupplyManager* SupplyManager = GSSupply();
	if (nullptr == SupplyManager)
	{
		GSLOG(Error, TEXT("nullptr == SupplyManager"));
		return;
	}

	SupplyManager->TryToNavigateItemId(EGsUnlockContentsType::CRAFT, CurrentItemId);
}

void UGsUIWindowCraft::OnClickedCraftMaterialShowCandidate(const int32 InIndex)
{
	DeselectCraftMaterialBtns();

	if (0 > InIndex)
	{
		GSLOG(Error, TEXT("0 > InIndex"));
		return;
	}

	OpenPopupSortCandidateGoods(StaticCast<uint8>(InIndex));
}

void UGsUIWindowCraft::OnChangedCalculateHelper(int32 InNumber)
{
	InvalidateCalculateHelper();
	InvalidateCraftMaterialBtns();
}

void UGsUIWindowCraft::OnClickedCalcReset()
{
	_calculateHelper.ToMin();
}

void UGsUIWindowCraft::OnClickedCalcPlus10()
{
	_calculateHelper.Add(10);
	ValidateCanCraftNow();
}

void UGsUIWindowCraft::OnClickedCalcMax()
{
	_calculateHelper.ToMax();
}

void UGsUIWindowCraft::OnClickedCalcMinus1()
{
	_calculateHelper.Add(-1);
}

void UGsUIWindowCraft::OnClickedCalcPlus1()
{
	_calculateHelper.Add(1);
	ValidateCanCraftNow();
}

void UGsUIWindowCraft::OnClickedCalcOpen()
{
	OpenPopupCalculator();
}

void UGsUIWindowCraft::OnClickedCraft()
{
	if (ValidateCanCraftNow())
	{
		SetSpellingState(true);
	}
}

void UGsUIWindowCraft::OnClickedCraftCancel()
{
	SetSpellingState(false);
}

void UGsUIWindowCraft::OnFinishCalculatePopupCalculator(int64 InValue)
{
	_calculateHelper.SetCurrent(static_cast<int32>(InValue));
	_isOpeningPopupCalculator = false;
	InvalidateUIInterationState();
}

void UGsUIWindowCraft::OnNativeDestructPopupCalculator()
{
	_isOpeningPopupCalculator = false;
	InvalidateUIInterationState();
}

void UGsUIWindowCraft::OnCraftResult(const bool InIsSuccess)
{
	SetSpellingState(false);

	UpdateCraftTargetListItems();
	InvalidateCraftTargetListEntries();

	_calculateHelper.ToMin();

	InvalidateCraftTargetDetail();
}

void UGsUIWindowCraft::OnChangeCurrency(const CurrencyType InCurrencyType)
{
	const UGsCraftManager* CraftManager = GSCraft();
	if (nullptr == CraftManager)
	{
		GSLOG(Error, TEXT("nullptr == CraftManager"));
		return;
	}

	const FGsCraftTarget* SelectedCraftTarget = GetSelectedCraftTarget();

	const TArray<UObject*>& CraftTargetListItems = _craftTargetList->GetListItems();
	for (int32 i = 0; i < CraftTargetListItems.Num(); ++i)
	{
		UGsUICraftTargetListItem* ListItem = Cast<UGsUICraftTargetListItem>(CraftTargetListItems[i]);
#ifdef WITH_EDITOR
		if (nullptr == ListItem)
		{
			GSLOG(Error, TEXT("nullptr == ListItem, i:%d"), i);
			continue;
		}
#endif // WITH_EDITOR

		const FGsCraftTarget* CraftTarget = CraftManager->GetCraftTarget(ListItem->_craftTargetId);
		if (nullptr == CraftTarget)
		{
			GSLOG(Error, TEXT("nullptr == CraftTarget, ListItem->_craftTargetId:%d"), ListItem->_craftTargetId);
			continue;
		}

		if (!CraftTarget->GetCostPackage()->HasCostElement(InCurrencyType))
		{
			continue;
		}

		// 비용 정보에 이 아이템이 필요한 경우, 리프레시를 수행함.
		UGsUICraftTargetListEntry* ListEntry = Cast<UGsUICraftTargetListEntry>(_craftTargetList->GetEntryWidgetFromItem(ListItem));
		if (nullptr == ListEntry)
		{
			GSLOG(Error, TEXT("nullptr == ListEntry, i:%d"), i);
		}
		else
		{
			UGsUIWindowCraft::UpdateCraftTargetListItem(*ListItem);
			ListEntry->Invalidate();
		}

		// 현재 선택중인 제작대상인 경우, 상세 정보도 새로 고침
		if (CraftTarget == SelectedCraftTarget && nullptr != SelectedCraftTarget)
		{
			InvalidateCraftTargetDetail();
		}
	}
}

void UGsUIWindowCraft::OnChangeItem(const ItemId InItemId)
{
	const UGsCraftManager* CraftManager = GSCraft();
	if (nullptr == CraftManager)
	{
		GSLOG(Error, TEXT("nullptr == CraftManager"));
		return;
	}

	const FGsCraftTarget* SelectedCraftTarget = GetSelectedCraftTarget();

	const TArray<UObject*>& CraftTargetListItems = _craftTargetList->GetListItems();
	for (int32 i = 0; i < CraftTargetListItems.Num(); ++i)
	{
		UGsUICraftTargetListItem* ListItem = Cast<UGsUICraftTargetListItem>(CraftTargetListItems[i]);
#ifdef WITH_EDITOR
		if (nullptr == ListItem)
		{
			GSLOG(Error, TEXT("nullptr == ListItem, i:%d"), i);
			continue;
		}
#endif // WITH_EDITOR

		const FGsCraftTarget* CraftTarget = CraftManager->GetCraftTarget(ListItem->_craftTargetId);
		if (nullptr == CraftTarget)
		{
			GSLOG(Error, TEXT("nullptr == CraftTarget, ListItem->_craftTargetId:%d"), ListItem->_craftTargetId);
			continue;
		}

		if (!CraftTarget->GetCostPackage()->HasCostElement(InItemId))
		{
			continue;
		}

		// 비용 정보에 이 아이템이 필요한 경우, 리프레시를 수행함.
		UGsUICraftTargetListEntry* ListEntry = Cast<UGsUICraftTargetListEntry>(_craftTargetList->GetEntryWidgetFromItem(ListItem));
		if (nullptr == ListEntry)
		{
			GSLOG(Error, TEXT("nullptr == ListEntry, i:%d"), i);
		}
		else
		{
			UGsUIWindowCraft::UpdateCraftTargetListItem(*ListItem);
			ListEntry->Invalidate();
		}

		// 현재 선택중인 제작대상인 경우, 상세 정보도 새로 고침
		if (CraftTarget == SelectedCraftTarget && nullptr != SelectedCraftTarget)
		{
			InvalidateCraftTargetDetail();
		}
	}
}

void UGsUIWindowCraft::OnClickCraftBack()
{
	FGsGameStateCraft* GameStateCraft = UGsUIWindowCraft::GetGameStateCraft();
	if (nullptr == GameStateCraft)
	{
		return;
	}

	Parameters NextParameters;
	if (GameStateCraft->PopHistory(NextParameters))
	{
		SetParametersPrivate(NextParameters);
	}
}

void UGsUIWindowCraft::OnOkPopupSortCandidateGoods()
{
	InvalidateCraftMaterialBtns();
}

void UGsUIWindowCraft::SetIsTutorialInteraction(bool bIsTutorial)
{
	_bIsTutorialInteraction = bIsTutorial;

	if (false == _bIsTutorialInteraction)
	{
		OnTutorialInteraction.Unbind();
	}
}

void UGsUIWindowCraft::OnClickTutorialRegion()
{
	if (IsTutorialInteraction())
	{
		// 선택된게 없으면 강제로 선택
		UObject* selected = _craftTargetList->GetSelectedItem();
		if (nullptr == selected)
		{
			_craftTargetList->SetSelectedIndex(0);
		}
	}
}

UWidget* UGsUIWindowCraft::SetTutorial(bool bIsTutorial)
{
	_bIsTutorial = bIsTutorial;

	if (_bIsTutorial)
	{
		UObject* targetItem = nullptr;
		if (UObject* selected = _craftTargetList->GetSelectedItem())
		{
			targetItem = selected;
		}
		else
		{
			_craftTargetList->ScrollToTop();
			targetItem = _craftTargetList->GetItemAt(0);
		}

		if (UWidget* firstEntry = _craftTargetList->GetEntryWidgetFromItem(targetItem))
		{
			return firstEntry;
		}
	}

	return nullptr;
}
