// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorEx/GsCharacterNonPlayerTreasure.h"
#include "GsCharacterBase.h"
#include "ActorComponentEx/GsSkillCollisionComponent.h"
#include "ActorComponentEx/GsSkillCollisioHandlerComponent.h"
#include "ActorComponentEx/GsSkillGuideEffectComponent.h"
#include "ActorComponentEx/GsWidgetComponentButton.h"
#include "ActorComponentEx/GsWidgetComponentPooling.h"
#include "Management/ScopeGlobal/GsPathManager.h"

void AGsCharacterNonPlayerTreasure::InitWidgetComponent()
{
	if (_nameplateWidget)
	{
		if (nullptr == _nameplateWidget->GetWidgetClass().Get())
		{
			if (UGsPathManager* pathMgr = GPath())
			{
				if (UClass* widgetClass = pathMgr->GetClass(EGsPathType::UI, TEXT("TreasureMonsterWidget")))
				{
					_nameplateWidget->SetWidgetClassWithPooling(TSubclassOf<UUserWidget>(widgetClass));
				}
			}
		}
	}

	Super::InitWidgetComponent();
}