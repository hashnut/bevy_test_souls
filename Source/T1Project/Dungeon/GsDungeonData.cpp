
#include "GsDungeonData.h"
#include "DataSchema/Dungeon/GsSchemaDungeonData.h"
#include "DataSchema/Map/GsSchemaMapBalance.h"
#include "Currency/GsCostPackage.h"
#include "Currency/GsCostOption.h"
#include "ContentsEvent/GsContentsEventTeleportCost.h"


FGsDungeonData::FGsDungeonData(const struct FGsSchemaDungeonData* InDungeonData
	, TFunction<bool(const FGsDungeonData*,bool)> InUnlockContentsFunc /*= nullptr*/)
	: _schemaData(InDungeonData), _unlockContentsFunc(InUnlockContentsFunc), _isPrevUnlockState(false)
{

}

bool FGsDungeonData::IsUnLockState() const
{
	return _isPrevUnlockState;
}

bool FGsDungeonData::IsUnLockContents(bool InShowTicker /* = false */)
{	
	if (_unlockContentsFunc)
	{
		_isPrevUnlockState = _unlockContentsFunc(this, InShowTicker);
	}
	return 	_isPrevUnlockState;
}

TTuple<CurrencyType, Currency> FGsDungeonData::GetEnterdCurrencyData() const
{
	CurrencyType OutCurrencyType = CurrencyType::NONE;
	Currency OutCurrency = 0;

	do 
	{
		if (nullptr == _schemaData )
		{
			break;
		}

		const FGsSchemaMapData* schemaMapData = _schemaData->mapId.GetRow();
		if (nullptr == schemaMapData)
		{
			break;
		}

		const FGsSchemaMapBalance* schemaMapBalance = schemaMapData->balanceId.GetRow();
		if (nullptr == schemaMapBalance)
		{
			break;
		}

		TSharedRef<FGsCostPackage> CostPackage(FGsCostPackage::CreateSimple(schemaMapBalance->cost.costCurrencyType, schemaMapBalance->cost.costCurrencyAmount));
		TSharedPtr<FGsCostOption> CostOption = FGsContentsEventTeleportCost::CreateActivatedTeleportCostOption();
		if (CostOption.IsValid())
		{
			// 텔레포트 비용 할인 객체가 있을 경우, 추가
			CostPackage->AddCostOption(CostOption.ToSharedRef());
		}

		const FGsCostElement* FirstCostElement = CostPackage->GetFirstCostElement();
		if (nullptr == FirstCostElement)
		{
			GSLOG(Error, TEXT("Failed to get first cost element."));
		}
		else
		{
			OutCurrencyType = FirstCostElement->GetCurrencyType();
			OutCurrency = FirstCostElement->GetCostAmount();
		}

	} while (0);

	return MakeTuple(OutCurrencyType, OutCurrency);
}

const FGsSchemaDungeonData* FGsDungeonData::GetSchema() const
{
	return _schemaData;
}