// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITray.h"
#include "UTIL/GsIntervalChecker.h"
#include "GsUITraySkillEnchantResult.generated.h"

class UGsButton;
class UTextBlock;
class UImage;
class UGsUISkillIcon;
class FGsSkill;

/**
 * 스킬강화 결과창
 *  - Tray_SkillEnchantResultSuccess
 *  - Tray_SkillEnchantResultFail
 */
UCLASS()
class T1PROJECT_API UGsUITraySkillEnchantResult : public UGsUITray
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GsUI")
	float _closeLockTime = 1.f;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnConfirm;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockTitle;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISkillIcon* _uiIcon;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockName;

protected:
	FGsIntervalChecker _checkerCloseLock;

protected:
	virtual void NativeOnInitialized() override;

public:
	void SetData(const FGsSkill* InSkill, bool bIsUpgrade, bool bInIsSuccess);

protected:
	UFUNCTION()
	void OnClickConfirm();
};
