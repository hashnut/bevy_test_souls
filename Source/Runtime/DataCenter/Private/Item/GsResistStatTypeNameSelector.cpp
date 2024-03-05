// Engine\Plugins\VirtualProduction\DMX\DMXProtocol\Source\DMXProtocol\Public\DMXProtocolTypes.cpp 에서 복사해 옴

#include "GsResistStatTypeNameSelector.h"

#include "Runtime/Core/Public/Misc/OutputDevice.h"
#include "Runtime/Core/Public/UObject/PropertyPortFlags.h"

#include "../Plugins/RSQL/Source/Runtime/Public/GsNameSelectorBase.h"

#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "Shared/Shared/SharedInclude/SharedFuncs.h"

IMPLEMENT_GS_NAME_SELECTOR_STATICVARS(FGsResistStatTypeNameSelector)

IMPLEMENT_GS_NAME_SELECTOR_GetAllValues(FGsResistStatTypeNameSelector)
{
	static TArray<FName> Names;
	if (Names.Num() == 0)
	{
		Names.Add(NAME_None);
		for (int32 i = 0; i < (int32)StatType::MAX; ++i)
		{
			auto statEnumName = StatTypeEnumToString((StatType)i);
			if (statEnumName.Contains(TEXT("RESIST"), ESearchCase::IgnoreCase))
			{
				Names.Add(FName(statEnumName));
			}
		}
	}
	return Names;
}

IMPLEMENT_GS_NAME_SELECTOR_GetPossibleValues(FGsResistStatTypeNameSelector)
{
	static TArray<FName> Names;
	if (Names.Num() == 0)
	{
		Names.Add(NAME_None);
		for (int32 i = 0; i < (int32)StatType::MAX; ++i)
		{
			const StatType CurrentStatType = (StatType)i;
			if (SharedFunc::StatTypeHidden(CurrentStatType))
			{
				continue;
			}

			const FString statEnumName = StatTypeEnumToString(CurrentStatType);
			if (!statEnumName.IsEmpty() && statEnumName.Contains(TEXT("RESIST"), ESearchCase::IgnoreCase))
			{
				Names.Add(FName(statEnumName));
			}
		}
	}
	return Names;
}

IMPLEMENT_GS_NAME_SELECTOR_IsValid(FGsResistStatTypeNameSelector)
{
	return !InName.IsNone();
}

FGsResistStatTypeNameSelector::FGsResistStatTypeNameSelector()
{
	Name = *StatTypeEnumToString(StatType::NONE);
}

FGsResistStatTypeNameSelector::FGsResistStatTypeNameSelector(const StatType InStatType)
{
	Name = *StatTypeEnumToString(InStatType);
}

FGsResistStatTypeNameSelector::FGsResistStatTypeNameSelector(const FName& InName)
{
	Name = InName;
}

StatType FGsResistStatTypeNameSelector::GetResistStatType() const
{
	if (Name.IsNone())
	{
		return StatType::NONE;
	}

	StatType CurrentStatType = StatType::NONE;
	CurrentStatType = StatTypeStringToEnum(*Name.ToString());
	return CurrentStatType;
}

FGsResistStatTypeNameSelector::operator StatType()
{
	return GetResistStatType();
}

FGsResistStatTypeNameSelector::operator const StatType() const
{
	return GetResistStatType();
}

bool FGsResistStatTypeNameSelector::operator==(const StatType& Other) const
{
	return GetResistStatType() == Other;
}

bool FGsResistStatTypeNameSelector::operator!=(const StatType& Other) const
{
	return GetResistStatType() != Other;
}

bool FGsResistStatTypeNameSelector::operator==(const FGsResistStatTypeNameSelector& Other) const
{
	return Name == Other.Name;
}

bool FGsResistStatTypeNameSelector::operator!=(const FGsResistStatTypeNameSelector& Other) const
{
	return !Name.IsEqual(Other.Name);
}

bool FGsResistStatTypeNameSelector::ExportTextItem(FString& ValueStr, FGsResistStatTypeNameSelector const& DefaultValue, UObject* Parent, int32 PortFlags, UObject* ExportRootScope) const
{
	if (0 != (PortFlags & PPF_ExportCpp))
	{
		return false;
	}

	ValueStr += FString::Printf(TEXT("%s"), *Name.ToString().ReplaceCharWithEscapedChar());
	return true;
}

bool FGsResistStatTypeNameSelector::ImportTextItem(const TCHAR*& Buffer, int32 PortFlags, UObject* Parent, FOutputDevice* ErrorText)
{
	if (nullptr != Buffer)
	{
		if (0 == (PortFlags & PPF_Delimited))
		{
			SetFromName(FName(FString(Buffer).Replace(TEXT(" "), TEXT("_"))));

			// in order to indicate that the value was successfully imported, advance the buffer past the last character that was imported
			Buffer += FCString::Strlen(Buffer);

			return true;
		}
		else
		{
			TStringBuilder<256> Token;
			Buffer = FPropertyHelpers::ReadToken(Buffer, /* out */ Token, true);
			if (!Buffer)
			{
				return false;
			}

			SetFromName(FName(FString(Token).Replace(TEXT(" "), TEXT("_"))));
			return true;
		}
	}

	return false;
}

bool FGsResistStatTypeNameSelector::IsValid() const
{
	return FGsResistStatTypeNameSelector::IsValid(Name);
}
