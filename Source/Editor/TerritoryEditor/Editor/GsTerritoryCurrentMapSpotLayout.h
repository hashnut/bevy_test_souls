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
class TERRITORYEDITOR_API FGsTerritoryCurrentMapSpotLayout : public IPropertyTypeCustomization
{
private:
	int32 _mapId;
	int64 _spotId;

	//spot index
public:
	TSharedPtr<SSearchableComboBox> _spotIdSearchableComboBox;	

private:	
	TSharedPtr<IPropertyHandle> _spotIdHandle;
	TArray<TSharedPtr<FString>> _spotIdTextList;
	TArray<int64> _spotIdList;
	TSharedPtr<FString> _selectedSpotIdText;
	TSharedPtr<FString> _initSelectedSpotIdText;	

public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

public:
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils);
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils);

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
