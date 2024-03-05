#include "GsScanHandler.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"

#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsPartyManager.h"
#include "Management/ScopeGame/GsPKBookManager.h"

#include "Option/GsGameUserSettings.h"

#include "Message/MessageParam/GsGameObjectMessageParam.h"

#include "ActorEx/GsCharacterBase.h"

#include "GameObject/Quest/GsQuestHandler.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/ObjectClass/GsGameObjectCreature.h"
#include "GameObject/ObjectClass/GsGameObjectNonPlayerBase.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectSanctumBase.h"
#include "GameObject/Data/GsGameObjectDataPlayer.h"
#include "GameObject/Data/GsGameObjectData.h"
#include "GameObject/Target/GsTargetHandlerBase.h"
#include "GameObject/HUD/GsHUDLocalPlayer.h"
#include "GameObject/Environment/GsAreaEnvHandler.h"
#include "GameObject/Abnormality/GsAbnormalityHandlerBase.h"

#include "Guild/GsGuildPlayerData.h"

#include "Scan/GsScanMemberInfo.h"
#include "Scan/GsScanMemberInfoPlayer.h"
#include "Scan/GsScanMemberInfoMonster.h"
#include "Scan/GsScanMemberInfoCamp.h"
#include "Scan/GsScanMemberInfoSanctum.h"

#include "PKBook/GsPKBookData.h"

#include "UI/UIContent/HUD/GsUIHUDFrameMain.h"
#include "UI/UIContent/HUD/GsUIHUDScan.h"

#include "UTIL/GsGameObjectUtil.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcBalance.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedIffEnum.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "DataCenter/Public/Shared/Shared/SharedInclude/SharedFuncs.h"

#include "GameFramework/PlayerController.h"

// 초기화
void FGsScanHandler::Init()
{
	if (FGsMessageHolder* msg = GMessage())
	{
		_gameObjectMsg.Emplace(
			msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_TARGET_CLEAR, this,
				&FGsScanHandler::OnLocalTargetClear));

		_gameObjectMsg.Emplace(
			msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_TARGET_CHANGED, this,
				&FGsScanHandler::OnLocalTargetChanged));

		_gameObjectMsg.Emplace(
			msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_HIT, this,
				&FGsScanHandler::OnLocalHit));

		_gameObjectMsg.Emplace(
			msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_SPAWN_ME_COMPLETE, this,
				&FGsScanHandler::OnLocalPlayerSpawnComplete));


		_gameObjectMsg.Emplace(
			msg->GetGameObject().AddRaw(MessageGameObject::ABNORMALITY_ALL_UPDATE, this,
				&FGsScanHandler::OnAbnormalityAllUpdate));

		_systemMsg.Emplace(
			msg->GetSystemParam().AddRaw(MessageSystem::LOAD_TOPO_START, this,
				&FGsScanHandler::OnLoadTopoStart));
	}
}

// 종료
void FGsScanHandler::Close()
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

		if (_systemMsg.Num() != 0)
		{
			for (auto iter : _systemMsg)
			{
				msg->GetSystemParam().Remove(iter);
			}
			_systemMsg.Empty();
		}
	}
}
// 스캔 시작
void FGsScanHandler::StartScan(bool In_isClick, bool In_useLocalPos, FVector In_scanCenterPos)
{
	// 로컬 오브젝트
	UGsGameObjectBase* localPlayerObjBase = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);

	if (localPlayerObjBase == nullptr)
	{
		return;
	}

	UGsGameObjectLocalPlayer* localPlayer = Cast<UGsGameObjectLocalPlayer>(localPlayerObjBase);
	FVector useScanCenterPos = (In_useLocalPos == true) ? localPlayerObjBase->GetLocation() : In_scanCenterPos;

	// 스캔 이펙트 처리 호출
	if (In_isClick == true)
	{
		// 옵션 값 체크
		UGsGameUserSettings* gameUserSettings = GGameUserSettings();
		if (gameUserSettings == nullptr)
		{
			return;
		}
		// 옵션 값 확인
		bool isScanEffectShow =
			(bool)gameUserSettings->GetCombatSetting(EGsOptionCombat::PLAYER_SCAN_EFFECT);

		if (isScanEffectShow == true)
		{
			if (localPlayer)
			{
				if (FGsAreaEnvHandler* envHandler = localPlayer->GetAreaEnvHandler())
				{
					float scanDist = GData()->GetGlobalData()->_scanDist;
					envHandler->FireScanEffect(useScanCenterPos, scanDist);
				}
			}
		}
	}

	// 크리처 타입 다 가져 오기
	TArray<UGsGameObjectBase*> spawnList = GSGameObject()->FindObjectArray(EGsGameObjectType::Creature);


	// 로컬 데이터
	FGsGameObjectDataPlayer* localPlayerData = localPlayerObjBase->GetCastData<FGsGameObjectDataPlayer>();
	if (localPlayerData == nullptr)
	{
		return;
	}
	// 로컬 길드 데이터
	const FGsGuildPlayerData* localGuildData = localPlayerData->GetGuildData();

	// 로컬 위치
	FVector2D localPos = FVector2D(useScanCenterPos);

	float scanDistSquared = GSGameObject()->GetScanDistSquared();

	WorldId myHomeWorldId = GGameData()->GetUserData()->mHomeWorldId;
	WorldId nowMapWorldId = GGameData()->GetLoadWorldId();
	EnterWorldServerType loadWorldType = GGameData()->GetLoadWorldType();

	// 스캔 정렬 정보(정렬전, 거리 및 제외조건에서 만족된것만)
	TArray<FGsScanSortInfo> arrayScanSortInfo;
	for (auto iter: spawnList)
	{
		// hp zero, 사망은 제외
		UGsGameObjectCreature* creature = Cast<UGsGameObjectCreature>(iter);
		check(creature);

		// 스캔 거리 밖이면 제외
		FVector2D targetObjectPos = FVector2D(iter->GetLocation());
		float distSquared = (localPos - targetObjectPos).SizeSquared();
		if (distSquared > scanDistSquared)
		{
			continue;
		}

		if (creature->IsTargetedAvailableState() == false)
		{
			continue;
		}
		
		// IFF 데이터를 통한 타겟 대상 아님
		if (false == localPlayer->CanScan(creature))
		{
			continue;
		}

		// 정렬 대상 타입
		int sortTypeTarget = (int)EGsScanSortTypeTarget::Monster;
		// 정렬 2차 타입
		int sortTypeSub = 0;

		int64 targetGameId = iter->GetGameId();
		bool isAttacker =  GSGameObject()->GetIsAttackerId(targetGameId);

		//-	아군(소속 기사단, 파티원)은 스캔 대상으로 잡지 않음
		// remote에서 검사
		if (iter->GetObjectType() == EGsGameObjectType::RemotePlayer)
		{
			// check group remote
			if (false == GSGameObject()->GetScanGroupTargetType(EGsScanOptionGroupTargetType::Remote))
			{
				continue;
			}

			// 타유저 옵션 체크 해제 했을때 빼는게 아니라
			// 하나라도 체크 되어있으면서 만족하면 키는걸로 수정
			// https://jira.com2us.com/jira/browse/C2URWQ-5594

			bool isAttackerCheckOK = false;
			// 1. check attack remote option
			if (isAttacker == true &&
				(true ==
					GSGameObject()->GetScanDetectType(EGsScanOptionTargetType::AttackerRemote)))
			{
				isAttackerCheckOK = true;
			}
			// 타겟 데이터
			FGsGameObjectDataPlayer* targetPlayerData = iter->GetCastData<FGsGameObjectDataPlayer>();
			// 데이터 없으면 넘긴다
			if (nullptr == targetPlayerData)
			{
				continue;
			}

			// 2. check enemy guild option
			// 적대 길드 인가
			bool isEnemyGuild = targetPlayerData->IsEnemyGuild();
			bool isEnemyGuildCheckOK = false;
			if (isEnemyGuild == true &&
				(true ==
					GSGameObject()->GetScanDetectType(EGsScanOptionTargetType::EnemyGuild)))
			{
				isEnemyGuildCheckOK = true;
			}

			// 3. check alert target
			// https://jira.com2us.com/jira/browse/C2URWQ-4578
			bool isAlertTarget = false;
			bool isAlertTargetCheckOK = false;
			if (FGsPKBookManager* pkBookManager = GsPKBook())
			{
				const FGsPKDetectionTargetData* pkDetectionTargetData = pkBookManager->FindDetectionTargetDataById(targetPlayerData->GetUserDBID());
				isAlertTarget = (pkDetectionTargetData == nullptr)? false: true;
			}

			if (isAlertTarget == true &&
				(true ==
					GSGameObject()->GetScanDetectType(EGsScanOptionTargetType::AlertTarget)))
			{
				isAlertTargetCheckOK = true;
			}

			// 4. other server
			// SharedFunc::HasOtherUserNamePrefix 함수에서 인터서버이면 무조건 false 나오게 되어있음
			//bool isOtherServerRemote = SharedFunc::HasOtherUserNamePrefix(loadWorldType, nowMapWorldId, myHomeWorldId, targetPlayerData->GetHomeWorldId());
			// world id 가 다르면 다른서버로 처리
			bool isOtherServerRemote =  myHomeWorldId != targetPlayerData->GetHomeWorldId()? true: false;
			bool isOtherServerRemoteCheckOK = false;
			if (isOtherServerRemote == true &&
				(true ==
					GSGameObject()->GetScanDetectType(EGsScanOptionTargetType::OtherServerRemote)))
			{
				isOtherServerRemoteCheckOK = true;
			}

			// 5. check chaos 
			bool isChao = targetPlayerData->GetUserLookInfo()->mPVPAlignmentGrade == PVPAlignmentGrade::CHAOTIC;

			bool isChaoCheckOK = false;
			if (isChao == true &&
				(true ==
					GSGameObject()->GetScanDetectType(EGsScanOptionTargetType::Chaos)))
			{
				isChaoCheckOK = true;
			}

			bool isNormalRemote = (isAttacker == false) && (isEnemyGuild == false) && (isChao == false) && (isOtherServerRemote == false) &&
				(isAlertTarget == false);

			// 6. check normal remote
			bool isNormalRemoteCheckOK = false;
			if (
				isNormalRemote == true &&
				(true ==
				GSGameObject()->GetScanDetectType(EGsScanOptionTargetType::NormalRemote)))
			{
				isNormalRemoteCheckOK = true;
			}

			// 하나라도 만족하면 킨다
			// https://jira.com2us.com/jira/browse/C2URWQ-5594 
			if(isAttackerCheckOK == false &&
				isEnemyGuildCheckOK == false &&
				isAlertTargetCheckOK == false &&
				isOtherServerRemoteCheckOK == false &&
				isChaoCheckOK == false &&
				isNormalRemoteCheckOK == false
				)
			{ 
				continue;
			}


			sortTypeTarget = (int)EGsScanSortTypeTarget::User;

			// 내소속과 같은가(파티, 길드)
			if (true == UGsGameObjectUtil::GetIsMyCrewRemote(iter))
			{
				continue;
			}

			// check hide skill(cant targeting target)
			if (FGsAbnormalityHandlerBase* abnormalityHandler = iter->GetAbnormalityHandler())
			{
				// can-> cant target state changed
				if (abnormalityHandler->IsTargetedAvailableState() == false)
				{
					continue;
				}
			}

			
			// 1. 나를 공격하는가(클라 정보 처리)
			if (isAttacker == true)
			{
				sortTypeSub = (int)EGsScanSortTypeUser::Attacker;
			}
			// 2. 적대 기사단인가
			else if (isEnemyGuild == true)
			{
				sortTypeSub = (int)EGsScanSortTypeUser::EnemyGuild;
			}
			// 3. other server
			else if (isOtherServerRemote == true)
			{
				sortTypeSub = (int)EGsScanSortTypeUser::OtherServer;
			}
			// 4. 경계 대상인가
			// https://jira.com2us.com/jira/browse/C2URWQ-4578
			else if (isAlertTarget == true)
			{
				sortTypeSub = (int)EGsScanSortTypeUser::Alert;
			}
			// 5. 카오인가 , 위협인가
			else if (targetPlayerData->GetUserLookInfo()->mPVPAlignmentGrade == PVPAlignmentGrade::CHAOTIC)
			{
				sortTypeSub = (int)EGsScanSortTypeUser::Chaotic;
			}
			else if (targetPlayerData->GetUserLookInfo()->mPVPAlignmentGrade == PVPAlignmentGrade::THREAT)
			{
				sortTypeSub = (int)EGsScanSortTypeUser::Threat;
			}
			else
			{
				sortTypeSub = (int)EGsScanSortTypeUser::Etc;
			}

		}
		// 몬스터 인가
		else if (iter->GetObjectType() == EGsGameObjectType::NonPlayer)
		{
			UGsGameObjectNonPlayerBase* npc = iter->CastGameObject<UGsGameObjectNonPlayerBase>();
			if (nullptr == npc)
			{
				continue;
			}
			const FGsSchemaNpcData* npcData = npc->GetNpcData();
			if (nullptr == npcData)
			{
				continue;
			}

			// 1. check treasure monster
			// or check etc group
			if (npc->IsTreasureMonster() == true &&
				((false ==
					GSGameObject()->GetScanDetectType(EGsScanOptionTargetType::TreasureMonster)) ||
					(false == GSGameObject()->GetScanGroupTargetType(EGsScanOptionGroupTargetType::ETC))
					))
			{
				continue;
			}

			if (npc->IsTreasureMonster() == true)
			{
				sortTypeTarget = (int)EGsScanSortTypeTarget::TreasureMonster;
			}
			else
			{
				switch (npcData->grade)
				{
				case CreatureRankType::RAID:
				case CreatureRankType::BOSS:
				case CreatureRankType::NAMED:
					sortTypeTarget = (int)EGsScanSortTypeTarget::Boss;
					break;
				default:
					sortTypeTarget = (int)EGsScanSortTypeTarget::Monster;
					break;
				}
			}
				
			// 1. 보스 타입 구분
			// 1-1. 보스, 네임드
			if (sortTypeTarget == (int)EGsScanSortTypeTarget::Boss)
			{
				// check group boss
				if (false == GSGameObject()->GetScanGroupTargetType(EGsScanOptionGroupTargetType::Boss))
				{
					continue;
				}


				// option check: boss
				// boss off
				if (npcData->grade == CreatureRankType::BOSS &&
					(false ==
						GSGameObject()->GetScanDetectType(EGsScanOptionTargetType::Boss)))
				{
					// 옵션 체크 안되어있다면 제외
					continue;
				}

				// option check: named
				// named off
				if (npcData->grade == CreatureRankType::NAMED &&
					(false ==
						GSGameObject()->GetScanDetectType(EGsScanOptionTargetType::Named)))
				{
					continue;
				}
			
				if (npcData->grade == CreatureRankType::RAID)
				{
					sortTypeSub = (int)EGsScanSortTypeBoss::Raid;
				}
				else if (npcData->grade == CreatureRankType::BOSS)
				{
					sortTypeSub = (int)EGsScanSortTypeBoss::Boss;
				}
				else if(npcData->grade == CreatureRankType::NAMED)
				{
					sortTypeSub = (int)EGsScanSortTypeBoss::Named;
				}
			}
			// 2. 몬스터 타입 구분
			// 2-1. 나를 공격하는가(클라 정보로 처리)
			// 2-2. 퀘스트 몬스터
			// 2-3. 일반 몬스터
			else if(sortTypeTarget == (int)EGsScanSortTypeTarget::Monster)
			{
				// check group monster
				if (false == GSGameObject()->GetScanGroupTargetType(EGsScanOptionGroupTargetType::Monster))
				{
					continue;
				}
				// 타유저 처럼 옵션 체크 했을때 빼는게 아니라
				// 하나라도 체크 되어 있으면서 만족하면 키는걸로 수정
				//https://jira.com2us.com/jira/browse/C2URWQ-5898

				// 1. check attack monster
				bool isAttackerMonsterCheckOK = false;
				if (isAttacker == true &&
					(true ==
						GSGameObject()->GetScanDetectType(EGsScanOptionTargetType::AttackerMonster)))
				{
					isAttackerMonsterCheckOK = true;
				}

				// 2. check quest monster
				bool isQuestMonster = false;
				if (FGsQuestHandler* questHandler = GSQuest()->GetQuestHandler())
				{
					if (FGsQuestTargetManagement* questTargetManagement = questHandler->GetQuestTargetManagement())
					{
						QuestType outQuestType;
						if (questTargetManagement->IsQuestMonster(npcData->id, outQuestType) == true)
						{
							isQuestMonster = true;
						}
					}
				}
				bool isQuestMonsterCheckOK = false;
				if (isQuestMonster == true &&
					(true ==
						GSGameObject()->GetScanDetectType(EGsScanOptionTargetType::QuestMonster)))
				{
					isQuestMonsterCheckOK = true;
				}

				// 3. check normal monster
				bool isNormalMonster = (isAttacker == false) && (isQuestMonster == false);
				bool isNormalMonsterCheckOK = false;
				if (isNormalMonster == true &&
					(true ==
					GSGameObject()->GetScanDetectType(EGsScanOptionTargetType::NormalMonster)))
				{					
					isNormalMonsterCheckOK = true;
				}


				// 타유저 처럼 옵션 체크 했을때 빼는게 아니라
				// 하나라도 체크 되어 있으면서 만족하면 키는걸로 수정
				//https://jira.com2us.com/jira/browse/C2URWQ-5898
				if (isAttackerMonsterCheckOK == false &&
					isQuestMonsterCheckOK == false &&
					isNormalMonsterCheckOK == false)
				{
					continue;
				}


				if (isAttacker == true)
				{
					sortTypeSub = (int)EGsScanSortTypeMonster::Attacker;
				}
				else
				{
					sortTypeSub = (isQuestMonster == true)?
						(int)EGsScanSortTypeMonster::QuestMonster:
						(int)EGsScanSortTypeMonster::NormalMonster;
				}
			}
			else if (sortTypeTarget == (int)EGsScanSortTypeTarget::TreasureMonster)
			{
				// reverse rank(low value is higher)
				sortTypeSub = (int)CreatureRankType::MAX - (int)npcData->grade;
			}

		}
		else if (iter->GetObjectType() == EGsGameObjectType::Camp)
		{
			// 1. check camp
			// 2. check etc group
			if ((false == GSGameObject()->GetScanDetectType(EGsScanOptionTargetType::Camp)) ||
				(false == GSGameObject()->GetScanGroupTargetType(EGsScanOptionGroupTargetType::ETC)))
			{
				continue;
			}

			sortTypeTarget = (int)EGsScanSortTypeTarget::Camp;
		}
		else if (iter->GetObjectType() == EGsGameObjectType::Sanctum)
		{
			// 주의: 일단은 성소를 캠프랑 똑같이 처리했음
			// 1. check camp, 2. check etc group
			if ((false == GSGameObject()->GetScanDetectType(EGsScanOptionTargetType::Camp)) ||
				(false == GSGameObject()->GetScanGroupTargetType(EGsScanOptionGroupTargetType::ETC)))
			{
				continue;
			}

			sortTypeTarget = (int)EGsScanSortTypeTarget::Camp;
		}
		// 나머진 처리 안함
		else
		{
			continue;
		}
		
		arrayScanSortInfo.Add(FGsScanSortInfo{iter, sortTypeTarget , distSquared ,sortTypeSub });
	}
	// 하나라도 있으면 정렬
	if (arrayScanSortInfo.Num() != 0)
	{
		// 정렬
		// -	우선 순위
		// 1. 내가 타겟으로 지정한 대상
		// 2. 감지 대상 타입
		//		보스 -> camp ->유저 -> treasure monster -> 몬스터
		//	3. 거리 오름차순
		//	4. 거리가 동일할 경우 아래 우선 순위를 따름
		//		보스 : 보스 / 네임드
		//		유저 대상 : 교전 대상 / 적대 기사단 / 경계 대상 / 카오 / 기타
		//		treasure monster: grade descending(ex: 4,3,2,1)
		//		몬스터 대상 : 교전 대상 / 퀘스트 몬스터 / 일반 몬스터
		//	5. 모든 조건 동일할 경우 무작위

		// 로컬 타겟 구하기
		FGsTargetHandlerBase* targetHandler = localPlayerObjBase->GetTargetHandler();
		if (targetHandler == nullptr)
		{
			return;
		}
		UGsGameObjectBase* localTargetObj = targetHandler->GetTarget();


		arrayScanSortInfo.Sort([localTargetObj](const FGsScanSortInfo& lhs, const FGsScanSortInfo& rhs)
			{
				// 1. 로컬의 타겟 대상인지
				// 왼쪽이 타겟이면 true
				// 오른쪽이 타겟이면 false
				if (localTargetObj == lhs._targetObj)
				{
					return true;
				}
				if (localTargetObj == rhs._targetObj)
				{
					return false;
				}

				// 2. 감지 대상 타입(작을수록 높은것)
				// 왼쪽이 작으면 true
				// 오른쪽이 작으면 false
				if (lhs._sortTargetType != rhs._sortTargetType)
				{
					return lhs._sortTargetType < rhs._sortTargetType;
				}
				// 3. 거리 오름 차순(작을수록 높은것)
				// 왼쪽이 작으면 true
				// 오른쪽이 작으면 false
				if (lhs._distSquared != rhs._distSquared)
				{
					return lhs._distSquared < rhs._distSquared;
				}

				// 4. 대상 타입 2차(작을수록 높은것)
				// 왼쪽이 작으면 true
				// 오른쪽이 작으면 false
				if (lhs._sortTargetTypeSub != rhs._sortTargetTypeSub)
				{
					return lhs._sortTargetTypeSub < rhs._sortTargetTypeSub;
				}

				return true;
			});
	}

	// 정렬 완료후 gameobject manager에 넣음

	// 1. 기존꺼 삭제
	GSGameObject()->ClearScanInfo();

	TMap<int64, FGsScanMemberInfo*> mapScanMemberInfo;
	// 2. 신규 등록
	for (int i=0; i< arrayScanSortInfo.Num(); ++i)
	{
		// 최대 갯수에 도달하면 그만
		if (i >= GData()->GetGlobalData()->_scanMemberCount)
		{
			break;
		}

		UGsGameObjectBase* targetObj = arrayScanSortInfo[i]._targetObj;

		FGsScanMemberInfo* newInfo = nullptr;
		if (targetObj->GetObjectType() == EGsGameObjectType::NonPlayer)
		{
			newInfo =
				new FGsScanMemberInfoMonster(i + 1,
					targetObj, localPlayerObjBase);
		}
		else if(targetObj->GetObjectType() == EGsGameObjectType::RemotePlayer)
		{
			newInfo =
				new FGsScanMemberInfoPlayer(i + 1,
					targetObj, localPlayerObjBase);
		}
		else if (targetObj->GetObjectType() == EGsGameObjectType::Camp)
		{
			newInfo =
				new FGsScanMemberInfoCamp(i + 1,
					targetObj, localPlayerObjBase);
		}
		else if (targetObj->GetObjectType() == EGsGameObjectType::Sanctum)
		{
			newInfo =
				new FGsScanMemberInfoSanctum(i + 1,
					targetObj, localPlayerObjBase);
		}
		newInfo->Initialize();
		mapScanMemberInfo.Add(newInfo->GetGameID(), newInfo);
		
	}
	GSGameObject()->SetScanInfo(mapScanMemberInfo);
	if (FGsMessageHolder* msg = GMessage())
	{
		msg->GetGameObject().SendMessage(MessageGameObject::UPDATE_SCAN_INFO, nullptr);
	}


	// hud ui 갱신
	
	if (In_isClick == true)
	{
		// side 탭 액티브
		if (localPlayerObjBase->GetCharacter())
		{
			if (APlayerController* controller = Cast<APlayerController>(localPlayerObjBase->GetCharacter()->GetController()))
			{
				if (AGsHUDLocalPlayer* hud = Cast<AGsHUDLocalPlayer>(controller->GetHUD()))
				{
					hud->SetActiveSideTab(UGsUIHUDSideTab::EGsHUDSideTabType::SCAN);
				}
			}
		}
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->GetWidgetByKey(TEXT("HUDMain"), true);
	if (widget.IsValid())
	{
		UGsUIHUDFrameMain* hudWidget = Cast<UGsUIHUDFrameMain>(widget.Get());
		if (hudWidget)
		{
			if (UGsUIHUDScan* scanUI = hudWidget->GetScan())
			{
				scanUI->SetDirtyRefreshUI();
			}
		}
	}

}


// 스캔 인포 삭제
void FGsScanHandler::RemoveScanMemberInfo(int64 In_gameId)
{
	// 공격자 정보 삭제
	GSGameObject()->RemoveScanAttackerId(In_gameId);

	// 대상 정보 삭제
	if (false == GSGameObject()->RemoveScanMemberInfo(In_gameId))
	{
		return;
	}

	// ui 갱신
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->GetWidgetByKey(TEXT("HUDMain"), true);
	if (widget.IsValid())
	{
		UGsUIHUDFrameMain* hudWidget = Cast<UGsUIHUDFrameMain>(widget.Get());
		if (hudWidget)
		{
			if (UGsUIHUDScan* scanUI = hudWidget->GetScan())
			{
				scanUI->SetDirtyRefreshUI();
			}
		}
	}
}
// 로컬 타겟 클리어
void FGsScanHandler::OnLocalTargetClear(const IGsMessageParam* In_param)
{
	const FGsGameObjectMessageParam* castParam = In_param->Cast<const FGsGameObjectMessageParam>();
	int64 clearTargetGameId = 0;
	if (castParam->_paramOwner != nullptr)
	{
		clearTargetGameId = castParam->_paramOwner->GetGameId();
	}
	FGsScanMemberInfo* memberInfo =
		GSGameObject()->FindScanMemberInfo(clearTargetGameId);

	// 없으면 안함
	if (memberInfo == nullptr)
	{
		return;
	}
	// dirty 처리
	memberInfo->AddDirtyInfo(EGsScanMemberInfoType::LocalTarget);
}

// 로컬 타겟 변경 됨
void FGsScanHandler::OnLocalTargetChanged(const IGsMessageParam* In_param)
{
	if (nullptr == In_param)
	{
		return;
	}

	const FGsGameObjectMessageParamTarget* castParam = In_param->Cast<const FGsGameObjectMessageParamTarget>();
	int64 newTargetGameId = 0;
	if (castParam->_paramTarget != nullptr)
	{
		newTargetGameId = castParam->_paramTarget->GetGameId();
	}


	//old clear
	ClearOldTarget();

	// back up target game id
	_oldTargetGameId = newTargetGameId;

	FGsScanMemberInfo* memberInfo =
		GSGameObject()->FindScanMemberInfo(newTargetGameId);

	// 없으면 안함
	if (memberInfo == nullptr)
	{
		return;
	}
	// dirty 처리
	memberInfo->AddDirtyInfo(EGsScanMemberInfoType::LocalTarget);
}

// 스캔 리스트 삭제
void FGsScanHandler::ClearScanList()
{
	// 기존 정보 삭제
	GSGameObject()->ClearScanInfo();

	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->GetWidgetByKey(TEXT("HUDMain"), true);
	if (widget.IsValid())
	{
		UGsUIHUDFrameMain* hudWidget = Cast<UGsUIHUDFrameMain>(widget.Get());
		if (hudWidget)
		{
			if (UGsUIHUDScan* scanUI = hudWidget->GetScan())
			{
				scanUI->SetDirtyRefreshUI();
			}
		}
	}
}

// 로컬 얻어 맞음
void FGsScanHandler::OnLocalHit(const IGsMessageParam* In_param)
{
	const FGsGameObjectMessageParamTarget* paramTarget = 
		static_cast<const FGsGameObjectMessageParamTarget*>(In_param);

	if (nullptr == paramTarget->_paramTarget)
	{
		return;
	}

	int64 attackerGameId = paramTarget->_paramTarget->GetGameId();
	GSGameObject()->AddScanAttackerId(attackerGameId);
}

// 로컬 로드 완료
void FGsScanHandler::OnLocalPlayerSpawnComplete(const IGsMessageParam*)
{
	// 정보 클리어하자
	ClearScanList();
}

// 워프전 처리
void FGsScanHandler::OnLoadTopoStart(const IGsMessageParam* inParam)
{
	// 정보 클리어하자
	ClearScanList();
}

void FGsScanHandler::ClearOldTarget()
{
	FGsScanMemberInfo* memberInfo =
		GSGameObject()->FindScanMemberInfo(_oldTargetGameId);

	// 없으면 안함
	if (memberInfo == nullptr)
	{
		return;
	}
	// dirty 처리
	memberInfo->AddDirtyInfo(EGsScanMemberInfoType::LocalTarget);
}

void FGsScanHandler::OnAbnormalityAllUpdate(const IGsMessageParam* In_param)
{
	const FGsGameObjectMessageParamAbnormality* param = In_param->Cast<const FGsGameObjectMessageParamAbnormality>();
	if (param && param->_paramOwner)
	{		
		if (FGsAbnormalityHandlerBase* abnormalityHandler = param->_paramOwner->GetAbnormalityHandler())
		{
			// can-> cant target state changed
			if (abnormalityHandler->IsTargetedAvailableState() == false)
			{
				// exist scan item remove
				int64 targetGameId = param->_paramOwner->GetGameId();
				RemoveScanMemberInfo(targetGameId);
			}			
		}		
	}
}