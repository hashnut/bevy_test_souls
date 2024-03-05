// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildDiplomacyStateListEntry.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "UI/UIContent/Popup/GsUIPopupGuildDiplomacyConfirm.h"
#include "Guild/GsGuildHelper.h"
#include "Guild/GsGuildDiplomacyData.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "Net/GsNetSendServiceGuild.h"
#include "DataSchema/Guild/GsSchemaGuildConfig.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"


void UGsUIGuildDiplomacyStateListEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnAccept->OnClicked.AddDynamic(this, &UGsUIGuildDiplomacyStateListEntry::OnClickAccept);
	_btnReject->OnClicked.AddDynamic(this, &UGsUIGuildDiplomacyStateListEntry::OnClickReject);
	_btnCancel->OnClicked.AddDynamic(this, &UGsUIGuildDiplomacyStateListEntry::OnClickCancel);
}

void UGsUIGuildDiplomacyStateListEntry::NativeDestruct()
{
	_stateData = nullptr;

	Super::NativeDestruct();
}

void UGsUIGuildDiplomacyStateListEntry::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (0 < _remainCheckTime)
	{
		// 일정 주기마다 한번씩 갱싱
		_remainCheckTime -= InDeltaTime;
		if (0 >= _remainCheckTime)
		{
			UpdateRemainTime();
		}
	}
}

void UGsUIGuildDiplomacyStateListEntry::SetData(const FGsGuildDiplomacyDataState* InData)
{
	SetBaseData(InData);

	_stateData = InData;

	// 보낸/받은 요청에 따라 UI 처리
	if (InData->IsRequest())
	{
		_switcherState->SetActiveWidgetIndex(1);
		_switcherBtn->SetActiveWidgetIndex(1);
	}
	else
	{
		_switcherState->SetActiveWidgetIndex(0);
		_switcherBtn->SetActiveWidgetIndex(0);
	}

	UpdateRemainTime();
}

void UGsUIGuildDiplomacyStateListEntry::OnClickAccept()
{
	if (nullptr == _stateData)
	{
		return;
	}

	if (false == CheckMaxAllyCount(true))
	{
		return;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupGuildDiplomacyConfirm"));
	if (widget.IsValid())
	{
		if (UGsUIPopupGuildDiplomacyConfirm* popup = Cast<UGsUIPopupGuildDiplomacyConfirm>(widget.Get()))
		{
			GuildDBId guildDBId = _stateData->_guildDBId;
			
			// TEXT: 위 기사단과의 동맹을 수락하시겠습니까?
			FText textMsg;
			FText::FindText(TEXT("GuildText"), TEXT("UI_Diplomacy_Confirm_Accept_Ally"), textMsg);

			popup->SetData(_stateData, textMsg, true, [guildDBId](bool bIsOk)
				{
					if (bIsOk)
					{
						FGsNetSendServiceGuild::SendAcceptAlly(guildDBId);
					}
				});
		}
	}
}

void UGsUIGuildDiplomacyStateListEntry::OnClickReject()
{
	if (nullptr == _stateData)
	{
		return;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupGuildDiplomacyConfirm"));
	if (widget.IsValid())
	{
		if (UGsUIPopupGuildDiplomacyConfirm* popup = Cast<UGsUIPopupGuildDiplomacyConfirm>(widget.Get()))
		{
			GuildDBId guildDBId = _stateData->_guildDBId;

			// TEXT: 위 기사단과의 동맹을 거절하시겠습니까?
			FText textMsg;
			FText::FindText(TEXT("GuildText"), TEXT("UI_Diplomacy_Confirm_Reject_Ally"), textMsg);

			popup->SetData(_stateData, textMsg, true, [guildDBId](bool bIsOk)
				{
					if (bIsOk)
					{
						FGsNetSendServiceGuild::SendCancelRejectAlly(guildDBId);
					}
				});
		}
	}
}

void UGsUIGuildDiplomacyStateListEntry::OnClickCancel()
{
	if (nullptr == _stateData)
	{
		return;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupGuildDiplomacyConfirm"));
	if (widget.IsValid())
	{
		if (UGsUIPopupGuildDiplomacyConfirm* popup = Cast<UGsUIPopupGuildDiplomacyConfirm>(widget.Get()))
		{
			GuildDBId guildDBId = _stateData->_guildDBId;

			// TEXT: 위 기사단에 보낸 동맹 요청을 취소 하시겠습니까?
			FText textMsg;
			FText::FindText(TEXT("GuildText"), TEXT("UI_Diplomacy_Confirm_Cancel_Ally"), textMsg);

			popup->SetData(_stateData, textMsg, true, [guildDBId](bool bIsOk)
				{
					if (bIsOk)
					{
						FGsNetSendServiceGuild::SendCancelRejectAlly(guildDBId);
					}
				});
		}
	}
}

void UGsUIGuildDiplomacyStateListEntry::UpdateRemainTime()
{
	if (nullptr == _stateData)
	{
		_remainCheckTime = 0;
		_tbRemainTime->SetText(FText::GetEmpty());

		return;
	}

	FTimespan remainTime = _stateData->GetRemainTime();

	_checkTermSec = 60;
	if (0 >= remainTime.GetHours())
	{
		_checkTermSec = 1;
	}

	if (0 >= remainTime.GetTotalSeconds())
	{
		_remainCheckTime = 0;
		_tbRemainTime->SetText(FText::GetEmpty());

		// 서버에 리스트 재요청해서 갱신받기
		if (FGsGuildManager* guildMgr = GGuild())
		{
			guildMgr->GetSendController().ResetCoolTime(FGsGuildSendCoolTimeController::EGsCoolTimeType::AllyStateList);
			guildMgr->GetSendController().SendReqAllyStateList();
		}
	}
	else
	{
		_remainCheckTime = _checkTermSec;

		FString strRemainTime;
		FGsTimeStringHelper::GetTimeStringCeil(remainTime, strRemainTime);
		_tbRemainTime->SetText(FText::FromString(strRemainTime));
	}
}

bool UGsUIGuildDiplomacyStateListEntry::CheckMaxAllyCount(bool bInShowMessage)
{
	if (const FGsGuildDataMine* myGuildData = GGuild()->GetMyGuildData())
	{
		if (const FGsSchemaGuildConfig* config = FGsGuildHelper::GetGuildConfig())
		{
			if (config->maxAllyCount > myGuildData->_allyList.Num())
			{
				return true;
			}
		}
	}

	if (bInShowMessage)
	{
		// TEXT: 동맹 기사단 목록이 가득차서 더 이상 동맹 기사단을 등록할 수 없습니다.
		FText textMsg;
		if (FText::FindText(TEXT("GuildText"), TEXT("Alert_Diplomacy_Full_Ally"), textMsg))
		{
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
		}
	}

	return false;
}
