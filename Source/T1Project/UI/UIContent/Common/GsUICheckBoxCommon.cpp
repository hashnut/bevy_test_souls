// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUICheckBoxCommon.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"


void UGsUICheckBoxCommon::BeginDestroy()
{
	_toggleCallback = nullptr;	

	Super::BeginDestroy();
}

void UGsUICheckBoxCommon::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	_sBtnSlot->OnClicked.AddDynamic(this, &UGsUICheckBoxCommon::OnClickSlot);
}

void UGsUICheckBoxCommon::SetIsSelected(bool bInIsSelected)
{
	_sBtnSlot->SetSwitcherIndex(bInIsSelected ? 1 : 0);
}

bool UGsUICheckBoxCommon::GetIsSelected() const
{
	return (0 < _sBtnSlot->GetSwitcherIndex()) ? true : false;
}

void UGsUICheckBoxCommon::OnClickSlot()
{
	// 토글을 위해 부호 바꿈
	bool bIsChekced = !GetIsSelected();

	// 토글 그룹이면 토글 그룹에서 SetIsSelected를 조절 해준다.
	if (_toggleCallback)
	{
		_toggleCallback(_indexInGroup, bIsChekced);
		_onClickSlot.ExecuteIfBound(bIsChekced, _indexInGroup);
	}
	else
	{
		// 토글 그룹이 아니면 스스로 세팅 후 보냄
		SetIsSelected(bIsChekced);

		_onClickSlot.ExecuteIfBound(bIsChekced, _indexInGroup);
	}
}
