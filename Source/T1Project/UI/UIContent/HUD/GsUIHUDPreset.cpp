// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIHUDPreset.h"

#include "Components/WidgetSwitcher.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/HUD/Preset/GsUIPresetSelect.h"
#include "UI/UIContent/Common/GsUICoolTimeEffectMaterial.h"

#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Management/ScopeGame/GsPresetManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"

#include "Management/GsMessageHolder.h"

#include "Message/MessageParam/GsCoolTimeMessageParam.h"

#include "Data/GsDataContainManager.h"


void UGsUIHUDPreset::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnPresetCurrent->OnClicked.AddDynamic(this, &UGsUIHUDPreset::OnClickPreset);
}

void UGsUIHUDPreset::NativeConstruct()
{
	Super::NativeConstruct();
	RegisterMessages();

	_isSelectOpen = false;
	_hasCoolTime = false; // 서버에서 받게되면 변경될 예정
}

void UGsUIHUDPreset::NativeDestruct()
{
	UnregisterMessages();
	Super::NativeDestruct();
}

bool UGsUIHUDPreset::UpdateManagerTick(float InDeltaTime)
{
	if (false == Super::UpdateManagerTick(InDeltaTime))
	{
		return false;
	}

	// 쿨타임 갱신
	if (_hasCoolTime)
	{
		float rate = GSPreset()->GetRemainCoolTimeRate();

		_effectCoolTime->SetProgress(rate);
	}

	return true;
}

void UGsUIHUDPreset::RegisterMessages()
{
	FGsMessageHolder* msg = GMessage();

	_gameObjectMsgHandlerList.Empty();
	_gameObjectMsgHandlerList.Emplace(msg->GetGameObject().AddUObject(MessageGameObject::LOCAL_COOLTIME_START,
		this, &UGsUIHUDPreset::OnLocalCooltimeStart));
	_gameObjectMsgHandlerList.Emplace(msg->GetGameObject().AddUObject(MessageGameObject::LOCAL_COOLTIME_END,
		this, &UGsUIHUDPreset::OnLocalCooltimeEnd));
	_gameObjectMsgHandlerList.Emplace(msg->GetGameObject().AddUObject(MessageGameObject::UPDATE_USER_PRESET_ID,
		this, &UGsUIHUDPreset::OnUpdateUserPresetId));
	_gameObjectMsgHandlerList.Emplace(msg->GetGameObject().AddUObject(MessageGameObject::UPDATE_USER_DATA,
		this, &UGsUIHUDPreset::OnUpdateUserPresetId));
	_gameObjectMsgHandlerList.Emplace(msg->GetGameObject().AddUObject(MessageGameObject::LOCAL_SPAWN_ME_COMPLETE,
		this, &UGsUIHUDPreset::OnUpdateUserPresetId));
}

void UGsUIHUDPreset::UnregisterMessages()
{
	FGsMessageHolder* msg = GMessage();

	for (MsgGameObjHandle& elemItem : _gameObjectMsgHandlerList)
	{
		msg->GetGameObject().Remove(elemItem);
	}
	_gameObjectMsgHandlerList.Empty();
}

void UGsUIHUDPreset::OnClickPreset()
{
	// 프리셋 선택 UI 토글
	OpenPresetSelect(!_isSelectOpen);

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDPreset::OnLocalCooltimeStart(const IGsMessageParam* InParam)
{
	if (const FGsCoolTimeMessageParam* param = InParam->Cast<const FGsCoolTimeMessageParam>())
	{
		if (EGsCoolTime::Preset == param->_type)
		{
			_hasCoolTime = true;

			InvalidateAllInternal();
			_uiPresetSelect->InvalidateSelectUI();
		}
		else if (EGsCoolTime::Change_Weapon == param->_type)
		{
			_uiPresetSelect->InvalidateSelectUI();
		}
	}
}

void UGsUIHUDPreset::OnLocalCooltimeEnd(const IGsMessageParam* InParam)
{
	if (const FGsCoolTimeMessageParam* param = InParam->Cast<const FGsCoolTimeMessageParam>())
	{
		if (EGsCoolTime::Preset == param->_type)
		{
			_hasCoolTime = false;

			InvalidateAllInternal();
			_uiPresetSelect->InvalidateSelectUI();
		}
		else if (EGsCoolTime::Change_Weapon == param->_type)
		{
			_uiPresetSelect->InvalidateSelectUI();
		}
	}
}

void UGsUIHUDPreset::OnUpdateUserPresetId(const IGsMessageParam* InParam)
{
	if (FGsPresetManager* presetManager = GSPreset())
	{
		presetManager->AckUserPresetChange();
	}
	
	InvalidateAllInternal();
}

void UGsUIHUDPreset::SetPresetSelect(UGsUIPresetSelect* InPresetSelect)
{
	_uiPresetSelect = InPresetSelect;

	InvalidateAllInternal();
}

void UGsUIHUDPreset::OpenPresetSelect(bool InIsOpen)
{
	_isSelectOpen = InIsOpen;
	if (_isSelectOpen)
	{
		_uiPresetSelect->Show();
	}
	else
	{
		_uiPresetSelect->Hide();
	}

	InvalidateAllInternal();
}

void UGsUIHUDPreset::InvalidateAllInternal()
{
	FGsPresetManager* presetManager = GSPreset();
	if (nullptr == presetManager)
		return;

	if (_uiPresetSelect)
	{
		_isSelectOpen = (_uiPresetSelect->GetVisibility() == ESlateVisibility::SelfHitTestInvisible);
	}

	if (_isSelectOpen)
	{
		_switcherPreset->SetVisibility(ESlateVisibility::Collapsed);
		_effectSelected->SetVisibility(ESlateVisibility::Collapsed);
		_effectWait->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		_switcherPreset->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_effectSelected->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_effectWait->SetVisibility(ESlateVisibility::Collapsed);

		PresetId presetId = presetManager->GetCurrentPresetId();

		_switcherPreset->SetActiveWidgetIndex(presetId - 1);
	}
	
	if (_hasCoolTime)
	{
		_effectCoolTime->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		float rate = presetManager->GetRemainCoolTimeRate();
		_effectCoolTime->SetProgress(rate);
	}
	else
	{
		_effectCoolTime->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (presetManager->IsReqUserPresetChange())
	{
		_effectCoolTime->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_effectCoolTime->SetProgress(1.5f);
	}
}

void UGsUIHUDPreset::SetHUDMode(EGsUIHUDMode InMode)
{
	Super::SetHUDMode(InMode);

	if (UGsDataContainManager* dataManager = GData())
	{
		if (UGsGlobalConstant* globalData = dataManager->GetGlobalData())
		{
			if (globalData->_showPreset)
			{
				_uiPresetSelect->SetHUDMode(IsHideMode(InMode));
			}
			else
			{
				SetVisibility(ESlateVisibility::Collapsed);
				_uiPresetSelect->SetHUDMode(true);
			}
		}
	}
	
}
