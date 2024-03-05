#include "GsUITraySummonDialogue.h"

#include "Management/ScopeGame/GsFairyManager.h"
#include "Management/ScopeGame/GsCostumeManager.h"
#include "Management/ScopeGlobal/GsLocalizationManager.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Sound/GsSoundPlayer.h"

#include "Fairy/GsFairyData.h"
#include "Costume/GsCostumeData.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "UMG/Public/Animation/WidgetAnimation.h"

// 데이터 세팅
void UGsUITraySummonDialogue::SetData(EGsSummonType In_type, int In_tblId)
{
	if (In_type == EGsSummonType::SUMMON_FAIRY)
	{
		const TWeakPtr<FGsFairyData> findData = GSFairy()->GetFairyData((FairyId)In_tblId);
		if (findData.IsValid() == false)
		{
			return;
		}

		TSharedPtr<FGsFairyData> data = findData.Pin();

		_textDialogue = data->GetSummonFairyDialogue();

#ifdef USE_LOCALIZED_SOUND
		if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
		{
			if (USoundBase* fairyVoice = GLocalization()->GetCharacterSound(data->GetSummonFairyVoiceFilePath()))
			{
				soundPlayer->PlaySoundObject(fairyVoice);
			}
		}
#endif
	}
	else if (In_type == EGsSummonType::SUMMON_COSTUME)
	{
		const TWeakPtr<FGsCostumeData> findData = GSCostume()->GetCostumeData((CostumeId)In_tblId);
		if (findData.IsValid() == false)
		{
			return;
		}
		TSharedPtr<FGsCostumeData> data = findData.Pin();

		_textDialogue = data->GetSummonCostumeDialogue();

	}

	PlayAnimation(_animation);

	FScriptDelegate animationDelegate;
	animationDelegate.BindUFunction(this, "FinishAnimationBP");

	BindToAnimationFinished(_animation, FWidgetAnimationDynamicEvent(animationDelegate));
}

// 연출 끝
void UGsUITraySummonDialogue::FinishAnimationBP()
{
	FScriptDelegate animationDelegate;
	animationDelegate.BindUFunction(this, "FinishAnimationBP");

	UnbindFromAnimationFinished(_animation, FWidgetAnimationDynamicEvent(animationDelegate));

	Close();
}