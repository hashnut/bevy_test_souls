// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "UI/UILib/Manager/GsWidgetPoolManager.h"
#include "Quest/GsQuestInstance.h"
#include "Quest/Table/GsQuestObjectiveTableCache.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "UTIL/GsIntervalChecker.h"
#include "GsUIHUDQuestSlot.generated.h"

class UWidgetSwitcher;
class UGsDynamicPanelSlotHelper;
class FGsQuestInstance;
class UGsUIVFX;
class UCanvasPanel;

/**
 *  Äù½ºÆ® ½½·Ô
 */
UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIHUDQuestSlot :public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()	
	
public:
	DECLARE_DELEGATE_OneParam(FOnClickTeleport, const int32);
	DECLARE_DELEGATE_TwoParams(FOnClickSlot, const int32, const int32);

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
	class UVerticalBox* _progressingVerticalBox;

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

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIVFX* _completeEffect;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIVFX* _renewalEffect;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIVFX* _newEffect;	
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIVFX* _rewardEffect;

	/*UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _windowQuestBtn;*/

	UPROPERTY()
	int8 _index = 0;

	UPROPERTY()
	bool _bIsSelected = false;

	UPROPERTY()
	int32 _objectiveNum = 0;

	// Quest»óÅÂ
	EGsQuestUIState _eState;
	// QuestInstance
	FGsQuestInstance* _questInstance = nullptr;

	StoryId _storyId = INVALID_STORY_ID;
	QuestKey _questKey = QuestKey::KeyNone();

protected:
	FGsIntervalChecker _checkerButton;
	
public: 
	FOnClickSlot _delegateClickSlot;
	FOnClickTeleport _delegateClickTeleport;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
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
	UFUNCTION()
	void OnClickWindowQuest();

	void CompleteEffect(bool bInValue);
	void RenewalEffect(bool bInValue);
	void NewEffect(bool bInValue);
	void RewardEffect(bool bInValue);
	void ClearEffect();

public:
	void Update(class FGsQuestInstance* inQuestInstance, int8 inIndex);
	void SetIsSelected(bool bInIsSelected);
	bool GetIsSelected() const;

	void SetQuestCategoryStyle(QuestType inType);
	void SetQuestComplete(bool inIsComplete);
	void SetTitleTextColor(QuestType inType, StoryId inStoryId);
	void SetVisibilityTeleportBtn(bool inIsTeleportAble);

	//------------------------------------------------------------------------------------------------------------------
	// Tutorial
protected:
	bool _bTutorialHideTeleport = false;
public:
	void SetTutorialHideTeleport(bool bHideTeleport);
};
