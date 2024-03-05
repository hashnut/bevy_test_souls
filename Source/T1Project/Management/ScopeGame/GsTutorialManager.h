// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Classes/GsManager.h"
#include "Management/GsScopeHolder.h"
#include "Message/GsMessageContents.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "DataCenter/Public/Tutorial/GsTutorialEnumDefine.h"
#include "Engine/Classes/Engine/EngineTypes.h"

class FGsTutorialBase;
struct IGsMessageParam;
struct FGsSchemaTutorial;

/**
 * 튜토리얼 정보 관리
 */
class T1PROJECT_API FGsTutorialManager :
	public IGsManager
{
private:
	TArray<TutorialId> _completeTutorialIdList;
	TMap<TutorialId, TSharedPtr<FGsTutorialBase>> _tutorialMap;
	TWeakPtr<FGsTutorialBase> _currTutorial;
	TArray<TutorialId> _reservedQueue;
	FTimerHandle _timerHandleEndDelay;

	// 테스트를 위한 변수
	bool _serverNotSave = false;

public:
	FGsTutorialManager() = default;
	virtual ~FGsTutorialManager() = default;

public:
	// IGsManager override
	virtual void Initialize() override;
	virtual void Finalize() override;
	virtual void InitializeAfterResourceDownload() override;

public:
	void SetCompleteTutorialList(PD::SC::PKT_SC_TUTORIAL_READ& InPacket);
	void SetCompleteTutorial(TutorialId InTutorialId);
	// 서버에서 완료된 튜토리얼의 리스트를 받아서, 그거 아닌 튜토리얼만 생성 (액션은 수행할때 생성하자)
	void CreateTutorialMap();
	void SetReserveTutorial(TutorialId InTutorialId);

	// 메시지 받아서 체크할 부분
public:
	void OnEndTutorial(TutorialId InTutorialId);
	void OnEndAction(int32 InTutorialActionId);
	void OnEventActionTutorialStart(const IGsMessageParam* InParam);
	void OnOpenUI(const IGsMessageParam* InParam);	
	void OnOpenUICheckParam(const IGsMessageParam* InParam);
	void OnEndUIInteraction(int32 InTutorialActionId);
	void OnLearnSkill(SkillId InSkillId);
	void OnSummonButtonState(const IGsMessageParam* InParam);
	void OnContentsUnlock(const IGsMessageParam* InParam);
	void OnAcceptQuest(const IGsMessageParam* InParam);
	void OnReadyQuest(const IGsMessageParam* InParam);
	void OnFirstTutorial(const IGsMessageParam* InParam);

	void OnCheckProcessQueue();

	// 튜토리얼 시작/종료
private:
	TutorialId CheckStartCondition(EGsTutorialConditionType InType, const IGsMessageParam* InParam);
	bool StartTutorial(TutorialId InTutorialId, bool bIsForced = false);
	void EndTutorial(TutorialId InTutorialId);
	void EndTutorialInter(TutorialId InTutorialId);
	void ProcessQueue();

	void ClearEndDelayTimer();
	void OnTimerEndDelay();

public:
	bool IsActiveTutorial() const;
	int32 GetActiveTutorialId() const;
	bool IsActiveTutorialUseStopAuto() const;
	bool IsFirstTutorial() const;

private:
	TWeakPtr<FGsTutorialBase> GetTutorial(TutorialId InTutorialId);

public:
	// 치트, 예외 상황(유저사망, 재접속 등)에서 강제로 닫는데 사용
	void ForcedEndTutorial(); 
	
	// 치트를 위해 존재하는 함수들 
public:
	void ForcedStartTutorial(TutorialId InTutorialId);	
	void ForcedResetTutorial();
	void NotSaveTutorial(bool bNotSave);
};

#define GSTutorial() UGsScopeHolder::GetGameManagerFType<FGsTutorialManager>(UGsScopeGame::EManagerType::Tutorial)
