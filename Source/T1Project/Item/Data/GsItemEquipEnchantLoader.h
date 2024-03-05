#pragma once

#include "DataSchema/GsSchemaEnums.h"

struct FGsItemStatData;

struct FGsLevelBonusList final
{

	int32 _level;
	TArray<FGsItemStatData> _statDatas;

};

struct FGsLevelBonusGroup final
{

	int32 _probability;
	FText _name;
	EGsItemEnchantBonusType _enchantBonusType;
	TArray<FGsLevelBonusList> _levelBonusLists;

};

struct FGsItemEquipEnchantBonus final
{

	int32 _id;
	TArray<FGsLevelBonusGroup> _levelBonusGroups;

};

struct FGsItemEquipEnchantBalance final
{

	int32 _id;
	TArray<FGsLevelBonusList> _levelBonusLists;

};

class FGsItemEquipEnchantLoader final
{

public:
	const TMap<int32, FGsItemEquipEnchantBonus> _enchantBonus;

	const TMap<int32, FGsItemEquipEnchantBalance> _enchantBalance;
	
public:
	FGsItemEquipEnchantLoader();
	
private:
	static const TMap<int32, FGsItemEquipEnchantBonus> MakeEnchantBonus();
	
	static const TMap<int32, FGsItemEquipEnchantBalance> MakeEnchantBalance();

};