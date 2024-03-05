// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"
#include "DetailWidgetRow.h"


/**
 * 
 */

class TERRITORYEDITOR_API FGsTerrEdLIneDataLayout : public IPropertyTypeCustomization
{
public:
	TSharedPtr<IPropertyHandle> _targetIDHandle;
	FDelegateHandle _getTIdRowNameHandle;

	//Searchable box for npci id
public:
	TSharedPtr<class SSearchableComboBox> _tIdSearchableComboBox;
	TArray<TSharedPtr<FString>> _tIdList;
	FString _selectedTId;
	TSharedPtr<FString> _tIdText;

public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();	

public:
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils);
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils);

protected:
	TSharedRef<SWidget> OnGenerateTIdWidget(TSharedPtr<FString> inSelect);
	void OnChangedTId(TSharedPtr<FString> inNewSelection, ESelectInfo::Type inSelectInfo);
	virtual void InvalidTIdInfos();
	virtual void InitTId();
	void CreateTIdSearchComboBox(IDetailChildrenBuilder& inChildBuilder);
	FText GetSelectedTIdText() const;
	int64 GetSelectedTId() const;
};
