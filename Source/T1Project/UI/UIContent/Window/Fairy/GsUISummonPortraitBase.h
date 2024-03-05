#pragma once
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Fairy/GsBasePortrait.h"
#include "GsUISummonPortraitBase.generated.h"

class UImage;
class UCanvasPanel;
class UWidgetSwitcher;
class UPaperSprite;

/**
 *
 */
UCLASS()
class T1PROJECT_API UGsUISummonPortraitBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _portraitImg;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _gradeBack;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _gradeSwitcher;

public:
	virtual void RefreshUI();

	virtual void ResetData() { _baseData.Reset(); }
	virtual void SetData(TWeakPtr<IGsBasePortrait> baseData, CreatureGenderType useGender = CreatureGenderType::ALL) 
	{ 
		_baseData = baseData; 
		_useGender = useGender;
	}

protected:
	void SetPortraiteImage(const FSoftObjectPath& path, bool masked = true);
	void SetGrade(ItemGrade grade);
	void SetIconImageSprite(UPaperSprite* InSprite);

protected:
	TWeakPtr<IGsBasePortrait> _baseData;
	CreatureGenderType _useGender{ CreatureGenderType::ALL };

};
