// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Common/Icon/GsUIIconBase.h"
#include "GsUIIconDefault.generated.h"


/**
 * 아이콘 기본 부모 클래스
 */
UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIIconDefault : public UGsUIIconBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	class UImage* _iconImage;

protected:
	FString _strPath;

public:
	virtual void SetIconImage(const FSoftObjectPath& InPath) override;
	virtual void SetIconImageColor(const FLinearColor& InColor) override;

public:
	void SetIconImageSprite(class UPaperSprite* InSprite);

	bool IsSamePath(const FString& InPath);
};
