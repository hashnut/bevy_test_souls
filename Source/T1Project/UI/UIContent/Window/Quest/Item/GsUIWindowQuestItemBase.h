// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object.h"
#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"
#include "NoExportTypes.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIWindowQuestItemBase.generated.h"

/**
 *	스크롤을 아이템 Base
 */
UCLASS()
class T1PROJECT_API UGsUIWindowQuestItemBase :	public UObject
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnClickSlot, const int32);
	DECLARE_DELEGATE_TwoParams(FOnClickSlot2, const int32, const int32);
	DECLARE_DELEGATE_ThreeParams(FOnClickSlot3, const int32, const int32, const int8);
		
protected:
	UPROPERTY()
	FText _titleText;

	UPROPERTY()
	FSoftObjectPath _iconPath;

	UPROPERTY()
	TArray<UGsUIWindowQuestItemBase*> _childList;
	
	int		_index			= 0;
	int32	_storyId		= 0;	
	bool	_isVisible		= true;
	bool	_isSelecting	= false;
	bool	_isRedDot		= false;	
	int		_menuIndex		= 0;
	int		_selectIndex	= 0;
	int32	_npcId			= 0;
	bool	_isWaiting		= false;

	QuestWindowUISlotType		_slotStateType;
	QuestWindowUISubSlotType	_slotType;
	QuestWindowUISlotType		_slotMarkType;

public:
	FOnClickSlot _delegateClick1;
	FOnClickSlot2 _delegateClick2;
	FOnClickSlot3 _delegateClick3;

public:
	void AddChild(UGsUIWindowQuestItemBase* InItem);
	void removeChild(UGsUIWindowQuestItemBase* InItem);
	void removeAllChildren();
	
public:	
	int		GetIndex() { return _index; }
	int32	GetStoryId() { return _storyId; }	
	bool	GetVisible() { return _isVisible; }
	bool	GetSelecting() { return _isSelecting; }
	bool	GetIsRedDot() { return _isRedDot; }
	int		GetMenuIndex() { return _menuIndex; }
	int		GetSelectIndex() { return _selectIndex; }
	int32	GetNpcId() { return _npcId; }
	bool	GetIsWaiting() { return _isWaiting; }

	QuestWindowUISlotType		GetSlotStateType()		{ return _slotStateType; }
	QuestWindowUISubSlotType	GetSlotType()			{ return _slotType; }
	QuestWindowUISlotType		GetSlotMarkType() { return _slotMarkType; }
	const FSoftObjectPath&		GetNpcIconPath()  const { return _iconPath; }
	FText&						GetTitleText()			{ return _titleText; }

	TArray<UGsUIWindowQuestItemBase*> GetChildList() const { return _childList; }

	bool GetIsVisible() { return _isVisible; }
	void SetVisible(bool inVisible) { _isVisible = inVisible; }
	void SetIsRedDot(bool inIsRedDot) { _isRedDot = inIsRedDot; }
	void SetSelectIndex(int inSelectIndex) { _selectIndex = inSelectIndex; }
	void SetIsWaiting(bool inIsWaiting) { _isWaiting = inIsWaiting; }

	void SetSlotMarkType(QuestWindowUISlotType inSlotType) { _slotMarkType = inSlotType; }

public:
	void SetData(int inIndex, int inMenuIndex, QuestWindowUISubSlotType inSlotType, const FText inTitleText);
	void SetData(int inIndex, QuestWindowUISubSlotType inSlotType, bool inIsVisible, const FText inTitleText,
		const FSoftObjectPath inIconPath);
	void SetData(int inIndex, StoryId inStoryId, int32 inNpcId, int inMenuIndex, QuestWindowUISubSlotType inSlotTyp, QuestWindowUISlotType inSlotStateType,
		bool inIsVisible, bool inIsSelecting, const FText inTitleText);
};
