#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Message/MessageParam/GsMessageParam.h"

/**
 * MultiLevelRank Message 사용될 구조체 정의
 */
struct FGsMultiLevelRankSingleDataMessageParam : public IGsMessageParam
{
protected:
	MultiLevelRank _rank;

public:
	FGsMultiLevelRankSingleDataMessageParam() = default;
	explicit FGsMultiLevelRankSingleDataMessageParam(MultiLevelRank InRank) : _rank(InRank) {/*None*/ }
	virtual ~FGsMultiLevelRankSingleDataMessageParam() {/*None*/ }

	MultiLevelRank GetData() const { return _rank; }
	FORCEINLINE void SetData(MultiLevelRank rank) { _rank = rank; }
};