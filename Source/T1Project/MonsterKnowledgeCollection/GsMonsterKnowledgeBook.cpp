
#include "GsMonsterKnowledgeBook.h"
#include "GsTable.h"
#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "DataSchema/GameObject/GsSchemaShapeData.h"
#include "DataSchema/MonsterKnowledgeCollection/GsSchemaMonsterKnowledgeMapData.h"
#include "DataSchema/MonsterKnowledgeCollection/GsSchemaMonsterKnowledgeLevelSet.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Shared/Client/SharedEnums/SharedMonsterknowledgeEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "../T1Project.h"





FGsMonsterKnowledgeBook::FGsMonsterKnowledgeBook(const FGsSchemaNpcData* InData)
	: _monsterData(InData), _spawnMapData(nullptr), _expNPointTable(BuildExpNPointTable(InData)), _level(0), _accumulateExp(0)
{

}

void FGsMonsterKnowledgeBook::Set(uint16 InLevel, MonsterKnowledgeExp InExp)
{
	CalcExp(InLevel, InExp);
}

void FGsMonsterKnowledgeBook::AddExp(MonsterKnowledgeExp InExp)
{
	CalcExp(_level, InExp);
}

void FGsMonsterKnowledgeBook::CalcExp(uint16 InLevel, MonsterKnowledgeExp InExp)
{
	_level = InLevel;
	_accumulateExp = InExp;

	// 2023/2/13 PKT - 현재 구간에서 레벨업이 가능 한가?
	const MonsterKnowledgeExp maxExp = ExpNPoint(_level).Key;
	if (_accumulateExp >= maxExp)
	{
		_state = EMonsterKnowledgeState::LevelUpPossible;
	}
	else
	{
		_state = EMonsterKnowledgeState::Normal;
	}

	const uint16 maxLevel = MaxLevel();
	if (InLevel >= maxLevel)
	{
		_state = EMonsterKnowledgeState::FinalLevel;
	}
	
	if (_OnBookDelegate.IsBound())
	{
		_OnBookDelegate.ExecuteIfBound(this);
	}
}

const FGsSchemaNpcData* FGsMonsterKnowledgeBook::NPCData() const
{
	return _monsterData;
}

EMonsterKnowledgeState FGsMonsterKnowledgeBook::State() const
{
	return _state;
}

//int32 FGsMonsterKnowledgeBook::SortOrder() const
//{
//	return _monsterData->monsterKnowledgeOrder;
//}

uint32 FGsMonsterKnowledgeBook::Id() const
{
	return _monsterData->id;
}

//MonsterKnowledgeBookGrade FGsMonsterKnowledgeBook::BookGrade() const
//{
//	return _monsterData->monsterKnowledgeBookGrade;
//}

const FGsSchemaMapData* FGsMonsterKnowledgeBook::SpawnMapData() const
{
	//if (nullptr == _spawnMapData)
	//{
	//	do 
	//	{
	//		const UGsTableMonsterKnowledgeMapData* monsterKnowledgeWorldMapTable = Cast< UGsTableMonsterKnowledgeMapData>(FGsSchemaMonsterKnowledgeMapData::GetStaticTable());
	//		if (nullptr == monsterKnowledgeWorldMapTable)
	//		{
	//			GSLOG(Error, TEXT("nullptr == monsterKnowledgeWorldMapTable"));
	//			break;
	//		}

	//		TArray<const FGsSchemaMonsterKnowledgeMapData*> rowDataSet;
	//		if (false == monsterKnowledgeWorldMapTable->GetAllRows(rowDataSet))
	//		{
	//			GSLOG(Error, TEXT("false == monsterKnowledgeWorldMapTable->GetAllRows(rowDataSet)"));
	//			break;
	//		}

	//		CreatureId id = StaticCast<CreatureId>(_monsterData->id);
	//		const auto& data = rowDataSet.FindByPredicate([id](const FGsSchemaMonsterKnowledgeMapData* InData)
	//			{
	//				return (InData && InData->id == id);
	//			}
	//		);

	//		if (data && (*data)->spawnMapDataArray.Num())
	//		{
	//			for (const auto& mapRowData : (*data)->spawnMapDataArray)
	//			{
	//				if (nullptr == _spawnMapData)
	//				{
	//					_spawnMapData = mapRowData.GetRow();
	//				}
	//				else
	//				{
	//					if (_spawnMapData->id > mapRowData.GetRow()->id)
	//					{
	//						_spawnMapData = mapRowData.GetRow();
	//					}
	//				}
	//			}
	//		}

	//	} while (0);
	//	
	//	/*FGsSchemaMonsterKnowledgeMapData
	//	*/
	//}
	//return _spawnMapData;
	return nullptr;
}

uint16 FGsMonsterKnowledgeBook::Level() const
{
	return _level;
}

uint16 FGsMonsterKnowledgeBook::MaxLevel() const
{
	return _expNPointTable.Num();
}

MonsterKnowledgeExp FGsMonsterKnowledgeBook::AccumulateExp() const
{
	return _accumulateExp;
}

TPair<MonsterKnowledgeExp, int32> FGsMonsterKnowledgeBook::ExpNPoint(int32 InLevel) const
{
	if (false == _expNPointTable.IsValidIndex(InLevel))
	{
		return TPair<MonsterKnowledgeExp, int32>(0, 0);
	}

	return _expNPointTable[InLevel];
}


int32 FGsMonsterKnowledgeBook::GainRewardPoint(uint16 InLevel, MonsterKnowledgeExp InExp) const
{
	int32 gainPoint = 0;
	for (uint16 level = InLevel; level < MaxLevel(); ++level)
	{
		const auto data = ExpNPoint(level);
		if (InExp >= data.Key)
		{
			gainPoint += data.Value;
		}
	}

	return gainPoint;
}

int32 FGsMonsterKnowledgeBook::RequiredAmountMaxLevel(MonsterKnowledgeExp InAmountExp) const
{
	MonsterKnowledgeExp accumulateExp = AccumulateExp();
	MonsterKnowledgeExp totalMaxExp = ExpNPoint(MaxLevel() - 1).Key;

	int32 useCount = 0;
	do
	{
		if (totalMaxExp <= (accumulateExp + (InAmountExp * useCount)))
		{
			break;
		}
		useCount++;
	} while (1);

	return useCount;
}

uint16 FGsMonsterKnowledgeBook::LevelByExp(MonsterKnowledgeExp inExp) const
{
	for (uint16 level = 0; level < MaxLevel(); ++level)
	{
		if (inExp < ExpNPoint(level).Key)
		{
			return level;
		}
	}

	return MaxLevel();
}
//
//TPair<MonsterKnowledgeExp, MonsterKnowledgeExp> FGsMonsterKnowledgeBook::RangeExpByLevel(MonsterKnowledgeExp inExp) const
//{
//	if (inExp > ExpNPoint(MaxLevel()).Key)
//	{
//		return TPair<MonsterKnowledgeExp, MonsterKnowledgeExp>(0, 1);
//	}
//
//	MonsterKnowledgeExp minExp = 0;
//	MonsterKnowledgeExp maxExp = ExpNPoint(0);
//
//	for (int32 level = 0; level < int32(MaxLevel()); ++level)
//	{
//		if (inExp < maxExp)
//		{
//			break;
//		}
//
//		minExp = ExpNPoint(level);
//		maxExp = ExpNPoint(level + 1);
//	}
//	
//	return TPair<MonsterKnowledgeExp, MonsterKnowledgeExp>(minExp, maxExp);
//}

//Level FGsMonsterKnowledgeBook::ExpectedLevel(MonsterKnowledgeExp InAccumulateExp) const
//{
//	Level outLevel = MaxLevel();
//	for (Level level = 0; level < MaxLevel(); ++level)
//	{
//		if (InAccumulateExp < ExpNPoint(level).Key)
//		{
//			outLevel = level;
//			break;
//		}
//		InAccumulateExp -= ExpNPoint(level).Key;
//	}
//
//	return outLevel;
//}
//
//MonsterKnowledgeExp FGsMonsterKnowledgeBook::AccumulateExp(MonsterKnowledgeExp IncreaseExp /*= 0*/) const
//{
//	MonsterKnowledgeExp outExp = 0;
//	for (Level level = 0; level < Level(); ++level)
//	{
//		outExp += ExpNPoint(level).Key;
//	}
//
//	outExp += AccumulateExp();
//	outExp += IncreaseExp;
//
//	return outExp;
//}
//
//TPair<MonsterKnowledgeExp, MonsterKnowledgeExp> RangeExpByAccumulateExp(MonsterKnowledgeExp InAccumulateExp)
//{
//	MonsterKnowledgeExp minExp = 0;
//	MonsterKnowledgeExp maxExp = 1;
//
//
//
//	return TPair<MonsterKnowledgeExp, MonsterKnowledgeExp>(minExp, maxExp);
//}
//
//MonsterKnowledgeExp FGsMonsterKnowledgeBook::AccumulateExpByLevel(Level InLevel) const
//{
//	MonsterKnowledgeExp outExp = 0;
//	for (Level level = 0; level < InLevel; ++level)
//	{
//		outExp += ExpNPoint(level).Key;
//	}
//
//	return outExp;
//}

//TPair<MonsterKnowledgeExp, MonsterKnowledgeExp> FGsMonsterKnowledgeBook::RangeExp(Level InLevel) const
//{
//	MonsterKnowledgeExp minExp = 0;
//	MonsterKnowledgeExp maxExp = 0;
//	if (InLevel >= MaxLevel())
//	{
//		minExp = 0;
//		maxExp = 0;		
//	}
//	else
//	{
//		minExp = (0 >= InLevel) ? 0 : ExpNPoint(InLevel - 1).Key;
//		maxExp = ExpNPoint(InLevel).Key;
//	}
//	
//	return TPair<MonsterKnowledgeExp, MonsterKnowledgeExp>(minExp, maxExp);
//}
//
//double FGsMonsterKnowledgeBook::ExpectedPercent(MonsterKnowledgeExp InAccumulateExp, Level& OutLevel) const
//{
//	OutLevel = 0;
//
//	if (0 >= InAccumulateExp)
//	{
//		return 0.0;
//	}
//
//	for (int32 level = 0; level < StaticCast<int32>(MaxLevel()); ++level)
//	{
//		if (InAccumulateExp < ExpNPoint(level).Key)
//		{
//			break;
//		}
//
//		OutLevel++;
//		InAccumulateExp -= ExpNPoint(level).Key;
//	}
//
//	if (OutLevel >= MaxLevel())
//	{
//		OutLevel = MaxLevel();
//		return 1.0;
//	}
//
//	return (double)InAccumulateExp / ExpNPoint(OutLevel).Key;
//}

//void FGsMonsterKnowledgeBook::ExpectedProgress(double InAccumulateExp, Level& OutLevel, double& OutSectionExp, MonsterKnowledgeExp& OutSectionMaxExp)
//{
//	OutLevel = 0;
//	OutSectionExp = 0;
//	OutSectionMaxExp = ExpNPoint(OutLevel).Key;
//
//	if (0 >= InAccumulateExp)
//	{
//		return;
//	}
//
//	for (int32 level = 0; level < StaticCast<int32>(MaxLevel()); ++level)
//	{
//		if (InAccumulateExp < double(ExpNPoint(level).Key))
//		{
//			break;
//		}
//
//		OutLevel++;
//		InAccumulateExp -= double(ExpNPoint(level).Key);
//	}
//
//	if (OutLevel >= MaxLevel())
//	{
//		OutLevel = MaxLevel();
//		OutSectionExp = 1;
//		OutSectionMaxExp = 1;
//	}
//
//	OutSectionExp = InAccumulateExp;
//	OutSectionMaxExp = ExpNPoint(OutLevel).Key;
//}
//
//float FGsMonsterKnowledgeBook::percentEXP() const
//{
//	if (Level() >= MaxLevel())
//	{
//		return 1.f;
//	}
//
//	MonsterKnowledgeExp maxExp = 0;
//
//	for (Level level = 0; level <= Level(); ++level)
//	{
//		maxExp += ExpNPoint(level).Key;
//	}
//
//	return 0 == maxExp ? 0.f : (FMath::Clamp(float(_sectionExp) / maxExp, 0.f, 1.0f));
//}
//
//MonsterKnowledgeExp FGsMonsterKnowledgeBook::MaxAccrueExpByLevel(int32 InLevel) const
//{
//	MonsterKnowledgeExp maxExp = 0;
//	do {
//		maxExp += ExpNPoint(InLevel).Key;
//		--InLevel;
//	} while (0 <= InLevel);
//	return maxExp;	
//}
//
//TPair<Level, float> FGsMonsterKnowledgeBook::ExpectedExpState(MonsterKnowledgeExp InExp) const
//{
//	Level deltaLevel = ExpectedLevel(InExp);
//	MonsterKnowledgeExp accrueExp = 0;
//	for (Level level = 0; level < deltaLevel; ++level)
//	{
//		accrueExp += ExpNPoint(level).Key;
//	}
//
//	MonsterKnowledgeExp deltaExp = ExpNPoint(deltaLevel).Key;
//
//	float percent = (0 > deltaExp) ? float(InExp - accrueExp) / deltaExp : 0.f;	
//
//	return TPair<Level, float>(deltaLevel, percent);
//}
//
//


TArray<TPair<MonsterKnowledgeExp, int32>> FGsMonsterKnowledgeBook::BuildExpNPointTable(const struct FGsSchemaNpcData* InData) const
{
	TArray<TPair<MonsterKnowledgeExp, int32>> outTable;

	//do 
	//{
	//	const UGsTable* table = FGsSchemaMonsterKnowledgeLevelSet::GetStaticTable();
	//	if (nullptr == table)
	//	{
	//		GSLOG(Error, TEXT("nullptr == table"));
	//		break;
	//	}

	//	const TMap<FName, uint8*>* OutRowMap;
	//	if (false == table->GetRowMap(OutRowMap))
	//	{
	//		GSLOG(Error, TEXT("false == table->GetRowMap(OutRowMap)"));
	//		break;
	//	}

	//	int32 levelSetId = InData->monsterKnowledgeLevelSetId.GetRow() ? InData->monsterKnowledgeLevelSetId.GetRow()->id : 0;

	//	for (const auto& pair : (*OutRowMap))
	//	{
	//		const FGsSchemaMonsterKnowledgeLevelSet* levelSetData = reinterpret_cast<const FGsSchemaMonsterKnowledgeLevelSet*>(pair.Value);
	//		if (levelSetData->id == levelSetId)
	//		{
	//			for (const auto& item : levelSetData->knowledgeLevelList)
	//			{
	//				outTable.Emplace(TPair<MonsterKnowledgeExp, int32>(item.levelUpExpRequired, item.monsterBookPoint));
	//			}
	//			break;
	//		}
	//	}

	//	outTable.Sort([](const TPair<MonsterKnowledgeExp, int32>& lhr, const TPair<MonsterKnowledgeExp, int32>& rhr)
	//		{
	//			return (lhr.Key < rhr.Key);
	//		}
	//	);		

	//} while (0);

	return outTable;
}