#include "GsUIWindowQuestBoard.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/ListView.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Common/GsUICurrencyRefreshCountButton.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/GsMessageUI.h"
#include "Message/GsMessageContentHud.h"
#include "Message/GsMessageGameObject.h"
#include "Message/MessageParam/GsQuestMessageParam.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "GameObject/Quest/GsQuestHandler.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateQuestBoard.h"
#include "GsUIQuestBoardListViewEntry.h"
#include "GsUIQuestBoardDetailPanel.h"
#include "Currency/GsCurrencyHelper.h"
#include "Shared/Shared/SharedInclude/SharedDefine.h"
#include "UMG/Public/Components/RichTextBlock.h"
#include "Unlock/GsUnlockDefine.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"

const FTextKey REFRESH_BUTTON_TEXT_KEY = TEXT("RefreshButton");
const FTextKey QUEST_ACCEPT_TEXT_KEY = TEXT("QuestAcceptdeny");
const FTextKey QUEST_BOARD_VILLAGE_TAB_KEY = TEXT("QuestBoardVillageTab");
const FTextKey QUEST_BOARD_INVADE_TAB_KEY = TEXT("QuestBoardInvadeTab");
const FTextKey NOTICE_DAILY_QUEST_ALL_CLEAR = TEXT("Notice_DailyQuestAllClear");
const FTextKey QUEST_ACCEPT_DENY_TEXT_KEY = TEXT("QuestAcceptdeny");
const FTextKey NO_PROGRESSABLE_QUESTS_TEXT_KEY = TEXT("NoProgressableQuests");

void UGsUIWindowQuestBoard::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_toggleGroupMainTab.AddToggleWidgetByParentPanel(_tabPanelWidget);
	_toggleGroupMainTab.OnSelectChanged.BindUObject(this, &UGsUIWindowQuestBoard::OnClickTab);
	_toggleGroupMainTab.OnCheckBlock.BindUObject(this, &UGsUIWindowQuestBoard::OnCheckTable);

	_closeButton->OnClicked.AddDynamic(this, &UGsUIWindowQuestBoard::OnClickCloseAllStack);
	_backButton->OnClicked.AddDynamic(this, &UGsUIWindowQuestBoard::OnClickBack);
	_refreshButton->OnClickRefreshButton.BindUObject(this, &UGsUIWindowQuestBoard::OnRefresh);
	
	_acceptButton->OnClicked.AddDynamic(this, &UGsUIWindowQuestBoard::OnAcceptQuest);
	_giveUpButton->OnClicked.AddDynamic(this, &UGsUIWindowQuestBoard::OnGiveUpQuest);	
	_completeButton->OnClicked.AddDynamic(this, &UGsUIWindowQuestBoard::OnCompleteQuest);

	_questBoardSortButton->OnClicked.AddDynamic(this, &UGsUIWindowQuestBoard::OnClickSort);
	_questAcceptedSortButton->OnClicked.AddDynamic(this, &UGsUIWindowQuestBoard::OnSortByAccept);
	_questNotAcceptedSortButton->OnClicked.AddDynamic(this, &UGsUIWindowQuestBoard::OnSortByUnaccept);
	_questCompletedSortButton->OnClicked.AddDynamic(this, &UGsUIWindowQuestBoard::OnSortByComplete);
}

void UGsUIWindowQuestBoard::NativeConstruct()
{
	Super::NativeConstruct();

	if (FGsMessageHolder* msg = GMessage())
	{
		////퀘스트 게시판 초기화
		_uiDelegateArray.Emplace(msg->GetUI().AddUObject(MessageUI::INITIALIZE_QUEST_BOARD, this, &UGsUIWindowQuestBoard::OnInitialize));
		//퀘스트 게시판에서 퀘스트 수주, 포기 했을 경우 갱신 
		_uiDelegateArray.Emplace(msg->GetUI().AddUObject(MessageUI::INVALID_QUEST_BOARD, this, &UGsUIWindowQuestBoard::OnInvaliate));					
		//퀘스트 게시판 퀘스트 선택
		_uiDelegateArray.Emplace(msg->GetUI().AddUObject(MessageUI::SELECT_QUEST_BOARD, this, &UGsUIWindowQuestBoard::OnSelect));
		_uiDelegateArray.Emplace(msg->GetUI().AddUObject(MessageUI::INVALID_QUEST_BOARD_INDEX, this, &UGsUIWindowQuestBoard::OnInvalidQuestIndex));
		_uiDelegateArray.Emplace(msg->GetUI().AddUObject(MessageUI::SORT_QUEST_BOARD, this, &UGsUIWindowQuestBoard::OnSort));

		//퀘스트 오브젝트 변경시 갱신
		_msgGameObjectHandleList.Emplace(msg->GetGameObject().AddUObject(MessageGameObject::QUEST_WINDOW_UPDATE_OBJECTIVE, this, &UGsUIWindowQuestBoard::OnInvalidButton));
	}

	_questBoard = FGsGameStateQuestBoard::GetInstance();

	//FText::FindText(QUEST_BOARD_TEXT_KEY, REFRESH_BUTTON_TEXT_KEY, _refreshText);
	FText::FindText(QUEST_BOARD_TEXT_KEY, QUEST_ACCEPT_TEXT_KEY, _questAcceptText);
	FText::FindText(QUEST_BOARD_TEXT_KEY, QUEST_BOARD_VILLAGE_TAB_KEY, _townQuestTitleText);
	FText::FindText(QUEST_BOARD_TEXT_KEY, QUEST_BOARD_INVADE_TAB_KEY, _invasionQuestTitleText);
	FText::FindText(QUEST_BOARD_TEXT_KEY, NOTICE_DAILY_QUEST_ALL_CLEAR, _noticeDailyQuestAllClearText);
	FText::FindText(QUEST_BOARD_TEXT_KEY, QUEST_ACCEPT_DENY_TEXT_KEY, _maxQuestReachText);
	FText::FindText(QUEST_BOARD_TEXT_KEY, TEXT("QuestAcceptRenewalDeny"), _renewerDenyText);
	FText::FindText(QUEST_BOARD_TEXT_KEY, NO_PROGRESSABLE_QUESTS_TEXT_KEY, _noticeNoProgressableQuestsText);
		
	if (IsStackRecoverFlag())
	{
		_questBoard->RecoveryQuestBoard();
		_questBoard->SetSelectFirst(false);
	}
}

void UGsUIWindowQuestBoard::NativeDestruct()
{
	if (FGsMessageHolder* msg = GMessage())
	{
		if (_uiDelegateArray.Num() > 0)
		{
			for (auto iter : _uiDelegateArray)
			{
				msg->GetUI().Remove(iter);
			}
			_uiDelegateArray.Empty();
		}

		if (_contentsDelegateArray.Num() > 0)
		{
			for (auto iter : _contentsDelegateArray)
			{
				msg->GetContentsHud().Remove(iter);
			}
			_contentsDelegateArray.Empty();
		}

		if (_msgGameObjectHandleList.Num() > 0)
		{
			for (auto iter : _msgGameObjectHandleList)
			{
				msg->GetGameObject().Remove(iter);
			}
			_msgGameObjectHandleList.Empty();
		}
	}

	// 닫을때 list 삭제
	_questBoardListView->ClearListItems();

	Super::NativeDestruct();
}

void UGsUIWindowQuestBoard::OnRefresh()
{	
	if (nullptr == _refreshCost)
	{
		return;
	}

	Currency myAmount = FGsCurrencyHelper::GetCurrencyAmount(_refreshCost->RefreshCurrencyType);
	if (_refreshCost->RefreshCurrencyCost > myAmount)
	{
		FGsCurrencyHelper::OpenLackCurrencyPopup(_refreshCost->RefreshCurrencyType, _refreshCost->RefreshCurrencyCost - myAmount);
	}
	else
	{
		//send refresh
		_questBoard->SetSelectFirst(true);
		FGsGameStateQuestBoard::GetInstance()->SendRefreshQuestBaord();
	}	
}

void UGsUIWindowQuestBoard::OnAcceptQuest()
{
	//최대 퀘스트 갯수가 도달했는지 확인 
	if (FGsGameStateQuestBoard::GetInstance()->GetAvailableQuestCount() <= 0)
	{
		FGsUIHelper::TrayMessageTicker(_maxQuestReachText);
		return;
	}

	bool isAccept = FGsGameStateQuestBoard::GetInstance()->IsCanAcceptQuest();	
	if (false == isAccept)
	{
		FGsUIHelper::TrayMessageTicker(_questAcceptText);
		return;
	}

	//send accept
	FGsGameStateQuestBoard::GetInstance()->SendAcceptQuestBoard();
}

void UGsUIWindowQuestBoard::OnGiveUpQuest()
{
	//send give up
	FGsGameStateQuestBoard::GetInstance()->SendGiveUpQuestBoard();
}

void UGsUIWindowQuestBoard::OnCompleteQuest()
{
	_questBoard->SetSelectFirst(true);
	OpenPopupQuestReward();	
}

void UGsUIWindowQuestBoard::OnClickTab(int32 InIndex)
{
	_questBoardTab = (QuestBoardType)(InIndex + 1);

	_questBoard->ClearSelectQuestData();
	_questBoard->SetSelectFirst(true);
	_questBoard->SelectQuestBoardType(_questBoardTab);

	InvalidQuestCountTitle();
}

void UGsUIWindowQuestBoard::OnInitialize(const struct IGsMessageParam* inParam)
{
	if (nullptr == _questBoard)
	{
		return;
	}

	_questBoardTab = _questBoard->GetQuestBoardType();	
	_selectQuest = _questBoard->GetSelectQuest();
	_toggleGroupMainTab.SetSelectedIndexNoEvent(((int)_questBoardTab) - 1);	

	ClearData();
	InvalidateAll();
	InvalidQuestCountTitle();
}

void UGsUIWindowQuestBoard::OnInvalidButton(const struct IGsMessageParam* inParam)
{
	InvalidButton();
}

void UGsUIWindowQuestBoard::OnInvaliate(const IGsMessageParam* inParam)
{	
	InvalidateAll();
}

void UGsUIWindowQuestBoard::InvalidateAll()
{
	InvalidQuestBoardList();
	InvalidateInfo();
	InvalidButton();
}

void UGsUIWindowQuestBoard::OnClickSort()
{
	if (_sortPanelWidget->IsVisible())
	{
		_sortPanelWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		_sortPanelWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UGsUIWindowQuestBoard::OnSortByAccept()
{	
	FGsGameStateQuestBoard::GetInstance()->SetSortType(EGsQuestStateType::Accept);
	HideSort();
	OnInitialize(nullptr);
	SelectFirstQuestBoard();

	FGsUIMsgParamSelectQuestBoard param(FGsGameStateQuestBoard::GetInstance()->GetSelectQuest());
	GMessage()->GetUI().SendMessage(MessageUI::SELECT_QUEST_BOARD, &param);
}

void UGsUIWindowQuestBoard::OnSortByUnaccept()
{	
	FGsGameStateQuestBoard::GetInstance()->SetSortType(EGsQuestStateType::Unaccept);	
	HideSort();
	OnInitialize(nullptr);
	SelectFirstQuestBoard();

	FGsUIMsgParamSelectQuestBoard param(FGsGameStateQuestBoard::GetInstance()->GetSelectQuest());
	GMessage()->GetUI().SendMessage(MessageUI::SELECT_QUEST_BOARD, &param);
}

void UGsUIWindowQuestBoard::OnSortByComplete()
{	
	FGsGameStateQuestBoard::GetInstance()->SetSortType(EGsQuestStateType::Complete);	
	HideSort();
	OnInitialize(nullptr);
	SelectFirstQuestBoard();

	FGsUIMsgParamSelectQuestBoard param(FGsGameStateQuestBoard::GetInstance()->GetSelectQuest());
	GMessage()->GetUI().SendMessage(MessageUI::SELECT_QUEST_BOARD, &param);
}

void UGsUIWindowQuestBoard::InvalidateInfo()
{
	FGsGameStateQuestBoard* board = FGsGameStateQuestBoard::GetInstance();
	if (nullptr == board)
	{
		return;
	}	
	
	_refreshCount = board->GetRefreshCount();
	_refreshCost = board->GetCostInfo();
	_selectQuest = board->GetSelectQuest();
	
	InvalidRefresh();	
	InvalidQuestCount();	
	HideSort();
	InvalidLockPanel();
}

bool UGsUIWindowQuestBoard::InvalidQuestBoardList()
{
	if (nullptr == _questBoard)
	{
		return false;
	}

	TArray<TSharedPtr<FGsQuestBoardData>> questArray = _questBoard->GetQuestDataArray();	
	if (0 == questArray.Num()
		|| false == IsMainQuestUnlock(_questBoardTab))
	{
		return false;
	}

	UGsQuestBoardListViewEntryData* entryData = nullptr;
	TArray<UGsQuestBoardListViewEntryData*> _questBoardDataArray;
	for (TSharedPtr<FGsQuestBoardData>& questPair : questArray)
	{
		entryData = NewObject<UGsQuestBoardListViewEntryData>();
		entryData->Initialize(questPair, this);

		_questBoardDataArray.Add(entryData);
	}	

	_questBoardListView->SetListItems(_questBoardDataArray);
	_questBoardListView->SetScrollbarVisibility(ESlateVisibility::Collapsed);

	//scroll
	QuestStoryIdPair selectQuest = _questBoard->GetSelectQuest();
	UGsQuestBoardListViewEntryData** find = _questBoardDataArray.FindByPredicate([&](const UGsQuestBoardListViewEntryData* inPair)->bool {
		if (inPair->_questboardData.IsValid())
		{
			return inPair->_questboardData.Pin()->IsEquals(selectQuest);
		}
		
		return false;
		});
	if (find)
	{		
		int32 index = _questBoardListView->GetIndexForItem(*find);
		_questBoardListView->NavigateToIndex(index);
	}	

	return true;
}

void UGsUIWindowQuestBoard::InvalidButton()
{
	QuestId  questId = _selectQuest.mQuestId;
	if (0 >= questId)
	{
		_buttonSwitcher->SetVisibility(ESlateVisibility::Hidden);
		_acceptButton->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	_buttonSwitcher->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	TSharedPtr<FGsQuestData> questData = GSQuest()->GetQuestData(questId);
	if (questData.IsValid())
	{
		if (GSQuest()->IsProgress(_selectQuest.mQuestId, _selectQuest.mQuestIndex))
		{
			FGsQuestDynamicData* dynmaicData = questData.Get()->GetQuestDynamicData(_selectQuest.mQuestIndex);
			if (dynmaicData)
			{
				switch (dynmaicData->GetQuestState())
				{
				case QuestState::COMPLETED:
				{
					_buttonSwitcher->SetActiveWidgetIndex(2);
				}
				break;
				case QuestState::ACCEPTED:
				{
					_buttonSwitcher->SetActiveWidgetIndex(1);
				}
				break;
				default:
				{
					_buttonSwitcher->SetActiveWidgetIndex(0);

					bool isCanAccept = FGsGameStateQuestBoard::GetInstance()->IsCanAcceptQuest();

					_acceptButton->SetVisibility(ESlateVisibility::Visible);
					_acceptButton->SetIsEnabled(isCanAccept);
				}
				break;
				}
			}
		}
		else
		{
			_buttonSwitcher->SetActiveWidgetIndex(0);

			bool isCanAccept = FGsGameStateQuestBoard::GetInstance()->IsCanAcceptQuest();

			_acceptButton->SetVisibility(ESlateVisibility::Visible);
			//_acceptButton->SetIsEnabled(isCanAccept);
		}
	}
	else
	{
		_buttonSwitcher->SetActiveWidgetIndex(0);

		bool isCanAccept = FGsGameStateQuestBoard::GetInstance()->IsCanAcceptQuest();

		_acceptButton->SetVisibility(ESlateVisibility::Visible);
		//_acceptButton->SetIsEnabled(isCanAccept);
	}
}

void UGsUIWindowQuestBoard::InvalidRefresh()
{
	FGsGameStateQuestBoard* board = FGsGameStateQuestBoard::GetInstance();
	if (nullptr == board)
	{
		return;
	}

	if (_refreshCost)
	{
		_refreshButton->SetData(_refreshCount, _refreshCost->RefreshCurrencyType, _refreshCost->RefreshCurrencyCost);
	}

	if (false == IsMainQuestUnlock(_questBoardTab))
	{
		_refreshButton->SetEnableButton(false);		
	}
	else if (0 >= board->GetQuestBoardCount())
	{
		_refreshButton->SetEnableButton(false);
	}
	else
	{		
		_refreshButton->SetEnableButton(_refreshCount > 0);
	}
}

void UGsUIWindowQuestBoard::InvalidQuestCount()
{
	//반복 퀘스트 갯수 
	int32 questCount = GSQuest()->GetQuestCount(QuestType::REPEAT);	
	FText questCountText = FText::FromString(FString::Printf(TEXT("%d/%d"), questCount, MAX_ACTIVE_QUEST_REPEAT_COUNT));
	_questCountTextBlock->SetText(questCountText);	

	//현재 수주한 퀘스트 갯수
	int32 dailyMaxQuestCount = _questBoard->GetDailyMaxAcceptableQuestCount();
	int32 count = dailyMaxQuestCount - FGsGameStateQuestBoard::GetInstance()->GetAvailableQuestCount();
	FText questBoardCountText = FText::FromString(FString::Printf(TEXT("%d/%d"), count, dailyMaxQuestCount));
	_questBoardCountTextBlock->SetText(questBoardCountText);
}

void UGsUIWindowQuestBoard::InvalidQuestCountTitle()
{
	switch (_questBoardTab)
	{
	case QuestBoardType::VILLAGE:
	{
		_questBoardTitleCountTextBlock->SetText(_townQuestTitleText);
	}
	break;
	case QuestBoardType::INVADE:
	{
		_questBoardTitleCountTextBlock->SetText(_invasionQuestTitleText);
	}
	break;
	default:
		break;
	}
}

void UGsUIWindowQuestBoard::HideSort()
{
	_sortPanelWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UGsUIWindowQuestBoard::OpenPopupQuestReward()
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
	{		
		return;
	}

	FGsQuestHandler* questHandler = questManager->GetQuestHandler();
	if (nullptr == questHandler)
	{
		return;
	}

	FGsQuestInstance* questInstance = questHandler->FindQuestInstance(QuestKey(_selectQuest.mQuestId, _selectQuest.mQuestIndex), _selectQuest.mStoryId);
	if (nullptr == questInstance)
	{
		// 존재하지않는 퀘스트이다.
		return;
	}

	questHandler->OpenPopupQuestRepeatReward(questInstance);
}

void UGsUIWindowQuestBoard::OnSelect(const struct IGsMessageParam* inParam)
{
	_selectQuest = FGsGameStateQuestBoard::GetInstance()->GetSelectQuest();
	InvalidQuestCount();
	InvalidButton();
}

void UGsUIWindowQuestBoard::OnInvalidQuestIndex(const struct IGsMessageParam* inParam)
{
	const FGsUIMsgParamSelectQuestBoard* param = inParam->Cast<const FGsUIMsgParamSelectQuestBoard>();
	if (nullptr == param)
	{
		return;
	}

	if (_selectQuest == param->_quest)
	{
		if (-1 != param->_newQuestIndex)
		{
			_selectQuest.mQuestIndex = param->_newQuestIndex;

#if QUEST_BOARD_DEBUG
			GSLOG(Log, TEXT("quest board debug - invalid select quest index - quest id : %lld\t story id : %lld\tindex : %lld")
				, (int64)_selectQuest.mQuestId
				, (int64)_selectQuest.mStoryId
				, (int64)_selectQuest.mQuestIndex);
#endif
		}		
	}	
}

void UGsUIWindowQuestBoard::OnSort(const struct IGsMessageParam* inParam)
{
	const FGsUIMsgParamSortQuestBoard* param = inParam->Cast<const FGsUIMsgParamSortQuestBoard>();
	if (nullptr == param)
	{
		return;
	}

	switch (param->_sortType)
	{
	case EGsQuestStateType::Accept:
	{
		OnSortByAccept();
	}
		break;
	case EGsQuestStateType::Complete:
	{
		OnSortByComplete();
	}
		break;
	case EGsQuestStateType::Unaccept:
	{
		OnSortByUnaccept();
	}
		break;
	}
}

void UGsUIWindowQuestBoard::SelectFirstQuestBoard()
{
	FGsGameStateQuestBoard::GetInstance()->SelectFirstQuestBoard();
	_questBoardListView->NavigateToIndex(0);	
}

bool UGsUIWindowQuestBoard::OnCheckTable(int32 inIndex)
{
	return (false == IsContentsUnlock((QuestBoardType)(inIndex + 1)));
}

void UGsUIWindowQuestBoard::ClearData()
{
	_questBoardListView->ClearListItems();

	_refreshCount = 0;
	_refreshCost = nullptr;
}

bool UGsUIWindowQuestBoard::IsContentsUnlock(QuestBoardType inBoardType)
{
	switch (inBoardType)
	{
	case QuestBoardType::VILLAGE:
	{
		if (false == GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::VILLAGE_QUEST_BOARD, true))
		{
			return false;
		}
	}
	break;
	case QuestBoardType::INVADE:
	{
		if (false == GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::INVADE_QUEST_BOARD, true))
		{
			return false;
		}
	}
	break;
	}

	return true;
}

bool UGsUIWindowQuestBoard::IsMainQuestUnlock(QuestBoardType inBoardType)
{
	//마을 의뢰 퀘스트 조건 체크, 메인 퀘스트가 없다면 true 반환, 존재한다면 현재 메인 퀘스트와 비교하여 언락 처리
	if (false == _questBoard->IsValidMainQuestCondition(inBoardType))
	{
		return false;
	}

	return true;
}


bool UGsUIWindowQuestBoard::IsInvalidBoardType(QuestBoardType inBoardType)
{
	// QuestBoardType에 매칭되는 데이타가 VillageQuestBoardList에 있는가?
	if (false == _questBoard->IsValidQuestCondition(inBoardType))
	{
		return false;
	}

	return true;
}

void UGsUIWindowQuestBoard::InvalidLockPanel()
{
	FGsGameStateQuestBoard* board = FGsGameStateQuestBoard::GetInstance();
	if (nullptr == board)
	{
		return;
	}

	if (false == IsInvalidBoardType(_questBoardTab))
	{
		_lockPanelWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_lockRichText->SetText(_noticeNoProgressableQuestsText);
		_detailPanel->SetVisibility(ESlateVisibility::Hidden);

		return;
	}

	if (false == IsMainQuestUnlock(_questBoardTab))
	{
		FText lockText;
		board->TryGetLockText(lockText, _questBoardTab);

		_lockPanelWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_lockRichText->SetText(lockText);
		_detailPanel->SetVisibility(ESlateVisibility::Hidden);
	}
	else if (0 == _questBoard->GetQuestBoardCount())
	{
		_lockPanelWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_lockRichText->SetText(_noticeDailyQuestAllClearText);
		_detailPanel->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		_lockPanelWidget->SetVisibility(ESlateVisibility::Collapsed);
		_detailPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_detailPanel->Invalidate(_selectQuest);
	}
}