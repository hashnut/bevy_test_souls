// Engine\Plugins\VirtualProduction\DMX\DMXProtocol\Source\DMXProtocol\Public\DMXProtocolTypes.cpp 에서 복사해 옴

#include "GsMailCategorySub.h"

#include "Runtime/Core/Public/Misc/OutputDevice.h"
#include "Runtime/Core/Public/UObject/PropertyPortFlags.h"

#include "../Plugins/RSQL/Source/Runtime/Public/GsNameSelectorBase.h"

#include "Shared/Client/SharedEnums/SharedMailEnum.h"
#include "Shared/Shared/SharedInclude/SharedFuncs.h"

IMPLEMENT_GS_NAME_SELECTOR_STATICVARS(FGsMailCategorySub)

IMPLEMENT_GS_NAME_SELECTOR_GetAllValues(FGsMailCategorySub)
{
	static TArray<FName> Names;
	if (Names.Num() == 0)
	{
		for (int32 i = 0; i < (int32)MailCategorySub::MAX; ++i)
		{
			const FString EnumName = LexToString((MailCategorySub)i);
			if (!EnumName.IsEmpty())
			{
				Names.Add(FName(EnumName));
			}
		}
	}
	return Names;
}

IMPLEMENT_GS_NAME_SELECTOR_GetPossibleValues(FGsMailCategorySub)
{
	static TArray<FName> Names;
	if (Names.Num() == 0)
	{
		for (int32 i = 0; i < (int32)MailCategorySub::MAX; ++i)
		{
			const MailCategorySub CurrentCategorySubType = (MailCategorySub)i;
			/*if (SharedFunc::StatTypeHidden(CurrentStatType))
			{
				continue;
			}*/

			const FString EnumName = LexToString(CurrentCategorySubType);
			if (!EnumName.IsEmpty())
			{
				Names.Add(FName(EnumName));
			}
		}
	}
	return Names;
}

IMPLEMENT_GS_NAME_SELECTOR_IsValid(FGsMailCategorySub)
{
	return !InName.IsNone();
}

FGsMailCategorySub::FGsMailCategorySub()
{
	Name = *LexToString(MailCategorySub::Event_Daily);
}

FGsMailCategorySub::FGsMailCategorySub(const MailCategorySub InCategorySubType)
{
	Name = *LexToString(InCategorySubType);
}

FGsMailCategorySub::FGsMailCategorySub(const FName& InName)
{
	Name = InName;
}

MailCategorySub FGsMailCategorySub::GetMailCategorySub() const
{
	if (Name.IsNone())
	{
		return MailCategorySub::Event_Daily;
	}

	MailCategorySub CurrentCategorySubType = MailCategorySub::Event_Daily;
	LexFromString(CurrentCategorySubType, *Name.ToString());
	return CurrentCategorySubType;
}

FGsMailCategorySub::operator MailCategorySub()
{
	return GetMailCategorySub();
}

FGsMailCategorySub::operator const MailCategorySub() const
{
	return GetMailCategorySub();
}

bool FGsMailCategorySub::operator==(const MailCategorySub& Other) const
{
	return GetMailCategorySub() == Other;
}

bool FGsMailCategorySub::operator!=(const MailCategorySub& Other) const
{
	return GetMailCategorySub() != Other;
}

bool FGsMailCategorySub::operator==(const FGsMailCategorySub& Other) const
{
	return Name == Other.Name;
}

bool FGsMailCategorySub::operator!=(const FGsMailCategorySub& Other) const
{
	return !Name.IsEqual(Other.Name);
}

bool FGsMailCategorySub::ExportTextItem(FString& ValueStr, FGsMailCategorySub const& DefaultValue, UObject* Parent, int32 PortFlags, UObject* ExportRootScope) const
{
	if (0 != (PortFlags & PPF_ExportCpp))
	{
		return false;
	}

	ValueStr += FString::Printf(TEXT("%s"), *Name.ToString().ReplaceCharWithEscapedChar());
	return true;
}

bool FGsMailCategorySub::ImportTextItem(const TCHAR*& Buffer, int32 PortFlags, UObject* Parent, FOutputDevice* ErrorText)
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

bool FGsMailCategorySub::IsValid() const
{
	return FGsMailCategorySub::IsValid(Name);
}

const FString LexToString(const MailCategorySub InCategorySubType)
{
	return MailCategorySubEnumToString(InCategorySubType);
}

void LexFromString(OUT MailCategorySub& OutCategorySubType, const TCHAR* InBuffer)
{
	OutCategorySubType = MailCategorySubStringToEnum(InBuffer);
}
