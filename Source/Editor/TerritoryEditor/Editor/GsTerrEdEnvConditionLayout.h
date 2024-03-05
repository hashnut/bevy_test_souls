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

class TERRITORYEDITOR_API FGsTerrEdEnvConditionLayout : public IPropertyTypeCustomization
{
public:
	FGsTerrEdSearchableComboBox _startQuestComboBox;
	FGsTerrEdSearchableComboBox _endQuestComboBox;
	
public:		
	FString _startQuestId;	
	FString _endQuestId;

	//Searchable box for npci id
public:
	TArray<TSharedPtr<FString>> _questIdList;	

public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

public:
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils);
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils);


private:
	TSharedRef<SWidget> OnGenerateQuestIDWidget(TSharedPtr<FString> inSelect);
	void InvalidQuestIDInfos();
	void InitQuestID(FGsTerrEdSearchableComboBox& inComboBox, FString& inQuestId);

private:
	void ChangedStartQuestID(FGsTerrEdSearchableComboBox& inComboBox, FString& inQuestId, TSharedPtr<FString> inNewSelection, ESelectInfo::Type inSelectInfo);	
	void CreateQuestIDSearchComboBox(FGsTerrEdSearchableComboBox& inComboBox
	, FString& inQuestId
	, IDetailChildrenBuilder& inChildBuilder
	, TFunction<void(TSharedPtr<FString>, ESelectInfo::Type)> inChangeEvent
	, TFunction<FText(void)> inGetEvent);
	FText GetSelectedQuestIDText(FString& inText) const;
	//int64 GetSelectedEndQuestID() const;
	/*int64 GetSelectedStartQuestID() const;
	FText GetSelectedEndQuestIDText() const;
	int64 GetSelectedEndQuestID() const;*/
};
