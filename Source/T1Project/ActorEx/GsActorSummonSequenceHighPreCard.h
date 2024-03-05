#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "GsActorSummonSequenceHighPreCard.generated.h"

/*
* 뽑기: 테미르 손에 회전하는 카드(코스튬, 페어리)
*/
class USkeletalMeshComponent;
UCLASS()
class AGsActorSummonSequenceHighPreCard : public AActor
{
	GENERATED_BODY()

	// 멤버 변수
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, EditFixedSize, Category = Summon, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh;

	// 생성자
public:
	AGsActorSummonSequenceHighPreCard();
	
	// ufunction
public:
	// 카드 세팅(현재)
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "SequenceSummon")
	void ApplyCurrentTypeCard();

	// get
public:
	USkeletalMeshComponent* GetSkeletalMeshComponent()
	{
		return Mesh;
	}
};