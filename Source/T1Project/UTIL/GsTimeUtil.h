// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "T1Project.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "GsTimeUtil.generated.h"

/**
 *
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGsSimpleTimerDelegate, float, in_delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGsSimpleTimerFinishDelegate, const UObject*, inTimer);

UCLASS()
class T1PROJECT_API UGsSimpleTimer : public UObject
{
	GENERATED_BODY()

public:
	FGsSimpleTimerDelegate			_tickDelegate;
	FGsSimpleTimerFinishDelegate	_finishDelegate;

private:
	UPROPERTY()
		float _currentTime;
	UPROPERTY()
		float _maxTime = 1.0f;
	UPROPERTY()
		bool _canTick = false;

public:
	virtual void BeginDestroy() override;

public:
	void Reset();
	bool IsFinish();
	virtual void Tick(float in_time);
	void Start()
	{
		_canTick = true;
	}
	void Stop();
	float GetRemainTime() { return _maxTime - _currentTime; }
	void SetMaxTime(float inMaxTime) { _maxTime = inMaxTime; }
	float GetMaxTime() { return _maxTime; }
	bool CanTick() { return _canTick; }
	void SetTick(bool inTick) { _canTick = inTick; }
	float GetCurrentTime() { return _currentTime; }
	float GetCurrentTimeRatio(){return _currentTime / _maxTime;}
	void Clear();
};

class UCurveFloat;
//-------------------------------------------------------------------------------------------------
// UGsCurveTimer
// 커브를 제어하기 위한 타이머
//-------------------------------------------------------------------------------------------------
UCLASS()
class T1PROJECT_API UGsCurveTimer : public UGsSimpleTimer
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	UCurveFloat* _curve;

private:
	float _curveValue;

public:
	virtual void Tick(float inTime) override;

public:
	bool IsValid() const { return _curve != nullptr; }

public:
	void SetCurve(UCurveFloat* inData);

public:
	float GetCurrentValue() const { return _curveValue; }
};


//-------------------------------------------------------------------------------------------------
// FGsSimpleTimer
// 일반 FGs클래스들이 사용할 타이머.. 
//-------------------------------------------------------------------------------------------------

class T1PROJECT_API FGsSimpleTimer
{
public:
	DECLARE_EVENT(FGsSimpleTimer, MessageDelegator);
	MessageDelegator Router;

	virtual ~FGsSimpleTimer();

private:
	bool	_canTick = false;
	float	_currentTime = 0.0f;
	float	_interVal = 1.0f;

public:
	void SetInterval(float inInterval) { _interVal = inInterval; }
	float GetCurrentTime(){return _currentTime;}
	float GetCurrentTimeRatio(){return _currentTime / _interVal;}
	bool IsFinish();

public:
	void Update(float inTick);

public:
	void Reset();
	void Start() { _canTick = true; }
	void Stop();
};

//-------------------------------------------------------------------------------------------------
// FGsSimpleTimer
// 일반 FGs클래스들이 사용할 타이머.. 
//-------------------------------------------------------------------------------------------------

DECLARE_EVENT(FGsDateTimer, MessageDelegator);

class T1PROJECT_API FGsDateTimer
{
public:
	MessageDelegator UpdateEvent;
	MessageDelegator FinishEvent;

private:
	FDateTime _endDateTime;
	FTimerHandle _timerHandle;
	TWeakObjectPtr<UWorld> _world;

public:
	FGsDateTimer();
	virtual ~FGsDateTimer();
	void Clear();

public:
	void Start(UWorld* inWorld, const FDateTime& inEndDateTime, float inRate, TFunction<void(void)>&& inUpdateCallback, TFunction<void(void)>&& inFinishCallback);
	void Stop(UWorld* inWorld);	
	void OnUpdate();
	bool IsFinish();	
	FTimespan GetRemainTime();
	FString GetRemainTimeText();
	static FString GetRemainTimeText(const FTimespan& inRemainTime);
};

// 네트워크 동기화등 경과 시간 체크 테스트용 Util
class T1PROJECT_API FGsStopWatch
{
protected:
	static FDateTime _timer;

public:
	static void Start();
	static void Stop();

	static float GetMillisecond();
	static float GetSecond();
	static FTimespan GetElapsedTime();	
};

class T1PROJECT_API FGsDateTimeUTIL
{
public:
	static FString GetCurrentTime();
};