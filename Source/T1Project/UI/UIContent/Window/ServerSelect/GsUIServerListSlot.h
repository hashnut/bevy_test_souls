// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "GsUIServerListSlot.generated.h"

class UPanelWidget;
class UWidgetSwitcher;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIServerListSlot :
	public UUserWidget,
	public IGsToggleGroupEntry, 
	public IGsEntryInterface
{
	GENERATED_BODY()
public:
	DECLARE_DELEGATE_OneParam(FOnClickSlot, int32 /*InIndex*/);
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnNormal;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnSelected;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _textTempId;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UPanelWidget* _panelRecommend;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UPanelWidget* _panelNew;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UPanelWidget* _panelMyCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _SwitcherCongestion;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UPanelWidget* _panelCharCreatable;

	UPROPERTY()
	bool _bIsSelected = false;

	UPROPERTY()
	int32 _index = 0;

public:
	FOnClickSlot OnClickSlot;

public:
	// UUserWidget override
	virtual void NativeOnInitialized() override;

	// IGsToggleGroupEntry override
	virtual void SetIsSelected(bool bInIsSelected) override;
	virtual bool GetIsSelected() const override;

public:
	void SetData(int InIndex, const FString& InServerName, bool InSelected = false);
	void SetData(int InIndex, const FString& InServerName, uint16 InUserCount, uint8 InCongestion, bool InCharCreatable,
		bool InRecommended, bool InNewOpened, bool InSelected = false);

protected:
	UFUNCTION()
	void OnClickNormal();

	UFUNCTION()
	void OnClickSelected();
	
public:
	//UFUNCTION()
	//void SetSelected(bool bInSelected);

	//UFUNCTION()
	//bool GetIsSelected() const;

	UFUNCTION()
	void SetIndex(int InIndex) { _index = InIndex; }

	UFUNCTION()
	int32 GetIndex() const { return _index; }
};
