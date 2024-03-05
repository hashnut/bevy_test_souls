#include "GsGameStateItemCollection.h"

#include "GsContentsGameBase.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"

#include "GameFlow/GameContents/GsContentsMode.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/MessageParam/GsItemCollectionMessageParam.h"

#include "UI/UIContent/Window/GsUIWindowItemCollection.h"
#include "UI/UILib/Base/GsUIWidgetBase.h"



FGsGameStateItemCollection::FGsGameStateItemCollection()
	: FGsContentsGameBase(FGsContentsMode::InGame::ContentsItemCollection)
{
}

void FGsGameStateItemCollection::Enter()
{
	FGsContentsGameBase::Enter();

	if (UGsUIManager* UiManager = GUI())
	{
		TWeakObjectPtr<UGsUIWidgetBase> Widget = UiManager->OpenAndGetWidget(TEXT("WindowItemCollection"));
		if (Widget.IsValid())
		{
			_window = Cast<UGsUIWindowItemCollection>(Widget);
		}
	}

	// 네비게이션 바 재화 세팅 (드롭다운 메뉴 감춤, 선택 슬롯 감춤)
	SetNavigationBar(CurrencyType::NONE);

	RegisterMessages();
}

void FGsGameStateItemCollection::Exit()
{
	// 주의: Exit 시 Window 닫기를 하지 않음(UI Stack과 State연계로직 때문. OnBack, CloseAll등의 로직에 의해 닫음)
	_window = nullptr;

	// 메세지 해지
	UnregisterMessages();

	// HUD 관련 레드닷 갱신 요청	
	GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::ITEM_COLLECTION, false);

	FGsContentsGameBase::Exit();
}

void FGsGameStateItemCollection::RegisterMessages()
{
	MItemCollection& MessageUiHolder = GMessage()->GetItemCollection();

	//_messageUiDelegates.Emplace(MessageUiHolder.AddRaw(MessageContentItemCollection::OPEN_HUD, this, &FGsGameStateItemCollection::OnOpenByParam));

	//_messageUiDelegates.Emplace(MessageUiHolder.AddRaw(MessageContentItemCollection::OPEN_SHORTCUT, this, &FGsGameStateItemCollection::OnOpenByParam));

	_messageUiDelegates.Emplace(MessageUiHolder.AddRaw(MessageContentItemCollection::FIELD_INVALID, this, &FGsGameStateItemCollection::OnInvalidate));

	_messageUiDelegates.Emplace(MessageUiHolder.AddRaw(MessageContentItemCollection::SEARCH_MATERIAL, this, &FGsGameStateItemCollection::OnSearchMaterial));


	_messageMaterialDelegate = GMessage()->GetMaterial().AddRaw(MessageContentMaterial::EXIT, this, &FGsGameStateItemCollection::OnMaterialSetUIExit);

	_unlockDelegate = GMessage()->GetUnlock().AddRaw(MessageContentUnlock::NOTIFY_CONTENTS_UNLOCK, this, &FGsGameStateItemCollection::OnContentsUnlock);
}

void FGsGameStateItemCollection::UnregisterMessages()
{
	MItemCollection& MessageUiHolder = GMessage()->GetItemCollection();
	for (const TPair<MessageContentItemCollection, FDelegateHandle>& DelegatePair : _messageUiDelegates)
	{
		MessageUiHolder.Remove(DelegatePair);
	}

	GMessage()->GetMaterial().Remove(_messageMaterialDelegate);

	GMessage()->GetUnlock().Remove(_unlockDelegate);
}

void FGsGameStateItemCollection::OnInvalidate(const IGsMessageParam* InParam)
{
	const FGsItemCollectionMessageParam* param = InParam->Cast<const FGsItemCollectionMessageParam>();
	if (nullptr == param)
	{
		GSLOG(Error, TEXT("nullptr == param"));
		return;
	}
	
	if (_window.IsValid())
	{
		_window->InvalidateFieldSet(param->_filedId);
	}
}

void FGsGameStateItemCollection::OnSearchMaterial(const IGsMessageParam* InParam)
{
	const FGsPrimitiveInt32* param = InParam->Cast<const FGsPrimitiveInt32>();
	if (nullptr == param)
	{
		GSLOG(Error, TEXT("nullptr == param"));
		return;
	}

	if (_window.IsValid())
	{
		_window->SearchMaterial(param->_data);
	}
}

void FGsGameStateItemCollection::OnOpenByParam(const IGsMessageParam* InParam)
{
	if (!_window.IsValid())
	{
		GSLOG(Warning, TEXT("!_window.IsValid()"));
		return;
	}

	const FGsItemCollectionMessageOpenParam* param = StaticCast<const FGsItemCollectionMessageOpenParam*>(InParam);
	if (nullptr == param)
	{
		GSLOG(Warning, TEXT("nullptr == param"));
		return;
	}

	//if (INVALID_ITEM_ID != param->_itemTId)
	//{	// 2021/10/14 PKT - 바로가기를 통해 들어온것.
	//	_window->OpenShortCut(param->_itemTId, param->_Name);
	//}
}

void FGsGameStateItemCollection::OnMaterialSetUIExit(const IGsMessageParam* InParam)
{
	if (_window.IsValid())
	{
		_window->ExitMaterialSetDetail();
	}
}

void FGsGameStateItemCollection::OnContentsUnlock(const IGsMessageParam* InParam)
{
	if (_window.IsValid() && InParam)
	{
		const FGsPrimitivePairInt32* param = StaticCast<const FGsPrimitivePairInt32*>(InParam);
		_window->OnContentsUnlock(StaticCast<EGsUnlockContentsType>(param->_first));
	}
}