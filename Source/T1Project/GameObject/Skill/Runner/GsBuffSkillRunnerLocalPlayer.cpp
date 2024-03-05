// Fill out your copyright notice in the Description page of Project Settings.


#include "GsBuffSkillRunnerLocalPlayer.h"
#include "Misc/App.h"
#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

FGsBuffSkillRunnerLocalPlayer::FGsBuffSkillRunnerLocalPlayer(UGsGameObjectLocalPlayer* Local, TFunction<void(int)> StartRun, TFunction<void(int)> EndRun) :
	_local(Local),
	_callbackStartRun(StartRun),
	_callbackEndRun(EndRun)
{
}

void FGsBuffSkillRunnerLocalPlayer::SetReady(const FGsSchemaSkillSet* Data)
{
	_isNetSend = true;
	_currData = Data;
}

void FGsBuffSkillRunnerLocalPlayer::StartRunner(const FGsSchemaSkillSet* Data)
{
	ClearRunner();

	static float globalBuffDelay = GData()->GetGlobalData() ? GData()->GetGlobalData()->BuffSkillDelayTime : 0.3f;

	if (Data)
	{
		if (_currData && _currData != Data)
		{
			GSLOG(Error, TEXT("[LocalSkillDebug] BuffSkillRunner not sync %d"), Data->id);
		}

		_currData = Data;
		_startTime = FApp::GetCurrentTime();
		_totalRuntime = Data->enableDelayTime + globalBuffDelay;

		_isRunning = true;

		if (_callbackStartRun) _callbackStartRun(Data->id);
	}
}

void FGsBuffSkillRunnerLocalPlayer::CancelRunner()
{
	ClearRunner();
}

void FGsBuffSkillRunnerLocalPlayer::EndRunner()
{
	int skillId = 0;
	if (_currData)
	{
		skillId = _currData->id;
	}

	ClearRunner();

	_isRunning = false;

	// Clear 이후 콜백 호출
	if (_callbackEndRun != nullptr)
	{
		_callbackEndRun(skillId);
	}
}

bool FGsBuffSkillRunnerLocalPlayer::Update(float DeltaTime)
{
	if (IsValid())
	{
		return _totalRuntime > GetElapsed();
	}
	return true;
}

void FGsBuffSkillRunnerLocalPlayer::ClearRunner()
{
	_currData = nullptr;
	_startTime = 0.f;
	_totalRuntime = 0.f;
	_isNetSend = false;
}

bool FGsBuffSkillRunnerLocalPlayer::IsValid() const
{
	return nullptr != _currData;
}

float FGsBuffSkillRunnerLocalPlayer::GetElapsed() const
{
	return FApp::GetCurrentTime() - _startTime;
}

const FGsSchemaSkillSet* FGsBuffSkillRunnerLocalPlayer::GetData() const
{
	return _currData;
}

