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

/**
 * 
 */

class TERRITORYEDITOR_API FGsTerritorySpawnLayoutBase : public IPropertyTypeCustomization
{
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
	TSharedPtr<IPropertyHandle> _conditionQuestIDHandle;
	FDelegateHandle _conditionQuestIdRowNameHandle;

	//Searchable box for npci id
public:
	TSharedPtr<SSearchableComboBox> _conditionQuestIdSearchableComboBox;
	TArray<TSharedPtr<FString>> _conditionQuestIdList;
	FString _selectedConditionQuestId;
	TSharedPtr<FString> _conditionQuestIdText;

public:
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils);
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils);

	//Serachable combo box for npc id selection
public:	
	TSharedRef<SWidget> OnGenerateTIdWidget(TSharedPtr<FString> inSelect);
	void OnChangedTId(TSharedPtr<FString> inNewSelection, ESelectInfo::Type inSelectInfo);
	virtual void InvalidTIdInfos(){}
	virtual void InitTId();
	void CreateTIdSearchComboBox(IDetailChildrenBuilder& inChildBuilder);
	FText GetSelectedTIdText() const;
	int32 GetSelectedTId() const;

public:
	TSharedRef<SWidget> OnGenerateConditionQuestIDWidget(TSharedPtr<FString> inSelect);
	void OnChangedConditionQuestID(TSharedPtr<FString> inNewSelection, ESelectInfo::Type inSelectInfo);
	virtual void InvalidConditionQuestIDInfos();
	virtual void InitConditionQuestID();
	void CreateConditionQuestIDSearchComboBox(IDetailChildrenBuilder& inChildBuilder);
	FText GetSelectedConditionQuestIDText() const;
	int32 GetSelectedConditionQuestID() const;
};
