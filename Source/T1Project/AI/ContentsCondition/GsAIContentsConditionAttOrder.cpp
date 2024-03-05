#include "GsAIContentsConditionAttOrder.h"

#include "GameObject/ObjectClass/GsGameObjectNonPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Target/GsTargetHandlerBase.h"
#include "Management/ScopeGame/GsGameObjectManager.h"

#include "Data/GsDataContainManager.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsAIManager.h"

#include "AI/ContentsCondition/GsAIContentsConditionAutoReturn.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "GameObject/Npc/GsSchemaNpcData.h"


FGsAIContentsConditionAttOrder::~FGsAIContentsConditionAttOrder()
{
	
}
// 초기화
void FGsAIContentsConditionAttOrder::Initialize(FGsAIManager* In_mng)
{
	_aiManager = In_mng;
	
	float playerSearchTargetRange = GData()->GetGlobalData()->_autoReturnDist4;
	_squaredPlayerSearchTargetRange = playerSearchTargetRange * playerSearchTargetRange;

	//_delegateUseAttOrder=
	//	GMessage()->GetGameObject().AddRaw(MessageGameObject::AI_MODE_ATTACK_ORDER, this,
	//		&FGsAIContentsConditionAttOrder::OnUseAttOrder);
	
}
// 해제
void FGsAIContentsConditionAttOrder::Finalize()
{
	//GMessage()->GetGameObject().Remove(_delegateUseAttOrder);
}

void FGsAIContentsConditionAttOrder::SetCharacter(UGsGameObjectLocalPlayer* In_player)
{
	_localPlayer = In_player;
}
void FGsAIContentsConditionAttOrder::RemoveCharacter()
{
	_localPlayer = nullptr;
}
// 정렬 함수 가져오기
//TFunction<bool(const TPair<UGsGameObjectBase*, float> & lhs, const TPair<UGsGameObjectBase*, float > & rhs)>
//	FGsAIContentsConditionAttOrder::GetFindObjSortFunc()
//{
//	return [this](const TPair<UGsGameObjectBase*, float>& lhs, const TPair<UGsGameObjectBase*, float >& rhs)
//	{
//		// 우선순위 사용하는지?
//		if (true == _isUseAttOrder)
//		{
//			// 1. 특정 id가 우선순위가 높다
//			UGsGameObjectNonPlayer* leftNPC = Cast<UGsGameObjectNonPlayer>(lhs.Key);
//			UGsGameObjectNonPlayer* rightNPC = Cast<UGsGameObjectNonPlayer>(rhs.Key);
//			if (nullptr == leftNPC ||
//				nullptr == rightNPC)
//			{
//				return true;
//			}
//
//			int leftId = FCString::Atoi(*leftNPC->GetNpcData()->GetRowName().ToString());
//			int rightId = FCString::Atoi(*rightNPC->GetNpcData()->GetRowName().ToString());
//			// id가 다르면 
//			// 둘중 하나가 특정 id면 
//			// 앞으로 감
//			if (leftId != rightId)
//			{
//				// 특정 id가 없으면 거리로 체크
//				if (leftId == _highOrderNPCId)
//				{
//					// 특정 id가 시야밖이고 나머지는 안이면 
//					// 나머지를 더위로
//					if (lhs.Value > _squaredPlayerSearchTargetRange&&
//						rhs.Value < _squaredPlayerSearchTargetRange)
//					{
//						return false;
//					}
//					return true;
//				}
//				else if (rightId == _highOrderNPCId)
//				{
//					// 특정 id가 시야밖이고 나머지는 안이면 
//					// 나머지를 더위로
//					if (rhs.Value > _squaredPlayerSearchTargetRange&&
//						lhs.Value < _squaredPlayerSearchTargetRange)
//					{
//						return true;
//					}
//
//					return false;
//				}
//			}
//		}
//
//		// 2. 거리로 정렬
//		return lhs.Value < rhs.Value;
//	};
//}

// 공격 우선순위 on, off
//void FGsAIContentsConditionAttOrder::OnUseAttOrder(const IGsMessageParam* In_isActive)
//{
//	const FGsPrimitiveInt32* param = In_isActive->Cast<const FGsPrimitiveInt32>();
//	bool isActive = static_cast<bool>(param->_data);
//	SetIsUseAttOrder(isActive);
//}


// 타겟 변경이 필요한지
//bool FGsAIContentsConditionAttOrder::IsNeedTargetChange()
//{
//	// 우선순위 사용안하면 필요없음
//	if (false == GetIsUseAttOrder())
//	{
//		return false;
//	}
//
//	FGsTargetHandlerBase* targetMng = _localPlayer->GetTargetHandler();
//	if (nullptr == targetMng)
//	{
//		return false;
//	}
//
//	// 타겟이 없거나
//	// 오토타겟이 아니면
//	// 타겟변경 할 필요 없음
//	UGsGameObjectBase* targetObj = targetMng->GetTarget();
//	if (nullptr == targetObj ||
//		false == targetMng->GetIsAutoTarget())
//	{
//		return false;
//	}
//
//	UGsGameObjectNonPlayer* npcObj = Cast<UGsGameObjectNonPlayer>(targetObj);
//	if (nullptr == npcObj)
//	{
//		return false;
//	}
//	int highOrderNPCId = GetHighOrderNPCId();
//
//	// 이미 타겟이 높은 순위면 필요 없음
//	if (highOrderNPCId == FCString::Atoi(*npcObj->GetNpcData()->GetRowName().ToString()))
//	{
//		return false;
//	}
//
//	// 시야안에 높은 순위 npc 있나
//	// id로 제일 가까운 npc 찾아와야함	
//	UGsGameObjectBase* nonPlayer =
//		GSGameObject()->FindObjectNearestNonPlayerByNPCId(_localPlayer->GetActor(),
//			highOrderNPCId);
//
//	// 저id로 없음
//	if (nullptr == nonPlayer)
//	{
//		return false;
//	}
//
//	float distanceToTarget = 0.0f;
//
//	FGsAIContentsConditionAutoReturn* autoReturn = _aiManager->GetContentsConditionAutoReturn();
//
//	bool isAutoReturnMode = autoReturn->GetIsAutoReturnMode();
//	const FVector autoStartPos = autoReturn->GetAutoStartPos();
//	float returnDist = autoReturn->GetReturnDist();
//
//	if (false == isAutoReturnMode)
//	{
//		// 일반 모드는 나와 적의 거리
//		distanceToTarget =
//			_localPlayer->GetActor()->GetHorizontalDistanceTo(nonPlayer->GetActor()) -
//			nonPlayer->GetData()->GetScaledCapsuleRadius();
//	}
//	else
//	{
//		// 컴백모드는 세이브한 중심과의 거리
//		// 빽업해놓은 위치와 적의 거리 구함
//		distanceToTarget = FVector::Dist2D(nonPlayer->GetActor()->GetActorLocation(),
//			autoStartPos) -
//			nonPlayer->GetData()->GetScaledCapsuleRadius();
//	}
//
//	// 시야 범위 밖인지
//	float searchRange = GData()->GetGlobalData()->_autoReturnDist4;
//
//	// 컴백모드면
//	// 돌아와야 하는 거리로
//	// 아니면 시야 거리로 체크
//	float checkRange = 
//		(isAutoReturnMode == true) ? returnDist : searchRange;
//	if (distanceToTarget > checkRange)
//	{
//		return false;
//	}
//	return true;
//}