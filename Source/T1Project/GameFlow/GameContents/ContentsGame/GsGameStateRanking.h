// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsContentsGameBase.h"
#include "Message/GsMessageContents.h"

class UGsUIWindowRanking;
struct IGsMessageParam;

/**
 * 랭킹시스템 로직
 */
class T1PROJECT_API FGsGameStateRanking : public FGsContentsGameBase
{
private:
	TArray<TPair<MessageContentRanking, FDelegateHandle>> _rankingMsgHandlerList;
	TWeakObjectPtr<UGsUIWindowRanking> _windowUI;

public:
	FGsGameStateRanking();
	virtual ~FGsGameStateRanking();

public:
	virtual void Enter() override;
	virtual void Exit() override;

private:
	void RegisterMessages();
	void RemoveMessage();
	void OnUpdateRankingPage(const IGsMessageParam* InParam);

};
