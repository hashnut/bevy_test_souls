// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Message/MessageParam/GsMessageParam.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Summon/GsSummonDefine.h"

/**
 * int형  정보 하나 보낼 때(tutorialActionId 등)
 */
struct FGsTutorialMsgParamInt : public IGsMessageParam
{
	int32 _data;

	FGsTutorialMsgParamInt() = default;
	explicit FGsTutorialMsgParamInt(int32 InData)
		: _data(InData)
	{
	}

	virtual ~FGsTutorialMsgParamInt() = default;
};

/**
 * uint32형  정보 하나 보낼 때
 */
struct FGsTutorialMsgParamUInt32 : public IGsMessageParam
{
	uint32 _data;

	FGsTutorialMsgParamUInt32() = default;
	explicit FGsTutorialMsgParamUInt32(uint32 InData)
		: _data(InData)
	{
	}

	virtual ~FGsTutorialMsgParamUInt32() = default;
};

/**
 * 
 */
struct FGsTutorialMsgParamTutorialId : public IGsMessageParam
{
	TutorialId _data;

	FGsTutorialMsgParamTutorialId() = default;
	explicit FGsTutorialMsgParamTutorialId(TutorialId InData)
		: _data(InData)
	{
	}

	virtual ~FGsTutorialMsgParamTutorialId() = default;
};

/**
 * 
 */
struct FGsTutorialMsgParamName : public IGsMessageParam
{
	FName _data;

	FGsTutorialMsgParamName() = default;
	explicit FGsTutorialMsgParamName(const FName& InData)
		: _data(InData)
	{
	}

	virtual ~FGsTutorialMsgParamName() = default;
};

/**
 * 스킬 습득 튜토리얼
 */
struct FGsTutorialMsgParamSkill : public IGsMessageParam
{
	SkillId _data;

	FGsTutorialMsgParamSkill() = default;
	explicit FGsTutorialMsgParamSkill(SkillId InData)
		: _data(InData)
	{
	}

	virtual ~FGsTutorialMsgParamSkill() = default;
};

/**
 * 소환 관련 튜토리얼
 */
struct FGsTutorialMsgParamSummonButtonState : public IGsMessageParam
{
	EGsSummonBtnType _data;

	FGsTutorialMsgParamSummonButtonState() = default;
	explicit FGsTutorialMsgParamSummonButtonState(EGsSummonBtnType InData)
		: _data(InData)
	{
	}

	virtual ~FGsTutorialMsgParamSummonButtonState() = default;
};

/**
 * 조건 체크 시
 */
struct FGsTutorialMsgParamOpenUI : public IGsMessageParam
{
	FName _widgetKey;
	FString _paramStr;
	int32 _paramInt;

	// 주의: InParamInt 기본값이 -1임에 유의(0부터 시작하는 인덱스 등의 체크 위함)
	FGsTutorialMsgParamOpenUI() = default;
	explicit FGsTutorialMsgParamOpenUI(const FName& InWidgetKey, const FString& InParamStr, int32 InParamInt = -1)
		: _widgetKey(InWidgetKey), _paramStr(InParamStr), _paramInt(InParamInt)
	{
	}
	explicit FGsTutorialMsgParamOpenUI(const FName& InWidgetKey, int32 InParamInt)
		: _widgetKey(InWidgetKey), _paramInt(InParamInt)
	{
		_paramStr.Empty();
	}

	virtual ~FGsTutorialMsgParamOpenUI() = default;
};
