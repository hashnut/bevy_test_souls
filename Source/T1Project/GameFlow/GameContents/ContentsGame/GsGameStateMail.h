// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsContentsGameBase.h"
#include "Message/GsMessageContents.h"

class UGsUIWindowMail;
class UGsUIWindowMailDetail;
struct IGsMessageParam;
/**
 * ∏ﬁ¿œ
 */
class T1PROJECT_API FGsGameStateMail : public FGsContentsGameBase
{
private:
	TWeakObjectPtr<class UGsUIWindowMail>		_windowMailUI;
	TWeakObjectPtr<class UGsUIWindowMailDetail>	_windowMailDetailUI;

private:
	TArray<TPair<MessageContentMail, FDelegateHandle>>	_listMailDelegate;

public:
	FGsGameStateMail();
	virtual ~FGsGameStateMail();

public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float In_deltaTime) override;

	virtual void Init() override;
	virtual void Close() override;

protected:
	void RegisterMessages();
	void UnregisterMessages();

	void WindowUIInvalidateAll(const IGsMessageParam* InParam);
	void OpenWindowMailDetail(const IGsMessageParam* InParam);
};
