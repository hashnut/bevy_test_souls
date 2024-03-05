// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "Fairy/GsFairySummonConfirmData.h"
#include "Costume/GsCostumeSummonConfirmData.h"
#include "GsUIPopupSummonConfirmProbability.generated.h"

class UListView;
class UTextBlock;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIPopupSummonConfirmProbability : public UGsUIPopup
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UListView* _listView;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* _descText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _titleText;

public:
	void SetTitleDesc(FText title, FText desc);
	void SetFairyData(const TArray<FGsFairySummonChangeData>& _dataList);
	void SetCostumeData(const TArray<FGsCostumeSummonChangeData>& _dataList);

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickClose();

	// 2022/10/20 PKT - KeyBoard Mapping
protected:
	virtual void OnInputCancel() override;
};
