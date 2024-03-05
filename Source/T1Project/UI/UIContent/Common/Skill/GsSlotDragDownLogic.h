// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"


class FGsSlotDragDownLogic
{
public:
	DECLARE_DELEGATE_OneParam(FOnSlotDragDownTranslation, const FVector2D&);
	DECLARE_DELEGATE(FOnSlotDragDownStateChanged);

private:
	float _sensitiveValue = 8.f;
	float _dragDownValue = 10.f;	

private:
	uint32 _pointerIndex = 0;

	FVector2D _dragStartPosAbs;
	FVector2D _dragStartPos;
	FVector2D _prevMovePos;

	bool _bIsDown = false;
	bool _bIsDownTemp = false; // 조작 중일 때의 상태
	bool _bCheckSensitive = false;
	bool _bCheckDragDown = false;

	FVector2D _slotTranslation;

public:
	FOnSlotDragDownTranslation OnSlotDragDownTranslation;
	FOnSlotDragDownStateChanged OnSlotDragDownStateChanged;

public:
	void SetIsDown(bool bIsDown, bool bSendChangedEvent);

	void OnMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& MouseEvent);	
	void OnMouseButtonMove(const FGeometry& Geometry, const FPointerEvent& MouseEvent);
	void OnButtonReleased();

	bool GetIsDown() const { return _bIsDown; }

	void SetSensitiveValue(float InValue) { _sensitiveValue = InValue; }
	void SetDragDownValue(float InValue) { _dragDownValue = InValue; }
	float GetSensitiveValue() const { return _sensitiveValue; }

	bool IsCheckingDragDown() const { return _bCheckDragDown; }

private:
	void SetMoveDown(bool bIsDown);
};
