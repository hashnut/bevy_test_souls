#pragma once
#include "Message/MessageParam/GsMessageParam.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCostumeEnum.h"

struct FGsMsgCostumeParamInt : public IGsMessageParam
{
	int32 _data;

	FGsMsgCostumeParamInt() = default;
	virtual ~FGsMsgCostumeParamInt() = default;
	explicit FGsMsgCostumeParamInt(int32 InData)
		: _data(InData)
	{
	}
};

/*
* 코스춤 합성 결과
*/
struct FGsCostumeMsgParamObtainList : public IGsMessageParam
{
	CostumeObtainReason _obtainReason;
	TArray<CostumeId> _dataList;

	FGsCostumeMsgParamObtainList() = default;
	virtual ~FGsCostumeMsgParamObtainList() = default;
	explicit FGsCostumeMsgParamObtainList(CostumeObtainReason obtainReason, TArray<CostumeId>& InDataList)
		: _obtainReason(obtainReason), _dataList(InDataList)
	{

	}
};

struct FGsMsgCostumeCeilingResult: public IGsMessageParam
{
	bool _success{ false };

	FGsMsgCostumeCeilingResult() = default;
	virtual ~FGsMsgCostumeCeilingResult() = default;
	explicit FGsMsgCostumeCeilingResult(bool success)
		: _success(success)
	{
	}
};

struct FGsCostumeMsgParamUpdateList : public IGsMessageParam
{
	TArray<CostumeId> _dataList;

	FGsCostumeMsgParamUpdateList() = default;
	virtual ~FGsCostumeMsgParamUpdateList() = default;
	explicit FGsCostumeMsgParamUpdateList(TArray<CostumeId>& InDataList)
		: _dataList(InDataList)
	{

	}
};

struct FGsCostumeMsgParamCostumeSummonWait final : public IGsMessageParam
{
	enum SummonUpdateType { Update, Confirm, Change };
	CostumeConfirmId _summonWaitDBId;
	SummonUpdateType _updateType;
	CostumeId _changedCostume;

	FGsCostumeMsgParamCostumeSummonWait() = default;
	virtual ~FGsCostumeMsgParamCostumeSummonWait() = default;
	explicit FGsCostumeMsgParamCostumeSummonWait(CostumeConfirmId confirmID
		, SummonUpdateType type = SummonUpdateType::Update
		, CostumeId changedCostume = INVALID_COSTUME_ID)
		: _summonWaitDBId(confirmID), _updateType(type), _changedCostume(changedCostume)
	{
	}
};

struct FGsCostumeMsgParamCollectionList : public IGsMessageParam
{
	enum CollectionUpdateType { Condition, Collection };
	TArray<CollectionId> _dataList;
	CollectionUpdateType _updateType;

	FGsCostumeMsgParamCollectionList() = default;
	virtual ~FGsCostumeMsgParamCollectionList() = default;
	explicit FGsCostumeMsgParamCollectionList(TArray<CollectionId>& InDataList, CollectionUpdateType type = CollectionUpdateType::Collection)
		: _dataList(InDataList), _updateType(type)
	{

	}
};

struct FGsCostumeMsgParamWearOn : public IGsMessageParam
{
	CostumeId _costumeId;
	bool _isHairOn;

	FGsCostumeMsgParamWearOn() = default;
	virtual ~FGsCostumeMsgParamWearOn() = default;
	explicit FGsCostumeMsgParamWearOn(CostumeId costumeId, bool isHairOn)
		: _costumeId(costumeId), _isHairOn(isHairOn)
	{

	}
};

struct FGsCostumeMsgParamAppearanceOn : public IGsMessageParam
{
	CostumeId _costumeId;
	bool _hairChange;
	bool _bodyChange;

	FGsCostumeMsgParamAppearanceOn() = default;
	virtual ~FGsCostumeMsgParamAppearanceOn() = default;
	explicit FGsCostumeMsgParamAppearanceOn(CostumeId costumeId, bool hairChange, bool bodyChange)
		: _costumeId(costumeId), _hairChange(hairChange), _bodyChange(bodyChange)
	{

	}
};

class FGsItem;
struct FGsCostumeMsgParamExchangeCostume : public IGsMessageParam
{
	const FGsItem* _selectedItem{ nullptr };
	FGsCostumeMsgParamExchangeCostume() = default;
	virtual ~FGsCostumeMsgParamExchangeCostume() = default;
	explicit FGsCostumeMsgParamExchangeCostume(const FGsItem* item)
		: _selectedItem(item)
	{
	}
};