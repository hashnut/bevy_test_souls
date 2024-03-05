// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedMarketEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"

/**
 * 거래소 패킷 Send 함수 모음
 */
class T1PROJECT_API FGsNetSendServiceMarket
{
	//------------------------------------------------------------------------------------------------------------------
	// Client -> Gateway

	// 거래소
public:
	// 아이템 그룹 목록 요청(마지막으로 받은 ItemId 넣어서 보낼 것)
	static void SendReqSalesItemList(bool bIsWorldMarket, MarketCategory InCategory, const TArray<ItemGrade> InGradeList,
		ItemId InLastItemId, bool bInShowBlockUI);
	// 쌓을 수 없는 아이템(예: 무기) 리스트 요청
	static void SendReqTransactionListInstackable(bool bIsWorldMarket, ItemId InItemId, time_t InLastUpdateTime);
	// 쌓을 수 있는 아이템(예: 주문서) 리스트 요청
	static void SendReqTransactionListStackable(bool bIsWorldMarket, ItemId InItemId, time_t InLastUpdateTime);
	// 정산 목록 요청
	static void SendReqCalculateList(bool bIsWorldMarket);
	// 판매 목록 요청
	static void SendReqMyTransactionList(bool bIsWorldMarket);
	// 가격 정보 요청(최근 30일간의 가격정보)
	static void SendReqItemPrice(bool bIsWorldMarket, ItemId InItemId, Level InEnchantLevel);
	// 검색 요청
	static void SendReqItemSearch(bool bIsWorldMarket, const TArray<ItemId>& InItemIdList);
	// 아이템 상세정보 요청
	static void SendReqTransactionDetailEquip(bool bIsWorldMarket, MarketTransactionId InTransactionId);
	// 즐겨찾기 리스트 요청
	static void SendReqBookmarkList(bool bIsWorldMarket);
	// 즐겨찾기 추가
	static void SendReqBookmarkRegister(bool bIsWorldMarket, ItemId InItemId);
	// 즐겨찾기 해제
	static void SendReqBookmarkDeregistered(bool bIsWorldMarket, ItemId InItemId);
	// 정산내역 목록 요청
	static void SendReqCalculatedList(bool bIsWorldMarket, MarketTransactionId InTransitionId, 
		time_t InLastSalesCompletedTime, bool bInShowBlockUI);

	// 컬렉션 거래소 아이템 최저가 문의
	static void SendReqCollectionItemMarketPriceLowest(bool bInIsPricePerUnit, CollectionId InCollectionId,
		TArray<struct FGsMarketLowestPriceRequestInfo>& InDataList);
	
	// 누적 세금 정보
	static void SendReqContinentTax();

	//------------------------------------------------------------------------------------------------------------------
	// Client -> Server
public:
	// 구매 요청
	static void SendReqTransactionPurchase(bool bIsWorldMarket, MarketTransactionId InTransactionId);
	static void SendReqTransactionPurchase(bool bIsWorldMarket, TArray<MarketTransactionId>& InTransactionIdList);
	// 판매 등록 요청
	static void SendReqTransactionRegister(bool bIsWorldMarket, ItemDBId InItemDBId, ItemAmount InAmount, MarketPrice InPrice);
	// 판매 취소 요청
	static void SendReqTransactionCancel(bool bIsWorldMarket, MarketTransactionId InTransactionId);
	// 정산 요청
	static void SendReqCalculate(bool bIsWorldMarket, bool bIsAll, MarketTransactionId InTransactionId);
};
