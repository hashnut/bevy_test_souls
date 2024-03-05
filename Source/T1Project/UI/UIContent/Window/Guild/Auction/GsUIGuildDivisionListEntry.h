// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "GsUIGuildDivisionListEntry.generated.h"


class UGsUIRedDotBase;
class UTextBlock;
class UGsButton;
class UGsHorizontalBoxIconSelector;
class UGsUIIconItemInventory;
class UGsUIIconItem;
class UWidgetSwitcher;
class UPanelWidget;
class UGsSwitcherButton;
class UImage;
/**
 * 기사단 분배 페이지의 분배 목록 정보
 */
UCLASS()
class T1PROJECT_API UGsUIGuildDivisionListEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	/* 배경 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imageBackGround;

	/* 레드닷 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRedDotBase* _uiRedDot;

	/* 획득 정보 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockDate;

	/* 참여자 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnParticipant;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockParticipant;	

	/* 아이템 정보 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsHorizontalBoxIconSelector* _iconSelector;

	UPROPERTY()
	UGsUIIconItemInventory* _uiIcon;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockItemName;

	/* 상태 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelStateUp;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockStateUp;

	/* 체크 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnCheck;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherCheck;

protected:
	ItemDBId _divisionItemDBId = INVALID_ITEM_DB_ID;

	FText _remainTimeText;

	FLinearColor _colorNormal;
	FLinearColor _colorEnough;
	FLinearColor _colorNotEnough;

	FLinearColor _currentStateColor;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

protected:
	UFUNCTION()
	void OnClickParticipant();
	UFUNCTION()
	void OnClickCheck();

	void OnLongPressIcon(UGsUIIconItem& InIcon);

	void SetStateColor(const FLinearColor InColor);

public:
	void InvalidateAll();
	void InvalidateItem();
	bool CheckAndInvalidateState();
	void InvalidateCheck();
};