// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsManager.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Management/GsScopeHolder.h"
#include "UTIL/GsIntervalChecker.h"

namespace PD
{
	namespace GC
	{
		struct PKT_GC_ACK_CONTINENT_TAX_READ;
	}
}

struct FGsContinentAccumTaxData
{
	//ContinentId _continentId = INVALID_CONTINENT_ID; // B1: 추후 필요 시 저장
	//GuildDBId _guildDBId = INVALID_GUILD_DB_ID; // B1: 추후 필요 시 저장
	Currency _accumGold = 0;
	Currency _accumDia = 0;

public:
	FGsContinentAccumTaxData() = default;
	~FGsContinentAccumTaxData() {}
};

/**
 * 공성전 매니저. 
 * 정식 개발 전까지는 누적 세금 집계 정보 관리 용도로 사용
 */
class T1PROJECT_API FGsTaxDataHandler final
{
public:
	struct FGsTaxPercent
	{
		int32 _systemTaxPercent = 0;
		int32 _defaultTaxPercent = 0;

		FGsTaxPercent(int32 InSystemTax, int32 InDefaultTax)
		{
			// 만분율 값이므로 100을 나눈다. 0.01을 곱하면 미묘하게 소수점이 나와서 나누기로 처리
			_systemTaxPercent = InSystemTax / 100;
			_defaultTaxPercent = InDefaultTax / 100;
		}

	public:
		int32 GetTotalPercent() const { return _systemTaxPercent + _defaultTaxPercent; }
	};


private:
	time_t _accumStartTime = 0;
	time_t _accumEndTime = 0;
	TArray<FGsContinentAccumTaxData> _accumTaxList;

	TMap<CurrencyType, FGsTaxPercent> _taxPercentMap;
	int32 _castleTaxPercent = 0;

	// 자주 보내지 않도록 막는 로직
	FGsIntervalChecker _sendChecker;

public:
	FGsTaxDataHandler() = default;
	~FGsTaxDataHandler();

public:	
	void Initialize();
	void Finalize();

public:
	void SetContinentTax(PD::GC::PKT_GC_ACK_CONTINENT_TAX_READ& InPacket);	
	void SendReqAccumTax(bool bResetCoolTime);

public:
	Currency GetAccumGold() const;
	Currency GetAccumDia() const;
	time_t GetAccumStartTime() const { return _accumStartTime; }
	time_t GetAccumEndTime() const { return _accumEndTime; }	
	
	int32 GetCastleTaxPercent() const { return _castleTaxPercent; }
	// T: 시스템세율 F: 기본세율
	int32 GetTaxPercent(CurrencyType InType, bool bIsSystem);
	int32 GetTotalTaxPercent(CurrencyType InType);
};

