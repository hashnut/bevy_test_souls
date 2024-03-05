#pragma once
#include "CoreMinimal.h"

#include "UI/UILib/Base/GsUIWindow.h"

#include "Shared/Client/SharedEnums/SharedCostumeEnum.h"
#include "Shared/Client/SharedEnums/SharedFairyEnum.h"

#include "Message/GsMessageFairy.h"
#include "Message/GsMessageCostume.h"

#include "GsUIWindowRecompose.generated.h"


class UGsButton;
class UTextBlock;
class UPanelWidget;
class UGsDynamicPanelSlotHelper;
/*
 * 합성강화 복구 시스템
 * 페어리/코스튬 재합성 Window
 */
UCLASS()
class T1PROJECT_API UGsUIWindowRecompose : public UGsUIWindow
{
	GENERATED_BODY()

public:
	using MsgFairyHandle = TPair<MessageFairy, FDelegateHandle>;
	using MsgCostumeHandle = TPair<MessageCostume, FDelegateHandle>;

protected:
	const FTextKey DELIVERY_BOX_TEXT = TEXT("DeliveryBoxText");

	enum class RecomposeType
	{
		NONE,
		COSTUME,
		FAIRY,
	};

protected:
	// 상단 영역
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnBack;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockTitle;

	// 중간 등급 표시 영역
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelGradeList;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfGradeSlot;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperGradeSlot;

	// 하단 텍스트 및 버튼 영역
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockNotice;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnRecompose;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockRecompose;

protected:
	RecomposeType _recomposeType = RecomposeType::NONE;

	TWeakObjectPtr<UUserWidget> _selectedGradeSlot;

	TArray<CostumeGrade> _validCostumeGradeList;
	CostumeGrade _minCostumeGrade;
	CostumeGrade _maxCostumeGrade;

	TArray<FairyGrade> _validFairyGradeList;
	FairyGrade _minFairyGrade;
	FairyGrade _maxFairyGrade;

protected:
	TArray<MsgFairyHandle> _msgFairyHandleList;
	TArray<MsgCostumeHandle> _msgCostumeHandleList;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual bool SyncContentState() override;

public:
	void SetCostumeData();
	void SetFairyData();

protected:
	UFUNCTION()
	void OnRefreshGradeSlot(int32 InIndex, class UWidget* InSlot);

	UFUNCTION()
	void OnClickRecompose();

	UFUNCTION()
	void ChangeGradeSlotFocus(UUserWidget* InWidget);

	void OnSummonComplete(const IGsMessageParam* InParam);

public:
	void SetIsSelected(UUserWidget* InWidget, bool InIsSelected);
};

