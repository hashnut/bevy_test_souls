#pragma once
#include "CoreMinimal.h"

struct BannedWordFileRowData {

	enum class ACT : bool { Off = false, On = true };

	FString _word;
	FString _language;
	FString _country;
	FString _usage;

	ACT _action{ ACT::On };
};

const FString BANNEDWORD_INFOFILE = TEXT("bannedword.info");
const FString BANNEDWORD_BUNDDLEFILE = TEXT("bannedword.bin");