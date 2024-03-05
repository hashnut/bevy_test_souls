// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorEx/GsCampBase.h"
#include "GsCharacterLocalPlayer.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "T1Project.h"

#include "UI/UIContent/Billboard/GsUIBillboardCamp.h"


AGsCampBase::AGsCampBase() : Super()
{
}

bool AGsCampBase::FindLocalPlayerPtr()
{
	UGsGameObjectManager* objMgr = GSGameObject();
	if (nullptr == objMgr)
		return false;

	if (UGsGameObjectBase* findLocal = objMgr->FindObject(EGsGameObjectType::LocalPlayer))
	{
		UGsGameObjectLocalPlayer* localPlayer = findLocal->CastGameObject<UGsGameObjectLocalPlayer>();
		AActor* localActor = localPlayer->GetActor();
		_localPlyerPtr = Cast<AGsCharacterLocalPlayer>(localActor);

		return true;
	}

	return false;
}

void AGsCampBase::SetNameplateHpBar(bool bIsShow, float InHpRate)
{
	if (UGsUIBillboardCamp* namePlate = GetCampNameplateWidget())
	{
		namePlate->ShowHPBar(bIsShow);
		namePlate->SetProgressBarValue(InHpRate);
	}
}

void AGsCampBase::SetSanctumName(const FText& InText)
{
	if (UGsUIBillboardCamp* namePlate = GetCampNameplateWidget())
	{
		namePlate->SetSanctumNameText(InText);
	}
}

void AGsCampBase::SetShowNamePanel(bool InShow)
{
	if (UGsUIBillboardCamp* namePlate = GetCampNameplateWidget())
	{
		namePlate->ShowNamePanel(InShow);
	}
}
