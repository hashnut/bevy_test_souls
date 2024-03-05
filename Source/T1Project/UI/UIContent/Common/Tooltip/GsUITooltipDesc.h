// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITooltip.h"
#include "GsUITooltipDesc.generated.h"

class UTextBlock;

/**
 * 내용을 출력하는 툴팁
 */
UCLASS()
class T1PROJECT_API UGsUITooltipDesc : public UGsUITooltip
{
	GENERATED_BODY()

protected:
	DECLARE_DELEGATE(FOnInvalidatePosition);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockDesc;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUITooltip")
	FLinearColor _colorDefaultDesc;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUITooltip")
	float _lifeTime = 5.f;
	
	float _elapsedTime = 0.f;

	int32 _needRefresh = 0;

public:
	// 위치 조정
	FOnInvalidatePosition OnInvalidatePosition;

protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	virtual void Open();
	virtual void Close();

	virtual void SetDefaultColor();

	void ToggleOpenClose();

	void SetDescText(const FText& InText);
	void SetDescColor(const FLinearColor& InColor);

	void SetLifeTime(float InLifeTime = 0.f);

	// 툴팁이 늘어나 화면 밖을 빠져나가지 않게 강제 글자 줄바꿈 설정 필요 시
	void SetWrapTextOption(float InWrapTextAt);

	void SetIsNeedRefresh(bool InIsNeedRefresh);

	FVector2D GetTextBlockSize();
};