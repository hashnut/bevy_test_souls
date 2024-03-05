#pragma once
#include "CoreMinimal.h"
#include "DataSchema/GsSchemaEnums.h"
#include "DataSchema/CommunityGirl/GsSchemaCommunityDialog.h"
#include "GameObject/Define/GsGameObjectDefine.h"

class T1PROJECT_API FGsComunityGirlDialogHandler
{
public:
	struct DialogProbability
	{
		float _totalProbability;
		TArray<const FGsSchemaCommunityDialog*> _dlgList;
	};
protected:
	TMap<EGsStateBase, DialogProbability> _stateDialogs;

public:
	void InitDialogTable();
	const FGsSchemaCommunityDialog* GetDialog(EGsStateBase state);
};
