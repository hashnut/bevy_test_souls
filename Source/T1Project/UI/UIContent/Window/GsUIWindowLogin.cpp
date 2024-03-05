// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIWindowLogin.h"

#include "Shared/Shared/SharedInclude/SharedDefine.h"
#include "Net/GsNetAutoLogin.h"

#include "EditableTextBox.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIControlLib/ContentWidget/GsCheckBox.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "Option/GsGameUserSettings.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Message/GsMessageStage.h"
#include "T1Project.h"


void UGsUIWindowLogin::NativeConstruct()
{
	Super::NativeConstruct();

	FGsUIHelper::TrayLobbyFadeIn(1.0f);

	_editVidID = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("ID_INPUT")));
	_btnConnect = Cast<UGsButton>(GetWidgetFromName(TEXT("GsButton_Connect")));

	if (nullptr != _checkBoxAuto)
	{
		_checkBoxAuto->OnCheckStateChangedEx.AddDynamic(this, &UGsUIWindowLogin::OnClickAutoLogin);
	}

	if (nullptr != _btnConnect)
	{
		_btnConnect->OnClicked.AddDynamic(this, &UGsUIWindowLogin::OnClickConnect);
	}

	if (nullptr != _editVidID)
	{
		_editVidID->OnTextChanged.AddDynamic(this, &UGsUIWindowLogin::OnTextChangeEvent);
	}

	GSLOG(Warning, TEXT("[TEST_LOGIN] UGsUIWindowLogin NativeConstruct"));
}

void UGsUIWindowLogin::NativeDestruct()
{
	if (nullptr != _checkBoxAuto)
	{
		_checkBoxAuto->OnCheckStateChangedEx.RemoveDynamic(this, &UGsUIWindowLogin::OnClickAutoLogin);
	}

	if (_btnConnect)
	{
		_btnConnect->OnClicked.RemoveDynamic(this, &UGsUIWindowLogin::OnClickConnect);
	}

	if (_editVidID)
	{
		_editVidID->OnTextChanged.RemoveDynamic(this, &UGsUIWindowLogin::OnTextChangeEvent);
	}

	GSLOG(Warning, TEXT("[TEST_LOGIN] UGsUIWindowLogin NativeDestruct Start. _bIsDestructed: %d"), _bIsDestructed);

	Super::NativeDestruct();

	GSLOG(Warning, TEXT("[TEST_LOGIN] UGsUIWindowLogin NativeDestruct End. _bIsDestructed: %d"), _bIsDestructed);
}

void UGsUIWindowLogin::SetData(const FString& InVidID)
{
	FText outText;
	if (false == IsInvalidateVID(FText::FromString(InVidID), outText))
		return;

	if (nullptr != _editVidID)
	{
		_editVidID->SetText(outText);
	}

	if (FGsNetManager* netManager = GNet())
	{
		if(FGsNetAutoLogin* autoLogin = netManager->GetAutoLogin())
			_checkBoxAuto->SetIsSelected(autoLogin->IsAutoLogin());
	}
}

void UGsUIWindowLogin::OnClickConnect()
{
	// 로그인 버튼을 눌렀을 때
	if (nullptr == _editVidID)
		return;

	FText outText;
	if (false == IsInvalidateVID(_editVidID->GetText(), outText))
		return;

	FString vidID = outText.ToString();
	if (vidID.IsEmpty())
	{
		GSLOG(Log, TEXT("[GsUIWindowAccountLogin] Please enter your Vid"));
		FGsUIHelper::PopupOK(FText::FromString(TEXT("VidEmptyError")));
	}
	else
	{
		//FGsNetSendService::SendAuthReqLogin(*_editVidID->GetText().ToString());
		OnClickButton.ExecuteIfBound(*_editVidID->GetText().ToString());
	}	
}

void UGsUIWindowLogin::OnClickAutoLogin(int32 In_indexGroup, bool In_isChecked)
{
	int32 active = static_cast<int32>(In_isChecked);

	if (FGsNetManager* netManager = GNet())
	{
		if (FGsNetAutoLogin* autoLogin = netManager->GetAutoLogin())
			autoLogin->SetAutoLogin(In_isChecked);
	}	
}

bool UGsUIWindowLogin::OnBack()
{
	// 접속 한 서버 나가기 처리 추가
	GNet()->OnExitGame();

	GMessage()->GetStage().SendMessage(MessageStage::ENTER_NETADDRESSSELECT_STAGE);
	return true;
}

void UGsUIWindowLogin::OnTextChangeEvent(const FText& InText)
{
	FString text = InText.ToString();
	FString resultString = text.Replace(TEXT(" "), TEXT(""));
	int32 length = FGsTextUtil::GetStringLengthRegex(resultString);

	// 지정 글자수 이상 입력하지 못하게 막음
	if (MAX_DEVICE_NAME_LEN < length)
	{
		resultString.RemoveAt(resultString.Len() - 1);
	}

	FText resultText = FText::FromString(resultString);
	if (true == resultText.IsEmpty())
	{
		_prevVidText = resultText;
		return;
	}
	else if (false == resultText.IsNumeric())
	{
		_editVidID->SetText(_prevVidText);
		return;
	}

	_prevVidText = resultText; 
	_editVidID->SetText(_prevVidText);
}

bool UGsUIWindowLogin::IsInvalidateVID(const FText& InText, OUT FText& outText)
{
	FString string = InText.ToString();
	FString resultString = string.Replace(TEXT(" "), TEXT(""));
	int32 length = FGsTextUtil::GetStringLengthRegex(resultString);

	// 지정 글자수 이상 입력하지 못하게 막음
	if (MAX_DEVICE_NAME_LEN < length)
	{
		resultString.RemoveAt(resultString.Len() - 1);
	}

	FText resultText = FText::FromString(resultString);
	if (true == resultText.IsEmpty())
	{
		return false;
	}
	else if (false == resultText.IsNumeric())
	{
		return false;
	}

	outText = resultText;
	return true;
}

void UGsUIWindowLogin::RemoveFromParent()
{
	GSLOG(Warning, TEXT("[TEST_LOGIN] UGsUIWindowLogin RemoveFromParent, _bNotDestroy: %d, _bIsDestructed: %d"), _bNotDestroy, _bIsDestructed);

	Super::RemoveFromParent();
}