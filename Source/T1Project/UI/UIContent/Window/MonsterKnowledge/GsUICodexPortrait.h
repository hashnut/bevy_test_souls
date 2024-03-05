// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG/Public/Blueprint/IUserObjectListEntry.h"
#include "UI/UIContent/Window/NPCPortrait/GsUIMonsterPortrait.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"

#include "GsUICodexPortrait.generated.h"

struct FGsSchemaNpcData;

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUICodexPortraitEntryData : public UObject
{
	GENERATED_BODY()

private:
	MapId _mapId;
	const FGsSchemaNpcData* _npcData;

public:
	void SetData(MapId InMapId, const FGsSchemaNpcData* InData)
	{
		if (nullptr == InData)
			return;

		_mapId = InMapId;
		_npcData = InData;
	}
	const FGsSchemaNpcData* GetData() const
	{
		return _npcData;
	}
	MapId GetMapId() const { return _mapId; }
};



/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUICodexPortrait : public UGsUIMonsterPortrait, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	virtual void NativeOnListItemObjectSet(UObject* InListItemObject) override;
};
