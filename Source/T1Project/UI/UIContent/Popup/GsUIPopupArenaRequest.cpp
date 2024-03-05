#include "GsUIPopupArenaRequest.h"

#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

#include "DataSchema/Arena/GsSchemaArenaData.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UTIL/GsTableUtil.h"

#include "Message/MessageParam/GsMessageParam.h"
#include "Management/GsMessageHolder.h"

#include "Management/ScopeGame/GsArenaManager.h"

#include "Net/GsNetSendServiceWorld.h"


void UGsUIPopupArenaRequest::BeginDestroy()
{
	if (_btnClose)
	{
		_btnClose->OnClicked.RemoveDynamic(this, &UGsUIPopupArenaRequest::OnClickClose);
	}
	_btnClose = nullptr;

	Super::BeginDestroy();
}

void UGsUIPopupArenaRequest::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupArenaRequest::OnClickClose);
}

void UGsUIPopupArenaRequest::NativeConstruct()
{
	Super::NativeConstruct();

	if (const FGsSchemaArenaData* arenaData = GSArena()->GetArenaData())
	{
		_maxRequestTime = arenaData->arenaRequestTime;
	}

	FGsMessageHolder* msg = GMessage();

	msg->GetUI().SendMessage(MessageUI::OPEN_MIDDLE_SMALL_POPUP, nullptr);

	_msgArenaHandlerList.Emplace(msg->GetArena().AddUObject(MessageContentArena::CLOSE_ARENA_UI,
		this, &UGsUIPopupArenaRequest::OnCloseArenaUI));

	_msgUIHandlerList.Emplace(msg->GetUI().AddUObject(MessageUI::OPEN_MIDDLE_SMALL_POPUP,
		this, &UGsUIPopupArenaRequest::OnOpenMiddleSmallPopup));

	_isTimeOver = true;
}

void UGsUIPopupArenaRequest::NativeDestruct()
{
	FGsMessageHolder* msg = GMessage();
	
	for (TPair<MessageContentArena, FDelegateHandle>& msgArenaHandler : _msgArenaHandlerList)
	{
		msg->GetArena().Remove(msgArenaHandler);
	}
	_msgArenaHandlerList.Empty();

	for (TPair<MessageStage, FDelegateHandle>& msgStageHandler : _msgStageHandlerList)
	{
		msg->GetStageParam().Remove(msgStageHandler);
	}
	_msgStageHandlerList.Empty();

	for (MsgUIHandle& handler : _msgUIHandlerList)
	{
		msg->GetUI().Remove(handler);
	}
	_msgUIHandlerList.Empty();

	Super::NativeDestruct();
}

void UGsUIPopupArenaRequest::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (_isTimeOver)
		return;

	double rate = GetWaitTimeRate();
	_progressBarWait->SetPercent(rate);

	if (rate >= 1.0f)
	{
		_isTimeOver = true;
		
		OnClickClose();
	}
}

bool UGsUIPopupArenaRequest::OnBack()
{
	if (FGsArenaManager* arenaManager = GSArena())
	{
		if (arenaManager->GetArenaOutState() == EGsArenaOutState::REQUEST)
		{
			FGsNetSendServiceWorld::SendReqDuelInviteCancel();
			arenaManager->ResetTargetInfo();
		}
	}

	return Super::OnBack();
}

void UGsUIPopupArenaRequest::OnClickClose()
{
	if (FGsArenaManager* arenaManager = GSArena())
	{
		if (arenaManager->GetArenaOutState() == EGsArenaOutState::REQUEST)
		{
			FGsNetSendServiceWorld::SendReqDuelInviteCancel();
			arenaManager->ResetTargetInfo();
		}
	}

	Close();
}

void UGsUIPopupArenaRequest::OnCloseArenaUI(const IGsMessageParam* InParam)
{
	Close();
}

void UGsUIPopupArenaRequest::OnOpenMiddleSmallPopup(const IGsMessageParam* param)
{
	OnClickClose();
}

double UGsUIPopupArenaRequest::GetWaitTimeRate()
{
	double currentTime = FApp::GetCurrentTime();
	double diffTime = currentTime - _startTime;

	if (_maxRequestTime != 0)
	{
		return diffTime / (double)_maxRequestTime;
	}
	return 0;
}

void UGsUIPopupArenaRequest::InitializeUI(ArenaUserData InTargetUserData)
{
	FText levelText;
	FText::FindText(TEXT("UICommonText"), TEXT("LevelShort"), levelText);

	FString strName;
	if (InTargetUserData._targetVisible)
	{
		strName = FString::Printf(TEXT("%s%d %s"), *(levelText.ToString()), InTargetUserData._targetLevel, *InTargetUserData._targetUserName);
	}
	else
	{
		strName = FString::Printf(TEXT("%s?? %s"), *(levelText.ToString()), *InTargetUserData._targetUserName);
	}
	_textBlockName->SetText(FText::FromString(strName));

	_startTime = FApp::GetCurrentTime();
	_isTimeOver = false;
}