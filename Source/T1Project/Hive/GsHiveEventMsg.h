#pragma once

#include "Hive/GsHiveEventEnum.h"
#include "Message/MessageParam/GsMessageParam.h"

/**
 * 하이브 관련 유저 추적용 이벤트 메시지
 */

template <typename TActionEnum>
class T1PROJECT_API FGsEventMsgBase : public IGsMessageParam
{
	static_assert(TIsEnumClass<TActionEnum>::Value, "TActionEnum is not enumerator type");

public:
	TActionEnum actionType;	// 이벤트 종류

public:
	FGsEventMsgBase(TActionEnum InActionType) : actionType(InActionType)
	{
	}
};

template <typename TActionEnum>
class T1PROJECT_API FGsHiveEventMsgBase : public FGsEventMsgBase<TActionEnum>
{
	static_assert(TIsDerivedFrom<TActionEnum, EGsHiveActionType>::IsDerived, "TActionEnum must derive from EGsHiveActionType");

public:
	int64 id;							// 상품, 출석부, 단발성 이벤트의 id 값
	int32 ordinalCount;					// 출석부의 경우, '몇 번째'

public:
	FGsHiveEventMsgBase(TActionEnum InActionType, int64 InId) : FGsEventMsgBase<TActionEnum>(InActionType), id(InId)
	{
	}

	FGsHiveEventMsgBase(TActionEnum InActionType, int64 InId, int32 InOrdinalCount) : FGsEventMsgBase<TActionEnum>(InActionType), id(InId), ordinalCount(InOrdinalCount)
	{
	}
};

// 출석부, 특수 이벤트 등 처리
using FGsHiveEventMsg = FGsHiveEventMsgBase<EGsHiveActionType>;


// 상품 구매 관련
template <typename TActionEnum>
class T1PROJECT_API FGsHiveEventPurchaseMsgBase : public FGsEventMsgBase<TActionEnum>
{
	static_assert(TIsDerivedFrom<TActionEnum, EGsHiveActionType>::IsDerived, "TActionEnum must derive from EGsHiveActionType");

public:
	TArray<TPair<uint32, CurrencyType>> productIdCurrencyList;		// 구매된 상품 ID 리스트

public:
	FGsHiveEventPurchaseMsgBase(TActionEnum InActionType, TArray<TPair<uint32, CurrencyType>>&& InProductIdCurrencyList) : FGsEventMsgBase<TActionEnum>(InActionType), productIdCurrencyList(Forward<TArray<TPair<uint32, CurrencyType>>>(InProductIdCurrencyList))
	{
	}
};

// 상품 구매 관련 처리
using FGsHiveEventPurchaseMsg = FGsHiveEventPurchaseMsgBase<EGsHiveActionType>;



/**
 * 하이브 계정 관련 메시지
 * HiveManager 에 url 값 저장하는 방식으로 변경하여 사용 안함
 */
//class T1PROJECT_API FGsHiveUrlMsg : public IGsMessageParam
//{
//public:
//	FString _playerImageUrl;
//
//	explicit FGsHiveUrlMsg(FString&& InPlayerImageUrl) : _playerImageUrl(Forward<FString>(InPlayerImageUrl))
//	{
//	}
//
//	explicit FGsHiveUrlMsg(std::string&& InPlayerImageUrl) : _playerImageUrl(FString(InPlayerImageUrl.c_str()))
//	{
//	}
//};
