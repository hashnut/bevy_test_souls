#pragma once

#include "CoreMinimal.h"
#include "Classes/Collection/GsCollectionData.h"
#include "Classes/Collection/GsCollectionDataContainer.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"


class T1PROJECT_API FGsTitleCollectionData : public IGsCollectionDataContainer, public IGsCollectionData
{
	/************************************************************************/
	/* Member                                                               */
	/************************************************************************/
private:
	// 2022/01/13 PKT - ¿Â¬¯ ¡ﬂ¿Œ PassivityIds
	TArray<PassivityId> _passivityIds;

	PassivityOwnerType _ownerType = PassivityOwnerType::MAX;

public:
	FGsTitleCollectionData(PassivityOwnerType InOwnerType);
	virtual ~FGsTitleCollectionData() {/**/ }

protected:
	
	void CleanUp();

	void Add(PassivityId InId, bool InForceRefresh = false);	
	void Remove(PassivityId InId, bool InForceRefresh = false);

	void HudCollectionUpdate();

	/************************************************************************/
	/* IGsCollectionDataContainer implements                                */
	/************************************************************************/
public:
	const IGsCollectionData* GetCollectionData() const final;

	/************************************************************************/
	/* IGsCollectionData implements                                         */
	/************************************************************************/
public:
	PassivityOwnerType GetPassivityOwnerType() const final;
	  
	int32 GetPassivityCount() const final;

	PassivityId GetPassivityId(int32 InIndex) const final;
};