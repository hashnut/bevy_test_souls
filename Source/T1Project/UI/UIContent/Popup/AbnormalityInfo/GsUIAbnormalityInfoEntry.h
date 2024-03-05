// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedPassivityEnum.h"
#include "GsUIAbnormalityInfoEntry.generated.h"

struct FGsAbnormalityData;
struct FGsPassivityData;

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIAbnormalityInfoEntry : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsHorizontalBoxIconSelector* _iconSelector;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnSlot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textBlockName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textDescription;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textRemainTime;

protected:
	float _remainCheckTime = 0.f;
	double _endTime = 0;
	float _maxRemainTime = 0.f;

	int32 _tableId = 0; // AbnormalityId, PassivityId
	int _level = 1;
	const FGsPassivityData* _passivityData = nullptr;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	virtual void OnHide() override;

public:
	void SetData(const FGsAbnormalityData* InData);
	void SetData(const FGsPassivityData* InData);

protected:
	UFUNCTION()
	void OnClickSlot();

protected:
	void UpdateRemainTimeText();
	class UGsUIIconAbnormality* GetIcon();
};
