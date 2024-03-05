// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

//~=============================================================================
// SkillDataExport
//~=============================================================================

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Factories/Factory.h"
#include "SkillEditorDataAsset.h"
#include "SkillEditorDataAssetContainerFactoryNew.generated.h"

UCLASS(hidecategories = Object, collapsecategories, MinimalAPI)
class USkillEditorDataAssetContainerFactoryNew : public UFactory
{
	GENERATED_UCLASS_BODY()

	/** An initial texture to place in the newly created material */
	/*UPROPERTY()
	class USkillEditorDataAsset* SkillEditorDataAsset;*/	

	//~ Begin UFactory Interface
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	//~ Begin UFactory Interface
};
