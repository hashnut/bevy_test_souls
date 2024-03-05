// Engine\Plugins\VirtualProduction\DMX\DMXProtocol\Source\DMXProtocol\Public\DMXProtocolTypes.cpp 에서 복사해 옴

#include "GsStatTypeNameSelector.h"

#include "Runtime/Core/Public/Misc/OutputDevice.h"
#include "Runtime/Core/Public/UObject/PropertyPortFlags.h"

#include "../Plugins/RSQL/Source/Runtime/Public/GsNameSelectorBase.h"

#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "Shared/Shared/SharedInclude/SharedFuncs.h"

IMPLEMENT_GS_NAME_SELECTOR_STATICVARS(FGsStatTypeNameSelector)

IMPLEMENT_GS_NAME_SELECTOR_GetAllValues(FGsStatTypeNameSelector)
{
	static TArray<FName> Names;
	if (Names.Num() == 0)
	{
		for (int32 i = 0; i < (int32)StatType::MAX; ++i)
		{
			const FString EnumName = LexToString((StatType)i);
			if (!EnumName.IsEmpty())
			{
				Names.Add(FName(EnumName));
			}
		}
	}
	return Names;
}

IMPLEMENT_GS_NAME_SELECTOR_GetPossibleValues(FGsStatTypeNameSelector)
{
	static TArray<FName> Names;
	if (Names.Num() == 0)
	{
		for (int32 i = 0; i < (int32)StatType::MAX; ++i)
		{
			const StatType CurrentStatType = (StatType)i;
			if (SharedFunc::StatTypeHidden(CurrentStatType))
			{
				continue;
			}

			const FString EnumName = LexToString(CurrentStatType);
			if (!EnumName.IsEmpty())
			{
				Names.Add(FName(EnumName));
			}
		}
	}
	return Names;
}

IMPLEMENT_GS_NAME_SELECTOR_IsValid(FGsStatTypeNameSelector)
{
	return !InName.IsNone();
}

FGsStatTypeNameSelector::FGsStatTypeNameSelector()
{
	Name = *LexToString(StatType::NONE);
}

FGsStatTypeNameSelector::FGsStatTypeNameSelector(const StatType InStatType)
{
	Name = *LexToString(InStatType);
}

FGsStatTypeNameSelector::FGsStatTypeNameSelector(const FName& InName)
{
	Name = InName;
}

StatType FGsStatTypeNameSelector::GetStatType() const
{
	if (Name.IsNone())
	{
		return StatType::NONE;
	}

	StatType CurrentStatType = StatType::NONE;
	LexFromString(CurrentStatType, *Name.ToString());
	return CurrentStatType;
}

FGsStatTypeNameSelector::operator StatType()
{
	return GetStatType();
}

FGsStatTypeNameSelector::operator const StatType() const
{
	return GetStatType();
}

bool FGsStatTypeNameSelector::operator==(const StatType& Other) const
{
	return GetStatType() == Other;
}

bool FGsStatTypeNameSelector::operator!=(const StatType& Other) const
{
	return GetStatType() != Other;
}

bool FGsStatTypeNameSelector::operator==(const FGsStatTypeNameSelector& Other) const
{
	return Name == Other.Name;
}

bool FGsStatTypeNameSelector::operator!=(const FGsStatTypeNameSelector& Other) const
{
	return !Name.IsEqual(Other.Name);
}

bool FGsStatTypeNameSelector::ExportTextItem(FString& ValueStr, FGsStatTypeNameSelector const& DefaultValue, UObject* Parent, int32 PortFlags, UObject* ExportRootScope) const
{
	if (0 != (PortFlags & PPF_ExportCpp))
	{
		return false;
	}

	ValueStr += FString::Printf(TEXT("%s"), *Name.ToString().ReplaceCharWithEscapedChar());
	return true;
}

bool FGsStatTypeNameSelector::ImportTextItem(const TCHAR*& Buffer, int32 PortFlags, UObject* Parent, FOutputDevice* ErrorText)
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

bool FGsStatTypeNameSelector::IsValid() const
{
	return FGsStatTypeNameSelector::IsValid(Name);
}

const FString LexToString(const StatType InStatType)
{
	return StatTypeEnumToString(InStatType);
}

void LexFromString(OUT StatType& OutStatType, const TCHAR* InBuffer)
{
	OutStatType = StatTypeStringToEnum(InBuffer);
}
