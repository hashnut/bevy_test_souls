// Fill out your copyright notice in the Description page of Project Settings.


#include "GsDungeonManager.h"

#include "GsTableManager.h"
#include "GsUnlockManager.h"
#include "Dungeon/CoopPartyDungeon/GsCoopPartyDungeonGroupInfo.h"
#include "Dungeon/GsDungeonMessageHolder.h"

#include "../Map/GsMapFunc.h"

#include "../Dungeon/GsDungeonHelper.h"
#include "../Dungeon/GsDungeonCommonData.h"
#include "../Dungeon/GsDungeonGroupNormal.h"
#include "../Dungeon/GsDungeonGroupParty.h"
#include "../Dungeon/GsDungeonGroupBattleArena.h"

#include "ContentsEvent/GsContentsEventTeleportCost.h"

#include "../Sequence/GsSequencePlayer.h"

#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedAbnormalityEnum.h"
#include "Net/GsNetSendServiceWorld.h"

#include "UTIL/GsTimeSyncUtil.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Popup/GsUIPopupCommonPay.h"
#include "UI/UIContent/Popup/GsUIPopupCommonItemConsume.h"

#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGame/GsEventProgressManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/GsMessageHolder.h"

#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageContentDungeon.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/MessageParam/GsMessageParam.h"

#include "GsGameObjectManager.h"
#include "../GameObject/Define/GsGameObjectDefine.h"
#include "../GameObject/ObjectClass/GsGameObjectBase.h"

#include "../GameFlow/GsGameFlowGame.h"
#include "../GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "../GameFlow/GameContents/ContentsGame/GsGameStateDungeon.h"
#include "../GameFlow/GameContents/GsContentsMode.h"
#include "../GameFlow/GameContents/ContentsGame/Dungeon/GsDungeonStateHandler.h"
#include "../GameFlow/GameContents/ContentsGame/Hud/BattleArena/GsBattleArenaHelper.h"

#include "Item/GsItemManager.h"
#include "Item/GsSchemaItemCommon.h"
#include "Item/GsItem.h"
#include "Currency/GsCurrencyHelper.h"
#include "Currency/GsCostPackage.h"

#include "Dungeon/GsSchemaDungeonGroupData.h"
#include "Dungeon/GsSchemaDungeonData.h"
#include "Dungeon/GsDungeonGroupInfo.h"

#include "Map/GsSchemaMapDataCost.h"
#include "Map/GsSchemaMapData.h"

#include "EventProgress/GsEventProgressDefine.h"

#include "Currency/GsCostOption.h"

#include "UTIL/GsCronUtil.h"
#include "DataSchema/GsSchemaEnums.h"

#include "../UTIL/GsUIUtil.h"
#include "UTIL/GsTableUtil.h"
#include "UI/UIContent/Tray/GsUITrayTickerSection.h"

#include "DataSchema/GsSchemaResetTimeTicketConfig.h"
#include "DataSchema/Dungeon/GsSchemaDungeonGroupData.h"
#include "DataSchema/Dungeon/GsSchemaCoopPartyDungeonConfig.h"

#include "../GameObject/Data/GsGameObjectData.h"

#include "T1Project.h"
#include "Shared/Shared/SharedInclude/SharedDefine.h"


void FGsDungeonManager::Initialize()
{
	// 2022/10/25 PKT - Message Hold
	FGsDungeonMessageHolder holder(_messageHolder);

	/**
	 * Data Init
	 */
	_configPartyOnly = nullptr;
	_dungeonGroupHolder.Empty();
	_warpHandler.Clear();

	/**
	 * Set Data
	 */
	LoadConfigData();
	BuildData();
	

	for (auto& item : _dungeonGroupHolder)
	{
		if (item.IsValid())
		{
			item.Get()->Initialized();
			item.Get()->_OnInvalidate.BindRaw(this, &FGsDungeonManager::ChangedDataDelegate);
		}
	}

	/**
	 * Main Tab Hidden Data
	 */
	_hiddenMainTabWithUnlockContentsType.Reset();
	// 2023/9/19 PKT - 현재 파티 던전만 있음.
	_hiddenMainTabWithUnlockContentsType.FindOrAdd(DungeonCategory::PARTY) = EGsUnlockContentsType::PARTY_DUNGEON_HIDDEN;
	_hiddenMainTabWithUnlockContentsType.FindOrAdd(DungeonCategory::ARENA) = EGsUnlockContentsType::ARENA_DUNGEON_HIDDEN;

	/**
	 * Event Delegate
	 */
	_gameObjectDelegate = GMessage()->GetGameObject().AddRaw(
		MessageGameObject::LOCAL_SPAWN_ME_COMPLETE, this, &FGsDungeonManager::OnLocalPlayerSpawnComplete
	);

	// 2022/06/10 PKT - 리셋
	_systemDelegate = GMessage()->GetSystemParam().AddRaw(
		MessageSystem::RESET_TIME_CONTENTS, this, &FGsDungeonManager::OnResetTimeContents
	);

	// 2023/7/31 PKT - 시련의 전당 언락 컨텐츠
	_unlockDelegate = GMessage()->GetUnlock().AddRaw(
		MessageContentUnlock::INVALIDATE_ALL_UNLOCK, this, &FGsDungeonManager::OnContentsUnlock
	);
}


void FGsDungeonManager::Finalize()
{
	// 2022/10/25 PKT - Message Hold
	FGsDungeonMessageHolder holder(_messageHolder);

	GMessage()->GetGameObject().Remove(_gameObjectDelegate);
	GMessage()->GetSystemParam().Remove(_systemDelegate);
	GMessage()->GetUnlock().Remove(_unlockDelegate);

	// 2023/9/19 PKT - Main Tab Hidden Data
	_hiddenMainTabWithUnlockContentsType.Reset();

	// 2022/10/26 PKT - Warp Handler
	_warpHandler.Clear();

	for (auto& item : _dungeonGroupHolder)
	{
		if (item.IsValid())
		{
			item.Get()->Finalized();
		}
	}	
	_dungeonGroupHolder.Empty();	
	_configPartyOnly = nullptr;
}

void FGsDungeonManager::SyncDungeonGroupTime(DungeonGroupId InDungeonGroupId, int32 InNormalTime, int32 InChargeTime)
{
	FGsDungeonGroupBase* findData = FindGroupByGroupId(InDungeonGroupId);
	if (nullptr == findData || DungeonCategory::PARTY == findData->GetCategory())
	{
		GSLOG(Error, TEXT("nullptr == findData || DungeonCategory::PARTY == findData->GetCategory() : [%d]"), InDungeonGroupId);
		return;
	}

	FGsDungeonGroupNormal* dungeonGroupData = StaticCast<FGsDungeonGroupNormal*>(findData);
	dungeonGroupData->SetTime(InNormalTime, InChargeTime);
}

FGsDungeonGroupBase* FGsDungeonManager::FindGroupByGroupId(DungeonGroupId InDungeonGroupId) const
{
	for (auto& item : _dungeonGroupHolder)
	{
		if (item.IsValid() && InDungeonGroupId == item.Get()->GetGroupId())
		{
			return item.Get();
		}
	}
	return nullptr;
}

const FGsDungeonGroupBase* FGsDungeonManager::FindGroupByMapId(MapId InMapId) const
{
	for (auto& item : _dungeonGroupHolder)
	{
		if (item.IsValid())
		{
			for (int32 idx = 0; idx < item.Get()->DungeonCountAll(); ++idx)
			{
				const auto& data = item.Get()->FindDungeonByIndex(idx);
				if (data && data->GetSchema()->mapId.GetRow() && data->GetSchema()->mapId.GetRow()->id == InMapId)
				{
					return item.Get();
				}
			}
		}
	}

	return nullptr;
}

const FGsDungeonData* FGsDungeonManager::FindDungeonByMapId(MapId InMapId, const FGsDungeonGroupBase* InGroupData /* = nullptr */) const
{
	const FGsDungeonGroupBase* groupData = InGroupData;
	if (nullptr == groupData)
	{
		groupData = FindGroupByMapId(InMapId);
	}

	if (nullptr == groupData)
	{
		GSLOG(Error, TEXT("nullptr == groupData : [%d]"), InMapId);
		return nullptr;
	}

	for (int32 idx = 0; idx < groupData->DungeonCountAll(); ++idx)
	{
		const auto& data = groupData->FindDungeonByIndex(idx);
		if (data && data->GetSchema()->mapId.GetRow() && data->GetSchema()->mapId.GetRow()->id == InMapId)
		{
			return data;
		}
	}

	GSLOG(Error, TEXT("not find dungeon data"), InMapId);
	return nullptr;
}


void FGsDungeonManager::SendWindowMessage(MessageContentDungeon InType /* = MessageContentDungeon::INVALIDATE_DATA */, struct IGsMessageParam* InMessageParm /* = nullptr */)
{
	// 2022/05/27 PKT - RedDot Refresh
	CheckRedDot();

	const static int32 IS_SEND_MESSAGE_UNLOCK = 0;
	if (IS_SEND_MESSAGE_UNLOCK < _messageHolder)
	{	// 2022/10/11 PKT - Message Hold 상태
		return;
	}

	GMessage()->GetDungeon().SendMessage(InType, InMessageParm);
}


bool FGsDungeonManager::CheckRedDot(bool bInUpdate /*= true*/)
{
	if (bInUpdate)
	{
		bool bIsRedDot = false;
		do
		{
			int32 maxCount = StaticCast<int32>(DungeonCategory::MAX);
			for (int32 idx = 0; idx < maxCount; ++idx)
			{
				if (CheckRedDot(StaticCast<DungeonCategory>(idx)))
				{
					bIsRedDot = true;
					break;
				}
			}

		} while (0);

		bool isPrevRedDot = IsRedDot();

		SetRedDot(bIsRedDot);

		if (IsRedDot() != isPrevRedDot)
		{	// 2021/08/13 PKT - RedDot의 변경을 HUD에 알린다.
			GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::DUNGEON, false);
		}
	}

	return IsRedDot();
}

bool FGsDungeonManager::CheckRedDot(DungeonCategory InCategoryType) const
{
	// 2023/5/25 PKT - https://jira.com2us.com/jira/browse/CHR-20522
	//					이와 같은 사유로 던전 내 레드닷 없음.
	
	//if (DungeonCategory::PARTY != InCategoryType)
	//{	// 2023/2/21 PKT - 파티 던전 외 레드닷 조건 없음.
	//	return false;
	//}

	//for (const auto& item : _dungeonGroupHolder)
	//{
	//	if (DungeonCategory::PARTY == InCategoryType)
	//	{
	//		if (item.IsValid() && item.Get()->GetCategory() == InCategoryType)
	//		{
	//			const FGsDungeonGroupParty* data = StaticCast<const FGsDungeonGroupParty*>(item.Get());
	//			if (data->IsUnLockContents() && data->GetDefaultHaveRewardCount())
	//			{
	//				return true;
	//			}
	//		}
	//	}		
	//}

	return false;
}

void FGsDungeonManager::OnLocalPlayerSpawnComplete(const IGsMessageParam*)
{
	// 2022/10/25 PKT - Message Hold
	FGsDungeonMessageHolder holder(_messageHolder);

	// 2022/10/26 PKT - 서버에 정보를 요청한다.(RedDot 갱신을 위해.. )
	FGsNetSendServiceWorld::SendReqDungeonGroupList();
}

void FGsDungeonManager::OnResetTimeContents(const IGsMessageParam* InParam)
{
	const FGsPrimitiveInt32* Param = InParam->Cast<const FGsPrimitiveInt32>();
	if (nullptr == Param)
	{
		GSLOG(Error, TEXT("nullptr == Param"));
		return;
	}

	const ResetTimeTicket CurrentResetTimeTicket = StaticCast<ResetTimeTicket>(Param->_data);

	if (ResetTimeTicket::RTT_DUNGEON == CurrentResetTimeTicket)
	{
		// 2023/2/21 PKT - Window가 활성화 되어 있다면.. 서버에게 시간값을 갱신해 달라는 패킷을 보내야 한다.
		SendWindowMessage(MessageContentDungeon::RESET_TIME);
	}
	else if (ResetTimeTicket::RTT_PARTY_DUNGEON_RESET == CurrentResetTimeTicket)
	{	// 2022/10/25 PKT - 파티 던전은 시간이 초기화 되는 것이 아니라 기본 입장시 얻을 수 있는 보상 카운트가 초기화 된다.
		for (auto& item : _dungeonGroupHolder)
		{
			if (item.IsValid() && DungeonCategory::PARTY == item.Get()->GetCategory())
			{
				FGsDungeonGroupParty* partyDungeonGroup = StaticCast<FGsDungeonGroupParty*>(item.Get());
				partyDungeonGroup->SetHaveWeekRewardCount(_configPartyOnly->weeklyRewardCount);
			}
		}		
		// 2023/2/21 PKT - 갱신
		SendWindowMessage();
	}
}

void FGsDungeonManager::ChangedDataDelegate(DungeonGroupId InDungeonGroupId /*= INVALID_DUNGEON_GROUP_ID*/)
{
	// 2023/2/21 PKT - 던전/그룹 Data 내 변화가 생겼다.
	FGsPrimitiveInt32 message(InDungeonGroupId);
	SendWindowMessage(MessageContentDungeon::INVALIDATE_DATA, &message);
}

void FGsDungeonManager::RecvDungeonGroupList(PD::SC::PKT_SC_ACK_DUNGEON_GROUP_LIST_READ& Packet)
{
	{
		// 2022/10/11 PKT - 소멸자 호출을 위해 중괄호로 묶음.
		FGsDungeonMessageHolder holder(_messageHolder);

		// 2023/2/21 PKT - 일단 전체 비활성화. 서버로 부터 받지 않은 Data는 비활성화 처리 (UI X, 정보 노출 X)
		for (auto& item : _dungeonGroupHolder)
		{
			if (item.IsValid())
			{
				item.Get()->SetVisible(false);
			}
		}

		/**
		 * Build Normal / Special Dungeon Data
		 */
		for (PD::SC::PKT_SC_ACK_DUNGEON_GROUP_LIST_READ::DungeonGroupListIterator iter = Packet.GetFirstDungeonGroupListIterator();
			iter != Packet.GetLastDungeonGroupListIterator(); ++iter)
		{
			FGsDungeonGroupBase* dungeonGroup = FindGroupByGroupId(iter->DungeonGroupId());
			if (nullptr == dungeonGroup)
			{
				continue;
			}

			if (DungeonCategory::ARENA == dungeonGroup->GetSchemaData()->dungeonCategory)
			{
				FGsDungeonGroupBattleArena* dungeonGroupBattleArena = StaticCast<FGsDungeonGroupBattleArena*>(dungeonGroup);
				if (dungeonGroupBattleArena)
				{
					dungeonGroupBattleArena->SetVisible(true);
				}
			}
			else
			{
				FGsDungeonGroupNormal* dungeonGroupNormal = StaticCast<FGsDungeonGroupNormal*>(dungeonGroup);
				if (dungeonGroupNormal)
				{
					dungeonGroupNormal->SetTime(iter->MyBaseTime(), iter->MyAddTime());
					dungeonGroupNormal->SetVisible(true);
				}
			}
		}

		/**
		 * Build Party Dungeon Data
		 */
		using PartyDungeonGroupIter = PD::SC::PKT_SC_ACK_DUNGEON_GROUP_LIST_READ::PartyDungeonGroupListIterator;
		for (PartyDungeonGroupIter iter = Packet.GetFirstPartyDungeonGroupListIterator(); iter != Packet.GetLastPartyDungeonGroupListIterator(); ++iter)
		{
			FGsDungeonGroupBase* dungeonGroup = FindGroupByGroupId(iter->DungeonGroupId());
			if (nullptr == dungeonGroup)
			{
				continue;
			}

			FGsDungeonGroupParty* dungeonGroupParty = StaticCast<FGsDungeonGroupParty*>(dungeonGroup);
			if (dungeonGroupParty)
			{
				dungeonGroupParty->SetClearDifficulty(iter->LastClearLevel());
				// 2023/2/27 PKT - 'Packet'에 주의. 파티 던전 보상 카운트는 파티 던전 공통임.
				dungeonGroupParty->SetHaveWeekRewardCount(Packet.MyBaseRewardCount());
				dungeonGroupParty->SetVisible(true);
			}
		}
	}
	
	// 2022/10/11 PKT - Window 갱신 메세지
	SendWindowMessage();
}

void FGsDungeonManager::RecvPartyEntryList(PD::GC::PKT_GC_ACK_COOP_PARTY_LIST_READ& Packet) 
{
	TArray<FGsPartyDungeonPartyStatusList> partyStatusList;

	using PartyDataIterator = PD::GC::PKT_GC_ACK_COOP_PARTY_LIST_READ::CoopPartyListIterator;
	for (PartyDataIterator iter = Packet.GetFirstCoopPartyListIterator(); iter != Packet.GetLastCoopPartyListIterator(); ++iter)
	{
		const CoopPartyInfo& partyData = iter->CoopPartyInfo();
		const FString leaderName = iter->LeaderName();

		FGsPartyDungeonPartyStatusList data;
		data._partyId = partyData.mPartyId;
		data._dungeonGroupId = partyData.mDungeonGroupId;
		data._difficulty = partyData.mDungeonDifficulty;
		data._status = partyData.mPartyDungeonStepStatus;
		data._canForcedEnter = partyData.mCanForcedEnter;

		data._leaderName = leaderName;
		data._leaderLevel = partyData.mLeaderLevel;
		data._leaderWeaponType = partyData.mLeaderWeaponType;

		for (int32 idx = 0; idx < MAX_PARTY_MEMBER; ++idx)
		{	// 2023/4/26 PKT - Slot Index가 0보다 작은건 정보가 없다는 이야기.
			if (0 > partyData.mMemberInfoList[idx].mSlotIndex)
			{
				continue;
			}
			data._memberList.Emplace(TPair<CreatureWeaponType, Level>(partyData.mMemberInfoList[idx].mWeaponType, partyData.mMemberInfoList[idx].mLevel));
		}

		partyStatusList.Emplace(data);
	}

	// 2022/10/11 PKT - 파티 리스트 중 파티 난이도 순 정렬
	partyStatusList.Sort([](const FGsPartyDungeonPartyStatusList& lhr, const FGsPartyDungeonPartyStatusList& rhr)
		{
			return lhr._difficulty < rhr._difficulty;
		}
	);

	// 2022/10/11 PKT - Data 그대로 Window에 보낸다.
	FGsUIMsgParamPartyDungeonMemberList message(&partyStatusList);
	GMessage()->GetDungeon().SendMessage(MessageContentDungeon::PARTY_DUNGEON_PARTYLIST, &message);
}

void FGsDungeonManager::RecvPartyEnterdRewardCount(PD::SC::PKT_SC_ACK_PARTY_DUNGEON_INFO_READ& Packet) 
{
	// 2023/2/21 PKT - 파티 던전 입장시 보상 얻을 수 있는 아이템 카운트는 모든 파티 던전이 공통으로 공유 함. 매니저에서 관리 할까 하다 내부로 옮김.
	{
		// 2022/10/11 PKT - Message Hold
		FGsDungeonMessageHolder holder(_messageHolder);

		for (auto& item : _dungeonGroupHolder)
		{
			if (item.IsValid() && DungeonCategory::PARTY == item.Get()->GetCategory())
			{
				StaticCast<FGsDungeonGroupParty*>(item.Get())->SetHaveWeekRewardCount(Packet.FreeKeyCount());
			}
		}
	}	
	// 2022/10/11 PKT - Send Window Message
	SendWindowMessage();
}

void FGsDungeonManager::RecvChargeTime(PD::SC::PKT_SC_DUNGEON_GROUP_UPDATE_READ& Packet) 
{
	// 2023/2/21 PKT - 던전 시간 충전.
	const FGsDungeonGroupBase* findData = FindGroupByGroupId(Packet.DungeonGroupId());
	if (nullptr == findData)
	{
		GSLOG(Error, TEXT("nullptr == findData : [%d]"), Packet.DungeonGroupId());
		return;
	}

	SyncDungeonGroupTime(Packet.DungeonGroupId(), Packet.MyBaseTime(), Packet.MyAddTime());

	// 2022/10/11 PKT - 던전 시간이 충전 되었음을 티커로 알림.
	const FGsSchemaDungeonGroupData* schemaData = findData->GetSchemaData();

	FText text;
	FGsDungeonHelper::FindText(TEXT("Notice_Use_Item_DungeonTimeCharge"), text);	
	FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, FText::Format(text, schemaData->dungeonGroupName));
}

bool FGsDungeonManager::IsPossibleWarpByMapId(MapId InMapId, bool InOnMessage /* = true */)
{
	const FGsDungeonGroupBase* data = FindGroupByMapId(InMapId);
	if (nullptr == data || true == data->IsDisable())
	{
		return false;
	}

	if (DungeonCategory::PARTY == data->GetCategory())
	{	// 2022/10/26 PKT - 파티 던전은 임의로 워프 불가.
		return false;
	}

	if (false == data->IsUnLockContents(InOnMessage))
	{
		return false;
	}

	if (UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		if (local->IsCrowdControlLock(CreatureActionType::CREATURE_ACTION_TYPE_WARP))
		{
			if (InOnMessage)
			{
				FText findText;
				FGsDungeonHelper::FindText(TEXT("WarpFailedAbnormality"), findText);
				FGsUIHelper::TrayMessageTicker(findText);
			}			
			return false;
		}
	}

	if (DungeonCategory::NORMAL == data->GetCategory() || DungeonCategory::SPECIAL == data->GetCategory())
	{
		const FGsDungeonGroupNormal* normalData = StaticCast<const FGsDungeonGroupNormal*>(data);
		bool isTimeLimit = normalData->IsTimeLimit();
		bool isOverTime = normalData->IsTimeOver();
		if (isTimeLimit && isOverTime)
		{
			if (InOnMessage)
			{
				// 2022/10/26 PKT - 제한 시간이 초과 되었음.( 입장 불가 )
				FText findText;
				FText::FindText(TEXT("UICommonText"), TEXT("DungeonTimeZeroWarning"), findText);
				FGsUIHelper::TrayMessageTicker(findText);
			}		
			return false;
		}
	}

	FText errorMessage;
	if (false == data->IsValidEnterDateTime(errorMessage))
	{	// 2022/10/26 PKT - 입장 가능 시간이 아님
		if (InOnMessage && false == errorMessage.IsEmpty())
		{
			FGsUIHelper::PopupOK(errorMessage);
		}
		return false;
	}

	if (true == UGsSequencePlayer::IsSequencePlaying())
	{	// 2023/07/07 PKT - 미란차석님 요청사항 https://jira.com2us.com/jira/browse/C2URWQ-5782
		return false;
	}
	
	return true;
}

void FGsDungeonManager::TryToEnterDungeon(MapId InMapId)
{
	const FGsDungeonGroupBase* findGroup = FindGroupByMapId(InMapId);	

	if (nullptr == findGroup)
	{
		GSLOG(Error, TEXT("nullptr == findGroup : [%d]"), InMapId);
		return;
	}

	if (false == IsPossibleWarpByMapId(InMapId))
	{	// 2022/10/26 PKT - 워프 및 입장 불가
		return;
	}

	// 업노말리티에서 막으면
	if (FGsMapFunc::IsLocalAbnormalityType(AbnormalityEffectType::CC_CANNOT_WARP) == true)
	{
		// 던전으로 이동 못하는 곳이면 설명 팝업 출력
		FText textPersonalEventWarpWarning;
		FText::FindText(TEXT("UICommonText"), TEXT("PersonalEventWarpWarning"), textPersonalEventWarpWarning);
		FGsUIHelper::PopupOK(textPersonalEventWarpWarning);
	}
	else
	{
		const FGsDungeonData* data = FindDungeonByMapId(InMapId, findGroup);
		const FGsSchemaMapData* mapData = data->GetSchema()->mapId.GetRow();

		if (false == data->IsUnLockState())
		{	// 2023/12/18 PKT - https://jira.com2us.com/jira/browse/C2URWQ-7722
			return;
		}

		// 2023/11/24 PKT - BattleArena 분기 처리
		if (DungeonCategory::ARENA == findGroup->GetCategory())
		{
			MapId mapId = mapData->id;

			FText dungeonName;
			FGsDungeonHelper::DungeonNameByMapData(findGroup->GetSchemaData(), data->GetSchema(), dungeonName);

			// 2022/07/28 PKT - {0}\n입장 하시겠습니까?
			FText textFormat;
			FGsDungeonHelper::FindText(TEXT("Popup_Common_Enter"), textFormat);

			FText textDest;
			FGsBattleArenaHelper::FindText(TEXT("Popup_GuildArena_Entrance_Notice"), textDest);

			_warpHandler.ReserveWarp(mapId, FText::Format(textFormat, dungeonName), textDest, findGroup);
		}
		else
		{
			SpotId spotId = data->GetSchema()->spotId;
			MapId mapId = mapData->id;
			CurrencyType currencyType = data->GetEnterdCurrencyData().Get<0>();
			currencyType = CurrencyType::NONE == currencyType ? CurrencyType::GOLD : currencyType;

			int32 costCurrencyAmount = data->GetEnterdCurrencyData().Get<1>();

			FText dungeonName;
			FGsDungeonHelper::DungeonNameByMapData(findGroup->GetSchemaData(), data->GetSchema(), dungeonName);

			// 2022/07/28 PKT - {0}\n입장 하시겠습니까?
			FText textFormat;
			FGsDungeonHelper::FindText(TEXT("Popup_Common_Enter"), textFormat);

			// 2022/10/26 PKT - Reserve Warp
			_warpHandler.ReserveWarp(mapId, spotId, currencyType, costCurrencyAmount, FText::Format(textFormat, dungeonName), findGroup);
		}
	}
}

int32 FGsDungeonManager::BuildViewGroup(DungeonCategory InType) const
{
	_viewGroupSet.Empty();

	for (const auto& item : _dungeonGroupHolder)
	{
		if (item.IsValid() 
			&& item.Get()->GetCategory() == InType 
			&& false == item.Get()->IsDisable() 
			&& false == item.Get()->IsUIHidden())
		{
			_viewGroupSet.Emplace(item.Get());
		}
	}

	return _viewGroupSet.Num();
}

const TArray<const FGsDungeonGroupBase*> FGsDungeonManager::ViewGroupSet() const
{
	return _viewGroupSet;
}

bool FGsDungeonManager::IsHiddenWithMainTab(DungeonCategory InCategory, bool InShowTicker /*= false*/) const
{
	if (nullptr == GSUnlock())
	{
		return false;
	}

	EGsUnlockContentsType type = EGsUnlockContentsType::NONE;

	if (_hiddenMainTabWithUnlockContentsType.Contains(InCategory))
	{
		if (DungeonCategory::PARTY == InCategory)
		{
			type = EGsUnlockContentsType::PARTY_DUNGEON_HIDDEN;
		}
		else if (DungeonCategory::ARENA == InCategory)
		{
			type = EGsUnlockContentsType::ARENA_DUNGEON_HIDDEN;
		}		
	}

	// 2023/9/19 PKT - Lock 상태일때 Hidden 처리 해야 하므로, false 검사를 한다.
	return (false == GSUnlock()->IsContentsUnlock(type, InShowTicker));
}

const FGsDungeonGroupBase* FGsDungeonManager::FindGroupDataById(DungeonGroupId InDungeonGroupId) const
{
	const FGsDungeonGroupBase* data = FindGroupByGroupId(InDungeonGroupId);
	return (data && false == data->IsDisable()) ? data : nullptr;
}

const FGsDungeonGroupBase* FGsDungeonManager::FindGroupDataByMapId(MapId InMapId) const
{
	const FGsDungeonGroupBase* data = FindGroupByMapId(InMapId);
	return (data && false == data->IsDisable()) ? data : nullptr;
}

const FGsDungeonData* FGsDungeonManager::FindDungeonDataByMapId(MapId InMapId) const
{
	const FGsDungeonGroupBase* data = FindGroupByMapId(InMapId);
	return (data && false == data->IsDisable()) ? data->FindDungeonByMapId(InMapId) : nullptr;
}

const FGsSchemaCoopPartyDungeonConfig* FGsDungeonManager::GetPartyDungeonConfig() const
{
	return _configPartyOnly;
}

void FGsDungeonManager::LoadConfigData()
{
	const UGsTable* table = FGsSchemaCoopPartyDungeonConfig::GetStaticTable();
	if (nullptr == table)
	{
		GSLOG(Error, TEXT("nullptr == table"));
		return;
	}

	const TMap<FName, uint8*>* OutRowMap;
	if (false == table->GetRowMap(OutRowMap))
	{
		GSLOG(Error, TEXT("false == table->GetRowMap(OutRowMap)"));
		return;
	}

	// 2022/06/09 PKT - Config Data는 한개만 존재해야 한다.
	const static int32 CONFIG_DATA_ONLY_ONE = 1;
	if (CONFIG_DATA_ONLY_ONE != OutRowMap->Num())
	{
		GSLOG(Warning, TEXT("CONFIG_DATA_ONLY_ONE != OutRowMap->Num()"));
	}
	
	/**
	 * 2022/10/11 PKT - 파티 던전 설정 파일
	 */
	for (const auto& pair : (*OutRowMap))
	{	// 2022/06/09 PKT - 하나만 적용 시키고 빠져 나간다.
		_configPartyOnly = reinterpret_cast<const FGsSchemaCoopPartyDungeonConfig*>(pair.Value);
		break;
	}
}

void FGsDungeonManager::BuildData()
{
	FGsDungeonMessageHolder holder(_messageHolder);

	const UGsTableDungeonGroupData* groupTable = Cast<UGsTableDungeonGroupData>(FGsSchemaDungeonGroupData::GetStaticTable());
	if (nullptr == groupTable)
	{
		GSLOG(Error, TEXT("nullptr == groupTable"));
		return;
	}

	const TMap<FName, uint8*>* RowMap = nullptr;
	if (false == groupTable->GetRowMap(RowMap))
	{
		GSLOG(Error, TEXT("false == groupTable->GetRowMap(RowMap)"));
		return;
	}

	_dungeonGroupHolder.Empty();

	for (auto& RowKvIt : *RowMap)
	{
		const FGsSchemaDungeonGroupData* schemaGroupData = reinterpret_cast<const FGsSchemaDungeonGroupData*>(RowKvIt.Value);
		FGsDungeonGroupBase* dungeonGroup = FGsDungeonHelper::Factory(schemaGroupData->id);
		if (nullptr == dungeonGroup)
		{
			GSLOG(Error, TEXT("nullptr == dungeonGroup : [%d]"), schemaGroupData->id);
			continue;
		}

		_dungeonGroupHolder.Emplace(MakeShareable(dungeonGroup));
	}
}

void FGsDungeonManager::OnContentsUnlock(const IGsMessageParam* InParam)
{
	SendWindowMessage();

	if (false == GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::HALL_OF_TRIALS_CANNOT_ENTER))
	{
		// 2023/7/31 PKT - 팝업도 막아야 됨
		_warpHandler.ReserveCancel();
	}
}