// Fill out your copyright notice in the Description page of Project Settings.


#include "GsAreaEnvHandler.h"
#include "Classes/Kismet/GameplayStatics.h"
#include "ActorEx/GsAreaEnvController.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "DataSchema/Map/Area/GsSchemaAreaData.h"
#include "DataSchema/Map/Area/GsSchemaAreaEnv.h"
#include "DataSchema/Map/Area/GsSchemaWeatherSchedule.h"
#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Sound/GsSoundPlayer.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "Engine/Engine.h"
#include "Shared/Shared/SharedInclude/SharedFuncs.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsGameObjectMessageParam.h"

#if WITH_EDITOR
#include "GameMode/GsGameModeTestClient.h"
#endif
#include "DataSchema/Map/GsSchemaMapTerritoryEnv.h"
#include "GameObject/Fence/GsEnvFence.h"
#include "GameObject/Fence/GsSpaceCrackFence.h"
#include "DataSchema/Map/GsSchemaMapTerritorySpaceCrack.h"
#include "UTIL/GsText.h"


void FGsAreaEnvHandler::Initialize(UGsGameObjectBase*)
{
	BindMessages();

	// 우선순위 정보 저장(수가 클수록 우선순위 높음)
	_mapEventPriority.Empty();
	_mapEventPriority.Emplace(EGsEnvEvent::FAIRY_ROOM, 3);
	_mapEventPriority.Emplace(EGsEnvEvent::GACHA_ROOM, 3);
	_mapEventPriority.Emplace(EGsEnvEvent::SPACECRACK, 2);
	_mapEventPriority.Emplace(EGsEnvEvent::INVASION, 1);

	if (const UGsGlobalConstant* globalConstant = GData()->GetGlobalData())
	{
		_weatherChangeCycleMinute = globalConstant->_weatherChangeCycleMinute;
	}
	
	_prevWeather = EGsEnvWeather::Max;
	_currEvent = EGsEnvEvent::NONE;
	_eventStack.Empty();

	LoadAreaSetting();
	UpdateDayTime();
	UpdateWeather();
}

void FGsAreaEnvHandler::Finalize()
{
	UnbindMessages();

	_envControllerList.Empty();
}

void FGsAreaEnvHandler::Update(float Delta)
{
	if (0 < _remainTimeWeather)
	{
		_remainTimeWeather -= Delta;
		if (0 >= _remainTimeWeather)
		{
			UpdateWeather();
		}
	}
}

void FGsAreaEnvHandler::BindMessages()
{
	FGsMessageHolder* msgMgr = GMessage();

	_msgHandleSystem = msgMgr->GetSystem().AddRaw(MessageSystem::SYNC_WORLD_TIME,
		this, &FGsAreaEnvHandler::OnSyncWorldTime);

	MGameObject& msgGameObject = msgMgr->GetGameObject();
	_msgHandleGameObjectList.Emplace(msgGameObject.AddRaw(MessageGameObject::ENV_START_EVENT,
		this, &FGsAreaEnvHandler::OnEnvStartEvent));
	_msgHandleGameObjectList.Emplace(msgGameObject.AddRaw(MessageGameObject::ENV_END_EVENT,
		this, &FGsAreaEnvHandler::OnEnvEndEvent));
	_msgHandleGameObjectList.Emplace(msgGameObject.AddRaw(MessageGameObject::ENV_TERRITORY_ENTERED,
		this, &FGsAreaEnvHandler::OnEnvTerritoryEnter));
	_msgHandleGameObjectList.Emplace(msgGameObject.AddRaw(MessageGameObject::ENV_TERRITORY_LEFT,
		this, &FGsAreaEnvHandler::OnEnvTerritoryExit));
	_msgHandleGameObjectList.Emplace(msgGameObject.AddRaw(MessageGameObject::ENV_SEPARATE_LIGHT_SOURCE_START,
		this, &FGsAreaEnvHandler::OnEnvSeparateLightSourceStart));
	_msgHandleGameObjectList.Emplace(msgGameObject.AddRaw(MessageGameObject::ENV_SEPARATE_LIGHT_SOURCE_END,
		this, &FGsAreaEnvHandler::OnEnvSeparateLightSourceEnd));

}

void FGsAreaEnvHandler::UnbindMessages()
{
	// 갑자기 종료할 경우등을 대비하여 널체크 수행
	FGsMessageHolder* msgMgr = GMessage();
	if (nullptr == msgMgr)
	{
		return;
	}	

	msgMgr->GetSystem().Remove(_msgHandleSystem);

	MGameObject& msgGameObject = msgMgr->GetGameObject();
	for (MsgGameObjHandle& msgHandle : _msgHandleGameObjectList)
	{
		msgGameObject.Remove(msgHandle);
	}
	_msgHandleGameObjectList.Empty();
}

void FGsAreaEnvHandler::LoadAreaSetting()
{
	_envControllerList.Empty();
	FindEnvironmentController(_envControllerList);

	if (false == IsEnvController())
	{
		return;
	}

#if WITH_EDITOR
	AGameModeBase* mode = UGameplayStatics::GetGameMode(_envControllerList[0]->GetWorld());
	_bIsTestClient = mode->IsA<AGsGameModeTestClient>();
#endif

	// 24시간을 몇 분에 흐를 것인가
	float cycleMin = 24.f / FGsTimeSyncUtil::GetWorldTimeDilation() * 60.f;
	SetTimeCycleDuration(cycleMin);

	//  테이블 값 기반 세팅
	const FGsSchemaAreaData* areaData = GetCurrentAreaData();
	if (areaData)
	{
		if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
		{			
			if (const FGsSchemaSoundResData* soundResData = areaData->areaEnv.audioPath.GetRow())
			{
				soundPlayer->PlaySoundEnv(soundResData);
			}
		}
	}

	// 로컬 플레이어 스폰 이벤트 보냄(BP에서 필요에 따라 파티클 컴포넌트 세팅) 
	for (TWeakObjectPtr<AGsAreaEnvController> envController : _envControllerList)
	{
		envController->LocalPlayerSpawnComplete();
	}
}

void FGsAreaEnvHandler::UpdateDayTime()
{
	if (false == IsEnvController())
	{
		return;
	}
	
	FTimespan worldTime = FGsTimeSyncUtil::GetWorldTime();

	// 시간 정보를 넘김
	SetTime(worldTime.GetTotalHours());	
}

void FGsAreaEnvHandler::UpdateWeather()
{
	if (false == IsEnvController())
	{
		return;
	}

	const FGsSchemaWeatherSchedule* schedule = GetCurrentWeatherSchedule();
	if (nullptr == schedule)
	{
		return;
	}

	time_t serverNowSecond = FGsTimeSyncUtil::GetServerNowSecond();

	// netNowSecond가 배속이 적용된 값이면, _weatherChangeCycleMinute 도 배속이 적용된 값이어야한다.
	// 현재는 배속이 없는(현실시간 동일) 값을 넣어 얻어온다.
	uint64 scheduleIndex = SharedFunc::CalcWeatherIdx(schedule->scheduleList.Num(), serverNowSecond, 
		_weatherChangeCycleMinute);

	EGsEnvWeather weather = EGsEnvWeather::NONE;
	
	if (schedule->scheduleList.IsValidIndex(scheduleIndex))
	{
		int32 weatherIndex = schedule->scheduleList[scheduleIndex];
		if (schedule->weatherList.IsValidIndex(weatherIndex))
		{
			weather = schedule->weatherList[weatherIndex];
		}
	}

	SetWeather(weather);

#if WITH_EDITOR
	GSLOG(Log, TEXT("[Weather] netNowSecond: %I64d, scheduleIndex:  %I64d, weather: %d"), 
		serverNowSecond, scheduleIndex, weather);
#endif
}

void FGsAreaEnvHandler::FindEnvironmentController(OUT TArray<TWeakObjectPtr<AGsAreaEnvController>>& OutList)
{
	OutList.Empty();

	UGsLevelManager* levelManager = GLevel();
	if (nullptr == levelManager)
	{
		return;
	}

	TArray<AActor*> actorList;
	UGameplayStatics::GetAllActorsOfClass(levelManager->GetWorld(), AGsAreaEnvController::StaticClass(), actorList);

	for (AActor* actor : actorList)
	{
		if (AGsAreaEnvController* envActor = Cast<AGsAreaEnvController>(actor))
		{
			OutList.Emplace(envActor);
		}
	}

	if (0 == OutList.Num())
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("Fail to find AGsAreaEnvController. Level: %d"), levelManager->GetCurrentLevelId());
#endif
	}
}

void FGsAreaEnvHandler::OnSyncWorldTime()
{
	UpdateDayTime();
	UpdateWeather();
}

void FGsAreaEnvHandler::OnEnvStartEvent(const IGsMessageParam* InParam)
{
	if (const FGsPrimitiveInt32* param = InParam->Cast<const FGsPrimitiveInt32>())
	{
		StartEnvEvent(static_cast<EGsEnvEvent>(param->_data));
	}
}

void FGsAreaEnvHandler::OnEnvEndEvent(const IGsMessageParam* InParam)
{
	if (const FGsPrimitiveInt32* param = InParam->Cast<const FGsPrimitiveInt32>())
	{
		EndEnvEvent(static_cast<EGsEnvEvent>(param->_data));
	}
}

void FGsAreaEnvHandler::OnEnvTerritoryEnter(const IGsMessageParam* InParam)
{
	if (const FGsGameObjectFenceParam* param = InParam->Cast<const FGsGameObjectFenceParam>())
	{
		StartEnvEvent(param->_envEvent);
	}
}

void FGsAreaEnvHandler::OnEnvTerritoryExit(const IGsMessageParam* InParam)
{
	// 무조건 NONE을 주고있다. 전달받은 Fence 정보를 통해 Env를 삭제한다
	const FGsGameObjectFenceParam* param = InParam->Cast<const FGsGameObjectFenceParam>();
	if (nullptr == param)
	{
		return;
	}

	const FGsFence* fence = param->_fence;
	if (nullptr == fence)
	{
		return;
	}

	switch (fence->GetTerritoryType())
	{
	case TerritoryType::Env:
	{
		const FGsEnvFence* envFence = static_cast<const FGsEnvFence*>(fence);
		if (nullptr == envFence)
		{
			return;
		}

		for (const FGsEnvCondition& itCondition : envFence->GetEnv()->envConditionList)
		{
			EndEnvEvent(itCondition._envEvent);
		}
	}
		break;
	case TerritoryType::SPACE_CRACK:
	{
		const FGsSpaceCrackFence* spaceCrackFence = static_cast<const FGsSpaceCrackFence*>(fence);
		if (nullptr == spaceCrackFence)
		{
			return;
		}

		if (const FGsSchemaMapTerritorySpaceCrack* spaceCrackData = spaceCrackFence->GetSpaceCrackData())
		{
			EndEnvEvent(spaceCrackData->envEvent);
		}
	}
		break;
	}
}

void FGsAreaEnvHandler::SetTime(float InHours)
{
#if WITH_EDITOR
	if (_bIsTestClient)
	{
		return;
	}
#endif

	for (TWeakObjectPtr<AGsAreaEnvController> envController : _envControllerList)
	{
		if (envController.IsValid())
		{
			envController->SetEnvTime(InHours, 1.f);
		}
	}
}

void FGsAreaEnvHandler::SetTimeCycleDuration(float InMinutes)
{
#if WITH_EDITOR
	if (_bIsTestClient)
	{
		return;
	}
#endif

	for (TWeakObjectPtr<AGsAreaEnvController> envController : _envControllerList)
	{
		if (envController.IsValid())
		{
			envController->SetEnvTimeCycleDuration(InMinutes);
		}
	}
}

void FGsAreaEnvHandler::SetWeather(EGsEnvWeather InWeather)
{
	for (TWeakObjectPtr<AGsAreaEnvController> envController : _envControllerList)
	{
		if (envController.IsValid())
		{
			// 이전 날씨와 같아도 계속 보내준다(TA님 요청사항. Controller에서 체크함)
			envController->SetEnvWeather(InWeather);
		}
	}	

	// 날씨가 바뀌었을 때만 보냄
	if (_prevWeather != InWeather)
	{
		if (InWeather == EGsEnvWeather::NONE ||
			InWeather == EGsEnvWeather::RAIN ||
			InWeather == EGsEnvWeather::SNOW)
		{
			if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
			{
				float ratio = static_cast<float>(InWeather) / (static_cast<float>(EGsEnvWeather::Max) - 1.0f);
				soundPlayer->SetAudioParameter(EGsAudioParameterType::EnvWeather, ratio);
			}
		}
		_prevWeather = InWeather;
	}

	// 타이머 돌림
	_remainTimeWeather = _weatherCheckSecond;
}

void FGsAreaEnvHandler::StartEnvEvent(EGsEnvEvent InEnvEvent)
{
	int32 currEventPriority = GetPriorityEvent(_currEvent);
	int32 nextEventPriority = GetPriorityEvent(InEnvEvent);
	bool bNotChange = (currEventPriority > nextEventPriority) ? true : false;

	// 현재 우선 순위가 높을 경우 바꾸지 않고, 스택에만 저장 후 종료 
	if (bNotChange)
	{
		PushEventStack(InEnvEvent);
		return;
	}

	// EndEnvEvent 없이 StartEnvEvent가 또 불리는 상태
	if (EGsEnvEvent::NONE != _currEvent)
	{
		if (InEnvEvent != _currEvent)
		{
			// 이전 상태를 강제로 End 시킴(스택 상태는 유지)
			for (TWeakObjectPtr<AGsAreaEnvController> envController : _envControllerList)
			{
				if (envController.IsValid())
				{
#if ENV_DEBUG
					FString envType = FGsTextUtil::GetStringFromEnum(TEXT("EGsEnvEvent"), _currEvent);
					GSLOG(Log, TEXT("FGsAreaEnvHandler::StartEnvEvent - end pre env - env type : %s"), *envType);
#endif
					envController->EndEnvEvent(_currEvent);
					envController->EndEnvEventWithNext(_currEvent, InEnvEvent);
				}
			}
		}
	}	

	// 현재 상태 세팅
	_currEvent = InEnvEvent;
	PushEventStack(InEnvEvent);

	// 플레이어블 이벤트 예외처리(서브레벨 로딩으로 Env가 두개가 되어 한 쪽을 꺼야함)
	if (EGsEnvEvent::PLAYABLE_EVENT == InEnvEvent)
	{
		// 로비를 끔
		FName tagName = FName(TEXT("Env1"));
		SetEnableActorsByTag(tagName, false);
	}

	for (TWeakObjectPtr<AGsAreaEnvController> envController : _envControllerList)
	{
		if (envController.IsValid())
		{
			if (false == envController->IsHidden())
			{
#if ENV_DEBUG
				FString envType = FGsTextUtil::GetStringFromEnum(TEXT("EGsEnvEvent"), InEnvEvent);
				GSLOG(Log, TEXT("FGsAreaEnvHandler::StartEnvEvent - start new env - env type : %s"), *envType);
#endif

				envController->StartEnvEvent(InEnvEvent);
			}
		}
	}
}

void FGsAreaEnvHandler::EndEnvEvent(EGsEnvEvent InEnvEvent)
{
	// 현재 이벤트일 경우 비우기
	bool bIsCurrEvent = false;
	if (InEnvEvent == _currEvent)
	{
		_currEvent = EGsEnvEvent::NONE;
		bIsCurrEvent = true;
	}

	// 스택에 존재할 경우 무조건 제거
	_eventStack.Remove(InEnvEvent);

	// 다음 스택
	EGsEnvEvent nextStart = EGsEnvEvent::NONE;	
	if (EGsEnvEvent::NONE == _currEvent)
	{
		if (0 < _eventStack.Num())
		{
			nextStart = _eventStack.Pop();
		}
	}

	// 현재 이벤트의 종료메시지 일 경우의 처리
	if (bIsCurrEvent)
	{
		for (TWeakObjectPtr<AGsAreaEnvController> envController : _envControllerList)
		{
			if (envController.IsValid())
			{
				if (false == envController->IsHidden())
				{
#if ENV_DEBUG
					FString envType = FGsTextUtil::GetStringFromEnum(TEXT("EGsEnvEvent"), InEnvEvent);
					GSLOG(Log, TEXT("FGsAreaEnvHandler::EndEnvEvent - stop current env - env type : %s"), *envType);
#endif
					envController->EndEnvEvent(InEnvEvent);
					
					// 아트팀 개선 위해 다음 이벤트 보내주도록 설정
					envController->EndEnvEventWithNext(InEnvEvent, nextStart);
				}
			}
		}

		// 플레이어블 이벤트 예외처리(서브레벨 로딩으로 Env가 두개가 되어 한 쪽을 꺼야함)
		if (EGsEnvEvent::PLAYABLE_EVENT == InEnvEvent)
		{
			// 로비를 다시 켜고
			FName tagNameL = FName(TEXT("Env1"));
			SetEnableActorsByTag(tagNameL, true);

			// PlayableEvent를 끔
			FName tagNameP = FName(TEXT("Env2"));
			SetEnableActorsByTag(tagNameP, false);
		}
	}

	// 스택 이벤트 복구
	if (EGsEnvEvent::NONE != nextStart)
	{
		StartEnvEvent(nextStart);
#if ENV_DEBUG
		FString envType = FGsTextUtil::GetStringFromEnum(TEXT("EGsEnvEvent"), nextStart);
		GSLOG(Log, TEXT("FGsAreaEnvHandler::EndEnvEvent - start pre env - env type : %s"), *envType);
#endif
	}
}

void FGsAreaEnvHandler::PushEventStack(EGsEnvEvent InEvent)
{
	if (EGsEnvEvent::NONE == InEvent)
	{
		return;
	}

	// 중복을 허용하지 않고, 기존에 있으면 지운 후 최신으로 넣음
	_eventStack.Remove(InEvent);

	int32 priority = GetPriorityEvent(InEvent);

	// 우선 순위가 높을 수록 끝(스택 위쪽)에 배치
	bool bIsAdded = false;
	for (int32 i = 0; i < _eventStack.Num(); ++i)
	{
		if (priority < GetPriorityEvent(_eventStack[i]))
		{
			_eventStack.Insert(InEvent, i);
			bIsAdded = true;

			break;
		}
	}

	// 없거나 맨 뒤일 경우엔 추가가 안되므로 추가
	if (false == bIsAdded)
	{
		_eventStack.Emplace(InEvent);
	}

#if ENV_DEBUG
	GSLOG(Log, TEXT("%s"), *ToString());
#endif
}

int32 FGsAreaEnvHandler::GetPriorityEvent(EGsEnvEvent InEvent) const
{
	if (const int32* priorityPtr = _mapEventPriority.Find(InEvent))
	{
		return *priorityPtr;
	}

	return 0;
}

void FGsAreaEnvHandler::SetEnableActorsByTag(const FName& InTagName, bool bIsEnable)
{
	if (0 == _envControllerList.Num())
	{
		return;
	}

	TWeakObjectPtr<AGsAreaEnvController> envController = _envControllerList[0];
	if (false == envController.IsValid())
	{
		return;
	}

	TArray<AActor*> tagActorList;
	UGameplayStatics::GetAllActorsWithTag(envController->GetWorld(), InTagName, tagActorList);

	for (AActor* tagActor : tagActorList)
	{
		tagActor->SetActorHiddenInGame(!bIsEnable);
		tagActor->SetActorEnableCollision(bIsEnable);
		tagActor->SetActorTickEnabled(bIsEnable);
	}
}

const FGsSchemaWeatherSchedule* FGsAreaEnvHandler::GetCurrentWeatherSchedule() const
{
	const FGsSchemaAreaData* areaData = GetCurrentAreaData();
	if (areaData)
	{
		return areaData->areaEnv.weatherSchedule.GetRow();
	}

	return nullptr;
}

const FGsSchemaAreaData* FGsAreaEnvHandler::GetCurrentAreaData() const
{
#if WITH_EDITOR
	if (_bIsTestClient)
	{
		for (TWeakObjectPtr<AGsAreaEnvController> envController : _envControllerList)
		{
			if (envController.IsValid())
			{
				// 테스트 클라이언트 실행 시 id정보가 없으므로 이름으로 테이블 검색
				FString levelName = UGameplayStatics::GetCurrentLevelName(envController->GetWorld());
				if (const UGsTable* table = FGsSchemaAreaData::GetStaticTable())
				{
					TArray<const FGsSchemaAreaData*> rowList;
					table->GetAllRows(rowList);

					for (const FGsSchemaAreaData* iter : rowList)
					{
						if (iter->levelName == levelName)
						{
							return iter;
						}
					}
				}
			}
		}		

		return nullptr;
	}
#endif

	return GLevel()->GetCurrentAreaData();
}

void FGsAreaEnvHandler::FireScanEffect(FVector In_centerPos, float In_maxDistance)
{
	for (TWeakObjectPtr<AGsAreaEnvController> envController : _envControllerList)
	{
		if (envController.IsValid())
		{
			envController->StartFireScanEffect(In_centerPos, In_maxDistance);
		}
	}
}
void FGsAreaEnvHandler::OnEnvSeparateLightSourceStart(const IGsMessageParam* InParam)
{
	const FGsActorMessageParam* param = InParam->Cast<const FGsActorMessageParam>();
	if (param == nullptr)
	{
		return;
	}
	
	for (TWeakObjectPtr<AGsAreaEnvController> envController : _envControllerList)
	{
		if (envController.IsValid())
		{
			envController->StartSeparateLightSource(param->_paramActor);
		}
	}
}
void FGsAreaEnvHandler::OnEnvSeparateLightSourceEnd(const IGsMessageParam*)
{
	for (TWeakObjectPtr<AGsAreaEnvController> envController : _envControllerList)
	{
		if (envController.IsValid())
		{
			envController->EndSeparateLightSource();
		}
	}
}

FString FGsAreaEnvHandler::ToString()
{
	FString log = TEXT("env list : ");
	for (EGsEnvEvent evn : _eventStack)
	{
		FString envType = FGsTextUtil::GetStringFromEnum(TEXT("EGsEnvEvent"), evn);
		log += FString::Printf(TEXT("%s,\t"), *envType);
	}

	return log;
}
