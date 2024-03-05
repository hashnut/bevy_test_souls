
#include "GsUIPopupPKBookTryToTease.h"
#include "../PKBook/GsPKBookHelper.h"
#include "../PKBook/GsPKBookData.h"
#include "../Management/ScopeGame/GsPKBookManager.h"
#include "../Management/ScopeGlobal/GsGameDataManager.h"
#include "../Management/ScopeGlobal/GsGameFlowManager.h"

#include "../GameFlow/GsGameFlowGame.h"
#include "../GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "../GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "../GameFlow/GameContents/ContentsGame/Hud/PKBook/GsPKBookhandler.h"

#include "TextBlock.h"
#include "RichTextBlock.h"
#include "../UI/UIControlLib/ContentWidget/GsButton.h"
#include "../UI/UIControlLib/ContentWidget/GsCheckBox.h"
#include "UMG/Public/Components/EditableTextBox.h"

#include "../Net/GsNetSendServiceWorld.h"
#include "../T1Project.h"


void UGsUIPopupPKBookTryToTease::TryToAction(PKRecordId InRecordId)
{
	// 2023/5/18 PKT - 금칙어 저장
	if (INVALID_PK_RECORD_ID != InRecordId)
	{
		// 2022/09/21 PKT - 조롱 메세지를 받음.
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
		
		// 2023/1/18 PKT - 설정 변경 되었으면 서버에 저장	
		if (PKBookHabdler->SavedMockerMessage(_checkBoxUseCustomMsg->GetIsSelected(), _textMockeryMessage->GetText()))
		{
			// 2023/1/18 PKT - 조롱 메세지를 보낸다.
			FText sendMessage = FText::GetEmpty();
			if (_checkBoxUseCustomMsg->GetIsSelected())
			{
				sendMessage = (_textMockeryMessage->GetText().IsEmpty()) ? _textMockeryMessage->HintText : _textMockeryMessage->GetText();
			}

			FGsNetSendServiceWorld::Send_ReqPKBookSendTeaseMessage(InRecordId, sendMessage);
		}
	}
	
	Super::TryToAction(InRecordId);
}

void UGsUIPopupPKBookTryToTease::BeginDestroy()
{
	if (_textMockeryMessage)
	{
		_textMockeryMessage->OnTextChanged.RemoveDynamic(this, &UGsUIPopupPKBookTryToTease::OnTextChangedMsg);
		_textMockeryMessage = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIPopupPKBookTryToTease::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_textMockeryMessage->OnTextChanged.AddDynamic(this, &UGsUIPopupPKBookTryToTease::OnTextChangedMsg);
}

void UGsUIPopupPKBookTryToTease::OnTextChangedMsg(const FText& InText)
{
	FString strTemp = InText.ToString();
	int32 length = strTemp.Len();

	// 지정 글자수 이상 입력하지 못하게 막음
	static const int32 maxLength = 20;
	if (maxLength < length)
	{
		strTemp.RemoveAt(strTemp.Len() - 1);
		_textMockeryMessage->SetText(FText::FromString(strTemp));
	}
}


void UGsUIPopupPKBookTryToTease::SetData(PKRecordId InRecordId, const struct FGsPKRecordingContributorData* InData, bool InUseMockeryMessage, const FText& InMockeryMessage)
{
	Super::SetData(InRecordId, InData);

	if (INVALID_PK_RECORD_ID == _recordId)
	{
		GSLOG(Error, TEXT("INVALID_PK_RECORD_ID == _recordId"));
		return;
	}

	FGsGameDataManager* gameDataManager = GGameData();
	if (nullptr == gameDataManager)
	{
		GSLOG(Error, TEXT("nullptr == gameDataManager"));
		return;
	}

	FText format;
	FGsPKBookHelper::FindText(TEXT("Popup_Try_Tease_Message"), format);

	// 2022/09/28 PKT - "{0}님이 {1}님을 처치하였습니다.
	FText targetName = FText::FromString(FGsTextUtil::CombineUserAndPrefix(InData->_name, InData->_homeWorldId));
	FText message = FText::Format(format, { FText::FromString(gameDataManager->GetUserData()->mUserName), targetName });
	_richTextMessage->SetText(message);

	_checkBoxUseCustomMsg->SetIsSelected(InUseMockeryMessage);
	_textMockeryMessage->SetText(InMockeryMessage);
}