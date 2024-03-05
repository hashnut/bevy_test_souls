
#include "GsGameStateTitle.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Message/MessageParam/GsMessageParam.h"


FGsGameStateTitle::FGsGameStateTitle()
	: FGsContentsGameBase(FGsContentsMode::InGame::ContentsTitle)
{

}


void FGsGameStateTitle::Enter()
{
	FGsContentsGameBase::Enter();

	if (UGsUIManager* UiManager = GUI())
	{
		TWeakObjectPtr<UGsUIWidgetBase> Widget = UiManager->OpenAndGetWidget(TEXT("WindowTitleContetns"));
		if (Widget.IsValid())
		{
			_window = Cast<UGsUIWindowTitleSystem>(Widget);
		}
	}

	RegisterMessages();

	SetNavigationBar(CurrencyType::NONE);
}


void FGsGameStateTitle::Exit()
{
	// 메세지 해지
	UnregisterMessages();

	// HUD 관련 레드닷 갱신 요청	
	GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::TITLE, false);

	FGsContentsGameBase::Exit();
}


void FGsGameStateTitle::RegisterMessages()
{
	FGsMessageHolder* messageHolder = GMessage();

	_messageUiDelegates.Emplace(messageHolder->GetTitle().AddRaw(MessageContentTitle::UPDATE_TITLE
		, this
		, &FGsGameStateTitle::UpdateTitle));

	_messageUiDelegates.Emplace(messageHolder->GetTitle().AddRaw(MessageContentTitle::UPDATE_CATEGORY_REDDOT
		, this
		, &FGsGameStateTitle::UpdateCategoryRedDot));

	_messageUiDelegates.Emplace(messageHolder->GetTitle().AddRaw(MessageContentTitle::UPDATE_EQUIP
		, this
		, &FGsGameStateTitle::UpdateEquip));

	_messageUiDelegates.Emplace(messageHolder->GetTitle().AddRaw(MessageContentTitle::UPDATE_COLLECTION
		, this
		, &FGsGameStateTitle::UpdateCollection));
}


void FGsGameStateTitle::UnregisterMessages()
{
	FGsMessageHolder* messageHolder = GMessage();

	for (TPair<MessageContentTitle, FDelegateHandle>& msgHandler : _messageUiDelegates)
	{
		messageHolder->GetTitle().Remove(msgHandler);
	}
	_messageUiDelegates.Empty();

	for (TPair<MessageInvasion, FDelegateHandle>& msgHandler : _listInvadeDelegates)
	{
		messageHolder->GetInvasion().Remove(msgHandler);
	}
	_listInvadeDelegates.Empty();
}


void FGsGameStateTitle::UpdateTitle(const IGsMessageParam* InParam)
{
	if (false == _window.IsValid())
	{
		return;
	}

	const FGsPrimitiveInt32* param = StaticCast<const FGsPrimitiveInt32*>(InParam);
	if (nullptr != param)
	{	// 2022/01/28 PKT - 칭호 상태 변경
		_window->InvalidTitle(StaticCast<TitleId>(param->_data));
		// 2022/01/28 PKT - 상태 변경에 따른 카테고리 RedDot 갱신
		_window->InvalidCategoryRedDot();
	}
}

void FGsGameStateTitle::UpdateCategoryRedDot(const IGsMessageParam* InParam)
{
	if (false == _window.IsValid())
	{
		return;
	}
	// 2022/01/28 PKT - 카테고리 RedDot 갱신
	_window->InvalidCategoryRedDot();
}


void FGsGameStateTitle::UpdateEquip(const IGsMessageParam* InParam)
{
	if (false == _window.IsValid())
	{
		return;
	}

	const FGsPrimitiveInt32* param = StaticCast<const FGsPrimitiveInt32*>(InParam);
	if (nullptr != param)
	{	// 2022/01/28 PKT - 장착 상태에 따른 칭호 상태 변경
		_window->InvalidTitle(StaticCast<TitleId>(param->_data));
		// 2022/01/28 PKT - 장착 상태 변경
		_window->InvalidEquip();
	}
}


void FGsGameStateTitle::UpdateCollection(const IGsMessageParam* InParam)
{
	if (false == _window.IsValid())
	{
		return;
	}
	// 2022/01/28 PKT - 컬렉션 리스트 변경
	_window->InvalidCollection();
}