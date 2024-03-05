#include "GsUIPopupServerMigrateServerList.h"
#include "Management/ScopeGlobal/GsBMShopManager.h"
#include "GameFlow/GameContents/ContentsGame/Hud/BMShop/ServerMigrate/GsServerMigrateData.h"
#include "UI/UIContent/Popup/BMShop/GsUIServerMigrateServerListEntry.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Components/PanelWidget.h"
#include "Message/MessageParam/GsMessageParam.h"

using MenuType = UGsUIServerMigrateServerListSortComboBox::MenuType;

void UGsUIPopupServerMigrateServerList::BeginDestroy()
{
	if (nullptr != _serverItemSlotHelper)
	{
		_serverItemSlotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupServerMigrateServerList::OnServerItemRefresh);
		_serverItemSlotHelper = nullptr;
	}

	if(_comboBox != nullptr) _comboBox->OnSelectedMenu.Unbind();
	
	Super::BeginDestroy();
}

void UGsUIPopupServerMigrateServerList::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (_btnClose)
	{
		_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupServerMigrateServerList::OnClose);
	}

	if (_btnSelected)
	{
		_btnSelected->OnClicked.AddDynamic(this, &UGsUIPopupServerMigrateServerList::OnServerSelected);
	}

	if (nullptr == _serverItemSlotHelper)
	{
		_serverItemSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_serverItemSlotHelper->Initialize(_serverItemEntryWidgetClass, _panelServerListRoot);
		_serverItemSlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPopupServerMigrateServerList::OnServerItemRefresh);
	}

	_comboBox->OnSelectedMenu.BindUObject(this, &UGsUIPopupServerMigrateServerList::OnMenuSelected);
}

void UGsUIPopupServerMigrateServerList::NativeConstruct()
{
	Super::NativeConstruct();
	FGsMessageHolder* msg = GMessage();
	_refreshMsgHandler = msg->GetBMShop().AddUObject(FGsBMShopInvalidateServerMigrage::MESSAGE_ID,
		this, &UGsUIPopupServerMigrateServerList::InvalidateServerMigrate);

	_callbackFunc = nullptr;

	_ownerType = ServerMigrateType::NONE;

	_sortType = EServerListViewSortType::Normal;

	_selectServer.Reset();
	_serverList.Reset();

	_comboBox->SetMenuSelect(MenuType::Normal);

	TArray<UWidget*> children = _panelServerListRoot->GetAllChildren();
	for (auto child : children)
	{
		if (auto entry = Cast<UGsUIServerMigrateServerListEntry>(child))
			entry->RemoveFromParent();
	}
}

void UGsUIPopupServerMigrateServerList::NativeDestruct()
{
	if (FGsMessageHolder* msg = GMessage())
	{
		msg->GetBMShop().Remove(_refreshMsgHandler);
	}

	Super::NativeDestruct();
}

void UGsUIPopupServerMigrateServerList::OnInputCancel()
{
	Close();
}

void UGsUIPopupServerMigrateServerList::OnClose()
{
	Close();
}

void UGsUIPopupServerMigrateServerList::InvalidateServerMigrate(FGsSharedParam& InMessageParam)
{
	_serverList.Reset();
	_selectServer.Reset();

	if (FGsServerMigrateHandler* handler = (GBMShop()) ? GBMShop()->GetServerMigrateHandler() : nullptr)
	{
		handler->GetServerList(_serverList);
		_selectServer = handler->GetSelectedServer();
	}
	Invalidate();
}

void UGsUIPopupServerMigrateServerList::OnServerItemRefresh(int32 InIndex, UWidget* InEntry)
{
	if (_serverList.IsValidIndex(InIndex))
	{
		if (auto serverData = _serverList[InIndex].Pin())
		{
			TWeakObjectPtr<UGsUIPopupServerMigrateServerList> thiz = this;
			auto CallBack = [thiz](TWeakPtr<MigrateServer> InData) -> void
				{
					if (thiz.IsValid())
						thiz->OnCallbackSelectedEntry(InData);
				};

			UGsUIServerMigrateServerListEntry* entry = Cast<UGsUIServerMigrateServerListEntry>(InEntry);
			if (entry)
			{
				WorldId selectWorld = INVALID_WORLD_ID;
				if (auto selectServer = _selectServer.Pin())
				{
					selectWorld = selectServer->_worldId;
				}
				entry->SetData(serverData, selectWorld, CallBack);
			}
		}
	}
}

void UGsUIPopupServerMigrateServerList::OnCallbackSelectedEntry(TWeakPtr<MigrateServer> InSelectedData)
{
	_selectServer = InSelectedData;

	// 2023/11/1 PKT - 갱신
	const int32 countMax = _serverItemSlotHelper->GetItemCount();
	for (int32 idx = 0; idx < countMax; ++idx)
	{
		UGsUIServerMigrateServerListEntry* entry = Cast<UGsUIServerMigrateServerListEntry>(_serverItemSlotHelper->GetEntry(idx));
		if (entry)
		{
			entry->Invalidate(InSelectedData.IsValid() ? InSelectedData.Pin()->_worldId : INVALID_WORLD_ID);
		}
	}
}

void UGsUIPopupServerMigrateServerList::OnServerSelected()
{
	if (_selectServer.IsValid() == false)
	{
		FText message;
		FText::FindText(TEXT("ServerMigrateText"), TEXT("ServerMigrate_Notice_ServerNotChoice"), message);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, message);

		return;
	}

	_callbackFunc(_selectServer.Pin()->_worldId);

	Close();
}

void UGsUIPopupServerMigrateServerList::OnMenuSelected(UGsUIServerMigrateServerListSortComboBox::MenuType type)
{
	static TMap<MenuType, EServerListViewSortType> sortCheck = {
		{MenuType::Normal, EServerListViewSortType::Normal },
		{MenuType::Recommended, EServerListViewSortType::Recommended },
		{MenuType::Spacious, EServerListViewSortType::Spacious },
	};

	_sortType = sortCheck[type];

	Invalidate();
}

void UGsUIPopupServerMigrateServerList::SetData(ServerMigrateType InOwnerType, TFunction<void(WorldId)> InCallbackFunc)
{
	_ownerType = InOwnerType;
	_callbackFunc = InCallbackFunc;

	_sortType = EServerListViewSortType::Normal;

	_serverList.Reset();
	_selectServer.Reset();

	if (FGsServerMigrateHandler* handler = (GBMShop()) ? GBMShop()->GetServerMigrateHandler() : nullptr)
	{
		handler->GetServerList(_serverList);
		_serverList.RemoveAll([](TWeakPtr<MigrateServer> e) { return e.IsValid() == false; });

		_selectServer = handler->GetSelectedServer();
	}
	Invalidate();
}

void UGsUIPopupServerMigrateServerList::Invalidate()
{
	int32 maxCount = BuildViewServerItemList(_sortType);
	_serverItemSlotHelper->RefreshAll(maxCount);
}

int32 UGsUIPopupServerMigrateServerList::BuildViewServerItemList(EServerListViewSortType InType)
{
	// 시간차로 popup을 띄운 상태에서, refresh로 서버 데이터를 삭제하는 경우가 있어 sort 진행 중에 data 가 valid 한지 체크하도록 수정
	// 서버 데이터도 같은 데이터의 경우 새로 만들지 않고 다시 사용하도록 수정

	if (EServerListViewSortType::Recommended == InType)
	{
		_serverList.StableSort([](TWeakPtr<MigrateServer> pe, TWeakPtr<MigrateServer> fe) {
				// 2023/10/31 PKT - 이 부분 테스트 하면서 해야 함.
				auto pRecommend = pe.IsValid() ? pe.Pin()->_isRecommended : false;
				auto fRecommend = fe.IsValid() ? fe.Pin()->_isRecommended : false;

				if (pRecommend != fRecommend) return pRecommend;
				return false;
			});
	}
	else if (EServerListViewSortType::Spacious == InType)
	{
		_serverList.StableSort([](TWeakPtr<MigrateServer> pe, TWeakPtr<MigrateServer> fe) {
				// 2023/10/31 PKT - 이 부분 테스트 하면서 해야 함.
				auto pSpacious = pe.IsValid() ? pe.Pin()->_status : ServerMigrateLimitStatus::NONE;
				auto fSpacious = fe.IsValid() ? fe.Pin()->_status : ServerMigrateLimitStatus::NONE;

				if (pSpacious != fSpacious) return pSpacious == ServerMigrateLimitStatus::FREE;
				return false;
			});
	}

	return _serverList.Num();
}