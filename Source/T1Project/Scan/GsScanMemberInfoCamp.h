#pragma once
#include "Scan/GsScanMemberInfo.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

class FGsScanMemberInfoCamp : public FGsScanMemberInfo
{

	// 생성자
public:
	FGsScanMemberInfoCamp(int In_orderNumber,
		TWeakObjectPtr<UGsGameObjectBase> In_targetObject,
		TWeakObjectPtr<UGsGameObjectBase> In_localObject) :
		FGsScanMemberInfo(In_orderNumber, In_targetObject, In_localObject)
	{
		UpdateName();
	}

	// virtual func
public:
	// update name
	virtual void UpdateName() override;

	// get
public:
	// 길드 엠블렘 id
	GuildEmblemId GetGuildEmblemId();
	// 길드에 속했는지
	bool GetIsInGuild();

	uint64 GetGuildDbId();
};