#include "GsUIPopupChatReport.h"
#include "Management/ScopeGame/GsChatManager.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "MultiLineEditableText.h"
#include "EditableTextBox.h"
#include "TextBlock.h"

#include "Net/GsNetSendService.h"

void UGsUIPopupChatReport::NativeOnInitialized()
{
	//1개 이상 선택이면 나중에 지워줌
	_toggleGroup.AddToggleWidget(_checkObscene);
	_toggleGroup.AddToggleWidget(_checkRepeat);
	_toggleGroup.AddToggleWidget(_checkTradeAdv);
	_toggleGroup.AddToggleWidget(_checkInsult);

	_toggleGroup.OnSelectChanged.BindUObject(this, &UGsUIPopupChatReport::OnCheckButtonChanged);

	_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupChatReport::OnClickClose);
	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupChatReport::OnClickCancel);
	_btnReport->OnClicked.AddDynamic(this, &UGsUIPopupChatReport::OnClickReport);

	//_editEnterText->OnTextChanged.AddDynamic(this, &UGsUIPopupChatReport::OnChangedTitleText);
	//_multiEditContents->OnTextChanged.AddDynamic(this, &UGsUIPopupChatReport::OnChangedContentText);

	Super::NativeOnInitialized();
}

void UGsUIPopupChatReport::OnClickClose()
{
	Close();
}
void UGsUIPopupChatReport::OnClickCancel()
{
	Close();
}

void UGsUIPopupChatReport::OnClickReport()
{
	int index = 0;
	for (auto e : { _checkObscene, _checkRepeat, _checkTradeAdv, _checkInsult })
	{
		if (e->GetIsSelected()) break;
		index++;
	}
	if (index >= 4) return;

	FText name = _editEnterText->GetText();
	FText content = _multiEditContents->GetText();

	FGsNetSendService::SendReqReportChat(name.ToString(), content.ToString(), index);
	Close();
}

void UGsUIPopupChatReport::InitializeData(PopupInitData* initParam)
{
	if (initParam)
	{
		_editEnterText->SetText(FText::FromString(initParam->_nickName));
		_multiEditContents->SetText(FText::FromString(initParam->_chatText));

		_btnReport->SetIsEnabled(false);
	}
	_selectedCheckIndex = -1;

	_checkObscene->SetIsSelected(false);
	_checkRepeat->SetIsSelected(false);
	_checkTradeAdv->SetIsSelected(false);
	_checkInsult->SetIsSelected(false);

	_toggleGroup.SetSelectedIndex(0, true);
}

//void UGsUIPopupChatReport::OnChangedTitleText(const FText& OriginalText)
//{
//	auto str = OriginalText.ToString();
//	auto arr = str.GetCharArray();
//
//	if (str.Len() > MAX_NICKNAME)
//	{
//		str = str.Left(MAX_NICKNAME);
//		_editEnterText->SetText(FText::FromString(str));
//	}
//}
//
//void UGsUIPopupChatReport::OnChangedContentText(const FText& OriginalText)
//{
//	auto str = OriginalText.ToString();
//	auto arr = str.GetCharArray();
//
//	if (str.Len() > MAX_CONTENTS)
//	{
//		str = str.Left(MAX_CONTENTS);
//		_multiEditContents->SetText(FText::FromString(str));
//	}
//}

void UGsUIPopupChatReport::OnCheckButtonChanged(int32 index)
{
	_selectedCheckIndex = index;

	_btnReport->SetIsEnabled(true);
}

void UGsUIPopupChatReport::OnInputCancel()
{
	OnClickClose();
}