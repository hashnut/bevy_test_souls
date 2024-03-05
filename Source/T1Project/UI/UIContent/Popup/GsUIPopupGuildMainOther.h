// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UI/UILib/Base/GsUIPopup.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"

#include "Message/GsMessageContents.h"

#include "GsUIPopupGuildMainOther.generated.h"

class UGsUIGuildContentBase;
class UGsButton;
class UImage;
struct IGsMessageParam;

/**
 * 다른 길드 정보보기 화면 (UGsUIWindowGuildMain과 일부 위젯 공유 주의)
 */
UCLASS()
class T1PROJECT_API UGsUIPopupGuildMainOther : public UGsUIPopup
{
	GENERATED_BODY()

public:
	using MsgHandleUnlock = TPair<MessageContentUnlock, FDelegateHandle>;

	enum EGsGuildMainTab
	{
		Main = 0,
		Operation,

		All,
		Max = All
	};

protected:
	// 메인 탭
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelMainTab;

	// 페이지
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherContent;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIGuildContentMain* _contentMain;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIGuildContentOperation* _contentOperation;	

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnBack;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

protected:
	FGsToggleGroup _toggleGroupMainTab;
	TArray<UGsUIGuildContentBase*> _contentList;
	EGsGuildMainTab _tabPrev = EGsGuildMainTab::Max;

	TMap<EGsGuildMainTab, EGsUnlockContentsType> _unlockTypeMap;

	TArray<MsgHandleUnlock> _msgUnlockHandleList;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

private:
	void BindMessages();
	void UnbindMessages();

protected:
	virtual void OnInputCancel() override;

protected:
	// All 값을 넘기면 전부 업데이트
	void SetUICurrentContent(int32 InIndex);
	void CheckUnlock();

protected:
	void OnClickMainTab(int32 InIndex);
	void OnClickBlockedTab(int32 InIndex);

	UFUNCTION()
	void OnClickCloseAllStack();

	void OnContentsUnlock(const IGsMessageParam* InParam);

protected:
	UGsUIGuildContentBase* GetCurrentContent() const;
};