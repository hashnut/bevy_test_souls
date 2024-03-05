// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "GsUIWindowCharacterCreate.generated.h"

class UGsSwitcherButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIWindowCharacterCreate : public UGsUIWindow
{
	GENERATED_BODY()
public:
	//DECLARE_DELEGATE_OneParam(FOnClickEvent, const CreatureGenderType);
	DECLARE_DELEGATE_OneParam(FOnClickPrset, const int);
	DECLARE_DELEGATE(FOnClickCustomizing)

protected:
	// 무기선택
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnCustomizing;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnCustomizingCancel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnBack;

	// 프리셋 1
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnPreset1;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnPreset2;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnPreset3;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnPreset4;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textBtnCustomizing;
	
	int32 _selectedWeaponIndex = 0;
	int8 _selectedPresetIndex = 0;
	
public:
	FOnClickCustomizing _onClickCustomizing;
	FOnClickPrset _onSelectPreset;

protected:
	virtual void BeginDestroy() override;

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// UGsUIWidgetBase override
	virtual bool OnClickBackProc();
	virtual bool OnBack() override;

private:
	void ShowHideCustomizingBtn(bool inIsShow);

public:
	UFUNCTION()
	void OnClickCustomizing();

	UFUNCTION()
	void OnClickPreset1();

	UFUNCTION()
	void OnClickPreset2();

	UFUNCTION()
	void OnClickPreset3();

	UFUNCTION()
	void OnClickPreset4();

	UFUNCTION()
	void OnClickCancelChangeCustomizing();
};
