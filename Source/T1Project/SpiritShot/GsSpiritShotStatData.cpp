#include "GsSpiritShotStatData.h"
#include "T1Project.h"

#include "GameObject/Stat/GsStatHelper.h"
#include "Management/ScopeGame/GsEffectTextManager.h"

#include "Runtime/DataCenter/Public/DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"

FGsSpiritShotStatData::FGsSpiritShotStatData()
{
	// 모든 것에는 이유가 있다.
	bool StatDataCanBeTSharedRefType = true;
}

FGsSpiritShotStatData::FGsSpiritShotStatData(const TArray<PassivityId>& InIdList) :
	_passivityIdList(InIdList)
{
	MakeStatData();
}

FGsSpiritShotStatData::FGsSpiritShotStatData(TArray<PassivityId>&& InIdList) :
	_passivityIdList(MoveTemp(InIdList))
{
	MakeStatData();
}

void FGsSpiritShotStatData::MakeStatData()
{
	// 스탯 관련 데이터 가공
	FGsEffectTextManager* effectTextMgr = GSEffectText();
	if (nullptr == effectTextMgr)
	{
		GSLOG(Error, TEXT("nullptr == effectTextMgr"));
		return;
	}

	effectTextMgr->GetEffectTextFromPassivityList(_passivityIdList, _effectCollection);
}

FText FGsSpiritShotStatData::GetStatDescText() const
{
	FString statDescString;
	for (TPair<StatType, int32> iter : _effectCollection._statMap)
	{
		// 개행처리
		if (false == statDescString.IsEmpty())
		{
			statDescString.Append(TEXT("\n"));
		}

		FGsStatHelper::GetDesplayStatNameAndValue(iter.Key, iter.Value, statDescString);
	}

	return FText::FromString(MoveTemp(statDescString));
}

TArray<PassivityId> FGsSpiritShotStatData::GetPassivityIdList() const
{
	return _passivityIdList;
}

void FGsSpiritShotStatData::SetStatData(const TArray<PassivityId>& InIdList)
{
	_passivityIdList = InIdList;
	MakeStatData();
}

void FGsSpiritShotStatData::SetStatData(TArray<PassivityId>&& InIdList)
{
	_passivityIdList = MoveTemp(InIdList);
	MakeStatData();
}
