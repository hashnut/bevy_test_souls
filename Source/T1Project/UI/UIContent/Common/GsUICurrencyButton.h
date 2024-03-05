// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Common/GsUICurrencySlot.h"
#include "Tutorial/GsTutorialInteractionInterface.h"
#include "GsUICurrencyButton.generated.h"


/**
 * 기본적인 재화 정보를 표시하는 버튼
 * - 재화량을 보여주는 버튼에 사용(예: 아이템, 스킬 강화하기 버튼)
 */
UCLASS()
class T1PROJECT_API UGsUICurrencyButton : public UGsUICurrencySlot, public IGsTutorialInteractionInterface
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE(FOnClickCurrencyButton);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnMain;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textButtonName;

public:
	FOnClickCurrencyButton OnClickCurrencyButton;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

public:
	void SetButtonName(const FText& InText);
	void SetOnClickLockTime(float InSec);
	void SetButtonEnable(bool bEnable);

protected:
	UFUNCTION()
	void OnClickMainButton();

//----------------------------------------------------------------------------------------------------------------------
		// Tutorial
private:
	bool _bIsTutorialInteraction = false;
	FGsOnTutorialInteraction OnTutorialInteraction;

public:
	virtual void SetIsTutorialInteraction(bool bIsTutorial) { _bIsTutorialInteraction = bIsTutorial; }
	virtual bool IsTutorialInteraction() const { return _bIsTutorialInteraction; }
	virtual FGsOnTutorialInteraction& GetTutorialInteraction() { return OnTutorialInteraction; }
};
