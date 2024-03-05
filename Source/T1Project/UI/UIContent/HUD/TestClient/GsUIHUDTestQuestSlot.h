// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "UI/UILib/Manager/GsWidgetPoolManager.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "GsUIHUDTestQuestSlot.generated.h"

class UWidgetSwitcher;
class UGsDynamicPanelSlotHelper;
class UGsUIVFX;

/**
 *  Äù½ºÆ® ½½·Ô
 */
UCLASS()
class T1PROJECT_API UGsUIHUDTestQuestSlot :public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()	
	
public:
	DECLARE_DELEGATE_OneParam(FOnClickTeleport, UGsUIHUDTestQuestSlot*);
	DECLARE_DELEGATE_OneParam(FOnClickSlot, UGsUIHUDTestQuestSlot*);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _objectiveSlotClass;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelper;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _bgSwicher;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _questTypeSwicher;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _questStateSwicher;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWrapBox* _progressingWrapBox;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textTitle;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textContent;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UBorder* _activeEffect;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIVFX* _touchEffect;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _teleportBtn;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _slotBtn;

	UPROPERTY()
	int8 _index = 0;

	UPROPERTY()
	bool _bIsSelected = false;
	
	
public: 
	FOnClickSlot _delegateClickSlot;
	FOnClickTeleport _delegateClickTeleport;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;
	// UObject override
	virtual void BeginDestroy() override;

public:
	virtual void OnShow() override;

protected:
	UFUNCTION()
	void OnRefreshEntry(int32 InIndex, class UWidget* InEntry);

	UFUNCTION()
	void OnClickSlot();
	UFUNCTION()
	void OnClickTeleport();

public:
	void Update(int8 inIndex);
	void SetIsSelected(bool bInIsSelected);
	bool GetIsSelected() const;

	void SetTitleText(FText inText);
	FText& GetTitleText();
	void SetQuestCategoryStyle(QuestType inType);
	void SetQuestComplete(bool inIsComplete);
	void SetTitleTextColor();
	void SetVisibilityTeleportBtn(bool inIsTeleportAble);
};
