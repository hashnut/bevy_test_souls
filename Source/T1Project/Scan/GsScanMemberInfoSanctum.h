// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Scan/GsScanMemberInfo.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

class FGsScanMemberInfoSanctum : public FGsScanMemberInfo
{
public:
	FGsScanMemberInfoSanctum(int In_orderNumber,
		TWeakObjectPtr<UGsGameObjectBase> In_targetObject,
		TWeakObjectPtr<UGsGameObjectBase> In_localObject) :
		FGsScanMemberInfo(In_orderNumber, In_targetObject, In_localObject)
	{
		UpdateName();
	}

	virtual ~FGsScanMemberInfoSanctum() {}

	// FGsScanMemberInfo override
public:
	virtual void UpdateName() override;
	
public:
	GuildEmblemId GetGuildEmblemId();
	bool GetIsInGuild();
	uint64 GetGuildDbId();
};