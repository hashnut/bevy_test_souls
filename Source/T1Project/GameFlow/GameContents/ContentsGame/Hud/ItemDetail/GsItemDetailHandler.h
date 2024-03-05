// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/GsItem.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageItem.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "UI/UIContent/Popup/GsUIPopupItemDetail.h"

/**
 * 
 */
class T1PROJECT_API FGsItemDetailHandler
{
private:
	TArray<TPair<MessageContentItem, FDelegateHandle>>	_listItemDetailDelegate;
	TArray<TPair<MessageItem, FDelegateHandle>>	_listItemDelegate;
	//TPair<MessageContentInven, FDelegateHandle> _invenDelegate;

public:
	FGsItemDetailHandler() = default;
	virtual ~FGsItemDetailHandler();

public:
	void Init();
	void Close();

private:
	void InitMessage();
	void RemoveMessage();

private:
	void OpenItemDetail(const struct IGsMessageParam* InMessageParam);
	void OpenItemDetailEx(const struct IGsMessageParam* InMessageParam);
	void OpenItemDetailBox(const struct IGsMessageParam* InMessageParam);
	void OpenItemDetailTID(const struct IGsMessageParam* InMessageParam);
	void OpenItemDetail_CharDetail(const struct IGsMessageParam* InMessageParam);
	void OpenCurrencyDetail(const struct IGsMessageParam* InMessageParam);
	void OpenItemDetailCompare(const struct IGsMessageParam* InMessageParam);
	void CloseDetail(const struct IGsMessageParam* InMessageParam);
	void UpdateItemDetail(struct FGsItemMessageParamBase& InParam);
	void RemoveItemMessage(struct FGsItemMessageParamBase& InParam);
	void RemoveProc(const FName& InWidgetKey, const uint64 InItemDBID);
	void OpenMarket(const struct IGsMessageParam* InMessageParam);
	void OpenItemDetailByItemCollcetion(const struct IGsMessageParam* InMessageParam);
	void SetPropertyText(const struct IGsMessageParam* InMessageParam);
	void OpenSummonDetailPopup(const struct IGsMessageParam* InMessageParam);
	void InvenPopupClose(struct FGsInvenMessageParamBase&);


};
