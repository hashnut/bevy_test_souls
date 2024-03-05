#include "GsUIPopupArenaAccept.h"

#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

#include "DataSchema/Arena/GsSchemaArenaData.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "UTIL/GsTableUtil.h"

#include "Message/MessageParam/GsMessageParam.h"
#include "Management/GsMessageHolder.h"

#include "Management/ScopeGame/GsArenaManager.h"

#include "Net/GsNetSendServiceWorld.h"


void UGsUIPopupArenaAccept::BeginDestroy()
{
	if (_btnCancel)
	{
		_btnCancel->OnClicked.RemoveDynamic(this, &UGsUIPopupArenaAccept::OnClickCancel);
	}
	_btnCancel = nullptr;

	if (_btnAccept)
	{
		_btnAccept->OnClicked.RemoveDynamic(this, &UGsUIPopupArenaAccept::OnClickAccept);
	}
	_btnAccept = nullptr;

	Super::BeginDestroy();
}

void UGsUIPopupArenaAccept::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupArenaAccept::OnClickCancel);
	_btnAccept->OnClicked.AddDynamic(this, &UGsUIPopupArenaAccept::OnClickAccept);
}

void UGsUIPopupArenaAccept::NativeConstruct()
{
	Super::NativeConstruct();

	if (const FGsSchemaArenaData* arenaData = GSArena()->GetArenaData())
	{
		_maxRequestTime = arenaData->arenaRequestTime;
	}

	FGsMessageHolder* msg = GMessage();

	msg->GetUI().SendMessage(MessageUI::OPEN_MIDDLE_SMALL_POPUP, nullptr);

	_msgArenaHandlerList.Emplace(msg->GetArena().AddUObject(MessageContentArena::CLOSE_ARENA_UI,
		this, &UGsUIPopupArenaAccept::OnCloseArenaUI));

	_msgUIHandlerList.Emplace(msg->GetUI().AddUObject(MessageUI::OPEN_MIDDLE_SMALL_POPUP,
		this, &UGsUIPopupArenaAccept::OnOpenMiddleSmallPopup));

	_isTimeOver = true;
}

void UGsUIPopupArenaAccept::NativeDestruct()
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

void UGsUIPopupArenaAccept::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (_isTimeOver)
		return;

	double rate = GetWaitTimeRate();
	_progressBarWait->SetPercent(rate);

	if (rate >= 1.0f)
	{
		_isTimeOver = true;
		
		OnClickCancel();
	}
}

bool UGsUIPopupArenaAccept::OnBack()
{
	if (FGsArenaManager* arenaManager = GSArena())
	{
		if (arenaManager->GetArenaHandler()->TryReqDuelAccept(false))
		{
			FText findText;
			FText::FindText(TEXT("ArenaText"), TEXT("ArenaDenyTicker"), findText);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		}

		arenaManager->ResetTargetInfo();
	}

	return Super::OnBack();
}

void UGsUIPopupArenaAccept::OnClickCancel()
{
	if (FGsArenaManager* arenaManager = GSArena())
	{
		if (arenaManager->GetArenaHandler()->TryReqDuelAccept(false))
		{
			FText findText;
			FText::FindText(TEXT("ArenaText"), TEXT("ArenaDenyTicker"), findText);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		}

		arenaManager->ResetTargetInfo();
	}

	Close();
}

void UGsUIPopupArenaAccept::OnClickAccept()
{
	if (FGsArenaManager* arenaManager = GSArena())
	{
		arenaManager->GetArenaHandler()->TryReqDuelAccept(true);
	}
	Close();
}

void UGsUIPopupArenaAccept::OnCloseArenaUI(const IGsMessageParam* InParam)
{
	Close();
}

void UGsUIPopupArenaAccept::OnOpenMiddleSmallPopup(const IGsMessageParam* param)
{
	OnClickCancel();
}

double UGsUIPopupArenaAccept::GetWaitTimeRate()
{
	double currentTime = FApp::GetCurrentTime();
	double diffTime = currentTime - _startTime;

	if (_maxRequestTime != 0)
	{
		return diffTime / (double)_maxRequestTime;
	}
	return 0;
}

void UGsUIPopupArenaAccept::InitializeUI(Level InLevel, FString InUserName, bool InIsVisible)
{
	FText levelText;
	FText::FindText(TEXT("UICommonText"), TEXT("LevelShort"), levelText);

	FString strName;
	if (InIsVisible)
	{
		strName = FString::Printf(TEXT("%s%d %s"), *(levelText.ToString()), InLevel, *InUserName);
	}
	else
	{
		strName = FString::Printf(TEXT("%s?? %s"), *(levelText.ToString()), *InUserName);
	}
	_textBlockName->SetText(FText::FromString(strName));

	_startTime = FApp::GetCurrentTime();
	_isTimeOver = false;
}