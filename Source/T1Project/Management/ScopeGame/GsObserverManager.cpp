#include "GsObserverManager.h"

#include "Management/GsMessageHolder.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/ObjectClass/GsGameObjectCreature.h"
#include "GameObject/Data/GsGameObjectData.h"

#include "Message/MessageParam/GsMessageParam.h"

void FGsObserverManager::Initialize()
{
	FGsMessageHolder* msg = GMessage();

	_gameObjectDelegates.Emplace(
		msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_SPAWN_ME_COMPLETE, this,
			&FGsObserverManager::OnLocalPlayerSpawnComplete)
	);

	_msgSystemNoParamDelegates.Emplace(
		msg->GetSystem().AddRaw(MessageSystem::GAME_SERVER_RECONNECTION_SUCCESS,
			this, &FGsObserverManager::OnGameServerReconnectionSuccess));
}
void FGsObserverManager::Finalize()
{
	FGsMessageHolder* msg = GMessage();

	if (_gameObjectDelegates.Num() != 0)
	{
		for (auto iter : _gameObjectDelegates)
		{
			msg->GetGameObject().Remove(iter);
		}
		_gameObjectDelegates.Empty();
	}

	if (_msgSystemNoParamDelegates.Num() != 0)
	{
		for (auto iter : _msgSystemNoParamDelegates)
		{
			msg->GetSystem().Remove(iter);
		}
		_msgSystemNoParamDelegates.Empty();
	}
}

// local spawn end
void FGsObserverManager::OnLocalPlayerSpawnComplete(const IGsMessageParam*)
{
	if (_isObserveFinished == true)
	{
		_isObserveFinished = false;
	}
}
// 관전 시작(remote 모두 긁어 온다(사망 제외))
void FGsObserverManager::SetAllRemoteToObserve(TArray<UGsGameObjectBase*> In_allRemoteObjs)
{
	// 이미 있다면 비운다
	if (_arrayPlayGuildDungeonMember.Num() != 0)
	{
		_arrayPlayGuildDungeonMember.Empty();
	}

	for (UGsGameObjectBase* iter : In_allRemoteObjs)
	{
		if (nullptr == iter )
		{
			continue;
		}
		UGsGameObjectCreature* creature = Cast<UGsGameObjectCreature>(iter);
		if (nullptr == creature)
		{
			continue;
		}
		if (creature->IsZeroHP() == true ||
			creature->IsDeadState() == true)
		{
			continue;
		}
		// 사망아닌거만 넣는다
		_arrayPlayGuildDungeonMember.Add(iter);
	}



	SortList();	
}

// 관전 타겟 변경
void FGsObserverManager::SetTargetObserve(UGsGameObjectBase* In_obj)
{
	_targetObserve = In_obj;
}

// 리스트상 옆의 타겟 접근
UGsGameObjectBase* FGsObserverManager::GetSideTarget(bool In_isRight)
{
	// 타겟이 없다면 nullptr
	if (nullptr == _targetObserve)
	{
		return nullptr;
	}
	// 한개도 없으면 nullptr
	if (_arrayPlayGuildDungeonMember.Num() == 0)
	{
		return nullptr;
	}
	// 한개면 그거(하나면 보여줄 필요 없다)
	if (_arrayPlayGuildDungeonMember.Num() == 1)
	{
		return nullptr;
	}
	// 그외는 좌, 우
	int targetIndex = 
		_arrayPlayGuildDungeonMember.Find(_targetObserve);

	// 타겟으로 못찾으면 nullptr
	if (targetIndex == INDEX_NONE)
	{
		return nullptr;
	}

	// 타겟이 첫번째면
	if (targetIndex == 0) 
	{
		// 왼쪽이면 한바꾸 돌아서 젤 마지막꺼
		if (In_isRight == false)
		{
			return _arrayPlayGuildDungeonMember.Last();
		}		
	}
	// 타겟이 마지막이면
	else if (targetIndex == _arrayPlayGuildDungeonMember.Num() -1)
	{
		// 오른족이면 한바꾸 돌아서 첫번째꺼
		if (In_isRight == true)
		{
			return _arrayPlayGuildDungeonMember[0];
		}
	}

	int findIndex = targetIndex + ((In_isRight == true) ? 1 : -1);
	return _arrayPlayGuildDungeonMember[findIndex];
}

// 디스폰 or  사망시 삭제
bool FGsObserverManager::RemoveObserveGameObject(UGsGameObjectBase* In_gameObject)
{
	// 없으면 안함
	if (false == _arrayPlayGuildDungeonMember.Contains(In_gameObject))
	{
		return false;
	}
	// 리스트에서 삭제
	_arrayPlayGuildDungeonMember.Remove(In_gameObject);
	return true;
}

// add member
void FGsObserverManager::AddObserveGameObject(UGsGameObjectBase* In_gameObject)
{
	_arrayPlayGuildDungeonMember.Add(In_gameObject);

	SortList();
}

void FGsObserverManager::SortList()
{
	if (_arrayPlayGuildDungeonMember.Num() == 0)
	{
		return;
	}

	// 레벨 순으로 정렬 한다
	_arrayPlayGuildDungeonMember.Sort([](UGsGameObjectBase& lhs, UGsGameObjectBase& rhs)
		{
			return lhs.GetData()->GetLevel() > rhs.GetData()->GetLevel();
		});

}
// https://jira.com2us.com/jira/browse/C2URWQ-4947
// 기사단 던전 관전중 재연결시 크래쉬 이슈
void FGsObserverManager::OnGameServerReconnectionSuccess()
{
	if (_arrayPlayGuildDungeonMember.Num() != 0)
	{
		_arrayPlayGuildDungeonMember.Empty();
	}
	
}
