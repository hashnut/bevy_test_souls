#pragma once
#include "Message/MessageParam/GsMessageParam.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedFairyEnum.h"

struct  FGsFairyMsgParamInt : public IGsMessageParam
{
	int32 _data;

	FGsFairyMsgParamInt() = default;
	virtual ~FGsFairyMsgParamInt() = default;
	explicit FGsFairyMsgParamInt(int32 InData)
		: _data(InData)
	{
	}
};

/*
* 페어리 합성 결과
*/
struct  FGsFairyMsgParamObtainList : public IGsMessageParam
{
	FairyObtainReason _obtainReason;
	TArray<FairyId> _dataList;

	FGsFairyMsgParamObtainList() = default;
	virtual ~FGsFairyMsgParamObtainList() = default;
	explicit FGsFairyMsgParamObtainList(FairyObtainReason obtainReason, TArray<FairyId>& InDataList)
		: _obtainReason(obtainReason), _dataList(InDataList)
	{

	}
};

struct  FGsFairyMsgParamUpdateList : public IGsMessageParam
{
	TArray<FairyId> _dataList;

	FGsFairyMsgParamUpdateList() = default;
	virtual ~FGsFairyMsgParamUpdateList() = default;
	explicit FGsFairyMsgParamUpdateList(TArray<FairyId>& InDataList)
		: _dataList(InDataList)
	{

	}
};

struct  FGsFairyMsgParamFairySummonWait final : public IGsMessageParam
{
	enum SummonUpdateType { Update, Confirm, Change };
	FairyConfirmId _summonWaitDBId;
	SummonUpdateType _updateType;
	FairyId _changedFairy;

	FGsFairyMsgParamFairySummonWait() = default;
	virtual ~FGsFairyMsgParamFairySummonWait() = default;
	explicit FGsFairyMsgParamFairySummonWait(FairyConfirmId confirmID, SummonUpdateType type = SummonUpdateType::Update, FairyId changedFairy = INVALID_FAIRY_ID)
		: _summonWaitDBId(confirmID), _updateType(type), _changedFairy(changedFairy)
	{
	}
};
struct  FGsFairyMsgParamCollectionList : public IGsMessageParam
{
	enum CollectionUpdateType {Condition, Collection};
	TArray<CollectionId> _dataList;
	CollectionUpdateType _updateType;

	FGsFairyMsgParamCollectionList() = default;
	virtual ~FGsFairyMsgParamCollectionList() = default;
	explicit FGsFairyMsgParamCollectionList(TArray<CollectionId>& InDataList, CollectionUpdateType type = CollectionUpdateType::Collection)
		: _dataList(InDataList), _updateType(type)
	{

	}
};

struct  FGsFairyMsgParamEnchantResult : public IGsMessageParam
{
	FairyId _fairyId;
	FairyEnchantResult _result;

	FGsFairyMsgParamEnchantResult() = default;
	virtual ~FGsFairyMsgParamEnchantResult() = default;
	explicit FGsFairyMsgParamEnchantResult(FairyId fairyId, FairyEnchantResult result)
		: _fairyId(fairyId), _result(result)
	{

	}
};

struct  FGsFairyMsgParamChangeTab : public IGsMessageParam
{
	int32 _tab;
	int32 _optData;

	FGsFairyMsgParamChangeTab() = default;
	virtual ~FGsFairyMsgParamChangeTab() = default;
	explicit FGsFairyMsgParamChangeTab(int32 tabIndex, int32 InData = 0)
		: _tab(tabIndex), _optData(InData)
	{
	}
};