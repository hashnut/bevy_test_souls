#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "GsActorSummonComposeCeline.generated.h"

class USkeletalMeshComponent;


UCLASS()
class T1PROJECT_API AGsActorSummonComposeCeline : public AActor
{
	GENERATED_BODY()

	// 멤버 변수
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, EditFixedSize, Category = Summon, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh;
	// 얼굴
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, EditFixedSize, Category = Summon, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* MeshFace;
public:
	AGsActorSummonComposeCeline();

	// logic func
public:
	// 드래그 값 변경
	void OnDragProgress(float In_progressVal, float In_preProgressVal);

	void SetAnimationPause();
};