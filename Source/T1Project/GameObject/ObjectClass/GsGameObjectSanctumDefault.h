// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameObject/ObjectClass/GsGameObjectSanctumBase.h"
#include "GsGameObjectSanctumDefault.generated.h"

class AGsCompleteCamp;
struct FGsSchemaSanctumNexusData;

/**
 * 성소 인터렉션 불가 ~ 인터렉션 가능 (건설 전)
 * 
 * 일단 임시로 캠프 재활용
 * AGsCompleteCamp
 */
UCLASS()
class T1PROJECT_API UGsGameObjectSanctumDefault final : public UGsGameObjectSanctumBase
{
	GENERATED_BODY()

public:
	virtual ~UGsGameObjectSanctumDefault() = default;

public:
	virtual void InitializeActor(AActor * Owner) override;
		
	// IGsGameObjectInteractionInterface override
public:
	virtual void SetInteractionActive(bool In_isActive, int In_index = 0) override;
	virtual void OnClickInteract(bool In_isByWidget) override;

	// UGsGameObjectSanctumBase override
public:
	virtual void OnSanctumSataus(SanctumAreaId InSanctumId, bool bInCanInteract) override;
	virtual void UpdateSanctumName(SanctumAreaId InSanctumId) override;
	virtual void UpdateInteractionState() override;
	virtual void OnBattleEnd() override;
	virtual bool IsNexus() const override { return false; };

public:
	AGsCompleteCamp* GetCampActor();
};
