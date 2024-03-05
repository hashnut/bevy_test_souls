#pragma once
#include "CoreMinimal.h"
#include "Fairy/GsFairySummonConfirmData.h"
#include "GsUIListDataFairySummonProbability.generated.h"

UCLASS()
class T1PROJECT_API UGsUIListDataFairySummonProbability : public UObject
{
	GENERATED_BODY()

protected:
	FGsFairySummonChangeData  _data;

public:
	void SetSummonChangeData(const FGsFairySummonChangeData& data) { _data = data; }
	const FGsFairySummonChangeData& GetSummonChangeData() { return _data; }

};