/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Reward/GsSchemaRewardData.h"
#include "Runtime/DataCenter/Public/DataSchema/Reward/GsSchemaRewardBoxData.h"
#include "Runtime/DataCenter/Public/DataSchema/Reward/GsSchemaRewardItemBagData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
FGsSchemaRewardData::FGsSchemaRewardData()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaRewardData)
	
	id = 0;
	expMin = 0;
	expMax = 0;
	currencyType = CurrencyType::NONE;
	currencyAmountMin = 0;
	currencyAmountMax = 0;
	rewardType = RewardType::ALL;
}
