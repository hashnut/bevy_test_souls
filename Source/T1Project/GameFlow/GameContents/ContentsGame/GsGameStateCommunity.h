// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsContentsGameBase.h"
#include "Message/GsMessageContents.h"
#include "CommunityGirl/GsCommunityGirlDialogHandler.h"

struct IGsMessageParam;
class UGsUIWindowCommunity;
class AGsActorCommunityNPCScene;

/**
 * 커뮤니티 소녀
 */
class T1PROJECT_API FGsGameStateCommunity : public FGsContentsGameBase
{
private:
	TWeakObjectPtr<class UGsUIWindowCommunity>	_windowCommunityUI;
	TWeakObjectPtr<class AGsActorCommunityNPCScene>	_actorScene;

	TArray<TPair<MessageContentCommunity, FDelegateHandle>>	_communityGirlDelegates;
	TUniquePtr<FGsComunityGirlDialogHandler> _dialogHandler;

public:
	FGsGameStateCommunity();
	virtual ~FGsGameStateCommunity();

public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float In_deltaTime) override;

	virtual void Init() override;
	virtual void Close() override;

protected:
	void RegisterMessages();
	void UnregisterMessages();

	void Finialize(bool In_isSendEventProgress);

	void WindowUIInvalidateAll(const IGsMessageParam* InParam);
	void ActionChangeGirlTabAction(const IGsMessageParam* InParam);
	void AnimStateStart(const IGsMessageParam* InParam);

	// 이벤트 프로그래스 종료
	// https://jira.com2us.com/jira/browse/C2URWQ-5168
	void FinishEventProgress(const IGsMessageParam* InParam);
	void OnEventExpire(const IGsMessageParam* InParam);
};
