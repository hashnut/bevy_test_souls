// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Message/MessageParam/GsMessageParam.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Skill/GsSkillUIHandler.h"
#include "GameFlow/Stage/GsStageMode.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Skill/GsSkillSlotData.h"
#include "Message/GsMessageStage.h"

/**
 * uint32 값 전달 시 사용
 */
struct FGsFence;

struct FGsStageFenceMsgParam : public IGsMessageParam
{
	bool _isSafety;
	bool _isShowNpc;
	const FGsFence* _fence;

	FGsStageFenceMsgParam() = default;
	virtual ~FGsStageFenceMsgParam() = default;
	explicit FGsStageFenceMsgParam(bool InisSafety, bool InisShowNpc, const FGsFence* inFence)
		: _isSafety(InisSafety), _isShowNpc(InisShowNpc), _fence(inFence)
	{
	}
};

struct FGsGameStageEnterMsgParam : public IGsMessageParam
{
	FGsStageMode::Game _type;

	FGsGameStageEnterMsgParam() = default;
	virtual ~FGsGameStageEnterMsgParam() = default;
	explicit FGsGameStageEnterMsgParam(FGsStageMode::Game InType) : _type(InType)
	{
	}
};

struct FGsLobbyFlowMsgParam : public IGsMessageParam
{
	MessageStage _messageType;

	FGsLobbyFlowMsgParam() = default;
	virtual ~FGsLobbyFlowMsgParam() = default;
	explicit FGsLobbyFlowMsgParam(MessageStage InMessageType) : _messageType(InMessageType)
	{
	}
};