// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ActorEx/GsSkeletalMeshActor.h"
#include "Animation/AnimNotify/GsMaterialParam.h"
#include "AnimNotifyState_SkeletalMesh.generated.h"
/**
 * https://jira.gamevilcom2us.com/wiki/pages/viewpage.action?pageId=251256802
 * VFX 팀 요청에 의해 SkeletalMesh 출력 기능 추가
 */
class UCurveFloat;
class UCurveVector;
class USkeletalMeshComponent;
class UAnimSequenceBase;


UENUM(BlueprintType, meta = (Tooltip = "스켈레탈메쉬 선택타입"))
enum class EGsSelectSkeletalMesh : uint8
{
	BLUEPRINT = 0	UMETA(Tooltip = "미리 제작된 BP 설정"),
	COMBINE = 1 UMETA(Tooltip = "개별 입력후 조립"),
};

UENUM(BlueprintType, meta = (Tooltip = "머터리얼 제어타입"))
enum class EGsSkeletalMeshMaterial : uint8
{
	NONE = 0	UMETA(Tooltip = "None"),
	SKELETAL_MESH = 1	UMETA(Tooltip = "Mesh Material Control"),
	CUSTOM = 2 UMETA(Tooltip = "Custom Material Contorl"),
};

UCLASS(Blueprintable, meta = (DisplayName = "Skeletal Mesh"))
class T1PROJECT_API UAnimNotifyState_SkeletalMesh : public UAnimNotifyState
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditAnywhere)
	FVector LookAtBaseOffsetPos;

	UPROPERTY(EditAnywhere)
	FRotator LookAtBaseOffsetRot;

	UPROPERTY(EditAnywhere)
	float ActorScale = 1.0f;

	UPROPERTY(EditAnywhere)
	EGsSelectSkeletalMesh SkeletalMeshSelect = EGsSelectSkeletalMesh::BLUEPRINT;

	UPROPERTY(EditAnywhere, Meta = (EditCondition = "SkeletalMeshSelect == EGsSelectSkeletalMesh::BLUEPRINT", EditConditionHides))
	FSoftObjectPath BluePrint;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowedClasses = "SkeletalMesh", EditCondition = "SkeletalMeshSelect == EGsSelectSkeletalMesh::COMBINE", EditConditionHides))
	FSoftObjectPath SkeletalMeshPath;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowedClasses = "AnimationAsset", EditCondition = "SkeletalMeshSelect == EGsSelectSkeletalMesh::COMBINE", EditConditionHides))
	FSoftObjectPath AniPath;

	UPROPERTY(EditAnywhere, Meta = (AllowedClasses = "AnimationAsset", EditCondition = "SkeletalMeshSelect == EGsSelectSkeletalMesh::COMBINE", EditConditionHides))
	bool AniLoop;
	UPROPERTY(EditAnywhere, Meta = (AllowedClasses = "AnimationAsset", EditCondition = "SkeletalMeshSelect == EGsSelectSkeletalMesh::COMBINE", EditConditionHides))
	bool ApplyAniPlayRate = true;

	UPROPERTY(EditAnywhere)
	EGsSkeletalMeshMaterial MaterialControlType = EGsSkeletalMeshMaterial::NONE;

	UPROPERTY(EditAnywhere, Meta = (EditCondition = "MaterialControlType == EGsSkeletalMeshMaterial::CUSTOM", EditConditionHides))
	UMaterialInterface*	CustomMaterial;

	UPROPERTY(EditAnywhere, Meta = (EditCondition = "MaterialControlType == EGsSkeletalMeshMaterial::CUSTOM", EditConditionHides))
	int TargetMaterialIndex = 0;

	UPROPERTY(EditAnywhere, Meta = (EditCondition = "MaterialControlType != EGsSkeletalMeshMaterial::NONE", EditConditionHides))
	TArray<FGsMaterialParam> MaterialPrarams;

	UPROPERTY(EditAnywhere)
	float MeshRemainTime = 0.0f;

private:
	float CurrentTime;
	float NotifyTotalTime;

	// 스폰된 메쉬 액터
	TWeakObjectPtr<AActor> SkeletalMeshActor;

	// 머터리얼 제저용 스켈레탈 메쉬 컴포넌트
	TWeakObjectPtr<USkeletalMeshComponent>	SkeletalMeshCompnent;

	// 효과 노티파이 조사
	bool UseMeshDissolveNotify = false;
public:
	//UObject
	virtual void BeginDestroy() override;

public:
	//UAnimNotifyState
	virtual FString GetNotifyName_Implementation() const override;


	virtual void NotifyBegin(USkeletalMeshComponent* in_MeshComp, UAnimSequenceBase* in_Animation, float in_TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* in_MeshComp, UAnimSequenceBase* in_Animation, float in_FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent* in_MeshComp, UAnimSequenceBase* in_Animation) override;

private:
	// 스켈레탈 메쉬액터 소멸
	void DestorySkeletalMeshActor();

	// 스켈레탈 메쉬액터 초기화
	bool InitSkeletalMeshActor(USkeletalMeshComponent* in_MeshComp);

private:
	// EGsSelectSkeletalMesh 타입에 따라 스켈레탈 메쉬 액터를 스폰한다.
	bool SpawnActorBySkeletalMeshSelect(USkeletalMeshComponent* in_MeshComp);

	// EGsSkeletalMeshMaterial 타입에 따라 머터리얼을 수정한다.
	void ModifySkeletalMeshMaterial();
	
};