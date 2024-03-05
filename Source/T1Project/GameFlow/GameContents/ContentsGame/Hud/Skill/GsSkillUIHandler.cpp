// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSkillUIHandler.h"
#include "Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "Net/GsNetSendServiceWorld.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsCoolTimeManager.h"

#include "Message/MessageParam/GsGameObjectMessageParam.h"
#include "Message/MessageParam/GsCoolTimeMessageParam.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/MessageParam/GsItemMessageParam.h"
#include "Message/MessageParam/GsStageMessageParam.h"
#include "Message/MessageParam/GsUIMessageParam.h"

#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/Data/GsGameObjectDataLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Target/GsTargetHandlerLocalPlayer.h"
#include "GameObject/Fence/GsFenceHandler.h"
#include "GameObject/Stat/GsStatInfo.h"
#include "GameObject/Fence/GsTownChaosFence.h"
#include "GameObject/Target/AutoTarget/GsTargetRule.h"
#include "GameObject/Skill/GsSkillHandlerLocalPlayer.h"
#include "GameObject/Skill/Checker/GsSkillCheckAbnormality.h"
#include "GameObject/Skill/Checker/GsSkillCheckCoolTime.h"
#include "GameObject/Skill/Checker/GsSkillCheckRequireCost.h"
#include "GameObject/Skill/Checker/GsSkillCheckStageMode.h"

#include "DataSchema/Skill/Level/GsSchemaSkillCostConsumeInfo.h"
#include "DataSchema/Skill/Level/GsSchemaSkillStatInfo.h"
#include "DataSchema/User/GsSchemaUserSkillSet.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "DataSchema/Map/GsSchemaMapBalance.h"

#include "Skill/GsSkill.h"
#include "Skill/GsSkillSlotData.h"
#include "Skill/GsSkillHelper.h"

#include "Item/GsItemManager.h"
#include "Item/GsItem.h"
#include "Item/GsItemConsumable.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

#include "UTIL/GsTableUtil.h"

#include "T1Project.h"
#include "GameFlow/Stage/StageGame/GsStageManagerGame.h"
#include "GameFlow/GsGameFlowGame.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"

bool FGsSkillUIHandler::isSkillSlotSet = false;

//----------------------------------------------------------------------------------------------------------------------
bool FGsSkillUIHandler::FGsAutoDelayData::Update(float InDeltaTime)
{
	_remainSec -= InDeltaTime;
	if (0 >= _remainSec)
	{
		_remainSec = 0.f;

		if (const FGsSkill* skill = GSkill()->FindSkill(_skillId, CreatureWeaponType::MAX, true))
		{
			// 다를 때만 보냄
			if (skill->IsAuto() != _bUseAuto)
			{
				SkillId rootSkillId = FGsSkillHelper::GetUpgradeRootSkillId(skill);
				if (INVALID_SKILL_ID == rootSkillId)
				{
					rootSkillId = skill->GetSkillId();
				}

				// 자동 사용 요청
				FGsNetSendServiceWorld::SendRequestSkillSlotAuto(
					skill->GetSkillRequireWeaponType(), rootSkillId, skill->GetSkillId(), _bUseAuto);
			}
		}

		return true;
	}

	return false;
}

//----------------------------------------------------------------------------------------------------------------------
FGsSkillUIHandler::~FGsSkillUIHandler()
{
	// 등록된 스킬 체커 제거
	_skillChecker.RemoveAll();
	_skillChecker.Finalize();

	RemoveMessages();
}

EGsMinimapAreaType FGsSkillUIHandler::GetSaftyZoneType()
{
	if (UGsGameObjectBase* player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		UGsGameObjectLocalPlayer* localPlayer = Cast<UGsGameObjectLocalPlayer>(player);
		if (localPlayer)
		{
			// 안전지대인지 체크
			if (FGsFenceHandler* fenceHandler = localPlayer->GetFenceHandler())
			{
				FGsFence* fence = nullptr;
				if (fenceHandler->TryGetPlayerTopPriorityServerFence(fence))
				{
					switch (fence->GetTerritoryType())
					{					
					case TerritoryType::ARENA_READY:
					{
						if (GGameData()->IsInvadeWorld() || GGameData()->GetInvasionDefenseState())
						{
							return EGsMinimapAreaType::CHAOS;
						}
						else
						{
							return EGsMinimapAreaType::SAFE;
						}
					}
						break;
					case TerritoryType::TOWN:
					case TerritoryType::NON_PK:
					case TerritoryType::ARRIVAL:
					case TerritoryType::SAFE_ZONE:
					{
						return EGsMinimapAreaType::SAFE;		
					}						
						break;
					case TerritoryType::ARENA:
					{
						return EGsMinimapAreaType::PVP;
					}
						break;
					case TerritoryType::TOWN_CHAOS:
					{
						if (FGsTownChaosFence* townChaosFence = static_cast<FGsTownChaosFence*>(fence))
						{
							if (townChaosFence->IsChaos())
							{
								return EGsMinimapAreaType::CHAOS;
							}
							else
							{
								return EGsMinimapAreaType::SAFE;
							}
						}
					}
						break;					
					}					
				}				
			}
		}
	}

	const FGsSchemaMapData* mapData = GLevel()->GetCurrentMapInfo();
	if (nullptr == mapData)
	{
		return EGsMinimapAreaType::SAFE;
	}

	const FGsSchemaMapBalance* mapBalanceData = mapData->balanceId.GetRow();
	if (nullptr == mapBalanceData)
	{
		return EGsMinimapAreaType::SAFE;
	}

	if (false == mapBalanceData->EnablePK)
	{
		return EGsMinimapAreaType::SAFE;
	}

	return EGsMinimapAreaType::PVP;	
}

bool FGsSkillUIHandler::IsSaftyZone()
{
	if (UGsGameObjectBase* player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		UGsGameObjectLocalPlayer* localPlayer = Cast<UGsGameObjectLocalPlayer>(player);
		return IsSaftyZone(localPlayer);
	}

	return false;
}

bool FGsSkillUIHandler::IsSaftyZone(UGsGameObjectLocalPlayer* InLocalPlayer)
{
	if (InLocalPlayer)
	{
		// 안전지대인지 체크
		if (FGsFenceHandler* fenceHandler = InLocalPlayer->GetFenceHandler())
		{
			return fenceHandler->IsPlayerInNonPVPFence();
		}
	}

	return false;
}

void FGsSkillUIHandler::Init()
{
	InitMessages();

	CheckSkillBook(true);

	// 스킬 슬롯 딤드 클릭시 스킬 에러메시지 출력용 체크 등록
	_skillChecker.Add(new FGsSkillCheckAbnormality());
	_skillChecker.Add(new FGsSkillCheckCoolTime());
	_skillChecker.Add(new FGsSkillCheckRequireCost());
	_skillChecker.Add(new FGsSkillCheckStageMode());
	_skillChecker.Initialize();
}

void FGsSkillUIHandler::Close()
{
	_reqAutoMap.Empty();
}

void FGsSkillUIHandler::OnReconnection()
{
	_reqAutoMap.Empty();
}

void FGsSkillUIHandler::Update(float InDeltaTime)
{
	if (0 < _reqAutoMap.Num())
	{
		TArray<SkillId> removeList;

		for (TPair<SkillId, FGsAutoDelayData>& iter : _reqAutoMap)
		{
			if (iter.Value.Update(InDeltaTime))
			{
				removeList.Emplace(iter.Key);
			}
		}

		for (SkillId& removeId : removeList)
		{
			_reqAutoMap.Remove(removeId);
		}
	}
}

void FGsSkillUIHandler::InitMessages()
{
	FGsMessageHolder* msgHolder = GMessage();

	MGameObject& msgGameObject = msgHolder->GetGameObject();
	_msgGameObjectHandleList.Emplace(msgGameObject.AddRaw(MessageGameObject::LOCAL_SPAWN_ME_COMPLETE,
		this, &FGsSkillUIHandler::OnLocalPlayerSpawnComplete));
	_msgGameObjectHandleList.Emplace(msgGameObject.AddRaw(MessageGameObject::LOCAL_REVIVE,
		this, &FGsSkillUIHandler::OnLocalPlayerSpawnComplete));

	MUI& msgUI = msgHolder->GetUI();
	_msgUIHandleList.Emplace(msgUI.AddRaw(MessageUI::REQUEST_SKILL_AUTO,
		this, &FGsSkillUIHandler::OnReqSkillAuto));
	_msgUIHandleList.Emplace(msgUI.AddRaw(MessageUI::DIMMED_SKILLBUTTON_ONCLICK,
		this, &FGsSkillUIHandler::OnClickDimmedSkillButton));	
	_msgUIHandleList.Emplace(msgUI.AddRaw(MessageUI::INVALIDATE_SLOT_AQUIRE_ITEM,
		this, &FGsSkillUIHandler::OnInvalidateAquireItem));

	MReserveSkillAction& msgReserveSkill = msgHolder->GetSkillRserveActiveState();
	_msgReserveSkillHandleList.Emplace(msgReserveSkill.AddRaw(MessageContentHud::SKILL_ACTIVE,
		this, &FGsSkillUIHandler::OnActiveSkill));	
	
	MItem& msgItem = msgHolder->GetItem();
	_msgItemHandleList.Emplace(msgItem.AddRaw(MessageItem::ADDITEMLIST,
		this, &FGsSkillUIHandler::OnAddItemList));
	_msgItemHandleList.Emplace(msgItem.AddRaw(MessageItem::REMOVEITEM,
		this, &FGsSkillUIHandler::OnRemoveItem));
	_msgItemHandleList.Emplace(msgItem.AddRaw(MessageItem::UPDATEITEM,
		this, &FGsSkillUIHandler::OnUpdateItem));

	MCostume& msgCostume = msgHolder->GetCostume();
	_msgCostumeHandleList.Emplace(msgCostume.AddRaw(MessageCostume::UPDATED,
		this, &FGsSkillUIHandler::OnUpdateCostume));
}

void FGsSkillUIHandler::RemoveMessages()
{
	FGsMessageHolder* msgHolder = GMessage();

	MUI& msgUI = msgHolder->GetUI();
	for (MsgUIHandle& msgUIHandle : _msgUIHandleList)
	{
		msgUI.Remove(msgUIHandle);
	}
	_msgUIHandleList.Empty();

	MGameObject& msgGameObect = msgHolder->GetGameObject();
	for (MsgGameObjHandle& msgGameObjectHandle : _msgGameObjectHandleList)
	{
		msgGameObect.Remove(msgGameObjectHandle);
	}
	_msgGameObjectHandleList.Empty();

	MReserveSkillAction& msgReserveSkill = msgHolder->GetSkillRserveActiveState();
	for (MsgHudHandle& msgReserveSkillHandle : _msgReserveSkillHandleList)
	{
		msgReserveSkill.Remove(msgReserveSkillHandle);
	}
	_msgReserveSkillHandleList.Empty();

	MItem& msgItem = msgHolder->GetItem();
	for (MsgItemHandle& msgItemHandle : _msgItemHandleList)
	{
		msgItem.Remove(msgItemHandle);
	}
	_msgItemHandleList.Empty();

	MCostume& msgCostume = msgHolder->GetCostume();
	for (MsgCostumeHandle& handle : _msgCostumeHandleList)
	{
		msgCostume.Remove(handle);
	}
	_msgCostumeHandleList.Empty();
}

void FGsSkillUIHandler::OnLocalPlayerSpawnComplete(const IGsMessageParam*)
{
	_localPlayer = Cast<UGsGameObjectLocalPlayer>(GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer));

	CheckSkillBook(false);
}

void FGsSkillUIHandler::OnLocalRevive(const IGsMessageParam*)
{
	_localPlayer = Cast<UGsGameObjectLocalPlayer>(GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer));
}

// bool FGsSkillUIHandler::IsNotEnoughCostStat(const FGsSkill* InSkill, bool InPlayNotEnoughTickerMsg)
// {
// 	if (nullptr == InSkill)
// 	{
// 		return false;
// 	}
// 
// 	if (false == _localPlayer.IsValid())
// 	{
// 		return false;
// 	}
//  
//  	// 데이터에 설정되어있는 stat 설정량 체크
//  	// 하나라도 false 가 나오면 별도 ticker 출력이 필요한지???
//  	const FGsSchemaSkillCostConsumeInfo* consumeInfoData = InSkill->GetCurrLevelCostConsumeInfoData(InSkill->GetTotalLevel());
//  	if (nullptr == consumeInfoData)
//  	{
//  		return false;
//  	}
//  
//  	TArray<FGsSchemaSkillCostItemInfo> costItemList = consumeInfoData->requireItemList;
//  	for (uint8 i = 0; i < costItemList.Num(); ++i)
//  	{
//  		int32 itemID = costItemList[i].itemId;
//  		ItemAmount amount = costItemList[i].itemAmount;
//  		if (false == GItem()->IsEnoughItemAmountByTID(itemID, amount))
//  		{
//  			if (const FGsSchemaItemCommon* errItem = FGsItem::GetTableDataByID(itemID))
//  			{
//  				FText textFormat;
//  				if (FText::FindText(TEXT("SkillUIText"), TEXT("Notice_SkillCheckResult_NotEnoughRequireItem"), textFormat))
//  				{
//  					FText findText = FText::Format(textFormat, errItem->name);
//  					FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
//  				}
//  			}
//  			return true;
//  		}
//  	}
//  
//  	TArray<FGsSchemaSkillStatInfo> statInfoList(consumeInfoData->statInfoList);
//  	for (uint8 i = 0; i < statInfoList.Num(); ++i)
//  	{
//  		StatType type = statInfoList[i].type;
//  		int32 value = statInfoList[i].value;
//  
//  		const FGsStatInfo* statInfo = _localPlayer->GetCreatureStatInfoByType(type);
//  		if (nullptr == statInfo)
//  		{
//  #if WITH_EDITOR
//  			GSLOG(Warning, TEXT("[FGsSkillUIHandler::IsEnoughAquireCostStat] - _localPlayer GetStatInfo() is nullptr - StatType : %d"), (uint16)type);
//  #endif
//  			continue;
//  		}
//  
//  		int32 curValue = statInfo->GetStatValue();
//  		if (value > curValue)
//  		{
//  			if (InPlayNotEnoughTickerMsg)
//  			{
//  				FText findText;
//  				switch (statInfo->GetStatType())
//  				{
//  				case StatType::HP:
//  					FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Notice_NotEnoughHP"), findText);
//  					break;
//  				case StatType::MP:
//  					FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Notice_NotEnoughMP"), findText);
//  					break;
//  				}
//  
//  				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
//  			}
//  			return true;
//  		}
//  	}
// 
// 	return false;
// }

void FGsSkillUIHandler::OnActiveSkill(int32 InSlotId)
{
	if (const FGsSkill* slotSkillData = GSkill()->FindSkillSlot(InSlotId))
	{
		// 타겟 선정
		if (FGsTargetHandlerLocalPlayer* targetHandler = _localPlayer->GetCastTarget<FGsTargetHandlerLocalPlayer>())
		{
			FGsTargetRule::EGsTargetRuleType ruleType = FGsTargetRule::EGsTargetRuleType::SemiAutoTarget;
			// 이동키를 누른 상태인지 확인
			if (FGsSkillHandlerLocalPlayer::IsPressType(FGsSkillHandlerLocalPlayer::InputPressType::Joystick))
			{
				ruleType = FGsTargetRule::ManualTarget;
			}

			targetHandler->OnTarget(ruleType, false, FGsTargetHandlerBase::SelectTargetFilter::AutoSkill,
				slotSkillData->GetIffType());

			if (FGsSkillHandlerBase* skillHandler = _localPlayer->GetSkillHandler())
			{
				skillHandler->OnSkill(slotSkillData->GetSkillId());
			}
		}
	}
}

void FGsSkillUIHandler::OnReqSkillAuto(const IGsMessageParam* InParam)
{
	if (const FGsUIMsgParamRequestSkillAuto* param = InParam->Cast<const FGsUIMsgParamRequestSkillAuto>())
	{
		if (FGsAutoDelayData* data = _reqAutoMap.Find(param->_skillId))
		{
			data->_bUseAuto = param->_bUseAuto;
			data->_remainSec = 0.2f;
		}
		else
		{
			FGsAutoDelayData delayData;
			delayData._skillId = param->_skillId;
			delayData._bUseAuto = param->_bUseAuto;
			delayData._remainSec = 0.2f;

			_reqAutoMap.Emplace(param->_skillId, delayData);
		}
	}
}

void FGsSkillUIHandler::OnInvalidateAquireItem(const IGsMessageParam* InParam)
{
	const FGsSkill* slotSkillData = GSkill()->FindSkillSlot(SKILL_SLOT_ID_NORMAL);
	if (nullptr == slotSkillData || nullptr == slotSkillData->_tableData)
		return;

	// 해당 슬롯만 갱신
	FGsGameObjectMessageParamSkillSlot param(slotSkillData->_tableData->requireWeapon, slotSkillData->GetSlotId(), slotSkillData);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_CHANGE_SKILL_SLOT, &param);
}

void FGsSkillUIHandler::OnClickDimmedSkillButton(const IGsMessageParam* InParam)
{
	const FGsUIMsgParamInt* param = InParam->Cast<const FGsUIMsgParamInt>();
	const FGsSkill* slotSkillData = GSkill()->FindSkillSlot(param->_data);
	if (nullptr == slotSkillData)
	{
		return;
	}

	// 각 스킬마다의 소모 스텟이 충분한지 체크
	EGsSkillCheckResultType errType = _skillChecker.ProcessCheck(slotSkillData);
	if (errType != EGsSkillCheckResultType::Pass)
	{
		FText errText = _skillChecker.GetErrMessage();
		if (false == errText.IsEmpty())
		{
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, errText);
		}
	}
	// IsNotEnoughCostStat(slotSkillData, true);
}

void FGsSkillUIHandler::OnAddItemList(FGsItemMessageParamBase& InParam)
{
	FGsAddItemList* param = static_cast<FGsAddItemList*>(&InParam);
	if (nullptr == param)
	{
		return;
	}

	UGsSkillManager* skillMgr = GSkill();
	if (nullptr == skillMgr)
	{
		return;
	}

	FGsSkillUIDataHandler* skillDataHandler = skillMgr->GetSkillUIDataHandler();
	if (nullptr == skillDataHandler)
	{
		return;
	}

	UGsItemManager* itemMgr = GItem();
	if (nullptr == itemMgr)
	{
		return;
	}

	for (int i = 0; i < param->_itemDbidList.Num(); ++i)
	{
		// 스킬 관련 재료 아이템 아니면 검사할 필요없음
		TWeakPtr<FGsItem> itemPtr = itemMgr->FindItem(param->_itemDbidList[i]);
		if (itemPtr.IsValid())
		{
			FGsItem* item = itemPtr.Pin().Get();
			if (nullptr == item)
			{
				return;
			}

			// 스킬 사용 소모품(비전석) 추가 시 스킬 체크 
			if (ItemCategorySub::SPELL_STONE == item->GetSubCategory())
			{
				skillMgr->CheckSkillSlotCost();
			}

			// 레드닷 체크
			bool bPrevRedDot = skillMgr->IsRedDot();

			// 스킬북 캐싱
			if (ItemCategoryMain::SKILLBOOK == item->GetMainCategory())
			{
				TArray<SkillId> skillIdList;
				if (GetSkillIdBySkilllBook(item, skillIdList))
				{
					for (int j = 0; j < skillIdList.Num(); ++j)
					{
						skillDataHandler->AddSkillBook(skillIdList[j], item->GetTID());
					}

					skillMgr->CheckRedDot(true);
				}
			}
			else
			{
				// 재료아이템 아니면 리턴
				if (false == IsSkillIngredientItem(item->GetTableData()))
				{
					return;
				}
			}

			if (IsActiveSkillWindow())
			{
				// 스킬창 상태이면 스킬창 레드닷 업데이트 이벤트를 보냄
				GMessage()->GetUI().SendMessage(MessageUI::SKILL_UPDATE_REDDOT, nullptr);
			}
			else
			{
				// 이전 스킬 레드닷이 켜져있지 않았으면 갱신
				if (false == bPrevRedDot)
				{
					GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::SKILL, true);
				}
			}
		}
	}
}

void FGsSkillUIHandler::OnRemoveItem(FGsItemMessageParamBase& InParam)
{
	FGsRemoveItem* param = static_cast<FGsRemoveItem*>(&InParam);
	if (nullptr == param)
	{
		return;
	}

	UGsSkillManager* skillMgr = GSkill();
	if (nullptr == skillMgr)
	{
		return;
	}

	FGsSkillUIDataHandler* skillDataHandler = skillMgr->GetSkillUIDataHandler();
	if (nullptr == skillDataHandler)
	{
		return;
	}

	const FGsSchemaItemCommon* itemTable = UGsTableUtil::FindRowById<UGsTableItemCommon, FGsSchemaItemCommon>(param->_itemTID);
	if (nullptr == itemTable)
	{
		return;
	}

	// 스킬 사용 소모품(비전석) 제거 시 스킬 체크 
	if (ItemCategorySub::SPELL_STONE == itemTable->categorySub)
	{
		skillMgr->CheckSkillSlotCost();
	}

	bool bPrevRedDot = skillMgr->IsRedDot();
	bool bPrevReinforce = skillMgr->IsReinforce();
	if (ItemCategoryMain::SKILLBOOK == itemTable->categoryMain)
	{
		// 내부에서 레드닷 로직
		skillDataHandler->RemoveSkillBook(param->_itemTID);
		skillMgr->CheckRedDot(true);
	}
	else
	{
		// 재료아이템 아니면 리턴
		if (false == IsSkillIngredientItem(itemTable))
		{
			return;
		}
	}
	
	if (IsActiveSkillWindow())
	{
		// 스킬창 갱신
		GMessage()->GetUI().SendMessage(MessageUI::SKILL_UPDATE_REDDOT, nullptr);
	}
	else
	{
		// 레드닷이 켜져있는 상태면 HUD 갱신
		if (bPrevRedDot || bPrevReinforce)
		{
			GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::SKILL, true);
		}
	}
}

void FGsSkillUIHandler::OnUpdateItem(FGsItemMessageParamBase& InParam)
{
	// Lock 상태, 개수가 바뀌는 등의 상황 체크
	FGsUpdateItem* param = static_cast<FGsUpdateItem*>(&InParam);
	if (nullptr == param)
	{
		return;
	}

	// 스킬 관련 재료 아이템 아니면 검사할 필요없음
	TWeakPtr<FGsItem> item = GItem()->FindItem(param->_itemDBID);
	if (item.IsValid())
	{
		// 스킬 사용 소모품(비전석) 제거 시 스킬 체크 
		if (ItemCategorySub::SPELL_STONE == item.Pin()->GetSubCategory())
		{
			GSkill()->CheckSkillSlotCost();
		}

		if (false == IsSkillIngredientItem(item.Pin().Get()->GetTableData()))
		{
			return;
		}
	}

	// 스킬창 상태일 때는 HUD 업데이트 요청할 필요 없음(스킬 창 닫을 때 요청함)
	if (IsActiveSkillWindow())
	{
		GMessage()->GetUI().SendMessage(MessageUI::SKILL_UPDATE_REDDOT, nullptr);
	}
	else
	{
		GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::SKILL, true);
	}
}

void FGsSkillUIHandler::OnUpdateCostume(const IGsMessageParam* InParam)
{
	if (nullptr == InParam)
	{
		return;
	}

	// 코스튬 스킬 레드닷 체크를 위한 로직
	UGsSkillManager* skillMgr = GSkill();
	if (nullptr == skillMgr)
	{
		return;
	}

	if (skillMgr->IsRedDot())
	{
		return;
	}	

	const FGsUIMsgMsgUpdateList* param = InParam->Cast<const FGsUIMsgMsgUpdateList>();
	if (nullptr == param)
	{
		return;
	}

	FGsSkillUIDataHandler* skillDataHandler = skillMgr->GetSkillUIDataHandler();
	if (nullptr == skillDataHandler)
	{
		return;
	}

	// 코스튬 스킬 획득 체크를 하기 위함이므로, 스킬이 없거나 스킬 배운 상태면 체크할 필요 없음
	bool isNeedCheck = false;
	for (int i = 0; i < param->_dataList.Num(); ++i)
	{
		TWeakPtr<const FGsSkill> skillPtr = skillDataHandler->GetSkillByCostumeId(param->_dataList[i]);
		if (skillPtr.IsValid())
		{
			if (false == skillPtr.Pin()->_isLearn)
			{
				isNeedCheck = true;
				break;
			}
		}
	}

	// 레드닷이 없을 경우에만 확인을 위해 진행함
	if (isNeedCheck)
	{
		GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::SKILL, true);
	}
}

void FGsSkillUIHandler::CheckSkillBook(bool bInForced)
{
	// 스킬북 캐싱하여 Learn 레드닷, 상태 체크
	UGsSkillManager* skillMgr = GSkill();
	if (nullptr == skillMgr)
	{
		return;
	}

	FGsSkillUIDataHandler* skillDataHandler = skillMgr->GetSkillUIDataHandler();
	if (nullptr == skillDataHandler)
	{
		return;
	}

	// 캐싱로직을 자주 타지 않기 위해 체크, forced가 true이면 강제로 다시 캐싱
	if (false == bInForced)
	{
		if (skillDataHandler->GetIsSkillBookCached())
		{
			skillMgr->CheckRedDot(true);
			return;
		}
	}	

	// 스킬북 캐싱
	TArray<TWeakPtr<FGsItem>> skillBookList = GItem()->FindItem(ItemType::CONSUME, ItemCategoryMain::SKILLBOOK);
	for (TWeakPtr<FGsItem> itemPtr : skillBookList)
	{
		if (itemPtr.IsValid())
		{
			FGsItem* item = itemPtr.Pin().Get();

			TArray<SkillId> skillIdList;
			if (GetSkillIdBySkilllBook(item, skillIdList))
			{
				for (int i = 0; i < skillIdList.Num(); ++i)
				{
					skillDataHandler->AddSkillBook(skillIdList[i], item->GetTID());
				}
			}
		}
	}

	// 캐싱로직을 자주 타지 않기 위해 체크
	skillDataHandler->SetIsSkillBookCached(true);

	// 한번에 검사하기 위함
	skillMgr->CheckRedDot(true);
}

bool FGsSkillUIHandler::IsActiveSkillWindow() const
{
	return GUI()->IsActiveWidget(TEXT("WindowSkill"));
}

bool FGsSkillUIHandler::IsSkillIngredientItem(const FGsSchemaItemCommon* InTable) const
{
	if (ItemCategoryMain::SKILLBOOK == InTable->categoryMain)
	{
		// 스킬 습득, 업그레이드 재료
		return true;
	}
#if USE_SKILL_ENCHANT
	else if (ItemCategoryMain::SCROLL == InTable->categoryMain)
	{
		// 강화재료
		if (ItemCategorySub::ENCHANT_SKILL_SCROLL == InTable->categorySub)
		{
			return true;
		}
	}
#endif

	return false;
}

bool FGsSkillUIHandler::GetSkillIdBySkilllBook(FGsItem* InItem, OUT TArray<SkillId>& OutList)
{
	if (nullptr == InItem)
	{
		return false;
	}

	FGsItemConsumable* itemConsumable = InItem->GetCastItem<FGsItemConsumable>();
	if (nullptr == itemConsumable)
	{
		return false;
	}

	const FGsNetUserData* userData = GGameData()->GetUserData();
	if (nullptr == userData)
	{
		return false;
	}

	TArray<int32> userSkillSetIdList;
	if (false == itemConsumable->GetUserSkillSetIdList(userSkillSetIdList))
	{
		return false;
	}

	for (int i = 0; i < userSkillSetIdList.Num(); ++i)
	{
		const FGsSchemaUserSkillSet* userSkillSetRow =
			UGsTableUtil::FindRowById<UGsTableUserSkillSet, FGsSchemaUserSkillSet>(userSkillSetIdList[i]);
		if (nullptr == userSkillSetRow)
		{
			continue;
		}

		if (CreatureGenderType::MALE == userData->mGender)
		{
			if (const FGsSchemaSkillSet* skillSetTable = userSkillSetRow->maleSkillId.GetRow())
			{
				OutList.Emplace(skillSetTable->id);
			}
		}
		else if (CreatureGenderType::FEMALE == userData->mGender)
		{
			if (const FGsSchemaSkillSet* skillSetTable = userSkillSetRow->femaleSkillId.GetRow())
			{
				OutList.Emplace(skillSetTable->id);
			}
		}
	}

	if (0 == OutList.Num())
	{
		return false;
	}

	return true;
}
