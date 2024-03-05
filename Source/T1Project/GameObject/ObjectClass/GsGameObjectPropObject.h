// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "../ActorEx/GsActorPropObject.h"
#include "GsGameObjectPropObject.generated.h"

/**
 * 설명 : 맵에 스폰되는 Character를 제외한 모든 액터의 부모
 * 인터렉션 가능한 프랍과 드랍 오브젝트의 부모이다.
 */
UCLASS()
class T1PROJECT_API UGsGameObjectPropObject : public UGsGameObjectBase
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient, VisibleInstanceOnly, Category = GameObject, Meta = (AllowPrivateAccess = true))
		class AGsActorPropObject* _actor;

private:
	bool _isCanceling;

public:
	virtual ~UGsGameObjectPropObject() {}

	virtual void Initialize(const struct FGsSpawnParam* SpawnParam) override;
	virtual void CreateSubClass() override;

protected:
	virtual void InitializeActor(class AActor* Owner);
	virtual void ActorDespawn() override;

public:
	UFUNCTION()
		virtual void OnBeginOverlap(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 intoherBodyIndex, bool inbFromSweep, const FHitResult& Hit) {}
	UFUNCTION()
		virtual void OnEndOverlap(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 intoherBodyIndex) {}

public:	
	virtual void InvalidWidget() {}
	virtual void HideWidget() {}
	virtual void CancelPropInteraction(){}
	virtual void SetInteractPropEnd(bool inSuccess) {}

public:
	virtual EGsGameObjectType GetObjectType() const override { return EGsGameObjectType::PropObject; }
	virtual AActor* GetActor() const override { return Cast<AActor>(_actor); }
	virtual void ClearActor() override { _actor = nullptr; }
	virtual class AGsActorPropObject* GetActorProp() const { return(GetActor()) ? Cast<AGsActorPropObject>(GetActor()) : nullptr; }
	bool IsCanceling() {return _isCanceling;}
	void SetCanceling(bool inIsCanceling) {	_isCanceling = inIsCanceling;}

};
