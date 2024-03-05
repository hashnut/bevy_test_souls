#pragma once

#include "CoreMinimal.h"
#include "../UI/UILib/Base/GsUIPopup.h"
#include "../UI/UIControlLib/Control/GsToggleGroup.h"
#include "../MonsterKnowledgeCollection/GsMonsterKnowledgeData.h"
#include "GsUIPopupMonsterCollectionBookGradeRewardInfo.generated.h"

class UGsButton;
class UScrollBox;
class UTextBlock;
class UGsUITooltipDesc;
class UGsDynamicPanelSlotHelper;

UCLASS()
class T1PROJECT_API UGsUIPopupMonsterCollectionBookGradeRewardInfo : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollCategoryBtn;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperCategoryView;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfCategoryView;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollDetailInfo;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperDetailView;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfDetailViewEntry;


	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnTooltip;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUITooltipDesc* _tooltipGradeReward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GsUI", meta = (ToolTip = "Tooltip Life Time"))
	float _toolTipLifetime = 1.0f;

private:
	const struct FGsSchemaMonsterKnowledgeLevelSet* _data = nullptr;

protected:
	FGsToggleGroup _toggleGroupCategoryTab;

public:
	virtual void BeginDestroy() override;
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	UFUNCTION()
	void OnClose();
	
	UFUNCTION()
	void OnClickedTooltip();

	UFUNCTION()
	void OnRefreshDetailInfo(int32 InIndex, UWidget* InEntry);

	UFUNCTION()
	void OnSelectedCategoryTab(int32 InIndex);

public:	
	void Open();

protected:
	virtual void OnInputCancel() override;
};