// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataSchema/Tutorial/GsSchemaTutorialAction.h"
#include "Tutorial/GsTutorialEnumDefine.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

class UGsUITrayTutorial;
struct IGsMessageParam;


/**
 * 튜토리얼에서 수행되는 동작 단위
 */
class T1PROJECT_API FGsTutorialActionBase
{
protected:
	const FGsSchemaTutorialAction* _table = nullptr;
	TWeakObjectPtr<UGsUITrayTutorial> _tutorialUI;
	bool _bIsComplete = false;

	bool _bIsInteractionComplete = false; // 인터렉션이랑 다른 조건 순서때문에 꼬이는 경우발생하여 둘다 체크
	bool _bIsEndConditionComplete = false;

public:
	FGsTutorialActionBase() = default;
	virtual ~FGsTutorialActionBase() = default;

public:
	virtual void Initialize(const FGsSchemaTutorialAction* InTable);
	virtual void Finalize() {}

public:
	// 필요할 경우 구현
	virtual void OnPrevStart(const IGsMessageParam* InParam) {}

	virtual bool OnStart();
	virtual void OnEnd(bool bInSendMessage = true);
	virtual bool IsEnd() { return _bIsComplete; }

	// 특정 자손에서 상속받아 구현할 함수들
public:
	// UGsUITrayTutorial 에서 UI 클릭이나 롱프레스 등 행위가 완료되면 들어옴
	virtual void OnEndUIInteraction();
	// UI가 열리면 들어옴
	virtual void OnOpenUI(const IGsMessageParam* InParam);

	virtual void OnLearnSkill(SkillId InSkillId) {}
	virtual void OnSummonButtonState(const IGsMessageParam* InParam);

protected:
	virtual bool CheckEndCondition();

protected:
	void SetEndCondition(bool bIsComplete) { _bIsEndConditionComplete = bIsComplete; }

public:
	int32 GetActionId() const { return (_table) ? _table->id : 0; }
	EGsTutorialActionType GetActionType() const;
};
