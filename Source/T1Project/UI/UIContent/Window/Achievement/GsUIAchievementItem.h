#pragma once
#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemReward.h"
#include "Achievement/GsAchievementData.h"
#include "GsUIAchievementItem.generated.h"

class UImage;
class UTextBlock;
class UGsWrapBoxIconSelector;
class UGsDynamicIconSlotHelper;
class UWidgetSwitcher;
class UGsUIIconBase;

UCLASS()
class T1PROJECT_API UGsUIAchievementItem : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _achievementIcon;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textLevel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textTitle;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textDesc;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsWrapBoxIconSelector* _rewardIconSelector;

	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//UGsUIProgressBar* _multiProgressBar;

	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//UTextBlock* _textProc;

	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//UGsButton* _btnRecv;

	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//UUserWidget* _tagComplete;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _completeSwitcher;

	UPROPERTY()
	UGsDynamicIconSlotHelper* _rewardIconSlotHelper;

protected:
	TWeakPtr<FGsAchievementData> _data;

protected:
	UFUNCTION()
	void OnCreateRewardIcon(UGsUIIconBase* InIcon);

	UFUNCTION()
	void OnRefreshRewardIcon(int32 InIndex, UGsUIIconBase* InIcon);

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

	void OnLongPressIcon(UGsUIIconItem& InIcon);

public:
	void SetData(TWeakPtr<FGsAchievementData> data) { _data = data;}
	virtual void RefreshUI();
	
};

