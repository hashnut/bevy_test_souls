// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Message/MessageParam/GsMessageParam.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Shared/SharedInclude/SharedDefine.h"

/**
 * MarketTransactionId, ItemId 등 uint64형 정보 전송
 */
struct FGsMarketMsgParamId : public IGsMessageParam
{
	uint64 _id = 0;

public:
	FGsMarketMsgParamId() = default;
	explicit FGsMarketMsgParamId(uint64 InId)
		: _id(InId)
	{
	}
	virtual ~FGsMarketMsgParamId() = default;
};

/**
 * 검색에 이용
 */
struct FGsMarketMsgParamString : public IGsMessageParam
{
	FString _str;

public:
	FGsMarketMsgParamString() = default;
	explicit FGsMarketMsgParamString(const FString& InStr)
		: _str(InStr)
	{
	}
	virtual ~FGsMarketMsgParamString() = default;
};

/**
 * 구매결과
 */
struct FGsMarketMsgParamPurchaseResult : public IGsMessageParam
{
	uint64 _id = 0;
	int32 _result = 0;

public:
	FGsMarketMsgParamPurchaseResult() = default;
	explicit FGsMarketMsgParamPurchaseResult(uint64 InId, int32 InResult)
		: _id(InId), _result(InResult)
	{
	}
	virtual ~FGsMarketMsgParamPurchaseResult() = default;

public:
	bool IsSuccess() const
	{
		return (PACKET_RESULT_SUCCESS == _result) ? true : false;
	}
};

/**
 * 
 */
struct FGsMarketMsgParamBool : public IGsMessageParam
{
	bool _data = false;

public:
	FGsMarketMsgParamBool() = default;
	explicit FGsMarketMsgParamBool(bool InData)
		: _data(InData)
	{
	}
	virtual ~FGsMarketMsgParamBool() = default;
};