// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "Runtime/Core/Public/Math/Box.h"
#include "ActorComponentEx/GsSceneComponent.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "DataSchema/Skill/GsSchemaSkillCollision.h"
#include "GsSkillCollisionComponent.generated.h"


USTRUCT(BlueprintType)
struct FGsSkillCollisionData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	SkillCollisionType _type;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float _maxRadius;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float _minRadius;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float _degAngle;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<FVector> _listLine;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float _offsetAngle;

	void Empty()
	{
		_type = SkillCollisionType::NONE;
		_maxRadius = 0.f;
		_minRadius = 0.f;
		_degAngle = 0.f;
		_listLine.Empty();
	}
};

/**
* 스킬 충돌 판정 컴퍼넌트
* 이 컴퍼넌트의 TM이 실제 Offset역활을 하며(FTransform)
* Set() 함수를 통해 정보를 설정한뒤 IsInSideCheck() 함수로 판정 처리를 진행
*/

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class T1PROJECT_API UGsSkillCollisionComponent : public USceneComponent
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGsSchemaSkillCollision _debugNotifyData;

	FGsSkillCollisionData _collisionData;
	const FColor _debugColor = FColor::Blue;

public:
	// 에디터등 특수 상황에서 초기화 회전 OffSet
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator _defaultRotator = FRotator::ZeroRotator;

public:
	UPROPERTY(EditAnywhere)
	bool _useDebugLine = false;
	UPROPERTY(EditAnywhere)
	float _debugLineLifeTime = 1.f;

	float _debugLineHeight = 0.f;

public:	
	// Sets default values for this component's properties
	UGsSkillCollisionComponent();

public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

#if WITH_EDITOR
public:
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif

public:
	void Set(const FGsSchemaSkillCollision& Data);
	//임의의 좌표로 설정할 경우(ex 타겟팅)
	void Set(const FVector& Location, const FGsSchemaSkillCollision& Data);

protected:
	void SetCollision(const FGsSchemaSkillCollision& Data);

	void SetCircle(const FGsSchemaSkillCollision& Data);
	void SetFan(const FGsSchemaSkillCollision& Data);
	void SetConcave(const FGsSchemaSkillCollision& Data);

public:
	bool IsInSideCheck(const FVector& Point) const;
	bool IsInsideCheck(const FVector& Point, float Radius) const;

protected:
	bool IsInSideCircle(const FVector& Point) const;
	bool IsInSideFan(const FVector& Point) const;
	//bool IsInSideConcave(const FVector& Point) const;

	bool IsInSideCircle(const FVector& Point, float Radius) const;
	bool IsInSideFan(const FVector& Point, float Radius) const;
	bool IsInSideRect(const FVector& Point, float Radius) const;
	
	bool IsInSideCircle2D(const FVector2D& Point) const;
	bool IsInSideFan2D(const FVector2D& Point) const;
	//bool IsInSideConcave2D(const FVector2D& Point) const;

	bool IsInSideCircle2D(const FVector2D& Point, float Radius) const;
	bool IsInSideFan2D(const FVector2D& Point, float Radius) const;
	bool IsInSideRect2D(const FVector2D& Point, float Radius) const;

	//int GetRectZone(const FBox& Rect, const FVector& Point) const;
	//bool IsInSideRect2D(const FBox& Rect, const FVector& Point, float Radius) const;

public:
	// AttachScene 의 현재 좌표 정보 얻기
	FVector GetCollisionLocation() const;
	// AttachScene 의 현재 회전 정보 얻기
	FRotator GetCollisionRotate() const;
	

#if WITH_EDITOR
	void DrawCollision(float DrawTime = -1.f);
private:
	void DrawCircle(float DrawTime);
	void DrawConcave(float DrawTime);
	void DrawFan(float DrawTime);
#endif
};
