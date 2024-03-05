// Fill out your copyright notice in the Description page of Project Settings.


#include "GsDynamicIconSlotHelper.h"
#include "Blueprint/UserWidget.h"
#include "Components/PanelWidget.h"
#include "UI/UIControlLib/Interface/GsIconSelectorInterface.h"
#include "UI/UIContent/Common/Icon/GsUIIconBase.h"


void UGsDynamicIconSlotHelper::BeginDestroy()
{
	Super::BeginDestroy();
}

void UGsDynamicIconSlotHelper::Initialize(IGsIconSelectorInterface* InSelector)
{
	_panel = InSelector->GetPanel();
}

void UGsDynamicIconSlotHelper::Clear()
{
	RefreshAll(0);
}

void UGsDynamicIconSlotHelper::RefreshAll(const int InItemCount)
{
	// GetIconSelector 안에너 _panel.IsValid() 검사를 하므로 또 할 필요 없음
	IGsIconSelectorInterface* iconSelector = GetIconSelector();
	if (nullptr == iconSelector)
	{
		return;
	}

	// 이미 Panel에 자손으로 있는 것보다 많을 경우에 생성
	int32 createEntryCount = InItemCount - _panel->GetChildrenCount();
	if (0 < createEntryCount)
	{
		for (int i = 0; i < createEntryCount; ++i)
		{
			UGsUIIconBase* icon = iconSelector->CreateIcon();
			_panel->AddChild(icon);
			
			OnCreateIcon.Broadcast(icon);
		}
	}

	TArray<UWidget*> childList = _panel->GetAllChildren();
	for (int32 i = 0, maxCount = childList.Num(); i < maxCount; ++i)
	{
		UGsUIIconBase* icon = Cast<UGsUIIconBase>(childList[i]);
		if (nullptr == icon)
		{
			continue;
		}

		if (InItemCount > i)
		{
			// 엔트리에 Item 세팅할 수 있게 델리게이트 호출
			OnRefreshIcon.Broadcast(i, icon);
		}
		else
		{
			// item 개수를 초과하는 entry는 패널에서 제거, NativeDestruct에 의해 Pool에 반납
			_panel->RemoveChild(icon);

			// Pool에 반납
			icon->ReleaseToPool();
		}
	}
}

void UGsDynamicIconSlotHelper::CreateIcon(const int InItemCount)
{
	// GetIconSelector 안에너 _panel.IsValid() 검사를 하므로 또 할 필요 없음
	IGsIconSelectorInterface* iconSelector = GetIconSelector();
	if (nullptr == iconSelector)
	{
		return;
	}

	// 이미 Panel에 자손으로 있는 것보다 많을 경우에 생성
	int32 createEntryCount = InItemCount - _panel->GetChildrenCount();
	if (0 < createEntryCount)
	{
		for (int i = 0; i < createEntryCount; ++i)
		{
			UGsUIIconBase* icon = iconSelector->CreateIcon();
			_panel->AddChild(icon);

			OnCreateIcon.Broadcast(icon);
		}
	}
}

void UGsDynamicIconSlotHelper::RefreshAll()
{
	TArray<UWidget*> childList = _panel->GetAllChildren();
	for (int32 i = 0, maxCount = childList.Num(); i < maxCount; ++i)
	{
		UGsUIIconBase* icon = Cast<UGsUIIconBase>(childList[i]);
		if (nullptr == icon)
		{
			continue;
		}

		OnRefreshIcon.Broadcast(i, icon);
	}
}

void UGsDynamicIconSlotHelper::RefreshByIndex(const int InIndex)
{
	if (_panel.IsValid())
	{	
		if (UWidget* entry = _panel->GetChildAt(InIndex))
		{
			OnRefreshIcon.Broadcast(InIndex, Cast<UGsUIIconBase>(entry));
		}
	}
}

UGsUIIconBase* UGsDynamicIconSlotHelper::GetIcon(const int InIndex)
{
	if (_panel.IsValid())
	{
		if (UWidget* entry = _panel->GetChildAt(InIndex))
		{
			return Cast<UGsUIIconBase>(entry);
		}
	}

	return nullptr;
}

// 파라미터 count 만큼 생성함 단, InMaxCount 갯수를 받아서 체크함
void UGsDynamicIconSlotHelper::CreateIconByTick(const int32 InCreateCount, const int32 InMaxCount)
{
	// GetIconSelector 안에너 _panel.IsValid() 검사를 하므로 또 할 필요 없음
	IGsIconSelectorInterface* iconSelector = GetIconSelector();
	if (nullptr == iconSelector)
	{
		return;
	}

	// 이미 Panel에 자손으로 있는 갯수가 InMaxCount 보다 많다면 생성하지 않음
	int32 realCreateCount = 0;
	int32 createEntryCount =  _panel->GetChildrenCount();
	if (createEntryCount < InMaxCount)
	{
		realCreateCount = (createEntryCount + InCreateCount) > InMaxCount ?
			InMaxCount - createEntryCount : InCreateCount;

		for (int i = 0; i < realCreateCount; ++i)
		{
			UGsUIIconBase* icon = iconSelector->CreateIcon();
			_panel->AddChild(icon);

			OnCreateIcon.Broadcast(icon);
			//OnRefreshIcon.Broadcast(createEntryCount, icon);

			++createEntryCount;
		}
	}
	else if(createEntryCount > InMaxCount) // max count 를 넘어가면 pool 로 반환
	{
		int32 count = createEntryCount - InMaxCount;
		if (0 < count)
		{
			for (int32 i = 0; i < count; ++i)
			{
				UGsUIIconBase* icon = GetIcon((InMaxCount + i));
				if (nullptr == icon)
					continue;

				_panel->RemoveChild(icon);

				// Pool에 반납
				icon->ReleaseToPool();
			}
		}
	}
}

void UGsDynamicIconSlotHelper::RefreshIconByTick(const int32 InRefreshCount , const int32 InStartIndex)
{
	for (int32 i = InStartIndex; i < (InStartIndex + InRefreshCount); ++i)
	{
		RefreshByIndex(i);
	}
}

IGsIconSelectorInterface* UGsDynamicIconSlotHelper::GetIconSelector() const
{
	if (_panel.IsValid())
	{
		return Cast<IGsIconSelectorInterface>(_panel.Get());
	}

	return nullptr;
}
