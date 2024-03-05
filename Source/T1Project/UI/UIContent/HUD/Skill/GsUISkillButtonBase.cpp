// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUISkillButtonBase.h"
#include "GameObject/Skill/Data/GsSkillDataBase.h"
#include "DataSchema/Skill/GsSchemaSkillSet.h"
#include "Engine/AssetManager.h"
#include "Paper2D/Classes/PaperSprite.h"
#include "Util/GsTableUtil.h"
#include "Components/PanelWidget.h"
#include "Skill/GsSkill.h"
#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UILib/Manager/GsTooltipManager.h"
#include "T1Project.h"


void UGsUISkillButtonBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnDimmed->OnClicked.AddDynamic(this, &UGsUISkillButtonBase::OnClickDimmedButton);
}

void UGsUISkillButtonBase::NativeConstruct()
{
	Super::NativeConstruct();

	_btnDimmed->SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUISkillButtonBase::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
#if WITH_EDITOR || PLATFORM_WINDOWS
	OpenNameTooltip();
#endif //WITH_EDITOR
}

void UGsUISkillButtonBase::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
#if WITH_EDITOR || PLATFORM_WINDOWS
	CloseNameTooltip();
#endif //WITH_EDITOR
	Super::NativeOnMouseLeave(InMouseEvent);
}

void UGsUISkillButtonBase::UpdateManagerTick(float InDeltaTime)
{
	if (false == IsVisible())
	{
		return;
	}

	if (_bIsCoolTime)
	{
		SetCoolTimeInter(_remainSec - GetElapsedTime());
		if (false == _bIsCoolTime)
		{
			UpdateEnableButton();
		}
	}
	else if (_enableStartDelay && _deltaEnableDelay > 0)
	{
		_deltaEnableDelay -= InDeltaTime;
		if (_deltaEnableDelay <= 0.f)
		{
			_deltaEnableDelay = 0.f;
			_enableStartDelay = false;
			UpdateEnableButton();
		}
	}
}

bool UGsUISkillButtonBase::UpdateData(const FGsSkill* InData)
{
	if (nullptr == InData)
	{
		SetEmptySlot();
		return false;
	}

	_skillId = InData->GetSkillId();
	_coolTime = InData->GetCooldownTime(InData->GetTotalLevel());

	SetReserveSlot(InData->IsReserve());
	SetCoolTime(InData->GetCurrentCooldownTime(), false);
	SetEnableButton(InData->IsEnable());

	return true;
}

void UGsUISkillButtonBase::OnClickDimmedButton()
{
	FGsUIMsgParamInt param(_slotId);
	GMessage()->GetUI().SendMessage(MessageUI::DIMMED_SKILLBUTTON_ONCLICK, &param);
}

void UGsUISkillButtonBase::SetEmptySlot()
{
	_skillId = -1;
	_coolTime = 0.f;
	_remainSec = 0.f;
	_syncCoolTime = 0;
	_bIsCoolTime = false;

	SetReserveSlot(false);
	SetCoolTime(0.0f, false);
	SetEnableButton(false);
}

void UGsUISkillButtonBase::SetEnableDelayTime(float delta)
{
	_deltaEnableDelay = delta;
	_enableStartDelay = delta > 0.f && !_bIsCoolTime ? true : false;

	if(_enableStartDelay) SetEnableButton(false);
}

void UGsUISkillButtonBase::UpdateEnableButton()
{
	if (false == _bIsFakePage)
	{
		FGsUIMsgParamInt param(_slotId);
		GMessage()->GetUI().SendMessage(MessageUI::REQUEST_UPDATE_SKILL_SLOT, &param);
	}
}

void UGsUISkillButtonBase::SetCoolTime(float InRemainSec, bool bInCheckEnableButton)
{
	if (0 < InRemainSec)
	{
		_remainSec = InRemainSec;
		_syncCoolTime = FApp::GetCurrentTime();

		SetCoolTimeInter(InRemainSec);
		// 쿨타임 중에 "대기" 안뜨도록 강제 설정
		SetReserveSlot(false);
	}
	else
	{
		_remainSec = 0.f;
		_syncCoolTime = 0.f;

		SetCoolTimeInter(0.0f);
	}

	if (bInCheckEnableButton)
	{
		UpdateEnableButton();
	}
}

void UGsUISkillButtonBase::SetCoolTimeInter(float InRemainSec)
{
	float percent = 0.0f;
	if (0 < InRemainSec)
	{
		_bIsCoolTime = true;

		float ratio = (0.0f < _coolTime) ? 1.0f / _coolTime : 0.0f;
		percent = FMath::Clamp(ratio * InRemainSec, 0.0f, 1.0f);

		SetCooltimeUI(InRemainSec, percent);
	}
	else
	{
		// 쿨타임이 있다가 사라지는 경우
		if (_bIsCoolTime)
		{
			OnFinishCoolTime();
		}

		_bIsCoolTime = false;
		_syncCoolTime = 0;

		SetCooltimeUI(0.f, 0.f);
	}
}

UPaperSprite* UGsUISkillButtonBase::GetEmptyIcon()
{
	if (false == _emptyIconRef.IsNull())
	{
		if (_emptyIconRef.IsPending())
		{
			FStreamableManager& assetMgr = UAssetManager::GetStreamableManager();
			_emptyIconRef = assetMgr.LoadSynchronous(_emptyIconRef, true);
		}

		return _emptyIconRef.Get();
	}

	return nullptr;
}

void UGsUISkillButtonBase::SetEnableButton(bool InEnable)
{
	_btnDimmed->SetVisibility((InEnable) ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
}

void UGsUISkillButtonBase::OpenNameTooltip()
{
	if (_nameToolTipObjPtr.IsValid())
	{
		return;
	}

	if (UGsTooltipManager* tooltipManager = GUI()->GetTooltipManager())
	{
		// 다른 이름 툴팁 닫기
		tooltipManager->CloseNameTooltip();

		// 이름 툴팁 추가 및 설정
		FText tooltipText;
		GetNameText(tooltipText);
		_nameToolTipObjPtr = tooltipManager->OpenNameTooltip(this, tooltipText, GetSkillGrade());
	}
}

float UGsUISkillButtonBase::GetElapsedTime()
{
	float elapsedSeconds = FApp::GetCurrentTime() - _syncCoolTime;

	return (0 < elapsedSeconds) ? elapsedSeconds : 0.f;
}
