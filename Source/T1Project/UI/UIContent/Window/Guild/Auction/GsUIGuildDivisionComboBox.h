// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "Shared/Client/SharedEnums/SharedGuildEnum.h"

#include "GsUIGuildDivisionComboBox.generated.h"


class UTextBlock;
class UPanelWidget;
class UGsSwitcherButton;
/**
 * 기사단 분배 페이지의 분배/정산 방식 콤보박스
 */
UCLASS()
class T1PROJECT_API UGsUIGuildDivisionComboBox : public UUserWidget
{
	GENERATED_BODY()

protected:
	/* 유형 - 분배 방식 / 정산 방식 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockType;

	// 
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnMenu;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockMenu;

	/* 목록 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelComboList;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnCombo1;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnCombo2;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnCombo3;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnCombo4;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockCombo1;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockCombo2;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockCombo3;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockCombo4;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockComboSelected1;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockComboSelected2;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockComboSelected3;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockComboSelected4;

public:
	DECLARE_DELEGATE(FOnChangeComboBox);
	FOnChangeComboBox OnChangeComboBox;

	DECLARE_DELEGATE(FOnClickComboBox);
	FOnClickComboBox OnClickComboBox;

protected:
	bool _isDivision = true;

	GuildAuctionType _divisionType = GuildAuctionType::NORMAL;
	GuildAuctionResultType _resultType = GuildAuctionResultType::NORMAL;

protected:
	virtual void NativeOnInitialized() override;

protected:
	UFUNCTION()
	void OnClickMenu();

	UFUNCTION()
	void OnClickCombo1();
	UFUNCTION()
	void OnClickCombo2();
	UFUNCTION()
	void OnClickCombo3();
	UFUNCTION()
	void OnClickCombo4();

	void OnClickDivision(GuildAuctionType InDivisionType);
	void OnClickResult(GuildAuctionResultType InResultType);

public:
	void SetDivisionType();
	void SetResultType();

	void Hide();

	void InvalidateComboBox();

	GuildAuctionType GetDivisionType() { return _divisionType; }
	GuildAuctionResultType GetResultType() { return _resultType; }

};