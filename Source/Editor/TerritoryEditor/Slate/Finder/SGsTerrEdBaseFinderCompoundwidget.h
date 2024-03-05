
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

class SGsTerrEdBaseFinderCompoundwidget : public SCompoundWidget, public IGsTerrEdFinderInterface
{
public:
	SLATE_BEGIN_ARGS(SGsTerrEdBaseFinderCompoundwidget) {}
	SLATE_END_ARGS()

	//Layout
protected:
	float _fieldLeftColumnSize;
	float _fieldRightColumnSize;
	float _fieldColumnSize;
	float _conditionColumnSize;
	float _height;
	FMargin _fieldLeftRowPadding;

protected:
	//actor list
	FGsTerrEdFinderActorListView _findActorListView;

protected:
	TSharedPtr<SCheckBox> _showOnlySelectedActorCheckBox;
	TSharedPtr<SCheckBox> _selectAllActorCheckBox;

protected:
	bool _isCheckOnlyShowSelectActor = false;
	bool _isCheckSelectAllActor = false;

protected:
	int32 _selectCount;
	TSharedPtr<STextBlock> _selectCountTextBlock;
	int32 _totalCount;
	TSharedPtr<STextBlock> _totalCountTextBlock;

protected:
	virtual TSharedPtr<SWidget> CreateSelectCountWidget();
	virtual TSharedPtr<SWidget> CreateTotalCountWidget();

public:	
	virtual void ClearListView() override;
};
