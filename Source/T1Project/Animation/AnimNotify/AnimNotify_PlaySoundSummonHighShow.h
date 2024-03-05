#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_PlaySoundSummonHighShow.generated.h"

class UAnimSequenceBase;
class USkeletalMeshComponent;

// 뽑기 고등급 등장 사운드(시퀀스에서 stop 처리하기 위한)
// https://jira.com2us.com/jira/browse/C2URWQ-5524

UCLASS(hidecategories = Object, meta = (DisplayName = "Play Sound Summon High Show"))
class T1PROJECT_API UAnimNotify_PlaySoundSummonHighShow : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify", meta = (ExposeOnSpawn = true))
	FString _summonHighShowSoundKey;

};