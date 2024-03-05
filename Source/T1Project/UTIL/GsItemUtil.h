// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/Define/EGsDropItemCategory.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsItemUtil.generated.h"

class FGsItem;
/**
 *
 */
UCLASS()
class T1PROJECT_API UGsItemUtil : public UObject
{
	GENERATED_BODY()

public:
	static EGsDropItemCategory GetDropObjectCategory(int32 inItemId);		
	static bool IsSendBlockItem(ItemCategorySub inItemCategory);
	static FText GetItemName(int32 inItemName);
	static const FGsItem* FindItemUtil(uint64 InItemDbid);
	static const FGsItem* FindItemByStorageType(uint64 InItemDbid , ItemStorageType InStorageType);
	static const ItemStorageType FindItemStorageType(uint64 InItemDbid);
	static bool TryGetServerDropItem(CreatureId inNpcId, OUT TArray<const struct FGsSchemaItemCommon*>& outEquipItemArray, OUT TArray<const struct FGsSchemaItemCommon*>& outEtcItemArray);
	static bool TryGetServerDropData(CreatureId inNpcId, OUT TArray<const struct FGsSchemaDropData*>& outDropDataArray);
	static bool TryGetInvadeDropItem(CreatureId inNpcId, InvadeDropType inInvadeType, OUT TArray<const struct FGsSchemaItemCommon*>& outEquipItemArray, OUT TArray<const struct FGsSchemaItemCommon*>& outEtcItemArray);
	static bool TryGetInvadeDropData(CreatureId inNpcId, InvadeDropType inInvadeType, OUT TArray<const struct FGsSchemaDropData*>& outDropDataArray);
	static bool TryConvertDropToItemData(TArray<const struct FGsSchemaDropData*>& inDropDataArray, OUT TArray<const struct FGsSchemaItemCommon*>& outEquipItemArray, OUT TArray<const struct FGsSchemaItemCommon*>& outEtcItemArray);
	static int32 GetItemSortOrder(ItemType inType);
};
