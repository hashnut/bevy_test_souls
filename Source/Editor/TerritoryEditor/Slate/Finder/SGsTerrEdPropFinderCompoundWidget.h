
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

class AGsTerritoryProp;

class SGsTerrEdPropFinderCompoundWidget : public SGsTerrEdBaseFinderCompoundwidget
{
public:
	SLATE_BEGIN_ARGS(SGsTerrEdPropFinderCompoundWidget){}	
	SLATE_END_ARGS()

	//prop id range
private:
	FGsTerrEdRangeSearchableComboBox _propIdRangeSearchableComboBox;
	FGsTerrEdSearchableComboBox _propIdSearchableComboBox;
	FGsTerrEdFinderNumericEntryBox<int64> _uniqueIdNumericEntryBox;	
	FGsTerrEdFinderRangeNumericEntryBox<int32> _interactionTimeRangeNumericEntryBox;
	FGsTerrEdFinderRangeNumericEntryBox<int32> _maxSpawnNumRangeNumericEntryBox;
	FGsTerrEdFinderRangeNumericEntryBox<int32> _interactionRangeRangeNumericEntryBox;
	FGsTerrEdFinderRangeNumericEntryBox<int32> _initiSpawnDelayRangeNumericEntryBox;	

	//Can random spawn
private:
	TSharedPtr<SCheckBox> _randomSpawnCheckBox;
	TSharedPtr<SCheckBox> _randomSpawnConditionCheckBox;

private:
	int32 _selectMaxPropId;
	int32 _selectMinPropId;
	int32 _selectPropId;

	//npc id
private:	
	TArray<TSharedPtr<FString>> _propIdList;	

private:
	bool _isCheckRandomSpawn = false;
	bool _isCheckRandomSpawnCondition = false;				

public:
	void Construct(const FArguments& InArgs);

	//layout 
private:	
	void OnFieldColumnResized(const float inWidth);
	void OnIncludeConditionColumnResized(const float inWidth);
	float GetFieldColumnSize() const {return _fieldColumnSize;}
	float GetIncludeConditionColumnSize() const { return _conditionColumnSize; }

	TSharedPtr<SWidget> CreatePropIdRangeWidget();
	TSharedPtr<SWidget> CreatePropIdWidget();
	TSharedPtr<SWidget> CreateUniqueIdWidget();
	TSharedPtr<SWidget> CreateInteractionTimeWidget();
	TSharedPtr<SWidget> CreateInteractionRangeWidget();
	TSharedPtr<SWidget> CreateMaxSpawnNumWidget();
	TSharedPtr<SWidget> CreateInitSpawnDelayWidget();
	TSharedPtr<SWidget> CreateRandomSpawnWidget();
	TSharedPtr<SWidget> CreatePropActorFindWidget();


private:
	FReply OnFindProp();
	FReply OnDelectAll();
	bool CanFindProp();

private:
	void InitPropIdList();
	TSharedRef<SWidget> OnGeneratePropIdWidget(TSharedPtr<FString> inSelect);

	//prop id range
private:
	void InitPropIdRange();
	FText GetSelectedMinPropIdText() const;
	FText GetSelectedMaxPropIdText() const;
	void OnMinPropIdSelectionChanged(TSharedPtr<FString> InItem, ESelectInfo::Type InSeletionInfo);
	void OnMaxPropIdSelectionChanged(TSharedPtr<FString> InItem, ESelectInfo::Type InSeletionInfo);
	ECheckBoxState GetPropIdRangeConditionCheckState() const;
	void OnPropIdRangeConditionCheckChanged(ECheckBoxState inState);
	bool IsValidPropIdRange(AGsTerritoryProp* inNpc);

	//npc id
private:	
	void InitPropId();
	FText GetSelectedPropIdText() const;
	void OnPropIdSelectionChanged(TSharedPtr<FString> InItem, ESelectInfo::Type InSeletionInfo);	
	ECheckBoxState GetPropIdConditionCheckState() const;
	void OnPropIdConditionCheckChanged(ECheckBoxState inState);
	bool IsValidPropId(AGsTerritoryProp* inNpc);

	//unique id
private:
	ECheckBoxState GetUniqueIdConditionCheckState() const;
	void OnUniqueIdConditionCheckChanged(ECheckBoxState inState);
	bool IsValidUniqueId(AGsTerritoryProp* inNpc);
	TOptional<int64> OnGetUniqueId() const;
	void OnSetUniqueId(int64 inValue, ETextCommit::Type inType);

	//interaction time
private:
	ECheckBoxState GetInteractionTimeConditionCheckState() const;
	void OnInteractionTimeConditionCheckChanged(ECheckBoxState inState);
	TOptional<int32> OnGetMinInteractionTime() const;
	void OnSetMinInteractionTime(int32 inValue, ETextCommit::Type inType);
	TOptional<int32> OnGetMaxInteractionTime() const;
	void OnSetMaxInteractionTime(int32 inValue, ETextCommit::Type inType);
	bool IsValidInteractionTime(AGsTerritoryProp* inNpc);

	//interaction range 
private:
	ECheckBoxState GetInteractionRangeConditionCheckState() const;
	void OnInteractionRangeConditionCheckChanged(ECheckBoxState inState);
	TOptional<int32> OnGetMinInteractionRange() const;
	void OnSetMinInteractionRange(int32 inValue, ETextCommit::Type inType);
	TOptional<int32> OnGetMaxInteractionRange() const;
	void OnSetMaxInteractionRange(int32 inValue, ETextCommit::Type inType);
	bool IsValidInteractionRange(AGsTerritoryProp* inNpc);

	//max spawn num 
private:
	ECheckBoxState GetMaxSpawnNumConditionCheckState() const;
	void OnMaxSpawnNumConditionCheckChanged(ECheckBoxState inState);
	TOptional<int32> OnGetMinMaxSpawnNum() const;
	void OnSetMinMaxSpawnNum(int32 inValue, ETextCommit::Type inType);
	TOptional<int32> OnGetMaxMaxSpawnNum() const;
	void OnSetMaxMaxSpawnNum(int32 inValue, ETextCommit::Type inType);
	bool IsValidMaxSpawnNum(AGsTerritoryProp* inNpc);

	//init spawn delay
private:
	ECheckBoxState GetInitSpawnDelayConditionCheckState() const;
	void OnInitSpawnDelayConditionCheckChanged(ECheckBoxState inState);	
	TOptional<int32> OnGetMinInitSpawnDelay() const;
	void OnSetMinInitSpawnDelay(int32 inValue, ETextCommit::Type inType);
	TOptional<int32> OnGetMaxInitSpawnDelay() const;
	void OnSetMaxInitSpawnDelay(int32 inValue, ETextCommit::Type inType);
	bool IsValidInitSpawnDelay(AGsTerritoryProp* inNpc);

	//random spawn
private:
	ECheckBoxState GetRandomSpawnCheckState() const;
	void OnRandomSpawnCheckChanged(ECheckBoxState inState);
	ECheckBoxState GetRandomSpawnConditionCheckState() const;
	void OnRandomSpawnConditionCheckChanged(ECheckBoxState inState);
	bool IsValidRandomSpawn(AGsTerritoryProp* inNpc);

private:
	ECheckBoxState GetOnlySelectActorSpawnCheckState() const;
	void OnOnlySelectActorSpawnChanged(ECheckBoxState inState) const;

private:
	ECheckBoxState GetSelectAllActorState() const;
	void  OnSelectAllActorChanged(ECheckBoxState inState);
	void SetSelectAllActor(bool inSelect);

	//find prop
private:
	int32 GetPropId(const FString& inPropId);
	void InitPropActorList();	
	TSharedRef<ITableRow> OnGeneratePropActorWidget(TSharedPtr<FString> inItem, const TSharedRef<STableViewBase>& inOwnerTable);
	void OnDoubleClickPropActor(TSharedPtr<FString> InNewSelection);
	void OnSelectionChangePropActor(TSharedPtr<FString> InNewSelection, ESelectInfo::Type InSelectInfo);	
};
