// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTaxDataHandler.h"
#include "DataSchema/Tax/GsSchemaTaxConfig.h"
#include "Shared/Shared/SharedPackets/PD_Gateway_Client.h"
#include "Net/GsNetSendServiceMarket.h"


FGsTaxDataHandler::~FGsTaxDataHandler()
{
	Finalize();
}

void FGsTaxDataHandler::Initialize()
{
	const FGsSchemaTaxConfig* row = nullptr;
	if (const UGsTable* table = FGsSchemaTaxConfig::GetStaticTable())
	{
		if (false == table->FindRow(TEXT("0"), row))
		{
			return;
		}
	}

	// 빠른 참조를 위해 만들어 둠
	_taxPercentMap.Empty();
	_taxPercentMap.Emplace(CurrencyType::DIA, FGsTaxPercent(row->diaSystemTaxRate, row->diaDefaultTaxRate));
	_taxPercentMap.Emplace(CurrencyType::GOLD, FGsTaxPercent(row->goldSystemTaxRate, row->goldDefaultTaxRate));

	// 자주 요청하지 않게 10초 제한
	_sendChecker.SetIntervalSecond(10.f);
}

void FGsTaxDataHandler::Finalize()
{
	_taxPercentMap.Empty();
	_accumTaxList.Empty();
	_castleTaxPercent = 0;
	_accumStartTime = 0;
	_accumEndTime = 0;
}

void FGsTaxDataHandler::SetContinentTax(PD::GC::PKT_GC_ACK_CONTINENT_TAX_READ& InPacket)
{
	_accumTaxList.Empty();
	_accumStartTime = 0;
	_accumEndTime = 0;

	// B1: 공성전 정식 개발 전까지는 데이터가 하나만 올 것
	for (PD::GC::PKT_GC_ACK_CONTINENT_TAX_READ::ContinentListIterator iter = InPacket.GetFirstContinentListIterator();
		iter != InPacket.GetLastContinentListIterator(); ++iter)
	{
		FGsContinentAccumTaxData taxData;
		//taxData._continentId = iter->ContinentId(); // B1: 추후 필요 시 저장
		//taxData._guildDBId = iter->GuildDBId(); // B1: 추후 필요 시 저장
		taxData._accumGold = iter->Accumul_Gold();
		taxData._accumDia = iter->Accumul_Dia();

		_accumTaxList.Emplace(taxData);

		_accumStartTime = iter->StartAccumlateTime();
		_accumEndTime = iter->EndAccumlateTime();
	}
}

void FGsTaxDataHandler::SendReqAccumTax(bool bResetCoolTime)
{
	if (false == bResetCoolTime)
	{
		if (false == _sendChecker.IsTimeOver())
		{
			return;
		}
	}

	FGsNetSendServiceMarket::SendReqContinentTax();

	_sendChecker.UpdateTime();
}

Currency FGsTaxDataHandler::GetAccumGold() const
{
	Currency accumGold = 0;
	for (const FGsContinentAccumTaxData& iter : _accumTaxList)
	{
		accumGold += iter._accumGold;
	}

	return accumGold;
}

Currency FGsTaxDataHandler::GetAccumDia() const
{
	Currency accumDia = 0;
	for (const FGsContinentAccumTaxData& iter : _accumTaxList)
	{
		accumDia += iter._accumDia;
	}

	return accumDia;
}

int32 FGsTaxDataHandler::GetTaxPercent(CurrencyType InType, bool bIsSystem)
{
	if (FGsTaxPercent* taxPercent = _taxPercentMap.Find(InType))
	{
		return bIsSystem ? taxPercent->_systemTaxPercent : taxPercent->_defaultTaxPercent;
	}

	return 0;
}

int32 FGsTaxDataHandler::GetTotalTaxPercent(CurrencyType InType)
{
	if (FGsTaxPercent* taxPercent = _taxPercentMap.Find(InType))
	{
		return taxPercent->GetTotalPercent() + _castleTaxPercent;
	}

	return _castleTaxPercent;
}
