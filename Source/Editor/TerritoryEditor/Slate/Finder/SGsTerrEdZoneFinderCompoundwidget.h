
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

class SGsTerrEdZoneFinderCompoundwidget : public SGsTerrEdBaseFinderCompoundwidget
{
public:
	SLATE_BEGIN_ARGS(SGsTerrEdZoneFinderCompoundwidget) {}
	SLATE_END_ARGS()

	//condition
private:
	//unique
	FGsTerrEdFinderNumericEntryBox<int64> _uniqueIdNumericEntryBox;
	//spawn gorup
	FGsTerrEdFinderComboBox _spawnGroupComboBox;
	//by client
	TSharedPtr<SCheckBox> _byClientCheckBox;
	TSharedPtr<SCheckBox> _byClientConditionCheckBox;
	//by default
	TSharedPtr<SCheckBox> _byDefaultCheckBox;
	TSharedPtr<SCheckBox> _byDefaultConditionCheckBox;
	//radius
	FGsTerrEdFinderRangeNumericEntryBox<float> _zoneRadiusNumericEntryBox;
	//section
	FGsTerrEdFinderNumericEntryBox<int32> _zoneSectionNumericEntryBox;
	//description
	TSharedPtr<SEditableTextBox> _noteEditableTextBox;
	TSharedPtr<SCheckBox> _noteTextCheckBox;

	//actor list
private:	
	TArray<TSharedPtr<FString>> _zoneIdList;

private:
	bool _isCheckByClient;
	bool _isCheckByClientCondition;

private:
	bool _isCheckByDefault;	
	bool _isCheckByDefaultCondition;

private:
	FString _note;
	bool _isCheckNoteCondition;

public:
	void Construct(const FArguments& InArgs);	
	//conditions

	//create widget
private:
	TSharedPtr<SWidget> CreateUniqueIdWidget();
	TSharedPtr<SWidget> CreateSpawnGroupWidget();
	TSharedPtr<SWidget> CreateByClientWidget();
	TSharedPtr<SWidget> CreateByDefualtWidget();
	TSharedPtr<SWidget> CreateRadiusRangeWidget();
	TSharedPtr<SWidget> CreateSectionWidget();
	TSharedPtr<SWidget> CreateNoteWidget();
	TSharedPtr<SWidget> CreateZoneFindWidget();

	//unique id
private:
	TOptional<int64> OnGetUniqueId() const;
	void OnSetUniqueId(int64 inValue, ETextCommit::Type inType);
	ECheckBoxState GetUniqueIdConditionCheckState() const;
	void OnUniqueIdConditionCheckChanged(ECheckBoxState inState);

	//spawn group
private:
	TArray<TSharedPtr<FString>> GetSpawnGroupTypeList();
	void OnSpawnGroupTypeSelectionChanged(TSharedPtr<FString> InItem, ESelectInfo::Type InSeletionInfo);
	TSharedRef<SWidget> OnGenerateComboWidget(TSharedPtr<FString> inItem);
	FText GetSelectedSpawnGroupTypeText() const;
	ECheckBoxState GetSpawnGroupTypeConditionCheckState() const;
	void OnSpawnGroupTypeConditionCheckChanged(ECheckBoxState inState);

	//by client
private:
	ECheckBoxState GetByClientCheckState() const;
	void OnByClientCheckChanged(ECheckBoxState inState);
	ECheckBoxState GetByClientConditionCheckState() const;
	void OnByClientConditionCheckChanged(ECheckBoxState inState);

	//by default
private:
	ECheckBoxState GetByDefaultCheckState() const;
	void OnByDefaultCheckChanged(ECheckBoxState inState);
	ECheckBoxState GetByDefaultConditionCheckState() const;
	void OnByDefaultConditionCheckChanged(ECheckBoxState inState);

	//zone radius
private:
	TOptional<float> OnGetMinZoneRadius() const;
	void OnSetMinZoneRadius(float inValue, ETextCommit::Type inType);
	TOptional<float> OnGetMaxZoneRadius() const;
	void OnSetMaxZoneRadius(float inValue, ETextCommit::Type inType);
	ECheckBoxState GetZoneRadiusConditionCheckState() const;
	void OnZoneRadiusConditionCheckChanged(ECheckBoxState inState);

	//section section
private:
	TOptional<int32> OnGetZoneSection() const;
	void OnSetZoneSection(int32 inValue, ETextCommit::Type inType);	
	ECheckBoxState GetZoneSectionConditionCheckState() const;
	void OnZoneSectionConditionCheckChanged(ECheckBoxState inState);

	//descriptoin
private:
	void OnNoteTextChanged(const FText& inText);
	void OnNoteTextCommit(const FText&, ETextCommit::Type);
	ECheckBoxState GetNoteConditionCheckState() const;
	void OnNoteConditionCheckChanged(ECheckBoxState inState);

private:
	void InitZoneActorList();
	FText GetSelectedZoneActorText() const;
	TSharedRef<ITableRow> OnGenerateTableWidget(TSharedPtr<FString> inItem, const TSharedRef<STableViewBase>& inOwnerTable);
	void OnDoubleClickZoneActor(TSharedPtr<FString> InNewSelection);
	void OnSelectionChangeZoneActor(TSharedPtr<FString> InNewSelection, ESelectInfo::Type InSelectInfo);	
	int32 GetZoneId(const FString& inZoneId);

private:
	ECheckBoxState GetSelectAllActorState() const;
	void OnSelectAllActorChanged(ECheckBoxState inState);

private:
	void SetSelectAllActor(bool inAllSelect);
	ECheckBoxState GetOnlySelectActorSpawnCheckState() const;	
	void OnOnlySelectActorSpawnChanged(ECheckBoxState inState) const;

	//find, deselect all
private:
	FReply OnFindZone();
	FReply OnDelectAll();
	bool CanFindZone();

	//validation
private:
	bool IsValidUniqueId(class AGsTerritoryZone* inNpc);
	bool IsValidSpawnType(class AGsTerritoryZone* inZone);
	bool IsValidZoneRadius(class AGsTerritoryZone* inZone);
	bool IsValidByClient(class AGsTerritoryZone* inZone);
	bool IsValidByDefault(class AGsTerritoryZone* inZone);
	bool IsValidSection(class AGsTerritoryZone* inZone);
	bool IsValidNote(class AGsTerritoryZone* inZone);
	
};
