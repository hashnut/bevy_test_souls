// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIHUDFieldBoss.h"
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

void UGsUIHUDFieldBoss::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_button->OnClicked.AddDynamic(this, &UGsUIHUDFieldBoss::OnClickButton);
}

void UGsUIHUDFieldBoss::NativeConstruct()
{
	Super::NativeConstruct();

	FText::FindText(FIELD_BOSS_NAMESPACE_TEXT_KEY, FIELD_BOSS_SPAWN_TIME_TEXT_KEY, _spawnTimeTextFromat);

	MBoss& msgBoss = GMessage()->GetBoss();

	_readyTime = GData()->GetGlobalData()->_fieldBossReadyMinutes;
	_fieldBossSpawnTime = FDateTime();
	_bossState = EGsFieldBossIconStateType::Deactive;
	_time = 0;
	_levelId = 0;

	InvalidVisibility();
}

void UGsUIHUDFieldBoss::NativeDestruct()
{
	MBoss& msgBoss = GMessage()->GetBoss();
	for (MsgBossHandle& msgBossHandle : _msgBossHandleList)
	{
		msgBoss.Remove(msgBossHandle);
	}
	_msgBossHandleList.Empty();

	Super::NativeDestruct();
}

void UGsUIHUDFieldBoss::NativeTick(const FGeometry& inMyGeometry, float inDeltaTime)
{
	Super::NativeTick(inMyGeometry, inDeltaTime);
}

void UGsUIHUDFieldBoss::UpdateState(float inDelta)
{
	_time += inDelta;
	if (_time > 1)
	{
		_time = 0;

		InvalidHudState();
	}
}

void UGsUIHUDFieldBoss::InvalidateAllInternal()
{
	//현재 맵에서 보스 정보가 있다면 Spawn, Ready 가능(패킷에 허드 처리 맡길 것)
	//없다면 Deactive

	int32 levelId = GLevel()->GetCurrentLevelId();
	TArray<const FGsSchemaFieldBossData*> fieldBossDataArray = FGsFieldBossHandler::GetFieldBossDataByLevelId(levelId);
	if (fieldBossDataArray.Num() == 0)
	{
		_fieldBossSpawnTime = FDateTime();
		_bossState = EGsFieldBossIconStateType::Deactive;
		_time = 0;
		_levelId = 0;

		InvalidVisibility();
	}		
}

void UGsUIHUDFieldBoss::InvalidHudState()
{
	if (EGsFieldBossIconStateType::Ready == _bossState)
	{
		InvalidRemainTime();
	}
	else if (EGsFieldBossIconStateType::Deactive == _bossState)
	{
		if (_fieldBossSpawnTime > FGsTimeSyncUtil::GetServerNowDateTimeUTC())
		{
			_timeSpan = _fieldBossSpawnTime - FGsTimeSyncUtil::GetServerNowDateTimeUTC();

			if (_timeSpan.GetTotalSeconds() <= 0)
			{
				_bossState = EGsFieldBossIconStateType::Spawn;
			}
			else if(_timeSpan.GetTotalMinutes() < _readyTime)
			{
				_bossState = EGsFieldBossIconStateType::Ready;				
			}

			InvalidVisibility();
		}
	}
}

void UGsUIHUDFieldBoss::InvalidRemainTime()
{
	_timeSpan = _fieldBossSpawnTime - FGsTimeSyncUtil::GetServerNowDateTimeUTC();

	FGsTimeStringHelper::GetTimeStringHMS(_timeSpan, _remainTime);

	_remainTimeText->SetText(FText::FromString(_remainTime));
}

void UGsUIHUDFieldBoss::OnClickButton()
{
	//move to worldmap ui at current map
	if (GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::BOSS))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_BOSS);

		if (GUI()->IsActiveWidget(WINDOW_BOSS_INFO))
		{
			FGsGameStateBoss* state = FGsGameStateBoss::GetInstance();
			if (state)
			{
				state->SelectedBossByMap(_levelId);
			}
		}
	}
	else
	{
		GSUnlock()->ShowLockTicker(EGsUnlockContentsType::BOSS);
	}
}

//로컬 플레이어 스폰 후 보스 스폰 메시지 내려온다
void UGsUIHUDFieldBoss::OnFieldBossSpawn(const IGsMessageParam* InParam)
{
	const FGsUIMsgFieldBoss* param = InParam->Cast<const FGsUIMsgFieldBoss>();
	if (nullptr == param)
	{
		return;
	}

	const FGsSchemaFieldBossData* bossData = FGsFieldBossHandler::FindFieldBossData(param->_fieldBossId);
	if (nullptr == bossData
	|| nullptr == bossData->mapId.GetRow())
	{
		return;
	}
	
	int32 mapId = bossData->mapId.GetRow()->id;	
	if (mapId == GLevel()->GetCurrentLevelId())
	{
		_bossState = EGsFieldBossIconStateType::Spawn;
		_levelId = mapId;
	}

	InvalidVisibility();
}

void UGsUIHUDFieldBoss::OnFieldBossDie(const IGsMessageParam* InParam)
{
	const FGsUIMsgFieldBoss* param = InParam->Cast<const FGsUIMsgFieldBoss>();
	if (nullptr == param)
	{
		return;
	}

	const FGsSchemaFieldBossData* bossData = FGsFieldBossHandler::FindFieldBossData(param->_fieldBossId);
	if (nullptr == bossData
	|| nullptr == bossData->mapId.GetRow())
	{
		return;
	}
	
	if (bossData->mapId.GetRow()->id == GLevel()->GetCurrentLevelId())
	{
		_bossState = EGsFieldBossIconStateType::Deactive;
	}

	InvalidVisibility();
}


//로컬 플레이어 스폰 전에 보스 정보 패킷이 온다
void UGsUIHUDFieldBoss::OnFieldInfo(const struct IGsMessageParam* InParam)
{
	const FGsUIMsgFieldBoss* param = InParam->Cast<const FGsUIMsgFieldBoss>();
	if (nullptr == param)
	{
		return;
	}

	const FGsSchemaFieldBossData* bossData = FGsFieldBossHandler::FindFieldBossData(param->_fieldBossId);
	if (nullptr == bossData)
	{
		return;
	}

	_fieldBossSpawnTime = FGsTimeSyncUtil::ConvertServerTimeToUTCDateTime(param->_msgTime);
	FDateTime nowTime = FGsTimeSyncUtil::GetServerNowDateTimeUTC();

#if FIELD_BOSS_DEBUG
	GSLOG(Error, TEXT("field boss - spawn time : %s"), *_fieldBossSpawnTime.ToString());
	GSLOG(Error, TEXT("field boss - current time : %s"), *nowTime.ToString());
#endif

	//spawn 시간이 0으로 오면 스폰된 상태이다. (시간으로 치면 DateTime 변환시 1970.01.01.00:00)
	bool isSpawn = nowTime >= _fieldBossSpawnTime;
	if (isSpawn)
	{
		_bossState = EGsFieldBossIconStateType::Spawn;
		_levelId = bossData->mapId.GetRow()->id;
	}
	else
	{
		FTimespan remainDateTime = _fieldBossSpawnTime - nowTime;

#if FIELD_BOSS_DEBUG
		GSLOG(Error, TEXT("field boss - time span : %s"), *remainDateTime.ToString());
#endif
		//현재 시간과 스폰 시간을 뺀 차이에서 ReadyTime 이상이면 Deactive, 이하면 Ready
		if (remainDateTime.GetTotalMinutes() > _readyTime)
		{
			_bossState = EGsFieldBossIconStateType::Deactive;
		}
		else
		{
			_bossState = EGsFieldBossIconStateType::Ready;
		}
	}

	InvalidVisibility();
}

void UGsUIHUDFieldBoss::InvalidVisibility()
{
	_fieldBossWidgetSwitcher->SetActiveWidgetIndex((int32)_bossState);

	if (_bossState == EGsFieldBossIconStateType::Deactive)
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}