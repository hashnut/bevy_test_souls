#pragma once

#include "CoreMinimal.h"

#include "Engine/Classes/Particles/Emitter.h"

#include "GsActorSummonSequenceEmitter.generated.h"

UCLASS()
class AGsActorSummonSequenceEmitter : public AEmitter 
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "SequenceEmitter")
	void ApplyParticle(bool In_isHighCardExist, bool In_isFairySummon);

	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "SequenceEmitter")
	void ApplyCostumeParticle(bool In_isHighCardExist, int In_effectNumber);

	// 날라가는 이펙트(일반, 고등급)
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "SequenceEmitter")
	void ApplyFairyMoveLightParticle(bool In_isHighCardExist);

	// 테미르 쾅에서 등급 결정 이펙트 출력
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "SequenceEmitter")
	void ApplyHighGetPreGradeParticle();
};