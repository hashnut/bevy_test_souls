
#include "GsTitleRewardData.h"
#include "UI/UIContent/Helper/GsEffectStringHelper.h"
#include "DataSchema/Title/GsSchemaTitleCollectionReward.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "T1Project/T1Project.h"



FGsTitleRewardData::FGsTitleRewardData(const struct FGsSchemaTitleCollectionReward* InData)
	: _data(InData)
{
	for (int32 passivityId : _data->passivitySetIds)
	{
		_passivitySetIds.Emplace(StaticCast<PassivityId>(passivityId));
	}

	if (0 < _passivitySetIds.Num())
	{
		const UGsTableManager& tableManager = UGsTableManager::GetInstance();

		const UGsTablePassivitySet* passivitySetTable
			= Cast<UGsTablePassivitySet>(tableManager.GetTable(TBaseStructure<FGsSchemaPassivitySet>::Get()));
		if (nullptr == passivitySetTable)
		{
			GSLOG(Error, TEXT("nullptr == passivitySetTable"));
			return;
		}

		for (PassivityId passivityId : _passivitySetIds)
		{
			const FGsSchemaPassivitySet* passivitySetData = nullptr;
			if (false == passivitySetTable->FindRowById(passivityId, passivitySetData))
			{
				GSLOG(Error, TEXT("false == passivitySetTable->FindRowById(passivity, passivitySetData)"));
				return;
			}

			FGsEffectStringHelper::FGsEffectAddData effectAddData;

			if (false == FGsEffectStringHelper::AddPassivityEffect(passivitySetData, effectAddData))
			{
				GSLOG(Error, TEXT("false == FGsEffectStringHelper::AddPassivityEffect(passivitySetData, effectAddData [%d])"), passivitySetData->id);
				continue;
			}

			for(const auto & pair : effectAddData._statMap)
			{
				_statSet.Emplace(pair);
			}
		}
	}

	// 2022/01/27 PKT - Sort
	_statSet.Sort([](const TPair<StatType, int32>& lhr, const TPair<StatType, int32>& rhr)
		{
			return lhr.Key < rhr.Key;
		}
	);
	
	/**
	 * 2022/01/14 PKT - ÃÊ±âÈ­
	 */
	SetState(ETitleRewardState::Disable);
}

void FGsTitleRewardData::SetState(ETitleRewardState InState)
{
	_state = InState;
}

int32 FGsTitleRewardData::GetId() const
{
	return _data->id;
}

ETitleRewardState FGsTitleRewardData::GetState() const
{
	return _state;
}

int32 FGsTitleRewardData::GetAcquisitionCount() const
{
	return _data->acquireCount;
}

const TArray<PassivityId>& FGsTitleRewardData::GetPassivityIds() const
{
	return _passivitySetIds;
}

int32 FGsTitleRewardData::GetStatCount() const
{
	return _statSet.Num();
}

bool FGsTitleRewardData::FindStatData(int32 InIndex, OUT TPair<StatType, int32>& OutStatData) const
{
	if (false == _statSet.IsValidIndex(InIndex))
	{
		GSLOG(Error, TEXT("false == _statSet.IsValidIndex(InIndex)"));
		return false;
	}

	OutStatData = _statSet[InIndex];

	return true;
}