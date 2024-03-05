// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSignificanceManager.h"
#include "Particles/ParticleSystemComponent.h"
#include "FileHelper.h"
#include "T1Project.h"
#include "ParticleHelper.h"
#include "UObject/UObjectBaseUtility.h"
#include "Management/ScopeGlobal/GsOptionManager.h"

const FName VFX_NAME = "Particle";

UGsSignificanceManager::UGsSignificanceManager()
{
	if (false == IsTemplate())
	{
		UParticleSystemComponent::OnSystemPreActivationChange.AddUObject(
			this, &UGsSignificanceManager::OnActivateParticle
		);
	}
}

float BasicSignificanceForVFX(USignificanceManager::FManagedObjectInfo* inObj, const FTransform& inViewPoint)
{
	UGsGameUserSettings* Setting = GGameUserSettings();
	if (nullptr == Setting)
		return 3.0f;
	
	//LOW = 0,		// 하
	//MID = 1,		// 중
	//HIGH = 2,		// 상 
	//TOP = 3,		// 최상

	int32 OptionValue = 3;
	Setting->GetGraphicSettingByType(EGsOptionGraphic::PARTICLE_QUALITY, OptionValue);
	return float(OptionValue);
}

void PostSignificanceForVFX(USignificanceManager::FManagedObjectInfo* inObj, float inOldSignificance, float inNewSignificance, bool inIsFinal)
{
	if (nullptr == inObj->GetObject())
	{
		return;
	}

	UParticleSystemComponent* particle = CastChecked<UParticleSystemComponent>(inObj->GetObject());
	if (nullptr == particle)
	{
		return;
	}

	particle->SetManagingSignificance(true);

	if (inIsFinal)
	{
		particle->SetRequiredSignificance(EParticleSignificanceLevel::Critical);
		return;
	}
	
	if ((inOldSignificance != inNewSignificance))
	{
		if (inNewSignificance == 0.0f)
		{
			particle->SetRequiredSignificance(EParticleSignificanceLevel::Critical);			
		}
		else if (inNewSignificance == 1.0f)
		{
			// 오재형 파트장님(이펙트)과 Critical / Medium / Low 로 처리하기로 협의
			particle->SetRequiredSignificance(EParticleSignificanceLevel::Medium);			
		}
		else if (inNewSignificance == 2.0f)
		{
			particle->SetRequiredSignificance(EParticleSignificanceLevel::Low);
		}
		else
		{
			particle->SetRequiredSignificance(EParticleSignificanceLevel::Low);
		}
	}
}


//https://youtu.be/gs7jnCejEUA?t=803
void UGsSignificanceManager::OnActivateParticle(UParticleSystemComponent* inParticle, bool inActive)
{
	if (nullptr == inParticle)
	{
		return;
	}

	if (nullptr == GetWorld())
	{
		return;
	}

	if (false == GetWorld()->IsGameWorld())
	{
		return;
	}

	if (inActive)
	{
		RegisterObject(inParticle, VFX_NAME, &BasicSignificanceForVFX, USignificanceManager::EPostSignificanceType::Concurrent, &PostSignificanceForVFX);
	}
	else
	{
		UnregisterObject(inParticle);
	}
}

