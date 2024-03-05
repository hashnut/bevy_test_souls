// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Message/MessageParam/GsMessageParam.h"

struct FGsViewPortFocusParam : public IGsMessageParam
{
	bool _isReceivedFocus = true;

	virtual ~FGsViewPortFocusParam() = default;
	explicit FGsViewPortFocusParam(bool inIsReceivedFocus) : _isReceivedFocus(inIsReceivedFocus)
	{
	}
};

struct FGsInitSmokeSignal final : public IGsMessageParam
{

public:
	virtual ~FGsInitSmokeSignal() = default;
	void Send() const
	{
		FGsMessageHolder* MessageHolder = GMessage();
		if (nullptr == MessageHolder)
		{
			GSLOG(Error, TEXT("nullptr == MessageHolder"));
			return;
		}

		MessageHolder->GetSystemParam().SendMessage(MessageSystem::INIT_SMOKE_SIGNAL, this);
	}
};

struct FGsSmokeSignal final : public IGsMessageParam
{
public:
	FGsSmokeSignal(FString inData) : _data(MoveTemp(inData)) {}
	virtual ~FGsSmokeSignal() = default;
public:
	const FString _data;

	void Send() const
	{
		FGsMessageHolder* MessageHolder = GMessage();
		if (nullptr == MessageHolder)
		{
			GSLOG(Error, TEXT("nullptr == MessageHolder"));
			return;
		}

		MessageHolder->GetSystemParam().SendMessage(MessageSystem::SEND_SMOKE_SIGNAL, this);
	}
};