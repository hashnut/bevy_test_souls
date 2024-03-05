// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Templates/Function.h"
#include "GameObject/Skill/Data/GsSkillDataBase.h"

/**
 * Local Player의 경우 SkillRunner와 동시에 동작 하는 Buff형 Runner가 필요
 * 이 Runner는 심플하게 딜레이 처리 용으로 사용하기 위해 제작
 */
class T1PROJECT_API FGsBuffSkillRunnerLocalPlayer final
{
private:
	class UGsGameObjectLocalPlayer*	_local	= nullptr;
	const FGsSchemaSkillSet*	_currData = nullptr;

	TFunction<void(int)>		_callbackStartRun{ nullptr };
	TFunction<void(int)>	_callbackEndRun{ nullptr };

	float						_startTime = 0.f;
	float						_totalRuntime = 0.f;

	bool						_isRunning{ false };
	bool						_isNetSend{ false };

public:
	FGsBuffSkillRunnerLocalPlayer(class UGsGameObjectLocalPlayer* Local, TFunction<void(int)> StartRun, TFunction<void(int)> EndRun);
	~FGsBuffSkillRunnerLocalPlayer() = default;

public:
	// 서버에서 응답받기전 상태를 기록한다.
	void SetReady(const FGsSchemaSkillSet* Data);
	void StartRunner(const FGsSchemaSkillSet* Data);
	

	void CancelRunner();
	void EndRunner();
	bool Update(float DeltaTime);

	bool IsRunning() { return _isRunning; }
	bool IsReady() { return _isNetSend; }

protected:
	void ClearRunner();

public:
	bool IsValid() const;
	float GetElapsed() const;

	const FGsSchemaSkillSet* GetData() const;
};
