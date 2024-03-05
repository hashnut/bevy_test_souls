// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "DataSchema/TalkBalloon/GsSchemaTalkBalloonDataSet.h"
#include "AnimNotify_TalkBalloon.generated.h"

/**
 * ¸»Ç³¼± AnimNotify
 */
UCLASS()
class T1PROJECT_API UAnimNotify_TalkBalloon : public UAnimNotify
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FGsSchemaTalkBalloonDataSetRow _talkBalloonData;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
