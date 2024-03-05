
#include "GsMonsterKnowledgeState.h"
#include "UTIL/GsGameObjectUtil.h"
#include "MonsterKnowledgeCollection/GsMonsterKnowledgeAreaCategory.h"
#include "MonsterKnowledgeCollection/GsMonsterKnowledgeLevelInfo.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcBalance.h"
#include "DataSchema/GameObject/GsSchemaShapeData.h"
#include "DataSchema/MonsterKnowledgeCollection/GsSchemaMonsterKnowledgeLevelSet.h"
#include "UTIL/GsGameObjectUtil.h"
#include "T1Project/T1Project.h"

FGsMonsterKnowledgeState::FGsMonsterKnowledgeState(const FGsSchemaNpcData* InData
	, const TArray<FGsMonsterKnowledgeLevelInfo>* levelInfos)
	: _monsterData(InData)
	, _monsterShapeData(InData->shapeId.GetRow())
	, _monsterBalance(InData->balanceId.GetRow())
{
	for (const FGsMonsterKnowledgeLevelInfo& levelInfo : (*levelInfos))
	{
		_expAndPointTableForLevel.Emplace(TPair<MonsterKnowledgeExp, int32>(levelInfo.levelUpExpRequired, levelInfo.monsterBookPoint));
	}

	_expAndPointTableForLevel.Sort([](const TPair<MonsterKnowledgeExp, int32>& lhr, const TPair<MonsterKnowledgeExp, int32>& rhr)
		{
			return (lhr.Key < rhr.Key);
		}
	);
}

void FGsMonsterKnowledgeState::SetData(Level InLevel, MonsterKnowledgeExp InExp)
{
	CalcAmountKnowledge(InLevel, InExp);
}

void FGsMonsterKnowledgeState::UpdateByKnowledgeExp(MonsterKnowledgeExp InExp)
{
	CalcAmountKnowledge(_currnetLevel, InExp);
}

void FGsMonsterKnowledgeState::CalcAmountKnowledge(Level InLevel, MonsterKnowledgeExp InExp)
{
	_currentExp = InExp;
	_currnetLevel = InLevel;

	// 2021/11/10 PKT - 현재 지식 수량 업데이트
	const int32 maxLevel = GetMaxLevel();
	
	EMonsterKnowledgeState nextState = EMonsterKnowledgeState::Normal;

	const int32 maxExp = GetExpAndPointForLevel(InLevel).Key;
	if (_currentExp >= maxExp)
	{	
		nextState = EMonsterKnowledgeState::LevelUpPossible;
	}

	if (_currnetLevel == maxLevel)
	{
		nextState = EMonsterKnowledgeState::FinalLevel;
		_currentExp = GetExpAndPointForLevel(maxLevel - 1).Key;
	}

	// 2021/12/09 PKT - 상태 변경
	_currentState = nextState;

	OnUpdateByKnowledgeState.ExecuteIfBound(this);
}

// 2021/11/10 PKT - 몬스터 종족
CreatureRaceType FGsMonsterKnowledgeState::GetRaceType() const
{
	return _monsterData->race;
}

// 2021/11/10 PKT - 몬스터 공격형 타입(선공/비선공)
bool FGsMonsterKnowledgeState::IsOffensive() const
{
	return _monsterData->isOffensive;
}

// 2021/11/10 PKT - 몬스터 속성
MonsterElement FGsMonsterKnowledgeState::GetElemnet() const
{
	return _monsterData->element;
}

//int32 FGsMonsterKnowledgeState::GetOrderNum() const
//{
//	return _monsterData->monsterKnowledgeOrder;
//}

EMonsterKnowledgeState FGsMonsterKnowledgeState::GetCurrentState() const
{
	return _currentState;
}

// 2021/11/10 PKT - 몬스터 아이디
uint32 FGsMonsterKnowledgeState::GetId() const
{
	return StaticCast<uint32>(_monsterData->id);
}

// 2021/11/10 PKT - 몬스터 이름.
const FText& FGsMonsterKnowledgeState::GetMonsterName() const
{
	return _monsterData->nameText;
}

// 2021/11/10 PKT - 몬스터 랭크
CreatureRankType FGsMonsterKnowledgeState::GetCreatureRank() const
{
	return _monsterData->grade;
}

MonsterKnowledgeExp FGsMonsterKnowledgeState::GetCurrentExp() const
{
	return _currentExp;
}

TPair<MonsterKnowledgeExp, int32> FGsMonsterKnowledgeState::GetExpAndPointForLevel(Level InLevel) const
{
	if (false == _expAndPointTableForLevel.IsValidIndex(InLevel))
	{
		return TPair<MonsterKnowledgeExp, int32>(0,0);
	}

	return _expAndPointTableForLevel[InLevel];
}

const FSoftObjectPath& FGsMonsterKnowledgeState::GetIllustrationPath() const
{
	return _monsterShapeData->imageRegionMap;
}

Level FGsMonsterKnowledgeState::GetCurrentLevel() const
{
	return _currnetLevel;
}

Level FGsMonsterKnowledgeState::GetMaxLevel() const
{	
	return _expAndPointTableForLevel.Num();
}

const FText& FGsMonsterKnowledgeState::GetTextCreatureRank() const
{
	if (_cachedTextCreatureRank.IsEmpty())
	{
		switch (GetCreatureRank())
		{
		case CreatureRankType::RAID:
			FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("CreatureRankRaid"), _cachedTextCreatureRank);
			break;
		case CreatureRankType::BOSS:
			FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("CreatureRankBoss"), _cachedTextCreatureRank);
			break;
		case CreatureRankType::NAMED:
			FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("CreatureRankNamed"), _cachedTextCreatureRank);
			break;
		case CreatureRankType::ELITE:
			FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("CreatureRankElite"), _cachedTextCreatureRank);
			break;
		case CreatureRankType::NORMAL:
			FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("CreatureRankNormal"), _cachedTextCreatureRank);
			break;
		case CreatureRankType::NONE:
			break;
		default:
			break;
		}
	}
	return _cachedTextCreatureRank;
}


const FText& FGsMonsterKnowledgeState::GetTextOffensive() const
{
	if (_cachedTextOffensive.IsEmpty())
	{
		if (IsOffensive())
		{
			FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("MonOffensive"), _cachedTextOffensive);
		}
		else
		{
			FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("MonNotOffensive"), _cachedTextOffensive);
		}
	}
	return _cachedTextOffensive;
}


const FText& FGsMonsterKnowledgeState::GetTextCreatureRace() const
{
	if (_cachedTextCreatureRace.IsEmpty())
	{
		_cachedTextCreatureRace = UGsGameObjectUtil::GetRaceText(GetRaceType());
	}
	return _cachedTextCreatureRace;
}


const FText& FGsMonsterKnowledgeState::GetTextDefenseMaterial() const
{
	if (_cachedTextDefenseMaterial.IsEmpty())
	{
		_cachedTextDefenseMaterial = UGsGameObjectUtil::GetRaceWeaknessItemMaterialText(GetRaceType());
	}
	return _cachedTextDefenseMaterial;
}


const FText& FGsMonsterKnowledgeState::GetTextMaterial() const
{
	if (_cachedTextMaterial.IsEmpty())
	{
		_cachedTextMaterial = UGsGameObjectUtil::GetMonsterElementalText(_monsterData->element);
	}
	return _cachedTextMaterial;
}

