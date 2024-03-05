// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsGameObjectNonPlayerBase.h"
#include "GameObject/Interaction/GsGameObjectInteractionInterface.h"

#include "GsGameObjectSandbag.generated.h"

/**
 * 
 */

UCLASS()
class T1PROJECT_API UGsGameObjectSandbag : public UGsGameObjectNonPlayerBase, public IGsGameObjectInteractionInterface
{
	GENERATED_BODY()

private:
	bool _isCanHit;

	//Quest 연출
protected:
	bool _isInitVisible = true;
	EGsVisibleType _visibleType = EGsVisibleType::Show;
	TArray<FGsSpawnObjectShowHide> _questCondtion;
	
public:
	virtual ~UGsGameObjectSandbag() = default;

	// 각종 가상함수
public:
	virtual void Initialize(const struct FGsSpawnParam* inSpawnParam) override;
	virtual void InitializeActor(AActor* inOwner) override;
	
	virtual void UpdateVisible() override;

protected:
	virtual TSharedPtr<FGsMovementHandlerBase> CreateMovement() override;

	virtual void InitializeFSM() override;

public:
	virtual EGsGameObjectType GetObjectType() const override { return EGsGameObjectType::Sandbag; }

public:	
	UFUNCTION(BlueprintCallable, Category = "GameObject")
	class AGsCharacterSandbag* GetSandbag();

public:
	virtual void SetQuestMarkActive(bool In_isActive, int In_typeIndex = 0) override;
	virtual void SetVisibleInfo(const FGsSchemaSpawnElement* inVisible) override;
	// visible 바로 세팅(대사 연출 관련 처리)
	virtual void SetGameObjectActorVisible(bool In_isVisible) override;
	
public:
	virtual bool IsAble() override;

	// 몸통 클릭 타게팅
	void OnClickTargeting();

public:
	void StartFadeIn();
	void StartFadeOut();
};
