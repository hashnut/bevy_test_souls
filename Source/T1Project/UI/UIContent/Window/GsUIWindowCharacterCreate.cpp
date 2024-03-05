// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIWindowCharacterCreate.h"
#include "Net/GsNetSendService.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeLobby/GsLobbyCharacterManager.h"
#include "Management/ScopeLobby/GsLobbyDataManager.h"

#include "UI/UIContent/Popup/GsUIPopupYesNo.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsToggleButton.h"

#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "T1Project.h"

#include "Message/GsMessageContents.h"
#include "Message/GsMessageStage.h"
#include "UMG/Public/Components/TextBlock.h"


void UGsUIWindowCharacterCreate::BeginDestroy()
{
	Super::BeginDestroy();
}

void UGsUIWindowCharacterCreate::NativeOnInitialized()
{
	_helpContentsKey = EGsHelpContents::CharacterCreate;

	_btnBack->OnClicked.AddDynamic(this, &UGsUIWindowCharacterCreate::OnClickBack);
	_btnPreset1->OnClicked.AddDynamic(this, &UGsUIWindowCharacterCreate::OnClickPreset1);
	_btnPreset2->OnClicked.AddDynamic(this, &UGsUIWindowCharacterCreate::OnClickPreset2);
	_btnPreset3->OnClicked.AddDynamic(this, &UGsUIWindowCharacterCreate::OnClickPreset3);
	_btnPreset4->OnClicked.AddDynamic(this, &UGsUIWindowCharacterCreate::OnClickPreset4);

	_btnCustomizing->OnClicked.AddDynamic(this, &UGsUIWindowCharacterCreate::OnClickCustomizing);
	_btnCustomizingCancel->OnClicked.AddDynamic(this, &UGsUIWindowCharacterCreate::OnClickCancelChangeCustomizing);

	Super::NativeOnInitialized();
}

void UGsUIWindowCharacterCreate::NativeConstruct()
{
	Super::NativeConstruct();

	if (_onClickCustomizing.IsBound())
	{
		_onClickCustomizing.ExecuteIfBound();
	}

	ShowHideCustomizingBtn(false);

	if (UGsLobbyDataManager* lobbyDataMgr = LSLobbyData())
	{
		// 외형변경 중인가?
		bool isChangeCustomizing = lobbyDataMgr->IsChangeCustomizing();
		_btnCustomizingCancel->SetVisibility(
			isChangeCustomizing ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

		FText findText;
		if (true == isChangeCustomizing)
		{
			FText::FindText(TEXT("LobbyText"), TEXT("Button_Change"), findText);
		}
		else
		{
			FText::FindText(TEXT("CustomizeText"), TEXT("BtnCustomizing"), findText);
		}

		_textBtnCustomizing->SetText(findText);
	}
}

void UGsUIWindowCharacterCreate::NativeDestruct()
{
	_selectedPresetIndex = 0;

	if (_onClickCustomizing.IsBound())
	{
		_onClickCustomizing.Unbind();
	}

	Super::NativeDestruct();
}

void UGsUIWindowCharacterCreate::OnClickCustomizing()
{
	if (_selectedPresetIndex == 0)
	{
		// 티커출력
		FText findText;
		FText::FindText(TEXT("UICommonText"), TEXT("GenderSelectRequest"), findText);
		FGsUIHelper::TrayMessageTicker(findText);

		return;
	}

	FGsUIHelper::TrayFadeIn(1.5f);

	// 외형변경 State로 이동
	if (FGsMessageHolder* msg = GMessage())
	{
		msg->GetStage().SendMessage(MessageStage::ENTER_CHARACTER_CUSTOMIZING);
	}
}

bool UGsUIWindowCharacterCreate::OnClickBackProc()
{
	if (UGsLobbyDataManager* lobbyDataMgr = LSLobbyData())
	{
		// 외형변경 중인가?
		bool isChangeCustomizing = lobbyDataMgr->IsChangeCustomizing();
		if (isChangeCustomizing)
		{
			OnClickCancelChangeCustomizing();
			return true;
		}
	}

	// 캐릭터 선택 Stage로 이동
	UGsUIManager* uiMgr = GUI();
	if (nullptr == uiMgr)
	{
		return true;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->OpenAndGetWidget(FName(TEXT("PopupYesNo")));
	if (false == widget.IsValid())
	{
		return true;
	}

	UGsUIPopupYesNo* uiPopup = Cast<UGsUIPopupYesNo>(widget.Get());
	if (nullptr == uiPopup)
	{
		return true;
	}

	// TEXT: 캐릭터 생성을 취소하시겠습니까?
	FText text;
	FText::FindText(TEXT("UICommonText"), TEXT("Cancel_CharacterCreation"), text);

	uiPopup->SetData(text, [this](bool bYes)
	{
		if (bYes)
		{
			Super::OnBack();

			if (UGsLobbyDataManager* lobbyDataMgr = LSLobbyData())
			{
				lobbyDataMgr->SetIsBackToCharacterSelect(true);
				FGsUIHelper::TrayFadeIn(1.5f);

				GMessage()->GetStage().SendMessage(MessageStage::ENTER_CHARACTERSELECT_STAGE);
				GMessage()->GetContentsLobby().SendMessage(MessageContentsLobby::UPDATE_UI);
			}
		}
	});
	return false;
}

bool UGsUIWindowCharacterCreate::OnBack()
{
	return OnClickBackProc();
}

void UGsUIWindowCharacterCreate::OnClickPreset1()
{
	if (1 == _selectedPresetIndex)
		return;
	
	_selectedPresetIndex = 1;

	if (_onSelectPreset.IsBound())
	{
		_onSelectPreset.ExecuteIfBound(_selectedPresetIndex);
	}

	ShowHideCustomizingBtn(true);
}

void UGsUIWindowCharacterCreate::OnClickPreset2()
{
	if (2 == _selectedPresetIndex)
		return;

	_selectedPresetIndex = 2;

	if (_onSelectPreset.IsBound())
	{
		_onSelectPreset.ExecuteIfBound(_selectedPresetIndex);
	}

	ShowHideCustomizingBtn(true);
}

void UGsUIWindowCharacterCreate::OnClickPreset3()
{
	if (3 == _selectedPresetIndex)
		return;

	_selectedPresetIndex = 3;

	if (_onSelectPreset.IsBound())
	{
		_onSelectPreset.ExecuteIfBound(_selectedPresetIndex);
	}

	ShowHideCustomizingBtn(true);
}

void UGsUIWindowCharacterCreate::OnClickPreset4()
{
	if (4 == _selectedPresetIndex)
		return;

	_selectedPresetIndex = 4;

	if (_onSelectPreset.IsBound())
	{
		_onSelectPreset.ExecuteIfBound(_selectedPresetIndex);
	}

	ShowHideCustomizingBtn(true);
}

void UGsUIWindowCharacterCreate::ShowHideCustomizingBtn(bool inIsShow)
{
	if (_btnCustomizing)
	{
		_btnCustomizing->SetIsEnabled(inIsShow);
	}
}

// 외형 변경 취소
void UGsUIWindowCharacterCreate::OnClickCancelChangeCustomizing()
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