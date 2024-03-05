// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITray.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUITrayPKGuildNotifyTicker.generated.h"

class UImage;
class URichTextBlock;

UCLASS()
class T1PROJECT_API UGsUITrayPKGuildNotifyTicker : public UGsUITray
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	URichTextBlock* _textMessage;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _iconKillGuildEmblem;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _iconDeadGuildEmblem;	

	class UWidgetAnimation* _animationData = nullptr;

	FWidgetAnimationDynamicEvent _finishDelegate;

	TFunction<void(void)> _endCallback;

protected:
	virtual void NativeOnInitialized() override;

	void AnimationStart();

	UFUNCTION()
	void OnAnimationFinish();
public:
	virtual void BeginDestroy() override;

	void SetData(GuildEmblemId InKillerGuildEmblemdId, GuildEmblemId InDeadGuildEmblemdId, const FText& InKillerName, const FText& InDeadName
		, TFunction<void(void)> InEndCallback = nullptr);
};
