// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIHUDBoss.h"
#include "Kismet/GameplayStatics.h"
#include "WidgetSwitcher.h"
#include "TextBlock.h"
#include "Cheat/GsCheatManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UILib/Define/GsUIDefine.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Boss/GsFieldBossHandler.h"
#include "GameObject/Data/GsGameObjectDataLocalPlayer.h"
#include "PvP/GsSchemaPVPAlignmentData.h"
#include "GsUIHUDMinimap.h"
#include "Unlock/GsUnlockDefine.h"
#include "DataSchema/Map/FieldBoss/GsSchemaFieldBossData.h"
#include "Data/GsDataContainManager.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateBoss.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Message/MessageParam/GsBossMessageParam.h"
#include "GameObject/Boss/GsWorldBossHandler.h"
#include "DataSchema/WorldBoss/GsSchemaWorldBossData.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

void UGsUIHUDBoss::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_button->OnClicked.AddDynamic(this, &UGsUIHUDBoss::OnClickButton);
}

void UGsUIHUDBoss::NativeConstruct()
{
	Super::NativeConstruct();

	FText::FindText(FIELD_BOSS_NAMESPACE_TEXT_KEY, FIELD_BOSS_SPAWN_TIME_TEXT_KEY, _spawnTimeTextFromat);

	MBoss& msgBoss = GMessage()->GetBoss();

	//필드 내 발생하는 이벤트
	//월드 보스 오픈(스폰 상태는 아님) 스폰전 시간은 보임
	_msgBossHandleList.Emplace(msgBoss.AddUObject(MessageBoss::INFORM_WORLD_BOSS_OPEN, this, &UGsUIHUDBoss::OnWorldBossRoomReady));
	
	//던전 내 발생하는 이벤트
	//월드 보스 오픈(스폰 상태는 아님) 스폰전 시간은 보임
	_msgBossHandleList.Emplace(msgBoss.AddUObject(MessageBoss::WORLD_BOSS_SPAWN_READY, this, &UGsUIHUDBoss::OnWorldBossSpawnReadyState));
	//월드 보스 스폰되면 Hide
	_msgBossHandleList.Emplace(msgBoss.AddUObject(MessageBoss::ALARM_WORLD_BOSS_SPAWN, this, &UGsUIHUDBoss::OnWorldBossSpawn));

	_readyTime = GData()->GetGlobalData()->_fieldBossReadyMinutes;	
	_bossState = EGsBossIconStateType::Deactive;
	_time = 0;	

	InvalidVisibility();
}

void UGsUIHUDBoss::NativeDestruct()
{
	MBoss& msgBoss = GMessage()->GetBoss();
	for (MsgBossHandle& msgBossHandle : _msgBossHandleList)
	{
		msgBoss.Remove(msgBossHandle);
	}
	_msgBossHandleList.Empty();

	_dateTimer.Stop(GetWorld());
	_dateTimer.Clear();

	Super::NativeDestruct();
}

void UGsUIHUDBoss::InvalidTime()
{
	FDateTime nowTime = FGsTimeSyncUtil::GetClientNowDateTime();

#if FIELD_BOSS_DEBUG
	GSLOG(Error, TEXT("field boss - spawn time : %s"), *_endTime.ToString());
	GSLOG(Error, TEXT("field boss - current time : %s"), *nowTime.ToString());
#endif

	//spawn 시간이 0으로 오면 스폰된 상태이다. (시간으로 치면 DateTime 변환시 1970.01.01.00:00)
	bool isSpawn = nowTime >= _endTime;
	if (isSpawn)
	{
		_bossState = EGsBossIconStateType::Spawn;
	}
	else
	{
		FTimespan remainDateTime = _endTime - nowTime;

#if FIELD_BOSS_DEBUG
		GSLOG(Log, TEXT("field boss - time span : %s"), *remainDateTime.ToString());
#endif
		//현재 시간과 스폰 시간을 뺀 차이에서 ReadyTime 이상이면 Deactive, 이하면 Ready
		if (remainDateTime.GetTotalMinutes() > _readyTime)
		{
			_bossState = EGsBossIconStateType::Deactive;
		}
		else
		{
#if WORLD_BOSS_DEBUG
			GSLOG(Log, TEXT("worldboss icon - boss spawn time : %s\tnow time : %s"), *_endTime.ToString(), *nowTime.ToString());
#endif

			_bossState = EGsBossIconStateType::Ready;
			_dateTimer.Start(GetWorld(), _endTime, 0.5f
				, [this]() {InvalidRemainTime();}
				, [this]() {
					_bossState = EGsBossIconStateType::Deactive;
					InvalidVisibility();
				});
			InvalidRemainTime();
		}
	}

	InvalidVisibility();
}

void UGsUIHUDBoss::InvalidHudState()
{
	if (EGsBossIconStateType::Ready == _bossState)
	{
		InvalidRemainTime();
	}
	else if (EGsBossIconStateType::Deactive == _bossState)
	{
		if (_endTime > FGsTimeSyncUtil::GetServerNowDateTimeUTC())
		{
			_timeSpan = _endTime - FGsTimeSyncUtil::GetServerNowDateTimeUTC();

			if (_timeSpan.GetTotalSeconds() <= 0)
			{
				_bossState = EGsBossIconStateType::Spawn;
			}
			else if(_timeSpan.GetTotalMinutes() < _readyTime)
			{
				_bossState = EGsBossIconStateType::Ready;

			}

			InvalidVisibility();
		}
	}
}

void UGsUIHUDBoss::InvalidRemainTime()
{
	_timeSpan = _dateTimer.GetRemainTime();
	FGsTimeStringHelper::GetTimeStringHMS(_timeSpan, _remainTime);
	_remainTimeText->SetText(FText::FromString(_remainTime));
}

void UGsUIHUDBoss::OnClickButton()
{
	//move to worldmap ui at current map
	if (false == GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::BOSS))
	{
		GSUnlock()->ShowLockTicker(EGsUnlockContentsType::BOSS);
		return;
	}

	const FGsSchemaWorldBossData* worldBossData = FGsWorldBossHandler::FindWorldBossData(_bossId);
	if (worldBossData->unlockContentsId.GetRow()
		&& GSUnlock()->IsContentsUnlock(worldBossData->unlockContentsId.GetRow()->id))
	{
		int32 id = worldBossData->unlockContentsId.GetRow()->id;

		FText message;
		GSUnlock()->MakeStringUnLockCondition(id, worldBossData->unlockContentsId.GetRow()->lockType, message);

		FGsUIHelper::TrayMessageTicker(message);		
		return;
	}
			
	GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_BOSS);
	if (GUI()->IsActiveWidget(WINDOW_BOSS_INFO))
	{
		FGsGameStateBoss* state = FGsGameStateBoss::GetInstance();
		if (state)
		{
			state->SelectedWorldBoss(_bossId);
		}
	}
}

//로컬 플레이어 스폰 전에 보스 정보 패킷이 온다
void UGsUIHUDBoss::OnWorldBossRoomReady(const struct IGsMessageParam* InParam)
{
	const FGsWorldBossRoomReadyParam* param = InParam->Cast<const FGsWorldBossRoomReadyParam>();
	if (nullptr == param)
	{
		return;
	}

	_endTime = param->bossSpawnTime;
	_bossId = param->bossId;

	InvalidTime();
}

void UGsUIHUDBoss::OnWorldBossSpawnReadyState(const struct IGsMessageParam* InParam)
{
	const FGsWorldBossRoomReadyParam* param = InParam->Cast<const FGsWorldBossRoomReadyParam>();
	if (nullptr == param)
	{
		return;
	}

	_endTime = param->bossSpawnTime;
	_bossId = param->bossId;

	InvalidTime();
}

//로컬 플레이어 스폰 후 보스 스폰 메시지 내려온다
void UGsUIHUDBoss::OnWorldBossSpawn(const IGsMessageParam* InParam)
{
	const FGsWorldBossSpawnParam* param = InParam->Cast<const FGsWorldBossSpawnParam>();
	if (nullptr == param)
	{
		return;
	}

	_bossState = EGsBossIconStateType::Deactive;

	InvalidVisibility();
}

void UGsUIHUDBoss::InvalidVisibility()
{
	_bossWidgetSwitcher->SetActiveWidgetIndex((int32)_bossState);

	//스폰되어 있거나 비활성화 되어 있으면 꺼진다. 
	switch (_bossState)
	{
	case EGsBossIconStateType::Spawn:		
	case EGsBossIconStateType::Deactive:
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
		break;
	case EGsBossIconStateType::Ready:
	{
		// 시련의 전당에서도 보스 아이콘 출력되게 요청와서 
		// switcher가 아닌 별도 visible 관리
		// 길드 던전에서는 안보이게 처리
		// https://jira.com2us.com/jira/browse/C2URWQ-6900
		if (_currentMinimapSideMode == EGsMinimapSideButtonMode::GuildDungeon)
		{
			SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{		
			SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
		break;
	default:
		break;
	}
}

void UGsUIHUDBoss::SetMinimapSideButtonMode(EGsMinimapSideButtonMode In_mode)
{
	_currentMinimapSideMode = In_mode;

	// visible 갱신
	InvalidVisibility();
}