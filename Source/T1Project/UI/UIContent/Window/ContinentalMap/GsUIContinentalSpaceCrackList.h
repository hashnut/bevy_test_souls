#pragma once

#include "CoreMinimal.h"

#include "UI/UIControlLib/Control/GsToggleGroup.h"

#include "UMG/Public/Blueprint/UserWidget.h"

#include "Map/WorldMap/GsWorldMapDefine.h"

#include "GsUIContinentalSpaceCrackList.generated.h"

class UVerticalBox;
class UWidget;
class UTextBlock;

class UGsDynamicPanelSlotHelper;

// continental space crack list
UCLASS()
class T1PROJECT_API UGsUIContinentalSpaceCrackList : public UUserWidget
{
	GENERATED_BODY()

protected:
	// original bp class
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _continentalSpaceCrackListItemClass;

	// item attach target 
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UVerticalBox* _continentalSpaceCrackListVerticalBox;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _textSpawnInterval;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _textLastSpawnTime;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockLastSpawnTime;

	// item maker
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _continentalSpaceCrackListScrollBoxHelper;

	// group controller
	FGsToggleGroup _toggleGroupList;

private:
	// active info size
	int _currentActiveListSize = 0;
	// deactive info size
	int _currentDeactiveListSize = 0;


	// virtual func
protected:
	virtual void NativeOnInitialized() override;

public:
	virtual void BeginDestroy() override;

	// logic func
protected:
	UFUNCTION()
	void OnListRefreshEntry(int32 In_index, UWidget* In_entry);
	UFUNCTION()
	void OnClickList(int32 In_index);

public:
	void SetListData();
	// total index -> find type & data index
	void FindInfoByTotalIndex(int In_totalIndex, EGsContinentalSpaceCrackListItemType& Out_itemType, 
		int& Out_dataIndex);
};