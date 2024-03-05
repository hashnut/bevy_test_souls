// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIHUDEmoticon.h"
#include "UserWidget.h"
#include "Engine/AssetManager.h"
#include "NpcDialog/GsSchemaEmoticonResData.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "UI/UILib/Manager/GsEmoticonManager.h"


void UGsUIHUDEmoticon::NativeDestruct()
{
	StopAllAnimationsEx();

	Super::NativeDestruct();
}

void UGsUIHUDEmoticon::OnFinished()
{
	Release();
}

void UGsUIHUDEmoticon::Release()
{
	// Ç®¿¡ ¹Ý³³
	if (UGsUIManager* uimgr = GUI())
	{
		UGsEmoticonManager* emoticonManager = uimgr->GetEmoticonManager();
		if (nullptr != emoticonManager)
		{
			emoticonManager->ReleaseWidget(this);
		}
	}
}

void UGsUIHUDEmoticon::PlayEmoticon(const FGsSchemaEmoticonResData* InData, FVector InLocation)
{
	if (UObject* loadObject = UAssetManager::GetStreamableManager().LoadSynchronous(InData->emoticonRes))
	{
		_emoticonImg.SetResourceObject(loadObject);
	}

	_targetLocation = InLocation;

	SetTransformInter();
}

void UGsUIHUDEmoticon::OnReleaseComplete()
{
	StopAllAnimationsEx();
}

void UGsUIHUDEmoticon::SetTransformInter()
{
	FVector2D screenLocation = FVector2D::ZeroVector;
	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	if (nullptr != playerController)
	{
		playerController->ProjectWorldLocationToScreen(_targetLocation, screenLocation);
	}

	SetPositionInViewport(screenLocation);
}