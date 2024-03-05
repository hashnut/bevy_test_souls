// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITray.h"
#include "GsUITrayTickerJoinGuild.generated.h"

class UTextBlock;
class UImage;

/**
 * UGsUITrayTickerJoinGuild
 * - 길드 가입 시
 */
UCLASS()
class T1PROJECT_API UGsUITrayTickerJoinGuild : public UGsUITray
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockDesc;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockGuild;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgEmblem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	float _playTime = 2.0f;

	float _remainTime = 0.0f;

protected:
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void SetData(const FString& InGuildName, int32 InEmblemId, bool bIsCreate);

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "GsUI")
	void StartAnimation();
};
