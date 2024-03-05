// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillNotifyBase.h"
#include "DataSchema/Skill/NotifyInfo/GsSchemaSkillNotifyProjectile.h"
#include "DataSchema/Skill/GsSchemaSkillNotifySet.h"
#include "SkillSection/SkillSectionData.h"

#include "ProjectileSkillNotify.generated.h"

/**
 * 
 */
UCLASS(const, hidecategories = Object, collapsecategories, meta = (DisplayName = "Projectile Skill Notify"))
class SKILLEDITOR_API UProjectileSkillNotify : public USkillNotify//USkillNotifyBase
{
	GENERATED_BODY()
	
public:
	UProjectileSkillNotify();

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DC SKillNotifyData")
	FGsSchemaSkillNotifyProjectile NotifyProjecttileInfo;

	UPROPERTY(VisibleAnywhere, Category = "DC SKillNotifyData")
	bool UseSkillNotifyCollision;

	UPROPERTY(VisibleAnywhere, Category = "DC SKillNotifyData")
	int32 SectionIndex;

	UPROPERTY(VisibleAnywhere, Category = "DC SKillNotifyData")
	TWeakObjectPtr<USkillSectionData> SkillSectionData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DC SKillNotifyData")
	FGsSchemaSkillNotifySetRow SkillNotifySet;

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
#if WITH_EDITOR
	//FSimpleDelegate OnChangeProjectileSkillNotify;
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif	

	//// ProjectileId
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SKillNotify")
	//int ProjectileID;

	//// 발사 스폰 거리
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SKillNotify")
	//float Distance = 0.f;

	//// &Yaw (UpVector) 회전 값
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SKillNotify")
	//float Angle = 0.f;

	//// Scale
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SKillNotify")
	//float Scale = 1.f;

	//// 대상 WorldLocation.z + Offset Z
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SKillNotify")
	//float OffsetHeight = -50.f;
};
