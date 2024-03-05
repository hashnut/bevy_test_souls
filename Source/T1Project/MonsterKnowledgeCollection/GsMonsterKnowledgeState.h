#pragma once

#include "CoreMinimal.h"
#include "EMonsterKnowledgeEnumData.h"
#include "MonsterKnowledgeCollectionTypeDefine.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

/**
* file		GsMonsterKnowledgeState.h
* @brief	몬스터 지식 상태 클래스
* @author	PKT
* @date		2021/11/09
**/

struct FGsSchemaNpcData;
struct FGsSchemaShapeData;
struct FGsSchemaNpcBalance;
struct FGsMonsterKnowledgeLevelInfo;
struct FGsMonsterKnowledgeSubCategoryInfo;
class FGsMonsterKnowledgeState;

DECLARE_DELEGATE_OneParam(FOnUpdateByKnowledgeState, const FGsMonsterKnowledgeState*);

class FGsMonsterKnowledgeState
{
	static const int32 INIT_LEVEL = 0;
	/************************************************************************/
	/* 멤버                                                                  */
	/************************************************************************/
private:
	// 2021/11/10 PKT - 몬스터 Data 정보
	const FGsSchemaNpcData* _monsterData = nullptr;

	// 2021/11/10 PKT - 몬스터 일러스트 정보
	const FGsSchemaShapeData* _monsterShapeData = nullptr;

	// 2021/11/10 PKT - 몬스터 밸런스
	const FGsSchemaNpcBalance* _monsterBalance = nullptr;

	// 2021/11/10 PKT - 상태
	EMonsterKnowledgeState _currentState = EMonsterKnowledgeState::Normal;

	// 2021/11/10 PKT - 현재 레벨
	Level _currnetLevel = 0;

	// 2021/11/10 PKT - 누적 지식 총량
	MonsterKnowledgeExp _currentExp = 0;

	// 2021/12/07 PKT - 레벨 당 최대 경험치 및 보상 지식 포인트
	TArray<TPair<MonsterKnowledgeExp, int32>> _expAndPointTableForLevel;

	/**
	 * UI 에서 필요한 Text를 캐싱해놓는다.
	 */
	// 2021/12/07 PKT - 일반/엘리트/보스 등등..
	mutable FText _cachedTextCreatureRank;
	// 2021/12/07 PKT - 선공/비선공
	mutable FText _cachedTextOffensive;
	// 2021/12/07 PKT - 종족 이름.
	mutable FText _cachedTextCreatureRace;
	// 2021/12/07 PKT - 약점/재질
	mutable FText _cachedTextDefenseMaterial;
	// 2021/12/07 PKT - 속성
	mutable FText _cachedTextMaterial;

	
	/************************************************************************/
	/* 대리자                                                                */
	/************************************************************************/
public:
	FOnUpdateByKnowledgeState OnUpdateByKnowledgeState;

public:
	// 2021/11/10 PKT - 몬스터 정보 / 몬스터가 소속 되어 있는 MapID
	FGsMonsterKnowledgeState() = default;

	explicit FGsMonsterKnowledgeState(const FGsSchemaNpcData* InData, const TArray<FGsMonsterKnowledgeLevelInfo>* levelInfos);

	explicit FGsMonsterKnowledgeState(const FGsSchemaNpcData* InData, MapId InMapId);

	bool operator==(const FGsMonsterKnowledgeState& InState) const
	{
		return (this->GetId() == InState.GetId());
	}
		
	/************************************************************************/
	/* Logic                                                                */
	/************************************************************************/
public:
	// 2021/11/10 PKT - Level & Amount 세팅
	void SetData(Level InLevel, MonsterKnowledgeExp InExp);
	
	// 2021/11/10 PKT - Amount 세팅
	void UpdateByKnowledgeExp(MonsterKnowledgeExp InExp);

protected:
	// 2021/11/10 PKT - 변경된 레벨 / 지식 수량에 대해 다음 상태를 계산.
	void CalcAmountKnowledge(Level InLevel, MonsterKnowledgeExp InExp);

	// 2021/11/10 PKT - 몬스터 종적 타입
	CreatureRaceType GetRaceType() const;

	// 2021/11/10 PKT - 몬스터 공격형 타입(선공/비선공)
	bool IsOffensive() const;

	// 2021/11/10 PKT - 몬스터 속성
	MonsterElement GetElemnet() const;

	/************************************************************************/
	/* Get                                                                  */
	/************************************************************************/
public:
	//int32 GetOrderNum() const;

	EMonsterKnowledgeState GetCurrentState() const;
	
	// 2021/11/10 PKT - 몬스터 지식 아이디
	MonsterKnowledgeId GetId() const;

	// 2021/11/10 PKT - 몬스터 이름.
	const FText& GetMonsterName() const;

	// 2021/11/10 PKT - 몬스터 랭크
	CreatureRankType GetCreatureRank() const;

	// 2021/12/07 PKT - 현재 몬스터 지식의 경험치
	MonsterKnowledgeExp GetCurrentExp() const;

	// 2021/12/07 PKT - 레벨당 최대 경험치 및 얻을 수 있는 포인트
	TPair<MonsterKnowledgeExp, int32> GetExpAndPointForLevel(Level InLevel) const;

	// 2021/11/28 PKT - 몬스터 이미지 Path
	const FSoftObjectPath& GetIllustrationPath() const;

	// 2021/11/28 PKT - 몬스터 지식 레벨
	Level GetCurrentLevel() const;

	// 2021/11/28 PKT - 몬스터 지식 레벨 최대치
	Level GetMaxLevel() const;

	/**
	 * UI에서 자주 사용되는 Text를 미리 캐싱함.
	 */
	// 2021/12/07 PKT - 일반/네임드/보스 등등...
	const FText& GetTextCreatureRank() const;
	// 2021/12/07 PKT - 선공/비선공
	const FText& GetTextOffensive() const;
	// 2021/12/07 PKT - 종족 이름
	const FText& GetTextCreatureRace() const;
	// 2021/12/07 PKT - 약점/재질
	const FText& GetTextDefenseMaterial() const;
	// 2021/12/07 PKT - 속성
	const FText& GetTextMaterial() const;
};