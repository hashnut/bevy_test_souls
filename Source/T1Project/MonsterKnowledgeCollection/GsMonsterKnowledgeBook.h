#pragma once

#include "CoreMinimal.h"
#include "EMonsterKnowledgeEnumData.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Shared/Client/SharedEnums/SharedMonsterknowledgeEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"


class FGsMonsterKnowledgeBook
{
private:
	// 2021/11/10 PKT - 몬스터 Data 정보
	const struct FGsSchemaNpcData* _monsterData = nullptr;

	// 2023/2/10 PKT - Spawn Map Data
	mutable const struct FGsSchemaMapData* _spawnMapData = nullptr;

	// 2023/1/30 PKT - 경험치 및 레벨업시 보상 포인트
	TArray<TPair<MonsterKnowledgeExp, int32>> _expNPointTable;

	// 2021/11/10 PKT - 상태
	EMonsterKnowledgeState _state = EMonsterKnowledgeState::Normal;

	// 2021/11/10 PKT - 현재 레벨
	uint16 _level = 0;

	// 2021/11/10 PKT -  누적 경험치
	MonsterKnowledgeExp _accumulateExp = 0;

public:
	DECLARE_DELEGATE_OneParam(FOnBookDelegate, const FGsMonsterKnowledgeBook*);
	FOnBookDelegate _OnBookDelegate;

public:
	explicit FGsMonsterKnowledgeBook(const struct FGsSchemaNpcData* InData);

	void Set(uint16  InLevel, MonsterKnowledgeExp InExp);

	void AddExp(MonsterKnowledgeExp InExp);

protected:
	void CalcExp(Level InLevel, MonsterKnowledgeExp InExp);

public:
	const struct FGsSchemaNpcData* NPCData() const;

	EMonsterKnowledgeState State() const;

	//int32 SortOrder() const;

	uint32 Id() const;

	//MonsterKnowledgeBookGrade BookGrade() const;

	const FGsSchemaMapData* SpawnMapData() const;

	// 2021/11/28 PKT - 몬스터 지식 레벨
	uint16 Level() const;

	uint16 MaxLevel() const;

	// 2021/12/07 PKT - 현재 몬스터 지식의 경험치
	MonsterKnowledgeExp AccumulateExp() const;

	// 2023/2/13 PKT - 현재 구간의 누적 경험치
	//MonsterKnowledgeExp SectionMaxExp() const;

	/**
	 * Util..
	 */
	 // 2023/2/13 PKT - 누적 최대 경험치
	TPair<MonsterKnowledgeExp, int32> ExpNPoint(int32 InLevel) const;

	int32 GainRewardPoint(uint16 InLevel, MonsterKnowledgeExp InExp) const;

	int32 RequiredAmountMaxLevel(MonsterKnowledgeExp inExp) const;

	uint16 LevelByExp(MonsterKnowledgeExp inExp) const;

	TPair<MonsterKnowledgeExp, MonsterKnowledgeExp> RangeExpByLevel(MonsterKnowledgeExp inExp) const;

private:
	TArray<TPair<MonsterKnowledgeExp, int32>> BuildExpNPointTable(const struct FGsSchemaNpcData* InData) const;
};