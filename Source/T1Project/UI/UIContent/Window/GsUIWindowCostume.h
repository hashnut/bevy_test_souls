#pragma once
#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"

#include "GsUIWindowCostume.generated.h"

struct FGsFairyData;

class IGsSelectedPanelInterface;
class UGsUIPageCostume;
class UGsUIPageCostumeCollection;
class UGsUIPageCostumeCompose;
class UGsUIPageCostumeSummonConfirm;
class UGsUIPageCostumeAppearance;

class UGsSwitcherButton;
class UGsButton;
class UCanvasPanel;
class UImage;
class UWidgetSwitcher;

UCLASS()
class T1PROJECT_API UGsUIWindowCostume: public UGsUIWindow
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnCostume;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnCompose;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnCollection;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnSummonWait;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _redDotCostume;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _redDotCompose;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _redDotCollection;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _redDotDress;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnBack;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _costumeBg;

	FGsToggleGroup _toggleGroupFunction;

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnFunctionTabChanged(int32 index);

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _tabSwitcher;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIPageCostume* _contentsCostume;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIPageCostumeCollection* _contentsCollection;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIPageCostumeCompose* _contentsCompose;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIPageCostumeSummonConfirm* _contentsSummonWait;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIPageCostumeAppearance* _contentsAppearance;

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

	void ChangeInitTab(UIInitData* initParam);
	void OnUpdateRedDot();
	void OnUpdateCostume(TArray<CostumeId>& dataList);
	void OnUpdateCostumeCollection(TArray<CollectionId>& dataList);

	void OnSummonComplete();
	void OnChangeDressUp();
	void OnObtainCostumeResult(CostumeObtainReason reason, TArray<CostumeId>& dataList, bool isHideWindow);
	void OnUpdateSummonWaiting(CostumeConfirmId summonWaitDBId);
	void OnConfirmSummonWaiting(CostumeConfirmId summonWaitDBId);
	void OnFindCostume(CostumeId id);
	void OnCeilingCostumeResult(bool success);

	void CloseProcess();

public:
	// 자동이동 워프전 확인용
	// 코스튬 window는 막음
	virtual bool GetPreventAutoClose() const override { return true; }

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual bool OnBack() override;

	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	virtual bool SyncContentState() override;

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

