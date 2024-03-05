// Fill out your copyright notice in the Description page of Project Settings.


#include "Option/GsAlertHelper.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameFramework/PlayerController.h"
#include "ActorEx/GsCharacterBase.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"
#include "GameFramework/ForceFeedbackEffect.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Sound/GsSoundPlayer.h"
#include "Option/GsOptionValueDefine.h"
#include "T1Project.h"

void FGsAlertHelper::PlayAlertEffect(EGsAlertType inAlertType, FSoftObjectPath inResource /* = FSoftObjectPath(TEXT("None")) */)
{
	if (inAlertType == EGsAlertType::VibrateAlert)
	{
		UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
		if (local && local->GetCharacter())
		{
			if (APlayerController* controller = Cast<APlayerController>(local->GetCharacter()->GetController()))
			{
				// 2022/10/04 PKT - Default
				FSoftObjectPath softPath = GData()->GetGlobalData()->_forceFeedbackEffect;
				if (inResource.IsValid())
				{
					softPath = inResource;
				}

				UObject* load = UAssetManager::GetStreamableManager().LoadSynchronous(softPath);
				if (load == nullptr)
				{
					GSLOG(Error, TEXT("Invalidate Vibrate Alert Resource [%s]"), *softPath.ToString());
					return;
				}

				UForceFeedbackEffect* effect = Cast<UForceFeedbackEffect>(load);
				if (effect == nullptr)
				{
					return;
				}

				controller->ClientPlayForceFeedback(effect);
			}
		}
	}
	else
	{
		if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
		{
			FSoftObjectPath softPath;

			switch (inAlertType)
			{
			case EGsAlertType::RingerAlert:
				softPath = GData()->GetGlobalData()->_alertRingSound;
				break;
			case EGsAlertType::PvpAlert:
				softPath = GData()->GetGlobalData()->_alertPvpAttackSound;
				break;
			case EGsAlertType::LowHpAlert:
				softPath = GData()->GetGlobalData()->_alertLowHpSound;
				break;
			default:
				break;
			}

			if (inResource.IsValid())
			{
				softPath = inResource;
			}

			const static int64 INVLIDATE_INDEX = 0;
			if (INVLIDATE_INDEX >= soundPlayer->PlaySoundByPath(softPath))
			{
				GSLOG(Error, TEXT("Not Find Alert Sound Resource [%s]"), *softPath.ToString());
				return;
			}
		}
	}
}

