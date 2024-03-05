#pragma once
#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Message/GsMessageBoss.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "UTIL/GsTimeUtil.h"
#include "GsUIWindowWorldBossResult.generated.h"
UCLASS()
class T1PROJECT_API UGsUIWindowWorldBossResult : public UGsUIWindow
{
	GENERATED_BODY()

public:
	enum class EGsResultType : uint8
	{
		SUCCESS,
		FAIL,
	};

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _bossNameTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _clearFailTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UWidgetSwitcher* _titleWidgetSwitcher;	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UWidget* _rewardPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsUICurrencySlot* _rewardCurrency;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UWidgetSwitcher* _rewardWidgetSwitcher;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UListView* _rewardListView;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UWidgetSwitcher* _resultWidgetSwitcher;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsUIWorldBossClearPanel* _clearPanel;	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _remainTimeTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsButton* _leaveButton;	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UImage* _bossImage;
		
private:
	TArray<TPair<MessageBoss, FDelegateHandle>> _bossDelegates;

private:
	FDateTime _endTime;
	FGsDateTimer _dateTimer;

private:
	FText _remainTimeTextFormat;
	FText _clearResultTextFormat;
	FText _clearFailTextFormat;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeDestruct() override;

public:
	void InvalidBossImage(const struct FGsSchemaWorldBossData* inWorldBossData);
	void InvalidBossName(const struct FGsSchemaWorldBossData* inWorldBossData);
	void InvalidReward();
	void InvalidParticipateReward();
	void InvalidConsolationReward(const struct FGsSchemaWorldBossData* inWorldBossData);

public:
	UFUNCTION()
	void OnLeaveButton();
	void OnFail(const struct IGsMessageParam* inParam);
	void OnSuccess(const struct IGsMessageParam* inParam);
	void StartTimer();
	void UpdateTimer();
};