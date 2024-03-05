#pragma once

#include "CoreMinimal.h"
#include "../Classes/GsManager.h"
#include "../RedDot/GsRedDotCheck.h"
#include "../Currency/GsCostPackage.h"
#include "Codex/GsCodexTypes.h"
#include "Codex/GsCodexExplorePassivityCollection.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"

struct FGsSchemaNpcData;

/**
 * 도감 > 몬스터 일람 쪽 구현은 앞으로 Codex 가 관리한다.
 * MonsterCollection 대신 Codex 라는 이름을 사용하기로 했다. (석판은 기존 구현을 계승)
 * MonsterCollection 에는 석판 관련 코드를, Codex 에는 일람 관련 코드를 찾아볼 수 있다.
 * CategoryData 나 MapGroupData 클래스를 만드는 것이 좋으나, 만들지 않으면 코드가 어떻게 되는지 비교해 보자.
 */
class T1PROJECT_API FGsCodexManager final : public IGsManager, public IGsRedDotCheck
{
private:
	/** 맵 ID -> 맵 그룹 맵핑*/
	TMap<MapId, CodexMapGroupId> mapToMapGroup;


	/** 맵 그룹 -> 맵 ID 셋*/
	TMap<CodexMapGroupId, TSet<MapId>> mapGroupToMap;

	/** 맵 그룹 -> 맵 그룹 이름 */
	TArray<TPair<CodexMapGroupId, FText>> mapGroupIdNamePairList;
	TMap<CodexMapGroupId, FText> mapGroupToMapGroupName;
	
	/** 맵 그룹 -> 맵 그룹 이미지 */
	TArray<TPair<CodexMapGroupId, FSoftObjectPath>> mapGroupIdImagePairList;
	TMap<CodexMapGroupId, FSoftObjectPath> mapGroupToMapGroupImage;

	/** 해당 맵 그룹에서 사용할 수 있는 Ingredient 의 ItemCommon Id 셋 */
	TMap<CodexMapGroupId, TArray<ItemId>> mapGroupToIngredients;

	/** 맵 그룹 -> 각 레벨당 필요한 경험치 리스트(인덱스 0부터 1레벨) 맵핑 */
	TMap<CodexMapGroupId, TArray<int32>> mapGroupToExpList;

	/** 맵 그룹 -> 각 레벨당 필요한 누적 경험치 리스트(인덱스 0부터 1레벨) 맵핑 */
	TMap<CodexMapGroupId, TArray<int32>> mapGroupToAccumulatedExpList;

	/** 맵 그룹 -> 카테고리 맵핑 */
	TMap<CodexMapGroupId, CodexCategoryId> mapGroupToCategory;


	/** 카테고리 -> 맵 그룹 리스트 */
	TMap<CodexCategoryId, TArray<CodexMapGroupId>> categoryToMapGroupList;

	TMap<CodexCategoryId, TSet<MapId>> categoryToMap;

	/** 카테고리 -> 맵 카테고리 이름 */
	TArray<TPair<CodexCategoryId, FText>> categoryIdNamePairList;
	TMap<CodexCategoryId, FText> categoryToCategoryName;

	/** 카테고리 -> 맵 그룹들의 레벨 업 총합과 PassivitySet Id 리스트 맵핑 (대륙탐사에 필요) */
	TMap<CodexCategoryId, TArray<CategoryBalanceData>> categoryToCategoryBalanceDataList;

	/** 카테고리 -> 레벨 -> 다음 레벨업에 필요한 맵 그룹 레벨 총합 (아주 약간의 최적화) */
	TMap<CodexCategoryId, TMap<CodexCategoryLevel, int32>> categoryToLevelToRequireLevelSum;

	/** 카테고리 -> 배경 이미지 */
	TMap<CodexCategoryId, FSoftObjectPath> categoryToBG;


	/** 카테고리 -> 현재 레벨 */
	TMap<CodexCategoryId, int32> categoryToCurLevel;

	/** 맵 그룹 -> 현재 레벨 및 경험치 */
	TMap<CodexMapGroupId, TPair<int32, int32>> mapGroupToCurLevelExpPair;

	
	/** 특정 맵 그룹 선택 시, 보여줄 몬스터 목록 (Grade 별로 구분, 동일맵/동일 NpcId 는 1개만) */
	TMap<CodexMapGroupId, TMap<CreatureRankType, TArray<const FGsSchemaNpcData*>>> mapGroupToRankToNpcList;

	/** 특정 맵 그룹 선택 시, 보여줄 몬스터 목록 (Grade 기준 정렬, 동일맵/동일 NpcId 는 1개만) */
	TMap<CodexMapGroupId, TArray<TPair<MapId, const FGsSchemaNpcData*>>> mapGroupToMapNpcPairList;


private:
	/** 총 획득 지식 */
	MonsterKnowledgeExp _accumulateMonsterKnowledgeExp;

	/** 도감 포인트로 변환 가능한 몬스터 지식 */
	MonsterKnowledgeExp _finalMonsterKnowledgeExp;

	/** 몬스터 도감 최종 포인트 */
	Currency _finalMonsterBookPoint;

	/** 도감 > 몬스터 일람에서 지식이 얼마나 쌓여야 도감 포인트로 변환할지 레드닷을 띄워주는 기준값 */
	int32 _codexKnowledgeThresholdForRedDot = 200;

	/** 지식의 결정 ItemId -> 획득 지식 */
	TMap<ItemId, int32> _knowledgeGemIdToGainAmount;

	/** 대륙 탐사 컬렉션 효과*/
	FGsCodexExplorePassivityCollection _explorePassivityCollectionData;

private:
	/** 특정 맵 그룹 선택 시, 보여줄 몬스터 목록 (Grade 별로 구분) 캐싱 */
	void BuildMapGroupToNpcList();

	/** 지식의 결정에 따른 지식 증가 수치 캐싱 */
	void BuildGemIdToKnowledGainAmount();

public:
	virtual void Initialize() final;
	virtual void Finalize() final;

	virtual bool CheckRedDot(bool bInUpdate = true) override;

	/** 지식 획득 레드닷 여부 체크 */
	bool CheckRedDotKnowledge();

	/** 대륙 탐사 레드닷 여부 체크 */
	bool CheckRedDotCategory();

	/** 레드닷 띄워주어야 할 대륙 정보 */
	TArray<CodexCategoryId> GetRedDotCategoryList();

	/** 컬렉션 데이터 파싱 */
	void InvalidateExplorePassivityCollectionData();


	/************************************************************************/
	/* 서버 -> 클라                                                         */
	/************************************************************************/
public:
	/** 월드 접속 시, 몬스터 일람 정보 모두 전송 */
	void NotifyCodex(CodexCategoryId InId, CodexCategoryLevel InLevel, TArray<TTuple<int32, int32, int32>>&& InMapGroupToLevelExpPair);

	/** 몬스터 도감 맵 그룹 경험치 업데이트 패킷 */
	void NotifyCodexMapGroupExpUpdate(int32 InMonsterKnowledgeExp, CodexMapGroupId InId, CodexMapGroupLevel InLevel, CodexMapGroupExp InExp);

	/** 요청 카테고리의 몬스터 일람 정보 및 지식/도감 포인트 정보 응답 */
	void AckCodex(CodexCategoryId InId, CodexCategoryLevel InLevel, TArray<TTuple<int32, int32, int32>>&& InMapGroupToLevelExpPair, MonsterKnowledgeExp InAccumulateExp, MonsterKnowledgeExp InFinalExp, Currency InFinalMonsterBookPoint);

	/** 몬스터 지식을 도감 포인트로 변환 응답 */
	void AckMonsterBookPointExchange(Currency InFinalMonsterBookPoint, MonsterKnowledgeExp InFinalMonsterKnowledgeExp);

	/** 몬스터 일람 대륙 탐사 레벨업 응답 */
	void AckCodexCategoryLevelUp(CodexCategoryId InId, CodexCategoryLevel InLevel);

	/** 몬스터 일람에서 지식의 결정 아이템 사용 */
	void AckMonsterKnowledgeExpGainItemUse(CodexMapGroupId InId, CodexMapGroupLevel InLevel, CodexMapGroupExp InExp);

	
	/************************************************************************/
	/* Getter                                                               */
	/************************************************************************/
public:
	/** 총 획득 지식 */
	MonsterKnowledgeExp GetAccumulateMonsterKnowledgeExp() const;

	/** 변환 가능한 지식 */
	MonsterKnowledgeExp GetFinalMonsterKnowledgeExp() const;


	/** 인덱스 순서대로 CategoryId 조회 */
	CodexCategoryId GetCategoryIdWithIndex(int32 InIndex) const;

	/** CategoryId 로 카테고리 이름 조회 */
	FText GetCategoryNameWithId(CodexCategoryId InId) const;

	/** CategoryId 로 MapGroupList 조회 */
	TArray<CodexMapGroupId> GetMapGroupListWithId(CodexCategoryId InId) const;

	/** CategoryId 로 밸런스 데이터 리스트 조회 */
	TArray<CategoryBalanceData> GetCategoryBalanceDataListWithId(CodexCategoryId InId) const;

	/** CategoryId 로 현재 카테고리 레벨 조회 */
	CodexCategoryLevel GetCurrentCategoryLevelWithId(CodexCategoryId InId) const;
	
	/** CategoryId 로 현재 카테고리 내의 맵그룹 레벨 합산치 조회 */
	CodexCategoryLevel GetCurrentCategoryMapGroupLevelSumWithId(CodexCategoryId InId) const;


	/** 인덱스 순서대로 MapGroupId 조회 */
	CodexMapGroupId GetMapGroupIdWithIndex(int32 InIndex) const;

	/** MapGroupId 로 카테고리 이름 조회 */
	FText GetMapGroupNameWithId(CodexMapGroupId InId) const;

	/** MapGroupId 로 만렙 여부 조회 */
	bool GetMapGroupIsMaxLevelWithId(CodexMapGroupId InId) const;

	/** MapGroupId 로 현재 레벨 조회 */
	int32 GetMapGroupCurrentLevelWithId(CodexMapGroupId InId) const;

	/** MapGroupId 로 현재 경험치 조회 */
	CodexMapGroupExp GetMapGroupCurrentExpWithId(CodexMapGroupId InId) const;

	/** MapGroupId 로 현재 레벨 대비 경험치 조회 */
	float GetMapGroupCurrentExpPercentWithId(CodexMapGroupId InId) const;

	/** MapGroupId 로 맵 그룹 이미지 조회 */
	FSoftObjectPath GetMapGroupImageWithId(CodexMapGroupId InId) const;


	/** MapGroupId 와 NPC Grade 를 통해 NPC 목록 조회 */
	TArray<const FGsSchemaNpcData*> GetNpcDataListWithIdAndGrade(CodexMapGroupId InId, CreatureRankType InGrade);
	
	/** MapGroupId 를 통해 NPC 목록 조회 */
	TArray<TPair<MapId, const FGsSchemaNpcData*>> GetNpcDataListWithId(CodexMapGroupId InId);

	/** 전체 카테고리 갯수 */
	int32 GetTotalCategoryCount();

	/** MapId 로 MapGroupId 조회 */
	CodexMapGroupId GetMapGroupIdWithMapId(int32 InMapId);

	/** MapGroupId 로 CategoryId 조회 */
	CodexCategoryId GetCategoryIdWithMapGroupId(CodexMapGroupId InMapGroupId);

	/** MapId 로 CategoryId 조회 */
	CodexCategoryId GetCategoryIdWithMapId(int32 InMapId);

	/** CategoryId 로 현재 카테고리의 순서 조회 */
	int32 GetIndexWithCategoryId(CodexCategoryId InId);

	/** 지식의 결정 Id 값으로 지식 획득량 조회 */
	int32 GetKnowledgeGainAmountWithItemId(ItemId InId);

	/** MapGroupId 로 사용 가능한 지식 결정 Id 리스트 */
	TArray<ItemId> GetKnowledgeGemListWithMapGroupId(CodexMapGroupId InId);

	/** MapGroupId 로 필요 지식 리스트 조회 (단계별) */
	TArray<int32> GetKnowledgeRequirementListWithMapGroupId(CodexMapGroupId InId);

	/** MapGroupId 로 필요 지식 리스트 조회 (누적) */
	TArray<int32> GetKnowledgeAccumlatedRequirementListWithMapGroupId(CodexMapGroupId InId);

	/** MapGroupId 로 해당 레벨에 필요한 누적 지식 리스트 조회 */
	TArray<int32> GetAccumulateKnowledgeExpListWithMapGroupId(CodexMapGroupId InId);

	/** MapGroupId 와 레벨로 해당 레벨에 필요한 누적 지식 조회 */
	CodexMapGroupExp GetAccumulateKnowledgeExpWithMapGroupIdAndLevel(CodexMapGroupId InId, int32 InLevel);

	/** MapGroupId 의 최대 레벨을 찍는데 필요한 경험치 */
	CodexMapGroupExp GetMaxExpWithMapGroupId(CodexMapGroupId InId);

	/** 대륙탐사 컬렉션 조회 */
	const FGsCodexExplorePassivityCollection* GetExplorePassivityCollectionData() const;

	/** 카테고리 Id 로 배경 이미지 조회 */
	FSoftObjectPath GetCategoryBGWithCategoryId(CodexCategoryId InId) const;

	/** 카테고리 Id 와 카테고리 레벨로 Passivity 에 맞는 FText 조회 */
	FText GetPassivityTextWithCategoryIdAndLevel(CodexCategoryId InId, CodexCategoryLevel InLevel);
};

#define GsCodex() UGsScopeHolder::GetGameManagerFType<FGsCodexManager>(UGsScopeGame::EManagerType::Codex)