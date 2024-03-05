#pragma once
#include "CoreMinimal.h"
#include "Costume/GsCostumeSummonConfirmData.h"
#include "GsUIListDataCostumeSummonProbability.generated.h"

UCLASS()
class T1PROJECT_API UGsUIListDataCostumeSummonProbability : public UObject
{
	GENERATED_BODY()

protected:
	FGsCostumeSummonChangeData  _data;

public:
	void SetSummonChangeData(const FGsCostumeSummonChangeData& data) { _data = data; }
	const FGsCostumeSummonChangeData& GetSummonChangeData() { return _data; }

};