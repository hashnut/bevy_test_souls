#pragma once

#include "CoreMinimal.h"
#include "MonsterKnowledgeCollection/MonsterKnowledgeCollectionTypeDefine.h"
#include "UI/UILib/Base/GsUITray.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUITrayMonsterKnowledgeSlateFilter.generated.h"


class UWrapBox;
class UGsButton;
class UUserWidget;
class UGsUICheckBoxCommonText;
class UGsDynamicPanelSlotHelper;

DECLARE_DELEGATE_OneParam(FOnApplySlateFilter, const FGsMonsterKnowledgeSlatFilterParam&);

UCLASS()
class T1PROJECT_API UGsUITrayMonsterKnowledgeSlateFilter : public UGsUITray
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnApply;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICheckBoxCommonText* _checkBoxGradeAll;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWrapBox* _panelGradeListRoot;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperGrade;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfGrade;
		

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICheckBoxCommonText* _checkBoxStatAll;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperStat;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWrapBox* _panelStatListRoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfStat;

public:
	FOnApplySlateFilter OnApplyFilter;

protected:
	void NativeOnInitialized() override;
	void BeginDestroy() override;
	void NativeConstruct() override;
	void NativeDestruct() override;

	UFUNCTION()
	void OnClose();

	UFUNCTION()
	void OnApply();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnCreateEntryGrade(UWidget* InEntry);
	
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnRefreshEntryGrade(int32 InIndex, UWidget* InEntry);
	
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnCreateEntryStat(UWidget* InEntry);

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnRefreshEntryStat(int32 InIndex, UWidget* InEntry);

	void OnClickedGradeAll(bool bIsChecked, int32 InIndexInGroup);
	void OnClickedGrade(bool bIsChecked, int32 InIndexInGroup);

	void OnClickedStatAll(bool bIsChecked, int32 InIndexInGroup);
	void OnClickedStat(bool bIsChecked, int32 InIndexInGroup);
};
