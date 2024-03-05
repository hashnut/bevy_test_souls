// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIHUDSkill.h"
#include "UI/UIContent/HUD/Skill/GsUISkillSlotSetHUD.h"
#include "UI/UIContent/HUD/Skill/GsUISkillButtonNormal.h"
#include "UI/UIContent/HUD/Skill/GsUISkillButtonActive.h"
#include "UI/UIContent/HUD/NPCFunction/GsUINPCFunctionSlotSetHUD.h"
#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGame/GsAIManager.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Skill/GsSkillUIHandler.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/MessageParam/GsGameObjectMessageParam.h"
#include "Message/MessageParam/GsCoolTimeMessageParam.h"
#include "Skill/GsSkill.h"
#include "UMG/Public/Animation/WidgetAnimation.h"
#include "Input/GsInputEventMsgBase.h"
#include "GameObject/Fence/GsFenceHandler.h"
#include "Shared/Client/SharedEnums/SharedAbnormalityEnum.h"
#include "Runtime/DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"


void UGsUIHUDSkill::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnNormal->InitializeSlot(0);
}

void UGsUIHUDSkill::NativeConstruct()
{
	Super::NativeConstruct();

	BindMessages();
}

void UGsUIHUDSkill::NativeDestruct()
{
	UnbindMessages();

	UGsBlueprintFunctionLibraryUI::StopAllAndClearAnimation(this);

	Super::NativeDestruct();
}

void UGsUIHUDSkill::InvalidateAllInternal()
{
	UWidgetAnimation* playAnimation;
	//플레이어가 마을 펜스 안에 있다면
	if (GFence()->IsPlayerInTownFence())
	{
		if (FGsSkillUIHandler::isSkillSlotSet == true)
		{
			playAnimation = _aniSkillToNPC;
		}
		else
		{
			playAnimation = _aniNPCIdle;
		}

		FGsSkillUIHandler::isSkillSlotSet = false;
		UpdateAllNPCFunctionSlots();
	}
	else
	{
		if (FGsSkillUIHandler::isSkillSlotSet == true)
		{
			playAnimation = _aniSkillIdle;
		}
		else
		{
			playAnimation = _aniNPCToSkill;
		}
		FGsSkillUIHandler::isSkillSlotSet = true;
		UpdateAllSkillSlots();
	}

	UGsBlueprintFunctionLibraryUI::StopAllAndClearAnimation(this);
	PlayAnimation(playAnimation);
}

bool UGsUIHUDSkill::UpdateManagerTick(float InDeltaTime)
{
	if (false == Super::UpdateManagerTick(InDeltaTime))
	{
		return false;
	}

	_btnNormal->UpdateManagerTick(InDeltaTime);
	_skillSlotSet->UpdateManagerTick(InDeltaTime);

	return true;
}

void UGsUIHUDSkill::BindMessages()
{
	MUI& msgUI = GMessage()->GetUI();
	_msgUIHandleList.Emplace(msgUI.AddUObject(MessageUI::RESERVE_SKILL_SLOT,
		this, &UGsUIHUDSkill::OnReserveSkillSlot));
	_msgUIHandleList.Emplace(msgUI.AddUObject(MessageUI::CANCEL_RESERVE_SKILL_SLOT,
		this, &UGsUIHUDSkill::OnCancelReserveSkillSlot));
	_msgUIHandleList.Emplace(msgUI.AddUObject(MessageUI::REQUEST_UPDATE_SKILL_SLOT,
		this, &UGsUIHUDSkill::OnRequestSlotUpdate));

	MGameObject& msgGameObject = GMessage()->GetGameObject();
	_msgGameObjectHandleList.Emplace(msgGameObject.AddUObject(MessageGameObject::LOCAL_CHANGE_SKILL_SET,
		this, &UGsUIHUDSkill::OnChangeSkillSet));
	_msgGameObjectHandleList.Emplace(msgGameObject.AddUObject(MessageGameObject::LOCAL_CHANGE_SKILL_SLOT,
		this, &UGsUIHUDSkill::OnChangeSkillSlot));
	_msgGameObjectHandleList.Emplace(msgGameObject.AddUObject(MessageGameObject::LOCAL_CHANGE_SKILL_SLOT_AUTO,
		this, &UGsUIHUDSkill::OnChangeSkillAuto));
	_msgGameObjectHandleList.Emplace(msgGameObject.AddUObject(MessageGameObject::LOCAL_CHANGE_CHAIN_SKILL,
		this, &UGsUIHUDSkill::OnChangeChainSkill));
	_msgGameObjectHandleList.Emplace(msgGameObject.AddUObject(MessageGameObject::LOCAL_CHANGE_BUFFSKILL_SLOT,
		this, &UGsUIHUDSkill::OnSetStartBuffSkillSlot));
	_msgGameObjectHandleList.Emplace(msgGameObject.AddUObject(MessageGameObject::LOCAL_COOLTIME_START,
		this, &UGsUIHUDSkill::OnSkillCoolDown));
	_msgGameObjectHandleList.Emplace(msgGameObject.AddUObject(MessageGameObject::LOCAL_SKILL_START_FOR_EFFECT,
		this, &UGsUIHUDSkill::OnSkillStart));
	_msgGameObjectHandleList.Emplace(msgGameObject.AddUObject(MessageGameObject::LOCAL_ABNORMALITY_ADD,
		this, &UGsUIHUDSkill::OnAddAbnormality));
	_msgGameObjectHandleList.Emplace(msgGameObject.AddUObject(MessageGameObject::LOCAL_ABNORMALITY_REMOVE,
		this, &UGsUIHUDSkill::OnRemoveAbnormality));
}

void UGsUIHUDSkill::UnbindMessages()
{
	FGsMessageHolder* msgMgr = GMessage();
	if (nullptr == msgMgr)
	{
		return;
	}

	MUI& msgUI = msgMgr->GetUI();
	for (MsgUIHandle& msgHandle : _msgUIHandleList)
	{
		msgUI.Remove(msgHandle);
	}
	_msgUIHandleList.Empty();

	MGameObject& msgGameObject = msgMgr->GetGameObject();
	for (MsgGameObjHandle& msgHandle : _msgGameObjectHandleList)
	{
		msgGameObject.Remove(msgHandle);
	}
	_msgGameObjectHandleList.Empty();
}

void UGsUIHUDSkill::OnReserveSkillSlot(const IGsMessageParam* InParam)
{
	const FGsUIMsgParamInt* param = InParam->Cast<const FGsUIMsgParamInt>();

	// data: slotId
	DefSkillSlotList slotList;
	FindSkillSlots(param->_data, slotList);

	for (UGsUISkillButtonBase* slot : slotList)
	{
		slot->SetReserveSlot(true);
	}
}

void UGsUIHUDSkill::OnCancelReserveSkillSlot(const IGsMessageParam* InParam)
{
	const FGsUIMsgParamInt* param = InParam->Cast<const FGsUIMsgParamInt>();

	// data: slotId
	DefSkillSlotList slotList;
	FindSkillSlots(param->_data, slotList);

	for (UGsUISkillButtonBase* slot : slotList)
	{
		slot->SetReserveSlot(false);
	}
}

void UGsUIHUDSkill::OnRequestSlotUpdate(const IGsMessageParam* InParam)
{
	if (const auto param = InParam->Cast<const FGsUIMsgParamInt>())
	{
		if (const FGsSkill* skill = GSkill()->FindSkillSlot(param->_data))
		{
			DefSkillSlotList slotList;
			FindSkillSlots(param->_data, slotList);

			for (UGsUISkillButtonBase* slot : slotList)
			{
				slot->SetData(skill);
			}
		}
	}
}

void UGsUIHUDSkill::OnChangeSkillSet(const IGsMessageParam* InParam)
{
	UpdateAllSkillSlots();
}

void UGsUIHUDSkill::OnChangeSkillSlot(const IGsMessageParam* InParam)
{
	if (nullptr == InParam)
	{
		return;
	}

	const FGsGameObjectMessageParamSkillSlot* param = InParam->Cast<const FGsGameObjectMessageParamSkillSlot>();
	if (param)
	{
		// 현재 무기 타입과 동일할 때만 Set (코스튬 스킬 제외)
		if (GSkill()->GetCurrentWeaponType() == param->_weaponType)
		{
			DefSkillSlotList slotList;
			FindSkillSlots(param->_slotId, slotList);

			for (UGsUISkillButtonBase* slot : slotList)
			{
				// 하나 갱신
				if (param->_skill)
				{
					slot->SetData(param->_skill);

					if (MAX_COSTUME_SKILL_SLOT_ID == slot->GetSlotId())
					{
						if (UGsUISkillButtonActive* slotActive = Cast<UGsUISkillButtonActive>(slot))
						{
							slotActive->PlayCostumeEffect();
						}
					}
				}
				else
				{
					slot->SetEmptySlot();
				}
			}
		}
	}
}

void UGsUIHUDSkill::OnChangeSkillAuto(const IGsMessageParam* InParam)
{
	const FGsGameObjectMessageParamSkillSlot* msgParam = InParam->Cast<const FGsGameObjectMessageParamSkillSlot>();
	if (msgParam)
	{
		// 현재 무기 타입과 동일할 때만 Set (코스튬 스킬 제외)
		if (GSkill()->GetCurrentWeaponType() == msgParam->_weaponType)
		{
			if (0 == msgParam->_slotId)
			{
				return;
			}

			if (msgParam->_skill)
			{
				DefSkillSlotList slotList;
				FindSkillSlots(msgParam->_slotId, slotList);

				for (UGsUISkillButtonBase* slot : slotList)
				{
					slot->SetAutoUse(msgParam->_skill->GetSkillId(), msgParam->_skill->IsAuto());
				}
			}
		}
	}
}

void UGsUIHUDSkill::OnChangeChainSkill(const IGsMessageParam* InParam)
{
	const FGsGameObjectMessageParamSkillSlotChain* msgParam = InParam->Cast<const FGsGameObjectMessageParamSkillSlotChain>();
	if (msgParam)
	{
		DefSkillSlotList slotList;
		FindSkillSlots(msgParam->_slotId, slotList);

		for (UGsUISkillButtonBase* slot : slotList)
		{
			slot->PlayChangeStepEffect(msgParam->_prevSkill, msgParam->_skill);
		}
	}
}

void UGsUIHUDSkill::OnSetStartBuffSkillSlot(const IGsMessageParam* InParam)
{
	if (nullptr == InParam)
	{
		return;
	}

	const FGsGameObjectMessageParamSkillSlot* param = InParam->Cast<const FGsGameObjectMessageParamSkillSlot>();
	if (nullptr == param ||
		nullptr == param->_skill)
	{
		return;
	}

	// 현재 무기 타입과 동일할 때만 Set
	if (GSkill()->GetCurrentWeaponType() == param->_weaponType)
	{
		float startDelayTime = param->_skill->GetSkillCateogrySet() == SkillCategorySet::BUFF ? 0.3f + param->_skill->GetUIEnableDelayTime() : 0.f; //test

		if (param->_slotId > MAX_ACTIVE_SKILL_SLOT_ID && param->_slotId <= MAX_BUFF_SKILL_SLOT_ID)
		{
			for (int32 i = MAX_ACTIVE_SKILL_SLOT_ID + 1; i <= MAX_BUFF_SKILL_SLOT_ID; i++)
			{
				DefSkillSlotList slotList;
				FindSkillSlots(i, slotList);

				for (UGsUISkillButtonBase* slot : slotList)
				{
					slot->SetEnableDelayTime(i == param->_slotId ? 0.f : startDelayTime);
				}
			}
		}
	}
}

void UGsUIHUDSkill::OnSkillCoolDown(const IGsMessageParam* InParam)
{
	if (const FGsCoolTimeMessageParam* param = InParam->Cast<const FGsCoolTimeMessageParam>())
	{
		DefSkillSlotList slotList;
		FindSkillSlotsBySkillId(param->_id, slotList);

		for (UGsUISkillButtonBase* slot : slotList)
		{
			slot->SetCoolTime(param->_remainTime);
		}
	}
}

void UGsUIHUDSkill::OnSkillStart(const IGsMessageParam* InParam)
{
	// 자동 전투 시 스킬 사용 이펙트를 보여주기 위한 로직, 자동전투가 아니면 재생하지 않음
	if (false == GSAI()->IsAIOn())
	{
		return;
	}

	if (const FGsGameObjectMessageParamSkillSlot* param = InParam->Cast<const FGsGameObjectMessageParamSkillSlot>())
	{
		DefSkillSlotList slotList;
		FindSkillSlots(param->_slotId, slotList);

		for (UGsUISkillButtonBase* slot : slotList)
		{
			slot->PlaySkillStartEffect();
		}
	}
}

void UGsUIHUDSkill::OnAddAbnormality(const IGsMessageParam* InParam)
{
	if (const FGsPrimitivePairInt32* param = InParam->Cast<const FGsPrimitivePairInt32>())
	{
		// first : InstanceId, second : AbnormalityEffectType
		if (static_cast<int32>(AbnormalityEffectType::UTIL_MOUNT_VEHICLE) == param->_second)
		{
			_btnNormal->OnMountVehicle(true);
		}
	}
}

void UGsUIHUDSkill::OnRemoveAbnormality(const IGsMessageParam* InParam)
{
	if (const FGsPrimitivePairInt32* param = InParam->Cast<const FGsPrimitivePairInt32>())
	{
		// first : InstanceId, second : AbnormalityEffectType
		if (static_cast<int32>(AbnormalityEffectType::UTIL_MOUNT_VEHICLE) == param->_second)
		{
			_btnNormal->OnMountVehicle(false);
		}
	}
}

void UGsUIHUDSkill::FindSkillSlots(int32 InSlotId, OUT DefSkillSlotList& OutList)
{
	OutList.Empty();

	if (0 == InSlotId)
	{
		OutList.Emplace(_btnNormal);
		return;
	}

	_skillSlotSet->FindSlots(InSlotId, OutList);
}

void UGsUIHUDSkill::FindSkillSlotsBySkillId(SkillId InSkillId, OUT DefSkillSlotList& OutList)
{
	OutList.Empty();

	if (INVALID_SKILL_ID == InSkillId)
	{
		return;
	}

	if (_btnNormal->GetCachedSkillId() == InSkillId)
	{
		OutList.Emplace(_btnNormal);
		return;
	}

	_skillSlotSet->FindSlotsBySkillId(InSkillId, OutList);
}

void UGsUIHUDSkill::UpdateAllSkillSlots()
{
	// 평타 버튼 처리
	if (const FGsSkill* skill = GSkill()->FindSkillSlot(0))
	{
		// 타스킬이 덮어쓰는 문제 방지
		if (skill->IsNormalSkill())
		{
			_btnNormal->SetData(skill);
		}
	}
	else
	{
		_btnNormal->SetEmptySlot();
	}

	_skillSlotSet->UpdateAllSlots();
}
void UGsUIHUDSkill::UpdateAllNPCFunctionSlots()
{
	if (true == GGameData()->IsInvadeWorld())
	{
		_npcFunctionSet->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		_npcFunctionSet->UpdateAllSlot();
		_npcFunctionSet->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UGsUIHUDSkill::OnKeyboardEvent(EGsKeyMappingType InKeyMappingType, EInputEvent InEventType)
{
	// 평타
	if (EGsKeyMappingType::KEY_MAPPING_NORMALATTACK == InKeyMappingType)
	{
		if (EInputEvent::IE_Pressed == InEventType)
		{
			_btnNormal->OnKeyboardPress();
		}
		else if (InEventType == EInputEvent::IE_Released)
		{
			_btnNormal->OnKeyboardRelease();
		}

		return;
	}

	// 슬롯
	int32 slotId = static_cast<int32>(InKeyMappingType) - static_cast<int32>(EGsKeyMappingType::KEY_MAPPING_SKILLSLOT_1) + 1;
	
	if (false == FGsSkillUIHandler::isSkillSlotSet)
	{
		if (InEventType == EInputEvent::IE_Pressed)
		{
			_npcFunctionSet->OnKeyboardPress(slotId);
		}
		else if (InEventType == EInputEvent::IE_Released)
		{
			_npcFunctionSet->OnKeyboardRelease(slotId);
		}
	}
	else
	{
		if (InEventType == EInputEvent::IE_Pressed)
		{	
			_skillSlotSet->OnKeyboardPress(slotId);
		}
		else if (InEventType == EInputEvent::IE_Released)
		{
			_skillSlotSet->OnKeyboardRelease(slotId);
		}
	}
}