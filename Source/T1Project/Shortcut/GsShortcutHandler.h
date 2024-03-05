// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Message/GsMessageUI.h"
#include "../Message/MessageParam/GsMessageParam.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

/**
 * 
 */
class T1PROJECT_API FGsShortcutHandler
{
public:
	FGsShortcutHandler() = default;
	virtual ~FGsShortcutHandler() = default;

protected:
	MsgUIHandleArray _uiMsgList;

public:
	// 초기화
	void Init();
	// 종료
	void Close();

public:
	void OnShortcutContents(const IGsMessageParam* inParam);
	void OnShortcutContentsMapId(IN int32 inMapId);
	void OnShortcutContentsItemId(IN ItemId inMapId);
	void OnShortcutContentsCreatureId(IN CreatureId inCreatureId);
};
