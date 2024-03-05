// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//ue4
#include "IDetailCustomization.h"
#include "IPropertyTypeCustomization.h"
#include "Editor/EditorWidgets/Public/SSearchableComboBox.h"
#include "Widgets/Input/SNumericEntryBox.h"

//territory editor
#include "DetailWidgetRow.h"
#include "GsTerritorySpawnLayoutBase.h"

/**
 * 
 */

class TERRITORYEDITOR_API FGsTerrEdShapeDataLayout : public FGsTerritorySpawnLayoutBase
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();	

protected:
	virtual void InvalidTIdInfos() override;
};
