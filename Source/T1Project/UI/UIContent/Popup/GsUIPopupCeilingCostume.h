// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Popup/GsUIPopupCeiling.h"
#include "Shared/Client/SharedEnums/SharedCostumeEnum.h"
#include "GsUIPopupCeilingCostume.generated.h"

struct FGsSchemaCostumeCeiling;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIPopupCeilingCostume : public UGsUIPopupCeiling
{
	GENERATED_BODY()

private:
	TMap<int32, CostumeGrade> _tabGradeIndex;
	TArray<const FGsSchemaCostumeCeiling*> _ceilingData;

	const FGsSchemaCostumeCeiling* _selectData{ nullptr };

protected:
	UFUNCTION()
	void OnClickCellingProbability();

protected:
	virtual void NativeOnInitialized() override;

	virtual void OnSelectedTab(int32 index) override;
	virtual void OnStartSummonCeiling() override;

protected:
	void SetSelectedGrade(CostumeGrade grade, bool refresh);
	bool CheckEnableCeiling();
};
