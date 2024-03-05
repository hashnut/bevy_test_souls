// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Popup/GsUIPopupCeiling.h"
#include "Shared/Client/SharedEnums/SharedFairyEnum.h"
#include "GsUIPopupCeilingFairy.generated.h"

struct FGsSchemaFairyCeiling;

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIPopupCeilingFairy : public UGsUIPopupCeiling
{
	GENERATED_BODY()

private:
	TMap<int32, FairyGrade> _tabGradeIndex;	
	TArray<const FGsSchemaFairyCeiling*> _ceilingData;

	const FGsSchemaFairyCeiling* _selectData{ nullptr };

protected:
	UFUNCTION()
	void OnClickCellingProbability();

protected:
	virtual void NativeOnInitialized() override;

	virtual void OnSelectedTab(int32 index) override;
	virtual void OnStartSummonCeiling() override;

protected:
	void SetSelectedGrade(FairyGrade grade, bool refresh);
	bool CheckEnableCeiling();
};
