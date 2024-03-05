// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSkillManager.h"
#include "Shared/Shared/SharedInclude/SharedFuncs.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "../GameFlow/Stage/GsStageMode.h"
#include "../ScopeGame/GsCostumeManager.h"
#include "../ScopeGame/GsCoolTimeManager.h"
#include "Item/GsItemManager.h"

#include "Item/GsItemEquipment.h"
#include "../Skill/GsComboSkillGroup.h"
#include "../Costume/GsCostumeData.h"

#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageContentHud.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsGameObjectMessageParam.h"
#include "Message/MessageParam/GsCoolTimeMessageParam.h"
#include "Message/MessageParam/GsStageMessageParam.h"
#include "Message/MessageParam/GsUIMessageParam.h"

#include "Data/GsDataContainManager.h"
#include "DataSchema/User/GsSchemaUserData.h"
#include "DataSchema/User/GsSchemaUserSkillSet.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcCommonActionData.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcReactionData.h"
#include "DataSchema/GameObject/Common/GsSchemaIdleEmotionActionData.h"
#include "DataSchema/User/GsSchemaUserCommonActionData.h"
#include "DataSchema/User/GsSchemaUserReactionData.h"
#include "DataSchema/Skill/GsSchemaSkillCommon.h"
#include "DataSchema/Skill/GsSchemaSkillReaction.h"
#include "DataSchema/Skill/Level/GsSchemaSkillLevelSet.h"
#include "DataSchema/Costume/GsSchemaCostumeSkill.h"
#include "DataSchema/Costume/GsSchemaCostumeCommon.h"

#include "GameObject/Data/GsGameObjectDataPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/ObjectClass/GsGameObjectPlayer.h"
#include "GameObject/Parts/GsPartsHandlerPlayer.h"
#include "GameObject/Skill/Checker/GsSkillCheckStageMode.h"
#include "GameObject/Skill/Checker/GsSkillCheckRequireCost.h"
#include "GameObject/Skill/Checker/GsSkillCheckAbnormality.h"
#include "GameObject/Skill/Checker/GsSkillCheckCoolTime.h"
#include "GameObject/Skill/Checker/GsSkillCheckExecuteConditionAbnormality.h"
#include "GameObject/Skill/Checker/GsSkillCheckExecuteConditionPassivity.h"
#include "GameObject/Skill/Checker/GsSkillCheckExecuteConditionStat.h"

#include "UI/UIContent/Define/GsUIContentDefine.h"
#include "Skill/GsSkillHelper.h"
#include "T1Project.h"


const FGsSchemaSkillLevelInfo* UGsSkillManager::FindSkillLevelSet(int InTId, uint8 InSkillLevel)
{
	if (const FGsSchemaSkillLevelSet* tableLevelSet =
		UGsTableUtil::FindRowById<UGsTableSkillLevelSet, FGsSchemaSkillLevelSet>(InTId))
	{
		for (int i = 0; i < tableLevelSet->levelInfoList.Num(); ++i)
		{
			if (InSkillLevel == tableLevelSet->levelInfoList[i].level)
			{
				return &tableLevelSet->levelInfoList[i];
			}
		}
	}
	return nullptr;
}

const FGsSchemaSkillCollision* UGsSkillManager::FindSkillLevelSetToColliosion(int InTId, uint8 InSkillLevel)
{
	if (const FGsSchemaSkillLevelInfo* skillLevelInfo = UGsSkillManager::FindSkillLevelSet(InTId, InSkillLevel))
	{
		for (int id : skillLevelInfo->notifyInfoList)
		{
			if (const FGsSchemaSkillNotifySet* skillNotifySet =
				UGsTableUtil::FindRowById<UGsTableSkillNotifySet, FGsSchemaSkillNotifySet>(id))
			{
				if (skillNotifySet->type == SkillNotifyType::DAMAGE && skillNotifySet->collisionInfoList.Num() > 0)
				{
					const FGsSchemaSkillCollision* skillCollision = skillNotifySet->collisionInfoList.Last().GetRow();
					return skillCollision;
				}
			}
		}
	}
	return nullptr;
}

EGsAnimSlotType UGsSkillManager::GetComboSkillAnimSlotType(const FGsSchemaSkillSet* InSkillData, EGsAnimSlotType InPreveAnimSlotType,
	bool InCurrentCritical)
{
	// 규칙 
	// 1. 선행 (first ComboSkill) 스킬 시작 슬롯은 평타 : 0, 크리 : 2
	// 2. 후행 (Next ComboSkill) 스킬은 선행 시작 Index + (평타 : 0, 크리 : 1)
	if (InSkillData->category == SkillCategory::COMBO)
	{
		// 선행 스킬
		if (InSkillData->sequenceInfo.comboInfo.currentStage == 1)
		{
			return InCurrentCritical ? EGsAnimSlotType::ThirdSlot : EGsAnimSlotType::DefaultSlot;
		}
		// 후행 스킬
		else
		{
			int addtiveIndex = (InCurrentCritical) ? 1 : 0;
			return (EGsAnimSlotType)((uint8)InPreveAnimSlotType + addtiveIndex);
		}
	}

	return EGsAnimSlotType::Max;
}

SkillId UGsSkillManager::FindPassiveSkillIdByUpgradeSkill(const SkillId InSkillId)
{
	// 지연 생성
	if (0 == _upgradeSkillIdAndPassiveSkillId.Num())
	{
		// TDOO: 최적화 필요
		const UGsTableUserSkillSet* UserSkillSetTable = Cast<UGsTableUserSkillSet>(FGsSchemaUserSkillSet::GetStaticTable());
		const UGsTableSkillUpgradeSet* SkillUpgradeSetTable = Cast<UGsTableSkillUpgradeSet>(FGsSchemaSkillUpgradeSet::GetStaticTable());

		if (nullptr != UserSkillSetTable && nullptr != SkillUpgradeSetTable)
		{
			TArray<const FGsSchemaUserSkillSet*> UserSkillSets;
			TArray<const FGsSchemaSkillUpgradeSet*> SkillUpgradeSets;
			if (UserSkillSetTable->GetAllRows(UserSkillSets) && SkillUpgradeSetTable->GetAllRows(SkillUpgradeSets))
			{
				// 패시비티ID가 등록된 UserSkillSetId 맵
				TMap<PassivityId, SkillId> PassivityIdUserSkillSetIdMap;
				for (const FGsSchemaUserSkillSet* UserSkillSet : UserSkillSets)
				{
					if (nullptr == UserSkillSet)
					{
						continue;
					}

					const SkillId UserSkillSetId = UserSkillSet->id;

					const FGsSchemaSkillSet* MaleSkillSet = UserSkillSet->maleSkillId.GetRow();
					if (nullptr != MaleSkillSet)
					{
						for (const FGsSchemaPassivitySetRow& PassivitySetRow : MaleSkillSet->passivityIdList)
						{
							if (!PassivitySetRow.IsNull())
							{
								PassivityIdUserSkillSetIdMap.Add(PassivitySetRow.GetRow()->id, UserSkillSetId);
							}
						}
					}

					const FGsSchemaSkillSet* FemaleSkillSet = UserSkillSet->maleSkillId.GetRow();
					if (nullptr != FemaleSkillSet)
					{
						for (const FGsSchemaPassivitySetRow& PassivitySetRow : FemaleSkillSet->passivityIdList)
						{
							if (!PassivitySetRow.IsNull())
							{
								PassivityIdUserSkillSetIdMap.Add(PassivitySetRow.GetRow()->id, UserSkillSetId);
							}
						}
					}
				}

				// 키: 업그레이드 UserSkillSetId, 값: 패시브 UserSkillSetId
				// TMap<SkillId, SkillId> UpgradeUserSkillSetIdPassiveUserSkillSetIdMap;

				for (const FGsSchemaSkillUpgradeSet* SkillUpgradeSet : SkillUpgradeSets)
				{
					if (nullptr == SkillUpgradeSet)
					{
						continue;
					}

					for (const FGsSchemaSkillUpgradeStepInfo& SkillUpgradeStepInfo : SkillUpgradeSet->upgradeInfoList)
					{
						if (SkillUpgradeStepInfo.upgradeSkillId.IsNull())
						{
							continue;
						}

						const FGsSchemaUserSkillSet* UserSkillSet = SkillUpgradeStepInfo.upgradeSkillId.GetRow();
						const SkillId UpgradeUserSkillSetId = UserSkillSet->id;

						if (SkillUpgradeStepInfo.requirePassivity.IsNull())
						{
							continue;
						}

						const FGsSchemaPassivitySet* PassivitySet = SkillUpgradeStepInfo.requirePassivity.GetRow();
						const SkillId* PassiveUserSkillSetId = PassivityIdUserSkillSetIdMap.Find(PassivitySet->id);
						if (nullptr == PassiveUserSkillSetId)
						{
							continue;
						}

						_upgradeSkillIdAndPassiveSkillId.FindOrAdd(UpgradeUserSkillSetId).Add(*PassiveUserSkillSetId);
					}
				}
			}
		}
	}

	if (INVALID_SKILL_ID == InSkillId)
	{
		return INVALID_SKILL_ID;
	}

	const TSet<SkillId>* PassiveSkillIds = _upgradeSkillIdAndPassiveSkillId.Find(InSkillId);
	if (nullptr == PassiveSkillIds || 0 >= PassiveSkillIds->Num())
	{
		return INVALID_SKILL_ID;
	}

	return *PassiveSkillIds->CreateConstIterator();
}

void UGsSkillManager::Initialize()
{
	// 메세지 바인딩
	_listGameObjectMsg.Emplace(GMessage()->GetGameObject().AddUObject(MessageGameObject::UPDATE_USER_DATA,
		this, &UGsSkillManager::OnInitializeData));

	// 스킬 체크에 필요한 이벤트 바인딩
	_listGameObjectMsg.Emplace(GMessage()->GetGameObject().AddLambda(MessageGameObject::LOCAL_SPAWN_ME_COMPLETE,
		[this](const IGsMessageParam*) { OnCheckSkillSlot(EGsSkillSlotCheckType::MAX); }));

	_listGameObjectMsg.Emplace(GMessage()->GetGameObject().AddLambda(MessageGameObject::LOCAL_REACTION,
		[this](const IGsMessageParam*) { OnCheckSkillSlot(EGsSkillSlotCheckType::CheckReaction); }));
	
	_listGameObjectMsg.Emplace(GMessage()->GetGameObject().AddUObject(MessageGameObject::LOCAL_ABNORMALITY_UPDATE,
		this, &UGsSkillManager::CallbackCheckAbnormality));

	_listGameObjectMsg.Emplace(GMessage()->GetGameObject().AddUObject(MessageGameObject::LOCAL_COOLTIME_START,
		this, &UGsSkillManager::CallbackCheckCooltime));
	_listGameObjectMsg.Emplace(GMessage()->GetGameObject().AddUObject(MessageGameObject::LOCAL_COOLTIME_END,
		this, &UGsSkillManager::CallbackCheckCooltime));

	_listStageMsg.Emplace(GMessage()->GetStageParam().AddUObject(MessageStage::ENTER_STAGE_CALLBACK,
		this, &UGsSkillManager::CallbackCheckStage));
	_listInvasionMsg.Emplace(GMessage()->GetInvasion().AddUObject(MessageInvasion::INVAION_START,
		this, &UGsSkillManager::CallbackCheckStage));
	_listInvasionMsg.Emplace(GMessage()->GetInvasion().AddUObject(MessageInvasion::INVAION_FINISH,
		this, &UGsSkillManager::CallbackCheckStage));

	_listGameObjectMsg.Emplace(GMessage()->GetGameObject().AddLambda(MessageGameObject::PLAYER_STAT_UPDATE,
			[this](const IGsMessageParam*) { OnCheckSkillSlot(EGsSkillSlotCheckType::CheckExecuteConditionStat); }));
	_listGameObjectMsg.Emplace(GMessage()->GetGameObject().AddLambda(MessageGameObject::PASSIVITY_ADD,
		[this](const IGsMessageParam*) { OnCheckSkillSlot(EGsSkillSlotCheckType::CheckExecuteConditionPassivity); }));
	_listGameObjectMsg.Emplace(GMessage()->GetGameObject().AddLambda(MessageGameObject::PASSIVITY_REMOVE,
		[this](const IGsMessageParam*) { OnCheckSkillSlot(EGsSkillSlotCheckType::CheckExecuteConditionPassivity); }));
	// 	
 	_listUiMsg.Emplace(GMessage()->GetUI().AddLambda(MessageUI::INVALIDATE_SLOT_AQUIRE_STAT,
 		[this](const IGsMessageParam*) { OnCheckSkillSlot(EGsSkillSlotCheckType::CheckRequireCost); }));

	_listUiMsg.Emplace(GMessage()->GetUI().AddUObject(MessageUI::SKILL_FORCED_DISABLE,
		this, &UGsSkillManager::OnForcedDisableSkill));

	// 스킬 예약
	_listReserveMsg.Emplace(GMessage()->GetSkillRserveActiveState().AddUObject(MessageContentHud::SKILL_RESERVE_ON,
		this, &UGsSkillManager::OnSkillSlotReserve));
	_listReserveMsg.Emplace(GMessage()->GetSkillRserveActiveState().AddUObject(MessageContentHud::SKILL_RESERVE_OFF,
		this, &UGsSkillManager::OnSkillSlotReserveClear));

	// 코스튬 스킬 정보 처리
	_listCostumeMsg.Emplace(GMessage()->GetCostume().AddUObject(MessageCostume::CHANGE_WEARON,
		this, &UGsSkillManager::CallbackCostumeChanged));

	// 프리셋 변경 처리
	_listGameObjectMsg.Emplace(GMessage()->GetGameObject().AddUObject(MessageGameObject::UPDATE_USER_PRESET_ID,
		this, &UGsSkillManager::CallbackPresetChanged));

	_reinforceChecker.SetCheckFunction([this]()
		{
			return CheckReinforce();
		});

	// 스킬 사용 가능 체크 함수 등록
	// 주의 : EGsSkillSlotCheckType 타입과 인덱스를 맞춰줘야 한다.
	_skillCheck.Add(new FGsSkillCheckStageMode());						// EGsSkillSlotCheckType::CheckStageMode
	_skillCheck.Add(new FGsSkillCheckRequireCost());					// EGsSkillSlotCheckType::CheckRequireCost
	_skillCheck.Add(new FGsSkillCheckExecuteConditionAbnormality());	// EGsSkillSlotCheckType::CheckExecuteConditionAbnormality
	_skillCheck.Add(new FGsSkillCheckExecuteConditionPassivity());		// EGsSkillSlotCheckType::CheckExecuteConditionPassivity
	_skillCheck.Add(new FGsSkillCheckExecuteConditionStat());			// EGsSkillSlotCheckType::CheckExecuteConditionStat
	_skillCheck.Add(new FGsSkillCheckAbnormality());					// EGsSkillSlotCheckType::CheckReaction
	_skillCheck.Add(new FGsSkillCheckCoolTime());						// EGsSkillSlotCheckType::CheckCoolTime
	_skillCheck.Initialize();

	_listSkillSlotUpdateBuffer.Empty();

	_skillUIDataHandler = TUniquePtr<FGsSkillUIDataHandler>(new FGsSkillUIDataHandler());
	_skillUIDataHandler.Get()->Initialize();

	_upgradeSkillIdAndPassiveSkillId.Empty();
}

void UGsSkillManager::Finalize()
{
	_upgradeSkillIdAndPassiveSkillId.Empty();

	// 등록된 스킬 체커 제거
	_skillCheck.RemoveAll();
	_skillCheck.Finalize();

	_listSkillSlotUpdateBuffer.Empty();

	ClearData();

	FGsMessageHolder* message = GMessage();
	for (MsgGameObjHandle& el : _listGameObjectMsg)
	{
		message->GetGameObject().Remove(el);
	}
	for (MsgStageHandle& el : _listStageMsg)
	{
		message->GetStageParam().Remove(el);
	}
	for (MsgInvasionHandle& el : _listInvasionMsg)
	{
		message->GetInvasion().Remove(el);
	}
	for (MsgUIHandle& el : _listUiMsg)
	{
		message->GetUI().Remove(el);
	}
	for (MsgHudHandle& el : _listReserveMsg)
	{
		message->GetSkillRserveActiveState().Remove(el);
	}
	for (DefCostumeMsg& el : _listCostumeMsg)
	{
		message->GetCostume().Remove(el);
	}

	if (_skillUIDataHandler.IsValid())
	{
		_skillUIDataHandler.Get()->Finalize();
	}
	_skillUIDataHandler.Reset();
}

void UGsSkillManager::Update(float inTick)
{
	if (_listSkillSlotUpdateBuffer.Num() > 0)
	{
		for (const FGsSkill* el : _listSkillSlotUpdateBuffer)
		{
			// 해당 슬롯만 갱신
			if (el->_tableData)
			{
				// 체인 스킬은 1프레임 드랍등의 현상으로 최종 정보가 맞지 않는 경우가 있음
				// 현재 업데이트 되려는 스킬 슬롯 정보를 얻어와 최종 체인 스킬 정보로 변경
				if (el->IsChainSkill())
				{
					el = FindSkillSlot(el->GetSlotId());
					if(nullptr == el) continue;
				}

				FGsGameObjectMessageParamSkillSlot param(el->_tableData->requireWeapon, el->GetSlotId(), el);
				GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_CHANGE_SKILL_SLOT, &param);
			}
		}
		_listSkillSlotUpdateBuffer.Empty();
	}
}

void UGsSkillManager::InitializeData(CreatureGenderType GenderType)
{
	InitializeSkillSet(GenderType);
	InitializeCommonAction(GenderType);
	InitializeReaction(GenderType);

	if (_skillUIDataHandler.IsValid())
	{
		_skillUIDataHandler->InitializeData(GenderType);
	}

	_currentGenderType = GenderType;
}

void UGsSkillManager::ClearData()
{
	GMessage()->GetGameObject().SendMessage(MessageGameObject::CLEAR_SKILL_DATA, nullptr);

	ClearDataSkillSet();
	ClearDataCommonAction();
	ClearDataReaction();

	if (_skillUIDataHandler.IsValid())
	{
		_skillUIDataHandler->ClearData();
	}

	_bForcedDisableSkill = false;
}

void UGsSkillManager::InitializeSkillSet(CreatureGenderType GenderType)
{
	if (_mapSkillData.Num() > 0)
	{
		ClearData();
	}

	// 해당 성별의 데이터 추출
	TArray<const FGsSchemaUserSkillSet*> userSkillsetList;
	if (const UGsTable* table = FGsSchemaUserSkillSet::GetStaticTable())
	{
		table->GetAllRows<FGsSchemaUserSkillSet>(userSkillsetList);
	}

	// 이전 스킬 정보 기록을 위한 저장소 key : next, value : current
	TMap<int32, int32> mapPrevSkill;

	DefSkillMap weaponTypeSkill[(uint8)CreatureWeaponType::MAX];
	for (const FGsSchemaUserSkillSet* el : userSkillsetList)
	{
		const FGsSchemaSkillSet* skillData = (GenderType == CreatureGenderType::MALE) ? 
			el->maleSkillId.GetRow() : el->femaleSkillId.GetRow();
		if (skillData)
		{
			TSharedPtr<FGsSkill> newInfo = MakeShareable(new FGsSkill);
			newInfo->_tableData = skillData;
			newInfo->_userSkillSetData = el;
			newInfo->CacheMaxLevel();
			newInfo->_isSelectTarget = el->addTargetButton;

			weaponTypeSkill[(uint8)skillData->requireWeapon].Emplace(skillData->id, newInfo);

			// 연결된 스킬 정보
			if (skillData->sequenceInfo.comboInfo.nextComboSkillId != skillData->id)
			{
				mapPrevSkill.Emplace(skillData->sequenceInfo.comboInfo.nextComboSkillId, skillData->id);
			}
		}
	}

	// 무기별 스킬 데이터 분리 
	_mapSkillData.Emplace(CreatureWeaponType::SWORD, weaponTypeSkill[(uint8)CreatureWeaponType::SWORD]);
	_mapSkillData.Emplace(CreatureWeaponType::DAGGER, weaponTypeSkill[(uint8)CreatureWeaponType::DAGGER]);
	_mapSkillData.Emplace(CreatureWeaponType::BOW, weaponTypeSkill[(uint8)CreatureWeaponType::BOW]);
	_mapSkillData.Emplace(CreatureWeaponType::WAND, weaponTypeSkill[(uint8)CreatureWeaponType::WAND]);
	_mapSkillData.Emplace(CreatureWeaponType::ORB, weaponTypeSkill[(uint8)CreatureWeaponType::ORB]);
	_mapSkillData.Emplace(CreatureWeaponType::SHOTGUN, weaponTypeSkill[(uint8)CreatureWeaponType::SHOTGUN]);
	_mapSkillData.Emplace(CreatureWeaponType::HAND, weaponTypeSkill[(uint8)CreatureWeaponType::HAND]);
	_mapSkillData.Emplace(CreatureWeaponType::ALL, weaponTypeSkill[(uint8)CreatureWeaponType::ALL]);

	// SlotMap 초기 데이터 버퍼 설정
	for (int i = 1; i < (int)CreatureWeaponType::ALL; ++i)
	{
		DefSkillSlotMap newSlotMap;
		_mapSkillSlotData.Emplace(CreatureWeaponType(i), newSlotMap);
	}

	// 프리셋 Slot Map 초기 데이터 버퍼 설정
	_mapPresetSkillSlotData.Empty();
	for (int32 i = 1; i < (int32)CreatureWeaponType::ALL; ++i)
	{
		DefPresetSkillSlotMap newPresetSlotMap;
		for (int32 presetId = MIN_PRESET_ID;presetId <= MAX_PRESET_ID;++presetId)
		{
			DefSkillSlotMap newSlotMap;
			newPresetSlotMap.Emplace(presetId, newSlotMap);
		}
		_mapPresetSkillSlotData.Emplace(CreatureWeaponType(i), newPresetSlotMap);
	}

	// 현재 무기타입 얻기
	TWeakPtr<FGsItem> item = GItem()->GetPlayerEquipedItemBySlotType(EGsEquipSlotType::WEAPON_TYPE);
	if (item.IsValid())
	{
		FGsItemEquipment* equipItem = item.Pin()->GetCastItem<FGsItemEquipment>();
		CreatureWeaponType weaponType = equipItem->GetCreatureWeaponType();
		_currentWeaponSkillSet = _mapSkillData.Find(weaponType);
	}

	// 연결된 스킬이 있을경우 이전 스킬 정보를 기록
	for (TPair<int32, int32> pair : mapPrevSkill)
	{
		if (const FGsSkill* findSkill = FindSkill(pair.Key, CreatureWeaponType::ALL, true))
		{
			_updateSkillData._data.Set(findSkill);
			_updateSkillData._data._prevSkillId = pair.Value;
			UpdateData(findSkill);
		}
	}
}

void UGsSkillManager::InitializeCommonAction(CreatureGenderType GenderType)
{
}

void UGsSkillManager::InitializeReaction(CreatureGenderType GenderType)
{
}

void UGsSkillManager::ClearDataSkillSet()
{
	_listSkillSlotUpdateBuffer.Empty();
	_currentWeaponSkillSet = nullptr;
	_currentWeaponActiveSkillSet.Empty();
	_mapSkillUiset.Empty();
	_mapSkillSlotData.Empty();
	_mapPresetSkillSlotData.Empty();
	_listChainSkillData.Empty();

// 	for (TPair<CreatureWeaponType, DefSkillMap>& pair : _mapSkillData)
// 	{
// 		for (TPair<int, TSharedPtr<const FGsSkill>>& pair2 : pair.Value)
// 		{
// 			pair2.Value = nullptr;
// 		}
// 	}

	_mapSkillData.Empty();
}

void UGsSkillManager::ClearDataCommonAction()
{
	_mapCommonActionData.Empty();
	_mapPlayerCommonActionData.Empty();
}

void UGsSkillManager::ClearDataReaction()
{
	_mapReactionData.Empty();
	_mapPlayerReactionData.Empty();
}

void UGsSkillManager::MakeDataWeaponActiveSkillSet()
{
	if (nullptr == _currentWeaponSkillSet)
	{
		GSLOG(Error, TEXT("_currentWeaponSkillSet nullptr"));
		return;
	}

	if (_currentWeaponActiveSkillSet.Num() <= 0)
	{
		for (const TPair<int, TSharedPtr<const FGsSkill>>& pair : *_currentWeaponSkillSet)
		{
			if (pair.Value->_isLearn)
			{
				_currentWeaponActiveSkillSet.Emplace(pair.Key, pair.Value);
			}
		}
		// ALL 타입 추가
		if (const DefSkillMap* allType = _mapSkillData.Find(CreatureWeaponType::ALL))
		{
			for (const TPair<int, TSharedPtr<const FGsSkill>>& pair : *allType)
			{
				if (pair.Value->_isLearn)
				{
					_currentWeaponActiveSkillSet.Emplace(pair.Key, pair.Value);
				}
			}
		}
	}
}

void UGsSkillManager::MakeDataSkillUiSet()
{
	for (TPair<CreatureWeaponType, DefSkillMap>& pair : _mapSkillData)
	{
		for (TPair<int, TSharedPtr<const FGsSkill>>& pair2 : pair.Value)
		{
			// 평타 제거
			if (pair2.Value->_tableData->isNormal)
			{
				continue;
			}

			SkillCategorySet categorySet = SharedFunc::GetSkillCategorySet(pair2.Value->_tableData->category);
			DefWeaponSkillCatergorySetKey key(pair.Key, categorySet);

			if (TArray<TSharedPtr<const FGsSkill>>* findList = _mapSkillUiset.Find(key))
			{
				findList->Emplace(pair2.Value);
			}
			else
			{
				TArray<TSharedPtr<const FGsSkill>> newList;
				newList.Emplace(pair2.Value);
				_mapSkillUiset.Emplace(key, newList);
			}
		}
	}
}

void UGsSkillManager::LoadTopoStart()
{
	// 그룹 스킬 리셋
	for (FGsComboSkillGroup& el : _listChainSkillData)
	{
		el.Reset();
	}
}

void UGsSkillManager::NetRecvUserSkillSet(PD::SC::PKT_SC_SKILL_READ* Packet)
{
	using skillSlotIter = PktIterator<PD::SC::PKT_SC_SKILL_READ::SkillSlotList>;
	int i = 0;
	for (skillSlotIter it = Packet->GetFirstSkillSlotListIterator();
		it != Packet->GetLastSkillSlotListIterator(); ++it, ++i)
	{
		const FGsSchemaSkillSet* skillTable = UGsTableUtil::FindRowById<UGsTableSkillSet, FGsSchemaSkillSet>(it->SkillId());
		if (nullptr == skillTable)
		{
			continue;
		}

		if (const FGsSkill* findData = FindSkill(skillTable->id, skillTable->requireWeapon))
		{
			_updateSkillData._data.Set(findData);
			_updateSkillData._data.SetLevel(it->SkillLevel());
			_updateSkillData._data._isLearn = true;
			_updateSkillData._data._presetData.Empty();
			_updateSkillData._data.SetSkillCycle(it->IsUseSkillInterval(), it->IsUseSkillIntervalInPVP(), 
				it->UseSkillIntervalSecond());

			using skillSlotPresetIter = PktIterator<PD::SC::PKT_SC_SKILL_READ::SkillSlotList::PresetList>;
			for (skillSlotPresetIter presetIter = it->GetFirstPresetListIterator();
				presetIter != it->GetLastPresetListIterator(); ++presetIter)
			{
				TPair<SkillSlotId, bool> presetData{ presetIter->SkillSlotId(), presetIter->AutoUse() };
				_updateSkillData._data._presetData.Emplace(presetIter->PresetId(), presetData);
			}
			PresetId currentPresetId = GGameData()->GetUserData()->mUserPresetId;
			if (TPair<SkillSlotId, bool>* presetData = _updateSkillData._data._presetData.Find(currentPresetId))
			{
				_updateSkillData._data._slotId = presetData->Key;
				_updateSkillData._data.SetUseAuto(presetData->Value);
			}

			UpdateData(findData);

			SetSkillSlot(skillTable->requireWeapon, _updateSkillData._data._slotId, findData, false);		
			SetSkillSlotPreset(findData, false);
		}		
	}
}

void UGsSkillManager::NetChangeWeaponFinishedAck(PD::SC::PKT_SC_ITEM_EQUIP_CHANGE_WEAPON_TYPE_FINISHED_READ* Packet)
{
	if (Packet->Reason() == ChangeWeaponTypeFinishReason::NORMAL)
	{
		CreatureWeaponType weaponType = CreatureWeaponType::HAND;
		// 현재 무기타입 얻기
		TWeakPtr<FGsItem> item = GItem()->GetPlayerEquipedItemBySlotType(EGsEquipSlotType::WEAPON_TYPE);
		if (item.IsValid())
		{
			FGsItemEquipment* equipItem = item.Pin()->GetCastItem<FGsItemEquipment>();
			weaponType = equipItem->GetCreatureWeaponType();			
		}

		ChangeWeapon(weaponType);
	}
}

void UGsSkillManager::UpdateData(const FGsSkill* Data)
{
	if(nullptr != Data)
	{
		FGsSkill* applay = const_cast<FGsSkill*>(Data);
		applay->Set(_updateSkillData._data);
	}
}

void UGsSkillManager::UpdateWeaponSkillSet(CreatureWeaponType WeaponType)
{
	// 현재 무기 정보 갱신
	_currentWeaponSkillSet = _mapSkillData.Find(WeaponType);

	// 배운스킬 복사본을 비운다.
	_currentWeaponActiveSkillSet.Empty();
	// 스킬중 배운 스킬 정보의 복사본을 생성한다.
	MakeDataWeaponActiveSkillSet();

	_currentWeaponType = WeaponType;

	// 체인 스킬 정보 업데이트
	for (const FGsComboSkillGroup& el : _listChainSkillData)
	{
		RestoreChainSkill(el);
	}
	_listChainSkillData.Empty();

	if (DefSkillSlotMap* findSlotData = _mapSkillSlotData.Find(_currentWeaponType))
	{
		for (DefSkillSlotKey pair : (*findSlotData))
		{
			SetChainSkill(pair.Value);
		}
	}	
}

void UGsSkillManager::ChangeWeapon(CreatureWeaponType WeaponType)
{
	UpdateWeaponSkillSet(WeaponType);

	// 슬롯 정보 전체 갱신
	OnCheckSkillSlot(EGsSkillSlotCheckType::MAX);

	// 메세지 전송
	GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_CHANGE_SKILL_SET, nullptr);
}

const FGsSchemaUserCommonActionInfo* UGsSkillManager::LoadCommonActionDataPlayer(CreatureGenderType GenderType,
	CreatureWeaponType WeaponType, CommonActionType Type)
{
	const UGsTableUserCommonActionData* gsTable = Cast<UGsTableUserCommonActionData>(
		FGsSchemaUserCommonActionData::GetStaticTable());
	const FGsSchemaUserCommonActionData* schema;
	if (gsTable->FindRowById(GenderType, schema))
	{
		const FGsSchemaUserCommonActionInfo* commonAction = schema->commonActionInfo.FindByPredicate(
			[Type](FGsSchemaUserCommonActionInfo& el) {
			return el.type == Type;
		});

		if (commonAction)
		{
			if (DefPlayerCommonMap* findMap = _mapPlayerCommonActionData.Find(GenderType))
			{
				findMap->Emplace(commonAction->type, commonAction);
			}
			else
			{
				DefPlayerCommonMap newInfo;
				newInfo.Emplace(commonAction->type, commonAction);
				_mapPlayerCommonActionData.Emplace(GenderType, newInfo);
			}

			return commonAction;
		}
	}

	return nullptr;
}

void UGsSkillManager::LoadCommonActionDataNpc(OUT TArray<const FGsSchemaSkillCommon*>& OutCommonSet, int GameObjectTId)
{
	// NpcData 얻기
	const FGsSchemaNpcData* npcData = UGsTableUtil::FindRowById<UGsTableNpcData, FGsSchemaNpcData>(GameObjectTId);
	if (nullptr == npcData)
	{
		return;
	}

	// NpcCommonActionData 얻기
	const FGsSchemaNpcCommonActionData* commonActionData = npcData->commonActionListId.GetRow();
	if (nullptr == commonActionData)
	{
		return;
	}

	DefObjectKey key(EGsGameObjectType::NonPlayer, GameObjectTId);
	for (const FGsSchemaNpcCommonActionInfo& el : commonActionData->commonActionInfo)
	{
		for (const FGsSchemaSkillCommonRow& el2 : el.commonActionId)
		{
			AddNpcCommonActionData(key, el2.GetRow());
		}
	}

	if (DefNpcCommonMap* findMap = _mapCommonActionData.Find(key))
	{
		for (TPair<CommonActionType, TArray<const FGsSchemaSkillCommon*>>& pair : (*findMap))
		{
			OutCommonSet.Append(pair.Value);
		}
	}
}

const FGsSchemaUserReactionInfo* UGsSkillManager::LoadReactionDataPlayer(CreatureGenderType GenderType, CreatureWeaponType WeaponType,
	EReactionType Type)
{
	const UGsTableUserReactionData* gsTable = Cast<UGsTableUserReactionData>(
		FGsSchemaUserReactionData::GetStaticTable());
	const FGsSchemaUserReactionData* schema;
	if (gsTable->FindRowById(GenderType, schema))
	{
		const FGsSchemaUserReactionInfo* reaction = schema->reactionInfo.FindByPredicate(
			[Type](FGsSchemaUserReactionInfo& el) {
			return el.type == Type;
		});

		if (reaction)
		{
			if (DefPlayerReactionMap* findMap = _mapPlayerReactionData.Find(GenderType))
			{
				findMap->Emplace(reaction->type, reaction);
			}
			else
			{
				DefPlayerReactionMap newInfo;
				newInfo.Emplace(reaction->type, reaction);
				_mapPlayerReactionData.Emplace(GenderType, newInfo);
			}

			return reaction;
		}
	}

	return nullptr;
}

void UGsSkillManager::LoadReactionDataNpc(OUT TArray<const FGsSchemaSkillReaction*>& OutReactionSet, int GameObjectTId)
{
	// NpcData 얻기
	const FGsSchemaNpcData* npcData = UGsTableUtil::FindRowById<UGsTableNpcData, FGsSchemaNpcData>(GameObjectTId);
	if (nullptr == npcData)
	{
		return;
	}

	// NpcRectionData 얻기
	const FGsSchemaNpcReactionData* reactionData = npcData->reactionListId.GetRow();
	if (nullptr == reactionData)
	{
		return;
	}

	DefObjectKey key(EGsGameObjectType::NonPlayer, GameObjectTId);
	for (const FGsSchemaNpcReactionInfo& el : reactionData->reactionInfo)
	{
		AddNpcReactionData(key, el.reactionSetId.GetRow());
	}

	if (const DefNpcReactionMap* findMap = _mapReactionData.Find(key))
	{
		for (TPair<EReactionType, TArray<const FGsSchemaSkillReaction*>> pair : *findMap)
		{
			OutReactionSet.Append(pair.Value);
		}
	}
}

void UGsSkillManager::LoadReactionDataPolymorphShape(OUT TArray<const FGsSchemaSkillReaction*>& OutReactionSet, int GameObjectTId)
{
	const FGsSchemaNpcReactionData* reactionData = nullptr;
	if (const auto table = Cast<UGsTableNpcReactionData>(FGsSchemaNpcReactionData::GetStaticTable()))
	{
		table->FindRowById(GameObjectTId, reactionData);
	}
	if (nullptr == reactionData)
	{
		return;
	}

	DefObjectKey key(EGsGameObjectType::NonPlayer, GameObjectTId);
	for (const FGsSchemaNpcReactionInfo& el : reactionData->reactionInfo)
	{
		AddNpcReactionData(key, el.reactionSetId.GetRow());
	}

	if (const DefNpcReactionMap* findMap = _mapReactionData.Find(key))
	{
		for (TPair<EReactionType, TArray<const FGsSchemaSkillReaction*>> pair : *findMap)
		{
			OutReactionSet.Append(pair.Value);
		}
	}
}

void UGsSkillManager::AddNpcCommonActionData(const DefObjectKey& key, const FGsSchemaSkillCommon* Data)
{
	if (nullptr == Data)
	{
		return;
	}

	if (DefNpcCommonMap* findData = _mapCommonActionData.Find(key))
	{
		if (TArray<const FGsSchemaSkillCommon*>* findList = findData->Find(Data->category))
		{
			findList->AddUnique(Data);
		}
		else
		{
			TArray<const FGsSchemaSkillCommon*> newList;
			newList.Emplace(Data);
			findData->Emplace(Data->category, newList);
		}
	}
	else
	{
		_mapCommonActionData.Emplace(key);
		AddNpcCommonActionData(key, Data);
	}
}

void UGsSkillManager::AddNpcReactionData(const DefObjectKey& key, const FGsSchemaSkillReaction* Data)
{
	if (nullptr == Data)
	{
		return;
	}

	if (DefNpcReactionMap* findData = _mapReactionData.Find(key))
	{
		if (TArray<const FGsSchemaSkillReaction*>* findList = findData->Find(Data->category))
		{
			findList->AddUnique(Data);
		}
		else
		{
			TArray<const FGsSchemaSkillReaction*> newList;
			newList.Emplace(Data);
			findData->Emplace(Data->category, newList);
		}
	}
	else
	{
		_mapReactionData.Emplace(key);
		AddNpcReactionData(key, Data);
	}
}

void UGsSkillManager::RemoveNpcCommonActionData(EGsGameObjectType Type, int GameObjectTId)
{
	_mapCommonActionData.Remove(DefObjectKey(Type, GameObjectTId));
}

void UGsSkillManager::RemoveNpcReactionData(EGsGameObjectType Type, int GameObjectTId)
{
	_mapReactionData.Remove(DefObjectKey(Type, GameObjectTId));
}

void UGsSkillManager::OnInitializeData(const IGsMessageParam* Param)
{
	const FGsNetUserData* castParam = Param->Cast<const FGsNetUserData>();
	InitializeData(castParam->mGender);
}

void UGsSkillManager::UpdateSkillSlotData(EGsSkillSlotCheckType InCheckType, SkillSlotId Index)
{
	if (const FGsSkill* skillData = FindSkillSlot(Index))
	{
		bool prevResult = skillData->IsEnable();
		_updateSkillData._data.Set(skillData);

		EGsSkillCheckResultType result = EGsSkillCheckResultType::Pass;
		// 전체 검사
		if (InCheckType == EGsSkillSlotCheckType::MAX)
		{
			for (int i = 0; i < (int)EGsSkillSlotCheckType::MAX; ++i)
			{
				result = _skillCheck.ProcessCheck(skillData, i);
				_updateSkillData._data.SetSkillCheckResult((EGsSkillSlotCheckType)i, !(result == EGsSkillCheckResultType::Pass));
			}
		}
		else
		{
			result = _skillCheck.ProcessCheck(skillData, (int)InCheckType);
			_updateSkillData._data.SetSkillCheckResult(InCheckType, !(result == EGsSkillCheckResultType::Pass));
		}

		UpdateData(skillData);

		if (prevResult != _updateSkillData._data.IsEnable())
		{
			// 업데이트 큐에 넣음
			_listSkillSlotUpdateBuffer.Emplace(skillData);
		}
#if 0 & WITH_EDITOR

		if (_updateSkillData._data._slotData._skillCheckResult != 0)
		{
			GSLOG(Warning, TEXT("EGsSkillSlotCheckType 확인용 SkillId : %d"), _updateSkillData._data._tableData->id);
			// 스킬 에러 타입 확인용
			// EGsSkillSlotCheckType 문자열변환
			static FString checkTypes[] = {
				TEXT("CheckStageMode"),
				TEXT("CheckRequireCost"),
				TEXT("CheckExecuteConditionAbnormality"),
				TEXT("CheckExecuteConditionPassivity"),
				TEXT("CheckExecuteConditionStat"),
				TEXT("CheckReaction"),
				TEXT("CheckCoolTime")
			};

			static FString checkClass[] = {
				TEXT("FGsSkillCheckStageMode"),
				TEXT("FGsSkillCheckRequireCost"),
				TEXT("FGsSkillCheckExecuteConditionAbnormality"),
				TEXT("FGsSkillCheckExecuteConditionPassivity"),
				TEXT("FGsSkillCheckExecuteConditionStat"),
				TEXT("FGsSkillCheckAbnormality"),
				TEXT("FGsSkillCheckCoolTime")
			};

			for (uint8 i = 0; i < (uint8)EGsSkillSlotCheckType::MAX; ++i)
			{
				if (_updateSkillData._data._slotData._skillCheckResult & (1 << i))
				{
					GSLOG(Warning, TEXT("EGsSkillSlotCheckType::%s 타입의 유효성 확인 ( Class::ProcessCheck %s )"),
						*checkTypes[i], *checkClass[i]);
				}
			};
		}
#endif
	}
}

void UGsSkillManager::OnCheckSkillSlot()
{
	if (DefSkillSlotMap* findSkillSlot = _mapSkillSlotData.Find(_currentWeaponType))
	{
		for (DefSkillSlotKey pair : *findSkillSlot)
		{
			UpdateSkillSlotData(EGsSkillSlotCheckType::MAX, pair.Key);
		}
	}
}

void UGsSkillManager::OnCheckSkillSlot(EGsSkillSlotCheckType InCheckType)
{
	// 현재 스킬 슬롯 정보에서 스킬 사용 가능 유무 체크
	if (DefSkillSlotMap* findSkillSlot = _mapSkillSlotData.Find(_currentWeaponType))
	{
		for (DefSkillSlotKey pair : *findSkillSlot)
		{
			UpdateSkillSlotData(InCheckType, pair.Key);
		}
	}
}

void UGsSkillManager::OnForcedDisableSkill(const IGsMessageParam* InParam)
{
	if (nullptr == InParam)
	{
		return;
	}

	const FGsUIMsgParamBool* param = InParam->Cast<const FGsUIMsgParamBool>();
	if (nullptr == param)
	{
		return;
	}

	_bForcedDisableSkill = param->_data;

	if (DefSkillSlotMap* findSkillSlot = _mapSkillSlotData.Find(_currentWeaponType))
	{
		for (DefSkillSlotKey pair : *findSkillSlot)
		{
			UpdateSkillSlotData(EGsSkillSlotCheckType::CheckStageMode, pair.Key);
		}
	}
}

void UGsSkillManager::OnSkillSlotReserve(int32 InSkillId)
{
	// 전체 슬롯 정보 갱신
	if (DefSkillSlotMap* findSkillSlot = _mapSkillSlotData.Find(_currentWeaponType))
	{
		for (DefSkillSlotKey pair : *findSkillSlot)
		{
			_updateSkillData._data.Set(pair.Value);
			_updateSkillData._data.SetReserve(InSkillId == pair.Value->GetSkillId());
			UpdateData(pair.Value);

			// 해당 슬롯만 갱신
			if (pair.Value->IsReserve())
			{
				FGsUIMsgParamInt param(pair.Value->GetSlotId());
				GMessage()->GetUI().SendMessage(MessageUI::RESERVE_SKILL_SLOT, &param);
			}
		}
	}
}

void UGsSkillManager::OnSkillSlotReserveClear(int32 InSkillId)
{
	if (const FGsSkill* findSkill = FindSkill(InSkillId))
	{
		_updateSkillData._data.Set(findSkill);
		_updateSkillData._data.SetReserve(false);
		UpdateData(findSkill);

		// 해당 슬롯만 갱신
		FGsUIMsgParamInt param(findSkill->GetSlotId());
		GMessage()->GetUI().SendMessage(MessageUI::CANCEL_RESERVE_SKILL_SLOT, &param);
	}
}

void UGsSkillManager::CallbackCheckAbnormality(const IGsMessageParam* InParam)
{
	if (const FGsLocalGameObjectMessageParamAbnormality* param = 
		InParam->Cast<const FGsLocalGameObjectMessageParamAbnormality>())
	{
		if (param->IsAbnormalityEffectType(AbnormalityEffectType::STAT_VENT))
		{
			OnCheckSkillSlot(EGsSkillSlotCheckType::CheckRequireCost);
		}
		OnCheckSkillSlot(EGsSkillSlotCheckType::CheckExecuteConditionAbnormality);
	}
}

void UGsSkillManager::CallbackCheckCooltime(const IGsMessageParam* InParam)
{
	if (const FGsCoolTimeMessageParam* param = InParam->Cast<const FGsCoolTimeMessageParam>())
	{
		if (const FGsSkill* skillData = FindActiveSkill(param->_id))
		{
			UpdateSkillSlotData(EGsSkillSlotCheckType::CheckCoolTime, skillData->GetSlotId());

			// 스킬 사용 주기 시간 갱신
			if (skillData->IsActiveSkillCycle() && param->_remainTime <= 0.f)
			{
				_updateSkillData._data.Set(skillData);
				_updateSkillData._data.UpdateCycleTimer();
				UpdateData(skillData);
			}
		}
	}
}

void UGsSkillManager::CallbackCheckStage(const IGsMessageParam* Param)
{
	OnCheckSkillSlot(EGsSkillSlotCheckType::CheckStageMode);

	// 스테이지 전환시 AutoBuff 사용 가능 구간이면 자동 사용 실행 메세지
	if (Param)
	{
		if (const FGsGameStageEnterMsgParam* castParam = Param->Cast<const FGsGameStageEnterMsgParam>())
		{
			FGsGameObjectMessageParamBuffSkill messageParam(nullptr, 0);
			if (castParam->_type == FGsStageMode::Game::TOWN ||
				castParam->_type == FGsStageMode::Game::SAFETY_ZONE ||
				castParam->_type == FGsStageMode::Game::ARENA_READY)
			{
				messageParam._autoEnable = FGsGameObjectMessageParamBuffSkill::BuffSkillAuto::Disable;
			}

			GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_AUTOSKILL_UPDATE, &messageParam);
		}
	}
}

void UGsSkillManager::CallbackChainSkillEnd(class FGsComboSkillGroup* SkillGroup)
{
	const FGsSkill* firstSkill = SkillGroup->GetCurrentData();
	SetSkillSlot(firstSkill->_tableData->requireWeapon, firstSkill->_slotId, firstSkill, true);
}

void UGsSkillManager::CallbackCostumeChanged(const IGsMessageParam* Param)
{
	if (const FGsUIMsgParamInt* castParam = Param->Cast<const FGsUIMsgParamInt>())
	{
		// 현재 활성화된 스킬셋 정보 재설정
		UpdateWeaponSkillSet(_currentWeaponType);
	}
}

void UGsSkillManager::CallbackPresetChanged(const IGsMessageParam* InParam)
{
	if (const FGsPrimitiveUInt8* param = InParam->Cast<const FGsPrimitiveUInt8>())
	{
		PresetId presetId = param->_data;

		for (TPair<CreatureWeaponType, DefSkillMap>& pair : _mapSkillData)
		{
			for (TPair<int, TSharedPtr<const FGsSkill>>& pair2 : pair.Value)
			{
				_updateSkillData._data.Set(pair2.Value.Get());
				if (TPair<SkillSlotId, bool>* presetData = _updateSkillData._data._presetData.Find(presetId))
				{
					_updateSkillData._data._slotId = presetData->Key;
					_updateSkillData._data.SetUseAuto(presetData->Value);
				}
				UpdateData(pair2.Value.Get());
			}
		}

		// 스킬 슬롯 정보 교체
		for (int32 i = (int32)CreatureWeaponType::NONE + 1; i < (int32)CreatureWeaponType::MAX; ++i)
		{
			CreatureWeaponType weaponType = (CreatureWeaponType)i;
			for (int32 slotId = 1;slotId <= MAX_COSTUME_SKILL_SLOT_ID;++slotId)
			{
				if (_mapPresetSkillSlotData.Contains(weaponType))
				{
					DefSkillSlotMap& presetSkillSlotMap = _mapPresetSkillSlotData[weaponType][presetId];
					if (presetSkillSlotMap.Contains(slotId))
					{
						SetSkillSlot(weaponType, slotId, presetSkillSlotMap[slotId], false);
					}
					else
					{
						SetSkillSlot(weaponType, slotId, nullptr, false);
					}
				}
			}
		}

		CreatureWeaponType weaponType = CreatureWeaponType::HAND;
		// 현재 무기타입 얻기
		TWeakPtr<FGsItem> item = GItem()->GetPlayerEquipedItemBySlotType(EGsEquipSlotType::WEAPON_TYPE);
		if (item.IsValid())
		{
			FGsItemEquipment* equipItem = item.Pin()->GetCastItem<FGsItemEquipment>();
			weaponType = equipItem->GetCreatureWeaponType();
		}
		ChangeWeapon(weaponType);
	}
}

void UGsSkillManager::CheckSkillSlotCost()
{
	OnCheckSkillSlot(EGsSkillSlotCheckType::CheckRequireCost);
}

int UGsSkillManager::GetWeaponSkillSet(OUT TArray<TSharedPtr<const FGsSkill>>& OutSkillSet, CreatureWeaponType WeaponType) const
{
	if (WeaponType != CreatureWeaponType::MAX)
	{
		if (const DefSkillMap* findWeaponType = _mapSkillData.Find(WeaponType))
		{
			findWeaponType->GenerateValueArray(OutSkillSet);
			return OutSkillSet.Num();
		}
	}
	else if (_currentWeaponSkillSet)
	{
		_currentWeaponSkillSet->GenerateValueArray(OutSkillSet);
		return OutSkillSet.Num();
	}
	
	return 0;
}

int UGsSkillManager::GetCurrentWeaponLeanSkillSet(OUT TArray<TSharedPtr<const FGsSkill>>& OutSkillSet) const
{
	_currentWeaponActiveSkillSet.GenerateValueArray(OutSkillSet);
	return OutSkillSet.Num();
}

int UGsSkillManager::GetSkillUiSet(OUT TArray<TSharedPtr<const FGsSkill>>& OutSkillSet,
	CreatureWeaponType WeaponType, SkillCategorySet CategorySet)
{
	if (_mapSkillUiset.Num() <= 0)
	{
		MakeDataSkillUiSet();
	}

	DefWeaponSkillCatergorySetKey key(WeaponType, CategorySet);
	if (TArray<TSharedPtr<const FGsSkill>>* findValue = _mapSkillUiset.Find(key))
	{
		OutSkillSet = (*findValue);
		return (*findValue).Num();
	}
	return 0;
}

TSharedPtr<const FGsSkill> UGsSkillManager::FindSkillSharedPtr(int tId, CreatureWeaponType WeaponType, bool bSearchAll) const
{
	if (bSearchAll)
	{
		// 무기를 모두 뒤져서 찾기
		for (TPair<CreatureWeaponType, DefSkillMap> iter : _mapSkillData)
		{
			if (const DefSkillMap* findWeapon = _mapSkillData.Find(iter.Key))
			{
				if (TSharedPtr<const FGsSkill>const* findSkill = findWeapon->Find(tId))
				{
					return *findSkill;
				}
			}
		}
	}
	else
	{
		if (WeaponType != CreatureWeaponType::MAX)
		{
			if (const DefSkillMap* findWeapon = _mapSkillData.Find(WeaponType))
			{
				if (TSharedPtr<const FGsSkill>const* findSkill = findWeapon->Find(tId))
				{
					return *findSkill;
				}
			}
		}
		else if (_currentWeaponSkillSet)
		{
			if (TSharedPtr<const FGsSkill>const* findSkill = _currentWeaponSkillSet->Find(tId))
			{
				// 현재 무기에서 찾기
				return *findSkill;
			}
		}
	}

#if WITH_EDITOR
	GSLOG(Error, TEXT("Fail to find FGsSkill. Weapon: %d, SkillId: %d"), WeaponType, tId);
#endif

	return nullptr;
}

TWeakPtr<const FGsSkill> UGsSkillManager::FindSkillWeakPtr(int tId, CreatureWeaponType WeaponType, bool bSearchAll) const
{
	return FindSkillSharedPtr(tId, WeaponType, bSearchAll);
}

const FGsSkill* UGsSkillManager::FindSkill(int tId, CreatureWeaponType WeaponType, bool bSearchAll) const
{
	TSharedPtr<const FGsSkill> skillData = FindSkillSharedPtr(tId, WeaponType, bSearchAll);
	return skillData.Get();
}

const FGsSkill* UGsSkillManager::FindActiveSkill(int tId) const
{
	if (TSharedPtr<const FGsSkill>const* findSkill = _currentWeaponActiveSkillSet.Find(tId))
	{
		return findSkill->Get();
	}
	return nullptr;
}

const FGsSkill* UGsSkillManager::FindSkillSlot(SkillSlotId Index) const
{
	if (const DefSkillSlotMap* slotMap = _mapSkillSlotData.Find(_currentWeaponType))
	{
		if (const FGsSkill*const* findSkill = slotMap->Find(Index))
		{
			return *findSkill;
		}
	}

	return nullptr;
}

const FGsComboSkillGroup* UGsSkillManager::FindChainSkillGroup(int tId) const
{
	return _listChainSkillData.FindByPredicate([tId](FGsComboSkillGroup& el)
		{
			return el.Contain(tId);
		});
}

const FGsSchemaSkillCommon* UGsSkillManager::GetPlayerCommonActionData(UGsGameObjectBase* ObjectClass, 
	CommonActionType CommonType)
{
	if (nullptr == ObjectClass || false == ObjectClass->IsObjectType(EGsGameObjectType::Player))
	{
		return nullptr;
	}

	FGsGameObjectDataPlayer* playerData = ObjectClass->GetCastData<FGsGameObjectDataPlayer>();
	CreatureGenderType genderType = playerData->GetCreatureGenderType();
	CreatureWeaponType weaponType = playerData->GetCurrentWeaponType();
	const FGsSchemaUserCommonActionInfo* result = nullptr;
	if (DefPlayerCommonMap* findMap = _mapPlayerCommonActionData.Find(genderType))
	{
		if (const FGsSchemaUserCommonActionInfo** findValue = (*findMap).Find(CommonType))
		{
			result = *findValue;
		}
	}

	if (nullptr == result)
	{
		result = LoadCommonActionDataPlayer(genderType, weaponType, CommonType);
	}

	if (result)
	{
		switch (weaponType)
		{
		case  CreatureWeaponType::SWORD:
			return result->sword.GetRow();
		case  CreatureWeaponType::DAGGER:
			return result->dagger.GetRow();
		case  CreatureWeaponType::BOW:
			return result->bow.GetRow();
		case  CreatureWeaponType::WAND:
			return result->wand.GetRow();
		case CreatureWeaponType::HAND:
			return result->hand.GetRow();
		case CreatureWeaponType::ORB:
			return result->orb.GetRow();
 		case CreatureWeaponType::SHOTGUN:
 			return result->shotgun.GetRow();
		default:
			break;
		}
	}

	return nullptr;
}

const FGsSchemaSkillReaction* UGsSkillManager::GetPlayerReactionData(UGsGameObjectBase* ObjectClass, 
	EReactionType ReactionType)
{
	if (nullptr == ObjectClass || false == ObjectClass->IsObjectType(EGsGameObjectType::Player))
	{
		return nullptr;
	}

	FGsGameObjectDataPlayer* playerData = ObjectClass->GetCastData<FGsGameObjectDataPlayer>();
	CreatureGenderType genderType = playerData->GetCreatureGenderType();
	CreatureWeaponType weaponType = playerData->GetCurrentWeaponType();
	const FGsSchemaUserReactionInfo* result = nullptr;
	if (DefPlayerReactionMap* findMap = _mapPlayerReactionData.Find(genderType))
	{
		if (const FGsSchemaUserReactionInfo** findValue = (*findMap).Find(ReactionType))
		{
			result = *findValue;
		}
	}

	if(nullptr == result)
	{
		result = LoadReactionDataPlayer(genderType, weaponType, ReactionType);
	}

	if (result)
	{
		// TODO 단일 RowPicker로 수정하기
		switch (weaponType)
		{
		case  CreatureWeaponType::SWORD:
			return  result->sword.GetRow();
		case  CreatureWeaponType::DAGGER:
			return  result->dagger.GetRow();
		case  CreatureWeaponType::BOW:
			return  result->bow.GetRow();
		case  CreatureWeaponType::WAND:
			return  result->wand.GetRow();
		case CreatureWeaponType::HAND:
			return result->hand.GetRow();
		default:
			break;
		}
	}

	return nullptr;
}

const FGsSchemaSkillReaction* UGsSkillManager::GetNpcReactionDataByTableID(int32 GameObjectTId, EReactionType ReactionType)
{
	TArray<const FGsSchemaSkillReaction*> OutCommonSet;
	if (GSkill()->GetNpcReactionSet(OutCommonSet, GameObjectTId, ReactionType) > 0)
		return OutCommonSet.Last();
	return nullptr;
}

const FGsSchemaSkillCommon* UGsSkillManager::GetNpcCommonActionData(UGsGameObjectBase* ObjectClass,
	CommonActionType CommonType)
{
	TArray<const FGsSchemaSkillCommon*> outlist;
	if (0 < GetNpcCommonActionSet(outlist, ObjectClass->GetData()->GetTableId(), CommonType))
	{
		return outlist.Last();
	}

	return nullptr;
}

const FGsSchemaSkillReaction* UGsSkillManager::GetReactionData(UGsGameObjectBase* ObjectClass, 
	EReactionType ReactionType)
{
	return ObjectClass->IsObjectType(EGsGameObjectType::Player) ? GetPlayerReactionData(ObjectClass, ReactionType) :
		GetNpcReactionData(ObjectClass, ReactionType);
}

const FGsSchemaSkillReaction* UGsSkillManager::GetNpcReactionData(UGsGameObjectBase* ObjectClass,
	EReactionType ReactionType)
{
	TArray<const FGsSchemaSkillReaction*> outlist;
	if (0 < GetNpcReactionSet(outlist, ObjectClass->GetData()->GetTableId(), ReactionType))
	{
		return outlist.Last();
	}

	return nullptr;
}

int UGsSkillManager::GetNpcCommonActionSet(OUT TArray<const FGsSchemaSkillCommon*>& OutCommonSet, int GameObjectTId)
{
	DefObjectKey key(EGsGameObjectType::NonPlayer, GameObjectTId);
	if (DefNpcCommonMap* findMap = _mapCommonActionData.Find(key))
	{
		for (TPair<CommonActionType, TArray<const FGsSchemaSkillCommon*>>& pair : (*findMap))
		{
			OutCommonSet.Append(pair.Value);
		}
	}
	else
	{
		LoadCommonActionDataNpc(OutCommonSet, GameObjectTId);
	}

	return OutCommonSet.Num();
}

int UGsSkillManager::GetNpcCommonActionSet(OUT TArray<const FGsSchemaSkillCommon*>& OutCommonSet, int GameObjectTId, CommonActionType FindType)
{
	if (DefNpcCommonMap* findMap = _mapCommonActionData.Find(DefObjectKey(EGsGameObjectType::NonPlayer, GameObjectTId)))
	{
		if (TArray<const FGsSchemaSkillCommon*>* findList = findMap->Find(FindType))
		{
			OutCommonSet.Append(*findList);
		}
	}
	else
	{
		GetNpcCommonActionSet(OutCommonSet, GameObjectTId);
		OutCommonSet.RemoveAll([FindType](const FGsSchemaSkillCommon* el)
		{
			return el->category != FindType;
		});
	}

	return OutCommonSet.Num();
}

int UGsSkillManager::GetNpcReactionSet(OUT TArray<const FGsSchemaSkillReaction*>& OutReactionSet, int GameObjectTId)
{
	DefObjectKey key(EGsGameObjectType::NonPlayer, GameObjectTId);
	if (const DefNpcReactionMap* findMap = _mapReactionData.Find(key))
	{
		for (TPair<EReactionType, TArray<const FGsSchemaSkillReaction*>> pair : *findMap)
		{
			OutReactionSet.Append(pair.Value);
		}
	}
	else
	{
		LoadReactionDataNpc(OutReactionSet, GameObjectTId);
	}

	return OutReactionSet.Num();
}

int UGsSkillManager::GetNpcReactionSet(OUT TArray<const FGsSchemaSkillReaction*>& OutCommonSet, int GameObjectTId, EReactionType FindType)
{
	if (DefNpcReactionMap* findMap = _mapReactionData.Find(DefObjectKey(EGsGameObjectType::NonPlayer, GameObjectTId)))
	{
		if (TArray<const FGsSchemaSkillReaction*>* findList = findMap->Find(FindType))
		{
			OutCommonSet.Append(*findList);
		}
	}
	else
	{
		GetNpcReactionSet(OutCommonSet, GameObjectTId);
		OutCommonSet.RemoveAll([FindType](const FGsSchemaSkillReaction* el)
		{
			return el->category != FindType;
		});
	}

	return OutCommonSet.Num();
}

int UGsSkillManager::GetPlayerSocialSet(OUT TArray<const FGsSchemaSkillCommon*>& OutCommonSet, UGsGameObjectBase* ObjectClass)
{
	if (nullptr == ObjectClass || false == ObjectClass->IsObjectType(EGsGameObjectType::Player))
	{
		return 0;
	}

	FGsGameObjectDataPlayer* playerData = ObjectClass->GetCastData<FGsGameObjectDataPlayer>();
	CreatureGenderType genderType = playerData->GetCreatureGenderType();

	const UGsTableUserCommonActionData* gsTable = Cast<UGsTableUserCommonActionData>(
		FGsSchemaUserCommonActionData::GetStaticTable());
	const FGsSchemaUserCommonActionData* schema;
	if (gsTable->FindRowById(genderType, schema))
	{
		for (const FGsSchemaSkillCommonRow& el : schema->socialActionInfo)
		{
			OutCommonSet.Emplace(el.GetRow());
		}
	}

	return OutCommonSet.Num();
}

int UGsSkillManager::GetPolymorphShapeReactionSet(OUT TArray<const FGsSchemaSkillReaction*>& OutReactionSet, int GameObjectTId)
{
	DefObjectKey key(EGsGameObjectType::NonPlayer, GameObjectTId);
	if (const DefNpcReactionMap* findMap = _mapReactionData.Find(key))
	{
		for (TPair<EReactionType, TArray<const FGsSchemaSkillReaction*>> pair : *findMap)
		{
			OutReactionSet.Append(pair.Value);
		}
	}
	else
	{
		LoadReactionDataPolymorphShape(OutReactionSet, GameObjectTId);
	}

	return OutReactionSet.Num();
}

//----------------------------------------------------------------------------------------------------------------------
void UGsSkillManager::NetAckSkillSlotSet(SkillSlotInfo& InSlotInfo, CreatureWeaponType InWeaponType)
{
	const FGsSkill* skill = nullptr;
	if (CreatureWeaponType::MAX == InWeaponType)
	{
		skill = FindSkill(InSlotInfo.mActiveSkillId, CreatureWeaponType::MAX, true);
	}
	else
	{
		skill = FindSkill(InSlotInfo.mActiveSkillId, InWeaponType);
	}

	if (nullptr == skill)
	{
		return;
	}

	CreatureWeaponType weaponType = skill->GetSkillRequireWeaponType();

	// INVALID_SKILL_SLOT_ID 이면 지우라고 온 것
	if (INVALID_SKILL_SLOT_ID == InSlotInfo.mSkillSlotId)
	{
		// 해당 스킬이 있던 슬롯을 비움
		SkillSlotId prevSlot = skill->GetSlotId();

		_updateSkillData._data.Set(skill);
		_updateSkillData._data._slotId = INVALID_SKILL_SLOT_ID;
		_updateSkillData._data.SetUseAuto(false);
		PresetId currentPresetId = GGameData()->GetUserData()->mUserPresetId;
		_updateSkillData._data.SetPresetInfo(currentPresetId, INVALID_SKILL_SLOT_ID, false);

		UpdateData(skill);

		// 체인 스킬 정보 업데이트
		_listChainSkillData.RemoveAll([skill](FGsComboSkillGroup& el) {
			return el.Contain(skill);
			});

		SetSkillSlot(weaponType, prevSlot, nullptr, true);
		SetSkillSlotPreset(skill, false);
	}
	else
	{
		_updateSkillData._data.Set(skill);
		_updateSkillData._data._slotId = InSlotInfo.mSkillSlotId;
		_updateSkillData._data.SetUseAuto(InSlotInfo.mAutoUse);
		PresetId currentPresetId = GGameData()->GetUserData()->mUserPresetId;
		_updateSkillData._data.SetPresetInfo(currentPresetId, InSlotInfo.mSkillSlotId, InSlotInfo.mAutoUse);

		UpdateData(skill);

		SetSkillSlot(weaponType, InSlotInfo.mSkillSlotId, skill, true);
		SetSkillSlotPreset(skill, false);
	}
}

void UGsSkillManager::NetAckSkillSlotAuto(PD::SC::PKT_SC_ACK_SKILL_SLOT_AUTO_READ* InPacket)
{
	const FGsSkill* skill = FindSkill(InPacket->ActiveSkillId(), InPacket->WeaponType());
	if (nullptr == skill)
	{
		return;
	}

	_updateSkillData._data.Set(skill);
	_updateSkillData._data.SetUseAuto(InPacket->AutoUse());
	PresetId currentPresetId = GGameData()->GetUserData()->mUserPresetId;
	_updateSkillData._data.SetPresetInfo(currentPresetId, skill->GetSlotId(), InPacket->AutoUse());
	UpdateData(skill);

	FGsGameObjectMessageParamSkillSlot param(InPacket->WeaponType(), skill->GetSlotId(), skill);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_CHANGE_SKILL_SLOT_AUTO, &param);
}

void UGsSkillManager::NetAckSkillEnchant(PD::SC::PKT_SC_ACK_SKILL_ENCHANT_READ* InPacket)
{
	if (SkillEnchantResult::SUCCESS != InPacket->EnchantResult())
	{
		return;
	}

	const FGsSkill* skill = FindSkill(InPacket->ActiveSkillId(), CreatureWeaponType::MAX, true);
	if (nullptr == skill)
	{
		return;
	}

	_updateSkillData._data.Set(skill);
	_updateSkillData._data.SetLevel(InPacket->SkillLevel());
	
	UpdateData(skill);
}

void UGsSkillManager::NetRecvSkillLearn(PD::SC::PKT_SC_SKILL_LEARN_READ* InPacket)
{
	// 연출용으로만 쓰는 것에 유의. isNew만 체크해준다.
	SkillId skillId = InPacket->ActiveSkillId();
	if (0 == skillId)
	{
		skillId = InPacket->RootSkillId();
	}

	const FGsSkill* skill = FindSkill(skillId, CreatureWeaponType::MAX, true);
	if (nullptr == skill)
	{
		return;
	}

	_updateSkillData._data.Set(skill);
	_updateSkillData._data._isNew = true;

	UpdateData(skill);

	// 업그레이드 패시브 스킬일 경우, 연결된 업그레이드 스킬도 New 처리함 
	if (SkillCategorySet::PASSIVE == skill->GetSkillCateogrySet())
	{
		// 업그레이드 패시브 스킬인지 확인해서 업그레이드 스킬도 뉴처리
		TWeakPtr<const FGsSkill> upgradeSkillPtr = FGsSkillHelper::GetUpgradeSkillByPassivitySkill(skill);

		if (upgradeSkillPtr.IsValid())
		{
			if (const FGsSkill* upgradeSkill = upgradeSkillPtr.Pin().Get())
			{
				_updateSkillData._data.Set(upgradeSkill);
				_updateSkillData._data._isNew = true;

				UpdateData(upgradeSkill);
			}
		}
	}

	CheckRedDot(true);
	//_reinforceChecker.CheckRedDot(true);
}

void UGsSkillManager::NetAckSkillSlotClear(PD::SC::PKT_SC_ACK_SKILL_SLOT_CLEAR_READ* InPacket)
{
	DefSkillSlotMap* slotMapPtr = _mapSkillSlotData.Find(InPacket->WeaponType());
	if (nullptr == slotMapPtr)
	{
		return;
	}

	DefSkillSlotMap& slotMap = *slotMapPtr;

	SkillSlotId startSlot = INVALID_SKILL_SLOT_ID;
	SkillSlotId endSlot = INVALID_SKILL_SLOT_ID;
	if (SkillCategorySet::ACTIVE == InPacket->SkillCategorySet())
	{
		startSlot = 1;
		endSlot = MAX_ACTIVE_SKILL_SLOT_ID;
	}
	else if (SkillCategorySet::BUFF == InPacket->SkillCategorySet())
	{
		startSlot = MAX_ACTIVE_SKILL_SLOT_ID + 1;
		endSlot = MAX_BUFF_SKILL_SLOT_ID;
	}

	if (INVALID_SKILL_SLOT_ID == startSlot ||
		INVALID_SKILL_SLOT_ID == endSlot)
	{
		return;
	}

	for (SkillSlotId i = startSlot; i <= endSlot; ++i)
	{
		if (const FGsSkill** skillptr = slotMap.Find(i))
		{
			const FGsSkill* skill = *skillptr;

			_updateSkillData._data.Set(skill);
			_updateSkillData._data._slotId = INVALID_SKILL_SLOT_ID;
			_updateSkillData._data.SetUseAuto(false);
			PresetId currentPresetId = GGameData()->GetUserData()->mUserPresetId;
			_updateSkillData._data.SetPresetInfo(currentPresetId, INVALID_SKILL_SLOT_ID, false);

			UpdateData(skill);
			SetSkillSlotPreset(skill, false);
		}

		// FIX: 별도 메시지를 만들면 최적화를 할 수 있을 것 같다
		SetSkillSlot(InPacket->WeaponType(), i, nullptr, true);
	}
}

void UGsSkillManager::NetUpdateActiveSkill(PD::SC::PKT_SC_UPDATE_ACTIVE_SKILL_READ* InPacket)
{
	// 스킬 업그레이드/다운그레이드 상황
	SkillId removeActiveSkillId = InPacket->RemoveActiveSkillId();
	SkillId addActiveSkillId = InPacket->AddActiveSkillId();

	TMap<PresetId, TPair<SkillSlotId, bool>> mapBackupAuto;

	// 첫 스킬 배울 때는 동일한 SkillId가 오므로 Remove 안함
	if (removeActiveSkillId != addActiveSkillId)
	{
		// 이전 스킬 정보 리셋시킴
		const FGsSkill* removeSkill = FindSkill(removeActiveSkillId, CreatureWeaponType::MAX, true);
		if (removeSkill)
		{
			mapBackupAuto.Append(removeSkill->_presetData);

			_updateSkillData._data.Set(removeSkill);
			_updateSkillData._data._isLearn = false;
			_updateSkillData._data._isNew = false;
			_updateSkillData._data._slotId = INVALID_SKILL_SLOT_ID;
			_updateSkillData._data.SetUseAuto(false);
			
			// 스킬이 지워지는 상황이므로 비워야 한다
			_updateSkillData._data._presetData.Empty();

			_updateSkillData._data.SetLevel(1);
			_updateSkillData._data.SetBonusLevel(0);
			_updateSkillData._data.SetSkillCycle(false, false, 0);

			UpdateData(removeSkill);
		}
	}

	// 새 스킬 할당
	const FGsSkill* addSkill = FindSkill(addActiveSkillId, CreatureWeaponType::MAX, true);
	if (nullptr == addSkill)
	{
		return;
	}

	_updateSkillData._data.Set(addSkill);
	_updateSkillData._data._isLearn = true;
	//_updateSkillData._data._isNew = true; // 배울때만 오는 패킷이 아니므로 New체크하면 안됨(C2URWQ-6689)
	_updateSkillData._data._slotId = InPacket->AddSkillSlotId();
	_updateSkillData._data.SetUseAuto(InPacket->AddAutoUse());
	
	PresetId currentPresetId = GGameData()->GetUserData()->mUserPresetId;
	// 이전 프리셋 오토 정보 계승. 단 현재 프리셋 정보는 안전을 위해 서버에서 준 데이터로 다시 덮어씀
	for (TPair<PresetId, TPair<SkillSlotId, bool>>& itPair : mapBackupAuto)
	{
		_updateSkillData._data.SetPresetInfo(itPair.Key, itPair.Value.Key, itPair.Value.Value);
	}
	_updateSkillData._data.SetPresetInfo(currentPresetId, InPacket->AddSkillSlotId(), InPacket->AddAutoUse());
	
	_updateSkillData._data.SetLevel(InPacket->AddSkillLevel());
	_updateSkillData._data.SetBonusLevel(0);
	_updateSkillData._data.SetSkillCycle(InPacket->IsUseSkillInterval(), InPacket->IsUseSkillIntervalInPVP(), 
		InPacket->UseSkillIntervalSecond());

	// 모든 프리셋에 이전 스킬 쓰는 것 있으면 현재 스킬로 갱신처리
	CreatureWeaponType weaponType = addSkill->GetSkillRequireWeaponType();

	if (DefPresetSkillSlotMap* presetMap = _mapPresetSkillSlotData.Find(weaponType))
	{
		for (TPair<PresetId, DefSkillSlotMap>& itPreset : *presetMap)
		{
			for (TPair<SkillSlotId, const FGsSkill*>& itSlot : itPreset.Value)
			{
				// 같은 스킬을 여러 슬롯에 꽂지 못하므로 바꾼 후 바로 break 해도 됨
				if (itSlot.Value->GetSkillId() == removeActiveSkillId)
				{
					itSlot.Value = addSkill;
					break;
				}
			}
		}
	}

	UpdateData(addSkill);
	
	// 쿨타임 갱신. 순서주의: SetSkillSlot 전에 수행되어야 UI에 쿨타임 반영됨
	GCoolTime()->SetCoolTime(EGsCoolTime::Skill, 0, addActiveSkillId, InPacket->AddCooldown());

	// 슬롯이 세팅되어 왔다면 갱신함
	if (0 < InPacket->AddSkillSlotId())
	{
		SetSkillSlot(addSkill->_tableData->requireWeapon, InPacket->AddSkillSlotId(), addSkill, true);
		SetSkillSlotPreset(addSkill, false);
	}

	// 현재 착용 중인 무기타입이면 목록 갱신(_isLearn이 바뀌었으므로)
	if (addSkill->_tableData->requireWeapon == _currentWeaponType)
	{
		_currentWeaponActiveSkillSet.Empty();
		MakeDataWeaponActiveSkillSet();
	}

	// 레드닷 체크
	CheckRedDot(true);
	_reinforceChecker.CheckRedDot(true);
}

void UGsSkillManager::NetSkillBonusLevelNotify(PD::SC::PKT_SC_SKILL_BONUS_LEVEL_READ* InPacket)
{
	const FGsSkill* skill = FindSkill(InPacket->ActiveSkillId(), CreatureWeaponType::MAX, true);
	if (nullptr == skill)
	{
		return;
	}

	_updateSkillData._data.Set(skill);
	_updateSkillData._data.SetBonusLevel(InPacket->SkillBonusLevel());

	UpdateData(skill);
}

void UGsSkillManager::NetCostumeSkillLearn(SkillId InActiveSkillId)
{
	const FGsSkill* addSkill = FindSkill(InActiveSkillId, CreatureWeaponType::MAX, true);
	if (nullptr == addSkill)
	{
		return;
	}

	_updateSkillData._data.Set(addSkill);
	_updateSkillData._data._isLearn = true;
	_updateSkillData._data._isNew = true;
	_updateSkillData._data._slotId = INVALID_SKILL_SLOT_ID;
	_updateSkillData._data.SetUseAuto(false);
	//PresetId currentPresetId = GGameData()->GetUserData()->mUserPresetId;
	//_updateSkillData._data.SetPresetInfo(currentPresetId, InPacket->AddSkillSlotId(), InPacket->AddAutoUse());
	_updateSkillData._data.SetLevel(1);
	_updateSkillData._data.SetBonusLevel(0);

	UpdateData(addSkill);

	// 현재 착용 중인 무기타입이면 목록 갱신(_isLearn이 바뀌었으므로)
	if (addSkill->_tableData->requireWeapon == _currentWeaponType)
	{
		_currentWeaponActiveSkillSet.Empty();
		MakeDataWeaponActiveSkillSet();
	}

	// 레드닷 체크 - 코스튬 스킬
	CheckRedDot(true);
	_reinforceChecker.CheckRedDot(true);
}

void UGsSkillManager::NetCostumeSkillRemoved(PD::SC::PKT_SC_ACK_CHANGE_COSTUME_READ* pkt)
{
	auto iter = pkt->GetFirstRemoveRootSkillIdListIterator();
	auto lastIter = pkt->GetLastRemoveRootSkillIdListIterator();

	TArray<TPair<SkillId, bool>> removeSkillList;
	for (; iter != lastIter; ++iter)
	{
		bool rootSkill = false;

		SkillId skillId = iter->RemoveRootSkillId();
		if (skillId != INVALID_SKILL_ID) rootSkill = true;
		skillId = skillId == INVALID_SKILL_ID ? iter->RemoveActiveSkillId() : skillId;

		removeSkillList.Add(TPair<SkillId, bool>(skillId, rootSkill));
	}

	if (removeSkillList.Num() > 0)
		RemoveSkillList(removeSkillList);
}

void UGsSkillManager::NetUseSkillCylceAck(PD::SC::PKT_SC_ACK_USE_SKILL_INTERVAL_READ* InPacket)
{
	const FGsSkill* skill = FindSkill(InPacket->ActiveSkillId(), CreatureWeaponType::MAX, true);
	if (nullptr == skill)
	{
		return;
	}

	// 스킬 주기 설정 세팅
	_updateSkillData._data.Set(skill);
	_updateSkillData._data.SetSkillCycle(InPacket->IsUseSkillInterval(), InPacket->IsUseSkillIntervalInPVP(), 
		InPacket->UseSkillIntervalSecond());

	UpdateData(skill);
}

void UGsSkillManager::NextChainSkill(int32 SkillId)
{
	for (FGsComboSkillGroup& el : _listChainSkillData)
	{
		if (el.Contain(SkillId))
		{
			if (const FGsSkill* currSkill = el.GetCurrentData())
			{
				SkillSlotId slotId = currSkill->_slotId;

				// 특정 타입은 리셋 타이머를 처리하지 않음
				bool ignoreTimer = currSkill->_tableData->skillChainCondition == SkillChainCondition::PREV_SKILL_REMAIN;
				if (const FGsSkill* nextSkill = el.Next(ignoreTimer))
				{
					SetSkillSlot(_currentWeaponType, slotId, nextSkill, true);

					// 체인 스킬 변경 알림
					FGsGameObjectMessageParamSkillSlotChain slotParam(_currentWeaponType, slotId, currSkill, nextSkill);
					GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_CHANGE_CHAIN_SKILL, &slotParam);

					// 쿨타임 갱신 알림
					float remain = GCoolTime()->GetCoolTime(EGsCoolTime::Skill, nextSkill->GetSkillId());
					FGsCoolTimeMessageParam messageParam(EGsCoolTime::Skill, 0, 0, remain);
					GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_COOLTIME_START, &messageParam);
				}
				else
				{
					// 다음 스킬이 없다면 리셋
					el.Reset();
				}
			}
		}
	}
}

void UGsSkillManager::NetStartSkill(PD::SC::PKT_SC_START_SKILL_READ* InPacket)
{
	if (_listChainSkillData.Num() > 0)
	{
		if (const FGsSkill* skillData = FindActiveSkill(InPacket->SkillId()))
		{
			// 체인 스킬이 아닌 스킬이 올 경우는 클리어 처리 필요
			// 서버 문의상 대시 스킬도 안됨...
			if (false == skillData->IsChainSkill())
			{
				// 진행중 체인 스킬 확인
				for (FGsComboSkillGroup& el : _listChainSkillData)
				{
					if (false == el.IsFirst() && false == el.IsRemainSkill())
					{
						el.Reset();
					}
				}
			}
			else if (SkillChainCondition::PREV_SKILL == skillData->_tableData->skillChainCondition ||
				SkillChainCondition::PREV_SKILL_REMAIN == skillData->_tableData->skillChainCondition)
			{
				NextChainSkill(InPacket->SkillId());
			}
		}
	}
}

void UGsSkillManager::NetSkillFinished(PD::SC::PKT_SC_SKILL_FINISHED_READ* InPacket)
{

}

void UGsSkillManager::NetSkillFailure(PD::SC::PKT_SC_ACK_START_SKILL_FAILURE_READ* InPacket)
{

}

void UGsSkillManager::NetSkillDamaged(PD::SC::PKT_SC_SKILL_DAMAGED_READ* InPacket)
{
	if (_listChainSkillData.Num() > 0 && InPacket->HitResultType() == HitResultType::NORMAL)
	{
		const FGsComboSkillGroup* skillgroup = FindChainSkillGroup(InPacket->SkillId());
		if (nullptr == skillgroup)
		{
			return;
		}

		const FGsSkill* currCainSkill = skillgroup->GetCurrentData();
		if (SkillChainCondition::HIT_PREV_SKILL != currCainSkill->_tableData->skillChainCondition)
		{
			return;
		}

		if (currCainSkill->GetSkillId() == InPacket->SkillId())
		{
			NextChainSkill(InPacket->SkillId());
		}
	}
}

const UGsSkillManager::DefSkillSlotMap* UGsSkillManager::GetSkillSlotMap(CreatureWeaponType InWeapon) const
{
	return _mapSkillSlotData.Find(InWeapon);;
}

const UGsSkillManager::DefSkillSlotMap* UGsSkillManager::GetCurrentSkillSlotMap() const
{
	return GetSkillSlotMap(_currentWeaponType);
}

const FGsSkill* UGsSkillManager::FindNormalSkill(CreatureWeaponType InWeapon) const
{
	const DefSkillMap* skillMap = _mapSkillData.Find(InWeapon);
	if (skillMap)
	{
		for (TPair<int, TSharedPtr<const FGsSkill>> iter : *skillMap)
		{
			const FGsSkill* skill = iter.Value.Get();
			
			// isNormal이어도 콤보스킬이 여러개가 있으므로, 첫 스킬을 찾아낸다
			if (skill->IsFirstNormalSkill())
			{
				return skill;
			}
		}
	}

	return nullptr;
}

const UGsSkillManager::DefSkillSlotMap* UGsSkillManager::GetSkillSlotMapByPresetId(CreatureWeaponType InWeapon, PresetId InPresetId) const
{
	if (_mapPresetSkillSlotData.Contains(InWeapon))
	{
		return _mapPresetSkillSlotData[InWeapon].Find(InPresetId);
	}

	return nullptr;
}

void UGsSkillManager::SetSkillSlot(CreatureWeaponType InWeapon, SkillSlotId InCurrSlotId, const FGsSkill* InSkill, 
	 bool bInUpdate)
{
	// 슬롯 비우기(nullptr) 처리 시
	if (nullptr == InSkill)
	{
		// 평타 슬롯은 비우지 않음
		if (INVALID_SKILL_SLOT_ID != InCurrSlotId)
		{
			if (DefSkillSlotMap* slotMapPtr = _mapSkillSlotData.Find(InWeapon))
			{
				if (slotMapPtr->Contains(InCurrSlotId))
				{
					slotMapPtr->Remove(InCurrSlotId);
				}			
			}

			if (bInUpdate)
			{
				FGsGameObjectMessageParamSkillSlot param(InWeapon, InCurrSlotId, nullptr);
				GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_CHANGE_SKILL_SLOT, &param);

				FGsGameObjectMessageParamSkillSlot paramEquip(InWeapon, InCurrSlotId, nullptr);
				GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_CHANGE_SKILL_SLOT_EQUIP, &paramEquip);
			}
		}

		return;
	}

	// 평타 슬롯에 대한 처리
	// 주의: 0번(INVALID_SKILL_SLOT_ID)은 서버에선 에러값으로 쓰지만, 클라에서 평타 스킬을 할당하여 활용
	if (INVALID_SKILL_SLOT_ID == InCurrSlotId)
	{
		// 평타인지 확인
		if (InSkill->IsFirstNormalSkill())
		{
			if (DefSkillSlotMap* slotMapPtr = _mapSkillSlotData.Find(InWeapon))
			{
				// 평타는 처음 서버에서 받아서 세팅 후 변하지 않으므로 없을때만 넣고 덮어쓰지는 않음
				if (false == slotMapPtr->Contains(INVALID_SKILL_SLOT_ID))				
				{
					slotMapPtr->Emplace(INVALID_SKILL_SLOT_ID, InSkill);
				}
			}
			else
			{
				DefSkillSlotMap newSlotMap;
				newSlotMap.Emplace(INVALID_SKILL_SLOT_ID, InSkill);
				_mapSkillSlotData.Emplace(InWeapon, newSlotMap);
			}

			if (bInUpdate)
			{
				FGsGameObjectMessageParamSkillSlot param(InWeapon, INVALID_SKILL_SLOT_ID, InSkill);
				GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_CHANGE_SKILL_SLOT, &param);

				FGsGameObjectMessageParamSkillSlot paramEquip(InWeapon, INVALID_SKILL_SLOT_ID, InSkill);
				GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_CHANGE_SKILL_SLOT_EQUIP, &paramEquip);
			}
		}

		return;
	}

	// 그 외의 일반 스킬에 대한 처리
	if (DefSkillSlotMap* slotMapPtr = _mapSkillSlotData.Find(InWeapon))
	{
		DefSkillSlotMap& slotMap = *slotMapPtr;

		// 이전에 꽂혀있던 슬롯이 있는지 확인
		SkillSlotId prevSlotId = 0;
		for (TPair<SkillSlotId, const FGsSkill*>& iter : slotMap)
		{
			if (iter.Value &&
				iter.Value->GetSkillId() == InSkill->GetSkillId())
			{
				prevSlotId = iter.Key;
				break;
			}
		}

		// 이전에 꽂혀있던 슬롯을 비움
		if (0 < prevSlotId)
		{
			slotMap.Remove(prevSlotId);

			if (bInUpdate)
			{
				FGsGameObjectMessageParamSkillSlot param(InWeapon, prevSlotId, nullptr);
				GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_CHANGE_SKILL_SLOT, &param);

				FGsGameObjectMessageParamSkillSlot paramEquip(InWeapon, prevSlotId, nullptr);
				GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_CHANGE_SKILL_SLOT_EQUIP, &paramEquip);
			}
		}

		// 새 슬롯에 할당
		if (slotMap.Contains(InCurrSlotId))
		{
			slotMap[InCurrSlotId] = InSkill;
		}
		else
		{
			slotMap.Emplace(InCurrSlotId, InSkill);
		}
	}
	else
	{
		DefSkillSlotMap newSlotMap;
		newSlotMap.Emplace(InCurrSlotId, InSkill);
		_mapSkillSlotData.Emplace(InWeapon, newSlotMap);
	}

	SetChainSkill(InSkill);

	UpdateSkillSlotData(EGsSkillSlotCheckType::MAX, InCurrSlotId);

	if (bInUpdate)
	{
		FGsGameObjectMessageParamSkillSlot param(InWeapon, InCurrSlotId, InSkill);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_CHANGE_SKILL_SLOT, &param);

		FGsGameObjectMessageParamSkillSlot paramEquip(InWeapon, InCurrSlotId, InSkill);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_CHANGE_SKILL_SLOT_EQUIP, &paramEquip);
	}
}

void UGsSkillManager::SetSkillSlotPreset(const FGsSkill* InSkill, bool bInUpdate /*= false*/)
{
	if (nullptr == InSkill)
	{
		return;
	}

	CreatureWeaponType weaponType = InSkill->GetSkillRequireWeaponType();
	for (const auto& presetData : InSkill->_presetData)
	{
		if (InSkill->IsFirstNormalSkill())
			continue;
		
		if (DefPresetSkillSlotMap* presetSlotMapPtr = _mapPresetSkillSlotData.Find(weaponType))
		{
			DefPresetSkillSlotMap& presetSlotMap = *presetSlotMapPtr;
			if (DefSkillSlotMap* slotMapPtr = presetSlotMap.Find(presetData.Key))
			{
				DefSkillSlotMap& slotMap = *slotMapPtr;

				// 이전에 꽂혀있던 슬롯이 있는지 확인
				SkillSlotId prevSlotId = 0;
				for (TPair<SkillSlotId, const FGsSkill*>& iter : slotMap)
				{
					if (iter.Value &&
						iter.Value->GetSkillId() == InSkill->GetSkillId())
					{
						prevSlotId = iter.Key;
						break;
					}
				}

				// 이전에 꽂혀있던 슬롯을 비움
				if (0 < prevSlotId)
				{
					slotMap.Remove(prevSlotId);
				}

				TPair<SkillSlotId, bool> slotData = presetData.Value;
				// 평타 or 잘못된 데이터 or 삭제된 슬롯
				if (INVALID_SKILL_SLOT_ID == slotData.Key)
					continue;

				// 새 슬롯에 할당
				if (slotMap.Contains(slotData.Key))
				{
					slotMap[slotData.Key] = InSkill;
				}
				else
				{
					slotMap.Emplace(slotData.Key, InSkill);
				}
			}
			else
			{
				TPair<SkillSlotId, bool> slotData = presetData.Value;
				// 평타 or 잘못된 데이터 or 삭제된 슬롯
				if (INVALID_SKILL_SLOT_ID == slotData.Key)
					continue;

				DefSkillSlotMap newSlotMap;
				newSlotMap.Emplace(slotData.Key, InSkill);
				presetSlotMap.Emplace(presetData.Key, newSlotMap);
			}

		}
	}

	GMessage()->GetUI().SendMessage(MessageUI::INVALIDATE_PRESET, nullptr);
}

void UGsSkillManager::RestoreChainSkill(const FGsComboSkillGroup& ComboSkillGroup)
{
	const FGsSkill* firstSkillData = ComboSkillGroup.GetFirstSkill();
	if (nullptr == firstSkillData)
	{
		return;
	}

	// 0번은 제외. 서버에서 슬롯없음 기본값으로 쓰지만 클라에선 평타슬롯으로 쓰고있다
	if (INVALID_SKILL_SLOT_ID == firstSkillData->GetSlotId())
	{
		return;
	}

	if (DefSkillSlotMap* findSlotData = _mapSkillSlotData.Find(firstSkillData->_tableData->requireWeapon))
	{
		if (findSlotData->Contains(firstSkillData->GetSlotId()))
		{
			(*findSlotData)[firstSkillData->GetSlotId()] = firstSkillData;
		}
	}
}

void UGsSkillManager::RemoveSkillList(TArray<TPair<SkillId, bool>>& removeSkillList)
{
	for (auto& pair : removeSkillList)
	{
		if (const auto skill = FindSkill(pair.Key, CreatureWeaponType::MAX, true))
		{
			CreatureWeaponType weaponType = skill->GetSkillRequireWeaponType();

			for (auto& weaponPair : _mapSkillData)
			{
				if (auto curWeaponSkill = weaponPair.Value.Find(pair.Key))
				{
					SkillSlotInfo info;
					info.mActiveSkillId = pair.Key;
					info.mRootSkillId = pair.Value ? pair.Key : INVALID_SKILL_ID;
					info.mAutoUse = false;
					info.mSkillSlotId = INVALID_SKILL_SLOT_ID;
					NetAckSkillSlotSet(info, CreatureWeaponType::MAX);
				}
			}

			//NetAckSkillSlotSet 보다 먼저 하면 _updateSkillData 의 내용이 바뀐다 순서 주의
			_updateSkillData._data.Set(*skill);
			_updateSkillData._data._isLearn = false;
			_updateSkillData._data._slotId = INVALID_SKILL_SLOT_ID;
			_updateSkillData._data._isNew = false;
			_updateSkillData._data.SetUseAuto(false);
			_updateSkillData._data.SetLevel(1);
			_updateSkillData._data.SetBonusLevel(0);
			_updateSkillData._data.SetSkillCycle(false, false, 0);
			
			if (DefPresetSkillSlotMap* presetSlotMapPtr = _mapPresetSkillSlotData.Find(weaponType))
			{
				for (auto& presetMap : *presetSlotMapPtr)
				{
					while (auto useSkillIter = Algo::FindByPredicate(presetMap.Value, [skill](auto& pair) { return pair.Value == skill; }))
					{
						_updateSkillData._data.SetPresetInfo(presetMap.Key, INVALID_SKILL_SLOT_ID, false);
						presetMap.Value.Remove(useSkillIter->Key);
					}
				}
			}

			UpdateData(skill);
			CheckRedDot(true);
		}
	}

}

void UGsSkillManager::SetChainSkill(const FGsSkill* InSkill)
{
	if (nullptr == InSkill)
	{
		return;
	}

	// 처음에 해당되는 체인 스킬만 처리
	if (false == InSkill->IsFirstChainSkill())
	{
		return;
	}

	// 이미 등록되어있으면 슬롯정보만 갱신해 준다.
	FGsComboSkillGroup* skillGroup = nullptr;
	for (FGsComboSkillGroup& el : _listChainSkillData)
	{
		if (el.Contain(InSkill))
		{
			skillGroup = &el;
		}
	}

	// 새로운 그룹 설정
	if (nullptr == skillGroup)
	{
		FGsComboSkillGroup newGroup;
		const FGsSkill* currSkill = InSkill;
		for (int i = 0; i < currSkill->_tableData->sequenceInfo.maxStage; ++i)
		{
			newGroup.Add(currSkill);

			int nextComboSkillId = currSkill->_tableData->sequenceInfo.comboInfo.nextComboSkillId;
			currSkill = FindSkill(nextComboSkillId, currSkill->_tableData->requireWeapon);
			if (nullptr == currSkill)
			{
				GSLOG(Error, TEXT("not Found nextComboSkillId %d "), nextComboSkillId);
				break;
			}
		}
		skillGroup = &_listChainSkillData.Emplace_GetRef(newGroup);
	}

	// Slot 정보 갱신
	if (skillGroup)
	{
		skillGroup->Init([this](FGsComboSkillGroup* GroupSkill) { CallbackChainSkillEnd(GroupSkill); });

		SkillSlotId slotId = InSkill->_slotId;
		for (const FGsSkill* el : skillGroup->GetAllData())
		{
			_updateSkillData._data.Set(el);
			_updateSkillData._data._slotId = slotId;
			UpdateData(el);
		}
	}
}

bool UGsSkillManager::CheckRedDot(bool bInUpdate)
{
	if (bInUpdate)
	{
		// 스킬 전부 돌면서 
		for (TPair<CreatureWeaponType, DefSkillMap>& itPairWeapon : _mapSkillData)
		{
			if (CreatureWeaponType::HAND == itPairWeapon.Key ||
				CreatureWeaponType::NONE == itPairWeapon.Key)
			{
				continue;
			}

			for (TPair<int, TSharedPtr<const FGsSkill>>& itPairSkill : itPairWeapon.Value)
			{
				if (FGsSkillHelper::CheckSkillRedDot(itPairSkill.Value.Get()))
				{
					SetRedDot(true);
					return true;
				}				
			}
		}

		SetRedDot(false);
		return false;
	}

	return IsRedDot();
}

void UGsSkillManager::SetIsNew(const FGsSkill* InSkill, bool bIsNew)
{
	_updateSkillData._data.Set(InSkill);
	_updateSkillData._data._isNew = bIsNew;

	UpdateData(InSkill);

	CheckRedDot(true);
}

bool UGsSkillManager::CheckReinforce()
{
#if USE_SKILL_ENCHANT
	for (TPair<CreatureWeaponType, DefSkillMap>& itPairWeapon : _mapSkillData)
	{
		if (CreatureWeaponType::HAND == itPairWeapon.Key ||
			CreatureWeaponType::NONE == itPairWeapon.Key)
		{
			continue;
		}

		for (TPair<int, TSharedPtr<const FGsSkill>>& itPairSkill : itPairWeapon.Value)
		{
			if (FGsSkillHelper::CanReinforce(itPairSkill.Value.Get(), false))
			{
				return true;
			}
		}
	}
#endif

	return false;
}

bool UGsSkillManager::IsReinforce() const
{
#if USE_SKILL_ENCHANT
	return _reinforceChecker.IsRedDot();
#else
	return false;
#endif
}

bool UGsSkillManager::IsAlreadyLearnedSkill(SkillId InSkillId) const
{
	const FGsSkill* skill = FindSkill(InSkillId, CreatureWeaponType::MAX, true);
	if (skill)
	{
		return skill->_isLearn;
	}

	return false;
}

const FGsSchemaIdleEmotionActionData* UGsSkillManager::GetIdleEmotionActionData(UGsGameObjectBase* ObjectClass)
{
	//if (ObjectClass && ObjectClass->IsObjectType(EGsGameObjectType::Player) && !ObjectClass->IsObjectType(EGsGameObjectType::LocalPlayer))
	//	return nullptr;

	return ObjectClass && ObjectClass->IsObjectType(EGsGameObjectType::Player) ? 
		GetPlayerIdleEmotionActionData(ObjectClass) : GetNpcIdleEmotionActionData(ObjectClass);
}

const FGsSchemaIdleEmotionActionData* UGsSkillManager::GetPlayerIdleEmotionActionData(UGsGameObjectBase* ObjectClass)
{
	if (FGsGameObjectDataPlayer* playerData = ObjectClass ? ObjectClass->GetCastData<FGsGameObjectDataPlayer>() : nullptr)
	{
		CreatureGenderType genderType = playerData->GetCreatureGenderType();

		const FGsSchemaUserCommonActionData* genderActionData = nullptr;

		const auto commonActTable = Cast<UGsTableUserCommonActionData>(FGsSchemaUserCommonActionData::GetStaticTable());
		if (commonActTable && commonActTable->FindRowById(genderType, genderActionData))
		{
			if(genderActionData->emotionActionInfo.motionList.Num() > 0)
				return &(genderActionData->emotionActionInfo);
		}
	}
	return nullptr;
}

const FGsSchemaIdleEmotionActionData* UGsSkillManager::GetNpcIdleEmotionActionData(UGsGameObjectBase* ObjectClass)
{
	auto npcId = ObjectClass && ObjectClass->GetData() ? ObjectClass->GetData()->GetTableId() : 0;

	if (const auto npcData = UGsTableUtil::FindRowById<UGsTableNpcData, FGsSchemaNpcData>(npcId))
	{
		if (const auto* commonActionData = npcData->commonActionListId.GetRow())
		{
			if(commonActionData->emotionActionInfo.motionList.Num() > 0)
				return &(commonActionData->emotionActionInfo);
		}
	}
	return nullptr;
}

void UGsSkillManager::SetIsShowSkillTargetSelect(bool In_isShowSkillTargetSelect, int In_slotId)
{
	bool oldShow = _isShowSkillTargetSelect;
	_isShowSkillTargetSelect = In_isShowSkillTargetSelect;

	int oldSkillTargetSlotId = _currentSkillTargetSlotId;
	if (In_slotId != -1)
	{
		_currentSkillTargetSlotId = In_slotId;
	}

	// check show changed
	if (oldShow != In_isShowSkillTargetSelect)
	{
		GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_SKILLTARGETSELECT);

		if (In_isShowSkillTargetSelect == false)
		{
			int slotId = GetCurrentSkillTargetSlotId();
			FGsUIMsgParamInt param(slotId);
			GMessage()->GetUI().SendMessage(MessageUI::REQUEST_UPDATE_SKILL_SLOT, &param);
		}
	}
	//https://jira.com2us.com/jira/browse/CHR-15477
	// already showed targetSelect, changed slot
	if (oldShow == true && In_isShowSkillTargetSelect == true)
	{
		FGsUIMsgParamInt param(oldSkillTargetSlotId);
		GMessage()->GetUI().SendMessage(MessageUI::REQUEST_UPDATE_SKILL_SLOT, &param);
	}
}