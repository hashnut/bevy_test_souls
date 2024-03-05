// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTimeUtil.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Curves/CurveFloat.h"
#include "GsTimeSyncUtil.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"

void UGsSimpleTimer::Reset()
{	
	_currentTime = 0.f;
}

void UGsSimpleTimer::Tick(float in_time)
{
	if (_canTick)
	{
		_currentTime += in_time;

		if (IsFinish())
		{
			_canTick = false;
			_finishDelegate.Broadcast(this);

			return;
		}		

		if (_tickDelegate.IsBound())
		{
			_tickDelegate.Broadcast(_currentTime);

		}
	}	
}

void UGsSimpleTimer::BeginDestroy()
{
	_tickDelegate.Clear();
	_finishDelegate.Clear();

	Super::BeginDestroy();
}

bool UGsSimpleTimer::IsFinish()
{
	return (_currentTime > _maxTime);
}

void UGsSimpleTimer::Stop()
{
	_canTick = false;

	Reset();
}


void UGsSimpleTimer::Clear()
{
	_tickDelegate.Clear();
	_finishDelegate.Clear();
}
//-------------------------------------------------------------------------------------------------
// UGsCurveTimer
//-------------------------------------------------------------------------------------------------
void UGsCurveTimer::SetCurve(UCurveFloat* inData)
{
	_curve = inData;

	if (_curve)
	{
		float min;
		float max;

		_curve->GetTimeRange(min, max);
		SetMaxTime(max);
	}

}

void UGsCurveTimer::Tick(float inTime)
{
	if (CanTick() && false == IsFinish())
	{
		UGsSimpleTimer::Tick(inTime);

		if (_curve)
		{
			_curveValue = _curve->GetFloatValue(GetCurrentTime());
		}
	}
}


//-------------------------------------------------------------------------------------------------
// FGsSimpleTimer
//-------------------------------------------------------------------------------------------------

void FGsSimpleTimer::Reset()
{	
	_currentTime = 0.f;
}

void FGsSimpleTimer::Update(float inTick)
{
	if (_canTick)
	{
		if (IsFinish())
		{
			_canTick = false;
			if (Router.IsBound())
			{
				Router.Broadcast();
				Reset();
			}
			return;
		}

		_currentTime += inTick;
	}
}

FGsSimpleTimer::~FGsSimpleTimer()
{
	Router.Clear();
}

bool FGsSimpleTimer::IsFinish()
{
	return (_currentTime > _interVal);
}

void FGsSimpleTimer::Stop()
{
	Reset();
	_canTick = false;
}
//==============================================================================
FGsDateTimer::FGsDateTimer()
{

}

FGsDateTimer::~FGsDateTimer()
{
	Clear();
}

void FGsDateTimer::Clear()
{
	UpdateEvent.Clear();
	FinishEvent.Clear();
}

bool FGsDateTimer::IsFinish()
{
	FTimespan timeSpan = _endDateTime - FGsTimeSyncUtil::GetClientNowDateTime();
	return timeSpan.GetTotalSeconds() == 0;
}

FTimespan FGsDateTimer::GetRemainTime()
{
	return _endDateTime - FGsTimeSyncUtil::GetClientNowDateTime();
}

FString FGsDateTimer::GetRemainTimeText()
{
	FTimespan timeSpan = GetRemainTime();
	return GetRemainTimeText(timeSpan);
}

FString FGsDateTimer::GetRemainTimeText(const FTimespan& inRemainTime)
{
	FString timeMessage;
	const static double ZERO_SEC = 0;
	if (ZERO_SEC <= inRemainTime.GetTotalSeconds())
	{
		timeMessage.Empty(0);

		if (0 < inRemainTime.GetDays())
		{	// 2022/10/24 PKT - Day
			timeMessage.Append(FGsTimeStringHelper::GetDayString(inRemainTime.GetDays()));
		}

		if (0 < inRemainTime.GetHours())
		{	// 2022/10/24 PKT - Hours
			if (false == timeMessage.IsEmpty())
			{
				timeMessage.Append(TEXT(" "));
			}

			timeMessage.Append(FGsTimeStringHelper::GetHourString(inRemainTime.GetHours()));
		}

		if (0 < inRemainTime.GetMinutes())
		{	// 2022/10/24 PKT - Minutes
			if (false == timeMessage.IsEmpty())
			{
				timeMessage.Append(TEXT(" "));
			}

			timeMessage.Append(FGsTimeStringHelper::GetMinuteString(inRemainTime.GetMinutes()));
		}

		if (0 < inRemainTime.GetSeconds())
		{	// 2022/10/24 PKT - Minutes
			if (false == timeMessage.IsEmpty())
			{
				timeMessage.Append(TEXT(" "));
			}

			timeMessage.Append(FGsTimeStringHelper::GetSecondString(inRemainTime.GetSeconds()));
		}
	}

	return timeMessage;
}

void FGsDateTimer::OnUpdate()
{
	if (IsFinish())
	{
		if (FinishEvent.IsBound())
		{
			FinishEvent.Broadcast();
		}

		Stop(_world.Get());			
	}

	if (UpdateEvent.IsBound())
	{
		UpdateEvent.Broadcast();
	}
}

void FGsDateTimer::Start(UWorld* inWorld, const FDateTime& inEndDateTime, float inRate, TFunction<void(void)>&& inUpdateCallback, TFunction<void(void)>&& inFinishCallback)
{
	Stop(inWorld);

	_world = inWorld;
	_endDateTime = inEndDateTime;

#if WORLD_BOSS_DEBUG
	GSLOG(Log, TEXT("DateTimer Start - end time : %s\tcurrent time : %s")
		, *_endDateTime.ToString()
		, *FGsTimeSyncUtil::GetClientNowDateTime().ToString());
#endif

	if (inUpdateCallback)
	{
		UpdateEvent.AddLambda(inUpdateCallback);
	}
	
	if (inFinishCallback)
	{
		FinishEvent.AddLambda(inFinishCallback);
	}	
	
	FTimerDelegate timerDelegate = FTimerDelegate::CreateRaw(this, &FGsDateTimer::OnUpdate);
	inWorld->GetTimerManager().SetTimer(_timerHandle, timerDelegate, inRate, true);
}

void FGsDateTimer::Stop(UWorld* inWorld)
{
	if (inWorld)
	{
		inWorld->GetTimerManager().ClearTimer(_timerHandle);
	}	

	UpdateEvent.Clear();
	FinishEvent.Clear();
}

//==============================================================================

FDateTime FGsStopWatch::_timer = FDateTime();

void FGsStopWatch::Start()
{
	FGsStopWatch::_timer = FDateTime::Now();
}

void FGsStopWatch::Stop()
{
}

float FGsStopWatch::GetMillisecond()
{
	FTimespan elapsed = FDateTime::Now() - FGsStopWatch::_timer;
	return elapsed.GetTotalMilliseconds();
}

float FGsStopWatch::GetSecond()
{
	FTimespan elapsed = FDateTime::Now() - FGsStopWatch::_timer;
	return elapsed.GetSeconds();
}

FTimespan FGsStopWatch::GetElapsedTime()
{
	return FDateTime::Now() - FGsStopWatch::_timer;
}

FString FGsDateTimeUTIL::GetCurrentTime()
{
	FDateTime time = FDateTime::Now();
	return time.ToString(TEXT("%H.%M.%S.%s"));
}