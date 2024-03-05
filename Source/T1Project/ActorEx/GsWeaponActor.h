// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "Message/MessageParam/GsEnchantMessageParam.h"
#include "Message/GsMessageContents.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "../../Runtime/DataCenter/Public/ActorComponentEx/EGsMeshShaderEffect.h"
#include "GsWeaponActor.generated.h"

/**
 *
 */
UCLASS()
class T1PROJECT_API AGsWeaponActor : public AActor
{
	GENERATED_BODY()
public:
	UPROPERTY()
		class USceneComponent* _rootComponent;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* _staticMeshComponent;
	UPROPERTY()
		class UGsMeshShaderControlComponent* _meshShaderComponent;
	UPROPERTY(EditAnywhere)
		class USceneComponent* _particleSceneComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UParticleSystemComponent* _particleSystemComponent;
	TArray<TPair<MessageContentEnchant, FDelegateHandle>> _listItemEnchantDelegate;

	UPROPERTY(EditAnywhere)
		class UCurveFloat* _enchanztEffectShowCurve;

	UPROPERTY(EditAnywhere)
		class UCurveFloat* _enchantEffectHideCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "UpdateLocation"))
		class UCurveVector* UpdateLocationCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "UpdateRotation"))
		class UCurveVector* UpdateRotationCurve;

	UPROPERTY()
		float LocationMultiply = 10.0f;

	UPROPERTY()
		float RatationMultiply = 10.0f;

	UFUNCTION(BlueprintImplementableEvent, Category = "Etc")
	void CallBlueprintApplyLodLevelDown(bool flag);

private:
	UPROPERTY()
		class UGsCurveTimer* _timer;

	float fLocationAccumTime;
	float fRotationAccumTime;
private:
	const struct FGsSchemaItemEquipWeaponEnchantEffect* _enchantInfo;

public:
	AGsWeaponActor();
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type inEndPlayReason) override;
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "GsWeapon")
		void SetEnchantEffectAlpha(float inAlpha);

public:
	void StartCurveAnim();
	void StopCurveAnim();

private:
	void OnEnchantSuccess(const struct IGsMessageParam* InMessageParam);
	FSoftObjectPath GetWeaponEffectSoftObjectPath(const CreatureWeaponType inWeaponType, const struct FGsSchemaItemEquipWeaponEnchantEffect* inInfo);
	const struct FGsSchemaItemEquipWeaponEnchantEffect* GetWeaponEnchantEffectInfo(int32 inEnchantLevel);
	void RegisterStaticMeshToMeshShader();
	void SetEnchantEffect(const CreatureWeaponType inWeaponType, const struct FGsSchemaItemEquipWeaponEnchantEffect* inInfo);

public:
	class UStaticMeshComponent* GetStaticMeshComponent() { return _staticMeshComponent; }
	class UGsMeshShaderControlComponent* GetMeshShaderControlComponent() { return _meshShaderComponent; }
	void SetEnchantEffect(const CreatureWeaponType inWeaponType, const int32 inLevel);
	void SetEnchantEffect(FGsItem* inItem);

	void ShowEnchantEffect(class UCurveFloat* WeaponDissolve, class UCurveFloat* WeaponFrsnel);
	void HideEnchantEffect(class UCurveFloat* WeaponDissolve, class UCurveFloat* WeaponFrsnel);

	void ShowMeshShaderEffect(EGsMeshShaderEffect inType, const class UCurveFloat* inCurveFloat, bool inIsRestor = false);

public:
	UFUNCTION(BlueprintCallable)
		void ChangeLightingChannels(bool channel1 = true, bool channel2 = false, bool channel3 = false);
};
