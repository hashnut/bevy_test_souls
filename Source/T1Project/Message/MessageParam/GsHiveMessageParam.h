// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameObject/Define/GsGameObjectDefine.h"
#include "Message/MessageParam/GsMessageParam.h"

/**
 * hive push 알림 상태
 */
struct T1PROJECT_API FGsHivePushMessageParam final : public IGsMessageParam
{
public:
	// 야간 알람
	bool _night = false;
	// 모든 알람
	bool _allAlert = false;

public:
	FGsHivePushMessageParam() = default;
	explicit FGsHivePushMessageParam(
		bool inNight,
		bool inAllAlert)
		:
		_night(inNight),
		_allAlert(inAllAlert)
	{
	}
	virtual ~FGsHivePushMessageParam() = default;
};
