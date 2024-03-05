// Fill out your copyright notice in the Description page of Project Settings.
// BSAM - Define CollisionSkillNotify to use in engine and editor

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "DataSchema/Skill/GsSchemaSkillCollision.h"
//#include "GameObject/Component/GsSkillCollisionComponent.h"

#include "CollisionSkillNotify.generated.h"

/**
 * 
 */
//class UGsSkillNotifyDataBase;
//class UGsSkillCollisionComponent;
//class UDrawPrimitiveComponent;
//class UParticleSystem;
//class USkillNotifyBase;

//UCLASS(const, hidecategories = Object, collapsecategories, meta = (DisplayName = "Collision Skill Notify"))
UCLASS(HideDropdown)
class SKILLEDITOR_API UCollisionSkillNotify : public USkillNotify //public USkillNotify//, public UDrawPrimitiveComponent
{
	GENERATED_BODY()

public:
	UCollisionSkillNotify();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DC SKillNotifyData")
	FGsSchemaSkillCollision NotifyCollisionInfo;

#if WITH_EDITOR
	FSimpleDelegate OnChangeCollisionSkillNotify;
#endif

	// Begin UObject interface
	virtual void PostLoad() override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	// End UObject interface

	// Begin UAnimNotify interface
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
#if WITH_EDITOR
	virtual void ValidateAssociatedAssets() override;
#endif
	// End UAnimNotify interface
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionSKillNotify")
	UGsSkillNotifyDataBase* NotifyData;*/

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionSKillNotify")
	//SkillCollisionType CollisionType;	

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionSKillNotify")
	//FTransform Tm;

	//////부가 효과
	////UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionSKillNotify")
	////EGsSkillCollisionAdditionType AdditionType;
	//// 수집 대상 갯수
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionSKillNotify")
	//int CollisionCount;

	//// 실제 충돌 정보를 입력하게 됐으므로 삭제 해야됨 (삭제)
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionSKillNotify")
	//int SkillNotifyGuideEffectId;

	//// 스폰 이펙트 정보 (추가)
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionSKillNotify")
	//UParticleSystem*			Particle;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillNotifyReaction")
	//int ReactionId;

	//// 부가 효과
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillNotifyReaction")
	//EGsSkillCollisionAdditionType AdditionType;

	//// 지속 시간
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillNotifyReaction")
	//float Duration;		

	////---------------------------------------------------------------------
	////
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DrawData")
	//float Radius;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DrawData")
	//float DegAngle;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DrawData")
	//float Width;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DrawData")
	//float Heigt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DrawData")
	FTransform Tm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DrawData")
	float LifeTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DrawData")
	bool FillMode;
	

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionSKillNotify")
	TArray<FVector2D> ListPoint;*/

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionSKillNotify")*/
	/*UGsSkillCollisionComponent *SkillCollisionComponent;

	UDrawPrimitiveComponent* DrawPrimitiveComponent;
	FPrimitiveSceneProxy* PrimitiveSceneProxy;*/
};