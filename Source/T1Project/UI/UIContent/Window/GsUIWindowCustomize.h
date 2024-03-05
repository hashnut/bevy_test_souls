// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "UI/UIContent/Window/Customize/GsUICustomizeBase.h"
#include "UI/UIContent/Window/Customize/GsCustomizeUIData.h"
#include "GsUIWindowCustomize.generated.h"

class UPanelWidget;
class UWidgetSwitcher;
class UGsButton;
class UGsSwitcherButton;
class UGsUICustomizePreset;
class FGsCustomizeHandler;
class UTextBlock;
class UVerticalBox;


/**
 * 외형변경 UI
 */
UCLASS()
class T1PROJECT_API UGsUIWindowCustomize : public UGsUIWindow
{
	GENERATED_BODY()

public:
	enum EGsMainMenu
	{
		Face = 0,
		Hair,
		Eyes,
		Body,
		Tattoo,

		Max
	};

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelEquip;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _sBtnFace;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _sBtnHair;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _sBtnEyes;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _sBtnBody;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _sBtnTattoo;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUICustomizePreset* _contentPreset;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherContent;	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUICustomizeFace* _contentFace;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUICustomizeHair* _contentHair;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUICustomizeEyes* _contentEyes;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUICustomizeBody* _contentBody;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUICustomizeTattoo* _contentTattoo;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnFinish;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnBack;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnChangeCustomizing;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancelChangeCustomizing;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherCustomizingMode;

	// 사전예약
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UVerticalBox* _verticalBoxReservationNickName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _reservationNickName;

protected:
	// Toggle Group
	FGsToggleGroup _toggleGroupEquip;
	FGsToggleGroup _toggleGroupMainTab;
	TArray<class UGsUICustomizeBase*> _mainContentList;

	FGsCustomizeHandler* _customizeHandler = nullptr;
	FGsCustomizeUIData _selectedData;

	bool _bIsZoomIn = false;
	const TArray<struct FGsSchemaCustomizeSlotData>* _bodyList = nullptr;

public:
	// UObject override
	virtual void BeginDestroy() override;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	// UGsUIWidgetBase override
	virtual bool OnBack() override;

public:
	void SetData(int32 InMenuId, class FGsCustomizeHandler* InCustomizeHandler);
	
protected:
	void SetCustomizeHandler(class FGsCustomizeHandler* InCustomizeHandler);
	void SetMenu(int32 InMenuId = 0);
	void SetUIMainTab(EGsMainMenu InType);
	void SetZoom(bool bIsZoomIn, bool bInForced = false);

	void SetBodyPartsChange(int32 InBodyPartsId);
	UGsUICustomizeBase* GetCurrPage() const;

protected:
	UFUNCTION()
	void OnClickEquip(int32 InIndex);

	UFUNCTION()
	void OnClickMainTab(int32 InIndex);

	// 커스터마이징 완료
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickFinishCustomizing();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickChangeCustomizing();

	UFUNCTION()
	void OnClickCancelChangeCustomizing();

	void OnSetPreset(int32 InIndex);
	void OnClickRandom();
	void OnClickFaceSlot();

protected:
	void OnCreateNickName(const FString& InNickName);
};
