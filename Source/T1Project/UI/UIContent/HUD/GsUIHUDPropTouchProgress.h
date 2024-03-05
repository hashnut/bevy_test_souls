// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIBillboard.h"
#include "../../UILib/Base/GsUIHUD.h"
#include "UI/UILib/Define/GsUIDefine.h"
#include "GsUIHUDPropTouchProgress.generated.h"

class UProgressBar;
class UTextBlock;

/**
 * 크리쳐 기본 빌보드. 위젯 컴포넌트에서 사용되는 객체.
 */
UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIHUDPropTouchProgress : public UGsUIHUD
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UTextBlock* _nameText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UTextBlock* _elapsedTimeText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UProgressBar* _interactionProgressbar;	

public:
	void SetName(const FText& inName);
	void SetTime(float inElapsed);
	void SetRatio(float inRatio);	
	virtual void SetHUDMode(EGsUIHUDMode inType) override;
	virtual void InvalidateAllInternal() override;
};
