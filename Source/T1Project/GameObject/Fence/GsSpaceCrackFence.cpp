// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSpaceCrackFence.h"
#include "UTIL/GsText.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "GsFenceHandler.h"
#include "Management/ScopeGame/GsSpaceCrackManager.h"
#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsGameObjectMessageParam.h"

void FGsSpaceCrackFence::InitSpaceCrack(const FGsSchemaMapTerritorySpaceCrack* inSpaceCrack, const struct FGsSchemaFenceElement* inFence)
{
	_spaceCrack = inSpaceCrack;
	_fence = inFence;
}

bool FGsSpaceCrackFence::Enter()
{
	const struct FGsSchemaMapTerritorySpaceCrack* spaceCrack = GetSpaceCrackData();	

	FGsGameObjectFenceParam param = FGsGameObjectFenceParam(this, spaceCrack->envEvent);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::ENV_TERRITORY_ENTERED, &param);

	return true;
}

bool FGsSpaceCrackFence::Leave()
{
	FGsGameObjectFenceParam param = FGsGameObjectFenceParam(this, EGsEnvEvent::NONE);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::ENV_TERRITORY_LEFT, &param);

	return true;
}

FString FGsSpaceCrackFence::GetFenceDescription()
{
	FString descriptionText;
	if (nullptr == _spaceCrack)
	{
		return descriptionText;
	}

	FString territoryTypeText = TEXT("Type : ") + FGsTextUtil::GetStringFromEnum(TEXT("TerritoryType"), TerritoryType::SPACE_CRACK);	
	FString soundIdText;	
	FString envEventTypeeText = TEXT("Env : ") + FGsTextUtil::GetStringFromEnum(TEXT("EGsEnvEvent"), _spaceCrack->envEvent);
		
	descriptionText += (TEXT("\n") + FString::Printf(TEXT("envEventTypeeText : %s"), *envEventTypeeText));

	return descriptionText;
}

bool FGsSpaceCrackFence::HaveActiveCondition()
{
	return true;
}

bool FGsSpaceCrackFence::IsActive() const
{
	//get space crack manager
	if (GSSpaceCrack()->IsCurrentMapActiveSpaceCrack())
	{
		int64 spaceCrackFenceId = GSSpaceCrack()->GetCurrentActivateSpaceCrack();

		//get activate id
		if (spaceCrackFenceId == _spaceCrack->fenceId)
		{
			return true;
		}
	}	
	
	return false;
}
