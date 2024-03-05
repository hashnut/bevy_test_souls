// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIHUD.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Quest/GsQuestData.h"
#include "GsUIHUDQuest.generated.h"

class UserWidget;
class FGsQuestHandler;
class FGsQuestInstance;
class UUserWidget;
class UGsUIHUDQuestSlot;
class UGsDynamicPanelSlotHelper;
class UScrollBox;
class UWrapBox;
/**
 * HUD 좌측 퀘스트 목록 UI
 */
UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIHUDQuest : public UGsUIHUD
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _scrollBoxEntryClass;

	// 스크롤헬퍼
	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _scrollBoxHelper;

	// 스크롤
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UScrollBox* _scrollBox;

	// 아래로 차례차례 추가
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UPanelWidget* _panelScrollRoot;

//	QuestId _activeQuestId;
	QuestKey _activeQuestKey;
	int32 _activeSlotIndex = -1;

private:
	TArray<FGsQuestInstance*> _questList;
	TArray<TWeakObjectPtr<UGsUIHUDQuestSlot>> _questSlotList;

protected:
	// UUserWidget override.
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

	// IGsInvalidateUIInterface overrde.
	virtual void InvalidateAllInternal() override;
	// UObject override
	virtual void BeginDestroy() override;

protected:
	// 스크롤 업데이트
	UFUNCTION()
	void OnRefreshEntryScrollBox(int32 InIndex, class UWidget* InEntry);

	// 메뉴 클릭
	UFUNCTION()
	void OnClickSlot(int32 InIndex, int32 InType);

	UFUNCTION()
	void OnClickTeleport(int32 InIndex);

private:
	void UpdateList();
	FGsQuestHandler* GetQuestHandler();

//----------------------------------------------------------------------------------------------------------------------
	// Tutorial
public:
	DECLARE_DELEGATE_OneParam(FOnTutorialQuestListEvent, UWidget* /*TargetWidget*/);
	FOnTutorialQuestListEvent OnTutorialQuestListEvent;

private:
	QuestId _tutorialQuestId = 0;
	bool _bTutorialHideTeleport = false;

private:
	UFUNCTION()
	void OnScrollQuestList(float InCurrentOffset);
	
	bool IsTutorial() const { return (0 < _tutorialQuestId) ? true : false; }
public:
	void SetTutorialTarget(QuestId InQuestId, bool bHideTeleport);
};
