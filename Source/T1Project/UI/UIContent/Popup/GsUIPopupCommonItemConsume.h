// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "GsUIPopupCommonItemConsume.generated.h"

class URichTextBlock;
class UTextBlock;
class UGsUIIconItemInventory;
class UGsUIIconItem;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIPopupCommonItemConsume : public UGsUIPopup
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget, MultiLine = true))
	class URichTextBlock*  _richTextContent;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget, MultiLine = true))
	class URichTextBlock* _underContentRichText;

	UPROPERTY(BlueprintReadOnly, Category = "GsIcon", meta = (BindWidget))
	class UGsHorizontalBoxIconSelector* _iconSelector;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget, MultiLine = true))
	class UTextBlock* _textItemCostAmount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget, MultiLine = true))
	class UTextBlock* _textLocalItemAmount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnNo;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnOK;

	// T: Yes, F: No
	TFunction<void(bool)> _buttonCallback;

protected:
	int32 _costItemId = 0;
	int64 _costItemAmount = 0;

protected:
	UPROPERTY()
	class UGsUIIconItemInventory* _uiIcon;

protected:
	virtual void NativeDestruct() override;
	virtual void NativeConstruct() override;

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickOk();
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickCancel();

protected:
	void OnClickItemIcon(class UGsUIIconItem& InIcon);

	void SetAmountCount(const int64 InCostItemAmount, const int64 localItemAmount);

public:
	void SetData(const FText& InContent, int32 InCostItemId, int64 InCostItemAmount,
		TFunction<void(bool)> InCallback = nullptr);

	void SetData(const FText& InContent, const FText& InUnderContent, int32 InCostItemId, int64 InCostItemAmount,
		TFunction<void(bool)> InCallback = nullptr);

	/**
	 * 기존 공통 방식은 대상 아이템이 없으면 내부적으로 메시지를 띄우고 팝업이 종료됨.
	 * 이 함수는 아이템이 없더라도 없는 상태로 보여준다.
	 */
	void SetDataNotCheckItem(const FText& InContent, const FText& InUnderContent, int32 InCostItemId, int64 InCostItemAmount
		, TFunction<void(bool)> InCallback = nullptr);

	/**
	 * Item GroupId를 검사(각인 비각인)하여 아이템 갯수를 출력 그외 기능은 'SetDataNotCheckItem'와 동일
	 */
	void SetDataNotCheckItemByGroupId(const FText& InContent, const FText& InUnderContent, int32 InDispayItemId, int64 InCostItemAmount
		, TFunction<void(bool)> InCallback = nullptr);


	void SetDataByGroupId(const FText& InContent, const FText& InUnderContent, int32 InDispayItemId, int64 InCostItemAmount
		, TFunction<void(bool)> InCallback = nullptr);

	// 2022/10/20 PKT - KeyBoard Mapping
protected:
	virtual void OnInputCancel() override;
};
