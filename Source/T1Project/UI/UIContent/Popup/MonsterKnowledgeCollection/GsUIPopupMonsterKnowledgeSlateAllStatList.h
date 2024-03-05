#pragma once

#include "CoreMinimal.h"
#include "../Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "GsUIPopupMonsterKnowledgeSlateAllStatList.generated.h"

class UWrapBox;
class UGsButton;
class UTextBlock;
class UWidgetSwitcher;
class UGsDynamicPanelSlotHelper;
//class UGsUIMonsterKnowledgeSlateStatListEntry;

/**
 *
 */
UCLASS()
class T1PROJECT_API UGsUIPopupMonsterKnowledgeSlateAllStatList : public UGsUIPopup
{
	GENERATED_BODY()

	const static int32 VERTICAL_MAX_COUNT = 9;

protected:

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textTitle;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherStatList;	

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnResetAll;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOK;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWrapBox* _wrapBoxStatList;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textEmpty;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperStatList;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfStat;

	TArray<TPair<StatType, StatValue>> _statListAll;

protected:
	virtual void NativeOnInitialized() override;
	virtual void BeginDestroy() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnRefreshEntryStat(int32 InIndex, UWidget* InEntry);

	UFUNCTION()
	void OnClickedOK();

	UFUNCTION()
	void OnClickedReset();

	virtual void Refresh();

public:
	void Invalid();
};

