// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Message/MessageParam/GsMessageParam.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

/**
 * 성소 메시지 파라미터
 */
struct FGsSanctumMsgParamSanctumId : public IGsMessageParam
{
	SanctumAreaId _sanctumId = INVALID_SANCTUM_AREA_ID;
	int64 _gameId = 0;

	FGsSanctumMsgParamSanctumId() = default;
	explicit FGsSanctumMsgParamSanctumId(SanctumAreaId InSanctumId, int64 InGameId)
		: _sanctumId(InSanctumId), _gameId(InGameId)
	{
	}

	virtual ~FGsSanctumMsgParamSanctumId() = default;
};
