#include "GameFlow/GameContents/ContentsGame/GsGameStateBMShop.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Message/MessageParam/GsBMShopMessageParam.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIContent/Window/BMShop/GsUIWindowBMShop.h"

FGsGameStateBMShop::FGsGameStateBMShop() : FGsContentsGameBase(FGsContentsMode::InGame::ContentsBMShop)
{
}
FGsGameStateBMShop::~FGsGameStateBMShop()
{
}

void FGsGameStateBMShop::Enter()
{
	FGsContentsGameBase::Enter();

	RegisterMessages();
	SetNavigationBar(CurrencyType::NONE);

	if (UGsUIManager* uiManager = GUI())
	{
		if (true == uiManager->IsActiveWidget(TEXT("WindowBMShop")))
		{
			return;
		}
		uiManager->OpenAndGetWidget(("WindowBMShop"));
	}
}

void FGsGameStateBMShop::Exit()
{
	RemoveMessage();

	FGsContentsGameBase::Exit();
}

void FGsGameStateBMShop::RemoveMessage()
{
	FGsMessageHolder* msg = GMessage();
	for (TPair<MessageContentStoreBM, FDelegateHandle>& elemItem : _bmShopMsgHandlerList)
	{
		msg->GetBMShop().Remove(elemItem);
	}
	_bmShopMsgHandlerList.Empty();
}


void FGsGameStateBMShop::RegisterMessages()
{
	if (0 < _bmShopMsgHandlerList.Num())
		return;

	//bak1210 : 처리내용 없을땐 선등록 금지 (퍼포먼스 낭비)
	FGsMessageHolder* msg = GMessage();
	_bmShopMsgHandlerList.Emplace(msg->GetBMShop().AddRaw(FGsBMShopInvalidateServerMigrage::MESSAGE_ID, this, &FGsGameStateBMShop::InvalidateServerMigrate));
}

void FGsGameStateBMShop::OpenBMShop(const struct IGsMessageParam* InMessageParam)
{

}

void FGsGameStateBMShop::CloseBMShop(const struct IGsMessageParam* InMessageParam)
{

}

void FGsGameStateBMShop::InvalidateServerMigrate(FGsSharedParam& InMessageParam)
{
	if (UGsUIManager* uiManager = GUI())
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->GetWidgetByKey(TEXT("WindowBMShop"));
		if (widget.IsValid())
		{
			UGsUIWindowBMShop* window = Cast<UGsUIWindowBMShop>(widget.Get());
			if (window)
			{
				window->ServerMigrateViewRefresh();
			}
		}
	}
}
