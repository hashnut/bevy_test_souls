// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Message/GsMessageContents.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

struct IGsMessageParam;

/**
 * 기사단 영지 관련, 영지밖에서도 받아서 처리해야 하는 항목들 처리 위함
 */
class FGsAgitHandler final
{
public:
	using MsgHandleGuild = TPair<MessageContentGuild, FDelegateHandle>;

private:
	TArray<MsgHandleGuild> _msgHandleGuildList;

public:
	FGsAgitHandler() = default;
	~FGsAgitHandler() {}

public:
	static bool CheckAgitError(int32 InResult);
	static bool CheckAgitDungeonError(int32 InResult);

public:
	void Init();
	void Close();
	void OnReconnection();

private:
	void BindMessages();
	void UnbindMessages();

private:
	void OnAgitLeave(const IGsMessageParam*);
	void OnAgitAlertDestroy(const IGsMessageParam* InParam);
	void OnAgitDungeonRaidStart(const IGsMessageParam* InParam);
	void OnAgitDungeonRaidClose(const IGsMessageParam* InParam);
	void OnAgitDungeonRaidClear(const IGsMessageParam* InParam);
	void OnAgitDungeonResetYet(const IGsMessageParam*);

private:
	void GetBossNameByRaidId(int32 InRaidId, OUT FText& OutText);
};
