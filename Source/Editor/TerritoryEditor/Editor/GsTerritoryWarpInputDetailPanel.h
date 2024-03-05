// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//ue4
#include "IDetailCustomization.h"
#include "IPropertyTypeCustomization.h"
#include "Editor/EditorWidgets/Public/SSearchableComboBox.h"
#include "Widgets/Input/SNumericEntryBox.h"

/**
 * 
 */
class TERRITORYEDITOR_API FGsTerritoryWarpInputDetailPanel : public IPropertyTypeCustomization
{
	//dest map 
public:
	TSharedPtr<SSearchableComboBox> _destMapIDSearchableComboBox;

private:
	TSharedPtr<IPropertyHandle> _destMapIDHandle;
	TArray<TSharedPtr<FString>> _destMapIDList;	
	FString _selectedDestMapID;
	TSharedPtr<FString> _initiallySelectedDestMapID;

	//spot index
public:
	TSharedPtr<SSearchableComboBox> _spotIndexSearchableComboBox;	

private:	
	TSharedPtr<IPropertyHandle> _spotIndexHandle;
	TArray<TSharedPtr<FString>> _spotIndexList;
	FString _selectedSpotIndex;	
	TSharedPtr<FString> _initiallySelectedSpotIndex;	

public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

public:
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils);
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils);

	//dest map id
private:
	void InvalidDestMapIDList();
	void InitDestMapIDSearchableComboBox();
	void CreateDestMapIDSearchableComboBox(IDetailChildrenBuilder& inChildBuilder);
	TSharedRef<SWidget> OnGenerateDestMapIDWidget(TSharedPtr<FString> inSelect);
	void OnChangeDestMapID(TSharedPtr<FString> inNewSelection, ESelectInfo::Type inSelectInfo);
	FText GetSelectedDestMapIDText() const;

	//spot index
private:
	void InvalidSpotIndexList();
	void InitSpotIndexSearchableComboBox();
	void CreateSpotIndexSearchableComboBox(IDetailChildrenBuilder& inChildBuilder);
	TSharedRef<SWidget> OnGenerateSpotIndexWidget(TSharedPtr<FString> inSelect);
	void OnChangeSpotIndex(TSharedPtr<FString> inNewSelection, ESelectInfo::Type inSelectInfo);
	FText GetSelectedSpotIndexText() const;
	//TSharedRef<SWidget> CreateContextMenu();
};
