// Fill out your copyright notice in the Description page of Project Settings.
#include "GsCustomizeComponent.h"

#include "Gameobject/Data/GsGameObjectDataLocalPlayer.h"

#include "GameObject/Customize/Data/GsCustomizeData.h"
#include "GameObject/Customize/GsCustomizeFunc.h"
#include "Util/GsTableUtil.h"
#include "Item/GsItemManager.h"
#include "DataSchema/GameObject/GsSchemaPartsShapeData.h"
#include "Dataschema/Customize/GsSchemaCustomizeCinematic.h"
#include "ActorEx/GsCharacterSequencePlayer.h"

#include "DataSchema/User/GsSchemaUserData.h"
//#include "T1Project/GameMode/GsGameModeBase.h"

// Sets default values for this component's properties
UGsCustomizeComponent::UGsCustomizeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
#if (WITH_EDITOR)
	PrimaryComponentTick.bCanEverTick = true;
#endif
	// ...	
}

void UGsCustomizeComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void UGsCustomizeComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (_bLateApply)
	{
		_customizeHandler.SetCustomizeBodyData(_bodydata);
		_bLateApply = false;
	}

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UGsCustomizeComponent::SetCustomize(UGsGameObjectLocalPlayer* Local, bool In_useWeapon)
{	
	FGsGameObjectDataLocalPlayer* LocalData = (Local->GetCastData<FGsGameObjectDataLocalPlayer>());
	if (LocalData)
	{
		_customizeHandler.SetActor(Cast<AGsCharacterSequencePlayer>(GetOwner()));
		_customizeHandler.InitializeBoneTargetName();
		_customizeHandler.SetGenderType(LocalData->GetCreatureGenderType());		
		_customizeHandler.InitializeScaleDisplacementAllBone(LocalData->GetCreatureGenderType());
		_bodydata = *LocalData->GetCustomizeBodyData();
		if (const FGsSchemaCustomizeCinematic* findOriginLevelSet = UGsTableUtil::FindRow<FGsSchemaCustomizeCinematic>(FName("sequence")))
		{
			if (auto FindItem = findOriginLevelSet->cinematicApply.Find(LocalData->GetCreatureGenderType()))
			{
				_bodydata.mHeight = FindItem->Height;
				_bodydata.mHeadSize = FindItem->Head;
				_bodydata.mChestSize = FindItem->Chest;
				_bodydata.mWaistSize = FindItem->Waist;
				_bodydata.mPelvisSize = FindItem->Pelvis;
				_bodydata.mLegSize = FindItem->Legs;
			}
		}

		_customizeHandler.SetCustomizeData((*LocalData->GetCustomizeFaceData()), _bodydata);
		_customizeHandler.SetCostume(LocalData->GetUserLookInfo()->mTransformCostumeId, LocalData->GetUserLookInfo()->mCostumeId, LocalData->GetUserLookInfo()->mViewHelmet, false, true, false, true, false);
		_bLateApply = true;
		if (In_useWeapon)
		{
			FGsCustomizeFunc::AttachWeapon(&_customizeHandler, LocalData->GetUserLookInfo(), LocalData->GetTableId(), false);
		}
		
	}
}

void UGsCustomizeComponent::SetCustomizeBoneScaleToSingleMode(UGsGameObjectLocalPlayer* Local, AGsCharacterSequencePlayer* actor)
{
	_customizeHandler.SetActor(actor);
	_customizeHandler.InitializeBoneTargetName();

	FGsGameObjectDataLocalPlayer* LocalData = (Local->GetCastData<FGsGameObjectDataLocalPlayer>());
	if (LocalData)
	{
		_customizeHandler.SetGenderType(LocalData->GetCreatureGenderType());

		_bodydata = *LocalData->GetCustomizeBodyData();
		if (const FGsSchemaCustomizeCinematic * findOriginLevelSet = UGsTableUtil::FindRow<FGsSchemaCustomizeCinematic>(FName("sequence")))
		{
			if (auto FindItem = findOriginLevelSet->cinematicApply.Find(LocalData->GetCreatureGenderType()))
			{
				_bodydata.mHeight = FindItem->Height;
				_bodydata.mHeadSize = FindItem->Head;
				_bodydata.mChestSize = FindItem->Chest;
				_bodydata.mWaistSize = FindItem->Waist;
				_bodydata.mPelvisSize = FindItem->Pelvis;
				_bodydata.mLegSize = FindItem->Legs;
			}
		}
		_customizeHandler.SetCustomizeBodyData(_bodydata);
		_bLateApply = true;
	}
}

