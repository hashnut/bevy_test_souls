// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStageGameBase.h"
#include "Message/GsMessageContents.h"

struct IGsMessageParam;


class FGsStageGameAgitDungeon : public FGsStageGameBase
{
public:
	using MsgHandleGuild = TPair<MessageContentGuild, FDelegateHandle>;

private:
	TArray<MsgHandleGuild> _msgHandleGuildList;

public:
	FGsStageGameAgitDungeon();
	virtual ~FGsStageGameAgitDungeon();

public:
	virtual void Enter() override;
	virtual void Exit() override;
	//virtual void Update(float In_deltaTime) override;

private:
	void BindMessages();
	void UnbindMessages();

private:
	void OnDungeonRaidLeave(const IGsMessageParam*);
	void OnDungeonRaidResult(const IGsMessageParam* InParam);
};
