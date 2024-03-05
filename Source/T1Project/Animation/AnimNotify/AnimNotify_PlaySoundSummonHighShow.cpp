#include "AnimNotify_PlaySoundSummonHighShow.h"

#include "Management/ScopeGame/GsSummonManager.h"

#include "GameFlow/GameContents/ContentsGAme/Hud/Summon/GsSummonHandler.h"
void UAnimNotify_PlaySoundSummonHighShow::Notify(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation)
{
	FGsSummonHandler* summonHandler = UGsSummonManager::GetSummonHandler();
	if (summonHandler == nullptr)
	{
		return;
	}
	summonHandler->PlaySummonHighShowSound(true, _summonHighShowSoundKey);
}