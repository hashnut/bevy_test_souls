/*
	RemortPc가 전송한 정보를 디버깅하기위한 클래스

*/


#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SceneComponent.h"
#include "GsMovementDebugShapeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class T1PROJECT_API UGsMovementDebugShapeComponent : public USceneComponent
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere)
	float	_helfheight					= 90.0f;

	UPROPERTY(EditAnywhere)
	float	_radius						= 25.0f;

	UPROPERTY(EditAnywhere)
	int		_segments					= 12;

	UPROPERTY(EditAnywhere)
	FLinearColor _locationColor			= FColor::Blue;

	UPROPERTY(EditAnywhere)
	FLinearColor _destinationColor		= FColor::Red;

	UPROPERTY(EditAnywhere)
	FLinearColor _serverLocationColor	= FColor::Green;

	UPROPERTY(EditAnywhere)
	FLinearColor _epicLocationColor		= FColor::Cyan;

	UPROPERTY(EditAnywhere)
	FLinearColor _directionColor		= FColor::Black;

	UPROPERTY(EditAnywhere)
	FLinearColor _serverDebugColor		= FColor::Purple;

	UPROPERTY(EditAnywhere)
	float	_lifeTime					= 0.0f;

	UPROPERTY(EditAnywhere)
	float	_lineThickness				= 0.0f;

	UPROPERTY(EditAnywhere)
	bool	_showDebugShape				= true;

private:
	FVector _location					= FVector::ZeroVector;
	FVector _destination				= FVector::ZeroVector;
	FVector _serverLocation				= FVector::ZeroVector;

	FVector _epicLocation				= FVector::ZeroVector;
	FVector _epicVelocity				= FVector::ZeroVector;
	FVector _netDirection				= FVector::ZeroVector;

	FVector _ServerDebugPos				= FVector::ZeroVector;

	FString _debugString				= TEXT("TEST");

public:
	// Sets default values for this component's properties
	UGsMovementDebugShapeComponent();

public:
	void SetLocation(const FVector& inLocation)				{ _location = inLocation; }
	void SetDestination(const FVector& inDestination)		{ _destination = inDestination; }
	void SetServerLocation(const FVector& inServerLocation) { _serverLocation = inServerLocation; }
	void SetEpicLocation(const FVector& inEpicLocation)		{ _epicLocation = inEpicLocation; }
	void SetEpicVelocity(const FVector& inEpicVelocity)		{ _epicVelocity = inEpicVelocity; }
	void SetNetDirection(const FVector& inDirection)		{ _netDirection = inDirection; }
	void SetServerDebugPos(const FVector& inLocation)		{ _ServerDebugPos = inLocation; }
	void SetDebugString(const FString& inText)				{ _debugString = inText; }	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

#if WITH_EDITOR
private:
	void DrawDebugShape();
	void DrawDebugCylinder(const FVector& inLocation, const FLinearColor& inColor);
	void DrawDebugArrow(const FVector& inDirection, const FLinearColor& inColor);
	void DrawDebugString(const FString& inText, const FLinearColor& inColor, const FVector& inOffset);
#endif
};
