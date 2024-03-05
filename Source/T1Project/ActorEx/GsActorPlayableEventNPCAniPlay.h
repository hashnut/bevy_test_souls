#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "Animation/GsPlayableEventNPCAnimInstance.h"

#include "Runtime/DataCenter/Public/PlayableEvent/GsStatePlayableEventNPCType.h"

#include "GsActorPlayableEventNPCAniPlay.generated.h"

class USkeletalMeshComponent;

UCLASS()
class T1PROJECT_API AGsActorPlayableEventNPCAniPlay : public AActor
{
	GENERATED_BODY()

	// 멤버 변수
protected:
	// 몸통
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, EditFixedSize, Category = Summon, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh;
	// 얼굴
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, EditFixedSize, Category = Summon, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* MeshFace;
public:
	UPROPERTY(Category = PlayableEventNonPlayerComponent, EditAnywhere)
	int _spawnId;

	// 생성자
public:
	AGsActorPlayableEventNPCAniPlay();

	// 로직 함수
public:
	// ani 상태 변경
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "PlayableEventNPCAniPlay")
	void ChangeAniState(EGsStatePlayableEventNPC In_state);

	// get, set
public:

	// body mesh 접근
	USkeletalMeshComponent* GetPartsSkeletalMeshComponentBody()
	{
		return Mesh;
	}
};