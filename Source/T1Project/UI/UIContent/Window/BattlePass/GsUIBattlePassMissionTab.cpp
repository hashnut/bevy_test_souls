// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIBattlePassMissionTab.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"

void UGsUIBattlePassMissionTab::SetIsSelected(bool bInIsSelected)
{
	_isSelected = bInIsSelected;
	SetTabLock(_isLock);
}

void UGsUIBattlePassMissionTab::SetTabLock(bool isLock)
{
	_isLock = isLock;

	_switchButton->SetSwitcherIndex(_isLock ? _slotIndexLock :
		_isSelected ? _switchButton->_slotIndexSelected : _switchButton->_slotIndexUnselected);
}
