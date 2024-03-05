#pragma once
#include "CoreMinimal.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "DataSchema/Map/WorldMap/GsSchemaWorldMapSpoilsMonsterLevel.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "GsUIRegionSceneItemDropPanel.generated.h"


UCLASS()
class T1PROJECT_API UGsUIRegionSceneItemDropPanel : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
		TSubclassOf<class UUserWidget> _entryWidgetClass;
	UPROPERTY()
		class UGsDynamicPanelSlotHelper* _slotHelper;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UScrollBox* _scrollBox;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UPanelWidget* _panelScrollRoot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsButton* _closeButton;

private:
	FGsToggleGroup _toggleGroup;

private:
	TArray<FGsSchemaWorldMapSpoilsMonsterLevel> _dataArray;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	virtual void BeginDestroy() override;

public:
	void InvalidateAllInternal();

private:	
	UFUNCTION()
		void OnClickClose();
	UFUNCTION()
		void OnRefreshEntry(int32 InIndex, class UWidget* InEntry);

public:
	void SetMonsterData(const TArray<FGsSchemaWorldMapSpoilsMonsterLevel>& inData);
};