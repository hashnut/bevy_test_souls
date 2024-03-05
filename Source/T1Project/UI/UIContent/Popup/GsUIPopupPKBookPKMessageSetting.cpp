

#include "GsUIPopupPKBookPKMessageSetting.h"
#include "../PKBook/GsPKBookHelper.h"
#include "../Management/ScopeGame/GsPKBookManager.h"
#include "../Management/ScopeGlobal/GsGameFlowManager.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/GsContentsMode.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/ContentsGame/Hud/PKBook/GsPKBookhandler.h"

#include "EditableTextBox.h"
#include "../UI/UIControlLib/ContentWidget/GsButton.h"
#include "../UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "../Helper/GsUIHelper.h"
#include "../UTIL/GsText.h"

#include "../Net/GsNetSendServiceWorld.h"
#include "T1Project.h"




void UGsUIPopupPKBookPKMessageSetting::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupPKBookPKMessageSetting::OnClickedCancel);
	_btnOK->OnClicked.AddDynamic(this, &UGsUIPopupPKBookPKMessageSetting::OnClickedOK);

	_btnCheck->OnSwitcherButtonSelected.AddDynamic(this, &UGsUIPopupPKBookPKMessageSetting::OnSelectedBtnCheck);
	_btnCheck->OnClicked.AddDynamic(this, &UGsUIPopupPKBookPKMessageSetting::OnClickedBtnCheck);

	_editorTextBoxKillMessage->OnTextChanged.AddDynamic(this, &UGsUIPopupPKBookPKMessageSetting::OnTextChangedKillMessage);
	_editorTextBoxDieMessage->OnTextChanged.AddDynamic(this, &UGsUIPopupPKBookPKMessageSetting::OnTextChangedDieMessage);	
}

void UGsUIPopupPKBookPKMessageSetting::BeginDestroy()
{
	if (_btnCancel)
	{
		_btnCancel->OnClicked.RemoveDynamic(this, &UGsUIPopupPKBookPKMessageSetting::OnClickedCancel);
		_btnCancel = nullptr;
	}

	if (_btnOK)
	{
		_btnOK->OnClicked.RemoveDynamic(this, &UGsUIPopupPKBookPKMessageSetting::OnClickedOK);
		_btnOK = nullptr;
	}

	if (_btnCheck)
	{
		_btnCheck->OnSwitcherButtonSelected.RemoveDynamic(this, &UGsUIPopupPKBookPKMessageSetting::OnSelectedBtnCheck);
		_btnCheck->OnClicked.RemoveDynamic(this, &UGsUIPopupPKBookPKMessageSetting::OnClickedBtnCheck);
		_btnCheck = nullptr;
	}	

	if (_editorTextBoxKillMessage)
	{
		_editorTextBoxKillMessage->OnTextChanged.RemoveDynamic(this, &UGsUIPopupPKBookPKMessageSetting::OnTextChangedKillMessage);
		_editorTextBoxKillMessage = nullptr;
	}

	if (_editorTextBoxDieMessage)
	{
		_editorTextBoxDieMessage->OnTextChanged.RemoveDynamic(this, &UGsUIPopupPKBookPKMessageSetting::OnTextChangedDieMessage);
		_editorTextBoxDieMessage = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIPopupPKBookPKMessageSetting::NativeConstruct()
{
	Super::NativeConstruct();

	FGsPKBookManager* PKBookManager = GsPKBook();
	if (nullptr == PKBookManager)
	{
		GSLOG(Error, TEXT("nullptr == PKBookManager"));
		return;
	}	

	FText message;
	FGsPKBookHelper::FindText(TEXT("Message_Kill_Default"), message);	
	_editorTextBoxKillMessage->SetText(PKBookManager->GetTeasePKMessage());
	_editorTextBoxKillMessage->SetHintText(message);

	FGsPKBookHelper::FindText(TEXT("Message_Die_Default"), message);
	_editorTextBoxDieMessage->SetText(PKBookManager->GetRevengePKMessage());
	_editorTextBoxDieMessage->SetHintText(message);

	_btnCheck->SetIsSelected(PKBookManager->IsPKMessageActive());
}

void UGsUIPopupPKBookPKMessageSetting::NativeDestruct()
{
	Super::NativeDestruct();
}

bool UGsUIPopupPKBookPKMessageSetting::LimitTextLength(const FText& InOrgText, int32 InLimitLength, FText& OutText)
{
	FString strTemp = InOrgText.ToString();
	int32 length = strTemp.Len();

	bool isChanged = false;
	// 지정 글자수 이상 입력하지 못하게 막음
	while (InLimitLength < length)
	{
		strTemp.RemoveAt(strTemp.Len() - 1);
		length = strTemp.Len();
		isChanged = true;
	}

	OutText = FText::FromString(strTemp);
	return isChanged;
}

void UGsUIPopupPKBookPKMessageSetting::OnClickedCancel()
{
	Close();
}

void UGsUIPopupPKBookPKMessageSetting::OnClickedOK()
{
	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	if (nullptr == gameFlow)
	{
		return;
	}

	FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame();
	if (nullptr == contents)
	{
		return;
	}

	FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
	if (nullptr == hud)
	{
		return;
	}

	FGsPKBookHandler* PKBookHabdler = hud->GetPKBookHandler();
	if (nullptr == PKBookHabdler)
	{
		return;
	}

	// 2022/10/06 PKT - Handler 에서 Org Data 와 Diff하여 변경된 설정이 있을 경우 서버에 보낸다.
	bool isSuccess = PKBookHabdler->SavePKSettingMessage(_btnCheck->GetIsSelected(), _editorTextBoxKillMessage->GetText(), _editorTextBoxDieMessage->GetText());

	if (true == isSuccess)
	{	// 2022/10/06 PKT - 저장 성공
		Close();
	}	
}

void UGsUIPopupPKBookPKMessageSetting::OnSelectedBtnCheck(bool InIsSelected)
{
	_editorTextBoxKillMessage->SetIsEnabled(InIsSelected);
	_editorTextBoxDieMessage->SetIsEnabled(InIsSelected);
}

void UGsUIPopupPKBookPKMessageSetting::OnClickedBtnCheck()
{
	_btnCheck->SetIsSelected(false == _btnCheck->GetIsSelected());
}

void UGsUIPopupPKBookPKMessageSetting::OnTextChangedKillMessage(const FText& InText)
{
	FText text;
	const static int32 LIMIT_LENGTH = 20;
	bool isChanged = LimitTextLength(InText, LIMIT_LENGTH, text);
	if (isChanged)
	{
		_editorTextBoxKillMessage->SetText(text);
	}
}

void UGsUIPopupPKBookPKMessageSetting::OnTextChangedDieMessage(const FText& InText)
{
	FText text;
	const static int32 LIMIT_LENGTH = 20;
	bool isChanged = LimitTextLength(InText, LIMIT_LENGTH, text);
	if (isChanged)
	{
		_editorTextBoxDieMessage->SetText(text);
	}
}


void UGsUIPopupPKBookPKMessageSetting::OnInputCancel()
{
	OnClickedCancel();
}