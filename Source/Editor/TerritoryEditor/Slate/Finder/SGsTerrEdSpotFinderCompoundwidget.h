
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

class SGsTerrEdSpotFinderCompoundWidget : public SGsTerrEdBaseFinderCompoundwidget
{
public:
	SLATE_BEGIN_ARGS(SGsTerrEdSpotFinderCompoundWidget){}	
	SLATE_END_ARGS()

	//include condition value when find spot 
private:
	FGsTerrEdFinderComboBox _spotTypeComboBox;
	FGsTerrEdFinderRangeNumericEntryBox<float> _spotRadiusRangeNumericEntryBox;		
		
public:
	void Construct(const FArguments& InArgs);

	//layout 
private:	
	void OnFieldColumnResized(const float inWidth);
	void OnIncludeConditionColumnResized(const float inWidth);
	float GetFieldColumnSize() const {return _fieldColumnSize;}
	float GetIncludeConditionColumnSize() const { return _conditionColumnSize; }

private:		
	TSharedPtr<SWidget> CreateSpotListViewWidget();


private:
	FReply OnFindSpot();
	FReply OnDelectAll();
	bool CanFindSpot();

private:
	TSharedRef<SWidget> OnGenerateNpcIdWidget(TSharedPtr<FString> inSelect);

	//spot type 
private:	
	TSharedPtr<SWidget> CreateSpotTypeComboBoxWidget();
	TArray<TSharedPtr<FString>> GetSpotTypeList();	
	void OnSpotTypeSelectionChanged(TSharedPtr<FString> InItem, ESelectInfo::Type InSeletionInfo);	
	FText GetSelectedSpotTypeText() const;
	ECheckBoxState GetSpotTypeConditionCheckState() const;
	void OnSpotTypeConditionCheckChanged(ECheckBoxState inState);
	bool IsValidSpotType(AGsTerritorySpot* inSpot);

	//radius
private:	
	TSharedPtr<SWidget> CreateRadiusNumericEntryBoxWidget();
	TOptional<float> OnGetMinSpotRadius() const;
	void OnSetMinSpotRadius(float inValue, ETextCommit::Type inType);
	TOptional<float> OnGetMaxSpotRadius() const;
	void OnSetMaxSpotRadius(float inValue, ETextCommit::Type inType);
	ECheckBoxState GetSpotRadiusConditionCheckState() const;
	void OnSpotRadiusConditionCheckChanged(ECheckBoxState inState);
	bool IsValidSpotRadius(AGsTerritorySpot* inSpot);

	//select only 
private:
	ECheckBoxState GetOnlySelectActorSpawnCheckState() const;
	void  OnOnlySelectActorSpawnChanged(ECheckBoxState inState) const;

private:
	ECheckBoxState GetSelectAllActorState() const;
	void  OnSelectAllActorChanged(ECheckBoxState inState);
	void SetSelectAllActor(bool inSelectAll);

	//find npc
private:	
	void InitSpotActorList();		
	TSharedRef<ITableRow> OnGenerateSpotActorWidget(TSharedPtr<FString> inItem, const TSharedRef<STableViewBase>& inOwnerTable);
	void OnDoubleClickSpotActor(TSharedPtr<FString> InNewSelection);
	void OnSelectionChangeSpotActor(TSharedPtr<FString> InNewSelection, ESelectInfo::Type InSelectInfo);
};
