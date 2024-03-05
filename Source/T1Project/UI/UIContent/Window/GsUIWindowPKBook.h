
#pragma once

#include "CoreMinimal.h"
#include "IDelegateInstance.h"
#include "Message/GsMessageContents.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "UI/UIContent/Helper/GsRedDotChecker.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "PKBook/GsPKBookEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h" 
#include "GsUIWindowPKBook.generated.h"


class UGsButton;
class UTextBlock;
class UScrollBox;
class UWidgetSwitcher;
class UGsToggleButton;
class UGsSwitcherButton;
class UGsDynamicPanelSlotHelper;


UCLASS()
class T1PROJECT_API UGsUIWindowPKBook : public UGsUIWindow
{
	GENERATED_BODY()

protected:
	// 2022/09/22 PKT - 뒤로 가기
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnBack;

	// 2022/09/22 PKT - 닫기
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherRecordList;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperRecordData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfRecordDataEntry;

	// 2022/09/22 PKT - View Record Set
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBoxRecordSet;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperDetectionTargetData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfDetectionTargetDataEntry;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBoxDetectionTargetSet;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherDetectionTarget;

	/**
	 * Filter
	 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsToggleButton* _toggleAll;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsToggleButton* _toggleKill;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsToggleButton* _toggleAssist;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsToggleButton* _toggleDead;

	// 2022/09/22 PKT - 기록 카운트 갯수 설명
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBoxDestPKCount;

	// 2022/09/22 PKT - Message Setting Button
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnPKMessageSetting;
	
	// 2022/09/22 PKT - Detection Target List Filter
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnCheckOnlyLogIn;

	// 2022/09/22 PKT - Detection Target Set Count
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockDetectionTargetCount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnAddBoundaryTarget;

	// 2022/09/22 PKT - record Data Set Filter Group
	FGsToggleGroup _toggleGroupRecordDataFilter;
	 
	// 2023/6/7 PKT - Close Delegate
	TPair<MessageContents, FDelegateHandle> _contentsDelegate;

	/************************************************************************/
	/* override                                                             */
	/************************************************************************/
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual bool SyncContentState() override;
public:
	virtual void BeginDestroy() override;

protected:
	void BuildRecordDataViewList(EPKBookFilter InFilter);
	void BuildTargetDetectionViewList(EPKDetectionTargetFilter InFilter);
	
	void RefreshDetailData();

public:
	void OnTryOenpPopupRecordDetail(PKRecordId InrecordId);
	void OenpPopupRecordDetail(const class FGsPKRecordData* InPKRecordData);	

protected:
	// 2022/09/22 PKT - Record Data
	UFUNCTION()
	void OnRefreshEntryRecordData(int32 InIndex, UWidget* InEntry);

	// 2022/09/22 PKT - Detection Target Data
	UFUNCTION()
	void OnRefreshEntryDetectionTarget(int32 InIndex, UWidget* InEntry);

	// 2022/09/22 PKT - Record Data Filter
	void OnSelectedBtnFilter(int32 InIndex);

	// 2022/09/22 PKT - Detection Target Filter Selected
	UFUNCTION()
	void OnSelectedDetectionFilter(bool bIsSelected);

	// 2022/09/22 PKT - Detection Target Filter Clicked
	UFUNCTION()
	void OnClickedDetectionFilter();

	UFUNCTION()
	void OnClcikedOpenPopupMessageSetting();

	UFUNCTION()
	void OnClcikedAddBoundaryTarget();

	void OnAddBoundaryTarget(WorldId InWorldId, const FString& InNickName);

	void OnCloseWindow();

public:
	void InvalidRecoredDataSet();
	void OpenRecordDetail(PKRecordId InRecordId);
	void InvalidRecordDetail(PKRecordId InRecordId);
	void InvalidDetectionTarget(UserDBId InUserDBId);
	void InvalidDetectionTargetSet();
};