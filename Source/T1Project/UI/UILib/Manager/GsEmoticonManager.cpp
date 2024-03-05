// Fill out your copyright notice in the Description page of Project Settings.


#include "GsEmoticonManager.h"
#include "NpcDialog/GsSchemaEmoticonResData.h"
#include "DataSchema/GsSchemaEnums.h"
#include "Management/ScopeGlobal/GsPathManager.h"
#include "UI/UIContent/HUD/GsUIHUDEmoticon.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "ActorEx/GsCharacterBase.h"
#include "Data/GsDataContainManager.h"
#include "GsWidgetPoolManager.h"
#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"
#include "../Container/GsWidgetPool.h"

void UGsEmoticonManager::Initialize()
{
	_onReleaseCompleteFunc = [](UUserWidget* InWidget)
	{
		UGsUIHUDEmoticon* widget = Cast<UGsUIHUDEmoticon>(InWidget);
		if (widget)
		{
			widget->OnReleaseComplete();
		}
	};
}

void UGsEmoticonManager::Finalize()
{

}

UGsUIHUDEmoticon* UGsEmoticonManager::AddWidget(EGsEmoticonType InType, UGsGameObjectBase* InTarget, FVector InDir)
{
	if (nullptr == InTarget)
		return nullptr;
	
	const FGsSchemaEmoticonResData* data = GetEmoticonResData(InType);
	if (nullptr == data)
	{
		return nullptr;
	}

	UClass* widgetClass = nullptr;
	if (UGsPathManager* pathMgr = GPath())
	{
		widgetClass = pathMgr->GetClass(EGsPathType::UI, "HUDEmoticon");
	}

	if (nullptr == widgetClass)
	{
		return nullptr;
	}

	UGsUIHUDEmoticon* widget = Claim(widgetClass);
	if (widget)
	{
		widget->AddToViewport();

		if (AGsCharacterBase* characterBase = InTarget->GetCharacter())
		{
			if (class USkeletalMeshComponent* mesh = characterBase->GetMesh())
			{
				FVector location = (nullptr != InTarget) ? mesh->GetSocketLocation(TEXT("OverHeadSocket")) : FVector::ZeroVector;				
				FVector vec = location + GData()->GetGlobalData()->_interactionButtonPosOffset;

				widget->PlayEmoticon(data, vec);
			}
		}
	}

	return widget;
}

void UGsEmoticonManager::ReleaseWidget(UGsUIHUDEmoticon* InWidget)
{
	if (InWidget)
	{
		if (_poolManager.IsValid())
		{
			InWidget->RemoveFromParent();
			_poolManager->ReleaseWidget(InWidget->GetClass(), InWidget);
		}
	}
}

UGsUIHUDEmoticon* UGsEmoticonManager::Claim(UClass* InClass)
{
	if (false == _poolManager.IsValid())
	{
		_poolManager = UGsBlueprintFunctionLibraryUI::GetWidgetPoolManager();
		if (false == _poolManager.IsValid())
		{
			return nullptr;
		}
	}

	UGsWidgetPool* pool = _poolManager->FindPool(InClass);
	if (nullptr == pool)
	{
		pool = _poolManager->CreatePool(InClass, _onReleaseCompleteFunc);
		if (nullptr == pool)
		{
			return nullptr;
		}
	}

	return pool->Claim<UGsUIHUDEmoticon>();
}

const FGsSchemaEmoticonResData* UGsEmoticonManager::GetEmoticonResData(EGsEmoticonType InRowKey) const
{
	const UGsTableEmoticonResData* table = Cast<UGsTableEmoticonResData>(FGsSchemaEmoticonResData::GetStaticTable());
	if (nullptr == table)
	{
		return nullptr;
	}

	const FGsSchemaEmoticonResData* schemaEmoticonResData = nullptr;
	if (false == table->FindRowById(InRowKey, schemaEmoticonResData))
	{
		return nullptr;
	}

	return schemaEmoticonResData;
}