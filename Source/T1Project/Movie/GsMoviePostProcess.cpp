#include "GsMoviePostProcess.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "Engine/Classes/Components/PostProcessComponent.h"
#include "Engine/Classes/Components/AudioComponent.h"
#include "Engine/Classes/Engine/Scene.h"
#include "Engine/Classes/Sound/SoundWave.h"

#include "Kismet/GameplayStatics.h"

#include "UTIL/GsSoundUtil.h"

AGsMoviePostProcess::AGsMoviePostProcess()
{
	PrimaryActorTick.bCanEverTick = true;

	
}
void AGsMoviePostProcess::BeginPlay()
{
	Super::BeginPlay();

	if (nullptr == _postProcessComponent)
	{
		_postProcessComponent = FindComponentByClass<UPostProcessComponent>();
	}

	SetRate(0.0f);

}


// Called every frame
void AGsMoviePostProcess::TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction)
{
	Super::TickActor(DeltaTime, TickType, ThisTickFunction);

	if (_isChangeStart == false)
	{
		return;
	}
	float currRate = GetRate();
	SetRate(currRate);
	if (currRate >= 1.0f)
	{
		OnFinish();
	}
}

float AGsMoviePostProcess::GetRate()
{
	int64 nowTick = FDateTime::UtcNow().GetTicks();
	int64 diffTick = nowTick - _startTimeTick;

	float rate =  diffTick / _maxTimeTick;
	return rate;
}

void AGsMoviePostProcess::SetRate(float In_rate)
{
	if (_postProcessComponent == nullptr)
	{
		return;
	}

	if (_postProcessComponent->Settings.WeightedBlendables.Array.Num() == 0)
	{
		return;
	}

	_postProcessComponent->Settings.WeightedBlendables.Array[0].Weight = In_rate;
}

// 종료 호출
void AGsMoviePostProcess::OnFinish()
{
	_isChangeStart = false;

	if (_callbackLerpEnd != nullptr)
	{
		_callbackLerpEnd();
	}
}

void AGsMoviePostProcess::StartLerp(TFunction<void()> In_callbackLerpEndFunc)
{
	_isChangeStart = true;
	_callbackLerpEnd = In_callbackLerpEndFunc;

	if (_audioComp != nullptr)
	{
		_audioComp->Stop();
	}

	if (_soundWave != nullptr)
	{
		if (nullptr == _audioComp)
		{
			_audioComp = UGameplayStatics::SpawnSound2D(GetWorld(), _soundWave);
		}
		else
		{
			_audioComp->SetSound(_soundWave);
		}
	}

	// 현재 시간 저장
	_startTimeTick = FDateTime::UtcNow().GetTicks();

	// 최대 시간
	_maxTimeTick = GData()->GetGlobalData()->_moviePlayTime * ETimespan::TicksPerSecond;
}