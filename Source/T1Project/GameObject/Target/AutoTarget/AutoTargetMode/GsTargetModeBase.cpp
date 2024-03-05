// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTargetModeBase.h"
#include "T1Project.h"
#include "ActorEx/GsCharacterLocalPlayer.h"
#include "Data/GsDataContainManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsAIManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "GameObject/Npc/GsSchemaNpcData.h"
#include "GameObject/ObjectClass/GsGameObjectNonPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Target/GsTargetHandlerLocalPlayer.h"
#include "AI/ContentsCondition/GsAIContentsConditionAutoReturn.h"
#include "AI/Data/GsAIDefine.h"
#include "AI/Function/GsAIFunc.h"
#include "UTIL/GsGameObjectUtil.h"


void FGsTargetModeBase::Initialize(UGsGameObjectLocalPlayer* Local)
{
	_local = Local;
	
	ApplyTargetList();
}

void FGsTargetModeBase::ApplyTargetList()
{
	// 검색 반경 저장
	_searchRange = GData()->GetGlobalData()->_autoReturnDist4;

	// 대상 검색 리스트 초기화
	_listSearchType.Empty();
}

void FGsTargetModeBase::SearchDataInitailize()
{
	_searchCenterPos2D = _local->GetLocation2D();
}

// 타겟 조건이 안되는지 체크 함수 추가
bool FGsTargetModeBase::IsNotTarget(UGsGameObjectBase* In_object)
{
	// 타겟 가능 검사
	return false == _local->CanTarget(In_object);
}

void FGsTargetModeBase::SearchTarget(OUT TArray<class UGsGameObjectBase*>& OutResult)
{
	// 검색 관련 참조 정보 설정
	SearchDataInitailize();

	// 타겟 확인
	FGsTargetHandlerLocalPlayer* targetHandler = _local->GetCastTarget<FGsTargetHandlerLocalPlayer>();
	UGsGameObjectBase* target = targetHandler->GetTarget();
	// 기존 타겟이 존재할 경우 유지
	if (target)
	{
		OutResult.Emplace(target);
		return;
	}

	CallSearchFunction(OutResult);
}

void FGsTargetModeBase::CallSearchFunction(OUT TArray<class UGsGameObjectBase*>& OutResult)
{
	// 결과 값 초기화 및 Once 타입 함수 결과 실행
	for (SearchTargetInfo& el : _listSearchType)
	{
		el._result.Empty();
		if (el._isOnce)
		{
			el._func(el._result, nullptr);
		}
	}

	// Creature 검출 (NonPlayer 타입)
	// SearchRange 유효 범위 및 공격 가능 타입만 가공
	// 추후 공격 불가능한 오브젝트라도 타겟이 될수도 있다.
	TArray<UGsGameObjectBase*> result = GSGameObject()->FindObjectArray(EGsGameObjectType::NonPlayerBase);
	result.RemoveAll([this](UGsGameObjectBase* el) {
		return ((true == IsNotTarget(el) ||
			(_searchCenterPos2D - el->GetLocation2D()).SizeSquared() > _searchRange * _searchRange));
		});

	// GameObject List Loop 타입 결과 실행
	// 앞조건에서 검색되었으면 다음 조건 검색 생략
	SearchTargetInfo* currentSearchInfo = nullptr;
	for (SearchTargetInfo& el1 : _listSearchType)
	{
		if (el1._isOnce)
		{
			continue;
		}

		for (UGsGameObjectBase* el2 : result)
		{
			el1._func(el1._result, el2);
		}

		if (el1._result.Num() > 0)
		{
			currentSearchInfo = &el1;
			break;
		}
	}

	// 검색 우선 순위에 따른 결과 도출
	if (currentSearchInfo && currentSearchInfo->_result.Num() > 0)
	{
		// 거리 정렬
		const FVector2D& captureVec2D = _searchCenterPos2D;
		currentSearchInfo->_result.Sort([captureVec2D](const UGsGameObjectBase& lhs, const UGsGameObjectBase& rhs) {
			return (lhs.GetLocation2D() - captureVec2D).SizeSquared() <
				(rhs.GetLocation2D() - captureVec2D).SizeSquared();
			});
		OutResult = currentSearchInfo->_result;
	}
}

void FGsTargetModeBase::FuncSearchPcAttacker(OUT TArray< UGsGameObjectBase*>& OutResult, UGsGameObjectBase* GameObject)
{
	if (false == GameObject->IsObjectType(EGsGameObjectType::Player))
		return;

	if (FGsTargetHandlerLocalPlayer* targetHandler = _local->GetCastTarget<FGsTargetHandlerLocalPlayer>())
	{
		if (targetHandler->GetPcAttacker() == GameObject->GetGameId())
		{
			OutResult.Add(GameObject);
		}
	}
}

void FGsTargetModeBase::FuncSearchNpcAttacker(OUT TArray<UGsGameObjectBase*>& OutResult, UGsGameObjectBase* GameObject)
{
	if (false == GameObject->IsObjectType(EGsGameObjectType::NonPlayer))
		return;

	if (FGsTargetHandlerLocalPlayer* targetHandler = _local->GetCastTarget<FGsTargetHandlerLocalPlayer>())
	{
		if (targetHandler->GetNpcAttacker() == GameObject->GetGameId())
		{
			OutResult.Add(GameObject);
		}
	}
}

void FGsTargetModeBase::FuncSearchQuestTarget(OUT TArray<UGsGameObjectBase*>& OutResult, UGsGameObjectBase* GameObject)
{
	if (false == GameObject->IsObjectType(EGsGameObjectType::NonPlayer))
		return;

	UGsGameObjectNonPlayer* nonPlayer = GameObject->CastGameObject<UGsGameObjectNonPlayer>();
	if (nonPlayer->GetQuestTargetActive())
	{
		OutResult.Add(GameObject);
	}
}

void FGsTargetModeBase::FuncSearchFirstAttackNpcTarget(OUT TArray<UGsGameObjectBase*>& OutResult, UGsGameObjectBase* inGameObject)
{
	if (false == inGameObject->IsObjectType(EGsGameObjectType::NonPlayer))
		return;

	UGsGameObjectNonPlayer* nonPlayer = inGameObject->CastGameObject<UGsGameObjectNonPlayer>();
	const FGsSchemaNpcData* tableData = nonPlayer->GetNpcData();
	
	if (nonPlayer->GetOffensive())
	{
		OutResult.Add(inGameObject);
	}
}

void FGsTargetModeBase::FuncSearchNearestEnemyTarget(OUT TArray<UGsGameObjectBase*>& OutResult, UGsGameObjectBase* inGameObject)
{
	// 우호의 보물몬스터는 타겟에 잡히지 않도록 한다
	if (inGameObject->GetObjectType() == EGsGameObjectType::NonPlayer)
	{
		UGsGameObjectNonPlayer* nonPlayer = inGameObject->CastGameObject<UGsGameObjectNonPlayer>();
		if (nonPlayer && nonPlayer->IsPlayerTeamTreasureMonster())
		{
			return;
		}
	}
	// 현재 타겟이 될 수 없는 플레이어나 몬스터는 타겟에 잡히지 않도록 한다.
	UGsGameObjectCreature* creature = inGameObject->CastGameObject<UGsGameObjectCreature>();
	if (creature->IsTargetedAvailableState() == false) //Hiding, Bury, death
	{
		return;
	}

	OutResult.Emplace(inGameObject);
}