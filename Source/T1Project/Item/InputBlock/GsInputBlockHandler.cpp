#include "GsInputBlockHandler.h"

#include "Management/GsMessageHolder.h"

// 초기화
void FGsInputBlockHandler::Initialize()
{
	if (FGsMessageHolder* msg = GMessage())
	{
		_gameObjectMsg.Emplace(
			msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_SPAWN_ME_COMPLETE, this,
				&FGsInputBlockHandler::OnLocalPlayerSpawnComplete));

		_gameObjectMsg.Emplace(
			msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_REVIVE, this,
				&FGsInputBlockHandler::OnLocalPlayerSpawnComplete));
	}
}

// 정리
void FGsInputBlockHandler::Finalize()
{
	if (FGsMessageHolder* msg = GMessage())
	{
		if (_gameObjectMsg.Num() != 0)
		{
			for (auto iter : _gameObjectMsg)
			{
				msg->GetGameObject().Remove(iter);
			}
			_gameObjectMsg.Empty();
		}
	}

	ClearCondition();
}

// 조건 등록(막는 ui 켜야함)
void FGsInputBlockHandler::AddBlockConditions(TArray<EGsInputBlockConditionType> In_conditions)
{
	for (EGsInputBlockConditionType type: In_conditions)
	{
		// 이미 있는거면 제낌
		if (true == _setCondition.Contains(type))
		{
			continue;
		}

		_setCondition.Add(type);
	}

	// 막는 ui 키자
	
}
// 조건 해제(set에서 빼고 크기가 0이면 막는 ui 꺼야함)
void FGsInputBlockHandler::DeleteBlockCondition(EGsInputBlockConditionType In_condition)
{
	if (_setCondition.Contains(In_condition))
	{
		_setCondition.Remove(In_condition);
	}

	if (_setCondition.Num() == 0)
	{
		// 막는 ui 끄자
	}
}
// 로컬 스폰 끝
void FGsInputBlockHandler::OnLocalPlayerSpawnComplete(const IGsMessageParam*)
{
	ClearCondition();
}

// 조건 정리
void FGsInputBlockHandler::ClearCondition()
{
	// 조건 비운다
	_setCondition.Empty();
}

// 잠겨있나
bool FGsInputBlockHandler::IsBlocked()
{
	return (_setCondition.Num() == 0)? false: true;
}