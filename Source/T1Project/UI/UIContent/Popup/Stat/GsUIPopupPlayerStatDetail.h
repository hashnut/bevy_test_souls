// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "GsUIPopupPlayerStatDetail.generated.h"

/**
 *
 */
 UENUM()
 enum class EGsPlayerStatDetailType : uint8
 {
	Distribution,
	Detail,
 };

UCLASS()
class T1PROJECT_API UGsUIPopupPlayerStatDetail : public UGsUIPopup
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UPanelWidget* _panelList;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsButton* _closeButton;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UWidgetSwitcher* _panelWidgetSwitcher;

public:
	FGsToggleGroup _toggleGroup;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	void SetTab(EGsPlayerStatDetailType inTabType);

private:	
	void OnClickTab(int32 InIndex);
	UFUNCTION()
		void OnClickClose();

protected:
	virtual void OnInputCancel() override;
};
