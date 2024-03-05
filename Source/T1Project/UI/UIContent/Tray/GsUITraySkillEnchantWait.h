// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITray.h"
#include "Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UTIL/GsIntervalChecker.h"
#include "GsUITraySkillEnchantWait.generated.h"

class UGsButton;
class FGsSkill;
class UGsUISkillEnchantProgress;
class UPanelWidget;

/**
 * 클릭하여 결과를 확인하라는 창
 */
UCLASS()
class T1PROJECT_API UGsUITraySkillEnchantWait : public UGsUITray
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnConfirm;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelWaitUI;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISkillEnchantProgress* _progressUI;

protected:
	FGsIntervalChecker _checkerEnchantLock;
	SkillId _skillId = 0;
	uint64 _soundId = 0;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	void SetData(SkillId InSkillId);
	
	// 패킷을 이미 보냈고 연출만 진행되는 상태
	bool IsProgressState() const;

protected:
	UFUNCTION()
	void OnClickConfirm();

	UFUNCTION()
	void OnProgressTimeFinished();

	void PlayEffectSound(const FString& InName);
	void StopEffectSound();

	bool GetSkillUpCostList(OUT TArray<ItemDBIdAmountPair>& OutList);
};
