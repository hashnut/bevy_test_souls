// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "UI/UIControlLib/Define/GsIconDefine.h"

class UImage;
class UGsUIRedDotBase;

/*
 * FGsRedDotChecker
 * 레드닷 구현 편의 함수
 */
struct T1PROJECT_API FGsRedDotChecker
{
	TWeakObjectPtr<UImage> _imgRedDot = nullptr;
	TWeakObjectPtr<UGsUIRedDotBase> _uiRedDot = nullptr;
	EGsIconRedDotType _redDotType = EGsIconRedDotType::NORMAL;
	TFunction<bool()> _redDotCheckFunc; // null일 수 있음

public:
	FGsRedDotChecker() = default;
	explicit FGsRedDotChecker(UImage* InImgRedDot, TFunction<bool()> InRedDotFunc)
	{
		Initialize(InImgRedDot, InRedDotFunc);
	}

	explicit FGsRedDotChecker(UGsUIRedDotBase* InRedDot, EGsIconRedDotType InType, TFunction<bool()> InRedDotFunc)
	{
		Initialize(InRedDot, InType, InRedDotFunc);
	}

	void Initialize(UImage* InImgRedDot, TFunction<bool()> InRedDotFunc); // 주의: 삭제 예정
	void Initialize(UGsUIRedDotBase* InRedDot, EGsIconRedDotType InType, TFunction<bool()> InRedDotFunc);

	// Set 만 진행
	void SetIsRedDot(bool bIsRedDot);

	// 체크 후 Set 진행
	bool UpdateRedDot();
};
