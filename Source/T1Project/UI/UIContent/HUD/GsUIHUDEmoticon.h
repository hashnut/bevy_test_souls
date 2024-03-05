// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWidgetBase.h"
#include "GsUIHUDEmoticon.generated.h"

struct FGsSchemaEmoticonResData;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIHUDEmoticon : public UGsUIWidgetBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI")
	FSlateBrush _emoticonImg;	

	FVector _targetLocation;

protected:
	virtual void NativeDestruct() override;

public:
	virtual EGsUIHideFlags GetHideFlagType() const override { return EGsUIHideFlags::UI_HIDE_DIALOG; }
	virtual void Release();

public:
	void PlayEmoticon(const struct FGsSchemaEmoticonResData* InData, FVector InLocation);

	// 풀에서 해제(Inactive) 완료되었을때 호출. 씬 전환 시 타이머 등을 해제하기 위해 사용.
	void OnReleaseComplete();
	void SetTransformInter();

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnFinished();
};
