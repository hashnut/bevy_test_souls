// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Blueprint/UserWidget.h"
#include "Transform.h"
#include "Components/ScrollBox.h"
#include "GsUIBattlePassLevelHelper.generated.h"

class UPanelSlot;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIBattlePassLevelHelper : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TArray<UPanelSlot*> _scrollItems;

	UPROPERTY()
	TArray<UUserWidget*> _cachedWidgets;

	UPROPERTY()
	UScrollBox* _scrollBox;

public:
	UFUNCTION(BlueprintCallable)
	void InitScrollPanel(UScrollBox* scrollBox);

	UFUNCTION(BlueprintCallable)
	void ClearScrollPanel();

	UFUNCTION(BlueprintCallable)
	int32 AddScrollWidget(UUserWidget* widget);

	//UFUNCTION(BlueprintCallable)
	//int32 InsertScrollWidget(int32 insertIndex, UUserWidget* widget);

	UFUNCTION(BlueprintCallable)
	TArray<UUserWidget*> CashedAddScrollWidget(TSubclassOf<UUserWidget> InEntryClass, int count = 1, bool clear = true);

	UFUNCTION(BlueprintCallable)
	void RemoveScrollWidget(UUserWidget* widget);

	UFUNCTION(BlueprintCallable)
	void RemoveScrollWidgetByIndex(int index);

	UFUNCTION(BlueprintCallable)
	void ClearAllScrollWidgets();

	UFUNCTION(BlueprintCallable)
	UUserWidget* GetScrollWidget(int index);

	void GetAllScrollWidgets(OUT TArray<UUserWidget*>& widgets);

	template <typename WidgetT>
	void GetAllScrollWidgetsByUClass(OUT TArray<WidgetT*>& widgets, TSubclassOf<UUserWidget> UserWidgetClass = WidgetT::StaticClass())
	{
		static_assert(TIsDerivedFrom<WidgetT, UUserWidget>::IsDerived, "CreateWidget can only be used to create UserWidget instances. If creating a UWidget, use WidgetTree::ConstructWidget.");

		TArray<UWidget*> arr = _scrollBox->GetAllChildren();
		Algo::TransformIf(arr, widgets, [](auto e) { return Cast<WidgetT>(e) != nullptr; }, [](auto e) { return Cast<WidgetT>(e); });
	}
};
