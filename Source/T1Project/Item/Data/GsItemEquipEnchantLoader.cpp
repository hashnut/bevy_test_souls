#include "GsItemEquipEnchantLoader.h"

#include "Runtime/Core/Public/Templates/Tuple.h"

#include "../../../../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "DataSchema/Item/GsSchemaItemEquipEnchantBalance.h"
#include "DataSchema/Item/GsSchemaItemEquipEnchantBonus.h"
#include "Item/Data/GsItemStatData.h"
#include "GsItemTableStruct.h"
#include "T1Project.h"

FGsItemEquipEnchantLoader::FGsItemEquipEnchantLoader()
	: _enchantBonus(FGsItemEquipEnchantLoader::MakeEnchantBonus())
	, _enchantBalance(FGsItemEquipEnchantLoader::MakeEnchantBalance())
{
}

const TMap<int32, FGsItemEquipEnchantBonus> FGsItemEquipEnchantLoader::MakeEnchantBonus()
{
	TMap<int32, FGsItemEquipEnchantBonus> Bonuses;

	const UGsTable* Table = FGsSchemaItemEquipEnchantBonus::GetStaticTable();
	if (nullptr != Table)
	{
		TArray<const FGsSchemaItemEquipEnchantBonus*> Rows;
		if (Table->GetAllRows<FGsSchemaItemEquipEnchantBonus>(Rows))
		{
			for (const FGsSchemaItemEquipEnchantBonus* Row : Rows)
			{
				if (!Bonuses.Contains(Row->id))
				{
					Bonuses.Add(Row->id)._id = Row->id;
				}
				FGsItemEquipEnchantBonus& EquipEnchantBonus = Bonuses.FindChecked(Row->id);

				TArray<FGsLevelBonusGroup>& LevelBonusGroups = EquipEnchantBonus._levelBonusGroups;
				FGsLevelBonusGroup* LevelBonusGroup = LevelBonusGroups.FindByPredicate([&Row] (FGsLevelBonusGroup& InLevelBonusGroup)
					{
						return Row->probability == InLevelBonusGroup._probability;
					});

				LevelBonusGroup = &LevelBonusGroups[LevelBonusGroups.AddDefaulted()];
				check(!!LevelBonusGroup);

				LevelBonusGroup->_probability = Row->probability;
				LevelBonusGroup->_name = Row->name;
				LevelBonusGroup->_enchantBonusType = Row->bonusType;

				const TArray<TTuple<int32, StatType, int32>> ROWVALUES_BY_COLUMN_NAME(
					{
						MakeTuple(Row->openLevel1, Row->stat1, Row->statValue1),
						MakeTuple(Row->openLevel2, Row->stat2, Row->statValue2),
						MakeTuple(Row->openLevel3, Row->stat3, Row->statValue3),
					});

				TArray<FGsLevelBonusList>& LevelBonusLists = LevelBonusGroup->_levelBonusLists;

				for (const TTuple<int32, StatType, int32>& ROWVALUES : ROWVALUES_BY_COLUMN_NAME)
				{
					if (StatType::NONE == ROWVALUES.Get<1>())
					{
						continue;
					}

					FGsLevelBonusList* LevelBonusList = LevelBonusLists.FindByPredicate([&ROWVALUES] (FGsLevelBonusList& InLevelBonusList)
						{
							return ROWVALUES.Get<0>() == InLevelBonusList._level;
						});

					if (nullptr == LevelBonusList)
					{
						LevelBonusList = &LevelBonusLists[LevelBonusLists.AddDefaulted()];
						check(!!LevelBonusList);

						LevelBonusList->_level = ROWVALUES.Get<0>();
						LevelBonusList->_statDatas.Emplace(FGsItemStatData{ ROWVALUES.Get<1>(), ROWVALUES.Get<2>() });
					}
				}
			}
		}
		else
		{
			GSLOG(Error, TEXT("!Table->GetAllRows<FGsSchemaItemEquipEnchantBonus>(Rows)"));
		}
	}
	else
	{
		GSLOG(Error, TEXT("nullptr == Table"));
	}

	return Bonuses; // nrvo
}

const TMap<int32, FGsItemEquipEnchantBalance> FGsItemEquipEnchantLoader::MakeEnchantBalance()
{
	TMap<int32, FGsItemEquipEnchantBalance> Balances;

	const UGsTable* Table = FGsSchemaItemEquipEnchantBalance::GetStaticTable();
	if (nullptr != Table)
	{
		TArray<const FGsSchemaItemEquipEnchantBalance*> Rows;
		if (Table->GetAllRows<FGsSchemaItemEquipEnchantBalance>(Rows))
		{
			for (const FGsSchemaItemEquipEnchantBalance* Row : Rows)
			{
				if (!Balances.Contains(Row->id))
				{
					Balances.Add(Row->id)._id = Row->id;
				}
				FGsItemEquipEnchantBalance& EquipEnchantBalance = Balances.FindChecked(Row->id);

				TArray<FGsLevelBonusList>& LevelBonusLists = EquipEnchantBalance._levelBonusLists;

				FGsLevelBonusList* LevelBonusList = LevelBonusLists.FindByPredicate([&Row] (FGsLevelBonusList& InLevelBonusList)
				{
					return Row->level == InLevelBonusList._level;
				});

				if (nullptr == LevelBonusList)
				{
					LevelBonusList = &LevelBonusLists[LevelBonusLists.AddDefaulted()];
					check(!!LevelBonusList);

					LevelBonusList->_level = Row->level;
				}

				const TArray<TTuple<StatType, int32>> ROWVALUES_BY_COLUMN_NAME(
					{
						MakeTuple(Row->stat1, Row->statValue1),
						MakeTuple(Row->stat2, Row->statValue2),
						MakeTuple(Row->stat3, Row->statValue3),
						MakeTuple(Row->stat4, Row->statValue4),
						MakeTuple(Row->stat5, Row->statValue5),
						MakeTuple(Row->stat6, Row->statValue6),
						MakeTuple(Row->stat7, Row->statValue7),
						MakeTuple(Row->stat8, Row->statValue8),
					});

				for (const TTuple<StatType, int32>& ROWVALUES : ROWVALUES_BY_COLUMN_NAME)
				{
					if (StatType::NONE == ROWVALUES.Get<0>())
					{
						continue;
					}

					LevelBonusList->_statDatas.Emplace(FGsItemStatData{ ROWVALUES.Get<0>(), ROWVALUES.Get<1>() });
				}
			}
		}
		else
		{
			GSLOG(Error, TEXT("!Table->GetAllRows<FGsSchemaItemEquipEnchantBalance>(Rows)"));
		}
	}
	else
	{
		GSLOG(Error, TEXT("nullptr == Table"));
	}

	return Balances; // nrvo
}

