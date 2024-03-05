#pragma once

#include "CoreMinimal.h"
#include "Message/GsMessageGameObject.h"
#include "Quest/GsQuestData.h"
#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"

struct IGsMessageParam;
struct FGsObjectiveData;
class UGsGameObjectLocalPlayer;

/*
    퀘스트 타겟 처리
*/
class T1PROJECT_API FGsQuestTargetManagement
{
    // 멤버 변수
private:
	//// 현재 진행중인 몬스터 table id
	TMap<int32, TSet<const QuestKey*>> _mapQuestMonsterId;
	// 현재 진행중인 npc id
	TMap<int32, TSet<const QuestKey*>> _mapQuestNpcId;
	// 현재 진행중인 위치 이동
	TMap<int32, TSet<const QuestKey*>> _mapQuestPos;
	// 현재 진행중인 npc id
	TMap<int32, TSet<const QuestKey*>> _mapQuestPropId;

	// 서브퀘스트를 진행 중인 npc
	// 서브퀘스트 수락을 대기 중인 npc
	TMap<int32, EQuestNpcState> _mapQuestSubNpcId;

	TMap<int32, EQuestNpcState> _mapQuestSubNpcState;

	TArray<TFunction<void(const QuestKey& inQuestKey, int32 inLevelKey, const TArray<FGsObjectiveData>& inObjectiveDataList)>> _addTargetFuncs;
	TArray<TFunction<void(const QuestKey& inQuestKey)>> _removeTargetFuncs;

public:
	FGsQuestTargetManagement() = default;
	~FGsQuestTargetManagement() = default;

    // 로직 함수
public:
    // 초기화
    void Initialize();
    // 클리어
    void Finalize();
	// 로비로 넘어갈때 호출되는 함수
	void Clear();
    // 퀘스트 몬스터인가   
	bool IsQuestMonster(int In_tblId);
	bool IsQuestMonster(int In_tblId, OUT QuestType& outType);
	bool IsActiveQuestMonster(int In_tblId);
    // 퀘스트 npc 인가
	bool IsQuestNpc(int In_tblId);
	// 퀘스트 prop 인가
	bool IsQuestProp(int In_tblId);
	// 퀘스트 서브 npc 인가?
	bool IsQuestSubNpc(int In_tblId);
	// 퀘스트 서브 npc 퀘스트상태에 따른 인터랙션마크 아이콘 index
	bool GetQuestSubNpcStateIconIndex(int In_tblId, OUT int32& outIndex);
   
private:
	// 퀘스트 타겟(몬스터) 추가
	void AddTargetMonster(const QuestKey& inQuestKey, int32 inLevelKey, const TArray<FGsObjectiveData>& inObjectiveDataList);
	// 퀘스트 타겟(npc) 추가
	void AddTargetNpc(const QuestKey& inQuestKey, int32 inLevelKey, const TArray<FGsObjectiveData>& inObjectiveDataList);
	// 퀘스트 타겟(pos) 추가
	void AddTargetPos(const QuestKey& inQuestKey, int32 inLevelKey, const TArray<FGsObjectiveData>& inObjectiveDataList);
	// 퀘스트 타겟(Prop) 추가
	void AddTargetProp(const QuestKey& inQuestKey, int32 inLevelKey, const TArray<FGsObjectiveData>& inObjectiveDataList);

	// 퀘스트 타겟(몬스터) 삭제
	void RemoveTargetNpc(const QuestKey& inQuestKey);
	// 퀘스트 타겟(npc) 삭제
	void RemoveTargetMonster(const QuestKey& inQuestKey);
	// 퀘스트 타겟(pos) 삭제
	void RemoveTargetPos(const QuestKey& inQuestKey);
	// 퀘스트 타겟(Prop) 삭제
	void RemoveTargetProp(const QuestKey& inQuestKey);

public:	
	// 퀘스트 타겟 Add on
	void OnAddTarget(const QuestKey& inQuestKey, const TArray<FGsObjectiveData>& inObjectiveDataList);

	// 수정 필요 --- 직접 호출로 처리 하지말것 !!
	void UpdateTartgetNpcInteractionMark(bool isActive, int32 inNpcID, EQuestNpcState inQuestState = EQuestNpcState::STATE_NONE);
	void UpdateTartgetNpcInteractionMark(bool isActive, TArray<TPair<int32, EQuestNpcState>> inSubNpcList);

	// 퀘스트 타겟 Remove on
	void OnRemoveTarget(const QuestKey& inQuestKey);

private:
	int PrivateGetQuestStateConvertToIndex(EQuestNpcState inState);
};