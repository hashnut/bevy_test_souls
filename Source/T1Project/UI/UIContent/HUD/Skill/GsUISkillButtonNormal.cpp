// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUISkillButtonNormal.h"
#include "Components/Image.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/Abnormality/GsAbnormalityHandlerLocalPlayer.h"
#include "Paper2D/Classes/PaperSprite.h"
#include "Data/GsResourceManager.h"
#include "Skill/GsSkill.h"


void UGsUISkillButtonNormal::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (_btnSkill)
	{
		_btnSkill->OnPressed.AddDynamic(this, &UGsUISkillButtonNormal::OnPressed);
		_btnSkill->OnReleased.AddDynamic(this, &UGsUISkillButtonNormal::OnReleased);
	}

	_btnVehicle->OnClicked.AddDynamic(this, &UGsUISkillButtonNormal::OnClickVehicle);

	UpdateClickTimer();
}

void UGsUISkillButtonNormal::NativeConstruct()
{
	Super::NativeConstruct();

	// 탈것 상태로 재접속 시 상태체크를 다시하기 위함
	SetVehicleState(IsVehicleMounted());
}

void UGsUISkillButtonNormal::SetData(const FGsSkill* InData)
{
	if (nullptr == InData)
	{
		SetEmptySlot();
		return;
	}

	// 타스킬이 덮어쓰는 문제 방지
	if (false == InData->IsNormalSkill())
	{
		return;
	}

	UpdateData(InData);
}

bool UGsUISkillButtonNormal::UpdateData(const FGsSkill* InData)
{
	if (nullptr == InData)
	{
		SetEmptySlot();
		return false;
	}

	// 타스킬이 덮어쓰는 문제 방지
	if (false == InData->IsNormalSkill())
	{
		return false;
	}

	int32 prevSkill = _skillId;
	if (false == Super::UpdateData(InData))
	{
		return false;
	}

	if (InData->GetSkillId() != prevSkill)
	{
		if (UPaperSprite* icon = GResource()->LoadSync<UPaperSprite>(*InData->GetIconPath()))
		{
			_imgIcon->SetBrushFromAtlasInterface(icon);
		}
		else
		{
			_imgIcon->SetBrushFromAtlasInterface(GetEmptyIcon());
		}
	}

	// B1 정확도가 떨어지면 켜자. 성능을 위해 막음
	//SetVehicleState(IsVehicleMounted());
	SetVehicleState(_bIsMountVehicle);

	return true;
}

void UGsUISkillButtonNormal::UpdateClickTimer()
{
	_timer = FDateTime::Now();
}

void UGsUISkillButtonNormal::SetEnableButton(bool InEnable)
{
	if (_btnSkill)
	{
		_btnSkill->SetIsEnabled(InEnable);
	}

	Super::SetEnableButton(InEnable);
}

void UGsUISkillButtonNormal::SetEmptySlot()
{
	Super::SetEmptySlot();

	if (_imgIcon)
	{
		_imgIcon->SetBrushFromAtlasInterface(GetEmptyIcon());
	}

	SetVehicleState(false);
}

void UGsUISkillButtonNormal::OnKeyboardPress()
{
	if (false == _btnSkill->GetIsEnabled())
	{
		return;
	}

	if (false == _bIsMountVehicle)
	{
		OnPressed();

	}
}

void UGsUISkillButtonNormal::OnKeyboardRelease()
{
	if (_bIsMountVehicle)
	{
		OnClickVehicle();
	}
	else
	{
		if (false == _btnSkill->GetIsEnabled() && _bSendPressKillMsg == false)
		{
			return;
		}

		OnReleased();
	}
}

void UGsUISkillButtonNormal::OnMountVehicle(bool bIsMounted)
{
	SetVehicleState(bIsMounted);
}

bool UGsUISkillButtonNormal::IsClickLock() const
{
	return (_timer + FTimespan(0, 0, _clickLockSecond)) > FDateTime::Now();
}

void UGsUISkillButtonNormal::SetVehicleState(bool bIsMounted)
{
	_bIsMountVehicle = bIsMounted;

	if (bIsMounted)
	{
		_btnSkill->SetVisibility(ESlateVisibility::Collapsed);
		_btnVehicle->SetVisibility(ESlateVisibility::Visible);

		// 끄고 싶은데 Visibility는 Enable 기능과 엮여있어서 RenderOpacity로 사용
		_btnDimmed->SetRenderOpacity(0.f);
	}
	else
	{
		_btnSkill->SetVisibility(ESlateVisibility::Visible);
		_btnVehicle->SetVisibility(ESlateVisibility::Collapsed);

		// 끄고 싶은데 Visibility는 Enable 기능과 엮여있어서 RenderOpacity로 사용
		_btnDimmed->SetRenderOpacity(1.f);
	}
}

bool UGsUISkillButtonNormal::IsVehicleMounted()
{
	if (UGsGameObjectManager* gameObjectMgr = GSGameObject())
	{
		if (UGsGameObjectBase* localPlayer = gameObjectMgr->FindObject(EGsGameObjectType::LocalPlayer))
		{
			if (FGsAbnormalityHandlerBase* abHandler = localPlayer->GetAbnormalityHandler())
			{
				if (abHandler->IsAbnormalityEffectType(AbnormalityEffectType::UTIL_MOUNT_VEHICLE))
				{
					return true;
				}
			}
		}
	}

	return false;
}

void UGsUISkillButtonNormal::OnPressed()
{
	if (false == IsClickLock())
	{
		_inputMsg.Id = _slotId;
		
		GMessage()->GetInput().SendMessage(MessageInput::PressSkill, _inputMsg);

		_bSendPressKillMsg = true;

		OnPressedEffect();

		UpdateClickTimer();
	}
}

void UGsUISkillButtonNormal::OnReleased()
{
	_inputMsg.Id = _slotId;
	GMessage()->GetInput().SendMessage(MessageInput::ReleaseSkill, _inputMsg);

	_bSendPressKillMsg = false;
}

void UGsUISkillButtonNormal::OnClickVehicle()
{
	if (false == IsClickLock())
	{
		_inputMsg.Id = _slotId;
		GMessage()->GetInput().SendMessage(MessageInput::UnmountVehicle, _inputMsg);

		OnPressedEffect();

		UpdateClickTimer();
	}
}
