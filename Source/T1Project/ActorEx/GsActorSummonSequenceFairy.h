#pragma once
#include "CoreMinimal.h"

#include "ActorEx/GsActorSummonFairy.h"
#include "Animation/GsSummonFairyAnimInstance.h"

#include "GsActorSummonSequenceFairy.generated.h"

UCLASS()
class T1PROJECT_API AGsActorSummonSequenceFairy : public AGsActorSummonFairy
{
	GENERATED_BODY()

	// 가상 함수
public:
	// 스케일 초기화(씨네마틱에서 scale 조절하고있어서 여기선 안함)
	virtual void InitScale() override {}
	
	// 로직 함수
public:
	// 선택된 페어리로 세팅(시네마틱으로 넘김)
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "CharacterSequencePlayer")
	void ApplySelectFairy(AGsActorSummonFairy* In_fairy);

	// 획득 ui 출력
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "CharacterSequencePlayer")
	void ShowTraySummonObtain(AGsActorSummonFairy* In_fairy);
	// 대사 ui 출력
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "CharacterSequencePlayer")
	void ShowTraySummonDialogue(AGsActorSummonFairy* In_fairy);
	// 정보 ui 출력
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "CharacterSequencePlayer")
	void ShowTraySummonInfo(AGsActorSummonFairy* In_fairy);

	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "CharacterSequencePlayer")
	void MakeSpawnEffectFairy(bool In_isHighGrade);
};