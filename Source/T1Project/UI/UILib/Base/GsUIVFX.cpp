// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIVFX.h"
#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"

void UGsUIVFX::NativeConstruct()
{	
	InitAnimationList();

	SetVisibility(ESlateVisibility::Collapsed);

	if (_bDynamicCreate)
	{
		if (_bAdjustToCenter)
		{
			// CachedGeometry가 계산되어 중앙으로 위치조정이 끝날때까지 감춰둠
			SetRenderOpacity(0.0f);
		}
	}
	else
	{
		if (_bAutoStart)
		{
			ShowVFX();
		}
	}

	Super::NativeConstruct();
}

void UGsUIVFX::NativeDestruct()
{
	_animationList.Empty();

	_bDynamicCreate = false;
	_bReserveShow = false;
	_bAdjustToCenter = false;

	Super::NativeDestruct();
}

void UGsUIVFX::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (_bReserveShow)
	{
		if (false == MyGeometry.GetLocalSize().IsZero())
		{
			_bReserveShow = false;

			if (_bAdjustToCenter)
			{
				FVector2D targetPos = _dynamicTargetPos - MyGeometry.GetLocalSize() * 0.5f;
				SetPositionInViewport(targetPos, false);
			}

			SetRenderOpacity(1.0f);
		}
	}
}

void UGsUIVFX::ShowVFX()
{
	if (_bDynamicCreate)
	{
		if (_bAdjustToCenter)
		{
			// Geometry 연산 후 위치 조정을 위해 예약한다. NativeTick에서 처리
			_bReserveShow = true;
		}
		else
		{
			SetRenderOpacity(1.0f);
		}
	}

	UGsBlueprintFunctionLibraryUI::StopAllAndClearAnimation(this);

	OnShowVFXEvent();
}

void UGsUIVFX::HideVFX()
{
	OnHideVFXEvent();

	if (_bDynamicCreate)
	{
		UGsBlueprintFunctionLibraryUI::RemoveVFX(this);
	}
}

void UGsUIVFX::OnShowVFXEvent_Implementation()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	// 연출 애니를 한개이상 넣을 가능성이 없다고 본다
	if (0 < _animationList.Num())
	{
		UWidgetAnimation* anim = _animationList[0];
		if (nullptr != anim)
		{
			PlayAnimation(anim, _startAtTime, _numLoopsToPlay, static_cast<EUMGSequencePlayMode::Type>(_playMode), _playbackSpeed);
		}
	}
}

void UGsUIVFX::OnHideVFXEvent_Implementation()
{
	UGsBlueprintFunctionLibraryUI::StopAllAndClearAnimation(this);

	SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUIVFX::InitAnimationList()
{
	_animationList.Empty();

	UWidgetBlueprintGeneratedClass* genClass = GetWidgetTreeOwningClass();

	if (nullptr != genClass)
	{
		for (UWidgetAnimation* anim : genClass->Animations)
		{
			_animationList.Add(anim);
		}
	}
}

void UGsUIVFX::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	if (0 < _animationList.Num())
	{
		// 동일한 애니메이션인지 체크
		UWidgetAnimation* anim = _animationList[0];
		if (anim == Animation)
		{
			OnVFXAnimationFinished.ExecuteIfBound();

			HideVFX();
		}
	}
}

void UGsUIVFX::SetDynamicCreate(bool InDynamic, const FVector2D& InPos, bool InAdjustToCenter)
{
	_bDynamicCreate = InDynamic;
	_dynamicTargetPos = InPos;
	_bAdjustToCenter = InAdjustToCenter;

	SetPositionInViewport(InPos, false);
}

float UGsUIVFX::GetAnimEndTime()
{
	if (0 < _animationList.Num())
	{
		UWidgetAnimation* anim = _animationList[0];
		if (nullptr != anim)
		{
			return anim->GetEndTime();
		}
	}
	return 0.f;
}