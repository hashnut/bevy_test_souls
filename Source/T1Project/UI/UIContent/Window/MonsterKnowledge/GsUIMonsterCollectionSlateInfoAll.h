#pragma once

#include "CoreMinimal.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIMonsterCollectionSlateInfoAll.generated.h"

class UGsButton;
class UTextBlock;
class UScrollBox;
class UGsUITooltipDesc;
class UGsDynamicPanelSlotHelper;
class UGsUIMonsterCollectionSlateProgressEntry;

UCLASS()
class T1PROJECT_API UGsUIMonsterCollectionSlateInfoAll : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSlateResetAll;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textSlateUnLockCount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textSlateAllCount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textNodeCompletedCount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textNodeAllCount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBoxStatSet;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMonsterCollectionSlateProgressEntry* _slateProgressAttack;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMonsterCollectionSlateProgressEntry* _slateProgressDefence;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMonsterCollectionSlateProgressEntry* _slateProgressHit;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMonsterCollectionSlateProgressEntry* _slateProgressResitance;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMonsterCollectionSlateProgressEntry* _slateProgressSurvival;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMonsterCollectionSlateProgressEntry* _slateProgressAmplification;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperStatSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfStatEntry;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnTooltip;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUITooltipDesc* _tooltipSlateAll;

protected:
	TArray<TPair<StatType, FText>> _statInfoSet;
	TMap<StatType, TPair<StatValue, StatValue>> _statValueSet;

	bool _isResetEnable = false;

public:
	virtual void BeginDestroy() override;
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
protected:
	UFUNCTION()
	void OnClickedResetAll();

	UFUNCTION()
	void OnRefreshStatSet(int32 InIndex, UWidget* InEntry);

	UFUNCTION()
	void OnTooltip();

public:
	void BuildView();
	void Invalidate();
};
