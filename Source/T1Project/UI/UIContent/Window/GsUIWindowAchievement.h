#pragma once
#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "DataSchema/GsSchemaEnums.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "UI/UIContent/Common/GsUITabButtonCommon.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "GsUIWindowAchievement.generated.h"

class UImage;
class UGsSwitcherButton;
class UScrollBox;
class UPanelWidget;
class UWidgetSwitcher;
class UGsUIProgressBar;
class UGsUIVFX;
class UGsWrapBoxIconSelector;
class UGsDynamicPanelSlotHelper;
class UGsDynamicIconSlotHelper;
class UGsUIAchievementItemMain;
struct FGsAchievementData;

UCLASS()
class T1PROJECT_API UGsUIWindowAchievement : public UGsUIWindow
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnBack;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _tabScrollPanel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _tabScroll;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIAchievementItemMain* _mainAchievementItem;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnRecvRewardAll;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _listScrollPanel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _listView;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (DisplayName = "TabButtonWidget"))
	TSubclassOf<UUserWidget> _tabWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (DisplayName = "ListEntryWidget"))
	TSubclassOf<UUserWidget> _listEntryWidgetClass;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _tabScrollBoxHelper;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _itemScrollBoxHelper;

	FGsToggleGroup _toggleGroup;

	TArray< UGsUITabButtonCommon*> _tabButtons;
	int32 _curTabIndex{ 100 };

	TWeakPtr<FGsAchievementData> _selectMainAchievement;
	TArray<TWeakPtr<FGsAchievementData>> _selectAchievements;

	int32 MaxTab{ 0 };

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnReceive();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnReceiveAll();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnAchievementGroupList();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnSelectTab(int32 index);

	UFUNCTION()
	void OnCreateTabButtonEntry(UWidget* InEntry);
	UFUNCTION()
	void OnRefreshTabButtonEntry(int32 index, UWidget* InEntry);

	UFUNCTION()
	void OnCreateListEntry(UWidget* InEntry);
	UFUNCTION()
	void OnRefreshListEntry(int32 index, UWidget* InEntry);

public:
	struct UIInitData { const int nodata = 0; };

	virtual void InitializeData(UIInitData* initParam = nullptr);

	void OnUpdateAchievementItem(TWeakPtr<FGsAchievementData> achievementData);
	void OnAckReceiveAchievementReward(AchievementId id);
	void OnAckReceiveAchievementRewardAll(const TArray<AchievementId>& achievementIds); //const TArray<AchievementThemeId>& themeIds, 
	void OnUpdateRedDot();

	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void OnClickBack() override;
	virtual void OnClickCloseAllStack() override;

	virtual bool SyncContentState() override;

	void OnLongPressIcon(UGsUIIconItem& InIcon);

protected:
	void RefreshAchievementItem(int32 tabIndex);

private:
	void RefreshMainAchievement(TWeakPtr<FGsAchievementData> mainAchievement);
	void RefreshAchievements(const TArray<TWeakPtr<FGsAchievementData>>& list);
};

