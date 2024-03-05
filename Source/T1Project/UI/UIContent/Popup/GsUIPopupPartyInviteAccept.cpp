#include "GsUIPopupPartyInviteAccept.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "Management/ScopeGlobal/GsGameDataManager.h"

#include "Management/ScopeGame/GsPartyManager.h"

#include "Party/GsPartyInviterInfo.h"
#include "Party/GsPartyFunc.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "Net/GsNetSendServiceWorld.h"
#include "Net/GsNetSendService.h"

#include "ContentsServer/GsContentsServerHelper.h"

#include "UTIL/GsText.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Core/Public/Delegates/Delegate.h"

#include "UMG/Public/Components/ProgressBar.h"

void UGsUIPopupPartyInviteAccept::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupPartyInviteAccept::OnClickOk);
	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupPartyInviteAccept::OnClickCancel);
}

// ok 클릭
void UGsUIPopupPartyInviteAccept::OnClickOk()
{
	FGsNetSendService::SendPartyConfirmInvitation(true);
	Close();
}
// cancel 클릭
void UGsUIPopupPartyInviteAccept::OnClickCancel()
{
	FGsNetSendService::SendPartyConfirmInvitation(false);
	Close();
}

void UGsUIPopupPartyInviteAccept::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGsUIPopupPartyInviteAccept::NativeConstruct()
{	
	Super::NativeConstruct();

	FGsPartyInviterInfo info = GSParty()->GetPartyInviteInfo();

	// 시작 시간 가져오기
	_startTime = info.GetStartTimeTick();

	// 이름
	FString name = info.GetName();

	WorldId homeWorldId = info.GetHomeWorldId();

	// 인터서버 필드면 prefix 붙인다	
	FString newName = FGsPartyFunc::GetPartyMemberName(name, homeWorldId);

	// 레벨
	int level = info.GetLevel();

	FString nameText;
	if (level != INVALID_LEVEL)
	{
		nameText = FString::Printf(TEXT("Lv.%d %s"), level, *newName);
	}
	else
	{
		nameText = FString::Printf(TEXT("Lv.?? %s"), *newName);
	}

	_textInviterInfo = FText::FromString(nameText);

	_isTimeover = false;

#ifdef USE_CHANNEL_MOVE_NOTI
	int currentChannel = GGameData()->GetCurrentChannel();

	FText findText;
	// 초대자가 내채널이랑 다르면
	if (info.GetChannelId() != currentChannel)
	{		
		FText::FindText(TEXT("PartyText"), TEXT("PartyChannelInvitation"), findText);
	}
	else
	{
		FText::FindText(TEXT("PartyText"), TEXT("PartyInvitationProgress"), findText);
	}

	_textInviteDesc = findText;
#endif
}

void UGsUIPopupPartyInviteAccept::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (_isTimeover == true)
	{
		return;
	}

	float rate = GetInviteWaitTimeRate();

	_progressBarInviteWait->SetPercent(rate);

	if (rate >= 1.0f)
	{
		_isTimeover = true;
		
		// 취소랑 같다
		OnClickCancel();
	}
}


// 안드로이드 백키 처리
bool UGsUIPopupPartyInviteAccept::OnBack()
{
	// 거절 요청
	FGsNetSendService::SendPartyConfirmInvitation(false);
	return Super::OnBack();
}
// 초대 대기 남은시간 rate 값 구하기
float UGsUIPopupPartyInviteAccept::GetInviteWaitTimeRate()
{
	int64 nowTick = FDateTime::UtcNow().GetTicks();
	int64 diffTick = nowTick - _startTime;

	static float maxTimeTick = 
		GData()->GetGlobalData()->_partyInviteWaitSecond * ETimespan::TicksPerSecond;

	float rate = diffTick / maxTimeTick;
	return rate;
}