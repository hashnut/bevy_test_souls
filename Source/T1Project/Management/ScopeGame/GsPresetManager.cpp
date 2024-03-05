// Fill out your copyright notice in the Description page of Project Settings.


#include "GsPresetManager.h"

#include "DataSchema/Preset/GsSchemaPresetConfig.h"

#include "Data/GsDataContainManager.h"

#include "Management/ScopeGame/GsCoolTimeManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "Management/ScopeGame/GsCostumeManager.h"
#include "Management/ScopeGame/GsFairyManager.h"
#include "Management/ScopeGame/GsCoolTimeManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"

#include "Item/GsItemManager.h"
#include "Item/GsItemEquipment.h"

#include "Net/GsNetSendServiceWorld.h"

#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsMessageParam.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"
#include "GameObject/State/GsGameObjectStateManager.h"


void FGsPresetManager::Initialize()
{
	if (const UGsTablePresetConfig* table = Cast<UGsTablePresetConfig>(FGsSchemaPresetConfig::GetStaticTable()))
	{
		TArray<const FGsSchemaPresetConfig*> rowList;
		if (table->GetAllRows(rowList))
		{
			for (const FGsSchemaPresetConfig* row : rowList)
			{
				_cachedChangeCoolTime = row->presetSlotChangeCoolTime;
				break;
			}
		}
	}

	_presetNameMap.Empty();
	for (int32 presetId = MIN_PRESET_ID; presetId <= MAX_PRESET_ID;++presetId)
	{
		SetPresetName(presetId, FString());
	}

	_gameObjectMsgHandlerList.Emplace(GMessage()->GetGameObject().AddRaw(MessageGameObject::LOCAL_REACTION, 
		this, &FGsPresetManager::OnLocalReaction));
}

void FGsPresetManager::Finalize()
{
	_presetNameMap.Empty();

	FGsMessageHolder* message = GMessage();
	for (MsgGameObjHandle& iter : _gameObjectMsgHandlerList)
	{
		message->GetGameObject().Remove(iter);
	}
	_gameObjectMsgHandlerList.Empty();
}

EGsPresetDimmedState FGsPresetManager::GetDimmedState()
{
	if (false == GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::PRESET))
	{
		return EGsPresetDimmedState::CONTENTS_LOCKED;
	}

	// 쿨타임 확인
	if (GCoolTime()->IsCoolTime(EGsCoolTime::Preset, 0, 0))
	{
		return EGsPresetDimmedState::COOLTIME;
	}

	// 행동제약 확인
	if (UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		if (local->IsCrowdControlLock(CreatureActionType::CREATURE_ACTION_TYPE_ITEM))
		{
			return EGsPresetDimmedState::CROWD_CONTROL;
		}
	}

	return EGsPresetDimmedState::NONE;
}

EGsPresetDimmedState FGsPresetManager::GetDimmedState(PresetId InPresetId)
{
	UGsItemManager* itemManager = GItem();
	if (nullptr == itemManager)
		return EGsPresetDimmedState::ERROR;

	FGsCoolTimeManager* coolTimeManager = GCoolTime();
	if (nullptr == coolTimeManager)
		return EGsPresetDimmedState::ERROR;

	EGsUnlockContentsType unlockContentsType = GetUnlockContentsType(InPresetId);
	if (false == GSUnlock()->IsContentsUnlock(unlockContentsType))
	{
		return EGsPresetDimmedState::CONTENTS_LOCKED;
	}
	
	// 무기 종류 비교 후 무기 변경 쿨타임 확인
	PresetId currentPresetId = GetCurrentPresetId();
	if (currentPresetId != InPresetId && coolTimeManager->IsCoolTime(EGsCoolTime::Change_Weapon, 0, 0))
	{
		ItemDBId currentWeaponDBId = itemManager->GetEquipedItemDBIDBySlotType(currentPresetId, EGsEquipSlotType::WEAPON_TYPE);
		ItemDBId compareWeaponDBId = itemManager->GetEquipedItemDBIDBySlotType(InPresetId, EGsEquipSlotType::WEAPON_TYPE);

		CreatureWeaponType currentWeaponType = CreatureWeaponType::HAND;
		CreatureWeaponType compareWeaponType = CreatureWeaponType::HAND;

		// 맨손일 경우
		if (currentWeaponDBId == INVALID_ITEM_DB_ID)
		{
			currentWeaponType = itemManager->GetLoginCreatureWeaponType();
		}
		// 무기 장착중이며, 다른 프리셋의 무기와 다른 경우
		else if (currentWeaponDBId != compareWeaponDBId)
		{
			TWeakPtr<FGsItem> currentWeapon = itemManager->FindItem(currentWeaponDBId, ItemType::EQUIP);
			if (currentWeapon.IsValid())
			{
				if (FGsItemEquipment* currentWeaponEquip = currentWeapon.Pin()->GetCastItem<FGsItemEquipment>())
				{
					currentWeaponType = currentWeaponEquip->GetCreatureWeaponType();
				}
			}
		}

		TWeakPtr<FGsItem> compareWeapon = itemManager->FindItem(compareWeaponDBId, ItemType::EQUIP);
		if (compareWeapon.IsValid())
		{
			if (FGsItemEquipment* compareWeaponEquip = compareWeapon.Pin()->GetCastItem<FGsItemEquipment>())
			{
				compareWeaponType = compareWeaponEquip->GetCreatureWeaponType();
			}
		}

		if (currentWeaponType != CreatureWeaponType::HAND && 
			compareWeaponType != CreatureWeaponType::HAND &&
			currentWeaponType != compareWeaponType)
		{
			return EGsPresetDimmedState::COOLTIME_WEAPON;
		}
	}

	return GetDimmedState();
}

EGsUnlockContentsType FGsPresetManager::GetUnlockContentsType(PresetId InPresetId)
{
	switch (InPresetId)
	{
	case 1:
		return EGsUnlockContentsType::PRESET_FIRST;
	case 2:
		return EGsUnlockContentsType::PRESET_SECOND;
	case 3:
		return EGsUnlockContentsType::PRESET_THIRD;

	default:
		return EGsUnlockContentsType::PRESET;
	}

	return EGsUnlockContentsType::PRESET;
}

PresetId FGsPresetManager::GetCurrentPresetId()
{
	if (const FGsNetUserData* userData = GGameData()->GetUserData())
	{
		return userData->mUserPresetId;
	}

	return INVALID_PRESET_ID;
}

CostumeId FGsPresetManager::GetCostumeId(PresetId InPresetId)
{
	return GSCostume()->GetCostumeId(InPresetId);
}

FairyId FGsPresetManager::GetFairyId(PresetId InPresetId)
{
	return GSFairy()->GetFairyId(InPresetId);
}

void FGsPresetManager::ReqUserPresetChange(PresetId InPresetId)
{
	UGsGameObjectBase* player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (nullptr == player)
		return;

	UGsGameObjectLocalPlayer* localPlayer = player->CastGameObject<UGsGameObjectLocalPlayer>();
	if (nullptr == localPlayer)
		return;

	FGsGameObjectStateManager* fsm = localPlayer->GetBaseFSM();
	if (nullptr == fsm)
		return;
	
	// 워프 진행 중에는 프리셋 변경을 막는다.
	if (fsm->IsState(EGsStateBase::Warp))
		return;

	FGsNetSendServiceWorld::SendReqUserPresetChange(InPresetId);
	_requestUserPresetChange = true;
}

void FGsPresetManager::AckUserPresetChange()
{
	_requestUserPresetChange = false;
}

float FGsPresetManager::GetRemainCoolTimeRate()
{
	float remainCoolTime = GCoolTime()->GetCoolTime(EGsCoolTime::Preset, 0, 0);
	if (0.f >= remainCoolTime || _cachedChangeCoolTime == 0.f)
	{
		return 0.f;
	}

	if (remainCoolTime >= _cachedChangeCoolTime)
	{
		return 1.f;
	}

	return remainCoolTime / _cachedChangeCoolTime;
}

void FGsPresetManager::SetPresetName(PresetId InPresetId, const FString& InPresetName)
{
	FString presetName = InPresetName;
	if (InPresetName.IsEmpty())
	{
		FString nameTextKey = TEXT("Preset_Slot_");
		FText presetText;
		FText::FindText(TEXT("PresetText"), nameTextKey + FString::FromInt(InPresetId), presetText);
		presetName = presetText.ToString();
	}

	if (_presetNameMap.Contains(InPresetId))
	{
		_presetNameMap[InPresetId] = presetName;
	}
	else
	{
		_presetNameMap.Emplace(InPresetId, presetName);
	}
}

FString FGsPresetManager::GetPresetName(PresetId InPresetId)
{
	if (_presetNameMap.Contains(InPresetId))
	{
		return _presetNameMap[InPresetId];
	}
	return FString();
}

void FGsPresetManager::ProcessNetError(int32 InResult)
{
	switch (InResult)
	{
	case PD::Result::CHANGE_WEAPON_TYPE_ERROR_COOLDOWN_TIME:
	case PD::Result::PRESET_ERROR_COOLDOWN_TIME:
	{
		// 아직 무기를 변경할 수 없습니다.
		FText findText;
		FText::FindText(TEXT("ItemUIText"), TEXT("Notice_WeaponChangeCooltime"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return;
	}
	default:
		break;
	}

	FGsUIHelper::PopupNetError(static_cast<PD::Result>(InResult));
}

void FGsPresetManager::OnLocalReaction(const IGsMessageParam* Param)
{
	if (UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		if (local->IsCrowdControlLock(CreatureActionType::CREATURE_ACTION_TYPE_MOVE_DEFAULT))
		{
			if (FGsMovementHandlerLocalPlayer* moveHandler = local->CastGameObject<UGsGameObjectLocalPlayer>()->GetCastMovement<FGsMovementHandlerLocalPlayer>())
			{
				moveHandler->SpectatorEnd();
			}
		}
	}
	GMessage()->GetUI().SendMessage(MessageUI::INVALIDATE_PRESET, nullptr);
}

bool FGsPresetManager::IsActivePreset()
{
	if (UGsDataContainManager* dataManager = GData())
	{
		if (UGsGlobalConstant* globalData = dataManager->GetGlobalData())
		{
			return globalData->_showPreset;
		}
	}
	return false;
}
