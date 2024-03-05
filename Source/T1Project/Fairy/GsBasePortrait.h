#pragma once
#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

struct IGsBasePortrait
{
	enum class PortraitType : uint8 { Fairy, Costume };

	virtual PortraitType GetType() = 0;
	virtual uint64 GetId() = 0;
	virtual bool IsCollected() = 0;
	virtual const FText GetItemName() = 0;
	virtual const FSoftObjectPath GetPortraitPath() { return FSoftObjectPath(); }
	virtual const FSoftObjectPath GetPortraitPath(CreatureGenderType gender) { return FSoftObjectPath(); }
	virtual ItemGrade GetGradeByItemBase() = 0;

	virtual ItemAmount GetAmount() { return false; }
	virtual bool isNewItem() { return false; }
	virtual bool IsNewSummon() { return false;  }
	virtual bool isEnableComposingItem() { return false; }
	virtual const FText GetFlavorText() { return FText::GetEmpty(); }


};

