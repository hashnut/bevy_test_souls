// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIHUDDungeonTime.h"
#include "UserWidget.h"
#include "TimerManager.h"
#include "TextBlock.h"

#include "Timespan.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"

#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsUIMessageParam.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/Stage/StageGame/GsStageManagerGame.h"
#include "GameFlow/Stage/GsStageMode.h"

#include "UTIL/GsTimeSyncUtil.h"



void UGsUIHUDDungeonTime::NativeConstruct()
{	
	auto& dungeonHUDTimeHandler = GMessage()->GetDungeonHUDTime();

	/**
	 * Dungeon HUDTime Delegate
	 */
	_listDungeonHUDTimeDelegate.Emplace(dungeonHUDTimeHandler.AddUObject(MessageContentDungeonHUDTime::HUD_TIME,
		this, &UGsUIHUDDungeonTime::SetData));

	_listDungeonHUDTimeDelegate.Emplace(dungeonHUDTimeHandler.AddUObject(MessageContentDungeonHUDTime::ON_HUD_TIME,
		this, &UGsUIHUDDungeonTime::Play));

	_listDungeonHUDTimeDelegate.Emplace(dungeonHUDTimeHandler.AddUObject(MessageContentDungeonHUDTime::PAUSE_HUD_TIME,
		this, &UGsUIHUDDungeonTime::Pause));

	_listDungeonHUDTimeDelegate.Emplace(dungeonHUDTimeHandler.AddUObject(MessageContentDungeonHUDTime::OFF_HUD_TIME,
		this, &UGsUIHUDDungeonTime::Stop));

	/**
	 * Guild Dungeon Delegate
	 */
	MGuildDungeon& guildDungeon = GMessage()->GetGuildDungeon();

	//guild dungeon event
	_listGuildDungeonDelegate.Emplace(guildDungeon.AddUObject(MessageContentGuildDungeon::ENTER,
		this, &UGsUIHUDDungeonTime::Play));

	_listGuildDungeonDelegate.Emplace(guildDungeon.AddUObject(MessageContentGuildDungeon::EXIT,
		this, &UGsUIHUDDungeonTime::Stop));

	// 타이머 등록
	GetWorld()->GetTimerManager().SetTimer(_updateTimerHandle, this, &UGsUIHUDDungeonTime::UpdateTime, _updateSec, true);
	GetWorld()->GetTimerManager().PauseTimer(_updateTimerHandle);

	_isShow = false;
	_isTimeLimit = true;

	// 2022/10/24 PKT - "제한 없음"
	FText::FindText(TEXT("UICommonText"), TEXT("DungeonPlayTimeLimit"), _timeFormat);	
	// 2022/10/24 PKT - "1분 미만"
	FText::FindText(TEXT("FriendText"), TEXT("FriendOffline_Sec"), _offlineTimeFormat);	

	Super::NativeConstruct();
}

void UGsUIHUDDungeonTime::NativeDestruct()
{	
	FGsMessageHolder* msg = GMessage();
	for (auto& iter : _listDungeonHUDTimeDelegate)
	{
		msg->GetDungeonHUDTime().Remove(iter);
	}
	_listDungeonHUDTimeDelegate.Empty();
	
	for (MsgGuildDungeonHandle& iter : _listGuildDungeonDelegate)
	{
		if (iter.Value.IsValid())
		{
			msg->GetGuildDungeon().Remove(iter);
		}		
	}
	_listGuildDungeonDelegate.Empty();

	if (_textTime)
	{
		_textTime->SetText(FText::FromString(TEXT("")));
	}	

	_isShow = false;
	_isTimeLimit = true;

	//타이머 해지
	GetWorld()->GetTimerManager().ClearTimer(_updateTimerHandle);

	Super::NativeDestruct();
}


void UGsUIHUDDungeonTime::SetHUDMode(EGsUIHUDMode InMode)
{
	if (IsHideMode(InMode) || InMode == EGsUIHUDMode::MENU)
	{
		SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	this->SetVisibility(_isShow ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
}

void UGsUIHUDDungeonTime::UpdateTime()
{
	if (false == _isTimeLimit || false == _isShow)
	{
		return;
	}

	FTimespan timeSpan = _endDateTime - FGsTimeSyncUtil::GetServerNowDateTime();
	
	// 2022/10/24 PKT - String 초기화
	PrintTime(timeSpan, _isShowSecond);
}

void UGsUIHUDDungeonTime::PrintTime(const FTimespan& InTimespan, bool InShowSecond)
{
	// 2022/10/24 PKT - String 초기화
	FString timeMessage = _offlineTimeFormat.ToString();
	FGsTimeStringHelper::GetTimeStringDungeon(InTimespan, InShowSecond, timeMessage);

	_textTime->SetText(FText::FromString(timeMessage));
}

void UGsUIHUDDungeonTime::SetData(const IGsMessageParam* inParam)
{
	if (inParam)
	{
		if (const FGsUIMsgParamDungeonTime* message = inParam->Cast<const FGsUIMsgParamDungeonTime>())
		{
			_isTimeLimit = message->_isTimeLimit;
			_endDateTime = message->_time;
			_isShowSecond = message->_isShowSencond;

			if (_isTimeLimit)
			{
				if (message->_isPause)
				{
					PrintTime(_endDateTime - FGsTimeSyncUtil::GetServerNowDateTime(), _isShowSecond);
				}
				else if (GetWorld()->GetTimerManager().IsTimerPaused(_updateTimerHandle))
				{
					GetWorld()->GetTimerManager().UnPauseTimer(_updateTimerHandle);
				}
			}
			else
			{
				FText findText;
				FText::FindText(TEXT("UICommonText"), TEXT("DungeonBaseTimeUnlimit"), findText);
				_textTime->SetText(findText);
			}
		}
	}
}

void UGsUIHUDDungeonTime::Play(const IGsMessageParam*)
{
	_isShow = true;

	if (_isTimeLimit)
	{
		GetWorld()->GetTimerManager().UnPauseTimer(_updateTimerHandle);
	}
}

void UGsUIHUDDungeonTime::Pause(const IGsMessageParam*)
{
	_isShow = true;

	if (_isTimeLimit)
	{
		GetWorld()->GetTimerManager().PauseTimer(_updateTimerHandle);
	}
}

void UGsUIHUDDungeonTime::Stop(const IGsMessageParam*)
{
	_isShow = false;

	GetWorld()->GetTimerManager().PauseTimer(_updateTimerHandle);
}
