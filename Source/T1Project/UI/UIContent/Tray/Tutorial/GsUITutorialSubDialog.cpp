// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITutorialSubDialog.h"
#include "Components/RichTextBlock.h"
#include "DataSchema/SubDialog/GsSchemaSubDialogActorData.h"
#include "DataSchema/SubDialog/GsSchemaSubDialogCutData.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Data/GsResourceManager.h"
#include "UTIL/GsGameObjectUtil.h"
#include "Classes/GsSpawn.h"
#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"


void UGsUITutorialSubDialog::NativeDestruct()
{
	DestroyActor();

	Super::NativeDestruct();
}

void UGsUITutorialSubDialog::StartAction(const FGsSchemaSubDialogCutData* inCutData)
{
	// 시작 할 때 앞전 액터가 남아 있으면 삭제
	DestroyActor();

	if (nullptr != inCutData)
	{
		_nameText = inCutData->name;
		FString strUserName;

		const FGsNetUserData* userData = GGameData()->GetUserData();
		if (userData)
		{
			strUserName = userData->mUserName;
		}

		FText textScrip = inCutData->script;
		FString strScript = inCutData->script.ToString();
		if (strScript.Contains(TEXT("{0}")))
		{
			textScrip = FText::Format(inCutData->script, FText::FromString(strUserName));
		}

		_richTextScript->SetText(textScrip);

		FGsSchemaSubDialogActorDataRow actorDataRow = inCutData->actor;
		if (const FGsSchemaSubDialogActorData* actorData = actorDataRow.GetRow())
		{
			if (UClass* uClass = GResource()->LoadSync<UClass>(actorData->actorPath, true))
			{
				if (const UGsGlobalConstant* data = GData()->GetGlobalData())
				{
					_actor = GsSpawn::BPClass(GetWorld(), uClass, data->_subActorSpawnPosOffset, FRotator::ZeroRotator);

				}
			}
		}
	}
}

void UGsUITutorialSubDialog::DestroyActor()
{
	if (nullptr != _actor)
	{
		GetWorld()->DestroyActor(_actor);
		_actor = nullptr;
	}
}
