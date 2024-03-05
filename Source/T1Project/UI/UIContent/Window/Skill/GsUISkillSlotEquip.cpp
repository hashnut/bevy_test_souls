// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUISkillSlotEquip.h"
#include "Components/WidgetSwitcher.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Skill/GsSkill.h"
#include "Skill/GsSkillHelper.h"
#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Net/GsNetSendServiceWorld.h"


void UGsUISkillSlotEquip::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnSlot->OnReleased.AddDynamic(this, &UGsUISkillSlotEquip::OnSlotReleased);
	_btnSlot->OnMouseButtonEventDownEx.BindUObject(this, &UGsUISkillSlotEquip::OnMouseButtonDownEx);

	_btnRemove->OnClicked.AddDynamic(this, &UGsUISkillSlotEquip::OnClickRemove);

	_dragDownLogic.OnSlotDragDownTranslation.BindUObject(this, &UGsUISkillSlotEquip::OnSlotDragDownTranslation);
	_dragDownLogic.OnSlotDragDownStateChanged.BindUObject(this, &UGsUISkillSlotEquip::OnSlotDragDownStateChanged);
}

void UGsUISkillSlotEquip::NativeConstruct()
{
	Super::NativeConstruct();

	_btnRemove->SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUISkillSlotEquip::NativeDestruct()
{
	_skill = nullptr;

	Super::NativeDestruct();
}

FReply UGsUISkillSlotEquip::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	// 버튼때문에 TouchStart는 안들어옴
	// 주의: 버튼이 PreciseClick 옵션일 때만 이곳에 들어온다
	FReply reply = Super::NativeOnTouchMoved(InGeometry, InGestureEvent);

	if (_bIsCaptureMouse)
	{
		if (false == _pressStartPosAbs.IsZero())
		{
			if (_bCanDragDown)
			{
				_dragDownLogic.OnMouseButtonMove(InGeometry, InGestureEvent);
			}

			FVector2D startPos = InGeometry.AbsoluteToLocal(_pressStartPosAbs);
			FVector2D movePos = InGeometry.AbsoluteToLocal(InGestureEvent.GetScreenSpacePosition());
			float diffY = FMath::Abs(startPos.Y - movePos.Y);
			float diffX = FMath::Abs(startPos.X - movePos.X);

			if (_dragDownLogic.GetSensitiveValue() < diffY &&
				_dragDownLogic.GetSensitiveValue() > diffX)
			{
				return FReply::Handled().CaptureMouse(TakeWidget());
			}
		}
	}

	return Super::NativeOnTouchMoved(InGeometry, InGestureEvent);
}

FReply UGsUISkillSlotEquip::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	Super::NativeOnTouchEnded(InGeometry, InGestureEvent);

	_pressStartPosAbs = FVector2D::ZeroVector;

	if (_bIsCaptureMouse)
	{
		if (_bCanDragDown)
		{
			_dragDownLogic.OnButtonReleased();
		}

		_bIsCaptureMouse = false;

		return FReply::Handled().ReleaseMouseCapture();
	}

	_bIsCaptureMouse = false;

	return FReply::Unhandled();
}

bool UGsUISkillSlotEquip::SetData(const FGsSkill* InData)
{
	if (false == Super::SetData(InData))
	{
		return false;
	}

	SetIsAuto(_skill->IsAuto());

	return true;
}

void UGsUISkillSlotEquip::SetEmpty()
{
	Super::SetEmpty();

	_switcherEffect->SetActiveWidgetIndex(EGsSkillSlotEffectType::NORMAL);
	_bCanDragDown = false;
	SetIsAuto(false);
	_btnRemove->SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUISkillSlotEquip::OnClickSlot()
{
	OnClickSkillSlot.ExecuteIfBound(this);
}

void UGsUISkillSlotEquip::OnClickRemove()
{
	RequestRemoveSlot();
}

void UGsUISkillSlotEquip::OnSlotReleased()
{
	// 스크롤 판정 혹은 캡쳐되지 않은 상태에서 영역 밖으로 나간경우에 초기화 위함
	if (_bIsCaptureMouse)
	{
		if (false == HasMouseCapture())
		{
			_bIsCaptureMouse = false;
			_pressStartPosAbs = FVector2D::ZeroVector;
		}
	}
}

void UGsUISkillSlotEquip::OnMouseButtonDownEx(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
	_bAutoChanged = false;

	// 주의: 버튼이 있으면 NativeOnTouchStarted 가 안들어오므로 여기서 저장. 
	// 계산을 버튼이 아닌 위젯으로 할거라 로컬좌표로 변환하지 않은 절대좌표 저장
	if (false == _bIsCaptureMouse)
	{
		_bIsCaptureMouse = true;
		_pressStartPosAbs = MouseEvent.GetScreenSpacePosition();

		if (_bCanDragDown)
		{
			_dragDownLogic.OnMouseButtonDown(Geometry, MouseEvent);
		}
	}
}

void UGsUISkillSlotEquip::OnSlotDragDownTranslation(const FVector2D& InTranslation)
{
	SetRenderTranslation(InTranslation);
}

void UGsUISkillSlotEquip::OnSlotDragDownStateChanged()
{
	bool bRequestAuto = _dragDownLogic.GetIsDown();

	if (_bCanDragDown)
	{
		if (false == IsEmpty())
		{
			// 오토 토글 메시지
			bRequestAuto = _dragDownLogic.GetIsDown();
			FGsUIMsgParamRequestSkillAuto param(_skill->GetSkillId(), bRequestAuto);
			GMessage()->GetUI().SendMessage(MessageUI::REQUEST_SKILL_AUTO, &param);
		}
	}

	SetIsAuto(bRequestAuto);
}

void UGsUISkillSlotEquip::SetEnableEquip(bool bInEnable, bool bIsEquipMode, const FGsSkill* InSelectedSkill)
{
	SetIconDimmed(!bInEnable);

	// 비어있지 않고, 활성상태면 컨트롤 ON
	bool bEnableControl = false;
	if (false == IsEmpty() && bInEnable)
	{
		bEnableControl = true;
	}

	_btnRemove->SetVisibility(bEnableControl ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	_bCanDragDown = bEnableControl;

	EGsSkillSlotEffectType effectType = EGsSkillSlotEffectType::NORMAL;

	if (_skill && 
		InSelectedSkill)
	{
		if (InSelectedSkill->GetSkillId() == _skill->GetSkillId())
		{
			_switcherEffect->SetVisibility(bInEnable ? 
				ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

			_switcherEffect->SetActiveWidgetIndex(EGsSkillSlotEffectType::SAME_SKILL);

			return;
		}
		
		if (InSelectedSkill->GetSkillGroupId() == _skill->GetSkillGroupId())
		{
			// 0인 값은 무시
			if (0 < InSelectedSkill->GetSkillGroupId())
			{
				effectType = EGsSkillSlotEffectType::SAME_GROUP;
			}
		}
	}

	_switcherEffect->SetVisibility((bInEnable && bIsEquipMode) ?
		ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	_switcherEffect->SetActiveWidgetIndex(effectType);
}

void UGsUISkillSlotEquip::SetIsAuto(bool bInIsAuto)
{
	_dragDownLogic.SetIsDown(bInIsAuto, false);

	_effectAuto->SetVisibility(bInIsAuto ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

void UGsUISkillSlotEquip::RequestRemoveSlot()
{
	// 여기서 _skill 널체크 함
	if (IsEmpty())
	{
		return;
	}

	SkillId rootSkillId = FGsSkillHelper::GetUpgradeRootSkillId(_skill);
	if (INVALID_SKILL_ID == rootSkillId)
	{
		rootSkillId = _skill->GetSkillId();
	}

	if (MAX_COSTUME_SKILL_SLOT_ID == _slotId)
	{
		CostumeId costumeId = FGsSkillHelper::GetCostumeIdBySkillId(_skill->GetSkillId());
		if (INVALID_COSTUME_ID != costumeId)
		{
			FGsNetSendServiceWorld::SendRequestCostumeSkillSlotSet(costumeId,
				rootSkillId, _skill->GetSkillId(), INVALID_SKILL_SLOT_ID);
		}
	}
	else
	{
		FGsNetSendServiceWorld::SendRequestSkillSlotSet(_skill->GetSkillRequireWeaponType(),
			rootSkillId, _skill->GetSkillId(), INVALID_SKILL_SLOT_ID);
	}
}
