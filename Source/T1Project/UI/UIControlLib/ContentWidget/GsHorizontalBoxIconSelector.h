// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/HorizontalBox.h"
#include "UI/UIControlLib/Interface/GsIconSelectorInterface.h"
#include "UI/UIControlLib/Define/GsIconDefine.h"
#include "UI/UIControlLib/Define/EGsIconType.h"
#include "GsHorizontalBoxIconSelector.generated.h"

/**
 * 아이콘 생성 기능이 부여된 UHorizontalBox
 */
UCLASS()
class T1PROJECT_API UGsHorizontalBoxIconSelector : 
	public UHorizontalBox, 
	public IGsIconSelectorInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "GsIcon")
	EGsIconType _iconType;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "GsIcon")
	uint8 _iconKey;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsIconEditor")
	int32 _previewCount = 0;
#endif

public:
	virtual void SynchronizeProperties() override;

	// IGsIconSelectorInterface override
	virtual class UGsUIIconBase* CreateIcon(EGsIconType InIconType, uint8 InIconKey) override;
	virtual class UGsUIIconBase* CreateIcon() override;
	virtual class UPanelWidget* GetPanel() override;
	virtual void WarmUp(int32 InWarmUpIconCount) override;
};
