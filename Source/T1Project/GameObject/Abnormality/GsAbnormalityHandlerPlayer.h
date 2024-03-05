// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsAbnormalityHandlerBase.h"
#include "Shared/Client/SharedEnums/SharedPassivityEnum.h"
#include "Message/GsMessageGameObject.h"

/**
 * 
 */
class T1PROJECT_API FGsAbnormalityHandlerPlayer : 
	public FGsAbnormalityHandlerBase
{
	using Super = FGsAbnormalityHandlerBase;

private:
	TArray<MsgGameObjHandle> _listGameObjectMsgHandler;

public:
	FGsAbnormalityHandlerPlayer() = default;
	virtual ~FGsAbnormalityHandlerPlayer() = default;

public:
	virtual void Initialize(class UGsGameObjectBase* Owner) override;
	virtual void Finalize() override;

protected:
	virtual void OnActive(AbnormalityInstanceId InstanceId, TWeakPtr<FGsAbnormalityData> inData, bool IsNewDataAdded, bool isSpawnEffect = true) override;
	virtual void OnDeActive(AbnormalityInstanceId InstanceId, TWeakPtr<FGsAbnormalityData> inData) override;

public:
	virtual bool IsTeamMate() override;

private:
	void CallbackChnageParts(const struct IGsMessageParam* Param);
};
