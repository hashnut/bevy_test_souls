#pragma once

#include "CoreMinimal.h"
#include "../Message/GsMessageGameObject.h"
#include "../Management/GsMessageHolder.h"
#include "../Message/MessageParam/GsMessageParam.h"
#include "../Classes/Collection/GsCollectionDataContainer.h"
#include "../Classes/Collection/GsCollectionData.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedPassivityEnum.h"



class FGsPassivityCollectionData : public IGsCollectionDataContainer, public IGsCollectionData
{
private:
	PassivityOwnerType _ownerType = PassivityOwnerType::MAX;
	int32 _messageObjectType = -1;

	TArray<PassivityId> _passivitySet;
public:
	virtual ~FGsPassivityCollectionData() {/**/ }

	void Set(PassivityOwnerType InOwnerType, MessageGameObject InMessageGameObject)
	{
		_ownerType = InOwnerType;
		_messageObjectType = int32(InMessageGameObject);
		_passivitySet.Empty();
	}

	void Clear(bool InIsMessage = true)
	{
		_passivitySet.Empty();

		if (InIsMessage)
			SendUpdateMessage();
	}

	void SendUpdateMessage()
	{
		if (PassivityOwnerType::MAX != _ownerType && -1 != _messageObjectType)
		{
			const FGsPrimitiveInt32 Param(StaticCast<int32>(_ownerType));
			GMessage()->GetGameObject().SendMessage(StaticCast<MessageGameObject>(_messageObjectType), &Param);
		}
	}

	void Add(int32 InId, bool InIsMessage = true)
	{
		_passivitySet.Emplace(InId);

		if (InIsMessage)
		{
			SendUpdateMessage();
		}
	}

	void Add(const TArray<PassivityId> InIds, bool InIsMessage = true)
	{
		if (0 >= InIds.Num())
		{
			return;
		}

		for (const auto& item : InIds)
		{
			_passivitySet.Emplace(item);
		}

		if (InIsMessage)
		{
			SendUpdateMessage();
		}
	}

	void Add(const TArray<int32> InIds, bool InIsMessage = true)
	{
		if (0 >= InIds.Num())
		{
			return;
		}

		for (const auto& item : InIds)
		{
			_passivitySet.Emplace(item);
		}

		if (InIsMessage)
		{
			SendUpdateMessage();
		}
	}

	const IGsCollectionData* GetCollectionData() const override
	{
		return this;
	}


	PassivityOwnerType GetPassivityOwnerType() const override
	{
		return StaticCast<PassivityOwnerType>(_ownerType);
	}

	int32 GetPassivityCount() const override
	{
		return _passivitySet.Num();
	}

	PassivityId GetPassivityId(int32 InIndex) const override
	{
		return _passivitySet.IsValidIndex(InIndex) ? _passivitySet[InIndex] : INVALID_PASSIVITY_ID;
	}
};