// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"

#include "GsUISpiritShotStatListItem.generated.h"

class UTextBlock;
class UWidgetSwitcher;

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUISpiritShotStatListItem : public UUserWidget
{
	GENERATED_BODY()

public:
	/** 스탯값 설정을 위한 파라미터 */
	struct Parameters
	{
		int32 _index;
		bool _isStatBefore;
	};

	/************************************************************************/
	/* 필요 위젯                                                            */
	/************************************************************************/
protected:
	/** 이전/이후 스탯 전환을 위한 위젯 스위쳐 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _statWidgetSwitcher;

	/** 이전 스탯 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _statBefore;

	/** 이후 스탯 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _statAfter;


	/************************************************************************/
	/* Overrides                                                            */
	/************************************************************************/
protected:
	void NativeOnInitialized() override;

	void NativeDestruct() override;


	/************************************************************************/
	/* Setter                                                               */
	/************************************************************************/
public:
	void SetParameters(Parameters InParameters);

};
