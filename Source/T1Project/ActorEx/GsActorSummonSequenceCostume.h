#pragma once
#include "CoreMinimal.h"
#include "ActorEx/GsActorSummonCostume.h"

#include "GsActorSummonSequenceCostume.generated.h"


class UGsGameObjectLocalPlayer;

// 소환후 연출에 활용되는 액터

UCLASS()
class T1PROJECT_API AGsActorSummonSequenceCostume : public AGsActorSummonCostume
{
	GENERATED_BODY()

	
public:
	virtual void BeginDestroy() override;

	// 로직 함수
public:
	// 현재 플레이어와 선택된 코스튬 적용
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "CharacterSequencePlayer")
	void ApplyCurrentLocalPlayerAndSelectCostume(UGsGameObjectLocalPlayer* In_GameObjectLocalPlayer,
		int In_costumeId);

	// 코스튬 뽑기 시퀀스에서 사용( 고등급 있때는 황금색 (누군지 모르게) 처리)
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "CharacterSequencePlayer")
	void ApplyCurrentLocalPlayerAndSelectCostumeWithVeilCostume(UGsGameObjectLocalPlayer* In_GameObjectLocalPlayer,
		int In_costumeId, bool In_isHigh);

	// 대사 ui 출력
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "CharacterSequencePlayer")
	void ShowTraySummonDialogue(int In_costumeId);

	// 정보 ui 출력
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "CharacterSequencePlayer")
	void ShowTraySummonInfo(int In_costumeId);

	// 획득 ui 출력
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "CharacterSequencePlayer")
	void ShowTraySummonObtain(int In_costumeId);

	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "CharacterSequencePlayer")
	void MakeSpawnEffectCostume(bool In_isHighGrade);
};