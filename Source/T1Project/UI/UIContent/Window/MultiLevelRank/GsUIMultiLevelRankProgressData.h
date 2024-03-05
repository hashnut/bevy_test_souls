// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "UI/UIContent/Helper/GsRedDotChecker.h"
#include "GsUIMultiLevelRankProgressData.generated.h"


class UGsSwitcherButton;
class UWidgetSwitcher;
class UScrollBar;
class UImage;
class UGsUIRedDotBase;
class UTextBlock;

struct FGsMultiLevelRankData;

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIMultiLevelRankProgressData : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()

	DECLARE_DELEGATE(FOnClickRewarBoxIcon);

	enum class ProgressBarSwitcherIndex
	{
		Begin = 0
		, Middle
		, End
	};

	enum SectionPointIndex
	{
		Front = 0		
		, Back
		, Max
	};

	struct SectionPoint
	{
		uint64 _beginePoint = INDEX_NONE;
		uint64 _endPoint = INDEX_NONE;
		float _maxRateValue = 0.f;
	};

	/************************************************************************/
	/* BP Bind...                                                           */
	/************************************************************************/	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _progressBarSwitcher;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherSlotPoint;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textExpPercent;

	const FGsMultiLevelRankData* _rankData = nullptr;

	SectionPoint _point[SectionPointIndex::Max];


	/************************************************************************/
	/* override                                                             */
	/************************************************************************/
public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;

	/************************************************************************/
	/* Init                                                                 */
	/************************************************************************/
public:
	void Initialized(const FGsMultiLevelRankData* InRank);

	void InvalidateExp(uint64 InCurrentExp);
	void InvalidateExpProgressBar();

	const FGsMultiLevelRankData* GetData() const;
};
