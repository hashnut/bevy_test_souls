#include "GsGameStateMultiLevelRank.h"

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"

#include "UI/UILib/Base/GsUIWidgetBase.h"
#include "UI/UIContent/Window/GsUIWindowMultiLevelRank.h"
#include "UI/UIContent/Popup/GsUIPopupMultiLevelRankPledgeSelect.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Message/MessageParam/GsMultiLevelRankMessageParam.h"



FGsGameStateMultiLevelRank::FGsGameStateMultiLevelRank()
	: FGsContentsGameBase(FGsContentsMode::InGame::ContentsMultiLevelRank)
{
	/**/
}

void FGsGameStateMultiLevelRank::Enter()
{
	FGsContentsGameBase::Enter();

	if (UGsUIManager* UiManager = GUI())
	{
		TWeakObjectPtr<UGsUIWidgetBase> Widget = UiManager->OpenAndGetWidget(TEXT("WindowMultiLevelRank"));
		if (Widget.IsValid())
		{
			_window = Cast<UGsUIWindowMultiLevelRank>(Widget);
		}
	}

	// 네비게이션 바 재화 세팅 (드롭다운 메뉴 감춤, 선택 슬롯 감춤)
	SetNavigationBar(CurrencyType::NONE);

	RegisterMessages();
}

void FGsGameStateMultiLevelRank::Exit()
{
	// 주의: Exit 시 Window 닫기를 하지 않음(UI Stack과 State연계로직 때문. OnBack, CloseAll등의 로직에 의해 닫음)
	_window = nullptr;

	// 메세지 해지
	UnregisterMessages();
	
	// HUD 관련 레드닷 갱신 요청
	GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::MULTILEVELRANK, false);

	FGsContentsGameBase::Exit();
}

void FGsGameStateMultiLevelRank::RegisterMessages()
{
	MMultiLevelRank& MessageUiHolder = GMessage()->GetMultiLevelRank();

	_messageUiDelegates.Emplace(MessageUiHolder.AddRaw(MessageContentMultiLevelRank::INVALIDATE_SLOT_ALL
		, this, &FGsGameStateMultiLevelRank::OnInvalidateSlotAll));

	_messageUiDelegates.Emplace(MessageUiHolder.AddRaw(MessageContentMultiLevelRank::INVALIDATE_QUEST
		, this, &FGsGameStateMultiLevelRank::OnInvalidateQuest));

	_messageUiDelegates.Emplace(MessageUiHolder.AddRaw(MessageContentMultiLevelRank::INITIALIZE_SLOT_ALL
		, this, &FGsGameStateMultiLevelRank::OnInitializeSlotAll));

	_messageUiDelegates.Emplace(MessageUiHolder.AddRaw(MessageContentMultiLevelRank::CHANGE_FOCUSED_SLOT
		, this, &FGsGameStateMultiLevelRank::OnChangeFocusedSlot));

}

void FGsGameStateMultiLevelRank::UnregisterMessages()
{
	MMultiLevelRank& MessageUiHolder = GMessage()->GetMultiLevelRank();
	for (const TPair<MessageContentMultiLevelRank, FDelegateHandle>& DelegatePair : _messageUiDelegates)
	{
		MessageUiHolder.Remove(DelegatePair);
	}
}

void FGsGameStateMultiLevelRank::OnInvalidateSlotAll(const IGsMessageParam* InParam)
{
	if (!_window.IsValid())
	{
		GSLOG(Warning, TEXT("!_window.IsValid()"));
		return;
	}

	_window->InvalidateSlotAll();

	if (UGsUIManager* UiManager = GUI())
	{
		TWeakObjectPtr<UGsUIWidgetBase> Widget = UiManager->GetWidgetByKey(TEXT("PopupMultiLevelRankPledgeSelect"));
		if (Widget.IsValid())
		{
			if (UGsUIPopupMultiLevelRankPledgeSelect* popupPledgeSelect = Cast<UGsUIPopupMultiLevelRankPledgeSelect>(Widget))
			{
				popupPledgeSelect->InvalidateSlot();
			}
		}
	}
}

void FGsGameStateMultiLevelRank::OnInvalidateQuest(const IGsMessageParam* InParam)
{
	if (!_window.IsValid())
	{
		GSLOG(Warning, TEXT("!_window.IsValid()"));
		return;
	}

	const FGsMultiLevelRankSingleDataMessageParam* param = InParam->Cast< const FGsMultiLevelRankSingleDataMessageParam >();
	_window->InvalidateQuest(param->GetData());
}

void FGsGameStateMultiLevelRank::OnInitializeSlotAll(const IGsMessageParam* InParam)
{
	if (!_window.IsValid())
	{
		GSLOG(Warning, TEXT("!_window.IsValid()"));
		return;
	}

	_window->InitializeSlotAll();
}

void FGsGameStateMultiLevelRank::OnChangeFocusedSlot(const IGsMessageParam* InParam)
{
	if (!_window.IsValid())
	{
		GSLOG(Warning, TEXT("!_window.IsValid()"));
		return;
	}

	const FGsMultiLevelRankSingleDataMessageParam* param = InParam->Cast<const FGsMultiLevelRankSingleDataMessageParam>();
	_window->ChangeFocusedSlot(param->GetData());
}
