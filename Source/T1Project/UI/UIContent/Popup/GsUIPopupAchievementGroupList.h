// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIPopupAchievementGroupList.generated.h"

class UGsButton;
class UScrollBox;
class UPanelWidget;
class UGsDynamicPanelSlotHelper;

struct FGsAchievementData;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIPopupAchievementGroupList : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _listScrollPanel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _listView;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (DisplayName = "ListEntryWidget"))
	TSubclassOf<UUserWidget> _listEntryWidgetClass;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _itemScrollBoxHelper;

	AchievementGroupId _groupId;
	TArray<TWeakPtr<FGsAchievementData>> _groupAchievements;

public:
	struct PopupInitData
	{
		AchievementGroupId groupId;
	};

protected:
	UFUNCTION()
	void OnCreateListEntry(UWidget* InEntry);

	UFUNCTION()
	void OnRefreshListEntry(int32 index, UWidget* InEntry);

	virtual void NativeOnInitialized() override;

public:
	virtual void BeginDestroy() override;

	void InitializeData(PopupInitData* param);
	void RefreshUI();

protected:
	virtual void OnInputCancel() override;
};
