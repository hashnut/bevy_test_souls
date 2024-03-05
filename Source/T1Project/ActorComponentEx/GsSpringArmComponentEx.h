#pragma once


#include "CoreMinimal.h"

#include "Engine/Classes/GameFramework/SpringArmComponent.h"

#include "Classes/Engine/EngineBaseTypes.h"

#include "GsSpringArmComponentEx.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class T1PROJECT_API UGsSpringArmComponentEx : public USpringArmComponent
{
	GENERATED_BODY()

public:
	// 물과 카메라 충돌시 몸안에 들어가는 이슈 때문에
	// 원래 체크하던 ArmOrigin 에서(SweepSingleByChannel 시작점)
	// 특정값 뺀만큼(콜리전 size 34 + a) 거리에서 시작
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraCollision, meta = (editcondition = "bDoCollisionTest"))
	float _minimalCamDist = 44.0f;

private:
	// 만약 FHitResult 의 ImpactNormal 값이 아래 값보다 크면, 물인지 여부를 체크한다
	static constexpr float _impactNormalCheckValue = 0.99f;
	// 물과 수직으로 부딪혔을 경우, 최소 오프셋만큼 카메라가 떨어져 있도록 만든다
	static constexpr float _targetArmOffset = 50.f;
	// 물 콜리전 프리셋의 이름 설정
	const FName _waterPreset = FName(TEXT("Water"));
	
public:
	// called tick update
	TFunction<void()> _callbackTickComponent;
public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 카메라와 캐릭터 사이 물 객체가 존재할 때, 충돌 판정이 점으로 잡혀서 카메라가 몸 속으로 파고드는 문제가 발생
	virtual void UpdateDesiredArmLocation(bool bDoTrace, bool bDoLocationLag, bool bDoRotationLag, float DeltaTime) override;
};