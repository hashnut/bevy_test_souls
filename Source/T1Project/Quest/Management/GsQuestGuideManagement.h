#pragma once

#include "CoreMinimal.h"
#include "Management/GsScopeHolder.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageUI.h"
#include "Quest/GsQuestData.h"
#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"
#include "Quest/Table/GsQuestGuideTableCacheSet.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"

class FGsQuestGuideTableCacheSet;
/*
    퀘스트 가이드 처리
*/
class T1PROJECT_API FGsQuestGuideManagement
{  
protected:
    enum GuideStateType
    {
        Completed,
        Progress,
        Waiting,
    };

    // Update 시, 가이드 퀘스트 갯수를 추가/제거/유지할 것인지 결정
    enum GuideListUpdateType
    {
        Add,        // 유저가 선택한 것만 추가 (자동 추가 X)
        Subtract,   // 유저가 선택한 것만 제거 (자동 추가 X)
        Preserve,   // HUD 노출된 가이드 퀘스트 갯수 변동 X
        None,
    };

protected:
    TArray<TPair<MessageContentQuest, FDelegateHandle>>	_listQuestDelegate;
    MsgUIHandleArray                                    _listUIDelegate;
    MsgGameObjHandleArray	                            _listGameObjectDelegate;

    FGsQuestGuideTableCacheSet*                         _guideQuestTableCacheSet;

    TArray<StoryId>						                _totalOnList;
    TArray<StoryId>						                _systemOnList;
    TArray<StoryId>						                _userOnList;
    TArray<StoryId>						                _userOffList;

public:
    FGsQuestGuideManagement() = default;
	~FGsQuestGuideManagement() = default;

public:
    // 초기화
    void Initialize(FGsQuestGuideTableCacheSet* inGuideQuestTableCacheSet);
    // 클리어
    void Finalize();
	// 로비로 넘어갈때 호출되는 함수
	void Clear();
private:
    void BindMessages();
    void UnbindMessages();

protected:
    void RecvQuestMainRewardedMessge(const IGsMessageParam* In_data);
    void RecvQuestGuideAcceptedMessge(const IGsMessageParam* In_data);
    void RecvQuestGuideCompletedMessge(const IGsMessageParam* In_data);
    void RecvQuestGuideRewardedMessge(const IGsMessageParam* In_data);
    void RecvQuestGuideHudOnOff(const IGsMessageParam* In_data);
    void OnChangeLocalPlayerLevel(const IGsMessageParam* In_data);
    void OnLocalPlayerSpawnComplete(const IGsMessageParam* In_data);

private:
    StoryId GetStoryId(GuideStateType inType);
    void InvalidateCheck();

private:
    // 현재 수락한 HUD 에 띄운 가이드 퀘스트 갯수가 지정 갯수보다 적을 경우, 추가 여부를 InAutoAdd 로 제어
    void Update(GuideListUpdateType InType = GuideListUpdateType::None);
    bool Claim(OUT StoryId& outStoryId);

    bool GetStoryCompleted(OUT StoryId& outStoryId);
    bool GetStoryProgress(OUT StoryId& outStoryId);
    bool GetStoryWaiting(OUT StoryId& outStoryId);

public:
    TArray<StoryId> GetGuideVaildList() { return _totalOnList; }
    bool IsShow(IN StoryId inStoryId) { return _totalOnList.Contains(inStoryId); }
};