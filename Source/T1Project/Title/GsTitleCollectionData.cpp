
#include "GsTitleCollectionData.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Management/GsMessageHolder.h"
#include "Message/GsMessageGameObject.h"
#include "T1Project/T1Project.h"


FGsTitleCollectionData::FGsTitleCollectionData(PassivityOwnerType InOwnerType)
{
	_ownerType = InOwnerType;
}

void FGsTitleCollectionData::CleanUp()
{
	_passivityIds.Empty();
}

void FGsTitleCollectionData::Add(PassivityId InId, bool InForceRefresh /* = false */)
{
	if (INVALID_PASSIVITY_ID == InId)
	{
		return;
	}

	_passivityIds.Emplace(InId);

	if (InForceRefresh)
	{
		HudCollectionUpdate();
	}
}

void FGsTitleCollectionData::Remove(PassivityId InId, bool InForceRefresh /* = false */)
{
	if (INVALID_PASSIVITY_ID == InId)
	{
		return;
	}

	_passivityIds.Remove(InId);

	if (InForceRefresh)
	{
		HudCollectionUpdate();
	}
}

void FGsTitleCollectionData::HudCollectionUpdate()
{
	const FGsPrimitiveInt32 Param(StaticCast<int32>(_ownerType));
	GMessage()->GetGameObject().SendMessage(MessageGameObject::PASSIVITY_UPDATE_COLLECTION, &Param);
}


const IGsCollectionData* FGsTitleCollectionData::GetCollectionData() const
{
	return this;
}

PassivityOwnerType FGsTitleCollectionData::GetPassivityOwnerType() const
{
	return _ownerType;
}

int32 FGsTitleCollectionData::GetPassivityCount() const
{
	return _passivityIds.Num();
}

PassivityId FGsTitleCollectionData::GetPassivityId(int32 InIndex) const
{
	if (false == _passivityIds.IsValidIndex(InIndex))
	{
		GSLOG(Error, TEXT("invalid InIndex %d"), InIndex);
		return INVALID_PASSIVITY_ID;
	}

	return _passivityIds[InIndex];
}