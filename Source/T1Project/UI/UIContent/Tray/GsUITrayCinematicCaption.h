// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITray.h"
#include "UI/UILib/Define/GsUIDefine.h"
#include "GsUITrayCinematicCaption.generated.h"

/**
 *  현재 위치한 지역명 출력 티커
 */
UCLASS()
class T1PROJECT_API UGsUITrayCinematicCaption : public UGsUITray
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class URichTextBlock* _richTextCaption;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class URichTextBlock* _richTextCaptionCenter;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	virtual EGsUIHideFlags GetHideFlagType() const override { return EGsUIHideFlags::UI_HIDE_SYSTEM_TRAY; }

public:
	void SetCaption(const FText& InText, EGsCinematicCaptionType InMode);
};
