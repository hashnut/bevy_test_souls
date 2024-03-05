
#pragma once

#include "CoreMinimal.h"
#include "GsTitleCollectionData.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedTitleEnum.h"


class T1PROJECT_API FGsTitleEquip final : public FGsTitleCollectionData
{
	/************************************************************************/
	/* Member                                                               */
	/************************************************************************/
private:
	class FGsTitleData* _frontData = nullptr;
	class FGsTitleData* _backData = nullptr;

public:
	FGsTitleEquip();

	// 2022/01/13 PKT - ÃÊ±âÈ­
	void Clear();

	// 2022/01/13 PKT - ÄªÈ£ ÀåÂø / ÇØÁ¦
	void Equip(class FGsTitleData* InData, bool InIsShowTicker = true);
	void UnEquip(class FGsTitleData* InData);
	void UnEquip(TitleId InId);

	// 2022/01/13 PKT - ÄªÈ£ À§Ä¡º° Data
	const class FGsTitleData* GetData(TitleEquipSlot InSlot) const;
};