// Fill out your copyright notice in the Description page of Project Settings.
#include "GsManualTargetMode.h"
#include "ActorComponentEx/GsSkillCollisionComponent.h"
#include "ActorEx/GsCharacterLocalPlayer.h"

#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"

#include "GameObject/Abnormality/GsAbnormalityHandlerBase.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/Data/GsGameObjectDataPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectRemotePlayer.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/Skill/GsSkillHandlerLocalPlayer.h"
#include "GameObject/Skill/GsSkillDataBuilder.h"
#include "GameObject/Target/GsTargetHandlerBase.h"
#include "GameObject/Target/GsTargetHandlerLocalPlayer.h"

#include "Data/GsDataContainManager.h"
#include "UTIL/GsGameObjectUtil.h"

#if WITH_EDITOR
#include "DrawDebugHelpers.h"
#endif


void FGsManualTargetMode::ApplyTargetList()
{
	Super::ApplyTargetList();

	TArray<int> CombatTargetOptionList;
	if (FGsServerOption* serverOption = GOption()->GetServerOption())
	{
		CombatTargetOptionList = serverOption->GetManualCombatTargetOptionList();
	}

	// 나를 공격중인 캐릭터 (ATTACK_ME_NPC 이넘 타입과 헷갈리면 안됨)
	if (0 != CombatTargetOptionList[(int)CombatOptionType::ATTACKED_ME_NPC])
	{
		SearchTargetInfo newInfo;
		newInfo.Add([this](OUT TArray<UGsGameObjectBase*>& OutResult, UGsGameObjectBase* GameObject)
			{ return FuncSearchPcAttacker(OutResult, GameObject); });
		_listSearchType.Emplace(newInfo);
	}

	// 나와 적대 관계인 캐릭터
	// 현재는 따로 적대라는 공통적인 개념이 없음 임의로 적대 길드 검색
	if (0 != CombatTargetOptionList[(int)CombatOptionType::ENEMY_ME])
	{
		SearchTargetInfo newInfo;
		newInfo.Add([this](OUT TArray<UGsGameObjectBase*>& OutResult, UGsGameObjectBase* GameObject)
			{ return FuncSearchEnemyUserTarget(OutResult, GameObject); });
		_listSearchType.Emplace(newInfo);
	}

	// 나를 공격한 몬스터
	if (0 != CombatTargetOptionList[(int)CombatOptionType::ATTACK_ME_NPC])
	{
		SearchTargetInfo newInfo;
		newInfo.Add([this](OUT TArray<UGsGameObjectBase*>& OutResult, UGsGameObjectBase* GameObject)
			{ return FuncSearchNpcAttacker(OutResult, GameObject); });
		_listSearchType.Emplace(newInfo);
	}

	// 퀘스트 목표
	if (0 != CombatTargetOptionList[(int)CombatOptionType::QUEST_TARGET])
	{
		SearchTargetInfo newInfo;
		newInfo.Add([this](OUT TArray<UGsGameObjectBase*>& OutResult, UGsGameObjectBase* GameObject)
			{ return FuncSearchQuestTarget(OutResult, GameObject); });
		_listSearchType.Emplace(newInfo);
	}

	// 선공 몬스터
	if (0 != CombatTargetOptionList[(int)CombatOptionType::FIRST_NPC])
	{
		SearchTargetInfo newInfo;
		newInfo.Add([this](OUT TArray<UGsGameObjectBase*>& OutResult, UGsGameObjectBase* GameObject)
			{ return FuncSearchFirstAttackNpcTarget(OutResult, GameObject); });
		_listSearchType.Emplace(newInfo);
	}

	SearchTargetInfo newInfo;
	newInfo.Add([this](OUT TArray<UGsGameObjectBase*>& OutResult, UGsGameObjectBase* GameObject)
		{ return FuncSearchNearestEnemyTarget(OutResult, GameObject); });
	_listSearchType.Emplace(newInfo);

	// 우선 검색 반경 설정
	_firstSearchRange = GData()->GetGlobalData()->ManualControlTargetSelectionDistance;
}

void FGsManualTargetMode::SearchDataInitailize()
{
	Super::SearchDataInitailize();

	// Local 데이터 저장
	_localData = _local->GetData();
}

void FGsManualTargetMode::SearchTarget(OUT TArray<class UGsGameObjectBase*>& OutResult)
{
	SearchDataInitailize();
	
	FGsTargetHandlerLocalPlayer* targetHandler = _local->GetCastTarget<FGsTargetHandlerLocalPlayer>();
	UGsGameObjectBase* target = targetHandler->GetTarget();
	// 기존 타겟이 존재할 경우 유지
	if (target)
	{
		OutResult.Emplace(target);
		return;
	}

	// 2차 타겟도 확인
	target = targetHandler->GetSecondTarget();
	if (target)
	{
		OutResult.Emplace(target);
		return;
	}

	CallSearchFunction(OutResult);
}

void FGsManualTargetMode::CallSearchFunction(OUT TArray<class UGsGameObjectBase*>& OutResult)
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
	TArray<UGsGameObjectBase*> result = GSGameObject()->FindObjectArray(EGsGameObjectType::Creature);
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

bool FGsManualTargetMode::IsNotTarget(UGsGameObjectBase* In_object)
{
	// 자기 자신
	if (In_object == _local)
	{
		return true;
	}

	// 상태이상
	if (FGsAbnormalityHandlerBase* abnormalityHandler = In_object->GetAbnormalityHandler())
	{
		if (false == abnormalityHandler->IsTargetedAvailableState())
		{
			return true;
		}
	}

	// 아니면 기본 체크
	return Super::IsNotTarget(In_object);
}

void FGsManualTargetMode::FuncSearchEnemyUserTarget(OUT TArray<class UGsGameObjectBase*>& OutResult, UGsGameObjectBase* GameObject)
{
	// 적대 관계
	if (GameObject->IsObjectType(EGsGameObjectType::RemotePlayer))
	{
		if (FGsGameObjectDataPlayer* targetData = GameObject->GetCastData<FGsGameObjectDataPlayer>())
		{
			if (SharedFunc::CheckPlayerEnemy(_local->GetData()->GetIff(), targetData->GetIff(), IffFactionType::PLAYER_ENEMY))
			{
				OutResult.Emplace(GameObject);
			}
		}
	}
}