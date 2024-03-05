
#include "GsUIServerMigrateCheckConditionEntry.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsBMShopManager.h"
#include "GameFlow/GameContents/ContentsGame/Hud/BMShop/ServerMigrate/GsServerMigrateData.h"
#include "GameFlow/GameContents/ContentsGame/Hud/BMShop/ServerMigrate/GsServerMigrateHandler.h"
#include "DataSchema/BMShop/ServerMigrate/GsSchemaServerMigrateGroupData.h"
#include "DataSchema/BMShop/ServerMigrate/GsSchemaServerMigrateConditionData.h"
#include "UI/UIContent/Popup/BMShop/GsUIPopupServerMigrateServerList.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UILib/Base/GsUIWidgetBase.h"
#include "T1Project.h"


void UGsUIServerMigrateCheckConditionEntry::BeginDestroy()
{
	Super::BeginDestroy();
}

void UGsUIServerMigrateCheckConditionEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (_btnFeature)
	{
		_btnFeature->OnClicked.AddDynamic(this, &UGsUIServerMigrateCheckConditionEntry::OnClickedFeature);
	}

	// 2023/11/6 PKT - 조건 충족 / 불충족에 따라 Color가 나뉘는데 불충족은 Color Table에서 찾고 조건 충족은 기본 컬러를 따라간다.
	_validColor = _textConditionName->ColorAndOpacity;
}

void UGsUIServerMigrateCheckConditionEntry::NativeConstruct()
{
	Super::NativeConstruct();

	_serverMigrateType = ServerMigrateType::NONE;

	// 2023/10/30 PKT - Empty Text
	const static FText TextEmpty = FText::GetEmpty();
	_textConditionName->SetText(TextEmpty);
	_textDesc->SetText(TextEmpty);

	// 2023/10/30 PKT - 기본 Off
	_btnFeature->SetVisibility(ESlateVisibility::Collapsed);

	// 2023/11/6 PKT - 
	FText::FindText(TEXT("ServerMigrateText"), TEXT("ServerMigrate_UiNotice_MigratePossible"), _msgPossible);
	FText::FindText(TEXT("ServerMigrateText"), TEXT("ServerMigrate_UiNotice_MigrateImPossible"), _msgImpossible);
}

void UGsUIServerMigrateCheckConditionEntry::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGsUIServerMigrateCheckConditionEntry::OnClickedFeature()
{
	if (!_data.IsValid())
	{
		GSLOG(Error, TEXT("nullptr == _schemaConditionData"));
		return;
	}

	if (auto data = _data.Pin())
	{
		if (ServerMigrateConditionType::SERVER_CHOICE == data->_type)
		{	// 2023/10/31 PKT - 서버 선택에 대한 기능이라면..
			if (UGsUIManager* uiManager = GUI())
			{
				TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(("PopupServerMigrateServerSelected"));
				if (widget.IsValid())
				{
					UGsUIPopupServerMigrateServerList* popup = Cast<UGsUIPopupServerMigrateServerList>(widget.Get());
					if (popup)
					{
						ServerMigrateType groupType = _serverMigrateType;
						ServerMigrateConditionType conditionType = data->_type;

						// 2023/11/6 PKT - Set Call Back Func
						auto OnCallback = [groupType, conditionType](WorldId InWorldId)
							{
								FGsServerMigrateHandler* handler = (GBMShop()) ? GBMShop()->GetServerMigrateHandler() : nullptr;
								if (nullptr == handler)
								{
									GSLOG(Error, TEXT("nullptr == handler"));
									return;
								}

								handler->SetConditionState(groupType, conditionType, (INVALID_WORLD_ID != InWorldId) ? true : false, InWorldId);
							};

						popup->SetData(_serverMigrateType, OnCallback);
					}
				}
			}
		}
	}
}

void UGsUIServerMigrateCheckConditionEntry::SetUIByFeatureType()
{
	if (auto data = _data.Pin())
	{
		if (ServerMigrateConditionType::SERVER_CHOICE == data->_type)
		{
			if (FGsServerMigrateHandler* handler = (GBMShop()) ? GBMShop()->GetServerMigrateHandler() : nullptr)
			{
				// 2023/11/6 PKT - 이주 하고자 하는 서버를 선택한 상황이라면..
				if (auto selectServer = handler->GetSelectedServer().Pin())
				{
					if (auto net = GNet())
					{	// 2023/10/31 PKT - 서버 이름.
						FString serverName;
						net->TryGetServerName(selectServer->_worldId, serverName);
						_textConditionName->SetText(FText::FromString(serverName));
					}

					if (0 < selectServer->_userCount)
					{	// 2023/10/31 PKT - 캐릭터가 있는 서버를 선택했습니다.
						FText message;
						FText::FindText(TEXT("ServerMigrateText"), TEXT("SEVER_CHOICE_Clear_HaveCharacter"), message);
						_textDesc->SetText(message);
					}
				}
			}
		}
	}
}

void UGsUIServerMigrateCheckConditionEntry::CheckedInvalidate()
{
	if (UGsUIManager* uiManager = GUI())
	{
		TWeakObjectPtr<UGsUIPopupServerMigrateServerList> widgetPopup
			= Cast<UGsUIPopupServerMigrateServerList>(uiManager->GetWidgetByKey(TEXT("PopupServerMigrateServerSelected")));
		if (widgetPopup.IsValid())
		{
			widgetPopup->Invalidate();
		}
	}
}

void UGsUIServerMigrateCheckConditionEntry::RefreshUI()
{
	const static int32 INVALID_INDEX = 0;
	const static int32 VALID_INDEX = 1;

	static FLinearColor defInvalidColor = FGsUIColorHelper::GetColor(EGsUIColorType::DEFAULT_INVALID);
	static FLinearColor defValidColor = FGsUIColorHelper::GetColor(EGsUIColorType::DEFAULT_VALID);

	_btnFeature->SetVisibility(ESlateVisibility::Collapsed);
	_switcherConditionImg->SetActiveWidgetIndex(INVALID_INDEX);
	_textConditionName->SetColorAndOpacity(defInvalidColor);
	_textDesc->SetColorAndOpacity(defInvalidColor);

	if (auto data = _data.Pin())
	{
		bool isValid = data->_isPossible;
		if (ServerMigrateConditionType::SERVER_CHOICE == data->_type)
		{
			if(FGsServerMigrateHandler* handler = (GBMShop()) ? GBMShop()->GetServerMigrateHandler() : nullptr)
				isValid = handler->GetSelectedServer().IsValid();

			_btnFeature->SetVisibility(ESlateVisibility::Visible);
		}

		// 2023/11/6 PKT - Img Switching
		_switcherConditionImg->SetActiveWidgetIndex(isValid ? VALID_INDEX : INVALID_INDEX);

		// 2023/11/6 PKT - 조건 이름
		_textConditionName->SetText(data->GetConditionTitle());
		_textConditionName->SetColorAndOpacity(isValid ? _validColor : defInvalidColor);

		// 2023/11/6 PKT - 조건 상태에 대한 설명
		FText descText = isValid ? _msgPossible : _msgImpossible;
		if (false == isValid)
		{	// 2023/10/30 PKT - 기획에서 Data를 넣을때 추가 설명이 없는 경우 스페이스바 하나를 넣는다. 이거 제거 후 체크 해야 함.
			FString temp = data->GetNotFitConditionDesc().ToString().Replace(TEXT(" "), TEXT(""), ESearchCase::IgnoreCase);
			if (false == temp.IsEmpty())
			{
				FString combindText = FString::Format(TEXT("{0} : {1}"), { *descText.ToString(), *data->GetNotFitConditionDesc().ToString() });
				descText = FText::FromString(combindText);
			}
		}
		_textDesc->SetText(descText);
		_textDesc->SetColorAndOpacity(isValid ? defValidColor : defInvalidColor);

		// 2023/11/1 PKT - 특수 기능들을 여기서 정의 함.
		if(isValid) SetUIByFeatureType();

		// 2023/11/6 PKT - 화면 갱신으로 넘어온 상황이라면.. 갱신 해줘야 할 것들 갱신함.
		CheckedInvalidate();
	}
}
