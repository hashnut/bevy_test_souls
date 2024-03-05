// Fill out your copyright notice in the Description page of Project Settings.
#include "GsAutoTargetMode.h"
#include "Data/GsDataContainManager.h"

#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGame/GsAIManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"

#include "Quest/Management/GsQuestTargetManagement.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectNonPlayer.h"
#include "GameObject/Target/GsTargetHandlerLocalPlayer.h"
#include "GameObject/Abnormality/GsAbnormalityHandlerBase.h"

#include "AI/Data/GsAIDefine.h"
#include "AI/Function/GsAIFunc.h"
#include "AI/ContentsCondition/GsAIContentsConditionAutoReturn.h"

#include "GsTargetModeBase.h"

#include "GameMode/GsGameModeBase.h"
#include "GameMode/GsGameModeDefine.h"
#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/Stage/StageGame/GsStageManagerGame.h"

#include "UTIL/GsGameObjectUtil.h"


void FGsAutoTargetMode::ApplyTargetList()
{
	Super::ApplyTargetList();

	TArray<int> CombatTargetOptionList;
	if (FGsServerOption* serverOption = GOption()->GetServerOption())
	{
		CombatTargetOptionList = serverOption->GetAutoCombatTargetOptionList();
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
	

	if (UWorld* world = GLevel()->GetWorld())
	{
		if (AGsGameModeBase* gameMode = world->GetAuthGameMode<AGsGameModeBase>())
		{
			if (gameMode->GetGameModeType() == EGsGameModeType::Lobby)
			{
				_currRetunType = EGsAIAutoReturnStartPosType::Dist50Meter;
			}
			else
			{
				_currRetunType = EGsAIAutoReturnStartPosType::NoLimit;

				if (FGsAIManager* aiManager = GSAI())
				{
					if (FGsAIContentsConditionAutoReturn* autoReturnAi = aiManager->GetContentsConditionAutoReturn())
					{
						if (autoReturnAi->GetIsMapUseAutoReturn())
						{
							// 검색 반경 타입
							_currRetunType =
								aiManager->GetContentsConditionAutoReturn()->GetAutoReturnStartPosType();
						}
					}
				}
			}
		}
	}
	

	// 검색 반경 반영
	if (_currRetunType == EGsAIAutoReturnStartPosType::Dist20Meter)
	{
		_searchRange = GData()->GetGlobalData()->_autoReturnDist2;
	}
	else if (_currRetunType == EGsAIAutoReturnStartPosType::Dist30Meter)
	{
		_searchRange = GData()->GetGlobalData()->_autoReturnDist3;
	}
	else if (_currRetunType == EGsAIAutoReturnStartPosType::Dist50Meter)
	{
		_searchRange = GData()->GetGlobalData()->_autoReturnDist4;
	}
	else if (_currRetunType == EGsAIAutoReturnStartPosType::NoLimit)
	{
		_searchRange = GData()->GetGlobalData()->_autoReturnNoLimit;
	}
}

void FGsAutoTargetMode::SearchDataInitailize()
{
	// 현재 내캐릭터 위치 기준
	//_searchCenterPos2D = _local->GetLocation2D();
	// 저장된 위치를 중심으로 지정
	if (false == GSAI()->GetContentsConditionAutoReturn()->GetIsAutoReturnMode() ||
		_currRetunType == EGsAIAutoReturnStartPosType::NoLimit)
	{
		_searchCenterPos2D = _local->GetLocation2D();
	}
	else
	{
		_searchCenterPos2D = FVector2D(GSAI()->GetContentsConditionAutoReturn()->GetAutoStartPos());
	}

	// 퀘스트 자동 활성화 반영
	if (GSQuest())
	{
		_isQuestAutoProgressing = GSQuest()->IsAutoProgressing();
	}

	// 아레나 스테이지 확인
	_isArena = false;
	if(FGsGameFlowGame* gameFlow = GMode()->GetGameFlow())
	{
		if (FGsStageManagerGame* stage = gameFlow->GetStageManagerGame())
		{
			_isArena = stage->GetCurrentStageMode() == FGsStageMode::ARENA;
		}
	}
}

void FGsAutoTargetMode::SearchTarget(OUT TArray<class UGsGameObjectBase*>& OutResult)
{
	// 검색 관련 참조 정보 설정
	SearchDataInitailize();

	// 자동 전투용 타겟 우선 순위
	// 1순위 : 현재 타겟
	// 2순위 : 아레나 예외 처리 (무조건 플레이어)
	// 3순위 : 2차 타겟
	// 4순위 : 반격 옵션이 따로 있음 (반격 옵션 활성 & 자동 사냥 Off & 나를 공격한 몬스터)
	// 5순위 : 옵션 자동사냥 우선순위에서 높은 순서 거리별
	// 6순위 : 그 외 거리별

	// 타겟 확인
	FGsTargetHandlerLocalPlayer* targetHandler = _local->GetCastTarget<FGsTargetHandlerLocalPlayer>();
	UGsGameObjectBase* target = targetHandler->GetTarget();
	// 기존 타겟이 존재할 경우 유지
	if (target)
	{
		OutResult.Emplace(target);
	}

	// 아레나 모드 확인
	if (_isArena)
	{
		TArray<UGsGameObjectBase*> playerTargetlist = GSGameObject()->FindObjectArray(EGsGameObjectType::RemotePlayer);
		if (playerTargetlist.Num() > 0)
		{
			// 더이상 검사는 의미없다.
			OutResult.Append(playerTargetlist);
			return;
		}
	}	

	// 2차 타겟
	if (UGsGameObjectBase* secondTarget = targetHandler->GetSecondTarget())
	{
		// Player는 자동 공격에서는 생략
		if (false == secondTarget->IsObjectType(EGsGameObjectType::Player))
		{
			OutResult.Emplace(secondTarget);
		}
	}

	// 반격 옵션
	if (FGsAIManager* aiManager = GSAI())
	{
		FGsServerOption* serverOption = GOption()->GetServerOption();
		if (serverOption->IsCounterAttackOn() && false == aiManager->IsAIOn() && 0 != targetHandler->GetNpcAttacker())
		{
			if (UGsGameObjectBase* attacker = GSGameObject()->FindObject(
				EGsGameObjectType::NonPlayerBase, targetHandler->GetNpcAttacker()))
			{
				OutResult.Emplace(attacker);
			}
		}
	}

	CallSearchFunction(OutResult);
}

void FGsAutoTargetMode::CallSearchFunction(OUT TArray<UGsGameObjectBase*>& OutResult)
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
		// 거리 정렬은 내위치 기준으로
		const FVector2D& captureVec2D = _local->GetLocation2D();
		currentSearchInfo->_result.Sort([captureVec2D](const UGsGameObjectBase& lhs, const UGsGameObjectBase& rhs) {
			return (lhs.GetLocation2D() - captureVec2D).SizeSquared() <
				(rhs.GetLocation2D() - captureVec2D).SizeSquared();
			});

		// 자동 전투에서는 모든 리스트를 던져 주므로 Append
		OutResult.Append(currentSearchInfo->_result);
	}

	// 반격 처리 - 검색 반경에서 검출 대상이 없고 나를 공격중인 몬스터가 있을때 반경에 상관 없이 대상 포함
	// https://jira.com2us.com/jira/browse/C2URWQ-7756
	if (OutResult.Num() <= 0)
	{
		FGsTargetHandlerLocalPlayer* targetHandler = _local->GetCastTarget<FGsTargetHandlerLocalPlayer>();
		UGsGameObjectBase* target = targetHandler->GetTarget();
		if (UGsGameObjectBase* attacker = GSGameObject()->FindObject(
			EGsGameObjectType::NonPlayerBase, targetHandler->GetNpcAttacker()))
		{
			OutResult.Emplace(attacker);
		}
	}
}

void FGsAutoTargetMode::FuncSearchQuestTarget(OUT TArray<UGsGameObjectBase*>& OutResult, UGsGameObjectBase* GameObject)
{
	if (GameObject->IsObjectType(EGsGameObjectType::NonPlayer))
	{
		UGsGameObjectNonPlayer* nonPlayer = GameObject->CastGameObject<UGsGameObjectNonPlayer>();
		if (_isQuestAutoProgressing)
		{
			FGsQuestTargetManagement* questTargetManagement = GSQuest()->GetQuestTargetManagement();
			if (questTargetManagement->IsActiveQuestMonster(nonPlayer->GetData()->GetTableId()))
			{
				OutResult.Add(GameObject);
			}
		}
		else if (nonPlayer->GetQuestTargetActive())
		{
			OutResult.Add(GameObject);
		}
	}
}

// 타겟 조건이 안되는지 체크 함수 추가
bool FGsAutoTargetMode::IsNotTarget(UGsGameObjectBase* In_object)
{
	// 매너 모드 관련 체크
	if (GSAI()->GetIsMannerTarget(In_object) == true)
	{
		return true;
	}

	// NPC 예외처리
	if (In_object->IsObjectType(EGsGameObjectType::NonPlayerBase))
	{
		UGsGameObjectNonPlayerBase* castObject = In_object->CastGameObject<UGsGameObjectNonPlayerBase>();
		if (const FGsSchemaNpcData* npcTable = castObject->GetNpcData())
		{
			if (IsIgnoreNpcFunctionType(npcTable->npcFunctionType))
			{
				return true;
			}
			
			IffFactionActor& faction = castObject->GetData()->GetIff();
			if (IsIgnoreNpcFactionType(faction.mIffFactionType))
			{
				return true;
			}
		}
	}

	// 상태이상
	if (FGsAbnormalityHandlerBase* abnormalityHandler = In_object->GetAbnormalityHandler())
	{
		if (false == abnormalityHandler->IsTargetedAvailableState())
		{
			return true;
		}
	}
	
	return Super::IsNotTarget(In_object);
}

bool FGsAutoTargetMode::IsIgnoreNpcFactionType(IffFactionType IffType)
{
	if (IffType == IffFactionType::NPC_GUARD ||
		IffType == IffFactionType::NPC_INVASIONGUARDIAN ||
		IffType == IffFactionType::NPC_WAGONGUARD ||
		IffType == IffFactionType::NPC_WAGONPEDDLER ||
		IffType == IffFactionType::NPC_WAGONTHIEF ||
		IffType == IffFactionType::MONSTER_TREASURE)
	{
		return true;
	}
	return false;
}

bool FGsAutoTargetMode::IsIgnoreNpcFunctionType(NpcFunctionType functionType)
{
	if (functionType == NpcFunctionType::SPACE_TRACER ||
		functionType == NpcFunctionType::CAMP ||
		functionType == NpcFunctionType::SANCTUM ||
		functionType == NpcFunctionType::SANCTUM_NEXUS)
	{
		return true;
	}
	return false;
}