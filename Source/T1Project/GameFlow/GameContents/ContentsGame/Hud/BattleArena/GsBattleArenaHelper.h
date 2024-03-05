#pragma once
#include "../Sound/GsSoundPlayer.h"
#include "../Management/ScopeGlobal/GsSoundManager.h"


class T1PROJECT_API FGsBattleArenaHelper
{
public:
	static bool FindText(const FTextKey& InKey, OUT FText& OutText)
	{
		if (false == FText::FindText(TEXT("BattleArenaText"), InKey, OutText) || OutText.IsEmpty() )
		{
			OutText = FText::FromString(FString::Format(TEXT("UnKnow Message : {0}"), { InKey.GetChars() }));
			return false;
		}

		return true;
	}

	static void PlayEffectSound(const FString& InResName)
	{
		if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
		{
			soundPlayer->PlaySound2D(InResName);
		}
	}
};