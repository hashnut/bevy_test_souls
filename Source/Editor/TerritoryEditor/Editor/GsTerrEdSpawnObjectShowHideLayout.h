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
#include "Editor/TerritoryEditor/Slate/GsTerrEdSlateHelper.h"
/**
 * 
 */

class TERRITORYEDITOR_API FGsTerrEdSpawnObjectShowHideLayout : public IPropertyTypeCustomization
{
public:
	FGsTerrEdSearchableComboBox _startQuestConditionComboBox;
	FGsTerrEdSearchableComboBox _endQuestConditionComboBox;

public:
	TSharedPtr<IPropertyHandle> _targetIDHandle;
	FDelegateHandle _getTIdRowNameHandle;

	//Searchable box for npci id
public:
	TSharedPtr<SSearchableComboBox> _tIdSearchableComboBox;
	TArray<TSharedPtr<FString>> _tIdList;
	FString _selectedTId;
	TSharedPtr<FString> _tIdText;

public:
	FDelegateHandle _conditionQuestIdRowNameHandle;

	//Searchable box for npci id
public:
	TArray<TSharedPtr<FString>> _conditionQuestIdList;	

public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

public:
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils);
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils);

public:
	TSharedRef<SWidget> OnGenerateConditionQuestIDWidget(TSharedPtr<FString> inSelect);
	virtual void InvalidConditionQuestIDInfos();

public:	
	void OnChangedStartConditionQuestID(TSharedPtr<FString> inNewSelection, ESelectInfo::Type inSelectInfo);	
	virtual void InitStartConditionQuestID();
	void CreateStartConditionQuestIDSearchComboBox(IDetailChildrenBuilder& inChildBuilder);
	FText GetSelectedStartConditionQuestIDText() const;
	int32 GetSelectedStartConditionQuestID() const;

public:
	void OnChangedEndConditionQuestID(TSharedPtr<FString> inNewSelection, ESelectInfo::Type inSelectInfo);	
	virtual void InitEndConditionQuestID();
	void CreateEndConditionQuestIDSearchComboBox(IDetailChildrenBuilder& inChildBuilder);
	FText GetSelectedEndConditionQuestIDText() const;
	int32 GetSelectedEndConditionQuestID() const;
};
