
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

class SGsTerrEdNpcFinderCompoundWidget : public SGsTerrEdBaseFinderCompoundwidget
{
public:
	SLATE_BEGIN_ARGS(SGsTerrEdNpcFinderCompoundWidget){}	
	SLATE_END_ARGS()

	//npc id
private:	
	FGsTerrEdRangeSearchableComboBox _npcIdRangeSearchableComboBox;
	FGsTerrEdSearchableComboBox _npcIdSearchableComboBox;
	FGsTerrEdFinderNumericEntryBox<int64> _uniqueIdNumericEntryBox;
	FGsTerrEdFinderRangeNumericEntryBox<int32> _spawnNumRangeNumericEntryBox;	
	FGsTerrEdFinderRangeNumericEntryBox<int32> _initSpawnDelayRangeNumericEntryBox;
	FGsTerrEdFinderRangeNumericEntryBox<int32> _respawnIntervalRangeNumericEntryBox;
	FGsTerrEdFinderNumericEntryBox<int32> _spawnLevelNumericEntryBox;

	//Can random spawn
private:
	TSharedPtr<SCheckBox> _randomSpawnCheckBox;
	TSharedPtr<SCheckBox> _randomSpawnConditionCheckBox;

private:
	bool _isCheckRandomSpawn = false;
	bool _isCheckRandomSpawnCondition = false;

private:
	int32 _selectNpcId;
	int32 _selectMaxNpcId;
	int32 _selectMinNpcId;


private:
	TArray<TSharedPtr<FString>> _npcIdList;	

	//Find npc
private:
	FGsTerrEdFinderActorListView _npcFinderActorListView;	
	TSharedPtr<FString> _selectedNpcActorStringPtr;
	FString _selectedNpcActorString;
	int32 _selectNpcActor;

public:
	void Construct(const FArguments& InArgs);

	//layout 
private:	
	void OnFieldColumnResized(const float inWidth);
	void OnIncludeConditionColumnResized(const float inWidth);
	float GetFieldColumnSize() const {return _fieldColumnSize;}
	float GetIncludeConditionColumnSize() const { return _conditionColumnSize; }

	//create respawn interval
	TSharedPtr<SWidget> CreateNpcIdRangeWidget();
	TSharedPtr<SWidget> CreateNpcIdWidget();
	TSharedPtr<SWidget> CreateUniqueIdWidget();
	TSharedPtr<SWidget> CreateMaxSpawnNumWidget();
	TSharedPtr<SWidget> CreateInitSpawnDelayWidget();
	TSharedPtr<SWidget> CreateRandomSpawnWidget();
	TSharedPtr<SWidget> CreateRespawnInterval();
	TSharedPtr<SWidget> CreateSpawnLevel();
	TSharedPtr<SWidget> CreateNpcActorFindWidget();

private:
	void InitNpcIdList();
	TSharedRef<SWidget> OnGenerateNpcIdWidget(TSharedPtr<FString> inSelect);

	//npc id range
private:
	void InitNpcIdRange();
	FText GetSelectedMinNpcIdText() const;
	FText GetSelectedMaxNpcIdText() const;
	void OnMinNpcIdSelectionChanged(TSharedPtr<FString> InItem, ESelectInfo::Type InSeletionInfo);
	void OnMaxNpcIdSelectionChanged(TSharedPtr<FString> InItem, ESelectInfo::Type InSeletionInfo);
	ECheckBoxState GetNpcIdRangeConditionCheckState() const;
	void OnNpcIdRangeConditionCheckChanged(ECheckBoxState inState);	

	//npc id
private:	
	void InitNpcId();
	FText GetSelectedNpcIdText() const;
	void OnNpcIdSelectionChanged(TSharedPtr<FString> InItem, ESelectInfo::Type InSeletionInfo);	
	ECheckBoxState GetNpcIdConditionCheckState() const;
	void OnNpcIdConditionCheckChanged(ECheckBoxState inState);	

	//unique id
private:
	ECheckBoxState GetUniqueIdConditionCheckState() const;
	void OnUniqueIdConditionCheckChanged(ECheckBoxState inState);	
	TOptional<int64> OnGetUniqueId() const;
	void OnSetUniqueId(int64 inValue, ETextCommit::Type inType);	

	//max spawn num 
private:
	ECheckBoxState GetMaxSpawnNumConditionCheckState() const;
	void OnMaxSpawnNumConditionCheckChanged(ECheckBoxState inState);
	TOptional<int32> OnGetMinMaxSpawnNum() const;
	void OnSetMinMaxSpawnNum(int32 inValue, ETextCommit::Type inType);
	TOptional<int32> OnGetMaxMaxSpawnNum() const;
	void OnSetMaxMaxSpawnNum(int32 inValue, ETextCommit::Type inType);	

	//init spawn delay
private:
	ECheckBoxState GetInitSpawnDelayConditionCheckState() const;
	void OnInitSpawnDelayConditionCheckChanged(ECheckBoxState inState);	
	TOptional<int32> OnGetMinInitSpawnDelay() const;
	void OnSetMinInitSpawnDelay(int32 inValue, ETextCommit::Type inType);
	TOptional<int32> OnGetMaxInitSpawnDelay() const;
	void OnSetMaxInitSpawnDelay(int32 inValue, ETextCommit::Type inType);	

	//random spawn
private:
	ECheckBoxState GetRandomSpawnCheckState() const;
	void OnRandomSpawnCheckChanged(ECheckBoxState inState);
	ECheckBoxState GetRandomSpawnConditionCheckState() const;
	void OnRandomSpawnConditionCheckChanged(ECheckBoxState inState);	

	//spawn level
private:
	TOptional<int32> OnGetSpawnLevel() const;
	void OnSetSpawnLevel(int32 inValue, ETextCommit::Type inType);
	ECheckBoxState GetSpawnLevelConditionCheckState() const;
	void OnSpawnLevelConditionCheckChanged(ECheckBoxState inState);	

	//Respawn Interval
private:
	ECheckBoxState GetRespawnIntervalConditionCheckState() const;
	void OnRespawnIntervalConditionCheckChanged(ECheckBoxState inState);
	TOptional<int32> OnGetMinRespawnInterval() const;
	void OnSetMinRespawnInterval(int32 inValue, ETextCommit::Type inType);
	TOptional<int32> OnGetMaxRespawnInterval() const;
	void OnSetMaxRespawnInterval(int32 inValue, ETextCommit::Type inType);	

	//select only 
private:
	ECheckBoxState GetOnlySelectActorSpawnCheckState() const;
	void  OnOnlySelectActorSpawnChanged(ECheckBoxState inState) const;

	//select all actor
private:
	ECheckBoxState GetSelectAllActorState() const;
	void  OnSelectAllActorChanged(ECheckBoxState inState);

	//npc list
private:
	void InitNpcActorList();	
	FText GetSelectedNpcActorText() const;	
	TSharedRef<ITableRow> OnGenerateNpcActorWidget(TSharedPtr<FString> inItem, const TSharedRef<STableViewBase>& inOwnerTable);				
	void OnDoubleClickNpcActor(TSharedPtr<FString> InNewSelection);
	void OnSelectionChangeNpcActor(TSharedPtr<FString> InNewSelection, ESelectInfo::Type InSelectInfo);
	void SetSelectAllActor(bool inSelect);

	//find
private:
	FReply OnFindNpc();
	FReply OnDelectAll();
	bool CanFindNpc();

	//valid
private:
	bool IsValidNpcId(class AGsTerritoryNpc* inNpc);
	bool IsValidUniqueId(class AGsTerritoryNpc* inNpc);
	bool IsValidMaxSpawnNum(class AGsTerritoryNpc* inNpc);
	bool IsValidNpcIdRange(class AGsTerritoryNpc* inNpc);
	bool IsValidInitSpawnDelay(class AGsTerritoryNpc* inNpc);
	bool IsValidRandomSpawn(class AGsTerritoryNpc* inNpc);
	bool IsValidRespawnInterval(class AGsTerritoryNpc* inNpc);
	bool IsValidSpawnLevel(class AGsTerritoryNpc* inNpc);

private:
	int32 GetNpcId(const FString& inNpcId);
};
