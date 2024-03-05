#pragma once
#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsCollectionData.h"

/* collection filter data*/
class T1PROJECT_API IGsCollectionGetterData
{
protected:
	CollectionId _id{ INVALID_COLLECTION_ID };
	bool _isComplete{ false };

	TArray<PassivityId> _rewards;
};

/* collection filter action */
class T1PROJECT_API IGsCollectionGetter
{
public:
	virtual const bool IsCompleted() = 0;
	virtual const TArray<PassivityId>& GetRewardPassivity() const = 0;
};

/*필터 되는 콜렉션 데이터*/
class T1PROJECT_API FGsCollectionFilterData : public IGsCollectionGetterData, public IGsCollectionGetter
{
public:
	virtual ~FGsCollectionFilterData() {};
	FGsCollectionFilterData() = default;

	virtual const bool IsCompleted() override
	{
		return _isComplete;
	}

	virtual const TArray<PassivityId>& GetRewardPassivity() const override
	{
		return _rewards;
	}
};

