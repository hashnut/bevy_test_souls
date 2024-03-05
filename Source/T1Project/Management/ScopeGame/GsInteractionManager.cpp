#include "GsInteractionManager.h"

#include "Management/GsMessageHolder.h"

#include "ActorComponentEx/GsWidgetComponentButton.h"

#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"

#include "Management/GsScopeHolder.h"
#include "Management/GsScopeGame.h"
#include "Management/ScopeGame/GsClientSpawnManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsEventProgressManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "Management/ScopeGame/GsTutorialManager.h"
#include "Management/ScopeGame/GsContentsEventManager.h"
#include "Management/ScopeGame/GsInterServerManager.h"
#include "Management/ScopeGame/GsAIManager.h"

#include "Message/GsMessageInput.h"
#include "Message/GsMessageContents.h"

#include "Message/MessageParam/GsInteractionMessageParam.h"
#include "Message/MessageParam/GsAIMessageParam.h"
#include "Message/MessageParam/GsGameObjectMessageParam.h"
#include "Message/MessageParam/GsNpcShopMessageParam.h"
#include "Message/MessageParam/GsMinimapMessageParam.h"

#include "ContentsEvent/GsContentsEventNpcPrayer.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/ObjectClass/GsGameObjectNonPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "Gameobject/ObjectClass/GsGameObjectPropInteraction.h"
#include "Gameobject/ObjectClass/GsGameObjectCamp.h"

#include "GameObject/Interaction/GsInteractionHandlerLocalPlayer.h"
#include "GameObject/Interaction/GsInteractionNpcInfo.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/Quest/GsQuestHandler.h"
#include "GameObject/Movement/GsMovementHandlerBase.h"
#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"
#include "GameObject/Movement/LocalPlayer/GsMovementAutoDefine.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/Define/GsGameObjectStruct.h"

#include "GameObject/Data/GsGameObjectData.h"

#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcFunctionData.h"
#include "DataSchema/Map/Spawn/GsSchemaTerritoryNpc.h"
#include "DataSchema/Map/Spawn/GsSchemaSpawnData.h"
#include "DataSchema/Map/Spawn/GsSchemaTerritoryZoneShape.h"
#include "DataSchema/ContentsEvent/GsSchemaContentsEventNpcPrayer.h"
#include "DataSchema/InterServer/GsSchemaInvadeWorldConfigDataEx.h"

#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedIffEnum.h"
#include "Runtime/DataCenter/Public/Shared/Shared/SharedInclude/SharedTypes.h"


#include "Data/GsDataContainManager.h"
#include "Data/GsGameClientVersion.h"

#include "RecoveryPenalty/GsRecoveryPenaltyFunc.h"

#include "UTIL/GsTableUtil.h"
#include "UTIL/GsLevelUtil.h"

#include "Net/GsNetSendServiceWorld.h"
#include "Net/GsNetSendServiceQuest.h"

#include "Input/GsInputEventMsgBase.h"
#include "Runtime/DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"

#include "GameFlow/GameContents/ContentsGame/GsGameStateQuestBoard.h"

#include "Item/GsItem.h"

#include "AI/Quest/GsAIQuestHandler.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Popup/GsUIPopupEventPrayer.h"
#include "UI/UIContent/Popup/GsUIPopupCommonPay.h"

#include "Shared/Client/SharedEnums/PD_ResultEnum.h"




FGsInteractionManager::~FGsInteractionManager()
{

}

// 초기화(처음 한번만)
void FGsInteractionManager::Initialize()
{
	// 메시지 연결
	BindMessage();


}
// 해제
void FGsInteractionManager::Finalize()
{
	// 바인딩 된 메시지 해제
	FGsMessageHolder* msg = GMessage();
	MGameObject& msgGameObject = msg->GetGameObject();
	if (0 != _interactionDelegate.Num())
	{
		for (auto iter : _interactionDelegate)
		{
			msgGameObject.Remove(iter);
		}
		_interactionDelegate.Empty();
	}

	ClearNpcListInfo();
}

// 메시지 바인딩 처리
void FGsInteractionManager::BindMessage()
{
	FGsMessageHolder* msg = GMessage();
	MGameObject& objetMsg = msg->GetGameObject();

	_interactionDelegate.Emplace(
		objetMsg.AddRaw(MessageGameObject::INTERACTION_BUTTON_CLICK,
			this, &FGsInteractionManager::OnInteractionButtonClick)
	);
	_interactionDelegate.Emplace(
		objetMsg.AddRaw(MessageGameObject::INTERACTION_DEBUG_DRAW_RANGE,
			this, &FGsInteractionManager::OnInteractionDebugDrawRange)
	);

	_interactionDelegate.Emplace(
		objetMsg.AddRaw(MessageGameObject::CLIENT_SPAWN_LIST_LOAD_FINISH,
			this, &FGsInteractionManager::OnClientSpawnListLoadFinish)
	);

	_interactionDelegate.Emplace(
		objetMsg.AddRaw(MessageGameObject::INTERACTION_CONTENTS_BY_NPC,
			this, &FGsInteractionManager::OnInteractionNpcContents)
	);

	_interactionDelegate.Emplace(
		objetMsg.AddRaw(MessageGameObject::QUEST_TARGET_PROP,
			this, &FGsInteractionManager::OnPropTargetUpdate)
	);

	_interactionDelegate.Emplace(
		objetMsg.AddRaw(MessageGameObject::INTERACTION_CONTENTS_BY_INVASIONPROP_REGISTER,
			this, &FGsInteractionManager::OnInteractionInvasionRegister)
	);

	_interactionDelegate.Emplace(
		objetMsg.AddRaw(MessageGameObject::INTERACTION_CONTENTS_BY_QUEST_CANCEL,
			this, &FGsInteractionManager::OnInteractionContentsByQuestCancle)
	);
}
// 인터랙션 버튼 클릭
void FGsInteractionManager::OnInteractionButtonClick(const IGsMessageParam* In_param)
{
	const FGsInteractionMessageParam* castData = In_param->Cast<const FGsInteractionMessageParam>();

	if (castData == nullptr)
	{
		return;
	}

	// 터치 이동(몸통, 인터랙션 버튼) 이동타입 저장해놓기
	// https://jira.com2us.com/jira/browse/C2URWQ-3193
	if(_localPlayer != nullptr)
	{
		FGsMovementHandlerLocalPlayer* movement = _localPlayer->GetCastMovement<FGsMovementHandlerLocalPlayer>();
		if (nullptr != movement)
		{
			movement->SetMovementAutoContentsType(EGsMovementAutoContentsType::TouchInteractionTargetMove);
		}
	}

	//  castData->_targetGameID : client game id is minus value

	UGsGameObjectBase* findObject = GSGameObject()->FindObject(castData->_objectType, castData->_targetGameID);
	if (nullptr != findObject)
	{
		TryInteractionGameObject(findObject);
	}

}
// 인터랙션 디버그 범위 그리기
void FGsInteractionManager::OnInteractionDebugDrawRange(const IGsMessageParam* In_param)
{
	const FGsPrimitiveInt32* param = In_param->Cast<const FGsPrimitiveInt32>();
	bool isActive = static_cast<bool>(param->_data);
	_isDrawInteractionRange = isActive;
}

// 캐릭터 세팅
void FGsInteractionManager::SetCharacter(UGsGameObjectLocalPlayer* In_char)
{
	_localPlayer = In_char;
}

// 인터랙션 시도
void FGsInteractionManager::TryInteractionGameObject(UGsGameObjectBase* In_target)
{
	if (nullptr == In_target)
	{
		return;
	}

	if (nullptr == _localPlayer)
	{
		return;
	}

	// 로컬 인터랙션 핸들러로 넘김	
	FGsInteractionHandlerLocalPlayer* localInteraction = _localPlayer->GetCastInteraction<FGsInteractionHandlerLocalPlayer>();
	if (nullptr == localInteraction)
	{
		return;
	}

	// 인터랙션 대상에 따른 분기
	if (In_target->IsObjectType(EGsGameObjectType::NonPlayer)
		|| In_target->IsObjectType(EGsGameObjectType::Camp)
		|| In_target->IsObjectType(EGsGameObjectType::Sanctum))
	{
		UGsGameObjectNonPlayer* interactionTarget = static_cast<UGsGameObjectNonPlayer*>(In_target);
		if (nullptr == interactionTarget)
		{
			return;
		}

		const FGsSchemaNpcData* npcTbl = interactionTarget->GetNpcData();
		if (nullptr == npcTbl)
		{
			return;
		}

		localInteraction->TryInteractionNpcData(In_target, npcTbl);
	}
	else if (In_target->IsObjectType(EGsGameObjectType::PropInteraction))
	{
		UGsGameObjectPropInteraction* interactionTarget = static_cast<UGsGameObjectPropInteraction*>(In_target);
		if (nullptr == interactionTarget)
		{
			return;
		}

		const FGsSchemaPropData* propTbl = interactionTarget->GetPropTableData();
		if (nullptr == propTbl)
		{
			return;
		}

		//프랍 캔슬 중 인터렉션 시도하면 취소 시킨다. 
		if (interactionTarget->IsCanceling())
		{
			GSLOG(Warning, TEXT("prop is canceling - id : %lld"), interactionTarget->GetGameId());
			return;
		}

		// 인터랙션 중이었으면 캔슬 패킷 보냄
		if (localInteraction->IsInteraction())
		{
			if (UGsGameObjectBase* propTarget = localInteraction->GetInteractionTarget())
			{
				// 켄슬 패킷 요청 보내기
				FGsNetSendServiceWorld::SendInteractPropCancel(Cast<UGsGameObjectPropObject>(propTarget));
			}
		}

		localInteraction->TryInteractionPropData(In_target, propTbl);
	}
}
// 인터랙션 시도(tbl id와 위치로)
void FGsInteractionManager::TryInteraction(EGsGameObjectType In_type, int In_tblId, int In_mapId, FVector In_targetPos, float In_addAcceptanceRadius)
{
	// 로컬 인터랙션 핸들러로 넘김
	if (_localPlayer != nullptr)
	{
		FGsInteractionHandlerLocalPlayer* localInteraction =
			_localPlayer->GetCastInteraction<FGsInteractionHandlerLocalPlayer>();

		if (localInteraction != nullptr)
		{
			localInteraction->TryInteractionPos(In_type, In_tblId, In_mapId, In_targetPos, In_addAcceptanceRadius);
		}
	}
}


FVector FGsInteractionManager::GetNpcPos(int In_npcId)
{
	for (FGsInteractionNpcInfo* info : _arrayClientSpawnInteractionNpcInfo)
	{
		if (info->_npcId == In_npcId)
		{
			return info->_pos;
		}
	}

	return FVector::ZeroVector;
}
FVector FGsInteractionManager::GetServerNpcPos(int In_npcId)
{
	for (FGsInteractionNpcInfo* info : _arrayServerSpawnInteractionNpcInfo)
	{
		if (info->_npcId == In_npcId)
		{
			return info->_pos;
		}
	}

	return FVector::ZeroVector;
}

void FGsInteractionManager::SendInteractionCancel()
{
	UGsGameObjectBase* player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (player)
	{
		UGsGameObjectLocalPlayer* local = player->CastGameObject<UGsGameObjectLocalPlayer>();
		if (local)
		{
			FGsInteractionHandlerLocalPlayer* interactionHandler = local->GetCastInteraction<FGsInteractionHandlerLocalPlayer>();
			if (interactionHandler)
			{
				const FGsInteractionTimer& timer = interactionHandler->GetTimerInfo();
				if (UGsGameObjectBase* target = interactionHandler->GetInteractionTarget())
				{
					if (timer._isEnable && timer._currTime < timer._endTime)
					{
						// 켄슬 패킷 요청 보내기
						FGsNetSendServiceWorld::SendInteractPropCancel(Cast<UGsGameObjectPropObject>(target));
						// 예약정보 Clear
						interactionHandler->ClearReserveInteraction();
					}
				}
			}
		}
	}
}

// 클라 스폰 리스트 로드 끝
void FGsInteractionManager::OnClientSpawnListLoadFinish(const IGsMessageParam* In_param)
{
	const FGsClientSpawnListLoadFinishMessageParam* param =
		In_param->Cast<const FGsClientSpawnListLoadFinishMessageParam>();

	if (param == nullptr)
	{
		return;
	}

	// 기존 데이터 날리기
	ClearNpcListInfo();

	// 로컬의 iff를 구한다(그림자 전장에서 아군, 적군 npc 구별용)
	// https://jira.com2us.com/jira/browse/CHR-24545
	FGsGameObjectData* localObjData = _localPlayer->GetData();
	if (localObjData == nullptr)
	{
		return;
	}

	IffFactionType localIffFactionType = localObjData->GetIff().mIffFactionType;
	MapType currentMapType = GLevel()->GetCurrentMapType();


	// 클라이언트 스폰 정보 가져오기		
	// 마을 npc만 가져온다
	TArray<const FGsSchemaTownData*> townNpcList;
	if (UGsLevelManager::TryGetClientSpawnNpcInTown(townNpcList))
	{
		for (const FGsSchemaTownData* iter : townNpcList)
		{
			if (nullptr == iter)
			{
				continue;
			}

			FVector pos = iter->Pos;
			const FGsSchemaNpcData* npcData = iter->CreatureId.GetRow();
			if (nullptr == npcData)
			{
				continue;
			}

			// 퀘스트 DIALOG 2개 나오는 이슈로
			// https://jira.gamevilcom2us.com/jira/browse/CHR-4137
			// NPC LIST에서 퀘스트 대화도 뺌
			if (npcData->npcFunctionType == NpcFunctionType::NONE ||
				npcData->npcFunctionType == NpcFunctionType::QUEST_DIALOG)
			{
				continue;
			}

			// 그림자 전장에서 적 iff npc 정보는 뺀다
			//https://jira.com2us.com/jira/browse/CHR-24545
			if(currentMapType == MapType::GUILD_WAR)
			{
				if (true == IsEnemyIffShadowBattleField(localIffFactionType, npcData->iffFactionType))
				{
					continue;
				}
			}


			const FGsSchemaNpcFunctionData* npcFunctionData =
				UGsTableUtil::FindRowById<UGsTableNpcFunctionData, FGsSchemaNpcFunctionData>(npcData->npcFunctionType);

			if (nullptr == npcFunctionData)
			{
				continue;
			}

			int slotOrderNumber =  GetSlotOrderNumber(npcFunctionData);
			// -1은 안쓰는 정보
			if (slotOrderNumber == -1)
			{
				continue;
			}

			TArray<FGsTownInteractionNpcInfo*>* findInfo =
				_mapTownInteractionNpcInfo.Find(slotOrderNumber);

			if (findInfo == nullptr)
			{
				// add info
				TArray<FGsTownInteractionNpcInfo*> addInfo;
				addInfo.Add(new FGsTownInteractionNpcInfo(
					npcData->id, pos, npcData));

				_mapTownInteractionNpcInfo.Add(slotOrderNumber, addInfo);
			}
			else
			{
				findInfo->Add(new FGsTownInteractionNpcInfo(
					npcData->id, pos, npcData));
			}

		}

	}



	// 클라이언트 스폰 정보 가져오기
	TArray<TSharedPtr<FGsSpawnParam>> spawnList =
		param->_arrayNonPlayer;

	for (TSharedPtr<FGsSpawnParam> iter : spawnList)
	{
		const FGsSchemaNpcData* npcData = UGsTableUtil::FindRowById<UGsTableNpcData, FGsSchemaNpcData>(iter->_tId);
		if (nullptr == npcData)
		{
			continue;
		}

		if (npcData->npcFunctionType == NpcFunctionType::NONE)
		{
			continue;
		}

		_arrayClientSpawnInteractionNpcInfo.Add(new FGsInteractionNpcInfo(iter->_tId, iter->_pos));
	}

}

TArray<FGsTownInteractionNpcInfo*> FGsInteractionManager::GetTownNpcDataBySlotId(int In_slotId)
{
	TArray<FGsTownInteractionNpcInfo*> findInfo =
		_mapTownInteractionNpcInfo.FindRef(In_slotId);

	// check quest state by npc tbl id
	TArray<FGsTownInteractionNpcInfo*> checkedInfo;
	for (FGsTownInteractionNpcInfo* infoIter : findInfo)
	{
		if (infoIter == nullptr ||
			infoIter->_npcData == nullptr)
		{
			continue;
		}

		// if npc function type event prayer
		if (infoIter->_npcData->npcFunctionType == NpcFunctionType::EVENT_PRAYER)
		{
			bool isInteractionActive = (nullptr == GSContentsEvent()->GetUniqueActivatedContentsEvent<const FGsContentsEventNpcPrayer>()) ? false : true;
			if (isInteractionActive == false)
			{
				continue;
			}
		}

		if (GSClientSpawn()->IsQuestProgressShowNpcByTblId(infoIter->_npcId))
		{
			checkedInfo.Add(infoIter);
		}


	}
	return checkedInfo;
}
TArray<FGsTownInteractionNpcInfo*> FGsInteractionManager::GetServerTownNpcDataBySlotId(int In_slotId)
{
	TArray<FGsTownInteractionNpcInfo*> findInfo =
		_mapServerTownInteractionNpcInfo.FindRef(In_slotId);

	return findInfo;
}
// npc list info 클리어
void FGsInteractionManager::ClearNpcListInfo()
{
	if (_arrayClientSpawnInteractionNpcInfo.Num() != 0)
	{
		for (FGsInteractionNpcInfo* iter : _arrayClientSpawnInteractionNpcInfo)
		{
			if (iter != nullptr)
			{
				delete iter;
			}
		}
		_arrayClientSpawnInteractionNpcInfo.Empty();
	}

	if (_mapTownInteractionNpcInfo.Num() != 0)
	{
		for (auto iter : _mapTownInteractionNpcInfo)
		{
			if (0 != iter.Value.Num())
			{
				for (FGsTownInteractionNpcInfo* infoIter : iter.Value)
				{
					if (infoIter != nullptr)
					{
						infoIter->Finalize();
						delete infoIter;
					}
				}
				iter.Value.Empty();
			}
		}
		_mapTownInteractionNpcInfo.Empty();
	}

	if (_arrayServerSpawnInteractionNpcInfo.Num() != 0)
	{
		for (FGsInteractionNpcInfo* iter : _arrayServerSpawnInteractionNpcInfo)
		{
			if (iter != nullptr)
			{
				delete iter;
			}
		}
		_arrayServerSpawnInteractionNpcInfo.Empty();
	}

	if (_mapServerTownInteractionNpcInfo.Num() != 0)
	{
		for (auto iter : _mapServerTownInteractionNpcInfo)
		{
			if (0 != iter.Value.Num())
			{
				for (FGsTownInteractionNpcInfo* infoIter : iter.Value)
				{
					if (infoIter != nullptr)
					{
						infoIter->Finalize();
						delete infoIter;
					}
				}
				iter.Value.Empty();
			}
		}
		_mapServerTownInteractionNpcInfo.Empty();
	}
}

// 캐릭터 삭제될때
void FGsInteractionManager::RemoveCharacter()
{
	_localPlayer = nullptr;
}

void FGsInteractionManager::OnPropTargetUpdate(const IGsMessageParam* In_data)
{
	if (_localPlayer == nullptr)
	{
		return;
	}

	const FGsMinimapQuestTargetPropParam* interactionParam = In_data->Cast<const FGsMinimapQuestTargetPropParam>();
	if (nullptr == interactionParam)
	{
		return;
	}

	if (interactionParam->_isAdd)
	{
		return;
	}

	FGsInteractionHandlerLocalPlayer* localInteraction = _localPlayer->GetCastInteraction<FGsInteractionHandlerLocalPlayer>();
	if (nullptr == localInteraction)
	{
		return;
	}

	if (UGsGameObjectBase* target = localInteraction->GetInteractionTarget())
	{
		if (interactionParam->_tableId.Contains(target->GetData()->GetTableId()))
		{
			// 예약 정보 Clear
			localInteraction->CancelInteration(target);
		}
	}
	else
	{
		const FGsReserveInteractionInfo& reserveInfo = localInteraction->GetReserveInfo();
		if (reserveInfo._target._gameObjectType == EGsGameObjectType::PropObject ||
			reserveInfo._target._gameObjectType == EGsGameObjectType::PropInteraction ||
			reserveInfo._target._gameObjectType == EGsGameObjectType::PropInteractInvasion)
		{
			if (interactionParam->_tableId.Contains(reserveInfo._target._tblId))
			{
				localInteraction->ClearReserveInteraction();
			}
		}
	}
}

void FGsInteractionManager::OnInteractionInvasionRegister(const IGsMessageParam* In_param)
{
	const FGsPropInteractionMessageParamContents* castData = In_param->Cast<const FGsPropInteractionMessageParamContents>();

	if (castData == nullptr || _localPlayer == nullptr)
	{
		return;
	}

	if (auto findObject = Cast<UGsGameObjectPropTouchInvasion>(GSGameObject()->FindObject(castData->_objectType, castData->_gameId)))
	{
		findObject->GetData()->GetTableId();
		if (auto mapInfo = GSInterServer()->GetCurrentInvasionMapInfo().Pin())
		{
			auto level = GGameData()->GetUserData()->mLevel;

			if (level < mapInfo->GetValidLowLevel() && level > mapInfo->GetValidHighLevel())
			{
				//Lv. {0} ~ Lv. {1} 유저만 입장할 수 있습니다
				FText Alert_NoLevel;
				FText::FindText(TEXT("InvadeText"), TEXT("Alert_NoLevel"), Alert_NoLevel);

				FText enableLevelText = FText::Format(Alert_NoLevel, { mapInfo->GetValidLowLevel(), mapInfo->GetValidHighLevel() });
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, enableLevelText);

				findObject->OnCancelInvasionInterAct();
				return;
			}

			//auto enrolledCount = findObject->GetEnrolledPlayerCountInProp(); //  GSInterServer()->GetInvaderCountEnrollingInPortal(castData->_gameId);
			//auto maxRegisterCount = mapInfo->GetEnrollingMaxCountInPortal();
			//auto isEnableEnrolling = GSInterServer()->IsEnableEnrollingPortal();
			//if (!isEnableEnrolling || enrolledCount >= maxRegisterCount)
			//{
			//	//“더 이상 포탈을 정화할 수 없습니다.” 
			//	FText Alert_MaxUser;
			//	FText::FindText(TEXT("InvadeText"), TEXT("Alert_MaxUser"), Alert_MaxUser);
			//	FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, Alert_MaxUser);
			//	return;
			//}

			findObject->OnStartInvasionInterAct();

			/*if (auto popup = Cast<UGsUIPopupCommonPay>(GUI()->OpenAndGetWidget(TEXT("PopupCommonPay")).Get()))
			{
				if (const FGsSchemaInvadeWorldConfigDataEx* invasionInfo = UGsTableUtil::FindFirstRow<FGsSchemaInvadeWorldConfigDataEx>())
				{
					if (auto itemData = FGsItem::GetTableDataByID(invasionInfo->invadePortalTicketItemId.GetRow()->id))
					{
						//{0}을 소모하여 {1}을 정화하시겠습니까?
						FText Popup_InteractionPortal;
						FText::FindText(TEXT("InvadeText"), TEXT("Popup_InteractionPortal"), Popup_InteractionPortal);

						FText title = FText::Format(Popup_InteractionPortal, { itemData->name, findObject->GetPropTableData()->nameString });

						TWeakObjectPtr<UGsGameObjectPropTouchInvasion> useProp = findObject;

						popup->SetData(title, FGsCostPackage::CreateSimple(itemData->id, 1), [useProp](bool isOK) {
							if (isOK)
							{
								if (useProp.IsValid()) useProp->OnStartRegisterInteract();
							}
							});
					}
				}
			}*/
		}
	}
}

void FGsInteractionManager::OnInteractionNpcContents(const IGsMessageParam* In_data)
{
	// 튜토리얼 중엔 인터렉션을 막음
	if (GSTutorial()->IsActiveTutorial())
	{
		return;
	}

	// https://jira.com2us.com/jira/browse/C2URWQ-2476
	// check warp state
	if (_localPlayer == nullptr)
	{
		return;
	}
	if (FGsGameObjectStateManager* fsm = _localPlayer->GetBaseFSM())
	{
		if (fsm->IsState(EGsStateBase::Warp))
		{
			return;
		}
	}
	// block ui 안켜져있네...
	// 이체크는 뺌
	// 워프 스크롤 사용 했는지 체크
	if (_currentConsumeItemCategory == ItemCategorySub::WARP_TO_TOWN_SCROLL ||
		_currentConsumeItemCategory == ItemCategorySub::WARP_TO_RANDOM_SCROLL ||
		_currentConsumeItemCategory == ItemCategorySub::WARP_TO_BOOKMARK_SCROLL ||
		_currentConsumeItemCategory == ItemCategorySub::WARP_TO_AGIT)
	{
		return;
	}

	// guild dungeon enter
	if (_guildDungeonWarpReq == true)
	{
		return;
	}

	// a 퀘스트 위치 도착 서버 send전에
	// b 퀘스트 이동 요청시(액티브 된 퀘스트)
	// a 퀘스트 서버 send 막기
	// https://jira.com2us.com/jira/browse/C2URWQ-5394
	UGsQuestManager* questManager = GSQuest();
	if (questManager == nullptr)
	{
		return;
	}

	FGsAIManager* aiMgr = GSAI();
	if (nullptr == aiMgr)
	{
		return;
	}
	FGsAIQuestHandler* aiQuestHandler = aiMgr->GetAIQuestHandler();
	if (nullptr == aiQuestHandler)
	{
		return;
	}

	const FGsInteractionMessageParamContents* interactionParam =
		In_data->Cast<const FGsInteractionMessageParamContents>();

	int npcTbID = interactionParam->_npcTblId;

	// 퀘스트 액티브이면서 자동이동 데이터 있는데
	if (true == questManager->IsAutoProgressing() &&
		true == aiQuestHandler->IsReserveQuestMoveData())
	{
		// 인터랙션 npc 가 아니면 이미 다른 이동으로 바뀐거다
		// 유효하지 않으므로 서버 send 안함
		if (QuestObjectiveType::INTERACT_NPC != aiQuestHandler->GetQuestAutoMoveType())
		{
			return;
		}
		// 인터랙션 npc 면서도 npc id 가 다르면 유효하지 않음
		else if (aiQuestHandler->GetQuestMoveTargetNpcId() != npcTbID)
		{
			return;
		}
	}

	// 파티장에게 이동시 이미 워프 중이면
	// 대화연출 서버 send 안함
	if (true == FGsUIHelper::IsOpenBlockUI())
	{
		return;
	}


	const FGsSchemaNpcData* npcData = UGsTableUtil::FindRowById<UGsTableNpcData, FGsSchemaNpcData>(npcTbID);
	if (nullptr == npcData)
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("FGsSchemaNpcData::FindRowById is EmptyData - npcTbID : %d"), npcTbID);
#endif
		return;
	}

	_reserveNpcTblId = interactionParam->_npcTblId;
	_reserveGameId = interactionParam->_gameId;
	_reserveSpawnId = interactionParam->_spawnId;
	_reserveNpcFunctionType = npcData->npcFunctionType;

	if (npcData->npcFunctionType == NpcFunctionType::QUEST_DIALOG)
	{
		if (GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::QUEST, true))
		{
			GSEventProgress()->OnReqStartEvent(EGsEventProgressType::NPC_DIALOG, this);
		}
	}
	else if (
		npcData->npcFunctionType == NpcFunctionType::SHOP ||
		npcData->npcFunctionType == NpcFunctionType::SHOP_EQUIP ||
		npcData->npcFunctionType == NpcFunctionType::SHOP_SKILL_BOOK ||
		npcData->npcFunctionType == NpcFunctionType::EVENT_SHOP
		)
	{
		if (GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::NPC_SHOP, true))
		{
			GSEventProgress()->OnReqStartEvent(EGsEventProgressType::NPC_SHOP, this);
		}
	}
	else if (npcData->npcFunctionType == NpcFunctionType::DEPOT)
	{
		if (GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::DEPOT, true))
		{
			GSEventProgress()->OnReqStartEvent(EGsEventProgressType::NPC_SHOP, this);
		}
	}
	else if (npcData->npcFunctionType == NpcFunctionType::CAMP)
	{
		FGsNetSendServiceWorld::SendTryInteract(
			_reserveNpcTblId, _reserveSpawnId, _reserveGameId);
	}
	else if (npcData->npcFunctionType == NpcFunctionType::SANCTUM_NEXUS)
	{
		FGsNetSendServiceWorld::SendTryInteract(
			_reserveNpcTblId, _reserveSpawnId, _reserveGameId);
	}
	// 타입이 부활이고
	// 페널티값이 있으면
	else if (npcData->npcFunctionType == NpcFunctionType::REVIVAL)
	{
		if (GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::REVIVE, true))
		{
			GSEventProgress()->OnReqStartEvent(EGsEventProgressType::UI_POPUP_RECOVERY_PENALTY, this);
		}
	}
	else if (npcData->npcFunctionType == NpcFunctionType::COMMUNITY)
	{
		if (GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::COMMUNITY, true))
		{
			GSEventProgress()->OnReqStartEvent(EGsEventProgressType::COMMUNITY_ROOM, this);
		}
	}
	//게시판 Npc function type 추가 
	else if (npcData->npcFunctionType == NpcFunctionType::QUEST_BOARD)
	{
		if (GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::QUEST_BOARD, true))
		{
#if ACTIVE_QUEST_BOARD_EVENT_PROGRESS
			GSEventProgress()->OnReqStartEvent(EGsEventProgressType::QUEST_BOARD, this, GSTutorial()->IsActiveTutorial());
#else
			if (FGsGameStateQuestBoard* board = FGsGameStateQuestBoard::GetInstance())
			{
				board->InitializeSetting();
				board->SetSelectFirst(true);
			}

			FGsNetSendServiceQuest::SendReqQuestList(_reserveNpcTblId, QuestBoardType::VILLAGE);
#endif
		}
	}
	else if (npcData->npcFunctionType == NpcFunctionType::EVENT_PRAYER)
	{
		if (GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::PRAYER, true))
		{
			GSEventProgress()->OnReqStartEvent(EGsEventProgressType::UI_POPUP_EVENT_PRAYER, this);
		}
	}
	else if (npcData->npcFunctionType == NpcFunctionType::AGIT_DUNGEON)
	{
		if (GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::AGIT_DUNGEON_RAID, true))
		{
			GSEventProgress()->OnReqStartEvent(EGsEventProgressType::AGIT_DUNGEON, this);
		}
	}
	else
	{
		GMessage()->GetGameObject().SendMessage(MessageGameObject::INTERACTION_CONTENTS_BY_NPC_START, interactionParam);
	}

	// Shop , Depot 타입이면 대상 local 바라보게 처리
	// 퀘스트 대화는 대화 연출에서 처리(안바라보는 타입도 있으므로)
	if (npcData->npcFunctionType == NpcFunctionType::SHOP ||
		npcData->npcFunctionType == NpcFunctionType::EVENT_SHOP ||
		npcData->npcFunctionType == NpcFunctionType::SHOP_EQUIP ||
		npcData->npcFunctionType == NpcFunctionType::SHOP_SKILL_BOOK ||
		npcData->npcFunctionType == NpcFunctionType::DEPOT)
	{
		if (0 < interactionParam->_gameId)
		{
			UGsGameObjectBase* findObject = GSGameObject()->FindObject(interactionParam->_objectType, interactionParam->_gameId);
			if (nullptr != findObject && _localPlayer != nullptr)
			{
				findObject->LookAtTarget(_localPlayer, true);
			}
		}
	}
}

void FGsInteractionManager::RecvTryInteractAck(PD::SC::PKT_SC_ACK_TRY_INTERACT_READ& In_packet,
	UGsGameObjectBase* In_findObj, int64 In_gameId)
{
	if (_localPlayer == nullptr)
	{
		return;
	}

	// [B1] | ejrrb10 | SETTING_WAIT_SERVER_RESPONSE 관련 인풋을 막았던 것을 여기서 해제하도록 하겠습니다.
	FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::SETTING_WAIT_SERVER_RESPONSE, false);
	GMessage()->GetInput().SendMessage(MessageInput::BlockInput, msg);

	// 퀘스트 대화 진행중(서버 응답 대기중) 다른 이벤트가 전환 해버리면
	// 여기서 진행 안하도록 한다(ex: 대화연출과 프로필을 동시에 실행할때 ui 이상하게 나오는현상)
	if (_reserveNpcFunctionType == NpcFunctionType::QUEST_DIALOG)
	{
		// 이미 다른넘이 바꾸었다면 유효하지 않음
		if (EGsEventProgressType::NPC_DIALOG != GSEventProgress()->GetCurrentProgressType())
		{
			// 퀘스트 액티브 끄기
			if (FGsQuestHandler* questHandler = _localPlayer->GetQuestHandler())
			{
				questHandler->SetAutoMoveStop();
			}
			return;
		}
	}


	SpawnId spawnId = In_packet.InteractNpcSpawnId();
	int64 gameId = In_gameId;

	UGsGameObjectBase* object = In_findObj;

	if (object == nullptr)
	{
		return;
	}

	int npcTblId = 0;
	if (object->GetObjectType() == EGsGameObjectType::NonPlayer)
	{
		if (UGsGameObjectNonPlayer* nonPlayer = Cast<UGsGameObjectNonPlayer>(object))
		{
			npcTblId = nonPlayer->GetNpcTblId();
		}
	}
	else if (object->GetObjectType() == EGsGameObjectType::Camp)
	{
		// In_findObj 로 캐스팅해야함
		if (UGsGameObjectCamp* campObject = Cast<UGsGameObjectCamp>(In_findObj))
		{
			npcTblId = campObject->GetNpcTblId();
		}
	}
	else if (object->GetObjectType() == EGsGameObjectType::Sanctum)
	{
		if (UGsGameObjectNonPlayerBase* campObject = Cast<UGsGameObjectNonPlayerBase>(In_findObj))
		{
			npcTblId = campObject->GetNpcTblId();
		}
	}

	// 대화 타입 메세지 전송
	FGsInteractionMessageParamContents sendParam(npcTblId, spawnId, gameId);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::INTERACTION_CONTENTS_BY_NPC_START, &sendParam);
}

void FGsInteractionManager::RecvNpcEventShopOpenAck(PD::SC::PKT_SC_ACK_NPC_EVENT_SHOP_OPEN_READ& In_packet)
{
	int64 gameId = In_packet.InteractNpcGameId();
	SpawnId spawnId = 0; // 서버 스폰

	UGsGameObjectBase* object = GSGameObject()->FindObject(EGsGameObjectType::NonPlayer, gameId);
	if (nullptr == object)
		return;

	int npcTblId = 0;
	if (UGsGameObjectNonPlayer* nonPlayer = Cast<UGsGameObjectNonPlayer>(object))
	{
		npcTblId = nonPlayer->GetNpcTblId();
	}

	FGsInteractionMessageParamContents sendParam(npcTblId, spawnId, gameId);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::INTERACTION_CONTENTS_BY_NPC_START, &sendParam);

	FGsUpdatePurchasedItem purchasedItemParam;
	In_packet.MakePurchasedListVector<TArray<ItemIdAmountPair>, ItemIdAmountPair>(purchasedItemParam._purchasedItemList);
	GMessage()->GetNpcShop().SendMessage(MessageContentNpcShop::UPDATE_PURCHASE_AMOUNT, &purchasedItemParam);
}

void FGsInteractionManager::RecvEnterTerritoryEventShopData(PD::SC::PKT_SC_NOTIFY_ENTER_TERRITORY_EVENT_SHOP_DATA_READ& In_packet)
{
	// clear old data
	const FGsSchemaNpcFunctionData* npcFunctionData =
		UGsTableUtil::FindRowById<UGsTableNpcFunctionData, FGsSchemaNpcFunctionData>(NpcFunctionType::EVENT_SHOP);

	int slotNumber = GetSlotOrderNumber(npcFunctionData);	

	if (slotNumber == -1)
	{
		return;
	}
	TArray<FGsTownInteractionNpcInfo*>* findInfo =
		_mapServerTownInteractionNpcInfo.Find(slotNumber);

	// 이벤트 활성화 되어서 추가된건지(1개만 들어오는지)
	// true: 1개만 활성화(기존꺼 나머지껀 놔둠)
	// false: 마을 들어와서 다내려 주는거(기존꺼 다 삭제)
	// https://jira.com2us.com/jira/browse/C2URWQ-5923
	bool isEventAdd = In_packet.EventAdd();

	const int eventShopCount = In_packet.GetEventShopListCount();

	// not exist new add data
	if (eventShopCount == 0)
	{
		return;
	}


	// 로컬의 iff를 구한다(그림자 전장에서 아군, 적군 npc 구별용)
	// https://jira.com2us.com/jira/browse/CHR-24545
	FGsGameObjectData* localObjData = _localPlayer->GetData();
	if (localObjData == nullptr)
	{
		return;
	}

	IffFactionType localIffFactionType = localObjData->GetIff().mIffFactionType;
	MapType currentMapType = GLevel()->GetCurrentMapType();

	// false 기존대로 처리
	if(isEventAdd == false)
	{ 
		// exist old data
		if (findInfo != nullptr)
		{
			if (findInfo->Num() != 0)
			{
				for (FGsTownInteractionNpcInfo* infoIter : *findInfo)
				{
					if (infoIter != nullptr)
					{
						infoIter->Finalize();
						delete infoIter;
					}
				}
				findInfo->Empty();
			}

			_mapServerTownInteractionNpcInfo.Remove(slotNumber);
		}

		TArray<FGsTownInteractionNpcInfo*> addInfo;

		int currentMapId = GLevel()->GetCurrentLevelId();

		using eventShopIter = PktIterator<PD::SC::PKT_SC_NOTIFY_ENTER_TERRITORY_EVENT_SHOP_DATA_READ::EventShopList>;
		for (eventShopIter iter = In_packet.GetFirstEventShopListIterator(); iter != In_packet.GetLastEventShopListIterator(); ++iter)
		{
			int mapId = iter->MapId();

			// only use current map id
			if (mapId != currentMapId)
			{
				continue;
			}

			int npcTblId = iter->NpcId();

			const FGsSchemaNpcData* npcData = UGsTableUtil::FindRowById<UGsTableNpcData, FGsSchemaNpcData>(npcTblId);
			if (nullptr == npcData)
			{
				continue;
			}

			// 그림자 전장에서 적 iff npc 정보는 뺀다
			//https://jira.com2us.com/jira/browse/CHR-24545
			if (currentMapType == MapType::GUILD_WAR)
			{
				if (true == IsEnemyIffShadowBattleField(localIffFactionType, npcData->iffFactionType))
				{
					continue;
				}
			}

			const FGsSchemaSpawnData* spawnData =
				GLevel()->GetZone(currentMapId, iter->SpawnGroupId());

			if (spawnData == nullptr)
			{
				continue;
			}
			FVector npcPos =
				spawnData->Region.Center;

			addInfo.Add(new FGsTownInteractionNpcInfo(
				npcTblId, npcPos, npcData));


			// already exist del
			int index =
				_arrayServerSpawnInteractionNpcInfo.IndexOfByPredicate([npcTblId](FGsInteractionNpcInfo* el)
					{
						return el->_npcId == npcTblId;
					});

			if (INDEX_NONE != index)
			{
				delete _arrayServerSpawnInteractionNpcInfo[index];
				_arrayServerSpawnInteractionNpcInfo.RemoveAt(index);
			}

			_arrayServerSpawnInteractionNpcInfo.Add(new FGsInteractionNpcInfo(npcTblId, npcPos));
		}

		// 정렬(npc id 순서 오름차순)
		addInfo.Sort([](FGsTownInteractionNpcInfo& lhs, FGsTownInteractionNpcInfo& rhs)
			{
				return lhs._npcId < rhs._npcId;
			});

		_mapServerTownInteractionNpcInfo.Add(slotNumber, addInfo);
	}
	// true 면 하나만 삭제 및 추가
	else
	{


		TArray<FGsTownInteractionNpcInfo*> addInfo;

		int currentMapId = GLevel()->GetCurrentLevelId();

		using eventShopIter = PktIterator<PD::SC::PKT_SC_NOTIFY_ENTER_TERRITORY_EVENT_SHOP_DATA_READ::EventShopList>;
		for (eventShopIter iter = In_packet.GetFirstEventShopListIterator(); iter != In_packet.GetLastEventShopListIterator(); ++iter)
		{
			int mapId = iter->MapId();

			// only use current map id
			if (mapId != currentMapId)
			{
				continue;
			}

			int npcTblId = iter->NpcId();

			const FGsSchemaNpcData* npcData = UGsTableUtil::FindRowById<UGsTableNpcData, FGsSchemaNpcData>(npcTblId);
			if (nullptr == npcData)
			{
				continue;
			}

			// 그림자 전장에서 적 iff npc 정보는 뺀다
			//https://jira.com2us.com/jira/browse/CHR-24545
			if (currentMapType == MapType::GUILD_WAR)
			{
				if (true == IsEnemyIffShadowBattleField(localIffFactionType, npcData->iffFactionType))
				{
					continue;
				}
			}

			const struct FGsSchemaSpawnData* spawnData =
				GLevel()->GetZone(currentMapId, iter->SpawnGroupId());

			if (spawnData == nullptr)
			{
				continue;
			}
			FVector npcPos =
				spawnData->Region.Center;

			// 기존에 없던 정보면 추가
			if (findInfo == nullptr)
			{ 
				addInfo.Add(new FGsTownInteractionNpcInfo(
					npcTblId, npcPos, npcData));
			}
			else
			{
				bool isFind = false;
				if (findInfo->Num() != 0)
				{					
					for (FGsTownInteractionNpcInfo* infoIter : *findInfo)
					{
						if (infoIter != nullptr)
						{
							if (infoIter->_npcId == npcTblId)
							{
								isFind = true;

								// 값 변경
								infoIter->_npcId = npcTblId;
								infoIter->_pos = npcPos;
								infoIter->_npcData = npcData;
							}
						}
					}					
				}
				// 기존에 없으면 추가
				if(isFind == false)
				{
					findInfo->Add(new FGsTownInteractionNpcInfo(
						npcTblId, npcPos, npcData));
				}
			}



			// already exist del
			int index =
				_arrayServerSpawnInteractionNpcInfo.IndexOfByPredicate([npcTblId](FGsInteractionNpcInfo* el)
					{
						return el->_npcId == npcTblId;
					});

			if (INDEX_NONE != index)
			{
				delete _arrayServerSpawnInteractionNpcInfo[index];
				_arrayServerSpawnInteractionNpcInfo.RemoveAt(index);
			}

			_arrayServerSpawnInteractionNpcInfo.Add(new FGsInteractionNpcInfo(npcTblId, npcPos));
		}
		// 기존에 없던 정보면 추가
		if (findInfo == nullptr)
		{ 
			// 정렬(npc id 순서 오름차순)
			addInfo.Sort([](FGsTownInteractionNpcInfo& lhs, FGsTownInteractionNpcInfo& rhs)
				{
					return lhs._npcId < rhs._npcId;
				});
			_mapServerTownInteractionNpcInfo.Add(slotNumber, addInfo);
		}
		else
		{
			// 정렬(npc id 순서 오름차순)
			findInfo->Sort([](FGsTownInteractionNpcInfo& lhs, FGsTownInteractionNpcInfo& rhs)
				{
					return lhs._npcId < rhs._npcId;
				});
		}
	}

}
void FGsInteractionManager::RecvLeaveTerritoryEventShopData(PD::SC::PKT_SC_NOTIFY_LEAVE_TERRITORY_EVENT_SHOP_DATA_READ& In_packet)
{
	RemoveEventShopServerTownInteractionInfo(In_packet.NpcId());
}

void FGsInteractionManager::RemoveEventShopServerTownInteractionInfo(int In_npcTblId)
{
	const FGsSchemaNpcFunctionData* npcFunctionData =
		UGsTableUtil::FindRowById<UGsTableNpcFunctionData, FGsSchemaNpcFunctionData>(NpcFunctionType::EVENT_SHOP);

	int slotNumber = GetSlotOrderNumber(npcFunctionData);

	if (slotNumber == -1)
	{
		return;
	}

	TArray<FGsTownInteractionNpcInfo*>* findInfo =
		_mapServerTownInteractionNpcInfo.Find(slotNumber);

	// exist old data
	if (findInfo != nullptr)
	{
		if (findInfo->Num() != 0)
		{
			int removeId = -1;
			for (int i = 0; i < findInfo->Num(); ++i)
			{
				FGsTownInteractionNpcInfo* infoIter = (*findInfo)[i];
				if (infoIter != nullptr &&
					infoIter->_npcId == In_npcTblId)
				{
					infoIter->Finalize();
					delete infoIter;

					removeId = i;
					break;
				}
			}

			if (removeId != -1)
			{
				findInfo->RemoveAt(removeId);
			}
		}

		if (findInfo->Num() == 0)
		{
			_mapServerTownInteractionNpcInfo.Remove(slotNumber);
		}
	}
}

// 이벤트 시작
void FGsInteractionManager::StartEvent(EGsEventProgressType In_startType, EGsEventProgressType In_preType)
{
	// 인터렉션 시 카메라 모드 중지
	if (EGsUIHUDMode::CAMERA == GUI()->GetHUDMode())
	{
		GUI()->SetHUDMode(EGsUIHUDMode::NORMAL);
	}

	if (_reserveNpcFunctionType == NpcFunctionType::NONE)
	{
		return;
	}
	if (_reserveNpcFunctionType == NpcFunctionType::QUEST_DIALOG ||
		_reserveNpcFunctionType == NpcFunctionType::SHOP ||
		_reserveNpcFunctionType == NpcFunctionType::SHOP_EQUIP ||
		_reserveNpcFunctionType == NpcFunctionType::SHOP_SKILL_BOOK ||
		_reserveNpcFunctionType == NpcFunctionType::DEPOT)
	{
		// lock input
		// https://jira.com2us.com/jira/browse/C2URWQ-2476
		// 인풋 막기(줌, 회전)
		//FGsInputEventMsgBase msg;
		//msg.Id = 1;
		FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::SETTING_WAIT_SERVER_RESPONSE, true);
		GMessage()->GetInput().SendMessage(MessageInput::BlockInput, msg);


		if (0 < _reserveSpawnId)
		{
			FGsNetSendServiceWorld::SendTryInteract(
				_reserveNpcTblId, _reserveSpawnId, 0);
		}
		else
		{
			FGsNetSendServiceWorld::SendTryInteract(
				_reserveNpcTblId, 0, _reserveGameId);
		}
	}
	else if (_reserveNpcFunctionType == NpcFunctionType::EVENT_SHOP)
	{
		FGsNetSendServiceWorld::SendTryNpcEventShopOpen(_reserveGameId);
	}
	else if (_reserveNpcFunctionType == NpcFunctionType::COMMUNITY)
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_COMMUNITY);
	}
	else if (_reserveNpcFunctionType == NpcFunctionType::AGIT_DUNGEON)
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_AGIT_DUNGEON);
	}
	else if (_reserveNpcFunctionType == NpcFunctionType::REVIVAL)
	{
		int32 totalCount = FGsRecoveryPenaltyFunc::GetRecoveryPenaltyCount();
		if (totalCount != 0)
		{
			GUI()->Open(TEXT("PopupRecoveryPenalty"));
		}
	}
	//게시판 추가해야함
	else if (_reserveNpcFunctionType == NpcFunctionType::QUEST_BOARD)
	{
		//퀘스트 게시판 열기 전 초기화 
		if (FGsGameStateQuestBoard* board = FGsGameStateQuestBoard::GetInstance())
		{
			board->InitializeSetting();
			board->SetSelectFirst(true);
		}

		FGsNetSendServiceQuest::SendReqQuestList(_reserveNpcTblId, QuestBoardType::VILLAGE);
	}
	else if (_reserveNpcFunctionType == NpcFunctionType::CAMP)
	{
		FGsNetSendServiceWorld::SendTryInteract(
			_reserveNpcTblId, _reserveSpawnId, _reserveGameId);
	}
	else if (_reserveNpcFunctionType == NpcFunctionType::SANCTUM ||
		_reserveNpcFunctionType == NpcFunctionType::SANCTUM_NEXUS)
	{
		FGsNetSendServiceWorld::SendTryInteract(
			_reserveNpcTblId, _reserveSpawnId, _reserveGameId);
	}
	else if (_reserveNpcFunctionType == NpcFunctionType::EVENT_PRAYER)
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupEventPrayer"));
		if (widget.IsValid())
		{
			if (UGsUIPopupEventPrayer* popupEventPrayer = Cast<UGsUIPopupEventPrayer>(widget.Get()))
			{

				const UGsTable* eventNpcPrayerDataTbl = FGsSchemaContentsEventNpcPrayer::GetStaticTable();

				if (nullptr == eventNpcPrayerDataTbl)
				{
					return;
				}

				TArray<const FGsSchemaContentsEventNpcPrayer*> rowsEventNpcData;
				eventNpcPrayerDataTbl->GetAllRows<FGsSchemaContentsEventNpcPrayer>(rowsEventNpcData);
				if (rowsEventNpcData.Num() == 0)
				{
					return;
				}

				FText buffName;
				FText buffDetail;
				for (const FGsSchemaContentsEventNpcPrayer* iter : rowsEventNpcData)
				{
					if (nullptr == iter
						|| nullptr == iter->npcData.GetRow())
					{
						continue;
					}

					if (iter->npcData.GetRow()->id == _reserveNpcTblId)
					{
						buffName = iter->blessName;
						buffDetail = iter->blessDesc;
						break;
					}
				}

				popupEventPrayer->SetEventPrayerData(_reserveNpcTblId, buffName, buffDetail);
			}
		}
	}
}
// 이벤트 종료
void FGsInteractionManager::FinishEvent(EGsEventProgressType In_finishType, EGsEventProgressType In_nextType)
{
	// 대사연출, shop 열린상태에서 강제로 외부에서 닫을일이 없을듯
	// 있다면 
	// 대사연출: 카메라 줌 상태 바로 복구하는 처리+ ui 복구 처리 필요
	// shop: ui 복구처리 필요


	// 커뮤니티였으면
	if (_reserveNpcFunctionType == NpcFunctionType::COMMUNITY)
	{
		// 커뮤니티 소녀 깨짐현상
		// https://jira.com2us.com/jira/browse/C2URWQ-5168
		GMessage()->GetCommunity().SendMessage(MessageContentCommunity::FINISH_EVENT_PROGRESS, nullptr);
		// main hud로 이동
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_HUD);
	}
	// 페널티 복구면
	else if (_reserveNpcFunctionType == NpcFunctionType::REVIVAL)
	{
		GUI()->CloseByKeyName(TEXT("PopupRecoveryPenalty"));
	}

#if ACTIVE_QUEST_BOARD_EVENT_PROGRESS
	if (In_finishType == EGsEventProgressType::QUEST_BOARD)
	{
		// main hud로 이동
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_HUD);
	}
#endif

	// 인풋 막기 풀기
	FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::SETTING_WAIT_SERVER_RESPONSE, false);
	GMessage()->GetInput().SendMessage(MessageInput::BlockInput, msg);
}

// 위젯 버튼 추가 
void FGsInteractionManager::AddWidgetComponentButton(UGsWidgetComponentButton* In_wcButton)
{
	if (_arrayWCButton.Contains(In_wcButton))
	{
		return;
	}

	_arrayWCButton.Add(In_wcButton);
}
// 위젯 버튼 삭제
void FGsInteractionManager::DelWidgetComponentButton(UGsWidgetComponentButton* In_wcButton)
{
	if (false == _arrayWCButton.Contains(In_wcButton))
	{
		return;
	}

	_arrayWCButton.Remove(In_wcButton);
}

// 인터랙션 버튼 클릭 체크
bool FGsInteractionManager::CheckInteractionButtonClick()
{
	for (auto iter : _arrayWCButton)
	{
		if (iter->ManageTouchEvent() == true)
		{
			return true;
		}
	}
	return false;
}

void FGsInteractionManager::InteractionErrorMessage(int32 inError)
{
	FText findText;
	switch (inError)
	{
	case PD::Result::SPACE_CRACK_ERROR_ENTRY_TIME:
		FText::FindText(TEXT("SpaceCrackText"), TEXT("NoticeEntryCooltime"), findText);
		break;
	case PD::Result::SPACE_CRACK_ERROR_PLAY_LIMIT_COUNT:
		FText::FindText(TEXT("SpaceCrackText"), TEXT("ErrorCrackPlayLimitCount"), findText);
		break;
	case PD::Result::INVADE_ERROR_INVALID_LEVEL:
	case PD::Result::INVADE_ERROR_INVALID_TIME:
	case PD::Result::INVADE_ERROR_ENTER_MAX_COUNT:
	case PD::Result::INVADE_ERROR_NOT_ENOUGH_TIME:
	case PD::Result::WARP_ERROR_NOT_ENOUGH_INVADE_COST:
	{
		FGsUIHelper::TraySectionMessageTickerNetError(static_cast<PD::Result>(inError));
		return;
	}
	case PD::Result::ITEM_ERROR_ADD_INVENTORY_FULL:
	{
		// 가방이 가득찼습니다.		
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Inventory_Full"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		return;
	}
	case PD::Result::ITEM_ERROR_ADD_WEIGHT_FULL:
	{
		// 소지 무게가 최대치입니다.		
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Weight_Full"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		return;
	}
	case PD::Result::INTERACT_ERROR_INVALID_PROP_START_USERCOUNT_OVER:
	{
		FText::FindText(TEXT("UICommonText"), TEXT("Alert_Interact_StartUserCountOver"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		return;
	}
	case PD::Result::INTERACT_ERROR_INVALID_PROP_MAX_USERCOUNT_LIMIT_OVER:
	{
		FText::FindText(TEXT("UICommonText"), TEXT("Alert_Interact_MaxUserCountOver"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		return;
	}
	case PD::Result::INTERACT_ERROR_INVALID_PROP_START_ALREADY:
	{
		FText::FindText(TEXT("UICommonText"), TEXT("Interact_Error_Repetition"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		return;
	}
	case PD::Result::INTERACT_ERROR_INVALID_PROP_NOT_ENDABLE:
	case PD::Result::INTERACT_ERROR_INVALID_STATE:
	case PD::Result::INTERACT_ERROR_NOT_ALIVE:
	{
		FText::FindText(TEXT("UICommonText"), TEXT("Alert_Interact_InvalidState"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		return;
	}
	}

	if (false == findText.IsEmpty())
	{
		FGsUIHelper::TrayMessageTicker(findText);
		return;
	}

	FGsUIHelper::PopupNetError((PD::Result)inError);
}
const FGsSchemaNpcFunctionData* FGsInteractionManager::FindNpcFunctionDataBySlotId(int In_slotId)
{
	const UGsTable* npcTblDataTbl = FGsSchemaNpcFunctionData::GetStaticTable();


	TArray<const FGsSchemaNpcFunctionData*> rowsNpcFunctionData;
	npcTblDataTbl->GetAllRows<FGsSchemaNpcFunctionData>(rowsNpcFunctionData);

	if (rowsNpcFunctionData.Num() == 0)
	{
		return nullptr;
	}

	MapType currentMapType = GLevel()->GetCurrentMapType();

	for (const FGsSchemaNpcFunctionData* iter : rowsNpcFunctionData)
	{
		if (nullptr == iter)
		{
			continue;
		}

		bool isNpcSlotBattleArena = IsBranch(EGsGameClientBranchType::NPC_SLOT_BATTLE_ARENA);

		// 이전 방식은 슬롯 번호 하나임
		if(isNpcSlotBattleArena == false)
		{
			if (iter->_slotOrderNumber == In_slotId)
			{
				return iter;
			}
		}
		else
		{
			if (currentMapType == MapType::GUILD_WAR)
			{
				if (iter->_slotOrderNumberBattleArena == In_slotId)
				{
					return iter;
				}
			}
			// 기사단 영지(아지트) 추가
			// https://jira.com2us.com/jira/browse/CHR-24721
			else if (currentMapType == MapType::AGIT)
			{
				if (iter->_slotOrderNumberAgit == In_slotId)
				{
					return iter;
				}
			}
			else
			{
				if (iter->_slotOrderNumber == In_slotId)
				{
					return iter;
				}
			}
		}
	}

	return nullptr;
}

//https://jira.com2us.com/jira/browse/C2URWQ-3121
// quest progress fail work
void FGsInteractionManager::OnInteractionContentsByQuestCancle(const IGsMessageParam* In_param)
{
	// release input lock
	//FGsInputEventMsgBase msg;
	//msg.Id = 0;
	FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::SETTING_WAIT_SERVER_RESPONSE, false);
	GMessage()->GetInput().SendMessage(MessageInput::BlockInput, msg);
}

void FGsInteractionManager::TryConsumeItem(const FGsItem* InItem)
{
	if (InItem == nullptr)
	{
		return;
	}

	ItemCategorySub itemCategory = InItem->GetSubCategory();

	if (itemCategory != ItemCategorySub::WARP_TO_TOWN_SCROLL &&
		itemCategory != ItemCategorySub::WARP_TO_RANDOM_SCROLL &&
		itemCategory != ItemCategorySub::WARP_TO_BOOKMARK_SCROLL &&
		itemCategory != ItemCategorySub::WARP_TO_AGIT)
	{
		return;
	}

	_currentConsumeItemCategory = itemCategory;
}

void FGsInteractionManager::ClearCurrentConsumeCategory()
{
	if (_currentConsumeItemCategory != ItemCategorySub::WARP_TO_TOWN_SCROLL &&
		_currentConsumeItemCategory != ItemCategorySub::WARP_TO_RANDOM_SCROLL &&
		_currentConsumeItemCategory != ItemCategorySub::WARP_TO_BOOKMARK_SCROLL &&
		_currentConsumeItemCategory != ItemCategorySub::WARP_TO_AGIT)
	{
		return;
	}

	_currentConsumeItemCategory = ItemCategorySub::NONE;
}

// 슬롯 넘버 구하기(그림자 전장 대응)
// https://jira.com2us.com/jira/browse/CHR-24545
int FGsInteractionManager::GetSlotOrderNumber(const FGsSchemaNpcFunctionData* In_findData)
{
	if(In_findData == nullptr)
	{
		return -1;
	}

	bool isNpcSlotBattleArena = IsBranch(EGsGameClientBranchType::NPC_SLOT_BATTLE_ARENA);

	int findSlotOrderNumber = -1;

	// 이전 방식, 슬롯 넘버는 한개만 있음
	if(isNpcSlotBattleArena == false)
	{
		findSlotOrderNumber = In_findData->_slotOrderNumber;
	}
	else
	{
		MapType currentMapType = GLevel()->GetCurrentMapType();

		// 그림자 전장일때는 다른 값 참조		
		if (currentMapType == MapType::GUILD_WAR)
		{
			findSlotOrderNumber = In_findData->_slotOrderNumberBattleArena;
		}
		// 기사단 영지(아지트) 추가
		// https://jira.com2us.com/jira/browse/CHR-24721
		else if (currentMapType == MapType::AGIT)
		{
			findSlotOrderNumber = In_findData->_slotOrderNumberAgit;
		}
		else
		{
			findSlotOrderNumber = In_findData->_slotOrderNumber;
		}
	}
	return findSlotOrderNumber;
}

// 그림자 전장에서 적군인가(나 iff, 타겟 npc iff)
bool FGsInteractionManager::IsEnemyIffShadowBattleField(IffFactionType In_localIff, IffFactionType In_npcIff)
{
	// 내가 블루면
	if (In_localIff == IffFactionType::PLAYER_ARENA_TEAM_BLUE)
	{
		// 레드가 적
		return (In_npcIff == IffFactionType::NPC_ARENA_TEAM_RED)? true: false;
	}
	// 내가 레드면
	else if (In_localIff == IffFactionType::PLAYER_ARENA_TEAM_RED)
	{
		// 블루가 적
		return (In_npcIff == IffFactionType::NPC_ARENA_TEAM_BLUE)? true: false;
	}
	// 나머진 적이 아니다
	return false;
}