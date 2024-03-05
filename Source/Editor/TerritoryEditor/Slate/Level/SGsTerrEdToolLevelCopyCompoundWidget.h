
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SDockTab.h"
#include "SDockableTab.h"
#include "SDockTabStack.h"
#include "SlateApplication.h"
#include "GsTerritoryLevelManager.h"
#include "Slate/GsTerrEdSlateHelper.h"

class SWindow;

class SGsTerrEdToolLevelCopyCompoundWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGsTerrEdToolLevelCopyCompoundWidget){}
	SLATE_END_ARGS()

private:
	FGsTerrEdSearchableComboBox _sourceServerLevelSearchableComboBox;
	FGsTerrEdSearchableComboBox _destServerLevelSearchableComboBox;
	TSharedPtr<SCheckBox> _spawnActorCheckBox;
	TSharedPtr<SCheckBox> _spotCheckBox;
	TSharedPtr<SCheckBox> _fenceCheckBox;
	TSharedPtr<SCheckBox> _lineCheckBox;

private:	
	TArray<TSharedPtr<FString>> _serverLevelIdList;
	TArray<TSharedPtr<FString>> _showTypeList;

private:
	float _height;
	float _descriptionColumnSize;
	float _excuteColumnSize;	
	float _padding;
	float _fieldLeftColumnSize;
	float _fieldRightColumnSize;
	float _fieldColumnSize;
	float _conditionColumnSize;	
	FMargin _fieldLeftRowPadding;

private:
	bool _isSpawnActorCopy;
	bool _isSpotCopy;
	bool _isFenceCopy;
	bool _isLineCopy;

	int32 _sourceServerLevelKey = INDEX_NONE;
	int32 _destServerLevelKey = INDEX_NONE;

public:
	void Construct(const FArguments& inArgs);

public:
	TSharedPtr<SWidget> CreateSourceServerLevelWidget();
	TSharedPtr<SWidget> CreateDestServerLevelWidget();
	TSharedPtr<SWidget> CreateSpawnActorCopyWidget();
	TSharedPtr<SWidget> CreateSpotCopyWidget();
	TSharedPtr<SWidget> CreateFenceCopyWidget();
	TSharedPtr<SWidget> CreateLineCopyWidget();

private:
	void InitServerLevelList();
	FText GetSelectSourceServerIdText() const;
	FText GetSelectDestServerIdText() const;
	void OnSourceServerLevelSelectionChanged(TSharedPtr<FString> InItem, ESelectInfo::Type InSeletionInfo);
	void OnDestServerLevelSelectionChanged(TSharedPtr<FString> InItem, ESelectInfo::Type InSeletionInfo);
	TSharedRef<SWidget> OnGenerateSearchBoxWidget(TSharedPtr<FString> inItem);

private:
	void OnSpawnActorCopyCheckChanged(ECheckBoxState inState);
	void OnSpotCopyCheckChanged(ECheckBoxState inState);
	void OnFenceCopyCheckChanged(ECheckBoxState inState);
	void OnLineCopyCheckChanged(ECheckBoxState inState);

private:
	int32 GetLevelId(const FString& inServerId);

private:
	FReply OnCopyLevel();

};