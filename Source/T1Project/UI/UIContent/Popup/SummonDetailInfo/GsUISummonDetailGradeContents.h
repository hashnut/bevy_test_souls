// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "GsUISummonDetailGradeContents.generated.h"

class UWidgetSwitcher;
class UPanelWidget;
class UScrollBox;
class UGsDynamicPanelSlotHelper;
struct IGsBasePortrait;

struct FGsSummonChangeData
{
	TWeakPtr<IGsBasePortrait> data { nullptr };
	int64 probability{ 0 };
	float perProbability{ 0.f };
};
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUISummonDetailGradeContents : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _selectGrade;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _gradeName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FLinearColor _gradeColor;

	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//UScrollBox* _scrollPortrait;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _candidatePortraits;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _scrollBoxHelper;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (DisplayName = "CandidatePortrait"))
	TSubclassOf<UUserWidget> _entryPortrait;

private:
	CreatureGenderType _genderType{ CreatureGenderType::ALL };
	TArray<FGsSummonChangeData> _data;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnCreateSummonItemEntry(UWidget* InEntry);
	
	UFUNCTION()
	void OnRefreshSummonItemEntry(int32 index, UWidget* InEntry);

	void OnPopupCollection(TWeakPtr<IGsBasePortrait> data);

public:
	void SetData(ItemGrade grade, TArray<FGsSummonChangeData>& data);

};
