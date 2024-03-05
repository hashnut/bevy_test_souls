// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUIIconBase.generated.h"


/**
 * 아이콘 기본 부모 클래스
 */
UCLASS(Abstract, meta = (DisableNativeTick))
class T1PROJECT_API UGsUIIconBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	bool bIsManagedByPool = false;

public:
	virtual void SetEmptyIcon() {}
	virtual void SetIconImage(const FSoftObjectPath& InPath) {}
	virtual void SetIconImageColor(const FLinearColor& InColor) {}
	virtual void SetImageGrayscale(const bool InIsGray) {}

public:
	void ReleaseToPool();
	void SetIsManagedByPool(bool InEnable) { bIsManagedByPool = InEnable; }
};
