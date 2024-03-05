// Copyright Epic Games, Inc. All Rights Reserved.

#include "GsSlotDragDownLogic.h"
#include "Runtime/UMG/Public/Components/Widget.h"


void FGsSlotDragDownLogic::OnMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
	_pointerIndex = MouseEvent.GetPointerIndex();
	
	_dragStartPosAbs = MouseEvent.GetScreenSpacePosition();
	_dragStartPos = FVector2D::ZeroVector;
	_prevMovePos = FVector2D::ZeroVector;	
	_bCheckSensitive = true;
	_bCheckDragDown = true;
}

void FGsSlotDragDownLogic::OnMouseButtonMove(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.GetPointerIndex() != _pointerIndex)
	{
		return;
	}

	if (false == _bCheckDragDown)
	{
		return;
	}

	if (_dragStartPos.IsZero())
	{
		_dragStartPos = Geometry.AbsoluteToLocal(_dragStartPosAbs);
		_prevMovePos = _dragStartPos;
	}

	FVector2D currPos = Geometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());	

	// 일정 거리 이상 이동 시 판정
	if (_bCheckSensitive)
	{
		float currDiffY = FMath::Abs(_dragStartPos.Y - currPos.Y);
		float currDiffX = FMath::Abs(_dragStartPos.X - currPos.X);

		// 스크롤을 방해하는 문제로 X축도 체크		
		if (_sensitiveValue < currDiffY &&
			_sensitiveValue > currDiffX)
		{
			_bCheckSensitive = false;
		}
		else
		{
			_prevMovePos = currPos;
			return;
		}
	}

	float moveDist = FMath::Abs(_prevMovePos.Y - currPos.Y);

	if (_bIsDown)
	{
		_slotTranslation.Y -= moveDist;
		if (0 >= _slotTranslation.Y)
		{
			SetMoveDown(false);
		}
	}
	else
	{
		_slotTranslation.Y += moveDist;
		if (_dragDownValue <= _slotTranslation.Y)
		{
			SetMoveDown(true);
		}
	}

	OnSlotDragDownTranslation.ExecuteIfBound(_slotTranslation);

	_prevMovePos = currPos;
}

void FGsSlotDragDownLogic::OnButtonReleased()
{
	_pointerIndex = 0;
	_dragStartPosAbs = FVector2D::ZeroVector;
	_dragStartPos = FVector2D::ZeroVector;

	if (_bCheckDragDown)
	{
		SetIsDown(_bIsDownTemp, true);
	}
}

void FGsSlotDragDownLogic::SetMoveDown(bool bIsDown)
{
	_bIsDownTemp = bIsDown;

	_slotTranslation.Y = _bIsDownTemp ? _dragDownValue : 0.f;

	OnSlotDragDownTranslation.ExecuteIfBound(_slotTranslation);
}

void FGsSlotDragDownLogic::SetIsDown(bool bIsDown, bool bSendChangedEvent)
{
	_bCheckSensitive = false;
	_bCheckDragDown = false;

	bool bIsChanged = (_bIsDown != bIsDown) ? true : false;
	_bIsDown = bIsDown;

	SetMoveDown(bIsDown);
	
	if (bIsChanged)
	{
		if (bSendChangedEvent)
		{
			OnSlotDragDownStateChanged.ExecuteIfBound();
		}
	}
}
