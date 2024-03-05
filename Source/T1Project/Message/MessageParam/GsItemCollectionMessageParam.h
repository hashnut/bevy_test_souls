// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Message/MessageParam/GsMessageParam.h"
#include "Item/Collection/EItemCollectionEnumData.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"


struct FGsItemCollectionMessageParam : public IGsMessageParam
{
	CollectionId _filedId;

	explicit FGsItemCollectionMessageParam(CollectionId InFiledId) : _filedId(InFiledId)
	{
		/*None*/
	}

	virtual ~FGsItemCollectionMessageParam() = default;
};

struct FGsItemCollectionMessageOpenParam : public IGsMessageParam
{
	ItemId _itemTId = INVALID_ITEM_ID;
	FText _Name = FText::GetEmpty();

	FGsItemCollectionMessageOpenParam() = default;
	explicit FGsItemCollectionMessageOpenParam(ItemId InItemId, const FText& InItemName)
		: _itemTId(InItemId), _Name(InItemName) {/*None*/ }
	virtual ~FGsItemCollectionMessageOpenParam() = default;
};