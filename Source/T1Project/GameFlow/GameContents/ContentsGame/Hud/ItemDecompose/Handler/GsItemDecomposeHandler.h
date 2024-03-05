// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Message/GsMessageContents.h"


class FGsItem;
/**
 * 
 */
class T1PROJECT_API FGsItemDecomposeHandler
{
private:
	TArray<TPair<MessageContentItem, FDelegateHandle>>	_listItemDeComposeDelegate;

public:
	FGsItemDecomposeHandler() = default;
	virtual ~FGsItemDecomposeHandler();

public:
	void Init();
	void Close();

private:
	void RegisterMessage();
	void RemoveMessage();

private:
	void DecomposeSingleProc(const struct IGsMessageParam* InParamData);
	void DecomposeMultiProc(const struct IGsMessageParam* InParamData);
	void DecomposeMultiResult(const struct IGsMessageParam* InParamData);

private:
	void ClosePopup();

	bool IsValidDecompose(const FGsItem* InItem);

	void SortItemList(OUT TArray<uint64>& outItemDBIdList);

};
