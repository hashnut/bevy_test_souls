#pragma once

#include "CoreMinimal.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "Shared/Client/SharedEnums/SharedMonsterknowledgeEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "EMonsterKnowledgeEnumData.h"

struct FGsSchemaSlateSpecialNode;
class FGsCostPackage;

class FGsSlateSpecialNode final
{
private:
	const FGsSchemaSlateSpecialNode* _schema = nullptr;

	EMonsterStoneSlateNodeState _state = EMonsterStoneSlateNodeState::Disable;

	TPair<StatType, StatValue> _effectStatSet;

	TSharedRef<FGsCostPackage> _requireCost;


public:
	FGsSlateSpecialNode(const FGsSchemaSlateSpecialNode* InSchema);
	
	bool operator==(const FGsSlateSpecialNode& InNode)
	{
		return InNode.GetId() == GetId();
	}

	bool operator==(SlateSpecialNodeId InId)
	{
		return InId == GetId();
	}

	/************************************************************************/
	/* Setter                                                               */
	/************************************************************************/
public:
	void SetState(EMonsterStoneSlateNodeState InState);


	/************************************************************************/
	/* Getter                                                               */
	/************************************************************************/
public:
	SlateSpecialNodeId GetId() const;

	int32 GetProbability() const;
	
	TSharedRef<FGsCostPackage> GetCostPackage() const;

	PassivityId GetPassivityId() const;

	const TPair<StatType, StatValue>& StatSet() const;

	const FSoftObjectPath& GetNodeSpritePath() const;

	const FSoftObjectPath& GetNodeSpriteActivePath() const;

	EMonsterStoneSlateNodeState GetState() const;

	const FGsSchemaSlateSpecialNode* GetSchemaData() const;

private:
	TPair<StatType, StatValue> BuildEffectStat(PassivityId InPassivityId) const;
};