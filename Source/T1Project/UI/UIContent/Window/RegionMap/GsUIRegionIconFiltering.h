#pragma once
#include "CoreMinimal.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "GsUIRegionIconFiltering.generated.h"

/*
* region map icon filtering
*/

class UUserWidget;
class UVerticalBox;
class UWidget;
class UPanelWidget;

class UGsDynamicPanelSlotHelper;

UCLASS()
class UGsUIRegionIconFiltering : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryWidgetClass;
	// add vertical
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UVerticalBox* _verticalBox;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _scrollBoxHelper;


	// virtual function
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	virtual void NativeConstruct() override;
public:
	virtual void BeginDestroy() override;

	// logic function
public:
	UFUNCTION()
	void OnRefreshEntry(int32 InIndex, UWidget* InEntry);

	void InitList();
};