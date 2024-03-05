// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameObject/Define/GsGameObjectDefine.h"
#include "Message/MessageParam/GsMessageParam.h"

/**
 * 인게임에서 사용하는 모든 쿨타임 메세지 처리에 사용되는 ParamData
 */

 using FGsCoolTimeMessageParamBase = IGsMessageParam;

// 쿨타임 정보
struct FGsCoolTimeMessageParam final : public FGsCoolTimeMessageParamBase
{
	EGsCoolTime _type;
	uint64 _groupId;
	int _id;
	float _remainTime;

	FGsCoolTimeMessageParam() = default;
	explicit FGsCoolTimeMessageParam(EGsCoolTime Type, uint64 GroupId, int Id, float RemainTime)
		:
		_type(Type),
		_groupId(GroupId),
		_id(Id),
		_remainTime(RemainTime)
	{
	}
	virtual ~FGsCoolTimeMessageParam() = default;
};