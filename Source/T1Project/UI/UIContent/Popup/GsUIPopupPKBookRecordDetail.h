// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "GsUIPopupPKBookRecordDetail.generated.h"

class UImage;
class UGsButton;
class UTextBlock;
class UVerticalBox;
class UHorizontalBox;
class UWidgetSwitcher;
class UGsUICurrencyButton;
class UGsDynamicPanelSlotHelper;

/**
 * 상태이상 상세 정보창
 */
UCLASS()
class T1PROJECT_API UGsUIPopupPKBookRecordDetail : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	// 2022/09/26 PKT - 닫기
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

	// 2022/09/26 PKT - main 이름
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockName;

	// 2022/09/26 PKT - 길드 마크
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _iconGuildEmblem;

	// 2022/09/26 PKT - 길드 이름
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockGuildName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UHorizontalBox* _horizontalBoxExpInfo;

	// 2022/09/26 PKT - 손실 경험치
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockPaneltyExp;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperContributorSetData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfContributorDataEntry;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UVerticalBox* _verticalBoxContributorSet;

	// 2022/09/26 PKT - 유저 정보 일때만 활성화( 대응 횟수 / 대응 시간 / 비용 버튼 의 그룹 )
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UVerticalBox* _verticalBoxStatePanel;
	
	// 2022/09/23 PKT - 0 : 횟수 1 : 이용 불가
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherActionCount;

	// 2022/09/26 PKT - 대응 횟수
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockLimitActionCount;

	// 2022/09/23 PKT - 0 : 횟수 1 : 시간 만료
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherActionTime;

	// 2022/09/26 PKT - 대응 시간
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockLimitActionTime;
	
	// 2022/09/26 PKT - Currenct Button(DIA / GOLD)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICurrencyButton* _btnButtonCurrency;

	// 2022/09/26 PKT - Record Data
	//const class FGsPKRecordData* _data = nullptr;
	// 2023/11/14 PKT - "https://jira.com2us.com/jira/browse/C2URWQ-5233"이슈로 pointer에서 id로 정보 변경
	PKRecordId _targetId = INVALID_PK_RECORD_ID;

	bool _isActionTimeOver = false;

protected:
	virtual void NativeOnInitialized() override;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void OnClose();

	UFUNCTION()
	void OnClickedAction();

	UFUNCTION()
	void OnRefreshEntryContributorData(int32 InIndex, UWidget* InEntry);

protected:
	void SetMainContributorData(const class FGsPKRecordData* InData);
	void SetCurrecntBtn(const class FGsPKRecordData* InData);

	void SetTextActionCount(int32 InRemaindCount);
	void SetTextActionTime(const time_t& InOccurrenceTime);

	void SetDisableCurrecntBtn(bool InIsDisable);

	// 2022/09/27 PKT - 대응을 할 수 있는 상황인가?
	bool IsActionEnableState() const;	

public:
	void SetData(PKRecordId InRecordId);
	void Invalidate();

	PKRecordId GetPKRecordId() const;

	// 2022/10/20 PKT - KeyBoard Mapping
protected:
	virtual void OnInputCancel();
};
