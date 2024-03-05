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

 //Prop을 넣을 수도 있으므로 SpanwLayout과 상속 구조를 만들테스트 한다. 
class TERRITORYEDITOR_API FGsTerritoryNpcInputDetailPanel : public FGsTerritorySpawnLayoutBase
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

protected:
	virtual void InvalidTIdInfos() override;
};
