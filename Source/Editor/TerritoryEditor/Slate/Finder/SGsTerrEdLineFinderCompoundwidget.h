
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGsTerrEdFinderCompoundwidget.h"
#include "Widgets/SCompoundWidget.h"
#include "Slate/Finder/SGsTerrEdFinderCompoundwidget.h"
#include "Slate/GsTerrEdSlateHelper.h"
#include "SGsTerrEdBaseFinderCompoundwidget.h"


class AGsTerritoryNpc;
class AGsTerritorySpot;

class SGsTerrEdLineFinderCompoundWidget : public SGsTerrEdBaseFinderCompoundwidget
{
public:
	SLATE_BEGIN_ARGS(SGsTerrEdLineFinderCompoundWidget){}
	SLATE_END_ARGS()

	//include condition value when find spot 
private:
	FGsTerrEdFinderComboBox _lineTypeComboBox;	
	FGsTerrEdFinderComboBox _lineShapeComboBox;
	FGsTerrEdFinderRangeNumericEntryBox<float> _repeatDelayRangeNumericEntryBox;		

public:
	void Construct(const FArguments& InArgs);

	//layout 
private:	
	void OnFieldColumnResized(const float inWidth);
	void OnIncludeConditionColumnResized(const float inWidth);
	float GetFieldColumnSize() const {return _fieldColumnSize;}
	float GetIncludeConditionColumnSize() const { return _conditionColumnSize; }

private:		
	TSharedPtr<SWidget> CreateLineListViewWidget();


private:
	FReply OnFindLine();
	FReply OnDelectAll();
	bool CanFindLine();

private:
	TSharedRef<SWidget> OnGenerateNpcIdWidget(TSharedPtr<FString> inSelect);

	//line type 
private:	
	TSharedPtr<SWidget> CreateLineTypeComboBoxWidget();
	TArray<TSharedPtr<FString>> GetLineTypeList();
	void OnLineTypeSelectionChanged(TSharedPtr<FString> InItem, ESelectInfo::Type InSeletionInfo);
	FText GetSelectedLineTypeText() const;
	ECheckBoxState GetLineTypeConditionCheckState() const;
	void OnLineTypeConditionCheckChanged(ECheckBoxState inState);
	bool IsValidLineType(class AGsTerritoryLine* inLine);

	//line shape
private:
	TSharedPtr<SWidget> CreateLineShapeComboBoxWidget();	
	TArray<TSharedPtr<FString>> GetLineShapeList();
	void OnLineShapeSelectionChanged(TSharedPtr<FString> InItem, ESelectInfo::Type InSeletionInfo);
	FText GetSelectedLineShapeText() const;
	ECheckBoxState GetLineShapeConditionCheckState() const;
	void OnLineShapeConditionCheckChanged(ECheckBoxState inState);
	bool IsValidLineShape(class AGsTerritoryLine* inLine);

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
	void InitLineActorList();
	TSharedRef<ITableRow> OnGenerateLineActorWidget(TSharedPtr<FString> inItem, const TSharedRef<STableViewBase>& inOwnerTable);
	void OnDoubleClickLineActor(TSharedPtr<FString> InNewSelection);
	void OnSelectionChangeLineActor(TSharedPtr<FString> InNewSelection, ESelectInfo::Type InSelectInfo);
};
