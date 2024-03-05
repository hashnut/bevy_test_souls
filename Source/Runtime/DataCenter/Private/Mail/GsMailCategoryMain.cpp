// Engine\Plugins\VirtualProduction\DMX\DMXProtocol\Source\DMXProtocol\Public\DMXProtocolTypes.cpp 에서 복사해 옴

#include "GsMailCategoryMain.h"

#include "Runtime/Core/Public/Misc/OutputDevice.h"
#include "Runtime/Core/Public/UObject/PropertyPortFlags.h"

#include "../Plugins/RSQL/Source/Runtime/Public/GsNameSelectorBase.h"

#include "Shared/Client/SharedEnums/SharedMailEnum.h"
#include "Shared/Shared/SharedInclude/SharedFuncs.h"

IMPLEMENT_GS_NAME_SELECTOR_STATICVARS(FGsMailCategoryMain)

IMPLEMENT_GS_NAME_SELECTOR_GetAllValues(FGsMailCategoryMain)
{
	static TArray<FName> Names;
	if (Names.Num() == 0)
	{
		for (int32 i = 0; i < (int32)MailCategoryMain::MAX; ++i)
		{
			const FString EnumName = LexToString((MailCategoryMain)i);
			if (!EnumName.IsEmpty())
			{
				Names.Add(FName(EnumName));
			}
		}
	}
	return Names;
}

IMPLEMENT_GS_NAME_SELECTOR_GetPossibleValues(FGsMailCategoryMain)
{
	static TArray<FName> Names;
	if (Names.Num() == 0)
	{
		for (int32 i = 0; i < (int32)MailCategoryMain::MAX; ++i)
		{
			const MailCategoryMain CurrentCategoryMainType = (MailCategoryMain)i;
			/*if (SharedFunc::StatTypeHidden(CurrentStatType))
			{
				continue;
			}*/

			const FString EnumName = LexToString(CurrentCategoryMainType);
			if (!EnumName.IsEmpty())
			{
				Names.Add(FName(EnumName));
			}
		}
	}
	return Names;
}

IMPLEMENT_GS_NAME_SELECTOR_IsValid(FGsMailCategoryMain)
{
	return !InName.IsNone();
}

FGsMailCategoryMain::FGsMailCategoryMain()
{
	Name = *LexToString(MailCategoryMain::Event);
}

FGsMailCategoryMain::FGsMailCategoryMain(const MailCategoryMain InCategoryMainType)
{
	Name = *LexToString(InCategoryMainType);
}

FGsMailCategoryMain::FGsMailCategoryMain(const FName& InName)
{
	Name = InName;
}

MailCategoryMain FGsMailCategoryMain::GetMailCategoryMain() const
{
	if (Name.IsNone())
	{
		return MailCategoryMain::Event;
	}

	MailCategoryMain CurrentCategoryMainType = MailCategoryMain::Event;
	LexFromString(CurrentCategoryMainType, *Name.ToString());
	return CurrentCategoryMainType;
}

FGsMailCategoryMain::operator MailCategoryMain()
{
	return GetMailCategoryMain();
}

FGsMailCategoryMain::operator const MailCategoryMain() const
{
	return GetMailCategoryMain();
}

bool FGsMailCategoryMain::operator==(const MailCategoryMain& Other) const
{
	return GetMailCategoryMain() == Other;
}

bool FGsMailCategoryMain::operator!=(const MailCategoryMain& Other) const
{
	return GetMailCategoryMain() != Other;
}

bool FGsMailCategoryMain::operator==(const FGsMailCategoryMain& Other) const
{
	return Name == Other.Name;
}

bool FGsMailCategoryMain::operator!=(const FGsMailCategoryMain& Other) const
{
	return !Name.IsEqual(Other.Name);
}

bool FGsMailCategoryMain::ExportTextItem(FString& ValueStr, FGsMailCategoryMain const& DefaultValue, UObject* Parent, int32 PortFlags, UObject* ExportRootScope) const
{
	if (0 != (PortFlags & PPF_ExportCpp))
	{
		return false;
	}

	ValueStr += FString::Printf(TEXT("%s"), *Name.ToString().ReplaceCharWithEscapedChar());
	return true;
}

bool FGsMailCategoryMain::ImportTextItem(const TCHAR*& Buffer, int32 PortFlags, UObject* Parent, FOutputDevice* ErrorText)
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

bool FGsMailCategoryMain::IsValid() const
{
	return FGsMailCategoryMain::IsValid(Name);
}

const FString LexToString(const MailCategoryMain InCategoryMainType)
{
	return MailCategoryMainEnumToString(InCategoryMainType);
}

void LexFromString(OUT MailCategoryMain& OutCategoryMainType, const TCHAR* InBuffer)
{
	OutCategoryMainType = MailCategoryMainStringToEnum(InBuffer);
}
