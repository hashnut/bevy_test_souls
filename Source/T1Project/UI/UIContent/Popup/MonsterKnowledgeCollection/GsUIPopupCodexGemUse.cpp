// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Popup/MonsterKnowledgeCollection/GsUIPopupCodexGemUse.h"

#include "../Management/ScopeGlobal/GsUIManager.h"
#include "../Management/ScopeGame/GsCodexManager.h"
#include "../UI/UIControlLib/ContentWidget/GsButton.h"
#include "../UI/UIControlLib/ContentWidget/GsToggleButton.h"
#include "../UI/UIContent/Common/GsUITabButtonCommon.h"
#include "../UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UMG/Public/Components/ScrollBox.h"
#include "UMG/Public/Components/TileView.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/HorizontalBox.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "UMG/Public/Components/Image.h"
#include "UMG/Public/Components/PanelWidget.h"
#include "../Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperSprite.h"
#include "../UI/UIContent/Helper/GsUIHelper.h"
#include "../Net/GsNetSendServiceWorld.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "T1Project.h"
#include "Codex/GsCodexTypes.h"
#include "GameObject/Stat/GsStatHelper.h"
#include "UI/UIContent/Popup/MonsterKnowledgeCollection/GsUICodexGemRequirementListEntry.h"
#include "../Management/ScopeGame/GsCodexManager.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIContent/Common/Inventory/GsWrapBoxInventoryIconSelector.h"
#include "UI/UIContent/Common/Icon/GsUIIconBase.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIContent/Common/GsUIProgressBar.h"
#include "../UI/UILib/Base/GsUIVFX.h"
#include "Currency/GsCostPackage.h"
#include "Currency/GsCurrencyHelper.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Popup/GsUIPopupYesNo.h"
#include "UI/UIContent/Common/Tooltip/GsUITooltipDesc.h"

#include "Message/MessageParam/GsMonsterKnowledgeCollection.h"
#include "MonsterKnowledgeCollection/EMonsterKnowledgeEnumData.h"
#include "Message/MessageParam/GsItemMessageParam.h"
#include "Item/GsItemManager.h"
#include "Management/GsMessageHolder.h"
#include "UI/UIContent/Helper/GsUIHelper.h"


//#define TEST_LOG_CODEX_GEM_USE


void UGsUIPopupCodexGemUse::NativeOnInitialized()
{
	// 대륙 카테고리
	_slotHelperCategoryView = NewObject<UGsDynamicPanelSlotHelper>();
	_slotHelperCategoryView->Initialize(_subClassOfCategoryView, _scrollCategoryView);
	_slotHelperCategoryView->OnRefreshEntry.AddUniqueDynamic(this, &UGsUIPopupCodexGemUse::OnRefreshEntryCategoryView);
	_toggleGroupCategoryView.OnSelectChanged.BindUObject(this, &UGsUIPopupCodexGemUse::OnSelectChangedCategoryView);

	// 사용 가능 지식의 결정
	if (nullptr == _slotHelperIcon)
	{
		_slotHelperIcon = NewObject<UGsDynamicIconSlotHelper>(this);
		_slotHelperIcon->Initialize(_iconSelector);
		_slotHelperIcon->OnRefreshIcon.AddUniqueDynamic(this, &UGsUIPopupCodexGemUse::OnRefreshGemIconItem);
	}

	_btnLeftMapGroup->OnClicked.AddUniqueDynamic(this, &UGsUIPopupCodexGemUse::OnClickedLeftMapGroup);
	_btnRightMapGroup->OnClicked.AddUniqueDynamic(this, &UGsUIPopupCodexGemUse::OnClickedRightMapGroup);
	_btnCalcReset->OnClicked.AddUniqueDynamic(this, &UGsUIPopupCodexGemUse::OnClickedCalcReset);
	_btnDecrease->OnClicked.AddUniqueDynamic(this, &UGsUIPopupCodexGemUse::OnClickedDecrease);
	_btnDecreaseTen->OnClicked.AddUniqueDynamic(this, &UGsUIPopupCodexGemUse::OnClickedDecreaseTen);
	_btnIncrease->OnClicked.AddUniqueDynamic(this, &UGsUIPopupCodexGemUse::OnClickedIncrease);
	_btnIncreaseTen->OnClicked.AddUniqueDynamic(this, &UGsUIPopupCodexGemUse::OnClickedIncreaseTen);
	_btnCalcMax->OnClicked.AddUniqueDynamic(this, &UGsUIPopupCodexGemUse::OnClickedCalcMax);
	_btnCancel->OnClicked.AddUniqueDynamic(this, &UGsUIPopupCodexGemUse::OnClickedCancel);
	_btnApply->OnClicked.AddUniqueDynamic(this, &UGsUIPopupCodexGemUse::OnClickedApply);

	_btnTooltip->OnPressed.AddUniqueDynamic(this, &UGsUIPopupCodexGemUse::OnPressedBtnTooltip);

	FText::FindText(TEXT("CodexUIText"), TEXT("UI_MapList_Text_MapGroupLevel"), _levelText);
	FText::FindText(TEXT("CodexUIText"), TEXT("UI_MapList_Popup_GemUse_ExceedKnowledge"), _knowledgeGemBurnText);

	_selectedKnowledgeGemIndex = -1;
}

void UGsUIPopupCodexGemUse::NativeConstruct()
{
	Super::NativeConstruct();

	_progressBarExp->SetProgressText(FText::GetEmpty());
	_effectAmountChanged->HideVFX();

	// 아이템 보유 수치 변동
	_itemActionDelegates.Empty();
	_itemActionDelegates.Emplace(GMessage()->GetItem().AddUObject(MessageItem::ADDITEMLIST, this, &UGsUIPopupCodexGemUse::OnAddItemList));
	_itemActionDelegates.Emplace(GMessage()->GetItem().AddUObject(MessageItem::UPDATEITEM, this, &UGsUIPopupCodexGemUse::OnUpdateItem));
	_itemActionDelegates.Emplace(GMessage()->GetItem().AddUObject(MessageItem::REMOVEITEM, this, &UGsUIPopupCodexGemUse::OnRemoveItem));

	_monsterKnowledeDelegates.Empty();
	_monsterKnowledeDelegates.Emplace(GMessage()->GetMonsterKnowledgeCollection().AddUObject(MessageContentMonsterKnowledge::INVALIDATE_CODEX_MAP_GROUP_EXP_UPDATE, this, &UGsUIPopupCodexGemUse::OnGainKnowledge));
	_monsterKnowledeDelegates.Emplace(GMessage()->GetMonsterKnowledgeCollection().AddUObject(MessageContentMonsterKnowledge::INVALIDATE_CODEX_MAP_GROUP_GEM, this, &UGsUIPopupCodexGemUse::OnGainKnowledgeGem));
}

void UGsUIPopupCodexGemUse::NativeDestruct()
{
	_iconSelector->RemoveAllChildren();
	
	FGsMessageHolder* messageHolder = GMessage();
	if (nullptr == messageHolder)
	{
		GSLOG(Error, TEXT("nullptr == messageHolder"));
		return;
	}

	MItem& itemMessageHandler = messageHolder->GetItem();
	for (const TPair<MessageItem, FDelegateHandle>& delegatePair : _itemActionDelegates)
	{
		itemMessageHandler.Remove(delegatePair);
	}
	_itemActionDelegates.Empty();
	
	MMonsterKnowledgeCollection& monsterCollectionMessageHandler = messageHolder->GetMonsterKnowledgeCollection();
	for (const TPair<MessageContentMonsterKnowledge, FDelegateHandle>& delegatePair : _monsterKnowledeDelegates)
	{
		monsterCollectionMessageHandler.Remove(delegatePair);
	}
	_monsterKnowledeDelegates.Empty();

	Super::NativeDestruct();
}

void UGsUIPopupCodexGemUse::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	InvalidateProgressBar(InDeltaTime);
}

void UGsUIPopupCodexGemUse::InvalidateCategoryTab()
{
	FGsCodexManager* codexManager = GsCodex();
	if (nullptr == codexManager)
		return;

	_slotHelperCategoryView->RefreshAll(codexManager->GetTotalCategoryCount());

	// 카테고리 탭 선택
	_currentTabIndex = codexManager->GetIndexWithCategoryId(_currentCategoryId);
	_toggleGroupCategoryView.SetSelectedIndex(_currentTabIndex == INDEX_NONE ? 0 : _currentTabIndex);
}

void UGsUIPopupCodexGemUse::InvalidateMapGroupArea()
{
	FGsCodexManager* codexManager = GsCodex();
	if (nullptr == codexManager)
		return;

	// 맵 이름 세팅
	_textMapGroupName->SetText(codexManager->GetMapGroupNameWithId(_currentMapGroupId));

	// 맵 그룹 이미지 세팅
	InvalidateMapGroupImage();

	// 현재 선택된 맵 그룹의 최대 누적 경험치 계산
	TArray<int32> accumulateExpList = codexManager->GetAccumulateKnowledgeExpListWithMapGroupId(_currentMapGroupId);
	if (accumulateExpList.Num() > 0)
		_maxAccumulateExpForCurrentMapGroup = accumulateExpList.Last();

	// 현재 선택된 맵 그룹의 누적 경험치
	_currentMapGroupExp = codexManager->GetMapGroupCurrentExpWithId(_currentMapGroupId);

	// 아직 지식의 결정 사용 여부를 체크하기 전이므로, 목표 경험치를 초기화
	_targetMapGroupExp = _currentMapGroupExp;
	

	// 만렙을 찍었다면 WidgetSwitcher 로 영역 분기
	InvalidateBottomArea();

	// 사용 가능한 지식의 결정 세팅
	InvalidateUsableItemList();

	// 맵 레벨 업이 가능한지 여부를 체크하여 UI 갱신
	InvalidateMapGroupLevelText();

	// 해당 맵 그룹의 달성 수치 업데이트
	InvalidateKnowledgeLevelList();

	// 적용 버튼 딤드 여부 처리
	InvalidateKnowledgeGemApplyState();

	// 특수 효과 숨김 처리
	_vfxGemUseLevelSame->SetVisibility(ESlateVisibility::Collapsed);
	_vfxGemUseLevelUp->SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUIPopupCodexGemUse::InvalidateMapGroupLevelText()
{
	FGsCodexManager* codexManager = GsCodex();
	if (nullptr == codexManager)
		return;

	int32 currentMapGroupLevel = codexManager->GetMapGroupCurrentLevelWithId(_currentMapGroupId);

	int32 targetMapGroupLevel = currentMapGroupLevel;

	// 만약 레벨업이 가능할 경우, 레벨업 텍스트 색상을 변경한다
	TArray<int32> accumlateExpList = codexManager->GetAccumulateKnowledgeExpListWithMapGroupId(_currentMapGroupId);
	for (int32 i = accumlateExpList.Num() - 1; i >= 0; --i)
	{
		if (_targetMapGroupExp >= accumlateExpList[i])
		{
			targetMapGroupLevel = i + 1;
			break;
		}
	}

	if (targetMapGroupLevel == currentMapGroupLevel)
	{
		_textMapGroupLevel->SetColorAndOpacity(FLinearColor::White);
		_panelCanLevelUp->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		_textMapGroupLevel->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::CODEX_MAPGROUP_LEVEL_UP));
		_panelCanLevelUp->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	_textMapGroupLevel->SetText(FText::Format(_levelText, targetMapGroupLevel));

#ifdef TEST_LOG_CODEX_GEM_USE
	GSLOG(Warning, TEXT("currentMapGroupLevel : %d, targetMapGroupLevel : %d"), currentMapGroupLevel, currentMapGroupLevel);
#endif
}

void UGsUIPopupCodexGemUse::InvalidateMapGroupImage()
{
	FGsCodexManager* codexManager = GsCodex();
	if (nullptr == codexManager)
		return;

	FSoftObjectPath imagePath = codexManager->GetMapGroupImageWithId(_currentMapGroupId);
	if (!imagePath.IsValid())
		return;

	//SetImageSprite(nullptr);
	SetImageTexture(nullptr);
	
	//TWeakObjectPtr<UGsUIPopupCodexGemUse> weakThis(this);
	//FGsUIHelper::AsyncLoadSpriteFromPath(imagePath, FStreamableDelegateParam::CreateWeakLambda(this, [weakThis, imagePath](UObject* InRes)
	//	{
	//		if (weakThis.IsValid())
	//		{
	//			weakThis->SetImageSprite(Cast<UPaperSprite>(InRes));
	//		}
	//	}));

	TWeakObjectPtr<UGsUIPopupCodexGemUse> weakThis(this);
	FGsUIHelper::AsyncLoadTextureFromPath(imagePath, FStreamableDelegateParam::CreateWeakLambda(this, [weakThis, imagePath](UObject* InRes)
		{
			if (weakThis.IsValid())
			{
				weakThis->SetImageTexture(Cast<UTexture2D>(InRes));
			}
		}));
}

void UGsUIPopupCodexGemUse::InvalidateProgressBar(float InDeltaTime /*= 0.f*/)
{
	// [주의]  이 함수를 고치는 것보다, 담배 한 갑을 피는 것이 건강에 덜 해롭습니다
	if (_targetMapGroupExp > _ongoingMapGroupExp)
	{
		_ongoingMapGroupExp = FMath::Clamp(_ongoingMapGroupExp + _rateExpChangeForSecond * InDeltaTime, _ongoingMapGroupExp, (float)_targetMapGroupExp);
		//GSLOG(Warning, TEXT("_targetMapGroupExp : %f, _ongoingMapGroupExp : %f"), _targetMapGroupExp, _ongoingMapGroupExp);
	}
	else /*(_targetMapGroupExp < _ongoingMapGroupExp)*/
	{
		_ongoingMapGroupExp = FMath::Clamp(_ongoingMapGroupExp - _rateExpChangeForSecond * InDeltaTime, (float)_targetMapGroupExp, _ongoingMapGroupExp);
		//GSLOG(Warning, TEXT("_targetMapGroupExp : %f, _ongoingMapGroupExp : %f"), _targetMapGroupExp, _ongoingMapGroupExp);
	}

	FGsCodexManager* codexManager = GsCodex();
	if (nullptr == codexManager)
		return;

	// 예외처리 : 만약 만렙일 경우, Base 프로그레스바를 가득 채우고 Mic Drop
	if (codexManager->GetMapGroupIsMaxLevelWithId(_currentMapGroupId))
	{
		_progressBarExp->SetPercent(1.0f, true);
		_progressBarExp->SetSecondPercent(0.f);

		// 현재 맵 그룹의 맥스 레벨 단계를 찍기 위해 필요한 경험치 (e.g. 9 -> 10 가는데 필요한 경험치
		int32 maxLevelExpForCurrentMapGroup = codexManager->GetKnowledgeRequirementListWithMapGroupId(_currentMapGroupId).Last();
		_textExpComplete->SetText(FText::FromString(FString::Format(TEXT("{0}/{1}"), { maxLevelExpForCurrentMapGroup, maxLevelExpForCurrentMapGroup })));
		return;
	}

	// 현재 경험치에 기반하여 프로그레스바 바로 세팅. knowledgeExpList 는 단계별로 쪼개진 요구 경험치이다.
	TArray<int32> knowledgeExpList = codexManager->GetKnowledgeRequirementListWithMapGroupId(_currentMapGroupId);

	// 현재 맵 그룹의 레벨
	int32 currentMapGroupLevel = codexManager->GetMapGroupCurrentLevelWithId(_currentMapGroupId);

	// 현재 맵 그룹의 레벨에 도달하기 위해 필요했던 경험치.
	int32 accumulateExpForCurrentLevel = codexManager->GetAccumulateKnowledgeExpWithMapGroupIdAndLevel(_currentMapGroupId, currentMapGroupLevel);

	// 현재 레벨 기준의 경험치
	int32 currentExpForCurrentMapGroupLevel = _currentMapGroupExp - accumulateExpForCurrentLevel;

	// 현재 맵 그룹의 누적 경험치 리스트를 조회한다
	TArray<int32> accumlateExpList = codexManager->GetAccumulateKnowledgeExpListWithMapGroupId(_currentMapGroupId);


	// 프로그레스바에서 표시할 수치를 위해 적절한 진행 중 경험치 대비 목표 레벨을 찾자. ongoingLevel는 '실제 레벨' 이다
	int32 ongoingLevel = 0;
	for (int32 i = 0; i < accumlateExpList.Num(); ++i)
	{
		// _ongoingMapGroupExp 는 Tick 을 거치면서 계속 증가하고 있다. _targetMapGroupExp 를 향하여...
		// 프로그레스바에서 표시할 수치는 ongoingExp 를 기준으로 표시를 해 주어야 정확히 표시가 될 것이다
		// 부등호에 등호가 포함되어 있지 않으므로, 해당 레벨에 딱 맞는 경험치를 갖고 있으면 다음 레벨을 바라보게 된다
		if ((int)FMath::RoundHalfFromZero(_ongoingMapGroupExp) < accumlateExpList[i])
		{
			ongoingLevel = i;
			break;
		}
	}

	// 만약 _ongoingMapGroupExp 가 최댓값이라면, ongoingLevel 는 최대 레벨일 것이다
	if (_ongoingMapGroupExp == accumlateExpList.Last())
		ongoingLevel = accumlateExpList.Num();
	
	// 증가 중인 경험치 대비, 프로그레스바에서 표시해줄 단계의 경험치를 세팅한다. knowledgeExpList 에서, 각 항목은,
	// [해당 Index 의 레벨] 에서 [다음 Index 의 레벨] 로 가기 위해 필요한 경험치를 의미한다. 즉, [3] 는 3 -> 4 렙 필요 경험치다
	int32 nextExpForOngoingMapGroupLevel = 0;
	
	// 만렙이면, 최대 수치일 것이다.
	if (ongoingLevel == knowledgeExpList.Num())
	{
		nextExpForOngoingMapGroupLevel = knowledgeExpList.Last();
	}
	// 일반적인 경우 (예를 들어, ongoingLevel 이 0~9 렙인 경우)
	else if (knowledgeExpList.IsValidIndex(ongoingLevel))
	{
		nextExpForOngoingMapGroupLevel = knowledgeExpList[ongoingLevel];
	}
	// 여기로 온다면 뭔가 심각하게 잘못된 것이다
	else
	{
		nextExpForOngoingMapGroupLevel = knowledgeExpList.Last();

		GSLOG(Error, TEXT("Something goes WRONG!!! ongoingLevel : %d, currentExpForCurrentMapGroupLevel : %d, _ongoingMapGroupExp : %f, accumlateExpList.Last() : %d"), ongoingLevel, currentExpForCurrentMapGroupLevel, _ongoingMapGroupExp, accumlateExpList.Last());
	}

	// 현재 레벨의 경험치 단계에 대한 프로그레스바 (빨강) 퍼센트를 세팅하자. 
	float currentMapGroupExpPercent = FMath::Clamp<float>(float(currentExpForCurrentMapGroupLevel) / float(nextExpForOngoingMapGroupLevel), 0.f, 1.0f);


	// Ongoing 경험치가 도달하려는 레벨(ongoingLevel)이 바로 현재 레벨(currentMapGroupLevel)보다 높다면, 현재 Exp (붉은색) 는 표시를 하면 안된다
	if (currentMapGroupLevel < ongoingLevel)
		currentMapGroupExpPercent = 0.0f;

	// 첫번째 프로그레스바의 경우, 만렙이면 꽉 채워주는 것이 자연스럽다
	_progressBarExp->SetPercent(currentMapGroupExpPercent, true);


	// 목표 레벨까지의 누적 Exp 조회. 현재 목표 진행 중 레벨이 4 라면, 4 레벨 도달까지 필요한 누적 경험치를 의미한다
	// 0 레벨은 0 일 것이다. 10 레벨(만렙)은 최대 누적 경험치일 것이다. 어차피 만렙일 때는 퍼센티지를 비워준다 
	int32 prevExpForOngoingMapGroupLevel = 0;
	if (accumlateExpList.IsValidIndex(ongoingLevel - 1))
		prevExpForOngoingMapGroupLevel = accumlateExpList[ongoingLevel - 1];

	// 두번째 프로그레스바에 대한 퍼센티지를 계산한다
	float ongoingMapGroupExpPercent = FMath::Clamp<float>((_ongoingMapGroupExp - prevExpForOngoingMapGroupLevel) / (float)nextExpForOngoingMapGroupLevel, 0.f, 1.f);

	// 두번째 프로그레스바의 경우, 다음 레벨로 올라갈 수 있으면 비워주는 것이 자연스럽다
	_progressBarExp->SetSecondPercent(ongoingMapGroupExpPercent);

	// 프로그레스바 텍스트 값을 세팅한다. 현재 단계의 최댓값을 넘으면 안될 것이다.
	int32 ongoingExpForProgressText = (int)FMath::RoundHalfFromZero(_ongoingMapGroupExp) - prevExpForOngoingMapGroupLevel;
	
	// 만약 만렙 도달의 경우에는, 좌측의 텍스트 값을 0 이 아닌 목표 단계 값으로 채워준다
	if (_ongoingMapGroupExp == accumlateExpList.Last())
		ongoingExpForProgressText = nextExpForOngoingMapGroupLevel;

	_textExpComplete->SetText(FText::FromString(FString::Format(TEXT("{0}/{1}"), { FMath::Min(ongoingExpForProgressText, nextExpForOngoingMapGroupLevel), nextExpForOngoingMapGroupLevel })));

#ifdef TEST_LOG_CODEX_GEM_USE
	GSLOG(Warning, TEXT("currentMapGroupExpPercent : %f, ongoingMapGroupExpPercent : %f"), currentMapGroupExpPercent, ongoingMapGroupExpPercent);
#endif
}

void UGsUIPopupCodexGemUse::InvalidateUsableItemList(int32 InSelectedIndex /*= -1*/)
{
	FGsCodexManager* codexManager = GsCodex();
	if (nullptr == codexManager)
		return;

	UGsItemManager* itemManager = GItem();
	if (nullptr == itemManager)
		return;

	TArray<ItemId> gemList = codexManager->GetKnowledgeGemListWithMapGroupId(_currentMapGroupId);
	
	// 만약 이전에 선택된 녀석이 있고, 갯수가 0 보다 크다면
	if (InSelectedIndex != -1 && itemManager->GetAllItemCountByTID(_knowledgeGemId) > 0)
	{
		// 변한 갯수 정도만 업데이트하고 마무리
		_knowledgeItemCount = 1;
		_knowledgeGemOwnAmount = itemManager->GetAllItemCountByTID(_knowledgeGemId);
	}
	// 선택된 것이 없다면 처음부터 진행
	else
	{
		_knowledgeGemId = INVALID_ITEM_ID;
		_knowledgeItemCount = 0;
		_knowledgeGemOwnAmount = 0;
		_knowledgeGainAmount = 0;

		// 선택한 아이콘 없도록 초기화
		_selectedKnowledgeGemIndex = -1;

		for (int32 i = gemList.Num() - 1; i >= 0; --i)
		{
			ItemId gemId = gemList[i];
			_knowledgeGemOwnAmount = itemManager->GetAllItemCountByTID(gemId);

			if (_knowledgeGemOwnAmount > 0)
			{
				_knowledgeGainAmount = codexManager->GetKnowledgeGainAmountWithItemId(gemId);
				_knowledgeGemId = gemId;
				_knowledgeItemCount = 1;
				_selectedKnowledgeGemIndex = i;
			}
		}
	}

	// 지식 결정 아이템 업데이트
	_slotHelperIcon->RefreshAll(gemList.Num());

	// 최대 사용 가능 결정 갯수 및 소각 여부 계산
	CalculateMaxUsableGemCountAndBurn();

	// 선택된 지식의 결정에 맞게 목표 경험치 업데이트
	CalculateExp(true, TIME_ELAPSED_PROGRESS_BAR_SHORT);

	// 사용 가능한 결정 갯수 세팅
	InvalidateGemCount();

#ifdef TEST_LOG_CODEX_GEM_USE
	GSLOG(Warning, TEXT("_knowledgeGemId : %d, _knowledgeItemCount : %d, _knowledgeGemOwnAmount : %d, _knowledgeGainAmount : %d, _knowledgeGainAmountBurn : %d"), _knowledgeGemId, _knowledgeItemCount, _knowledgeGemOwnAmount, _knowledgeGainAmount, _knowledgeGainAmountBurn);

	GSLOG(Warning, TEXT("_currentMapGroupExp : %f, _targetMapGroupExp : %f, _ongoingMapGroupExp : %f"), _currentMapGroupExp, _targetMapGroupExp, _ongoingMapGroupExp);
#endif
}

void UGsUIPopupCodexGemUse::InvalidateGemCount()
{
	_textCurrentAmount->SetText(FText::FromString(FString::FromInt(_knowledgeItemCount)));

	// 만약 갖고 있는 지식의 결정이 하나도 없다면, 버튼 가리기
	if (_knowledgeItemCount == 0)
		_expCompletedHideUI->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	else
		_expCompletedHideUI->SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUIPopupCodexGemUse::InvalidateKnowledgeLevelList()
{
	FGsCodexManager* codexManager = GsCodex();
	if (nullptr == codexManager)
		return;

	TArray<int32> requirementList = codexManager->GetKnowledgeAccumlatedRequirementListWithMapGroupId(_currentMapGroupId);
	int32 currentMapGroupLevel = codexManager->GetMapGroupCurrentLevelWithId(_currentMapGroupId);

	TArray<UGsUICodexGemRequirementListItem*> itemList;
	for (int32 i = 0; i < requirementList.Num(); ++i)
	{
		UGsUICodexGemRequirementListItem* item = NewObject<UGsUICodexGemRequirementListItem>(this);
		bool isCompleted = currentMapGroupLevel >= i + 1;
		item->SetData(FText::FromString(FString::FromInt(i + 1)), FText::FromString(FString::FromInt(requirementList[i])), isCompleted);

		itemList.Emplace(MoveTemp(item));
	}

	_listViewCodexGemRequirement->SetListItems(itemList);
	_listViewCodexGemRequirement->SetScrollbarVisibility(ESlateVisibility::Collapsed);
	_listViewCodexGemRequirement->ScrollToTop();
}

void UGsUIPopupCodexGemUse::InvalidateKnowledgeGemApplyState()
{
	FGsCodexManager* codexManager = GsCodex();
	if (nullptr == codexManager)
		return;

	// 적용 버튼 딤드 처리할지를 체크
	bool canApply = true;
	
	// 1. 현재 맵 그룹이 최대 레벨이 아니고
	canApply &= !codexManager->GetMapGroupIsMaxLevelWithId(_currentMapGroupId);
	
	// 2. 사용 가능 지식의 결정이 존재해야
	canApply &= _knowledgeItemCount > 0;
	
	// 적용이 가능
	_btnApply->SetIsEnabled(canApply);
}

void UGsUIPopupCodexGemUse::InvalidateBottomArea()
{
	FGsCodexManager* codexManager = GsCodex();
	if (nullptr == codexManager)
		return;

	bool isMaxLevel = codexManager->GetMapGroupIsMaxLevelWithId(_currentMapGroupId);
	_widgetSwitcherBottomArea->SetActiveWidgetIndex(isMaxLevel);
}

void UGsUIPopupCodexGemUse::CalculateMaxUsableGemCountAndBurn()
{
	// 지식의 결정의 획득 수치 중 일부가 소각되더라도, 사용할 수 있는 최대 갯수를 산정한다 (사용할 수 있는 것이 있으면)
	if (_knowledgeItemCount > 0)
	{
		int32 maxUsableGemWithRemainingGainAmount = (_maxAccumulateExpForCurrentMapGroup - _currentMapGroupExp) / _knowledgeGainAmount;

		// 만약 남는 것이 있다면... 하나 더해주고, 소각 되는 것에 더해준다
		bool existBurn = (_maxAccumulateExpForCurrentMapGroup - _currentMapGroupExp) % _knowledgeGainAmount > 0;
		maxUsableGemWithRemainingGainAmount += (existBurn ? 1 : 0);

		_maxUsableKnowledgeGem = FMath::Max(maxUsableGemWithRemainingGainAmount, 1);

		// 지식결정 사용시, 최대 레벨까지 찍을려고 시도할때 소각되는 잉여 지식량
		if (existBurn && _maxUsableKnowledgeGem == _knowledgeItemCount)
			_knowledgeGainAmountBurn = _knowledgeGainAmount - ((_maxAccumulateExpForCurrentMapGroup - _currentMapGroupExp) % _knowledgeGainAmount);
		else
			_knowledgeGainAmountBurn = 0;
	}
	// 사용 가능한 결정이 없으면, 기본값을 세팅해 준다
	else
	{
		_maxUsableKnowledgeGem = 0;

		// 지식결정 미사용시, 소각되는 잉여 지식량은 0 일 것임
		_knowledgeGainAmountBurn = 0;
	}
}

void UGsUIPopupCodexGemUse::CalculateExp(bool InIsForce /*= true*/, float InTimeElapsed /*= 0.1f*/)
{
	// 목표 지식 경험치 수치 계산
	_targetMapGroupExp = FMath::Min(_maxAccumulateExpForCurrentMapGroup, _currentMapGroupExp + _knowledgeGainAmount * _knowledgeItemCount);

	// 지식 획득 혹은 맵 그룹 변경 시, _ongoingMapGroupExp 은 목표치로 바로 설정해 준다
	if (InIsForce)
		_ongoingMapGroupExp = _currentMapGroupExp;

	// ongoingMapGroupExp 가 변하면, 반드시 아래 값도 설정해야 함
	_rateExpChangeForSecond = static_cast<float>(FMath::Abs(_targetMapGroupExp - _ongoingMapGroupExp)) / InTimeElapsed;
}

void UGsUIPopupCodexGemUse::OnGainKnowledge(const IGsMessageParam* InParam /*= nullptr*/)
{
	FGsCodexManager* codexManager = GsCodex();
	if (nullptr == codexManager)
		return;

	// 현재 선택된 맵 그룹의 누적 경험치 다시 조회
	_currentMapGroupExp = codexManager->GetMapGroupCurrentExpWithId(_currentMapGroupId);

	// 아직 지식의 결정 사용 여부를 체크하기 전이므로, 목표 경험치를 초기화
	_targetMapGroupExp = _currentMapGroupExp;


	// 만렙을 찍었다면 WidgetSwitcher 로 영역 분기
	InvalidateBottomArea();

	// 지식 결정 아이템 업데이트
	InvalidateUsableItemList(_selectedKnowledgeGemIndex);

	// 프로그레스바도 한번 업뎃해 주어야 함 (현재 경험치 때문)
	InvalidateProgressBar();

	// 맵 레벨 업이 가능한지 여부를 체크하여 UI 갱신
	InvalidateMapGroupLevelText();

	// 해당 맵 그룹의 달성 수치 업데이트
	InvalidateKnowledgeLevelList();

	// 적용 버튼 딤드 여부 처리
	InvalidateKnowledgeGemApplyState();
}

void UGsUIPopupCodexGemUse::OnGainKnowledgeGem(const IGsMessageParam* InParam /*= nullptr*/)
{
	OnGainKnowledge(InParam);

	// 지식의 결정 사용의 경우 뭔가 추가 처리가 있었으나, 변경됨
}

void UGsUIPopupCodexGemUse::PlayGemUseEffect()
{
	FGsCodexManager* codexManager = GsCodex();
	if (nullptr == codexManager)
		return;

	int32 currentMapGroupLevel = codexManager->GetMapGroupCurrentLevelWithId(_currentMapGroupId);

	int32 targetMapGroupLevel = currentMapGroupLevel;

	// 만약 레벨업이 가능할 경우, 레벨업 텍스트 색상을 변경한다
	TArray<int32> accumlateExpList = codexManager->GetAccumulateKnowledgeExpListWithMapGroupId(_currentMapGroupId);
	for (int32 i = accumlateExpList.Num() - 1; i >= 0; --i)
	{
		if (_targetMapGroupExp >= accumlateExpList[i])
		{
			targetMapGroupLevel = i + 1;
			break;
		}
	}

	if (targetMapGroupLevel == currentMapGroupLevel)
	{
		// 레벨업이 아니면, 테두리가 반짝이는 연출
		_vfxGemUseLevelSame->ShowVFX();

		FGsUIHelper::Play2DSound(GEM_SOUND_USE_GEM);
	}
	else
	{
		// 레벨업이면, 푸른색 파티클이 퍼지는 연출
		_vfxGemUseLevelUp->ShowVFX();

		FGsUIHelper::Play2DSound(GEM_SOUND_USE_LEVEL_UP);
	}
}

TSharedRef<FGsCostPackage> UGsUIPopupCodexGemUse::MakeCostPackage()
{
	// 현재 _costPackage 에는 아이템만 담기므로, 재화까지 포함한 비용 체크해야 함
	TSharedRef<FGsCostPackage> costPackage = MakeShared<FGsCostPackage>(ECachedValueItemDelegateType::CONSUMABLE_INVEN_DEPOT);
	
	// 재료 후보들을 합산해서 계산할 것인지
	costPackage->SetJointCostCandidateElements(true);

	costPackage->AddCost(_knowledgeGemId, _knowledgeItemCount);

	return MoveTemp(costPackage);
}

bool UGsUIPopupCodexGemUse::IsItemGem(TArray<ItemDBId> InItemDBIdList)
{
	FGsCodexManager* codexManager = GsCodex();
	if (nullptr == codexManager)
		return false;

	UGsItemManager* itemManager = GItem();
	if (nullptr == itemManager)
		return false;

	TArray<ItemId> gemList = codexManager->GetKnowledgeGemListWithMapGroupId(_currentMapGroupId);

	for (const auto DBId : InItemDBIdList)
	{
		ItemId tId = itemManager->GetItemTIdByDBId(DBId);
		if (gemList.Contains(tId))
			return true;
	}

	return false;
}

bool UGsUIPopupCodexGemUse::IsItemGem(ItemDBId InItemDBId)
{
	FGsCodexManager* codexManager = GsCodex();
	if (nullptr == codexManager)
		return false;

	UGsItemManager* itemManager = GItem();
	if (nullptr == itemManager)
		return false;

	TArray<ItemId> gemList = codexManager->GetKnowledgeGemListWithMapGroupId(_currentMapGroupId);

	ItemId tId = itemManager->GetItemTIdByDBId(InItemDBId);
	if (gemList.Contains(tId))
		return true;

	return false;
}

void UGsUIPopupCodexGemUse::SetImageSprite(UPaperSprite* InSprite)
{
	if (InSprite)
	{
		_imageMapGroup->SetBrushFromAtlasInterface(InSprite);
		_imageMapGroup->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		_imageMapGroup->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUIPopupCodexGemUse::SetImageTexture(UTexture2D* InTexture)
{
	if (InTexture)
	{
		_imageMapGroup->SetBrushFromTexture(InTexture);
		_imageMapGroup->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		_imageMapGroup->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUIPopupCodexGemUse::SetParameters(Parameters InParameters)
{
	if (InParameters._currentCategoryId == INVALID_CODEX_CATEGORY_ID)
	{
		GSLOG(Error, TEXT("InParameters._currentCategoryId == INVALID_CODEX_CATEGORY_ID"));
		return;
	}

	if (InParameters._currentMapGroupId == INVALID_CODEX_MAP_GROUP_ID)
	{
		GSLOG(Error, TEXT("InParameters._currentMapGroupId == INVALID_CODEX_MAP_GROUP_ID"));
		return;
	}

	_currentCategoryId = InParameters._currentCategoryId;
	InvalidateCategoryTab();

	_currentMapGroupId = InParameters._currentMapGroupId;
	InvalidateMapGroupArea();
}

void UGsUIPopupCodexGemUse::OnRefreshEntryCategoryView(int32 InIndex, UWidget* InEntry)
{
	UGsUITabButtonCommon* tabButtonCommon = Cast<UGsUITabButtonCommon>(InEntry);
	if (nullptr == tabButtonCommon)
	{
		GSLOG(Error, TEXT("nullptr == tabButtonCommon, *InEntry->GetName():%s"), *InEntry->GetName());
		return;
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
		_toggleGroupCategoryView.AddToggleWidget(tabButtonCommon->GetToggleGroupEntry());
	}

	FGsCodexManager* codexManager = GsCodex();
	if (nullptr == codexManager)
		return;

	CodexCategoryId categoryId = codexManager->GetCategoryIdWithIndex(InIndex);
	tabButtonCommon->SetTitleText(codexManager->GetCategoryNameWithId(categoryId));
	tabButtonCommon->SetIsEnabled(true);
}

void UGsUIPopupCodexGemUse::OnSelectChangedCategoryView(int32 InIndex)
{
	FGsCodexManager* codexManager = GsCodex();
	if (nullptr == codexManager)
		return;

	_currentTabIndex = _toggleGroupCategoryView.GetSelectedIndex();
	_currentCategoryId = codexManager->GetCategoryIdWithIndex(_currentTabIndex);

	TArray<CodexMapGroupId> mapGroupList = codexManager->GetMapGroupListWithId(_currentCategoryId);
	if (mapGroupList.IsValidIndex(0))
		_currentMapGroupId = mapGroupList[0];

	InvalidateMapGroupArea();
}

void UGsUIPopupCodexGemUse::OnRefreshGemIconItem(int32 InIndex, UGsUIIconBase* InIcon)
{
	FGsCodexManager* codexManager = GsCodex();
	if (nullptr == codexManager)
		return;

	UGsItemManager* itemManager = GItem();
	if (nullptr == itemManager)
		return;

	TArray<ItemId> gemList = codexManager->GetKnowledgeGemListWithMapGroupId(_currentMapGroupId);
	if (!gemList.IsValidIndex(InIndex))
		return;

	ItemId gemId = gemList[InIndex];

	UGsUIIconItemInventory* icon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == icon)
	{
		GSLOG(Error, TEXT("nullptr == icon, InIndex:%d, GetName():%s"), InIndex, *GetName());
		return;
	}

	int32 itemAmount = itemManager->GetAllItemCountByTID(gemId);
	icon->SetNotOwnItem(gemId, itemAmount);
	icon->SetDimmedImage(itemAmount == 0);

	if (!icon->OnFinishedLongPressEvent.IsBoundToObject(this))
	{
		icon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIPopupCodexGemUse::OnFinishedLongPressEventItemIcon);
	}

	if (!icon->OnClickSlot.IsBoundToObject(this))
	{
		icon->OnClickSlot.AddUObject(this, &UGsUIPopupCodexGemUse::OnClickedItemIcon);
	}

	icon->SetItemNameVisibility(false);
	icon->SetRedDot(false);
	icon->SetSlotSelectionImage(InIndex == _selectedKnowledgeGemIndex);

#ifdef TEST_LOG_CODEX_GEM_USE
	GSLOG(Warning, TEXT("_knowledgeGemId : %d, _knowledgeItemCount : %d, _knowledgeGemOwnAmount : %d, _knowledgeGainAmount : %d, _knowledgeGainAmountBurn : %d"), _knowledgeGemId, _knowledgeItemCount, _knowledgeGemOwnAmount, _knowledgeGainAmount, _knowledgeGainAmountBurn);

	GSLOG(Warning, TEXT("_currentMapGroupExp : %f, _targetMapGroupExp : %f, _ongoingMapGroupExp : %f"), _currentMapGroupExp, _targetMapGroupExp, _ongoingMapGroupExp);
#endif
}

void UGsUIPopupCodexGemUse::OnClickedLeftMapGroup()
{
	FGsCodexManager* codexManager = GsCodex();
	if (nullptr == codexManager)
		return;

	TArray<CodexMapGroupId> mapGroupList = codexManager->GetMapGroupListWithId(_currentCategoryId);

	// 맵그룹이 1개밖에 없으면 UI 갱신 X
	if (mapGroupList.Num() == 1)
		return;

	int32 currentIndex = -1;
	for (int32 i = 0; i < mapGroupList.Num(); ++i)
	{
		if (_currentMapGroupId == mapGroupList[i])
		{
			currentIndex = i;
			break;
		}
	}

	currentIndex = FMath::Clamp((currentIndex - 1 + mapGroupList.Num()) % mapGroupList.Num(), 0, mapGroupList.Num() - 1);
	_currentMapGroupId = mapGroupList[currentIndex];

	InvalidateMapGroupArea();
}

void UGsUIPopupCodexGemUse::OnClickedRightMapGroup()
{
	FGsCodexManager* codexManager = GsCodex();
	if (nullptr == codexManager)
		return;

	TArray<CodexMapGroupId> mapGroupList = codexManager->GetMapGroupListWithId(_currentCategoryId);

	// 맵그룹이 1개밖에 없으면 UI 갱신 X
	if (mapGroupList.Num() == 1)
		return;

	int32 currentIndex = -1;
	for (int32 i = 0; i < mapGroupList.Num(); ++i)
	{
		if (_currentMapGroupId == mapGroupList[i])
		{
			currentIndex = i;
			break;
		}
	}

	currentIndex = FMath::Clamp((currentIndex + 1) % mapGroupList.Num(), 0, mapGroupList.Num() - 1);
	_currentMapGroupId = mapGroupList[currentIndex];

	InvalidateMapGroupArea();
}

void UGsUIPopupCodexGemUse::OnClickedCalcReset()
{
	// 감소 처리 불가능 체크
	if (_knowledgeItemCount <= 1)
		return;

	_knowledgeItemCount = 1;

	InvalidateGemCount();
	
	CalculateExp(false, TIME_ELAPSED_PROGRESS_BAR_LONG);

	_rateExpChangeForSecond = FMath::Max(BASE_RATE_EXP_CHANGE_FOR_SECOND, _rateExpChangeForSecond);

	_effectAmountChanged->ShowVFX();

	// 최대 사용 가능 결정 갯수 및 소각 여부 계산
	CalculateMaxUsableGemCountAndBurn();

	// 맵 레벨 업이 가능한지 여부를 체크하여 UI 갱신
	InvalidateMapGroupLevelText();
}

void UGsUIPopupCodexGemUse::OnClickedDecreaseTen()
{
	// 감소 처리 불가능 체크
	if (_knowledgeItemCount <= 1)
		return;

	_knowledgeItemCount = FMath::Clamp(_knowledgeItemCount - 10, 1, _knowledgeItemCount);

	InvalidateGemCount();

	CalculateExp(false, TIME_ELAPSED_PROGRESS_BAR_LONG);
	_rateExpChangeForSecond = FMath::Max(BASE_RATE_EXP_CHANGE_FOR_SECOND, _rateExpChangeForSecond);

	_effectAmountChanged->ShowVFX();

	// 최대 사용 가능 결정 갯수 및 소각 여부 계산
	CalculateMaxUsableGemCountAndBurn();

	// 맵 레벨 업이 가능한지 여부를 체크하여 UI 갱신
	InvalidateMapGroupLevelText();

	//FGsUIHelper::Play2DSound(SOUND_GAUGE_CHANGE);
}

void UGsUIPopupCodexGemUse::OnClickedDecrease()
{
	// 감소 처리 불가능 체크
	if (_knowledgeItemCount <= 1)
		return;

	_knowledgeItemCount = FMath::Clamp(_knowledgeItemCount - 1, 1, _knowledgeItemCount);

	InvalidateGemCount();

	CalculateExp(false, TIME_ELAPSED_PROGRESS_BAR_SHORT);

	_effectAmountChanged->ShowVFX();


	// 최대 사용 가능 결정 갯수 및 소각 여부 계산
	CalculateMaxUsableGemCountAndBurn();

	// 맵 레벨 업이 가능한지 여부를 체크하여 UI 갱신
	InvalidateMapGroupLevelText();
}

void UGsUIPopupCodexGemUse::OnClickedIncrease()
{
	UGsItemManager* itemManager = GItem();
	if (nullptr == itemManager)
		return;
	int32 itemAmount = itemManager->GetAllItemCountByTID(_knowledgeGemId);

	// 증가 처리 불가능 체크
	int32 updatedKnowledgeItemCount = FMath::Min(itemAmount, _maxUsableKnowledgeGem);

	if (_knowledgeItemCount == updatedKnowledgeItemCount)
		return;

	_knowledgeItemCount = FMath::Clamp(_knowledgeItemCount + 1, 1, FMath::Min(itemAmount, _maxUsableKnowledgeGem));

	InvalidateGemCount();

	// 헤당 맵 그룹에서 최대로 올릴 수 있는 수치보다 _targetMapGroupExp 가 높으면 안됨
	FGsCodexManager* codexManager = GsCodex();
	if (nullptr == codexManager)
		return;
	
	CalculateExp(false, TIME_ELAPSED_PROGRESS_BAR_SHORT);

	_effectAmountChanged->ShowVFX();

	// 최대 사용 가능 결정 갯수 및 소각 여부 계산
	CalculateMaxUsableGemCountAndBurn();

	// 맵 레벨 업이 가능한지 여부를 체크하여 UI 갱신
	InvalidateMapGroupLevelText();
}

void UGsUIPopupCodexGemUse::OnClickedIncreaseTen()
{
	UGsItemManager* itemManager = GItem();
	if (nullptr == itemManager)
		return;
	int32 itemAmount = itemManager->GetAllItemCountByTID(_knowledgeGemId);

	// 증가 처리 불가능 체크
	int32 updatedKnowledgeItemCount = FMath::Min(itemAmount, _maxUsableKnowledgeGem);

	if (_knowledgeItemCount == updatedKnowledgeItemCount)
		return;

	_knowledgeItemCount = FMath::Clamp(_knowledgeItemCount + 10, 1, FMath::Min(itemAmount, _maxUsableKnowledgeGem));

	InvalidateGemCount();

	// 헤당 맵 그룹에서 최대로 올릴 수 있는 수치보다 _targetMapGroupExp 가 높으면 안됨
	FGsCodexManager* codexManager = GsCodex();
	if (nullptr == codexManager)
		return;

	int32 maxAccumulateExpForCurrentMapGroup = 0;
	TArray<int32> accumulateExpList = codexManager->GetAccumulateKnowledgeExpListWithMapGroupId(_currentMapGroupId);
	if (accumulateExpList.Num() > 0)
		maxAccumulateExpForCurrentMapGroup = accumulateExpList.Last();

	CalculateExp(false, TIME_ELAPSED_PROGRESS_BAR_LONG);
	_rateExpChangeForSecond = FMath::Max(BASE_RATE_EXP_CHANGE_FOR_SECOND, _rateExpChangeForSecond);

	_effectAmountChanged->ShowVFX();


	// 최대 사용 가능 결정 갯수 및 소각 여부 계산
	CalculateMaxUsableGemCountAndBurn();

	// 맵 레벨 업이 가능한지 여부를 체크하여 UI 갱신
	InvalidateMapGroupLevelText();
}

void UGsUIPopupCodexGemUse::OnClickedCalcMax()
{
	UGsItemManager* itemManager = GItem();
	if (nullptr == itemManager)
		return;
	int32 itemAmount = itemManager->GetAllItemCountByTID(_knowledgeGemId);

	// 증가 처리 불가능 체크
	int32 updatedKnowledgeItemCount = FMath::Min(itemAmount, _maxUsableKnowledgeGem);

	if (_knowledgeItemCount == updatedKnowledgeItemCount)
		return;

	_knowledgeItemCount = FMath::Min(_maxUsableKnowledgeGem, itemAmount);

	InvalidateGemCount();

	// 헤당 맵 그룹에서 최대로 올릴 수 있는 수치보다 _targetMapGroupExp 가 높으면 안됨
	FGsCodexManager* codexManager = GsCodex();
	if (nullptr == codexManager)
		return;

	int32 maxAccumulateExpForCurrentMapGroup = 0;
	TArray<int32> accumulateExpList = codexManager->GetAccumulateKnowledgeExpListWithMapGroupId(_currentMapGroupId);
	if (accumulateExpList.Num() > 0)
		maxAccumulateExpForCurrentMapGroup = accumulateExpList.Last();

	CalculateExp(false, TIME_ELAPSED_PROGRESS_BAR_LONG);
	_rateExpChangeForSecond = FMath::Max(BASE_RATE_EXP_CHANGE_FOR_SECOND, _rateExpChangeForSecond);

	_effectAmountChanged->ShowVFX();


	// 최대 사용 가능 결정 갯수 및 소각 여부 계산
	CalculateMaxUsableGemCountAndBurn();

	// 맵 레벨 업이 가능한지 여부를 체크하여 UI 갱신
	InvalidateMapGroupLevelText();
}

void UGsUIPopupCodexGemUse::OnClickedApply()
{
	UGsItemManager* itemManager = GItem();
	if (nullptr == itemManager)
		return;

	TArray<TWeakPtr<FGsItem>> ownItemList = itemManager->FindByTID(_knowledgeGemId);
	if (ownItemList.Num() == 0)
	{
		GSLOG(Error, TEXT("ownItemList.Num() == 0"));
		return;
	}

	ItemDBId itemDBId = ownItemList[0].Pin()->GetDBID();

	if (_knowledgeGainAmountBurn == 0)
	{
		PlayGemUseEffect();
		
		FGsNetSendServiceWorld::Send_MonsterKnowledgeExpGainItemUse(itemDBId, _knowledgeItemCount, _currentMapGroupId);
	}
	// 소각되는 지식의 결정이 있을 경우, 확인 팝업을 한 번 띄워준다
	else
	{
		UGsUIManager* uiMgr = GUI();
		if (nullptr == uiMgr)
			return;

		TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->OpenAndGetWidget(TEXT("PopupYesNo"));
		if (false == widget.IsValid())
			return;

		UGsUIPopupYesNo* popup = Cast<UGsUIPopupYesNo>(widget.Get());
		if (nullptr == popup)
			return;

		FText warningText = FText::Format(_knowledgeGemBurnText, _knowledgeGainAmountBurn);

		popup->SetData(warningText, [itemDBId, knowledgeItemCount = this->_knowledgeItemCount, currentMapGroupId = this->_currentMapGroupId, this](bool InIsOk)
			{
				if (InIsOk)
				{
					PlayGemUseEffect();

					FGsNetSendServiceWorld::Send_MonsterKnowledgeExpGainItemUse(itemDBId, knowledgeItemCount, currentMapGroupId);
				}
			});
	}
}

void UGsUIPopupCodexGemUse::OnClickedCancel()
{
	if (OnClickedCancelDelegate.IsBound())
		OnClickedCancelDelegate.Execute();

	Close();
}

void UGsUIPopupCodexGemUse::OnFinishedLongPressEventItemIcon(UGsUIIconItem& InIcon)
{
	UGsUIIconItem::DefaultItemDetailOpener(InIcon, true);
}

void UGsUIPopupCodexGemUse::OnClickedItemIcon(UGsUIIconItem& InIcon)
{
	FGsCodexManager* codexManager = GsCodex();
	if (nullptr == codexManager)
		return;

	// 보유하지 않았을 경우, 티커 띄워주고 리턴
	if (0 == InIcon.GetAmount())
	{
		FText ticker;
		FText::FindText(TEXT("CodexUIText"), TEXT("UI_MapList_Ticker_NoItem"), ticker);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, ticker);
		return;
	}

	// 선택 가능하다면, 지식의 결정 관련 정보를 업데이트
	_knowledgeGemId = InIcon.GetTableId();
	_knowledgeItemCount = InIcon.GetAmount() > 0 ? 1 : 0;
	_knowledgeGemOwnAmount = InIcon.GetAmount();
	_knowledgeGainAmount = codexManager->GetKnowledgeGainAmountWithItemId(_knowledgeGemId);

	TArray<ItemId> gemList = codexManager->GetKnowledgeGemListWithMapGroupId(_currentMapGroupId);
	for (int32 i = 0; i < gemList.Num(); ++i)
	{
		if (gemList[i] == _knowledgeGemId)
		{
			_selectedKnowledgeGemIndex = i;
			break;
		}
	}

	// 아이콘 선택 하이라이트 이미지 띄워주기 토글
	for (int32 i = 0; i < gemList.Num(); ++i)
	{
		UGsUIIconBase* iconBase = _slotHelperIcon->GetIcon(i);
		if (nullptr == iconBase)
			return;

		UGsUIIconItemInventory* icon = Cast<UGsUIIconItemInventory>(iconBase);
		if (nullptr == icon)
			return;

		icon->SetSlotSelectionImage(i == _selectedKnowledgeGemIndex);
	}

	InvalidateGemCount();

	CalculateExp(true, TIME_ELAPSED_PROGRESS_BAR_SHORT);

	// 잉여 지식 
	CalculateMaxUsableGemCountAndBurn();

	_effectAmountChanged->ShowVFX();

	InvalidateMapGroupLevelText();
}

void UGsUIPopupCodexGemUse::OnInputCancel()
{
	Close();
}

void UGsUIPopupCodexGemUse::OnPressedBtnTooltip()
{
	if (nullptr == _tooltipCodexGemUse)
		return;

	if (_tooltipCodexGemUse->IsOpen())
		_tooltipCodexGemUse->Close();

	_tooltipCodexGemUse->Open();
}

void UGsUIPopupCodexGemUse::OnAddItemList(FGsItemMessageParamBase& InParam)
{
	FGsAddItemList* Param = StaticCast<FGsAddItemList*>(&InParam);
	if (nullptr == Param)
		return;

	if (!IsItemGem(Param->_itemDbidList))
		return;

	FGsCodexManager* codexManager = GsCodex();
	if (nullptr == codexManager)
		return;

	// 만약 갖고 있는 것이 없었다면, 그냥 편하게 전부 갱신해 줘도 된다
	if (_selectedKnowledgeGemIndex == -1)
	{
		// 사용 가능한 지식의 결정 세팅
		InvalidateUsableItemList();
	}
	// 선택했던 아이템이 있었다면, 아이템 슬롯 헬퍼만 Update 한다
	else
	{
		TArray<ItemId> gemList = codexManager->GetKnowledgeGemListWithMapGroupId(_currentMapGroupId);

		// 지식 결정 아이템 업데이트
		_slotHelperIcon->RefreshAll(gemList.Num());
	}

	// 맵 레벨 업이 가능한지 여부를 체크하여 UI 갱신
	InvalidateMapGroupLevelText();

	// 해당 맵 그룹의 달성 수치 업데이트
	InvalidateKnowledgeLevelList();

	// 적용 버튼 딤드 여부 처리
	InvalidateKnowledgeGemApplyState();
}

void UGsUIPopupCodexGemUse::OnUpdateItem(FGsItemMessageParamBase& InParam)
{
	const FGsUpdateItem* Param = StaticCast<const FGsUpdateItem*>(&InParam);
	if (nullptr == Param)
		return;

	if (!IsItemGem(Param->_itemDBID))
		return;

	FGsCodexManager* codexManager = GsCodex();
	if (nullptr == codexManager)
		return;

	// 만약 갖고 있는 것이 없었다면, 그냥 편하게 전부 갱신해 줘도 된다
	if (_selectedKnowledgeGemIndex == -1)
	{
		// 사용 가능한 지식의 결정 세팅
		InvalidateUsableItemList();
	}
	// 선택했던 아이템이 있었다면, 아이템 슬롯 헬퍼만 Update 한다
	else
	{
		TArray<ItemId> gemList = codexManager->GetKnowledgeGemListWithMapGroupId(_currentMapGroupId);

		// 지식 결정 아이템 업데이트
		_slotHelperIcon->RefreshAll(gemList.Num());
	}

	// 맵 레벨 업이 가능한지 여부를 체크하여 UI 갱신
	InvalidateMapGroupLevelText();

	// 해당 맵 그룹의 달성 수치 업데이트
	InvalidateKnowledgeLevelList();

	// 적용 버튼 딤드 여부 처리
	InvalidateKnowledgeGemApplyState();
}

void UGsUIPopupCodexGemUse::OnRemoveItem(FGsItemMessageParamBase& InParam)
{
	const FGsRemoveItem* Param = StaticCast<const FGsRemoveItem*>(&InParam);
	if (nullptr == Param)
		return;

	if (!IsItemGem(Param->_itemDBID))
		return;

	FGsCodexManager* codexManager = GsCodex();
	if (nullptr == codexManager)
		return;

	TArray<ItemId> gemList = codexManager->GetKnowledgeGemListWithMapGroupId(_currentMapGroupId);

	// 지식 결정 아이템 업데이트
	_slotHelperIcon->RefreshAll(gemList.Num());

	// 만약 선택되었던 녀석을 전부 소진했다면... 선택될 수 있는 녀석이 있는지 탐색을 처음부터 다시 시도한다
	{
		UGsItemManager* itemManager = GItem();
		if (nullptr == itemManager)
			return;

		UGsUIIconBase* iconBase = _slotHelperIcon->GetIcon(_selectedKnowledgeGemIndex);
		if (nullptr == iconBase)
			return;

		UGsUIIconItemInventory* icon = Cast<UGsUIIconItemInventory>(iconBase);
		if (nullptr == icon)
			return;

		int32 itemAmount = itemManager->GetAllItemCountByTID(icon->GetTableId());
		if (itemAmount <= 0)
		{
			InvalidateUsableItemList();
		}
	}

	// 맵 레벨 업이 가능한지 여부를 체크하여 UI 갱신
	InvalidateMapGroupLevelText();

	// 해당 맵 그룹의 달성 수치 업데이트
	InvalidateKnowledgeLevelList();

	// 적용 버튼 딤드 여부 처리
	InvalidateKnowledgeGemApplyState();
}
