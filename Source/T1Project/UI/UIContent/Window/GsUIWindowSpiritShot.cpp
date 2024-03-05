// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/GsUIWindowSpiritShot.h"

//#include "Runtime/UMG/Public/Animation/UMGSequencePlayer.h"
//#include "Runtime/UMG/Public/Animation/WidgetAnimation.h"
//#include "Runtime/UMG/Public/Components/ListView.h"
//#include "Runtime/Core/Public/Math/UnrealMath.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/Components/Image.h"
#include "Runtime/UMG/Public/Components/ListView.h"
#include "Runtime/UMG/Public/Components/PanelWidget.h"
#include "Runtime/UMG/Public/Components/ScrollBox.h"
#include "Runtime/UMG/Public/Components/SlateWrapperTypes.h"
#include "Runtime/UMG/Public/Components/RichTextBlock.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Runtime/UMG/Public/Components/Widget.h"
#include "Runtime/UMG/Public/Components/WidgetSwitcher.h"
#include "Runtime/UMG/Public/Components/CanvasPanel.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"


#include "Management/GsMessageHolder.h"
#include "Management/GsScopeGame.h"
#include "Management/GsScopeGlobal.h"

#include "GameFlow/GameContents/ContentsGame/GsContentsGameBase.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateSpiritShot.h"
#include "GameFlow/GameContents/GsContentsMode.h"
#include "GameFlow/GsGameFlowGame.h"

#include "Management/ScopeGame/GsSpiritShotmanager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Message/MessageParam/GsSpiritShotMessageParam.h"
#include "Net/GsNetSendServiceSeal.h"

#include "GameFlow/GameContents/GsContentsMode.h"

#include "SpiritShot/GsSpiritShotCategoryData.h"
#include "SpiritShot/GsSpiritShotLevelData.h"
#include "SpiritShot/GsSpiritShotStatData.h"

#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/ContentWidget/GsToggleButton.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "UI/UILib/Base/GsUIVFX.h"
#include "UI/UILib/Base/GsUIWidgetBase.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Common/GsUITabButtonCommon.h"
#include "UI/UIContent/Common/GsUICurrencyButton.h"
#include "UI/UIContent/Window/SpiritShot/GsUISpiritShotMaterialListItem.h"
#include "UI/UIContent/Window/SpiritShot/GsUISpiritShotStatListItem.h"
#include "UI/UIContent/Window/SpiritShot/GsUIPopupSpiritShotSumBuff.h"
#include "UI/UIContent/Common/Tooltip/GsUITooltipDesc.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Window/SpiritShot/GsUISpiritShotSceneTabBase.h"
#include "UI/UIContent/Window/SpiritShot/GsUITooltipSpiritShotProb.h"

#include "Currency/GsCostPackage.h"
#include "Currency/GsCurrencyHelper.h"
#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"
#include "Management/ScopeGame/GsStatChangeNotifyManager.h"
#include "Reward/GsRewardHelper.h"

#include "T1Project.h"

// DTGsPathDataUI 에 있는 UI 위젯 경로
const FName UGsUIWindowSpiritShot::WINDOW_ASSET_NAME = TEXT("WindowSpiritShot");

void UGsUIWindowSpiritShot::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 천마석 시스템 설명 가이드 페이지 연동에 필요
	_helpContentsKey = EGsHelpContents::SpiritShot;

	GSLOG(Log, TEXT("UGsUIWindowSpiritShot::NativeOnInitialized"));

#if WITH_EDITOR
	if (nullptr == _subClassOfCategoryBtn || nullptr == _subClassOfEnchantMaterial)
	{
		GSLOG(Error, TEXT("필수 오브젝트 링크 오류"));
	}
#endif

	// 상단 버튼
	_btnBack->OnClicked.AddUniqueDynamic(this, &UGsUIWindowSpiritShot::OnClickBack);
	_btnClose->OnClicked.AddUniqueDynamic(this, &UGsUIWindowSpiritShot::OnClickCloseAllStack);
	_btnHelp->OnClicked.AddUniqueDynamic(this, &UGsUIWindowSpiritShot::OnClickContentsHelp);

	// 카테고리 탭
	_categoryBtnSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_categoryBtnSlotHelper->Initialize(_subClassOfCategoryBtn, _scrollCategoryBtn);
	//_categoryBtnSlotHelper->OnCreateEntry.AddUniqueDynamic(this, &UGsUIWindowCommunity::OnCreateEntryCategoryTabBtn);
	_categoryBtnSlotHelper->OnRefreshEntry.AddUniqueDynamic(this, &UGsUIWindowSpiritShot::OnRefreshEntryCategoryTabBtn);
	_toggleGroupCategoryTabBtns.OnSelectChanged.BindUObject(this, &UGsUIWindowSpiritShot::OnSelectChangedCategoryTabBtn);

	// 잠식 통합 레벨 설명 팝업 버튼
	_btnSumLevelInfo->OnClicked.AddUniqueDynamic(this, &UGsUIWindowSpiritShot::OnClickedTotalLevelPopup);


	// 잠식 확률 툴팁 텍스트 조회
	FText::FindText(TEXT("SpiritShotUIText"), TEXT("ToolTip_Enchant_Rate"), _enchantProbTooltipText);

	// 잠식 확률 포맷팅 텍스트 : {0}%
	FText::FindText(TEXT("SpiritShotUIText"), TEXT("SpiritShot_Category_Rate"), _enchantProbSign);

	// 잠식 확률 툴팁 버튼
	_btnEnchantProbInfo->OnPressed.AddUniqueDynamic(this, &UGsUIWindowSpiritShot::OnPressedEnchantProbTooltip);

	// 잠식 확률 조절 버튼
	_switcherBtnAddProb->OnClicked.AddUniqueDynamic(this, &UGsUIWindowSpiritShot::OnClickedProbAddBtn);
	_switcherBtnMaxProb->OnClicked.AddUniqueDynamic(this, &UGsUIWindowSpiritShot::OnClickedProbMaxBtn);
	_switcherBtnSubProb->OnClicked.AddUniqueDynamic(this, &UGsUIWindowSpiritShot::OnClickedProbSubBtn);
	_switcherBtnMinProb->OnClicked.AddUniqueDynamic(this, &UGsUIWindowSpiritShot::OnClickedProbMinBtn);

	// 잠식 재료
	_enchantMaterialSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_enchantMaterialSlotHelper->Initialize(_subClassOfEnchantMaterial, _scrollEnchantMaterial);
	_enchantMaterialSlotHelper->OnRefreshEntry.AddUniqueDynamic(this, &UGsUIWindowSpiritShot::OnRefreshEntryEnchantMaterialList);

	// 잠식 버튼
	if (nullptr != _btnEnchant)
	{
		float tryButtonLockSecs = 0.5f;
		UGsDataContainManager* dataContainManager = GData();
		if (nullptr != dataContainManager)
		{
			const UGsGlobalConstant* globalConstant = dataContainManager->GetGlobalData();
			if (nullptr != globalConstant)
			{
				tryButtonLockSecs = globalConstant->_spiritShotWindowTryButtonLockSecs;
			}
		}

		_btnEnchant->SetOnClickLockTime(tryButtonLockSecs);
		_btnEnchant->OnClickCurrencyButton.BindUObject(this, &UGsUIWindowSpiritShot::OnClickedEnchantBtn);
	}

	//// 현재 잠식 효과
	//_enchantCurrentStatSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	//_enchantCurrentStatSlotHelper->Initialize(_subClassOfEnchantStat, _scrollCurrentStat);
	//_enchantCurrentStatSlotHelper->OnRefreshEntry.AddUniqueDynamic(this, &UGsUIWindowSpiritShot::OnRefreshEntryEnchantMaterialList);

	//// 다음 잠식 효과
	//_enchantNextStatSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	//_enchantNextStatSlotHelper->Initialize(_subClassOfEnchantStat, _scrollNextStat);
	//_enchantNextStatSlotHelper->OnRefreshEntry.AddUniqueDynamic(this, &UGsUIWindowSpiritShot::OnRefreshEntryEnchantMaterialList);


	// 잠식 효과 툴팁 텍스트 캐싱
	FText::FindText(TEXT("SpiritShotUIText"), TEXT("ToolTip_Category_Reward"), _enchantStatTooltipText);

	// 잠식 효과 툴팁
	_btnEnchantStatInfo->OnPressed.AddUniqueDynamic(this, &UGsUIWindowSpiritShot::OnPressedStatInfoTooltip);

	// 기본 레벨 아이디 값은 0 으로 설정
	_currentLevelId = INVALID_SPIRIT_SHOT_LEVEL_ID;

	// 연출 스킵 버튼
	_switcherBtnSkipPrev->OnClicked.AddUniqueDynamic(this, &UGsUIWindowSpiritShot::OnClickedSkipPrevAniBtn);

	// 연출 스킵 여부 초기화 (기본값은 false)
	_isSkipPrevAni = false;

	// 카테고리별로 보여주어야 할 SceneTab 을 캐싱한다. 
	// 이렇게 쓰는 이유는, 추후 카테고리 추가 시 코드 작업 없이 데이터 및 리소스 추가만으로 대응하길 원하기 때문
	for (int32 i = 0; i < _sceneTabSwitcher->GetNumWidgets(); ++i)
	{
		UWidget* sceneTabWidget = _sceneTabSwitcher->GetWidgetAtIndex(i);
		if (nullptr == sceneTabWidget)
		{
			GSLOG(Error, TEXT("nullptr == sceneTabWidget at Index : %d"), i);
			continue;
		}

		_sceneTabList.Emplace(MakeWeakObjectPtr(Cast<UGsUISpiritShotSceneTabBase>(sceneTabWidget)));
	}
}

void UGsUIWindowSpiritShot::NativeConstruct()
{
	Super::NativeConstruct();

	if (FGsStatChangeNotifyManager* statChangeNotifyMgr = GSStatChangeNotify())
	{
		statChangeNotifyMgr->Save(StatUpdateReason::SPIRIT_SHOT_ENCHANT);
	}

	// 연출 스킵 버튼 세팅
	_switcherBtnSkipPrev->SetSwitcherIndex(!_isSkipPrevAni);
}

bool UGsUIWindowSpiritShot::SyncContentState()
{
	FGsMessageHolder* messageHolder = GMessage();
	if (nullptr == messageHolder)
	{
		GSLOG(Error, TEXT("nullptr == messageHolder"));
		return false;
	}

	if (false == IsSameContentState(FGsContentsMode::InGame::ContentsSpiritShot))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_SPIRIT_SHOT);
		return true;
	}

	return false;
}

void UGsUIWindowSpiritShot::NativeDestruct()
{
	if (FGsStatChangeNotifyManager* statChangeNotifyMgr = GSStatChangeNotify())
	{
		statChangeNotifyMgr->CheckNotify(StatUpdateReason::SPIRIT_SHOT_ENCHANT);
	}

	Super::NativeDestruct();
}

TWeakObjectPtr<UGsUIWindowSpiritShot> UGsUIWindowSpiritShot::OpenWindow(const FGsContentsGameBase* InContentsGameBase)
{
	check(!!InContentsGameBase);
	check(InContentsGameBase->GetType() == FGsContentsMode::InGame::ContentsSpiritShot);

	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
	{
		GSLOG(Error, TEXT("nullptr == uiManager"));
		return nullptr;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(UGsUIWindowSpiritShot::WINDOW_ASSET_NAME);
	if (!widget.IsValid())
	{
		GSLOG(Error, TEXT("!widget.IsValid()"));
		return nullptr;
	}

	TWeakObjectPtr<UGsUIWindowSpiritShot> window = Cast<UGsUIWindowSpiritShot>(widget);
	if (!window.IsValid())
	{
		GSLOG(Error, TEXT("!window.IsValid()"));
		return nullptr;
	}

	return window;
}

void UGsUIWindowSpiritShot::InitializeData()
{
	// 확률 조절 값 초기화
	_rateModifyCount = 0;
}

void UGsUIWindowSpiritShot::ChangeSceneTabState(EGsStateSpiritShotSceneState InSceneState, EGsStateSpiritShotLevel InLevelState /*= EGsStateSpiritShotLevel::LEVEL_SAME*/, TArray<ItemDBIdAmountPair> InMaterialInfo /*= TArray<ItemDBIdAmountPair>()*/)
{
	const FGsSpiritShotLevelData* levelData = GetSelectedLevelData();
	if (nullptr == levelData)
	{
		GSLOG(Error, TEXT("nullptr == levelData"));
		return;
	}

	// 천마석 연출 위젯에 인자 전달 (현재 카테고리 탭의 연출 위젯에서 Play 된다)
	int32 tabIndex = _toggleGroupCategoryTabBtns.GetSelectedIndex();

	if (!_sceneTabList.IsValidIndex(tabIndex))
	{
		GSLOG(Error, TEXT("!_sceneTabList.IsValidIndex(tabIndex) at Index : %d"), tabIndex);
		return;
	}

	if (!_sceneTabList[tabIndex].IsValid())
	{
		GSLOG(Error, TEXT("!_sceneTabList[tabIndex].IsValid() at Index : %d"), tabIndex);
		return;
	}

	// 카테고리 선택에 맞게 SceneTab 도 변경한다
	_sceneTabSwitcher->SetActiveWidgetIndex(tabIndex);

	switch (InSceneState)
	{
	case EGsStateSpiritShotSceneState::CATEGORY_SWITCH:
	{
		// 강화 결과 재생
		_sceneTabList[tabIndex]->SetParameters(
			{
				static_cast<Level>(levelData->GetLevel()),
				EGsStateSpiritShotSceneState::CATEGORY_SWITCH,
				InLevelState,
				INVALID_SPIRIT_SHOT_CATEGORY_ID,
				INVALID_SPIRIT_SHOT_LEVEL_ID,
				0,
				TArray<ItemDBIdAmountPair>()
			});
		break;
	}
	case EGsStateSpiritShotSceneState::ENCHANT_PREV_VFX:
	{
		// 전조연출 재생
		_sceneTabList[tabIndex]->SetParameters(
			{
				static_cast<Level>(levelData->GetLevel()),
				EGsStateSpiritShotSceneState::ENCHANT_PREV_VFX,
				InLevelState,
				_paramters._categoryId,
				levelData->GetLevelId(),
				_rateModifyCount,
				MoveTemp(InMaterialInfo)
			});
		break;
	}
	case EGsStateSpiritShotSceneState::ENCHANT_RESULT_VFX:
	{
		// 강화 결과 재생
		_sceneTabList[tabIndex]->SetParameters(
			{
				static_cast<Level>(levelData->GetLevel()),
				EGsStateSpiritShotSceneState::ENCHANT_RESULT_VFX,
				InLevelState,
				INVALID_SPIRIT_SHOT_CATEGORY_ID,
				INVALID_SPIRIT_SHOT_LEVEL_ID,
				0,
				TArray<ItemDBIdAmountPair>(),
			});
		break;
	}
	default:
	{
		GSLOG(Error, TEXT("SceneState is not valid! SceneState : %d"), static_cast<int32>(InSceneState));
		return;
	}
	}
}

void UGsUIWindowSpiritShot::BlockInputIfEnchantInProgress(bool InBlockInput)
{
	//for (TWeakObjectPtr<UWidget> categoryWidget : _categoryTabList)
	//{
	//	UGsUITabButtonCommon* tabButtonWidget = Cast<UGsUITabButtonCommon>(categoryWidget);
	//	if (nullptr == tabButtonWidget)
	//	{
	//		GSLOG(Error, TEXT("nullptr == tabButtonWidget, Cast<UGsUITabButtonCommon>(categoryWidget) failed!"));
	//		continue;
	//	}

	//	tabButtonWidget->SetInputBlock(InBlockInput);
	//}

	if (InBlockInput)
	{
		_scrollCategoryBtn->SetVisibility(ESlateVisibility::HitTestInvisible);
		_btnEnchant->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		_scrollCategoryBtn->SetVisibility(ESlateVisibility::Visible);
		_btnEnchant->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UGsUIWindowSpiritShot::InvalidateCategoryTab()
{
	FGsSpiritShotManager* spiritShotMgr = GSSpiritShot();
	if (nullptr == spiritShotMgr)
	{
		GSLOG(Error, TEXT("nullptr == spiritShotMgr"));
		return;
	}

	// 카테고리 탭 재생성
	_categoryBtnSlotHelper->RefreshAll(spiritShotMgr->GetTotalCategoryCount());

	//// 카테고리 탭 캐싱
	//_categoryTabList.Empty();
	//for (UWidget* categoryWidget : _scrollCategoryBtn->GetAllChildren())
	//{
	//	_categoryTabList.Emplace(MakeWeakObjectPtr(categoryWidget));
	//}

	// 카테고리 탭 선택
	int32 tabIndex = spiritShotMgr->GetCategoryIdList().Find(_paramters._categoryId);
	_toggleGroupCategoryTabBtns.SetSelectedIndex(tabIndex == INDEX_NONE ? 0 : tabIndex);
}

void UGsUIWindowSpiritShot::InvalidateCategoryData()
{
	const FGsSpiritShotCategoryData* categoryData = GetSelectedCategoryData();
	if (nullptr == categoryData)
	{
		GSLOG(Error, TEXT("nullptr == categoryData"));
		return;
	}

	// 천마석 전체 레벨 갱신
	_txtTotalLevel->SetText(FText::FromString(FString::FromInt(GetEnchantSumLevel())));

	// 상단 카테고리 이름 갱신
	_txtCategory->SetText(categoryData->GetCategoryName());

	// 상단 카테고리 옆 레벨 갱신
	FText findText;
	FText::FindText(TEXT("SpiritShotUIText"), TEXT("SpiritShot_Level"), findText);
	_txtCategoryLevel->SetText(FText::Format(findText, categoryData->GetLevelData()->GetLevel()));
}

void UGsUIWindowSpiritShot::InvalidateEnchantProbability()
{
	const FGsSpiritShotLevelData* levelData = GetSelectedLevelData();
	if (nullptr == levelData)
	{
		GSLOG(Error, TEXT("nullptr == levelData"));
		return;
	}

	// 잠식 레벨이 최대치에 도달했을 경우, 잠식 확률 위젯을 전부 가려준다
	if (IsSelectedCategoryCompleted())
	{
		_widgetSwitcherProb->SetActiveWidgetIndex(1);
	}
	else
	{
		_widgetSwitcherProb->SetActiveWidgetIndex(0);
	}

	int32 prob = levelData->GetDefaultSuccessRate() + _rateModifyCount * levelData->GetRateModifyValue();
	prob = FMath::Clamp(prob, levelData->GetDefaultSuccessRate(), levelData->GetMaxSuccessRate());

	// 확률값 설정
	FText::FindText(TEXT("SpiritShotUIText"), TEXT("SpiritShot_Category_Rate"), _enchantProbSign);
	_txtProb->SetText(FText::Format(_enchantProbSign, FText::FromString(FString::FromInt(prob))));

	// 확률이 기본 확률값이라면 감소 버튼 비활성화
	if (0 == _rateModifyCount)
	{
		_switcherBtnSubProb->SetSwitcherIndex(EGsSpiritShotButtonState::Disabled);
		_switcherBtnMinProb->SetSwitcherIndex(EGsSpiritShotButtonState::Disabled);
		_switcherBtnSubProb->SetIsEnabled(false);
		_switcherBtnMinProb->SetIsEnabled(false);
	}
	else
	{
		_switcherBtnSubProb->SetSwitcherIndex(EGsSpiritShotButtonState::Enabled);
		_switcherBtnMinProb->SetSwitcherIndex(EGsSpiritShotButtonState::Enabled);
		_switcherBtnSubProb->SetIsEnabled(true);
		_switcherBtnMinProb->SetIsEnabled(true);
	}

	// 확률이 최대 확률값이라면 증가 버튼 비활성화
	if (levelData->GetMaxModifyCount() == _rateModifyCount)
	{
		_switcherBtnAddProb->SetSwitcherIndex(EGsSpiritShotButtonState::Disabled);
		_switcherBtnMaxProb->SetSwitcherIndex(EGsSpiritShotButtonState::Disabled);
		_switcherBtnAddProb->SetIsEnabled(false);
		_switcherBtnMaxProb->SetIsEnabled(false);
	}
	else
	{
		_switcherBtnAddProb->SetSwitcherIndex(EGsSpiritShotButtonState::Enabled);
		_switcherBtnMaxProb->SetSwitcherIndex(EGsSpiritShotButtonState::Enabled);
		_switcherBtnAddProb->SetIsEnabled(true);
		_switcherBtnMaxProb->SetIsEnabled(true);
	}


	// 하락 확률이 있을 경우, 윈도우에서 하락 경고 이미지 활성화
	if (nullptr == _fallAlertPanel)
	{
		GSLOG(Error, TEXT("nullptr == _fallAlertPanel"));
	}
	else
	{
		if (levelData->GetLevelFallRate() > 0)
		{
			_fallAlertPanel->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			_fallAlertPanel->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UGsUIWindowSpiritShot::InvalidateCost()
{
	const FGsSpiritShotLevelData* levelData = GetSelectedLevelData();
	if (nullptr == levelData)
	{
		GSLOG(Error, TEXT("nullptr == levelData"));
		return;
	}

	// 재료 값 갱신
	_costPackage = levelData->CreateItemCostPackage(_rateModifyCount);

	// 재료 목록 UI 갱신 (골드 미포함)
	_enchantMaterialSlotHelper->RefreshAll(_costPackage->GetCostElementsCount());
}

void UGsUIWindowSpiritShot::InvalidateCurrency()
{
	const FGsSpiritShotLevelData* levelData = GetSelectedLevelData();
	if (nullptr == levelData)
	{
		GSLOG(Error, TEXT("nullptr == levelData"));
		return;
	}

	// 잠식 버튼에 텍스트 세팅 (잠식)
	FText findText;
	FText::FindText(TEXT("SpiritShotUIText"), TEXT("SpiritShot_Enchant"), findText);
	_btnEnchant->SetButtonName(findText);

	// 잠식 버튼에 골드 세팅 및 부족 유무 업데이트
	_btnEnchant->SetData(CurrencyType::GOLD, levelData->GetRequireGold());
	_btnEnchant->UpdateAmountTextColorLocalPlayer();

	// 어차피 추후 '잠식 확률' 부분을 전부 가릴 거라 큰 의미는 없지만, 만렙 도달시 잠식 버튼 비활성화
	if (IsSelectedCategoryCompleted())
	{
		_btnEnchant->SetIsEnabled(false);
	}
	else
	{
		_btnEnchant->SetIsEnabled(true);
	}
}

void UGsUIWindowSpiritShot::InvalidateEnchantStat()
{
	const FGsSpiritShotLevelData* levelData = GetSelectedLevelData();
	if (nullptr == levelData)
	{
		GSLOG(Error, TEXT("nullptr == levelData"));
		return;
	}

	// 현재 레벨 세팅 (Lv.{0})
	{
		FText findText;
		FText::FindText(TEXT("SpiritShotUIText"), TEXT("SpiritShot_Level"), findText);
		_txtCurrentLevel->SetText(FText::Format(findText, FText::FromString(FString::FromInt(levelData->GetLevel()))));
	}


	// 현재 스탯 효과 데이터 세팅
	_txtCurrentLevelStat->SetText(levelData->GetStatData()->GetStatDescText());

	// 만약 효과 텍스트가 비어 있다면, "효과 없음" 처리
	if (_txtCurrentLevelStat->Text.IsEmpty())
	{
		_panelNoCurrentStat->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		_panelNoCurrentStat->SetVisibility(ESlateVisibility::Collapsed);
	}


	// 다음 레벨 세팅 (Lv.{0})
	{
		int32 nextLevel = levelData->GetNextLevelId() == INVALID_SPIRIT_SHOT_LEVEL_ID ? 0 : levelData->GetLevel() + 1;
		if (INVALID_SPIRIT_SHOT_LEVEL_ID == nextLevel)
		{
			_txtNextLevel->SetText(FText::GetEmpty());
		}
		else
		{
			FText findText;
			FText::FindText(TEXT("SpiritShotUIText"), TEXT("SpiritShot_Level"), findText);
			_txtNextLevel->SetText(FText::Format(findText, FText::FromString(FString::FromInt(nextLevel))));
		}
	}

	// 다음 스탯 효과 데이터 세팅
	SpiritShotLevelId nextLevelId = levelData->GetNextLevelId();
	if (INVALID_SPIRIT_SHOT_LEVEL_ID == nextLevelId)
	{
		_txtNextLevelStat->SetText(FText::GetEmpty());
	}
	else
	{
		if (const FGsSpiritShotLevelData* nextLevelData = GetSpecificLevelData(nextLevelId))
		{
			_txtNextLevelStat->SetText(nextLevelData->GetStatData()->GetStatDescText());
		}
	}

	// 현재 레벨이 만렙일 경우, 다음 레벨 공간 "최대 레벨에 도달했습니다" 처리
	if (levelData->GetNextLevelId() == INVALID_SPIRIT_SHOT_LEVEL_ID)
	{
		_panelNoNextStat->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		_panelNoNextStat->SetVisibility(ESlateVisibility::Collapsed);
	}


	//// 스탯 데이터 세팅되어 있는지 체크
	//if (levelData->GetStatData().IsValid())
	//{
	//	_enchantCurrentStatSlotHelper->RefreshAll(levelData->GetStatData().Pin()->GetTotalStatCount());
	//}
	//else
	//{
	//	_enchantCurrentStatSlotHelper->RefreshAll(0);
	//}


	//// 다음 스탯 데이터에 대해 Refresh
	//SpiritShotLevelId nextLevelId = levelData->GetNextLevelId();
	//if (INVALID_SPIRIT_SHOT_LEVEL_ID == nextLevelId)
	//{
	//	_enchantNextStatSlotHelper->RefreshAll(0);
	//}
	//else
	//{
	//	const FGsSpiritShotLevelData* nextLevelData = GetSpecificLevelData(nextLevelId);
	//	if (nullptr == nextLevelData)
	//	{
	//		GSLOG(Error, TEXT("nullptr == nextLevelData, nextLevelId : %d"), nextLevelId);
	//		return;
	//	}

	//	if (nextLevelData->GetStatData().IsValid())
	//	{
	//		_enchantNextStatSlotHelper->RefreshAll(nextLevelData->GetStatData().Pin()->GetTotalStatCount());
	//	}
	//	else
	//	{
	//		_enchantNextStatSlotHelper->RefreshAll(0);
	//	}
	//}
}

const FGsSpiritShotCategoryData* UGsUIWindowSpiritShot::GetSelectedCategoryData() const
{
	FGsSpiritShotManager* spiritShotMgr = GSSpiritShot();
	if (nullptr == spiritShotMgr)
	{
		GSLOG(Error, TEXT("nullptr == spiritShotMgr"));
		return nullptr;
	}

	return spiritShotMgr->GetCategoryData(_paramters._categoryId);
}

const FGsSpiritShotLevelData* UGsUIWindowSpiritShot::GetSelectedLevelData() const
{
	FGsSpiritShotManager* spiritShotMgr = GSSpiritShot();
	if (nullptr == spiritShotMgr)
	{
		GSLOG(Error, TEXT("nullptr == spiritShotMgr"));
		return nullptr;
	}

	return spiritShotMgr->GetLevelData(_paramters._categoryId);
}

const FGsSpiritShotLevelData* UGsUIWindowSpiritShot::GetSpecificLevelData(SpiritShotLevelId InLevelId) const
{
	FGsSpiritShotManager* spiritShotMgr = GSSpiritShot();
	if (nullptr == spiritShotMgr)
	{
		GSLOG(Error, TEXT("nullptr == spiritShotMgr"));
		return nullptr;
	}

	return spiritShotMgr->GetLevelDataWithLevel(InLevelId);
}

const FGsSpiritShotCategoryData* UGsUIWindowSpiritShot::GetCategoryDataAtIndex(int32 InIndex) const
{
	FGsSpiritShotManager* spiritShotMgr = GSSpiritShot();
	if (nullptr == spiritShotMgr)
	{
		GSLOG(Error, TEXT("nullptr == spiritShotMgr"));
		return nullptr;
	}

	return spiritShotMgr->GetCategoryDataAtIndex(InIndex);
}

const int32 UGsUIWindowSpiritShot::GetEnchantSumLevel() const
{
	FGsSpiritShotManager* spiritShotMgr = GSSpiritShot();
	if (nullptr == spiritShotMgr)
	{
		GSLOG(Error, TEXT("nullptr == spiritShotMgr"));
		return 0;
	}

	return spiritShotMgr->GetSumLevel();
}

bool UGsUIWindowSpiritShot::IsSelectedCategoryCompleted() const
{
	const FGsSpiritShotCategoryData* categoryData = GetSelectedCategoryData();
	if (nullptr == categoryData)
	{
		GSLOG(Error, TEXT("nullptr == categoryData"));
		return false;
	}

	return categoryData->GetCurrentState() == EGsSpiritShotEnchantState::Completed;
}

void UGsUIWindowSpiritShot::SetParameters(Parameters&& InParameters, bool InInternal /*= false*/)
{
	_paramters = MoveTemp(InParameters);

	// 현재 카테고리에서 바라보고 있는 레벨
	const FGsSpiritShotLevelData* levelData = GetSelectedLevelData();
	if (nullptr == levelData)
	{
		GSLOG(Error, TEXT("nullptr == levelData"));
		return;
	}


	// 윈도우가 바라보고 있는 레벨 Id 가 바뀌었을 경우, 데이터 초기화 
	// SetParameters 함수는 외부에서 윈도우를 열거나, 카테고리 탭을 바꾸는 경우이므로 무조건 초기화
	InitializeData();
	_currentLevelId = levelData->GetLevelId();

	// 카테고리와 강화 버튼 활성화
	BlockInputIfEnchantInProgress(false);

	// 외부에서 창 오픈한 경우, 카테고리 탭 재생성
	if (!InInternal)
	{
		InvalidateCategoryTab();
	}

	InvalidateCategoryData();
	InvalidateEnchantProbability();
	InvalidateCost();
	InvalidateCurrency();
	InvalidateEnchantStat();

	ChangeSceneTabState(EGsStateSpiritShotSceneState::CATEGORY_SWITCH);
}

void UGsUIWindowSpiritShot::OnSpiritShotAckEnchant(bool InPacketNoError, SpiritShotLevelId InLevelId, TArray<ItemIdAmountPair> InItemIdAmountPairList)
{
	// 만약 패킷 요청 자체가 실패했다면, 카테고리/강화 버튼 잠금만 풀어주고 return 한다
	if (!InPacketNoError)
	{
		GSLOG(Error, TEXT("!InPacketNoError, Call 'BlockInputIfEnchantInProgress(false)'"));
		BlockInputIfEnchantInProgress(false);
		return;
	}

	// 현재 카테고리에서 바라보고 있는 레벨
	const FGsSpiritShotLevelData* levelData = GetSelectedLevelData();
	if (nullptr == levelData)
	{
		GSLOG(Error, TEXT("nullptr == levelData"));
		return;
	}
	
	// 강화 단계 상승/유지/하락 여부를 SceneTab 에 전달할 것임
	EGsStateSpiritShotLevel levelState;

	// 강화 단계 하락/유지/상승에 따라 티커 출력 (현재는 천마석 전용 애니메이션 티커를, 메인 강화 연출 이후에 띄워주도록 변경)
	{
		//FText tickerMessage;

		// 강화 단계 상승
		if (levelData->GetLevelId() > _currentLevelId)
		{
			//FText::FindText(TEXT("SpiritShotUIText"), TEXT("Alert_Enchant_Result_Success"), tickerMessage);
			levelState = EGsStateSpiritShotLevel::LEVEL_UP;
		}
		// 강화 단계 유지
		else if (levelData->GetLevelId() == _currentLevelId)
		{
			//FText::FindText(TEXT("SpiritShotUIText"), TEXT("Alert_Enchant_Result_Fail_Keep"), tickerMessage);
			levelState = EGsStateSpiritShotLevel::LEVEL_SAME;
		}
		// 강화 단계 하락
		else
		{
			//FText::FindText(TEXT("SpiritShotUIText"), TEXT("Alert_Enchant_Result_Fail_Drop"), tickerMessage);
			levelState = EGsStateSpiritShotLevel::LEVEL_DOWN;
		}
				
		//FGsUIHelper::TrayTickerLauncher(tickerMessage);
	}


	// 윈도우가 바라보고 있는 레벨 Id 가 바뀌었을 경우, 데이터 초기화 및 UI 갱신
	// 레벨이 유지될 경우, 레벨 및 확률값 변경하지 않는다.
	if (levelData->GetLevelId() != _currentLevelId)
	{
		InitializeData();
		_currentLevelId = levelData->GetLevelId();

		InvalidateCategoryData();
		InvalidateEnchantProbability();
		InvalidateEnchantStat();
	}

	// 재료 및 재화는 무조건 갱신
	InvalidateCost();
	InvalidateCurrency();

	
	// 만약 실패 보상이 있을 경우, 각각의 아이템에 대해 채팅 메시지 출력
	UGsItemManager* itemMgr = GItem();
	if (nullptr == itemMgr)
	{
		GSLOG(Error, TEXT("nullptr == itemMgr"));
		return;
	}

	for (const ItemIdAmountPair& pair : InItemIdAmountPairList)
	{
		FText itemName = itemMgr->GetItemNameByTId(pair.mItemId);
		FText itemAmount = FText::FromString(FString::FromInt(pair.mItemAmount));

		// 천마석 잠식에 실패하여 {0} {1}개를 획득했습니다.
		FText rewardText;
		FText::FindText(TEXT("SpiritShotUIText"), TEXT("Alert_Enchant_Result_Fail_Reward"), rewardText);

		rewardText = FText::Format(rewardText, itemName, itemAmount);

		FGsUIHelper::AddChatMessageSystem(rewardText);
	}

	ChangeSceneTabState(EGsStateSpiritShotSceneState::ENCHANT_RESULT_VFX, levelState);
}

void UGsUIWindowSpiritShot::OnChangeCurrency(const CurrencyType InCurrencyType)
{
	InvalidateCurrency();
}

void UGsUIWindowSpiritShot::OnChangeItem(const ItemId InItemId)
{
	if (_costPackage.IsValid() && _costPackage->HasCostElement(InItemId))
	{
		InvalidateCost();
	}
}

void UGsUIWindowSpiritShot::OnUnlockCategory()
{
	InvalidateCategoryTab();
}

void UGsUIWindowSpiritShot::OnFinishedEnchantEffect()
{
	BlockInputIfEnchantInProgress(false);
}

void UGsUIWindowSpiritShot::OnClickedTotalLevelPopup()
{

	const FGsSpiritShotManager* spiritShotMgr = GSSpiritShot();
	if (nullptr == spiritShotMgr)
	{
		GSLOG(Error, TEXT("nullptr == spiritShotMgr"));
		return;
	}


	// 천마석 보상 정보 팝업을 띄운다
	UGsUIPopupSpiritShotSumBuff* popupSpiritShotSumBuff = Cast<UGsUIPopupSpiritShotSumBuff>(GUI()->OpenAndGetWidget(TEXT("PopupSpiritShotSumBuff")));
	popupSpiritShotSumBuff->SetParameters(
		{
			spiritShotMgr->GetPassivityCollectionData(),
			spiritShotMgr->GetSumLevelPassivityIdPairList()
		}
	);
}

//void UGsUIWindowSpiritShot::OnCreateEntryCategoryTabBtn(UWidget* InEntry)
//{
//	if (auto tab = Cast<UGsUITabButtonCommon>(InEntry))
//	{
//		_toggleGroupCategoryTabBtns.AddToggleWidget(tab);
//	}
//}

void UGsUIWindowSpiritShot::OnRefreshEntryCategoryTabBtn(int32 InIndex, UWidget* InEntry)
{
	UGsUITabButtonCommon* tabButtonCommon = Cast<UGsUITabButtonCommon>(InEntry);
	if (nullptr == tabButtonCommon)
	{
		GSLOG(Error, TEXT("nullptr == tabButtonCommon, *InEntry->GetName():%s"), *InEntry->GetName());
		return;
	}

	const FGsSpiritShotCategoryData* categoryData = GetCategoryDataAtIndex(InIndex);
	if (nullptr == categoryData)
	{
		GSLOG(Error, TEXT("nullptr == categoryData"));
		return;
	}

	// 잠금 상태 갱신
	if (categoryData->GetCurrentState() == EGsSpiritShotEnchantState::Locked)
	{
		tabButtonCommon->SetTitleText(FText::GetEmpty());
		tabButtonCommon->SetLockedText(categoryData->GetCategoryName());
		tabButtonCommon->SetIsEnabled(false);
	}
	else
	{
		tabButtonCommon->SetTitleText(categoryData->GetCategoryName());
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

void UGsUIWindowSpiritShot::OnSelectChangedCategoryTabBtn(int32 InIndex)
{
	if (0 > InIndex)
	{
		return;
	}

	const FGsSpiritShotManager* spiritShotMgr = GSSpiritShot();
	if (nullptr == spiritShotMgr)
	{
		GSLOG(Error, TEXT("nullptr == spiritShotMgr"));
		return;
	}

	SpiritShotCategoryId id = spiritShotMgr->GetCategoryIdAtIndex(InIndex);
	if (INVALID_SPIRIT_SHOT_CATEGORY_ID == id)
	{
		GSLOG(Error, TEXT("INVALID_SPIRIT_SHOT_CATEGORY_ID == id"));
		return;
	}

	SetParameters(UGsUIWindowSpiritShot::Parameters(id), true);
}

void UGsUIWindowSpiritShot::OnPressedEnchantProbTooltip()
{
	if (nullptr == _tooltipEnchantProb)
	{
		GSLOG(Error, TEXT("nullptr == _tooltipEnchantProb"));
		return;
	}

	const FGsSpiritShotLevelData* levelData = GetSelectedLevelData();
	if (nullptr == levelData)
	{
		GSLOG(Error, TEXT("nullptr == levelData"));
		return;
	}

	// _enchantProbSign : {0}%

	FText minProbText;
	minProbText = FText::Format(_enchantProbSign, FText::FromString(FString::FromInt(levelData->GetDefaultSuccessRate())));

	FText maxProbText;
	maxProbText = FText::Format(_enchantProbSign, FText::FromString(FString::FromInt(levelData->GetMaxSuccessRate())));

	FText fallProbText;
	fallProbText = FText::Format(_enchantProbSign, FText::FromString(FString::FromInt(levelData->GetLevelFallRate())));


	if (_tooltipEnchantProb->IsOpen())
	{
		_tooltipEnchantProb->Close();
	}

	_tooltipEnchantProb->InvalidateAll(_enchantProbTooltipText, MoveTemp(minProbText), MoveTemp(maxProbText), MoveTemp(fallProbText));
	_tooltipEnchantProb->Open();
}

void UGsUIWindowSpiritShot::OnClickedProbMinBtn()
{
	const FGsSpiritShotLevelData* levelData = GetSelectedLevelData();
	if (nullptr == levelData)
	{
		GSLOG(Error, TEXT("nullptr == levelData"));
		return;
	}

	if (0 == _rateModifyCount)
	{
		return;
	}

	_rateModifyCount = 0;

	InvalidateEnchantProbability();
	InvalidateCost();
}

void UGsUIWindowSpiritShot::OnClickedProbSubBtn()
{
	const FGsSpiritShotLevelData* levelData = GetSelectedLevelData();
	if (nullptr == levelData)
	{
		GSLOG(Error, TEXT("nullptr == levelData"));
		return;
	}

	if (0 == _rateModifyCount)
	{
		return;
	}

	_rateModifyCount = FMath::Clamp(_rateModifyCount - 1, 0, levelData->GetMaxModifyCount());

	InvalidateEnchantProbability();
	InvalidateCost();
}

void UGsUIWindowSpiritShot::OnClickedProbMaxBtn()
{
	const FGsSpiritShotLevelData* levelData = GetSelectedLevelData();
	if (nullptr == levelData)
	{
		GSLOG(Error, TEXT("nullptr == levelData"));
		return;
	}

	if (_rateModifyCount == levelData->GetMaxModifyCount())
	{
		return;
	}

	_rateModifyCount = levelData->GetMaxModifyCount();

	InvalidateEnchantProbability();
	InvalidateCost();
}

void UGsUIWindowSpiritShot::OnClickedProbAddBtn()
{
	const FGsSpiritShotLevelData* levelData = GetSelectedLevelData();
	if (nullptr == levelData)
	{
		GSLOG(Error, TEXT("nullptr == levelData"));
		return;
	}

	if (_rateModifyCount == levelData->GetMaxModifyCount())
	{
		return;
	}

	_rateModifyCount = FMath::Clamp(_rateModifyCount + 1, 0, levelData->GetMaxModifyCount());

	InvalidateEnchantProbability();
	InvalidateCost();
}

void UGsUIWindowSpiritShot::OnClickedEnchantBtn()
{
	// 비용이 충분하다면, 바로 강화 요청 시도
	const FGsSpiritShotLevelData* levelData = GetSelectedLevelData();
	if (nullptr == levelData)
	{
		GSLOG(Error, TEXT("nullptr == levelData"));
		return;
	}

	// 현재 _costPackage 에는 아이템만 담기므로, 재화까지 포함한 비용 체크해야 함
	TSharedRef<FGsCostPackage> totalCostPackage = levelData->CreateTotalCostPackage(_rateModifyCount);

	// 부족할 경우, 부족 팝업을 띄운다
	if (!totalCostPackage->PlayerHasEnough())
	{
		FGsCurrencyHelper::OpenLackCostPackagePopup(*totalCostPackage);
	}
	// 만약 실패 보상을 수령 불가능한 상태일 경우, 관련 티커를 띄운다
	else if (!FGsRewardHelper::CheckRewardState(levelData->GetFailRewardId()))
	{
		return;
	}
	else
	{
		GSLOG(Log, TEXT("FGsSpiritShotMessageParamReqEnchant, Level Id : %d, Current Level : %d"), levelData->GetLevelId(), levelData->GetLevel());

		// 강화가 시작되었다면, 강화 버튼 및 카테고리 탭 인풋도 막아준다
		BlockInputIfEnchantInProgress(true);

		UGsItemManager* itemManager = GItem();
		if (nullptr == itemManager)
		{
			GSLOG(Error, TEXT("nullptr == itemManager"));
			return;
		}

		TArray<ItemDBIdAmountPair> inMaterialInfos;
		if (!itemManager->MakeDisposableItems(inMaterialInfos, *totalCostPackage))
		{
			GSLOG(Warning, TEXT("!itemManager->MakeDisposableItems(inMaterialInfos, totalCostPackage), Level Id : %d, Current Level : %d"), levelData->GetLevelId(), levelData->GetLevel());
			return;
		}

		// 전조 연출 Skip 의 경우, 바로 패킷 전송
		if (_isSkipPrevAni)
		{
			FGsSpiritShotMessageParamReqEnchant(_paramters._categoryId, levelData->GetLevelId(), _rateModifyCount, &inMaterialInfos).Send();
		}
		// 전조 연출 Skip 안할 경우, 전조 연출 이후 패킷 전송해야 함
		else
		{
			ChangeSceneTabState(EGsStateSpiritShotSceneState::ENCHANT_PREV_VFX, EGsStateSpiritShotLevel::LEVEL_SAME, MoveTemp(inMaterialInfos));
		}

	}
}

void UGsUIWindowSpiritShot::OnRefreshEntryEnchantMaterialList(int32 InIndex, UWidget* InEntry)
{
	if (!_costPackage.IsValid())
	{
		GSLOG(Error, TEXT("!_costPackage.IsValid()"));
		return;
	}

	const FGsCostElement* costElement = _costPackage->GetCostElementAt(InIndex);
	if (nullptr == costElement)
	{
		GSLOG(Error, TEXT("nullptr == costElement, InIndex:%d"), InIndex);
		return;
	}

	UGsUISpiritShotMaterialListItem* listItem = Cast<UGsUISpiritShotMaterialListItem>(InEntry);
	if (nullptr == listItem)
	{
		GSLOG(Error, TEXT("nullptr == listItem, InIndex:%d"), InIndex);
		return;
	}

	listItem->SetParameters(UGsUISpiritShotMaterialListItem::Parameters({
		InIndex,
		costElement->_costType,
		costElement->GetCurrencyType(),
		costElement->GetItemId(),
		costElement->GetItemLevel(),
		costElement->GetCostAmount(),
		costElement->PlayerHasAmountMax(),
		costElement->PlayerHasEnough()
		}));
}

void UGsUIWindowSpiritShot::OnPressedStatInfoTooltip()
{
	if (nullptr == _enchantStatInfoTooltip)
	{
		GSLOG(Error, TEXT("nullptr == _enchantStatInfoTooltip"));
		return;
	}

	if (_enchantStatInfoTooltip->IsOpen())
	{
		_enchantStatInfoTooltip->Close();
	}

	_enchantStatInfoTooltip->SetDescText(_enchantStatTooltipText);
	_enchantStatInfoTooltip->Open();
}

void UGsUIWindowSpiritShot::OnClickedSkipPrevAniBtn()
{
	_isSkipPrevAni = !_isSkipPrevAni;

	_switcherBtnSkipPrev->SetSwitcherIndex(!_isSkipPrevAni);
}

//void UGsUIWindowSpiritShot::OnRefreshEntryCurrentStatList(int32 InIndex, UWidget* InEntry)
//{
//}
//
//void UGsUIWindowSpiritShot::OnRefreshEntryNextStatList(int32 InIndex, UWidget* InEntry)
//{
//}