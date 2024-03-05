// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "GsUIGuildCalculateListEntry.generated.h"


class UTextBlock;
class UGsButton;
/**
 * 기사단 분배 페이지의 분배 목록 정보
 */
UCLASS()
class T1PROJECT_API UGsUIGuildCalculateListEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIRedDotBase* _uiRedDot;
	
	/* 정산일 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockDate;

	/* 획득 정보 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockHistory;

	/* 참여자 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnParticipant;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockParticipant;	

	/* 아이템 정보 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsHorizontalBoxIconSelector* _iconSelector;

	UPROPERTY()
	class UGsUIIconItemInventory* _uiIcon;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockItemName;

	/* 낙찰 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockBidderName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UPanelWidget* _panelPrice;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockPrice;

	/* 정산 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnResult;

	/* 상태 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockState;

protected:
	ItemDBId _calculateItemDBId = INVALID_ITEM_DB_ID;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

protected:
	UFUNCTION()
	void OnClickParticipant();

	UFUNCTION()
	void OnClickResult();

	void OnLongPressIcon(class UGsUIIconItem& InIcon);

};