#pragma once
#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "UI/UILib/Define/GsUIDefine.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"

#include "GsUIWindowFairy.generated.h"

struct FGsFairyData;

class UGsUIPageFairy;
class UGsUIPageFairyCollection;
class UGsUIPageFairyCompose;
class UGsUIPageFairySummonConfirm;
class IGsSelectedPanelInterface;

class UGsSwitcherButton;
class UGsButton;
class UCanvasPanel;
class UImage;
class UWidgetSwitcher;

class FGsGameStateSummon;

UCLASS()
class T1PROJECT_API UGsUIWindowFairy: public UGsUIWindow
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnFairy;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnCompose;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnCollection;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnSummoned;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _redDotFairy;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _redDotCompose;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _redDotCollection;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _redDotSummon;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnBack;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _fairyBg;

	FGsToggleGroup _toggleGroupFunction;

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnFunctionTabChanged(int32 index);

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _tabSwitcher;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIPageFairy* _contentsFairy;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIPageFairyCollection* _contentFairyCollection;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIPageFairyCompose* _contentFairyCompose;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIPageFairySummonConfirm* _contentFairySummonComfirm;

	TMap<int32, IGsSelectedPanelInterface*> _calls;

	int32 _initTab{ 0 };

public:
	struct UIInitData
	{
		int32 _tabType = 0;
		int32 _initData = 0;
	};

	virtual void InitializeData(UIInitData* initParam = nullptr);
	virtual void ReleaseData();
	void ChangeInitTab(UIInitData* initParam = nullptr);

	// 자동이동 워프전 확인용
	// 페어리 window는 막음
	virtual bool GetPreventAutoClose() const override { return true; }

	void OnUpdateRedDot();
	void OnUpdateFairy(TArray<FairyId>& dataList);
	void OnUpdateFairyCollection(TArray<CollectionId>& dataList);

	void OnSummonComplete();
	void OnChangedSummonFairy();
	void OnEnchantFairyResult(FairyId id, FairyEnchantResult result);
	void OnCeilingFairyResult(FairyEnchantResult result = FairyEnchantResult::NONE);
	void OnObtainFairyResult(FairyObtainReason reason, TArray<FairyId>& dataList, bool isHideWindow);
	void OnUpdateSummonWaitingFairy(FairyConfirmId summonWaitDBId);
	void OnConfirmSummonWaitingFairy(FairyConfirmId summonWaitDBId);
	void OnFindFairy(FairyId id);

	void CloseProcess();

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual bool OnBack() override;

	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	virtual bool SyncContentState() override;

	//virtual void OnClickBack() override;
	virtual void OnClickCloseAllStack() override;

private:
	int32 _currentFunction = -1;	

public:
	bool _isOpend{ false };

	//------------------------------------------------------------------------------------------------------------------
	// 튜토리얼
public:
	DECLARE_DELEGATE_OneParam(FOnTutorialListEvent, UWidget*);
	FOnTutorialListEvent OnTutorialListEvent;

public:
	void SetTutorial(int32 InTargetIndex);

private:
	void OnTutorialSettingComplete(UWidget* InTarget);
};

