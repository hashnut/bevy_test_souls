// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupResolutionList.h"
#include "RichTextBlock.h"

#include "Management/ScopeGame/GsEventProgressManager.h"
#include "EventProgress/GsEventProgressDefine.h"

#include "GameMode/GsGameModeBase.h"
#include "GameMode/GsGameModeDefine.h"
#include "IntPoint.h"

#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "UI/UIContent/Window/Option/GsUIOptionButtonGroup.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "Option/GsGameUserSettings.h"
#include "Management/ScopeGlobal/GsOptionManager.h"


void UGsUIPopupResolutionList::NativeOnInitialized()
{
	_panelSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_panelSlotHelper->Initialize(_entryWidgetClass, _scrollPanel);
	_panelSlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPopupResolutionList::OnRefreshEntry);
	
	_toggleGroup.AddToggleWidgetByParentPanel(_scrollPanel);
	_toggleGroup.OnSlotClicked.BindUObject(this, &UGsUIPopupResolutionList::OnClickSlot);

	_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupResolutionList::OnClickClose);

	Super::NativeOnInitialized();
}

void UGsUIPopupResolutionList::NativeDestruct()
{
	_preIndex = -1;
	_initIndex = 0;
	_selectCallback = nullptr;

	if (nullptr != _destructCallback)
	{
		_destructCallback(_index);
		_destructCallback = nullptr;
	}

	Super::NativeDestruct();
}

void UGsUIPopupResolutionList::OnClickSlot(int32 InIndex, bool bIsSelected)
{
	int size = _resolutionTextList.Num();
	if (InIndex >= size)
	{
		return;
	}

	if (_index == InIndex)
	{
		return;
	}

#if WITH_EDITOR || PLATFORM_WINDOWS
	if (UGsGameUserSettings* gameUserSettings = GGameUserSettings())
	{
		int myResolutionNum = gameUserSettings->GetResolutionsNum();
		if (myResolutionNum == InIndex)
		{
			_toggleGroup.SetSelectedIndex(_index, false);
			// 수동설정값은 처리 없음
			return;
		}
	}
#endif

	_preIndex = _index;
	_index = InIndex;
	_isCallEventProcessFinish = false;

	if (nullptr != _selectCallback)
	{
		_selectCallback(_index);
	}
}

void UGsUIPopupResolutionList::SetData(TArray<FText>& InResolutionTextList, int InMyIndex, TFunction<void(int)> InCallback,
	TFunction<void(int)> InDestructCallback)
{
	_initIndex = InMyIndex;
	_index = InMyIndex;
	_resolutionTextList = InResolutionTextList;
	_selectCallback = InCallback;
	_destructCallback = InDestructCallback;

	_panelSlotHelper->RefreshAll(_resolutionTextList.Num());

	_toggleGroup.Clear();
	_toggleGroup.AddToggleWidgetByParentPanel(_scrollPanel);
	_toggleGroup.SetSelectedIndex(_index, false);
}

void UGsUIPopupResolutionList::OnRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	int size = _resolutionTextList.Num();
	if (InIndex >= size)
	{
		return;
	}
	
	UGsUIOptionButtonGroup* entry = Cast<UGsUIOptionButtonGroup>(InEntry);
	if (entry)
	{
		entry->SetData(_resolutionTextList[InIndex]);
	}
}

// 닫기 클릭
void UGsUIPopupResolutionList::OnClickClose()
{	
	_isCallEventProcessFinish = false;

	Close();
}

void UGsUIPopupResolutionList::OnInputCancel()
{
	OnClickClose();
}

void UGsUIPopupResolutionList::ApplyPreIndexButton()
{
	if (-1 < _preIndex)
	{
		_toggleGroup.SetSelectedIndex(_preIndex, false);
		OnClickSlot(_preIndex, true);
	}
}