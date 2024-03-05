// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStatHelper.h"

#include "Item/GsItemStat.h"
#include "Item/Data/GsItemTableStruct.h"
#include "Item/Data/GsItemStatData.h"
#include "Item/GsItemManager.h"

#include "GameObject/Stat/GsStatInfo.h"
#include "GameObject/Define/EGsDisPlayFormatType.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "DataSchema/Stat/GsSchemaStatConvertData.h"
#include "DataSchema/Stat/GsSchemaBonusStatConfig.h"

#include "UTIL/GsTableUtil.h"

#include "GenericPlatformMath.h"

#include "Shared/Client/SharedEnums/SharedStatEnum.h"

#include "Management/ScopeGlobal/GsGameDataManager.h"

#include "T1Project.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "GameObject/Data/GsGameObjectDataLocalPlayer.h"
#include "DataSchema/CombatPower/GsSchemaCombatPowerData.h"


const FGsSchemaStatFormatingData* FGsStatHelper::FindStatFormatingDataByType(const StatType InStatType)
{
	const UGsTableStatFormatingData* table = Cast<UGsTableStatFormatingData>(FGsSchemaStatFormatingData::GetStaticTable());
	if (nullptr == table)
	{
		return nullptr;
	}

	const FGsSchemaStatFormatingData* row = nullptr;
	if (false == table->FindRowById(InStatType, row))
	{
		return nullptr;
	}

	return row;
}

bool FGsStatHelper::GetFormatedValue(const FGsSchemaStatFormatingData* InData, const int32 InValue, OUT FString& outString, bool bUseSign)
{
	if (InData)
	{
		// 만분률을 %표기하기 위해 0.01을 곱한다
		if (EGsDisPlayFormatType::Format_RateInTenThousand == InData->displayFormatType)
		{
			// 소숫점이 존재할 경우 1자리 까지만 표기한다
			bool isFractionalDigits = (0 != (InValue % 100)) ? true : false;
			if (isFractionalDigits)
			{
				outString.Append(FString::Printf(TEXT("%.1f"), InValue * 0.01f));
			}
			else
			{
				outString.Append(FString::FromInt(InValue * 0.01f));
			}
		}
		else if (EGsDisPlayFormatType::Format_RealNumberWithTwoDecimalPointDigit == InData->displayFormatType)
		{						
			outString = GetRealNumberText(InValue / 100.0f, false);
		}
		else
		{
			outString.Append(FString::FromInt(InValue));
		}

		if (false == InData->markFormat.IsEmpty())
		{
			outString.Append(InData->markFormat.ToString());
		}

		if (bUseSign &&
			0 <= InValue)
		{
			outString.InsertAt(0, TEXT("+"));
		}

		return true;
	}

	return false;
}

bool FGsStatHelper::TryGetValidDecimalPointDigit(OUT int32& outDigit, float inValue, int32 inMaxDecimalPointDigit)
{
	outDigit = 0;
	FString temp = FString::SanitizeFloat(inValue);
	TArray<FString> out;
	temp.ParseIntoArray(out, TEXT("."), true);

	if (out.Num() < 2)
	{
		return false;
	}
	
	FString find = out[1];
	int32 length = find.Len();
	int32 initValue = length - 1;
	int32 result = 0; 
	
	if (inMaxDecimalPointDigit < length)
	{
		initValue = inMaxDecimalPointDigit - 1;
	}
	
	for (int i = initValue; i >= 0; i--)
	{
		if ((*find)[i] == (*TEXT("0")))
		{
			result = i;
			break;
		}
	}

	outDigit = result;
	
	return true;
}

FString FGsStatHelper::GetRealNumberText(float inValue, bool inAddPercent)
{
	float value = inValue;

	int32 decimalPointDigit;
	if (false == TryGetValidDecimalPointDigit(decimalPointDigit, value))
	{
		return FString::Printf(TEXT("%.0f"), value) + (inAddPercent ? TEXT("%"):TEXT(""));
	}

	if (decimalPointDigit == 0)
	{
		return FString::Printf(TEXT("%.0f"), value) + (inAddPercent ? TEXT("%") : TEXT(""));
	}
	else if (decimalPointDigit == 1)
	{
		return FString::Printf(TEXT("%.1f"), value) + (inAddPercent ? TEXT("%") : TEXT(""));
	}

	return FString::Printf(TEXT("%.2f"), value) + (inAddPercent ? TEXT("%") : TEXT(""));
}

bool FGsStatHelper::GetDisPlayStatName(const StatType InStatType , OUT FText& outText)
{
	const FGsSchemaStatFormatingData* formatingData = FindStatFormatingDataByType(InStatType);
	if (nullptr == formatingData)
		return false;

	outText = formatingData->statName;
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool FGsStatHelper::GetDisPlayStatValueWithMarkFormat(const StatType InStatType, const int32 InValue , OUT FText& outText, bool bUseSign /*= false*/)
{
	const FGsSchemaStatFormatingData* formatingData = FindStatFormatingDataByType(InStatType);
	if (nullptr == formatingData)
		return false;

	FString formatedValue;
	if (IsConvertRealDisplay(formatingData, InValue))
	{
		GetConvertFormatedValue(formatingData, InValue, formatedValue, bUseSign);
	}
	else
	{
		GetFormatedValue(formatingData, InValue, formatedValue, bUseSign);
	}

	outText = FText::FromString(formatedValue);

	return true;
}

bool FGsStatHelper::GetDesplayStatNameAndValue(const StatType InStatType, const int32 InValue, OUT FString& OutString, bool bUseSign)
{
	const FGsSchemaStatFormatingData* formatingData = FindStatFormatingDataByType(InStatType);
	if (formatingData)
	{
		FString formatedValue;
		if (IsConvertRealDisplay(formatingData, InValue))
		{
			GetConvertFormatedValue(formatingData, InValue, formatedValue, bUseSign);
		}
		else
		{
			GetFormatedValue(formatingData, InValue, formatedValue, bUseSign);
		}

		OutString.Append(formatingData->statName.ToString());
		OutString.Append(TEXT(" "));
		OutString.Append(formatedValue);

		return true;
	}

#if WITH_EDITOR
	GSLOG(Warning, TEXT("[STAT_FORMAT] Fail to fine table. StatType: %d"), static_cast<int32>(InStatType));
#endif

	return false;
}

bool FGsStatHelper::GetDesplayStatNameAndValue(const StatType InStatType, const int32 InValue, OUT TPair<FText, FText>& OutPair, bool bUseSign)
{
	const FGsSchemaStatFormatingData* formatingData = FindStatFormatingDataByType(InStatType);
	if (formatingData)
	{
		FString formatedValue;
		if (IsConvertRealDisplay(formatingData, InValue))
		{
			GetConvertFormatedValue(formatingData, InValue, formatedValue, bUseSign);
		}
		else
		{
			GetFormatedValue(formatingData, InValue, formatedValue, bUseSign);
		}

		OutPair.Key = formatingData->statName;
		OutPair.Value = FText::FromString(formatedValue);

		return true;
	}

#if WITH_EDITOR
	GSLOG(Warning, TEXT("[STAT_FORMAT] Fail to fine table. StatType: %d"), static_cast<int32>(InStatType));
#endif

	return false;
}

bool FGsStatHelper::GetStatWithAdditionalValue(const StatType InStatType, const int32 InValue, 
	const int32 InAdditionalValue, OUT FString& OutString)
{
	const FGsSchemaStatFormatingData* formatingData = FindStatFormatingDataByType(InStatType);
	if (formatingData)
	{
		FString formatedValue;		
		if (IsConvertRealDisplay(formatingData, InValue))
		{
			GetConvertFormatedValue(formatingData, InValue, formatedValue, true);
		}
		else
		{
			GetFormatedValue(formatingData, InValue, formatedValue, true);
		}

		OutString.Append(formatingData->statName.ToString());
		OutString.Append(TEXT(" "));
		OutString.Append(formatedValue);

		if (0 != InAdditionalValue)
		{
			FString formatedValueAdd;
			if (IsConvertRealDisplay(formatingData, InValue))
			{
				GetConvertFormatedValue(formatingData, InAdditionalValue, formatedValueAdd, true);
			}
			else
			{
				GetFormatedValue(formatingData, InAdditionalValue, formatedValueAdd, true);
			}
			OutString.Append(TEXT("("));
			OutString.Append(formatedValueAdd);
			OutString.Append(TEXT(")"));
		}

		return true;
	}

	return false;
}

bool FGsStatHelper::GetStatNameAndValueWithAdditionalValue(const StatType InStatType, const int32 InValue, const int32 InAdditionalValue,
	OUT FText& OutTextName, OUT FString& OutStrValue)
{
	const FGsSchemaStatFormatingData* formatingData = FindStatFormatingDataByType(InStatType);
	if (formatingData)
	{
		FString formatedValue;
		if (IsConvertRealDisplay(formatingData, InValue))
		{
			GetConvertFormatedValue(formatingData, InValue, formatedValue, true);
		}
		else
		{
			GetFormatedValue(formatingData, InValue, formatedValue, true);
		}
		OutStrValue.Append(formatedValue);

		OutTextName = formatingData->statName;

		if (0 != InAdditionalValue)
		{
			FString formatedValueAdd;
			if (IsConvertRealDisplay(formatingData, InValue))
			{
				GetConvertFormatedValue(formatingData, InAdditionalValue, formatedValueAdd, true);
			}
			else
			{
				GetFormatedValue(formatingData, InAdditionalValue, formatedValueAdd, true);
			}
			OutStrValue.Append(TEXT("("));
			OutStrValue.Append(formatedValueAdd);
			OutStrValue.Append(TEXT(")"));
		}

		return true;
	}

	return false;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool FGsStatHelper::IsConvertRealDisplay(const FGsSchemaStatFormatingData* InData, const int32 InValue)
{
	if (nullptr == InData || 0 == InValue || false == GItem()->IsUseRealDisplayStatValue())
		return false;

	if (EGsDisPlayFormatType::Format_Normal != InData->displayFormatType)
		return false;

	// 1차 스텟은 제외처리 하는데,
	// 어떤 스텟이 1차이고 2차인지 알 방법이 현재까진 없어서 
	// 현재 데이터 입력되어있는 기준에서 order 값이 100 미만은 1차 스텟으로 입력되어있기에 임시로 설정함
	// 추후 수정 필요함
	if (100 > InData->uiOrderNum)
		return false;

	return true;
}

bool FGsStatHelper::GetConvertFormatedValue(const FGsSchemaStatFormatingData* InData, const int32 InValue, OUT FString& outString, bool bUseSign)
{
	if (nullptr == InData)
		return false;

	float value = (float)InValue / GItem()->GetRealDisplayDivideValue();
	float calc = FMath::Floor(value * 10.f) / 10.0f;
	if (0 == (calc - (int32)calc))
	{
		outString.Append(FString::Printf(TEXT("%d"), (int32)calc));
	}
	else
	{
		outString.Append(FString::Printf(TEXT("%.1f"), calc));
	}

	if (bUseSign &&
		0 < InValue)
	{
		outString.InsertAt(0, TEXT("+"));
	}

	return true;
}

bool FGsStatHelper::GetStatIconImagePath(const StatType InStatType, OUT FString& outPathText)
{
	const FGsSchemaStatFormatingData* formatingData = FindStatFormatingDataByType(InStatType);
	if (nullptr == formatingData)
		return false;

	outPathText = formatingData->statIconImagePath.ToString();
	return true;
}

void FGsStatHelper::SortUiOrder(OUT TArray<FGsItemStatData>& outStatList)
{
	if (outStatList.Num() > 1)
	{
		outStatList.Sort([](FGsItemStatData itemA, FGsItemStatData itemB)
			{
				const FGsSchemaStatFormatingData* formatingDataA = FindStatFormatingDataByType(itemA.type);
				if (nullptr == formatingDataA)
					return false;

				const FGsSchemaStatFormatingData* formatingDataB = FindStatFormatingDataByType(itemB.type);
				if (nullptr == formatingDataB)
					return false;

				return formatingDataB->uiOrderNum < formatingDataA->uiOrderNum;
			}
		);
	}
}

void FGsStatHelper::SortUiOrder(OUT TArray<FGsItemStat*>& outStatList)
{
	if (outStatList.Num() > 1)
	{
		outStatList.Sort([](FGsItemStat& itemA, FGsItemStat& itemB)
			{
				const FGsSchemaStatFormatingData* formatingDataA = FindStatFormatingDataByType(itemA.GetStatType());
				if (nullptr == formatingDataA)
					return false;

				const FGsSchemaStatFormatingData* formatingDataB = FindStatFormatingDataByType(itemB.GetStatType());
				if (nullptr == formatingDataB)
					return false;

				return formatingDataB->uiOrderNum < formatingDataA->uiOrderNum;
			}
		);
	}
}

void FGsStatHelper::SortUiOrder(OUT TArray<const FGsStatInfo*>& outStatList)
{
	if (outStatList.Num() > 1)
	{
		outStatList.Sort([](const FGsStatInfo& itemA, const FGsStatInfo& itemB)
			{
				const FGsSchemaStatFormatingData* formatingDataA = FindStatFormatingDataByType(itemA.GetStatType());
				if (nullptr == formatingDataA)
					return false;

				const FGsSchemaStatFormatingData* formatingDataB = FindStatFormatingDataByType(itemB.GetStatType());
				if (nullptr == formatingDataB)
					return false;

				return formatingDataB->uiOrderNum < formatingDataA->uiOrderNum;
			}
		);
	}
}

void FGsStatHelper::SortUiOrder(OUT TArray<StatInfo>& outStatList)
{
	if (outStatList.Num() > 1)
	{
		outStatList.Sort([](const StatInfo& itemA, const StatInfo& itemB)
			{
				const FGsSchemaStatFormatingData* formatingDataA = FindStatFormatingDataByType(itemA.mType);
				if (nullptr == formatingDataA)
					return false;

				const FGsSchemaStatFormatingData* formatingDataB = FindStatFormatingDataByType(itemB.mType);
				if (nullptr == formatingDataB)
					return false;

				return formatingDataB->uiOrderNum < formatingDataA->uiOrderNum;
			}
		);
	}
}

// 만분율 값을 실제 비율로 변환
float FGsStatHelper::ConvertValueToPermyriad(const FGsStatInfo* inStatValue)
{
	return (nullptr == inStatValue) ? 0.0f : ((float)inStatValue->GetStatValue() * 0.0001f);
}

float FGsStatHelper::ApplyHardcapSpeedRateStatType(const StatType inStatType, float currentValue)
{
	float result = 0.0f;

	if (const UGsGlobalConstant* gConstant = GData()->GetGlobalData())
	{
		switch (inStatType)
		{
		case StatType::ATTACK_SPEED_RATE:
			result = FMath::Min(FMath::Max(currentValue, gConstant->_hardcapAttackSpeedRateMin), gConstant->_hardcapAttackSpeedRateMax);
			break;
// 		case StatType::MOVE_SPEED_RATE:
// 			result = FMath::Min(FMath::Max(currentValue, gConstant->_hardcapMoveSpeedRateMin), gConstant->_hardcapMoveSpeedRateMax);
// 			break;
		case StatType::CASTING_SPEED_RATE:
			result = FMath::Min(FMath::Max(currentValue, gConstant->_hardcapCastingSpeedRateMin), gConstant->_hardcapCastingSpeedRateMax);
			break;
		default:
			result = currentValue;
		}
	}
	
	return result;
}

bool FGsStatHelper::TryGetConvertData(OUT FGsSchemaStatConvertTable& outData, StatType inStatType, int32 inValue)
{
	const FGsSchemaStatConvertData* strConvertData = UGsTableUtil::FindRowById<UGsTableStatConvertData, FGsSchemaStatConvertData>(inStatType);
	if (nullptr == strConvertData)
	{
		return false;
	}

	const FGsSchemaStatConvertTable* find = strConvertData->convertList.FindByPredicate([=](const FGsSchemaStatConvertTable& data)->bool {
		return data.baseValue == inValue;
	});

	if (find)
	{
		outData = (*find);

		return true;
	}

	return false;
}

int32 FGsStatHelper::GetMaxBonusStatItemCount()
{
	if (const UGsTableBonusStatConfig* statConfigTable = Cast<UGsTableBonusStatConfig>(FGsSchemaBonusStatConfig::GetStaticTable()))
	{
		TArray<const FGsSchemaBonusStatConfig*> rows;
		if (statConfigTable->GetAllRows(rows) && 0 < rows.Num())
		{
			return GGameData()->GetUserData()->GetBonusStatItemExtendCount() + rows[0]->bonusStatItemConsumeMax;
		}
	}

	return 0;
}

const FGsSchemaStatFormatingData* FGsStatHelper::FindStat(StatType inStat)
{
	return UGsTableUtil::FindRowById<UGsTableStatFormatingData, FGsSchemaStatFormatingData>(inStat);
}

const struct FGsSchemaCombatPowerData* FGsStatHelper::FindCombatPowerStat(StatType inStat)
{
	return UGsTableUtil::FindRowById<UGsTableCombatPowerData, FGsSchemaCombatPowerData>(inStat);
}

CreatureWeaponType FGsStatHelper::GetPlayerWeapon()
{
	UGsGameObjectBase* findPlayer = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (nullptr == findPlayer)
	{
		return CreatureWeaponType::ALL;
	}

	UGsGameObjectLocalPlayer* localPlayerGameObject = Cast<UGsGameObjectLocalPlayer>(findPlayer);
	if (nullptr == localPlayerGameObject)
	{
		return CreatureWeaponType::ALL;
	}

	FGsGameObjectDataLocalPlayer* data = localPlayerGameObject->GetCastData<FGsGameObjectDataLocalPlayer>();
	if (nullptr == data)
	{
		return CreatureWeaponType::ALL;
	}

	return data->GetCurrentWeaponType();
}