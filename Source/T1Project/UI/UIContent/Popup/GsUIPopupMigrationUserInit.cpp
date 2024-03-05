#include "GsUIPopupMigrationUserInit.h"
#include "Components/RichTextBlock.h"

void UGsUIPopupMigrationUserInit::OnClickConfirm()
{
	if (_okCallback) _okCallback();

	Close();
}

void UGsUIPopupMigrationUserInit::OnClickCancel()
{
	Close();
}

void UGsUIPopupMigrationUserInit::OnInputCancel()
{
	OnClickCancel();
}

void UGsUIPopupMigrationUserInit::OnInputOk()
{
	OnClickConfirm();
}

void UGsUIPopupMigrationUserInit::SetInit(InitParam* param, TFunction<void()> okCallback)
{
	_okCallback = okCallback;

	if (param)
	{
		FText ServerMigrate_Popup_ResetDesc;
		FText::FindText(TEXT("ServerMigrateText"), TEXT("ServerMigrate_Popup_ResetDesc"), ServerMigrate_Popup_ResetDesc);

		FString strFormat = ServerMigrate_Popup_ResetDesc.ToString();
		strFormat = strFormat.Replace(TEXT("\r\n"), TEXT("\n"), ESearchCase::IgnoreCase);
		strFormat = strFormat.Replace(TEXT("\\n"), TEXT("\n"), ESearchCase::IgnoreCase);
		strFormat = strFormat.Replace(TEXT("\n\n"), TEXT("\n"), ESearchCase::IgnoreCase);

		FText textDesc = FText::Format(FText::FromString(strFormat), param->_characterCount);

		_richTextNotice->SetText(textDesc);

		//ServerMigrate_Popup_HaveCharacter
		//이전하려는 서버에 <P>{0}명의 캐릭터</>가 있습니다\n
		//서버 이전 시 생성한 캐릭터들을 포함한
		//보유 다이아, 패스 진행도 등 <P>이전하려는 서버의 모든 데이터가 초기화</> 됩니다\n
		//초기화 후 이전 하시겠습니까?
	}
}
