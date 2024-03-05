
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SBoxPanel.h"
#include "SCheckBox.h"
#include "SListView.h"
#include "SComboBox.h"
#include "Editor.h"
#include "SSearchableComboBox.h"

struct FGsTerrEdFinderCheck
{
public:
	bool _isCheck;
	TSharedPtr<SCheckBox> _checkBox;

public:
	FGsTerrEdFinderCheck() {}

	virtual void Initialize() {
		_isCheck = false;
	}
	virtual bool IsValid() {
		return true;
	}

	virtual void Clear(){}

	ECheckBoxState GetCheckState() const
	{
		return (_isCheck ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
	}

	void SetConditionCheckState(ECheckBoxState inState)
	{
		_isCheck = (inState == ECheckBoxState::Checked ? true : false);
	}

	virtual ~FGsTerrEdFinderCheck() { Clear(); }
};

template <typename T>
struct FGsTerrEdFinderNumericEntryBox : public FGsTerrEdFinderCheck
{
public:
	T _value;
	TSharedPtr<SNumericEntryBox<T>> _numericEntryBox;

public:
	virtual void Initialize() override {

		FGsTerrEdFinderCheck::Initialize();
		_value = 0;		
	}

	virtual ~FGsTerrEdFinderNumericEntryBox() {}
};

template <typename T>
struct FGsTerrEdFinderRangeNumericEntryBox : public FGsTerrEdFinderCheck
{
public:
	T _minValue;
	T _maxValue;
	TSharedPtr<SNumericEntryBox<T>> _minNumericEntryBox;
	TSharedPtr<SNumericEntryBox<T>> _maxNumericEntryBox;

public:
	virtual void Initialize() override {

		FGsTerrEdFinderCheck::Initialize();
		_minValue = 0;
		_maxValue = TNumericLimits<T>::Max();
	}

	virtual bool IsValid() override {
		return _minValue <= _maxValue;
	}

	bool IsValid(T inValue) {
		return _minValue <= inValue
			&& inValue <= _maxValue;
	}

	virtual ~FGsTerrEdFinderRangeNumericEntryBox() {}
};

struct FGsTerrEdFinderComboBox : public FGsTerrEdFinderCheck
{
public:	
	TArray<TSharedPtr<FString>> _valueList;
	TSharedPtr<SComboBox<TSharedPtr<FString>>> _comboBox;
	TSharedPtr<FString> _selectedValue;

public:
	virtual void Initialize() override {	
		FGsTerrEdFinderCheck::Initialize();
		_isCheck = false;
	}

	virtual void Clear() override {
		_valueList.Empty();
	}

	virtual ~FGsTerrEdFinderComboBox() {}
};

struct FGsTerrEdRangeSearchableComboBox : public FGsTerrEdFinderCheck
{
public:
	TArray<TSharedPtr<FString>> _valueList;
	TSharedPtr<SListView<TSharedPtr<FString>>> _listView;

public:
	TSharedPtr<FString> _minSelectStringPtr;
	TSharedPtr<FString> _maxSelectStringPtr;
	FString _minSelectedString;
	FString _maxSelectedString;	

public:
	TSharedPtr<SSearchableComboBox> _minSearchableComboBox;
	TSharedPtr<SSearchableComboBox> _maxSearchableComboBox;

public:
	virtual void Initialize() override {
		FGsTerrEdFinderCheck::Initialize();
		_isCheck = false;
	}

	virtual void Clear() override {
		_valueList.Empty();
	}

	virtual ~FGsTerrEdRangeSearchableComboBox() {}
};

struct FGsTerrEdSearchableComboBox : public FGsTerrEdFinderCheck
{
public:
	TSharedPtr<SSearchableComboBox> _searchableComboBox;	
	TArray<TSharedPtr<FString>> _valueList;
	TSharedPtr<IPropertyHandle> _selectStringHandle;	

public:
	FString _selectedString;
	TSharedPtr<SListView<TSharedPtr<FString>>> _listView;
	TSharedPtr<FString> _selectStringPtr;

public:
	virtual void Initialize() override {
		FGsTerrEdFinderCheck::Initialize();
		_isCheck = false;
	}

	virtual void Clear() override {
		_valueList.Empty();
	}

	virtual ~FGsTerrEdSearchableComboBox() {}
};

struct FGsTerrEdFinderListView : public FGsTerrEdFinderCheck
{
public:
	TArray<TSharedPtr<FString>> _valueList;
	FString _selectedString;
	TSharedPtr<SListView<TSharedPtr<FString>>> _listView;
	TSharedPtr<FString> _selectStringPtr;

public:
	virtual void Initialize() override {		
		FGsTerrEdFinderCheck::Initialize();
		_isCheck = false;
	}

	virtual void Clear() override {
		_valueList.Empty();
	}

	virtual ~FGsTerrEdFinderListView() {}
};

struct FGsTerrEdFinderActorListView
{
public:
	TArray<TSharedPtr<FString>> _valueList;
	FString _selectedString;
	TSharedPtr<SListView<TSharedPtr<FString>>> _listView;
	TSharedPtr<FString> _selectStringPtr;
	TArray<AActor*> _actorList;

public:
	virtual void Initialize() {

	}
	void Clear()
	{		
		_valueList.Empty();
		_selectStringPtr.Reset();
		_actorList.Empty();

		if (_listView.IsValid())
		{
			_listView->ClearSelection();
			_listView->SetListItemsSource(_valueList);
			_listView->RebuildList();
			_listView->RequestListRefresh();			
		}

		GEditor->SelectNone(true, true, false);
	}

	virtual ~FGsTerrEdFinderActorListView() {}
};

class FGsTerrEdSlateHelper
{
public:

	static TSharedPtr<SWidget> CreateNumericWidget(FString inDescription, TFunction<float(void)> inReturnVlaue, TFunction<void(float, ETextCommit::Type)> inSetVlaue, float inFieldColumSize = 100, float inValueColumSize = 100);
};