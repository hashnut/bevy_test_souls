// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class FGsPlayerDetailInfoMsgReceiver;
/**
 * 
 */
class T1PROJECT_API FGsPlayerDetailInfoHandler
{
private:
	FGsPlayerDetailInfoMsgReceiver* _msgReciver;

public:
	FGsPlayerDetailInfoHandler() = default;
	virtual ~FGsPlayerDetailInfoHandler();

public:
	void Init();
	void Close();
	void OnReconection();
};