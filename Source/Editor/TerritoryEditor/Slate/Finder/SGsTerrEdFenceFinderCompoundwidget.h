
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SDockTab.h"
#include "SDockableTab.h"
#include "SDockTabStack.h"
#include "SlateApplication.h"
#include "SCompoundWidget.h"
#include "SComboBox.h"
#include "SCheckBox.h"
#include "Blutility/Input/GsTerritorySpawnActorInput.h"
#include "Editor/GsTerritoryNpcInputDetailPanel.h"
#include "Margin.h"
#include "SNumericEntryBox.h"
#include "SGsTerrEdFinderCompoundwidget.h"
#include "Slate/GsTerrEdSlateHelper.h"
#include "SGsTerrEdBaseFinderCompoundwidget.h"

class AGsTerritoryNpc;
class AGsTerritorySpot;

class SGsTerrEdFenceFinderCompoundwidget : public SGsTerrEdBaseFinderCompoundwidget
{
public:
	SLATE_BEGIN_ARGS(SGsTerrEdFenceFinderCompoundwidget){}
	SLATE_END_ARGS()

	//include condition value when find spot 
private:
	//unique
	FGsTerrEdFinderNumericEntryBox<int64> _uniqueIdNumericEntryBox;

	//spot type
	FGsTerrEdFinderComboBox _fenceTypeComboBox;

	//radius
	FGsTerrEdFinderRangeNumericEntryBox<float> _radiusRangeNumericEntryBox;

	//name
	TSharedPtr<SEditableTextBox> _nameEditableTextBox;
	TSharedPtr<SCheckBox> _nameTextCheckBox;
				
private:
	FString _name;
	bool _isCheckNameCondition;


public:
	void Construct(const FArguments& InArgs);	

	//layout 
private:	
	void OnFieldColumnResized(const float inWidth);
	void OnIncludeConditionColumnResized(const float inWidth);
	float GetFieldColumnSize() const {return _fieldColumnSize;}
	float GetIncludeConditionColumnSize() const { return _conditionColumnSize; }

private:		
	TSharedPtr<SWidget> CreateUniqueIdWidget();
	TSharedPtr<SWidget> CreateFenceTypeComboBoxWidget();	
	TSharedPtr<SWidget> CreateRadiusNumericEntryBoxWidget();
	TSharedPtr<SWidget> CreateNameWidget();	
	TSharedPtr<SWidget> CreateFenceListViewWidget();


	//unique id
private:
	TOptional<int64> OnGetUniqueId() const;
	void OnSetUniqueId(int64 inValue, ETextCommit::Type inType);
	ECheckBoxState GetUniqueIdConditionCheckState() const;
	void OnUniqueIdConditionCheckChanged(ECheckBoxState inState);

	//fence type 
private:	
	TArray<TSharedPtr<FString>> GetFenceTypeList();
	void OnFenceTypeSelectionChanged(TSharedPtr<FString> InItem, ESelectInfo::Type InSeletionInfo);
	FText GetSelectedFenceTypeText() const;
	ECheckBoxState GetFenceTypeConditionCheckState() const;
	void OnFenceTypeConditionCheckChanged(ECheckBoxState inState);

	//radius
private:	
	TOptional<float> OnGetMinRadius() const;
	void OnSetMinRadius(float inValue, ETextCommit::Type inType);
	TOptional<float> OnGetMaxRadius() const;
	void OnSetMaxRadius(float inValue, ETextCommit::Type inType);
	ECheckBoxState GetRadiusConditionCheckState() const;
	void OnRadiusConditionCheckChanged(ECheckBoxState inState);

	//name
private:
	void OnNameTextChanged(const FText& inText);
	void OnNameTextCommit(const FText&, ETextCommit::Type);
	ECheckBoxState GetNameConditionCheckState() const;
	void OnNameConditionCheckChanged(ECheckBoxState inState);

	//find fence
private:
	void InitFenceActorList();
	TSharedRef<ITableRow> OnGenerateFenceActorWidget(TSharedPtr<FString> inItem, const TSharedRef<STableViewBase>& inOwnerTable);
	void OnDoubleClickFenceActor(TSharedPtr<FString> InNewSelection);
	void OnSelectionChangeFenceActor(TSharedPtr<FString> InNewSelection, ESelectInfo::Type InSelectInfo);
	int32 GetFenceId(const FString& inFenceId);
	TSharedRef<SWidget> OnGenerateComboWidget(TSharedPtr<FString> inSelect);

private:
	ECheckBoxState GetSelectAllActorState() const;
	void  OnSelectAllActorChanged(ECheckBoxState inState);
	void SetSelectAllActor(bool inSelectAll);
	ECheckBoxState GetOnlySelectActorSpawnCheckState() const;
	void  OnOnlySelectActorSpawnChanged(ECheckBoxState inState) const;

private:
	FReply OnFindFence();
	FReply OnDelectAll();
	bool CanFindFence();

public:
	bool IsValidUniqueId(class AGsTerritoryFenceBase* inFence);
	bool IsValidFenceType(class AGsTerritoryFenceBase* inFence);
	bool IsValidRadius(class AGsTerritoryFenceBase* inFence);
	bool IsValidName(class AGsTerritoryFenceBase* inFence);

};
