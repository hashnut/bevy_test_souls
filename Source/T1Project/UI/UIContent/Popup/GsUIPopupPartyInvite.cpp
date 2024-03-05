#include "GsUIPopupPartyInvite.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UMG/Public/Components/EditableTextBox.h"

#include "Party/GsPartyFunc.h"

#include "UTIL/GsUIUtil.h"
#include "UTIL/GsText.h"

#include "Management/ScopeGame/GsPartyManager.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "Net/GsNetSendServiceWorld.h"
#include "Net/GsNetSendService.h"

#include "DataCenter/Public/Shared/Shared/SharedInclude/SharedDefine.h"

void UGsUIPopupPartyInvite::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupPartyInvite::OnClickOk);
	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupPartyInvite::OnClickCancel);

	_editInviteeName->OnTextChanged.AddDynamic(this, &UGsUIPopupPartyInvite::OnTextChanged);
}
// ok 클릭
void UGsUIPopupPartyInvite::OnClickOk()
{
	FString sendText = _editInviteeName->GetText().ToString();
	// 1. 일단 prefix 파싱
	// 2. 값이 없으면 나의 홈world id로 보낸다

	auto result = FGsTextUtil::IsValidUserSearchNameOnly(sendText);
	if (FGsTextUtil::EUserNameValid::Valid != result.Get<0>())
	{
		return;
	}


	FString sendName = result.Get<2>().ToString();
	FGsPartyFunc::InviteParty(sendName, result.Get<1>());
	Close();
}

void UGsUIPopupPartyInvite::NativeConstruct()
{
	Super::NativeConstruct();

	// 클리어
	_editInviteeName->SetText(FText::GetEmpty());
}

// cancel 클릭
void UGsUIPopupPartyInvite::OnClickCancel()
{
	Close();
}

void UGsUIPopupPartyInvite::SetInitInvitee(FString inviteeName)
{
	_editInviteeName->SetText(FText::FromString(inviteeName));
}

void UGsUIPopupPartyInvite::OnTextChanged(const FText& InText)
{
	// 인터서버 필드에서 파티 초대 되어서 prefix 까지 포함된 최대 길이로 변경
	int textMaxLen = MAX_CHAR_SERVER_NAME_LEN;
	UGsUIUtil::CheckEditableTextNameLength(_editInviteeName, textMaxLen, true);
}

void UGsUIPopupPartyInvite::OnInputCancel()
{
	OnClickCancel();
}