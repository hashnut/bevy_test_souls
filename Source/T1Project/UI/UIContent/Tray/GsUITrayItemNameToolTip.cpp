// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/UIContent/Tray/GsUITrayItemNameToolTip.h"

#include "Components/TextBlock.h"

#include "Data/GsDataContainManager.h"

#include "UI/UIContent/Helper/GsUIColorHelper.h"

#include "UMG/Public/Blueprint/SlateBlueprintLibrary.h"


void UGsUITrayItemNameToolTip::NativeConstruct()
{
	Super::NativeConstruct();

	_curTime = 0.f;
	_isHovered = false;

	// CachedGeometry가 계산되어 중앙으로 위치조정이 끝날때까지 감춰둠	
	if (_bReserveShow)
	{
		SetRenderOpacity(0.0f);
	}

	_padding = GData()->GetGlobalData()->_nameToolTipPadding;
}

void UGsUITrayItemNameToolTip::NativeDestruct()
{
	_onFinishedDelegate.Clear();

	_bReserveShow = false;

	Super::NativeDestruct();
}

void UGsUITrayItemNameToolTip::SetNameTooltipData(const FText& InName, ItemGrade InGrade)
{
	_itemName->SetText(InName);
	_itemName->SetColorAndOpacity(FGsUIColorHelper::GetColorItemGrade(InGrade));
}

void UGsUITrayItemNameToolTip::SetHoverMode(bool InIsHover)
{
	_isHovered = InIsHover;
	_curTime = 0.f;
}

void UGsUITrayItemNameToolTip::SetTarget(TWeakObjectPtr<UUserWidget> InTarget)
{
	_targetWidget = InTarget;
}

void UGsUITrayItemNameToolTip::SetTargetPosition(const FVector2D& InAbsPos)
{
	_absTargetPos = InAbsPos;
	_bReserveShow = true;

	SetPositionInViewport(FVector2D::ZeroVector);
}

void UGsUITrayItemNameToolTip::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (_bReserveShow)
	{
		if (false == MyGeometry.GetLocalSize().IsZero())
		{
			_bReserveShow = false;

			FVector2D targetPos = _absTargetPos;
			targetPos.X -= MyGeometry.GetAbsoluteSize().X * 0.5f;

			FVector2D pixel;
			FVector2D viewport;
			USlateBlueprintLibrary::AbsoluteToViewport(this, targetPos, pixel, viewport);

			SetPositionInViewport(pixel);
			SetRenderOpacity(1.0f);
		}
	}

	UpdateTooltipPosition(MyGeometry);

	if (_isHovered)
		return;

	_curTime += InDeltaTime;
	if (_curTime >= _nameToolTipDurationTime)
	{
		_onFinishedDelegate.Broadcast();
	}
}

void UGsUITrayItemNameToolTip::UpdateTooltipPosition(const FGeometry& MyGeometry)
{
	if (false == _targetWidget.IsValid())
	{
		return;
	}

	if (nullptr == GEngine || nullptr == GEngine->GameViewport)
	{
		return;
	}

	FVector2D iconTopLeft;
	FVector2D viewport;
	USlateBlueprintLibrary::LocalToViewport(_targetWidget.Get(), _targetWidget->GetTickSpaceGeometry(), FVector2D::ZeroVector, iconTopLeft, viewport);
	

	FVector2D iconTopMiddle;
	iconTopMiddle.X = iconTopLeft.X + (_targetWidget->GetTickSpaceGeometry().GetAbsoluteSize().X * 0.5f);
	iconTopMiddle.Y = iconTopLeft.Y;

	// 화면 크기
	FVector2D viewportSize;
	GEngine->GameViewport->GetViewportSize(viewportSize);

	float halfToolTipGeometryX = MyGeometry.GetAbsoluteSize().X * 0.5f;
	float halfToolTipGeometryY = MyGeometry.GetAbsoluteSize().Y * 0.5f;

	// 화면 영역과 툴팁 우측 사이의 거리. 양수일 경우 우측 영역 밖으로 나간 것으로 판단
	float distanceRight = iconTopMiddle.X + halfToolTipGeometryX - viewportSize.X + _padding.X;
	bool rightOver = (distanceRight > 0);

	// 화면 영역과 툴팁 좌측 사이의 거리. 음수일 경우 좌측 영역 밖으로 나간 것으로 판단
	float distanceLeft = iconTopMiddle.X - halfToolTipGeometryX - _padding.X;
	bool leftOver = (distanceLeft < 0);

	FVector2D toolTipTopLeft;
	toolTipTopLeft.X = iconTopMiddle.X - halfToolTipGeometryX;
	toolTipTopLeft.Y = iconTopMiddle.Y - halfToolTipGeometryY;

	// 패딩 계산
	toolTipTopLeft.X += (rightOver ? -distanceRight : (leftOver ? -distanceLeft : 0));
	toolTipTopLeft.Y -= _padding.Y;

	SetPositionInViewport(toolTipTopLeft);
}
