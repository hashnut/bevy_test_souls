#include "GsRecoveryPenaltyFunc.h"

#include "Management/ScopeGlobal/GsGameDataManager.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

// 페널피 복구 갯수 구하기
int FGsRecoveryPenaltyFunc::GetRecoveryPenaltyCount()
{
	TArray<DeathPenaltyExp> expPenaltyList = GGameData()->GetUserData()->expPenalty;
	TArray<DeathPenaltyItem> itemPenaltyList = GGameData()->GetUserData()->itemPenalty;
	int32 totalCount = expPenaltyList.Num() + itemPenaltyList.Num();

	return totalCount;
}
