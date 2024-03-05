// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataSchema/Tutorial/GsSchemaTutorial.h"
#include "DataCenter/Public/Tutorial/GsTutorialEnumDefine.h"

struct FGsSchemaTutorial;
struct IGsMessageParam;
class FGsTutorialConditionBase;
class FGsTutorialActionBase;


/**
 * 튜토리얼 객체
 */
class T1PROJECT_API FGsTutorialBase
{
protected:
	int32 _tutorialId = 0;
	const FGsSchemaTutorial* _table = nullptr;
	TSharedPtr<FGsTutorialConditionBase> _condition;
	TArray<TSharedPtr<FGsTutorialActionBase>> _actionList;
	TWeakPtr<FGsTutorialActionBase> _currAction;
	bool _bIsComplete = false;
	
public:
	FGsTutorialBase() = default;
	virtual ~FGsTutorialBase();

public:
	virtual void Initialize(const FGsSchemaTutorial* InTable);
	virtual void Finalize();
	virtual void Reset();

public:
	virtual bool CheckStartCondition(const IGsMessageParam* InParam);
	// 각 액션이 완료될 때 들어옴
	virtual void OnEndAction(int32 InTutorialActionId);

	// 큐에 들어있다가 정작 발동시점에 없는 경우를 위해 체크
	virtual bool CheckIsValidToStart();

protected:
	// 튜토리얼 시작 전 액션 생성 및 사전 준비
	virtual bool OnPrevStart();

	// 튜토리얼 실제 시작
	virtual bool OnStart();

	// 튜토리얼 실제 종료. END_TUTORIAL 메시지 발송
	virtual void OnEnd(bool bInSendMessage = true);

	virtual bool StartActionInternal();
	
public:
	bool StartTutorial();
	void ForcedEndTutorial();

	int32 GetTutorialId() const { return _tutorialId; }
	EGsTutorialConditionType GetConditionType() const { return _table ? _table->conditionType : EGsTutorialConditionType::NONE; }
	TWeakPtr<FGsTutorialActionBase> GetCurrentAction() { return _currAction; }
	bool IsComplete() const { return _bIsComplete; }
	void SetIsComplete(bool bIsComplete) { _bIsComplete = bIsComplete; }
	bool UseStopAuto() const { return _table ? _table->UseStopAuto : false; }
};
