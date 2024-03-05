#include "GsUIServerMigrateServerListEntry.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsBMShopManager.h"
#include "GameFlow/GameContents/ContentsGame/Hud/BMShop/ServerMigrate/GsServerMigrateData.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Tray/NotiMessage/GsUISectionMessageDefine.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/PanelWidget.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "T1Project.h"


void UGsUIServerMigrateServerListEntry::BeginDestroy()
{
	Super::BeginDestroy();
}

void UGsUIServerMigrateServerListEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (_btnNormal)
	{
		_btnNormal->OnClicked.AddDynamic(this, &UGsUIServerMigrateServerListEntry::OnClickedNormal);
	}
}

void UGsUIServerMigrateServerListEntry::NativeConstruct()
{
	Super::NativeConstruct();

	_data = nullptr;

	_callbackFunc = nullptr;
}

void UGsUIServerMigrateServerListEntry::OnClickedNormal()
{
	if (auto data = _data.Pin())
	{
		if (ServerMigrateLimitStatus::CLOSING == data->_status)
		{	// 2023/11/1 PKT - 제한된 서버는 선택 할 수 없습니다.
			FText message;
			FText::FindText(TEXT("ServerMigrateText"), TEXT("ServerMigrate_Notice_ServerConditionClosing"), message);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, message);
			return;
		}

		if (_myServer)
		{
			FText message;
			FText::FindText(TEXT("ServerMigrateText"), TEXT("ServerMigrate_Notice_ServerConditionHere"), message);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, message);
			return;
		}

		// 2023/11/1 PKT - 정보를 전달 한다.
		_callbackFunc(_data);
	}
}
void UGsUIServerMigrateServerListEntry::SetData(TWeakPtr<MigrateServer> InData, WorldId InSelectedWorldId, TFunction<void(TWeakPtr<MigrateServer>)> InCallbackFunc)
{
	_data = InData;
	_callbackFunc = InCallbackFunc;
	
	if (auto data = _data.Pin())
	{
		_myServer = data->_worldId == GGameData()->GetUserData()->mHomeWorldId;

		if (GNet())
		{	// 2023/10/31 PKT - 서버 이름.
			FString serverName;
			GNet()->TryGetServerName(data->_worldId, serverName);
			_textServerNormalTypeName->SetText(FText::FromString(serverName));
			_textServerSelectedTypeName->SetText(FText::FromString(serverName));
		}

		// 2023/10/31 PKT - 추천 서버
		_panelRecommend->SetVisibility(data->_isRecommended ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
		// 2023/10/31 PKT - 신규 서버
		_panelNew->SetVisibility(data->_isNewOpenServer ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
		// 2023/10/31 PKT - 내 케릭터
		_panelMyCharacter->SetVisibility(0 < data->_userCount ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
		// 2023/10/31 PKT - 현재 서버
		_connectedServer->SetVisibility(_myServer ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);

		if (ServerMigrateLimitStatus::FREE == data->_status)
		{
			const static int32 FREE_INDEX = 0;
			_switcherServerState->SetActiveWidgetIndex(FREE_INDEX);
		}
		else if (ServerMigrateLimitStatus::IMMINENT == data->_status)
		{
			const static int32 IMMINENT_INDEX = 1;
			_switcherServerState->SetActiveWidgetIndex(IMMINENT_INDEX);
		}
		else if (ServerMigrateLimitStatus::CLOSING == data->_status)
		{
			const static int32 CLOSING_INDEX = 2;
			_switcherServerState->SetActiveWidgetIndex(CLOSING_INDEX);
		}
	}

	Invalidate(InSelectedWorldId);
}

void UGsUIServerMigrateServerListEntry::Invalidate(WorldId InSelectedWorldId)
{
	if (auto data = _data.Pin())
	{
		if (data->_worldId == InSelectedWorldId)
		{
			_btnNormal->SetVisibility(ESlateVisibility::Collapsed);
			_btnSelected->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			_btnNormal->SetVisibility(ESlateVisibility::Visible);
			_btnSelected->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

bool UGsUIServerMigrateServerListEntry::IsSelected() const
{
	return (ESlateVisibility::Collapsed != _btnSelected->GetVisibility());
}