
#include "GsTitlePlayerData.h"
#include "../T1Project.h"


const FGsTitlePlayerData::TitleSet FGsTitlePlayerData::InitData = TitleSet(FText::GetEmpty(), FLinearColor::White);


void FGsTitlePlayerData::CleanUp()
{
	_frontSet = InitData;
	_backSet = InitData;
}

void FGsTitlePlayerData::Set(const TitleSet& InFront, const TitleSet& InBack)
{
	_frontSet = InFront;
	_backSet = InBack;
}

void FGsTitlePlayerData::SetFront(const TitleSet& InFront)
{
	_frontSet = InFront;
}

void FGsTitlePlayerData::SetBack(const TitleSet& InBack)
{
	_backSet = InBack;
}

const FGsTitlePlayerData::TitleSet& FGsTitlePlayerData::Get(TitleEquipSlot InEquipSlot) const
{
	if (TitleEquipSlot::MAX == InEquipSlot)
	{
		GSLOG(Error, TEXT("TitleEquipSlot::MAX == InEquipSlot"));
		return InitData;
	}

	return (TitleEquipSlot::FRONT == InEquipSlot) ? _frontSet : _backSet;
}
