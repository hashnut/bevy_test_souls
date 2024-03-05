// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIWindowMultiLevelRank.h"

#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Image.h"

#include "Classes/PaperSprite.h"

#include "MultiLevelRank/GsMultiLevelRankData.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsMultiLevelRankManager.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"

#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"

#include "UI/UIContent/Window/MultiLevelRank/GsUIMultiLevelRankSlot.h"

#include "UI/UIContent/Popup/GsUIPopupMultiLevelRankStat.h"
#include "UI/UIContent/Popup/GsUIPopupMultiLevelRankPledgeSelect.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Management/GsMessageHolder.h"

#include "T1Project.h"


void UGsUIWindowMultiLevelRank::BeginDestroy()
{
	if (nullptr != _slotHelperRank)
	{
		_slotHelperRank->OnRefreshEntry.RemoveDynamic(this, &UGsUIWindowMultiLevelRank::OnInitializedRankEntry);
	}
	_slotHelperRank = nullptr;

	if (nullptr != _btnBack)
	{
		_btnBack->OnClicked.RemoveDynamic(this, &UGsUIWindowMultiLevelRank::OnClickBack);
	}
	_btnBack = nullptr;

	if (nullptr != _btnClose)
	{
		_btnClose->OnClicked.RemoveDynamic(this, &UGsUIWindowMultiLevelRank::OnClickCloseAllStack);
	}
	_btnClose = nullptr;

	if (nullptr != _btnPledgeChange)
	{
		_btnPledgeChange->OnClicked.RemoveDynamic(this, &UGsUIWindowMultiLevelRank::OnClickedPledgeChange);
	}
	_btnPledgeChange = nullptr;

	if (nullptr != _btnAcquiredStat)
	{
		_btnAcquiredStat->OnClicked.RemoveDynamic(this, &UGsUIWindowMultiLevelRank::OnClickedAcquiredStat);
	}
	_btnAcquiredStat = nullptr;

	if (nullptr != _btnInfo)
	{
		_btnInfo->OnClicked.RemoveDynamic(this, &UGsUIWindowMultiLevelRank::OnClickedInfo);
	}
	_btnInfo = nullptr;

	Super::BeginDestroy();
}

void UGsUIWindowMultiLevelRank::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	//_helpContentsKey = EGsHelpContents::Ranking;

	_slotHelperRank = NewObject<UGsDynamicPanelSlotHelper>(this);
	if (nullptr != _slotHelperRank)
	{
		_slotHelperRank->Initialize(_subClassOfRankEntry, _mainScrollBox);
		_slotHelperRank->OnRefreshEntry.AddDynamic(this, &UGsUIWindowMultiLevelRank::OnInitializedRankEntry);
	}

	_btnBack->OnClicked.AddDynamic(this, &UGsUIWindowMultiLevelRank::OnClickBack);
	_btnClose->OnClicked.AddDynamic(this, &UGsUIWindowMultiLevelRank::OnClickCloseAllStack);
	_btnPledgeChange->OnClicked.AddDynamic(this, &UGsUIWindowMultiLevelRank::OnClickedPledgeChange);
	_btnAcquiredStat->OnClicked.AddDynamic(this, &UGsUIWindowMultiLevelRank::OnClickedAcquiredStat);
	_btnInfo->OnClicked.AddDynamic(this, &UGsUIWindowMultiLevelRank::OnClickedInfo);

	_mainScrollBox->OnUserScrolled.AddDynamic(this, &UGsUIWindowMultiLevelRank::OnScrollMain);
}

void UGsUIWindowMultiLevelRank::NativeConstruct()
{
	Super::NativeConstruct();

	InitializeSlotAll();
}

void UGsUIWindowMultiLevelRank::NativeDestruct()
{
	SetIsSelected(_focusSlotWidget, false);
	_focusSlotWidget = nullptr;
	_slotRankList.Empty();

	Super::NativeDestruct();
}

bool UGsUIWindowMultiLevelRank::SyncContentState()
{
	if (false == IsSameContentState(FGsContentsMode::InGame::ContentsMultiLevelRank))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_MULTI_LEVEL_RANK);
		return true;
	}

	return false;
}

void UGsUIWindowMultiLevelRank::OnInitializedRankEntry(int32 InIndex, UWidget* InEntry)
{
	TWeakObjectPtr<UGsUIMultiLevelRankSlot> slot = Cast<UGsUIMultiLevelRankSlot>(InEntry);
	if (slot.IsValid())
	{
		int32 rank = _slotRankList[InIndex];
		bool isPledgeSlot = (rank == 0);

		if (isPledgeSlot)
		{
			rank = _slotRankList[InIndex - 1] + 1;
		}

		const FGsMultiLevelRankData* rankData = GSMultiLevelRank()->GetRankById(rank);
		if (nullptr == rankData)
			return;

		// 데이터 설정 후 경험치 바 갱신
		if (isPledgeSlot)
		{
			slot->InitializePledgeSlot(rankData);
		}
		else
		{
			slot->OnInitialized(rankData);
		}

		if (false == slot->_onClickRankSlot.IsBoundToObject(this))
		{
			slot->_onClickRankSlot.BindUObject(this, &UGsUIWindowMultiLevelRank::OnClickedRankSlot);
		}
	}
}

void UGsUIWindowMultiLevelRank::InitializeEntryAll()
{
	FGsMultiLevelRankManager* manager = GSMultiLevelRank();
	if (nullptr == manager)
		return;

	if (nullptr == _slotHelperRank)
		return;


	// (서약 슬롯 or 선택한 서약 끝)까지 슬롯 생성 및 랭크 데이터 설정
	TArray<FGsMultiLevelRankData> rankDataList = manager->GetRankDataList();
	_slotRankList.Empty();
	for (const FGsMultiLevelRankData& rankData : rankDataList)
	{
		// 일반 슬롯 정보 저장
		_slotRankList.Emplace(rankData._rankSchema->rank);

		// 서약 슬롯 필요 확인 
		if (rankData._rankSchema->linkType == ERankLinkType::SingleToMulti)
		{
			// 서약 슬롯 : 0
			_slotRankList.Emplace(0);

			// 서약 슬롯 정보 저장 후 서약이 선택되지 않았다면 이후 정보 비공개
			if (false == manager->IsPledgeSelected())
			{
				break;
			}
		}
	}

	_slotHelperRank->RefreshAll(_slotRankList.Num());

	if (IsTutorial())
	{
		CheckTutorialTargetList();		
	}
}

void UGsUIWindowMultiLevelRank::InitializeFocus()
{
	FGsMultiLevelRankManager* manager = GSMultiLevelRank();
	if (nullptr == manager)
		return;

	/**
	 * 진입 규칙에 맞게 포커스 설정
	 * (포커스된 슬롯은 화면 가운데로 이동)
	 * 1. 퀘스트가 진행 중인 슬롯
	 * 2. 활성화 되어 있는 슬롯
	 * 3. nullptr( 제일 처음 슬롯이 타겟으로 됨 )
	 */

	SetIsSelected(_focusSlotWidget, false);
	_focusSlotWidget = nullptr;

	const FGsMultiLevelRankSlotData* targetSlot = manager->GetQuestInProgressSlot();
	if (nullptr == targetSlot)
	{
		targetSlot = manager->GetActiveLastSlot();
	}
	SetFocusToForce(targetSlot);
}

void UGsUIWindowMultiLevelRank::OnClickedRankSlot(UUserWidget* InWidget)
{
	// focus swap
	TWeakObjectPtr<UUserWidget> prevWidget = _focusSlotWidget.Get();
	_focusSlotWidget = InWidget;

	// focus release / selected
	SetIsSelected(prevWidget, false);
	SetIsSelected(_focusSlotWidget, true);

	ForceAutoScroll(InWidget);
}

void UGsUIWindowMultiLevelRank::OnClickedPledgeChange()
{
	if (GSMultiLevelRank()->IsPledgeSelected())
	{
		GUI()->Open(TEXT("PopupMultiLevelRankPledgeSelect"));
	}
	else
	{
		// 눌리지 않아야 정상
		InvalidateSlotAll();
	}
}

void UGsUIWindowMultiLevelRank::OnClickedAcquiredStat()
{
	GUI()->Open(TEXT("PopupMultiLevelRankStat"));
}

void UGsUIWindowMultiLevelRank::OnClickedInfo()
{
	InitializeFocus();
}

void UGsUIWindowMultiLevelRank::ChangeFocusedSlot(MultiLevelRank InRank)
{
	int32 findIndex = _slotRankList.Find(InRank);
	TWeakObjectPtr<UGsUIMultiLevelRankSlot> slot = Cast<UGsUIMultiLevelRankSlot>(_slotHelperRank->GetEntry(findIndex));
	if (slot.IsValid())
	{
		OnClickedRankSlot(slot.Get());
	}
}

void UGsUIWindowMultiLevelRank::ForceAutoScroll(TWeakObjectPtr<UWidget> InWidget)
{
	if (InWidget.IsValid())
	{
		_mainScrollBox->ScrollWidgetIntoView(InWidget.Get(), true, EDescendantScrollDestination::Center);
	}
	else
	{
		_mainScrollBox->ScrollToStart();
	}
	_mainScrollBox->EndInertialScrolling();
}

void UGsUIWindowMultiLevelRank::SetIsSelected(TWeakObjectPtr<UWidget> InWidget, bool IsSelected)
{
	TWeakObjectPtr<UGsUIMultiLevelRankSlot> slot = Cast<UGsUIMultiLevelRankSlot>(InWidget.Get());
	if (slot.IsValid())
	{
		slot->SetIsSelected(IsSelected);
	}
}

void UGsUIWindowMultiLevelRank::SetFocusToForce(const FGsMultiLevelRankSlotData* InFocusData)
{
	int32 findIndex = INDEX_NONE;
	if (nullptr != InFocusData)
	{	// 포커스 데이타를 갖고 있는 Widget Index를 찾기.
		findIndex = _slotRankList.Find(InFocusData->_ownerRank->_rankSchema->rank);
	}
	else
	{	// 포커스가 없다면 가장 처음 슬롯에 포커스를 둔다.
		findIndex = 0;
	}

	TWeakObjectPtr<UGsUIMultiLevelRankSlot> slot = Cast<UGsUIMultiLevelRankSlot>(_slotHelperRank->GetEntry(findIndex));
	if (slot.IsValid())
	{
		OnClickedRankSlot(slot.Get());
	}
}

void UGsUIWindowMultiLevelRank::InitializeSlotAll()
{
	InitializeEntryAll();
	InitializeFocus();
	InvalidateInfo();
}

void UGsUIWindowMultiLevelRank::InvalidateSlotAll()
{
	int32 maxCount = _slotHelperRank->GetItemCount();
	for (int idx = 0; idx < maxCount; ++idx)
	{
		TWeakObjectPtr<UGsUIMultiLevelRankSlot> slot = Cast<UGsUIMultiLevelRankSlot>(_slotHelperRank->GetEntry(idx));
		if (slot.IsValid())
		{
			slot->InvalidateSlotAll();
		}
	}

	InvalidateInfo();
}

void UGsUIWindowMultiLevelRank::InvalidateInfo()
{
	FGsMultiLevelRankManager* rankManager = GSMultiLevelRank();
	if (nullptr == rankManager)
		return;

	enum InfoState { START = 0, IN_PROGRESS, PLEDGE, ACTIVE };

	FText infoText;
	FText::FindText(TEXT("MultiLevelRank"), TEXT("UI_InfoText_None"), infoText);
	_switcherInfoImage->SetActiveWidgetIndex(InfoState::START); // 승급 임무 시작 가능의 아이콘

	// 서약을 선택할 수 있는가?
	if (rankManager->CanSelectPledge())
	{
		_switcherInfoImage->SetActiveWidgetIndex(InfoState::PLEDGE);
		FText::FindText(TEXT("MultiLevelRank"), TEXT("UI_InfoText_NeedPledge"), infoText);
	}
	// 임무 수행중인가?
	else if (nullptr != rankManager->GetQuestInProgressSlot())
	{
		_switcherInfoImage->SetActiveWidgetIndex(InfoState::IN_PROGRESS);
		FText::FindText(TEXT("MultiLevelRank"), TEXT("UI_InfoText_InProgress"), infoText);
		infoText = FText::Format(infoText, rankManager->GetQuestInProgressSlot()->_slotSchema->NameText);
	}
	// 달성한 랭크가 있는가?
	else if (nullptr != rankManager->GetActiveLastSlot())
	{
		const FGsMultiLevelRankSlotData* slot = rankManager->GetActiveLastSlot();
		int32 nextRank = slot->_ownerRank->_rankSchema->rank + 1;
		int32 nextIndex = _slotRankList.Find(nextRank);
		if (nextIndex != INDEX_NONE)
		{
			TWeakObjectPtr<UGsUIMultiLevelRankSlot> nextSlotWidget = Cast<UGsUIMultiLevelRankSlot>(_slotHelperRank->GetEntry(nextIndex));
			if (nextSlotWidget.IsValid())
			{
				const FGsMultiLevelRankSlotData* nextSlot = nextSlotWidget->GetData();
				if (nextSlot && nextSlot->_slotState == MultiLevelRankSlotState::FINISH)
				{
					slot = nextSlot;
				}
			}
		}

		_switcherInfoImage->SetActiveWidgetIndex(InfoState::ACTIVE);
		FText::FindText(TEXT("MultiLevelRank"), TEXT("UI_InfoText_Completed"), infoText);
		infoText = FText::Format(infoText, slot->_slotSchema->NameText);

		// 최고 달성한 랭크 슬롯의 아이콘 설정
		TWeakObjectPtr<UImage> weakThis(_imageInfo);
		FGsUIHelper::AsyncLoadSpriteFromPath(slot->_slotSchema->iconPath, FStreamableDelegateParam::CreateWeakLambda(this, [weakThis](UObject* inRes)
			{
				if (UImage* strongThis = weakThis.Get())
				{
					strongThis->SetBrushFromAtlasInterface(Cast<UPaperSprite>(inRes));
				}
			}));
	}
	_textBlockInfo->SetText(infoText);

	// 서약 변경 버튼 갱신
	if (rankManager->IsPledgeSelected())
	{
		_btnPledgeChange->SetIsEnabled(true);
	}
	else
	{
		_btnPledgeChange->SetIsEnabled(false);
	}
}

void UGsUIWindowMultiLevelRank::InvalidateQuest(MultiLevelRank InRank)
{
	int32 findIndex = _slotRankList.Find(InRank);
	TWeakObjectPtr<UGsUIMultiLevelRankSlot> slot = Cast<UGsUIMultiLevelRankSlot>(_slotHelperRank->GetEntry(findIndex));
	if (slot.IsValid())
	{
		slot->InvalidateSlotAll();
	}

	InvalidateInfo();
}

void UGsUIWindowMultiLevelRank::SetTutorial(bool bIsTutorial)
{
	_bIsTutorial = bIsTutorial;

	if (_bIsTutorial)
	{
		_mainScrollBox->ScrollToStart();
		_mainScrollBox->SetAllowOverscroll(false);

		CheckTutorialTargetList();
	}
	else
	{		
		OnTutorialListEvent.Unbind();
		_mainScrollBox->SetAllowOverscroll(true);
	}
}

void UGsUIWindowMultiLevelRank::CheckTutorialTargetList()
{
	if (OnTutorialListEvent.IsBound())
	{
		// 첫번째 리스트 아이템을 튜토리얼에 전송
		if (UWidget* firstWidget = _slotHelperRank->GetEntry(0))
		{
			OnTutorialListEvent.Execute(firstWidget);

			// 한 번만 보내도록 바인드 바로제거
			OnTutorialListEvent.Unbind();
		}
	}
}

void UGsUIWindowMultiLevelRank::OnScrollMain(float InCurrentOffset)
{
	if (IsTutorial())
	{
		_mainScrollBox->SetScrollOffset(0.f);
	}
}
