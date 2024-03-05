// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "Message/GsMessageContents.h"

class FGsPlayerDetailInfoHandler;
struct FGsPlayerDetailInfoMessageParamBase;
/**
 * 
 */
class T1PROJECT_API FGsPlayerDetailInfoMsgReceiver
{
private:
	FGsPlayerDetailInfoHandler* _playerDetailInfoHandler;
	TArray<TPair<MessageContentPlayerInfo, FDelegateHandle>>	_listPlayerDetailInfoDelegate;

public:
	FGsPlayerDetailInfoMsgReceiver() = default;
	~FGsPlayerDetailInfoMsgReceiver();

public:
	void RegisterMessage();

private:
	void Open(FGsPlayerDetailInfoMessageParamBase* );
	void Close(FGsPlayerDetailInfoMessageParamBase* );
	void RemoveMessage();

public:
	void SetHandler(FGsPlayerDetailInfoHandler* InHandler) { _playerDetailInfoHandler = InHandler; }

public:
	FGsPlayerDetailInfoHandler* GetHandler() { return _playerDetailInfoHandler; }
};
