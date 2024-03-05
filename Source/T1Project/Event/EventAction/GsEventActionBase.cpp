#include "GsEventActionBase.h"

#include "Runtime/CoreUObject/Public/UObject/Object.h"
#include "Runtime/Engine/Classes/Engine/EngineTypes.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Public/TimerManager.h"

#include "GsSchemaBase.h"
#include "T1Project.h"

void UGsEventActionBase::Initialize(const FGsSchemaBase* EventActionData)
{
}

bool UGsEventActionBase::IsPlaying() const
{
	return _isPlaying;
}

bool UGsEventActionBase::Play()
{
	const bool IsPlaySucceeded = OnPlay();
	_isPlaying = true;
	return IsPlaySucceeded;
}

void UGsEventActionBase::Finish()
{
	if (!_isPlaying)
	{
		GSLOG(Warning, TEXT("EventAction already finished!!"));
		return;
	}

	OnFinish();

	_onFinished.ExecuteIfBound();
	_onFinished.Unbind();

	_isPlaying = false;
}

bool UGsEventActionBase::ShouldReplayOnReconnection() const
{
	return false;
}

bool UGsEventActionBase::Rewind()
{
	if (OnRewind())
	{
		_isPlaying = false;
		return true;
	}
	
	return false;
}

bool UGsEventActionBase::OnPlay()
{
	return true;
}

bool UGsEventActionBase::OnAfterPlay()
{
	return true;
}

void UGsEventActionBase::OnFinish()
{
}

bool UGsEventActionBase::OnRewind()
{
	GSLOG(Log, TEXT("UGsEventActionBase::Rewind() doing nothing"));
	return false;
}

void UGsEventActionBase::OnApplicationWillEnterBackground()
{
}

void UGsEventActionBase::OnApplicationHasEnteredForeground()
{
}
