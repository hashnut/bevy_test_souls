// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"

class FGsCostElement;
class FGsCostPackage;
class FGsCostPackageMultiple;
struct FGsSchemaCurrencyData;
struct FGsSchemaTaxConfig;

/*
 * FGsCurrencyHelper
 * 각종 Currency static 함수 등록
 */

struct T1PROJECT_API FGsCurrencyHelper
{
private:
	static FSoftObjectPath GetCashCurrencyIconPath();

public:
	// 재화 타입에 따라 저장 위치가 다를 수 있으므로, 해당 정보 처리
	static Currency GetCurrencyAmount(CurrencyType InType);
	static Currency GetItemCurrencyAmount(const int32 InItemTid, bool bCheckLock = false);
	static Currency GetCurrencyMaxAmount(CurrencyType InType);

	static const FGsSchemaCurrencyData* GetCurrencyData(const CurrencyType InType);

	/** 재화의 로컬라이즈 텍스트 반환 */
	static FText GetCurrencyLocalizedText(const CurrencyType InType);
	static FSoftObjectPath GetCurrencyIconPath(const CurrencyType InType);

	// 재화 타입에 따라 저장 위치가 다를 수 있으므로, 해당 정보 처리 
	static void SetGuildAsset(CurrencyType InType, Currency InAmount, bool bSendMessage = true);

	// 재화량 체크. 내 보유량이 InRequireAmount 미만이면 false 리턴.
	// bInShowTicker가 True이면 재화부족에 대한 티커 출력
	static bool CheckCurrency(CurrencyType InType, Currency InRequireAmount, bool bInShowTicker = true);
	static bool CheckCurrency(uint32 InItemTid, Currency InRequireAmount, bool bInShowTicker = true);

	static FText MakeSupplyHyperLink(const CurrencyType InType);

	static FText MakeUtilizeHyperLink(const CurrencyType InType);

	// 재화 구매 UI로 보내기
	static bool TryToNavigate(const CurrencyType InType);
	static bool TryToNavigate(const ItemId InItemId);

	// 연결된 재화 구매 UI가 존재하는지 확인
	static bool CanTryToNavigate(const CurrencyType InType);
	static bool CanTryToNavigate(const ItemId InItemId);

	// 재화부족 확인/구매 유도팝업 열기
	// TODO: 구현
	static void OpenLackCurrencyPopup(CurrencyType InType, Currency InLackAmount, 
		bool bInUseLink = true, TFunction<void(bool)> InCallback = nullptr);
	static void OpenLackItemPopup(ItemId InItemId, ItemAmount InLackAmount, 
		bool bInUseLink = true, TFunction<void(bool)> InCallback = nullptr);
	static bool OpenLackCostElementPopup(const FGsCostElement& InCostElement,
		bool bInUseLink = true, TFunction<void(bool)> InCallback = nullptr);
	static bool OpenLackCostPackagePopup(const FGsCostPackage& InCostPackage,
		bool bInUseLink = true, TFunction<void(bool)> InCallback = nullptr);
	static bool OpenLackCostPackageMultiplePopup(const FGsCostPackageMultiple& InCostPackageMultiple,
		bool bInUseLink = true, TFunction<void(bool)> InCallback = nullptr);

	// 세금 관련
	static const FGsSchemaTaxConfig* GetTaxConfig();
	static int32 GetTotalTaxPercent(CurrencyType InType);
	static Currency GetTaxIncludedPrice(CurrencyType InType, Currency InAmount);
	static FText GetTextTotalTax(CurrencyType InType);
	static FText GetTextTaxInfo(CurrencyType InType);
	static bool IsTaxNpcType(NpcFunctionType InType);

};
