// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIHUD.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIHUDTestQuest.generated.h"

class UserWidget;
class UGsButton;
class FGsQuestHandler;
class FGsQuestInstance;
class UUserWidget;
class UGsUIHUDTestQuestSlot;
class UGsDynamicPanelSlotHelper;
class UGsButton;
class UScrollBox;
class UWrapBox;
struct IGsMessageParam;
/**
 * HUD 좌측 퀘스트 목록 UI
 */
UCLASS()
class T1PROJECT_API UGsUIHUDTestQuest : public UGsUIHUD
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnRefresh;

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
	class UWrapBox* _wrapBox;

	TWeakObjectPtr<UGsUIHUDTestQuestSlot> _selectedSlot = nullptr;

protected:
	// UUserWidget override.
	virtual void NativeOnInitialized() override;	

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
	void OnClickSlot(UGsUIHUDTestQuestSlot* inSelect);

	UFUNCTION()
	void OnClickTeleport(UGsUIHUDTestQuestSlot* inSelect);

	UFUNCTION()
	void OnClickRefrash();

private:
	void UpdateList();
	void OnQuestAutoMoveStop(const IGsMessageParam*);
public:
	void ClearSelect();



};
