#include "GsGameOptions.h"

TWeakObjectPtr<UGsGameOptions> UGsGameOptions::GameOptions = nullptr;

UGsGameOptions::UGsGameOptions(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// this will only call the base version of SetToDefaults but some constructors may rely on it being set
}

void UGsGameOptions::SaveOptionConfig()
{
	SaveConfig(CPF_Config);
}

void UGsGameOptions::CreateGameOptions()
{
	auto Options = NewObject<UGsGameOptions>(GetTransientPackage());
	bool allAdd = Options->SystmeMessageOpt.IsValidIndex(0) == false;

	for (auto e : { ItemGrade::NORMAL, ItemGrade::MAGIC, ItemGrade::RARE, ItemGrade::EPIC, ItemGrade::UNIQUE, ItemGrade::LEGEND, ItemGrade::ETERNAL })
	{
		if (allAdd || (e >= Options->SystemMessageMax && Options->SystmeMessageOpt.Find(e) == INDEX_NONE))
			Options->SystmeMessageOpt.Add(e);
	}

	GameOptions = Options;
}

void UGsGameOptions::Reset()
{
	if (GameOptions.IsValid())
	{
		GameOptions->SaveOptionConfig();
	}
}

UGsGameOptions* UGsGameOptions::GetGameUserOptions()
{
	if (!GameOptions.IsValid())
	{
		CreateGameOptions();
	}
	return GameOptions.Get();
}

bool UGsGameOptions::GetSystemMessageGradeOption(int32 numVal)
{
	ItemGrade grade = static_cast<ItemGrade>(numVal);

	return SystmeMessageOpt.Find(grade) != INDEX_NONE;
}

void UGsGameOptions::SetSystemMessageGradeOption(int32 numVal, bool use)
{
	ItemGrade grade = static_cast<ItemGrade>(numVal);

	if (use) SystmeMessageOpt.Emplace(grade);
	else SystmeMessageOpt.Remove(grade);
}

