#include "GsAIContentsConditionAutoReturn.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGame/GsAIManager.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "DataSchema/Map/GsSchemaMapData.h"

#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Option/GsGameUserSettings.h"

#include "DrawDebugHelpers.h"


FGsAIContentsConditionAutoReturn::~FGsAIContentsConditionAutoReturn()
{	
}

// 초기화
void FGsAIContentsConditionAutoReturn::Initialize(FGsAIManager* In_mng)
{	
	_aiManager = In_mng;

	SetAutoReturnStartPosType(EGsAIAutoReturnStartPosType::Dist20Meter);

	if (FGsMessageHolder* msg = GMessage())
	{
		_gameObjectMsg.Emplace(
			msg->GetGameObject().AddRaw(MessageGameObject::AI_MODE_SAVE_AUTO_POSITION, this,
				&FGsAIContentsConditionAutoReturn::OnSaveAutoStartPos));


		_gameObjectMsg.Emplace(
			msg->GetGameObject().AddRaw(MessageGameObject::AI_MODE_RETURN_START_POS_TYPE, this,
				&FGsAIContentsConditionAutoReturn::OnReturnStartPosTypeChanged));		
	}
	
}

// 돌아갈 거리 세팅
void FGsAIContentsConditionAutoReturn::SetReturnDist(float In_returnDist)
{
	_returnDist = In_returnDist;
	// 변수 값 저장
	_squaredReturnDist = _returnDist * _returnDist;
}
// 해제
void FGsAIContentsConditionAutoReturn::Finalize()
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
	
}
void FGsAIContentsConditionAutoReturn::SetCharacter(UGsGameObjectLocalPlayer* In_player)
{
	_localPlayer = In_player;
}
void FGsAIContentsConditionAutoReturn::RemoveCharacter()
{
	_localPlayer = nullptr;
}
// 돌아가야할 거리보다 멀리 갔는지
bool FGsAIContentsConditionAutoReturn::IsTooFarFromAutoStartPos()
{
	if (nullptr == _localPlayer || _localPlayer->GetActor() == nullptr)
	{
		return false;
	}

	FVector localPos = _localPlayer->GetActor()->GetActorLocation();
	float distSquared = FVector::DistSquared2D(localPos, _autoStartPos);

	return _squaredReturnDist < distSquared;
}

// 돌아가야할 거리 근처인지
//bool FGsAIContentsConditionAutoReturn::IsNearFromAutoStartPos()
//{
//	if (nullptr == _localPlayer)
//	{
//		return false;
//	}
//
//	FGsMovementHandlerLocalPlayer* MovementHandler = _localPlayer->GetCastMovement<FGsMovementHandlerLocalPlayer>();
//	if (nullptr == MovementHandler)
//	{
//		return false;
//	}
//
//	return MovementHandler->GetStateAutoMove().HasReached(_autoStartPos, _returnAcceptanceDist);
//}

// 타겟팅 할 obj 거리 체크하는 함수 가져오기
TFunction<bool(UGsGameObjectBase * obj)> 
	FGsAIContentsConditionAutoReturn::GetReturnDistCheckFunc()
{
	return [this](UGsGameObjectBase* obj)
	{
		if (nullptr == obj)
		{
			return false;
		}

		FVector objPos = obj->GetActor()->GetActorLocation();

		// 빽업해놓은 위치와 적의 거리 구함
		float dist = FVector::Dist2D(objPos, GetAutoStartPos()) -
			obj->GetData()->GetScaledCapsuleRadius();

		// 빽업 거리 안에 있어야 true 
		return _returnDist > dist;
	};
}

// 제자리 모드 인지 저장
void FGsAIContentsConditionAutoReturn::SetIsAutoReturnMode(bool In_val)
{
	_isAutoReturnMode = In_val;
	SaveAutoStartPos();
}
// 돌아갈 위치 저장
void FGsAIContentsConditionAutoReturn::SaveAutoStartPos()
{
	// 모드 on 일때 플레이어 위치 저장
	if (true == _isAutoReturnMode &&
		nullptr != _localPlayer &&
		nullptr != _localPlayer->GetActor())
	{
		_autoStartPos = _localPlayer->GetActor()->GetActorLocation();
	}
}
// 위치 저장 message
void FGsAIContentsConditionAutoReturn::OnSaveAutoStartPos(const IGsMessageParam*)
{
	SaveAutoStartPos();
}
// 제자리 돌아오기 모드 변경
void FGsAIContentsConditionAutoReturn::OnReturnStartPosTypeChanged(const IGsMessageParam* In_param)
{
	const FGsPrimitiveInt32* param = In_param->Cast<const FGsPrimitiveInt32>();
	EGsAIAutoReturnStartPosType type = static_cast<EGsAIAutoReturnStartPosType>(param->_data);
	SetAutoReturnStartPosType(type);
}
// 돌아오기 타입 세팅
void FGsAIContentsConditionAutoReturn::SetAutoReturnStartPosType(EGsAIAutoReturnStartPosType In_type)
{
	_returnStartPosType = In_type;

	bool isAutoReturnMode = true;
	float dist = 0.0f;
	switch (_returnStartPosType)
	{
	case EGsAIAutoReturnStartPosType::Dist20Meter:
		dist = GData()->GetGlobalData()->_autoReturnDist2;
		break;
	case EGsAIAutoReturnStartPosType::Dist30Meter:
		dist = GData()->GetGlobalData()->_autoReturnDist3;
		break;
	case EGsAIAutoReturnStartPosType::Dist50Meter:
		dist = GData()->GetGlobalData()->_autoReturnDist4;
		break;
	case EGsAIAutoReturnStartPosType::NoLimit:
		isAutoReturnMode = false;
		break;
	}

	if (isAutoReturnMode == true)
	{
		SetReturnDist(dist);
	}

	// 저장
	SetIsAutoReturnMode(isAutoReturnMode);
}

bool FGsAIContentsConditionAutoReturn::GetIsMapUseAutoReturn()
{
	int mapId = GLevel()->GetCurrentLevelId();
	const FGsSchemaMapData* mapData = UGsTableUtil::FindRowById<UGsTableMapData, FGsSchemaMapData>(mapId);
	if (mapData == nullptr)
	{
		return true;
	}

	return mapData->isUseAutoReturn;
}