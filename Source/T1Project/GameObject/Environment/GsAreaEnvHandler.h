// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Message/GsMessageSystem.h"
#include "Message/GsMessageGameObject.h"
#include "DataSchema/GsSchemaEnums.h"
#include "GameObject/GsGameObjectHandler.h"

class AGsAreaEnvController;
struct IGsMessageParam;

/*
 * FGsAreaEnvHandler
 * : AGsAreaEnvController를 얻어와 로직 제어하기 위한 클래스
 */
class T1PROJECT_API FGsAreaEnvHandler final : public IGsGameObjectHandler
{

protected:
	TArray<TWeakObjectPtr<AGsAreaEnvController>> _envControllerList;

	MsgSystemHandle _msgHandleSystem;
	MsgGameObjHandleArray _msgHandleGameObjectList;

protected:
	const float _weatherCheckSecond = 60; // n초마다 날씨 갱신
	int32 _weatherChangeCycleMinute = 30; // 미리 받아두기 위함
	float _remainTimeWeather = 0.f;
	EGsEnvWeather _prevWeather = EGsEnvWeather::Max;

	TMap<EGsEnvEvent, int32> _mapEventPriority; // value: priority (값이 클수록 우선순위 높음)	
	TArray<EGsEnvEvent> _eventStack;
	EGsEnvEvent _currEvent = EGsEnvEvent::NONE;

	// 테스트 클라이언트인지 확인(에디터에서만 사용)
	bool _bIsTestClient = false;

public:	
	FGsAreaEnvHandler() : IGsGameObjectHandler(GameObjectHandlerType::AREAENV) {};
	virtual ~FGsAreaEnvHandler() = default;

public:
	virtual void Initialize(class UGsGameObjectBase*) override;
	virtual void Finalize() override;
	virtual void Update(float Delta) override;

private:
	void BindMessages();
	void UnbindMessages();
	
public:
	// 컨트롤러를 다시 찾아서 세팅, 테이블 값으로 세팅
	void LoadAreaSetting();

	// 현재 서버시간으로 갱신
	void UpdateDayTime();
	void UpdateWeather();

private:
	void FindEnvironmentController(OUT TArray<TWeakObjectPtr<AGsAreaEnvController>>& OutList);

	void OnSyncWorldTime();
	void OnEnvStartEvent(const IGsMessageParam* InParam);
	void OnEnvEndEvent(const IGsMessageParam* InParam);
	void OnEnvTerritoryEnter(const IGsMessageParam* InParam);
	void OnEnvTerritoryExit(const IGsMessageParam* InParam);

	void OnEnvSeparateLightSourceStart(const IGsMessageParam* InParam);
	void OnEnvSeparateLightSourceEnd(const IGsMessageParam*);

public:
	void SetTime(float InHours);
	void SetTimeCycleDuration(float InMinutes);
	void SetWeather(EGsEnvWeather InWeather);
	// 스캔 이펙트 출력 시작
	void FireScanEffect(FVector In_centerPos, float In_maxDistance);
	FString ToString();

private:
	void StartEnvEvent(EGsEnvEvent InEnvEvent);
	void EndEnvEvent(EGsEnvEvent InEnvEvent);

	void PushEventStack(EGsEnvEvent InEvent);
	int32 GetPriorityEvent(EGsEnvEvent InEvent) const;

	void SetEnableActorsByTag(const FName& InTagName, bool bIsEnable);

private:
	const struct FGsSchemaWeatherSchedule* GetCurrentWeatherSchedule() const;
	const struct FGsSchemaAreaData* GetCurrentAreaData() const;

	bool IsEnvController() const { return (0 < _envControllerList.Num()) ? true : false; }
};
