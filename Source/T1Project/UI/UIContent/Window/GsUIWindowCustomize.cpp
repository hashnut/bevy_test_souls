// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIWindowCustomize.h"
#include "Components/WidgetSwitcher.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Window/Customize/GsUICustomizeBase.h"
#include "UI/UIContent/Window/Customize/GsUICustomizePreset.h"
#include "UI/UIContent/Window/Customize/GsUICustomizeFace.h"
#include "UI/UIContent/Window/Customize/GsUICustomizeHair.h"
#include "UI/UIContent/Window/Customize/GsUICustomizeEyes.h"
#include "UI/UIContent/Window/Customize/GsUICustomizeBody.h"
#include "UI/UIContent/Window/Customize/GsUICustomizeTattoo.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Popup/GsUIPopupCreateNickName.h"
#include "UI/UIContent/Popup/GsUIPopupReservationCreateNickName.h"
#include "UI/UIContent/Popup/GsUIPopupYesNo.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeLobby/GsLobbyCharacterManager.h"
#include "Management/ScopeLobby/GsLobbyDataManager.h"
#include "Management/ScopeGlobal/GsNetManager.h"

#include "DataSchema/Customize/GsSchemaCustomizeMenu.h"
#include "DataSchema/Customize/GsSchemaCustomizePartsGroup.h"
#include "DataSchema/Customize/GsSchemaCustomizeSlotData.h"

#include "GameObject/Customize/GsCustomizeHandler.h"

#include "T1Project.h"

#include "Message/GsMessageStage.h"
#include "Message/GsMessageContents.h"


void UGsUIWindowCustomize::BeginDestroy()
{
	_mainContentList.Empty();
	_customizeHandler = nullptr;
	_bodyList = nullptr;

	Super::BeginDestroy();
}

void UGsUIWindowCustomize::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_toggleGroupEquip.AddToggleWidgetByParentPanel(_panelEquip);
	_toggleGroupEquip.OnSelectChanged.BindUObject(this, &UGsUIWindowCustomize::OnClickEquip);

	// 주의: 순서에 유의
	_toggleGroupMainTab.AddToggleWidget(_sBtnFace);
	_toggleGroupMainTab.AddToggleWidget(_sBtnHair);
	_toggleGroupMainTab.AddToggleWidget(_sBtnEyes);
	_toggleGroupMainTab.AddToggleWidget(_sBtnBody);
	_toggleGroupMainTab.AddToggleWidget(_sBtnTattoo);
	_toggleGroupMainTab.OnSelectChanged.BindUObject(this, &UGsUIWindowCustomize::OnClickMainTab);

	// EGsMainMenu의 순서대로
	_mainContentList.Emplace(_contentFace);
	_mainContentList.Emplace(_contentHair);
	_mainContentList.Emplace(_contentEyes);
	_mainContentList.Emplace(_contentBody);
	_mainContentList.Emplace(_contentTattoo);

	_contentFace->OnClickSlot.BindUObject(this, &UGsUIWindowCustomize::OnClickFaceSlot);

	// 페이지 이벤트 연결
	_contentPreset->OnSetPreset.BindUObject(this, &UGsUIWindowCustomize::OnSetPreset);
	_contentPreset->OnClickRandom.BindUObject(this, &UGsUIWindowCustomize::OnClickRandom);
		
	_btnFinish->OnClicked.AddDynamic(this, &UGsUIWindowCustomize::OnClickFinishCustomizing);
	_btnBack->OnClicked.AddDynamic(this, &UGsUIWindowCustomize::OnClickBack);

	_btnChangeCustomizing->OnClicked.AddDynamic(this, &UGsUIWindowCustomize::OnClickChangeCustomizing);
	_btnCancelChangeCustomizing->OnClicked.AddDynamic(this, &UGsUIWindowCustomize::OnClickCancelChangeCustomizing);

	
}

void UGsUIWindowCustomize::NativeConstruct()
{
	Super::NativeConstruct();

	// 데이터 세팅
	_contentPreset->SetSelectedData(&_selectedData);
	for (int i = 0; i < _mainContentList.Num(); ++i)
	{
		_mainContentList[i]->SetSelectedData(&_selectedData);
	}

	if (UGsLobbyDataManager* lobbyDataMgr = LSLobbyData())
	{
		// 외형변경 중인가?
		if (_switcherCustomizingMode)
		{
			_switcherCustomizingMode->SetActiveWidgetIndex(lobbyDataMgr->IsChangeCustomizing() ? 1 : 0);
		}
	}
}

void UGsUIWindowCustomize::NativeDestruct()
{
	/**
	 * https://jira.com2us.com/jira/browse/C2URWQ-3626
	 * 2023/4/4 PKT - 요 이슈로 인해 창닫는 시점을 이쪽으로 옮김.
	 */
	if (UGsUIManager* uiMgr = GUI())
	{
		TWeakObjectPtr<UGsUIPopupCreateNickName> uiPopup = Cast<UGsUIPopupCreateNickName>(uiMgr->GetWidgetByKey(TEXT("PopupCreateNickName")));
		if (uiPopup.IsValid())
		{
			uiPopup->Close();
		}
	}

	_bIsZoomIn = false;
	_customizeHandler = nullptr;
	_bodyList = nullptr;

	Super::NativeDestruct();
}

bool UGsUIWindowCustomize::OnBack()
{
	// 캐릭터 생성 State로 이동
	//FGsUIHelper::TrayDimmed(true);
	FGsUIHelper::TrayFadeIn(1.5f);
	GMessage()->GetStage().SendMessage(MessageStage::ENTER_CHARACTERCREATE_STAGE);
	
	return true;
}

void UGsUIWindowCustomize::SetData(int32 InMenuId, class FGsCustomizeHandler* InCustomizeHandler)
{
	SetCustomizeHandler(InCustomizeHandler);

	// 순서주의: 핸들서 세팅 후 수행할 것
	SetMenu(InMenuId);

	_contentPreset->Show();

	_toggleGroupMainTab.SetSelectedIndex(0, true);
	_toggleGroupEquip.SetSelectedIndex(0, true);

	SetZoom(false, true);
	
	FString advReservationUserName = GNet()->GetAdvReservationUserName();
	if (!advReservationUserName.IsEmpty())
	{
		// 사전예약 캐릭터인 경우
		_verticalBoxReservationNickName->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_reservationNickName->SetText(FText::FromString(advReservationUserName));
	}
	else
	{
		// 사전예약 캐릭터가 아닌경우
		_verticalBoxReservationNickName->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUIWindowCustomize::SetCustomizeHandler(class FGsCustomizeHandler* InCustomizeHandler)
{
	if (_customizeHandler == InCustomizeHandler)
	{
		return;
	}

	_customizeHandler = InCustomizeHandler;

	for (UGsUICustomizeBase* mainContent : _mainContentList)
	{
		mainContent->SetCustomizeHandler(InCustomizeHandler);
	}

	_customizeHandler->SetByCustomizeUIData(&_selectedData);
}

void UGsUIWindowCustomize::SetMenu(int32 InMenuId)
{
	FName menuKey = FName(*FString::FromInt(InMenuId));
	if (const UGsTable* table = FGsSchemaCustomizeMenu::GetStaticTable())
	{
		const FGsSchemaCustomizeMenu* row = nullptr;
		if (table->FindRow<FGsSchemaCustomizeMenu>(menuKey, row))
		{
			_selectedData.SetMenu(row);
			_selectedData.SetPresetData(0);
		}
	}

	_bodyList = nullptr;

	// 프리셋 슬롯 세팅
	if (const FGsSchemaCustomizeMenu* menu = _selectedData.GetMenu())
	{		
		// 복장 리스트 세팅
		if (const FGsSchemaCustomizePartsGroup* bodyGroup = menu->bodyPartsGroup.GetRow())
		{
			_bodyList = &bodyGroup->slotList;
		}
	}
}

void UGsUIWindowCustomize::SetUIMainTab(EGsMainMenu InType)
{
	// 탭전환 처리
	_switcherContent->SetActiveWidgetIndex(InType);

	for (int32 i = 0; i < _mainContentList.Num(); ++i)
	{
		if (i == InType)
		{
			_mainContentList[i]->Show();
			break;
		}
	}

	// 카메라 Zoom 처리
	if (EGsMainMenu::Body == InType)
	{
		SetZoom(false);
	}
	else
	{
		SetZoom(true);
	}
}

void UGsUIWindowCustomize::SetZoom(bool bIsZoomIn, bool bInForced)
{
	if (false == bInForced)
	{
		if (bIsZoomIn == _bIsZoomIn)
		{
			return;
		}
	}

	if (bIsZoomIn)
	{
		GMessage()->GetContentsLobby().SendMessage(MessageContentsLobby::ZOOM_HEAD);
		_bIsZoomIn = true;
	}
	else
	{
		GMessage()->GetContentsLobby().SendMessage(MessageContentsLobby::ZOOM_WHOLE_BODY);
		_bIsZoomIn = false;
	}
}

void UGsUIWindowCustomize::SetBodyPartsChange(int32 InBodyPartsId)
{
	if (nullptr == _customizeHandler)
	{
		return;
	}

	_customizeHandler->SetBodyChange(InBodyPartsId);

	// 스킨컬러 덮어쓰기... 리셋이 되버린다
	/*int32 colorIndex = _selectedData.GetSkinColorIndex();
	if (0 <= colorIndex)
	{
		_customizeHandler->SetSkinColorChange(colorIndex);
	}*/
}

UGsUICustomizeBase* UGsUIWindowCustomize::GetCurrPage() const
{
	return Cast<UGsUICustomizeBase>(_switcherContent->GetActiveWidget());
}

void UGsUIWindowCustomize::OnClickEquip(int32 InIndex)
{ 
	if (_bodyList &&
		_bodyList->IsValidIndex(InIndex))
	{
		SetBodyPartsChange(_bodyList->GetData()[InIndex].targetId);
	}
}

void UGsUIWindowCustomize::OnClickMainTab(int32 InIndex)
{
	SetUIMainTab(static_cast<EGsMainMenu>(InIndex));
}

void UGsUIWindowCustomize::OnClickFinishCustomizing()
{
	if (UGsUIManager * uiMgr = GUI())
	{
		FString advReservationUserName = GNet()->GetAdvReservationUserName();
		if (!advReservationUserName.IsEmpty())
		{
			auto widget = uiMgr->OpenAndGetWidget(TEXT("PopupReservationCreateNickName"));
			if (widget.IsValid())
			{
				UGsUIPopupReservationCreateNickName* uiPopup = Cast<UGsUIPopupReservationCreateNickName>(widget.Get());
				uiPopup->SetOkCallback([this](const FString& InNickName)
					{
						OnCreateNickName(InNickName);
					});
			}
		}
		else
		{
			auto widget = uiMgr->OpenAndGetWidget(TEXT("PopupCreateNickName"));
			if (widget.IsValid())
			{
				UGsUIPopupCreateNickName* uiPopup = Cast<UGsUIPopupCreateNickName>(widget.Get());
				uiPopup->SetOkCallback([this](const FString& InNickName)
					{
						OnCreateNickName(InNickName);
					});
			}
		}		
	}
}

void UGsUIWindowCustomize::OnClickChangeCustomizing()
{
	UGsLobbyCharacterManager* characterMgr = LSLobbyCharacter();
	if (nullptr == characterMgr)
	{
		GSLOG(Error, TEXT("[Customize] OnCreateNickName, UGsLobbyCharacterManager is null"));
		return;
	}

	if (nullptr == _customizeHandler)
	{
		GSLOG(Error, TEXT("[Customize] OnCreateNickName, _customizeHandler is null"));
		return;
	}

	UGsUIManager* uiMgr = GUI();
	if (nullptr == uiMgr)
	{
		return;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->OpenAndGetWidget(FName(TEXT("PopupYesNo")));
	if (false == widget.IsValid())
	{
		return;
	}

	UGsUIPopupYesNo* uiPopup = Cast<UGsUIPopupYesNo>(widget.Get());
	if (nullptr == uiPopup)
	{
		return;
	}

	// TEXT: 캐릭터의 외형을 변경하시겠습니까?
	FText text;
	FText::FindText(TEXT("LobbyText"), TEXT("Popup_Confirm_Text"), text);

	uiPopup->SetData(text, [this](bool bYes)
	{
		if (bYes)
		{
			_customizeHandler->SendGatewayReqCustomizing(&_selectedData);
			
		}
	});
}

void UGsUIWindowCustomize::OnSetPreset(int32 InIndex)
{
	int32 prevIndex = _selectedData.GetValue(EGsUICustomizeType::PRESET_INDEX);
	if (prevIndex != InIndex)
	{
		_selectedData.SetPresetData(InIndex);
	}

	// 다시 갱신시켜서 바뀐 프리셋에 맞게 선택되어 있도록 함
	_toggleGroupMainTab.SetSelectedIndex(0, true);

	if (_customizeHandler)
	{
		_customizeHandler->SetByCustomizeUIData(&_selectedData);

		// 주의: 옷이 기본장비로 리셋되므로 현재 선택 복장으로 다시 덮어써준다.
		/*int32 equipIndex = _toggleGroupEquip.GetSelectedIndex();
		if (_bodyList &&
			_bodyList->IsValidIndex(_toggleGroupEquip.GetSelectedIndex()))
		{
			SetBodyPartsChange(_bodyList->GetData()[equipIndex].targetId);
		}*/
	}

	// 카메라 줌 아웃 처리
	SetZoom(false, true);
}

void UGsUIWindowCustomize::OnClickRandom()
{
	_selectedData.SetRandom();

	if (_customizeHandler)
	{
		_customizeHandler->SetByCustomizeUIData(&_selectedData);
		//bak1210 : 컬러 적용이 외형에 맞추어 처리되있어 외형이 바뀌지 않았을때를 대비해서 강제로 컬러 설정한다.
		/*_customizeHandler->ForceUpdateSkinColor(0);*/
		/*_customizeHandler->ForceFaceAttibuteColor();*/

		// 주의: 옷이 기본장비로 리셋되므로 현재 선택 복장으로 다시 덮어써준다.
		/*int32 equipIndex = _toggleGroupEquip.GetSelectedIndex();
		if (_bodyList &&
			_bodyList->IsValidIndex(_toggleGroupEquip.GetSelectedIndex()))
		{
			SetBodyPartsChange(_bodyList->GetData()[equipIndex].targetId);
		}*/
	}

	// 현재 UI 갱신
	if (UGsUICustomizeBase* page = GetCurrPage())
	{
		page->Show();
	}
}

void UGsUIWindowCustomize::OnClickFaceSlot()
{
	SetZoom(true);
}

void UGsUIWindowCustomize::OnCreateNickName(const FString& InNickName)
{
	UGsLobbyCharacterManager* characterMgr = LSLobbyCharacter();
	if (nullptr == characterMgr)
	{
		GSLOG(Error, TEXT("[Customize] OnCreateNickName, UGsLobbyCharacterManager is null"));
		return;
	}		

	if (nullptr == _customizeHandler)
	{
		GSLOG(Error, TEXT("[Customize] OnCreateNickName, _customizeHandler is null"));
		return;
	}

	_customizeHandler->SendGatewayReqCreateUser(InNickName, &_selectedData);
}

void UGsUIWindowCustomize::OnClickCancelChangeCustomizing()
{
	// 캐릭터 선택 Stage로 이동
	UGsUIManager* uiMgr = GUI();
	if (nullptr == uiMgr)
	{
		return;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->OpenAndGetWidget(FName(TEXT("PopupYesNo")));
	if (false == widget.IsValid())
	{
		return;
	}

	UGsUIPopupYesNo* uiPopup = Cast<UGsUIPopupYesNo>(widget.Get());
	if (nullptr == uiPopup)
	{
		return;
	}

	// TEXT: 외형 변경을 취소하시겠습니까?
	FText text;
	FText::FindText(TEXT("LobbyText"), TEXT("Popup_Cancel_Text"), text);

	uiPopup->SetData(text, [this](bool bYes)
		{
			if (bYes)
			{
				if (UGsLobbyDataManager* lobbyDataMgr = LSLobbyData())
				{
					//lobbyDataMgr->ClearCustomizingUserDBId();
					lobbyDataMgr->SetIsBackToCharacterSelect(true);
					FGsUIHelper::TrayFadeIn(1.5f);

					GMessage()->GetStage().SendMessage(MessageStage::ENTER_CHARACTERSELECT_STAGE);
					GMessage()->GetContentsLobby().SendMessage(MessageContentsLobby::UPDATE_UI);
				}
			}
		});
}