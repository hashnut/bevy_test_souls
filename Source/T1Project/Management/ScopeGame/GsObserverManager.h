#pragma once

#include "CoreMinimal.h"
#include "Classes/GsManager.h"

#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageSystem.h"

#include "Management/GsScopeHolder.h"
#include "Management/GsScopeGame.h"

/*
* 관전 시스템 매니저
*/

class UGsGameObjectBase;

struct IGsMessageParam;

class FGsObserverManager : public IGsManager
{
	// 멤버 변수
private:
	// 길드던전 플레이 멤버(GameObject)
	// 레벨순으로 정렬됨
	TArray<UGsGameObjectBase*> _arrayPlayGuildDungeonMember;
	// 관전 대상
	UGsGameObjectBase* _targetObserve;

	// 관전 시작인가
	bool _isObserveStart = false;
	// is observeFinished(next spawn complete clear flag)
	bool _isObserveFinished = false;

	MsgGameObjHandleArray _gameObjectDelegates;
	MsgSystemHandleArray _msgSystemNoParamDelegates;


	// logic func
public:
	// first time
	virtual void Initialize() override;
	// 
	virtual void Finalize() override;

	// 로직 함수
public:
	// 관전 시작(remote 모두 긁어 온다(사망 제외))
	void SetAllRemoteToObserve(TArray<UGsGameObjectBase*> In_allRemoteObjs);
	// despawn or die
	bool RemoveObserveGameObject(UGsGameObjectBase* In_gameObject);
	// spawn
	void AddObserveGameObject(UGsGameObjectBase* In_gameObject);

	void SortList();

	// msg
public:
	// local spawn end
	void OnLocalPlayerSpawnComplete(const IGsMessageParam*);
	// https://jira.com2us.com/jira/browse/C2URWQ-4947
	// 기사단 던전 관전중 재연결시 크래쉬 이슈
	void OnGameServerReconnectionSuccess();

	// get
public:
	TArray<UGsGameObjectBase*> GetPlayGuildDungeonMembers()
	{
		return _arrayPlayGuildDungeonMember;
	}
	// 관전 타겟 변경
	void SetTargetObserve(UGsGameObjectBase* In_obj);

	bool GetIsObserveStart()
	{
		return _isObserveStart;
	}
	void SetIsObserveStart(bool In_val)
	{
		_isObserveStart = In_val;
	}
	bool GetIsObserveFinished()
	{
		return _isObserveFinished;
	}
	void SetIsObserveFinished(bool In_val)
	{
		_isObserveFinished = In_val;
	}

	UGsGameObjectBase* GetTargetObserve()
	{
		return _targetObserve;
	}
	// 리스트상 옆의 타겟 접근
	UGsGameObjectBase* GetSideTarget(bool In_isRight);
};

#define GSObserver() UGsScopeHolder::GetGameManagerFType<FGsObserverManager>(UGsScopeGame::EManagerType::Observer)