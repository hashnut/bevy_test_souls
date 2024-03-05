// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITraySpaceCrackResult.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UI/UIContent/Common/Icon/GsUIIconBase.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemReward.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "UI/UIContent/Common/GsUICurrencySlot.h"
#include "UI/UIContent/Popup/GsUIPopupItemDetailMain.h"

#include "T1Project.h"
#include "Engine/Public/TimerManager.h"

#include "Item/GsItemManager.h"
#include "Item/GsItem.h"

#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "Message/GsMessageContents.h"
#include "Management/GsMessageHolder.h"

#include "TextBlock.h"


void UGsUITraySpaceCrackResult::BeginDestroy()
{
	_iconSlotHelper = nullptr;
	Super::BeginDestroy();
}

void UGsUITraySpaceCrackResult::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_backBtn->OnClicked.AddDynamic(this, &UGsUITraySpaceCrackResult::OnClickBack);

	if (nullptr == _iconSlotHelper)
	{
		_iconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_iconSlotHelper->Initialize(_iconSelector);
		_iconSlotHelper->OnCreateIcon.AddDynamic(this, &UGsUITraySpaceCrackResult::OnCreateIcon);
		_iconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUITraySpaceCrackResult::OnRefreshIcon);
	}
}

void UGsUITraySpaceCrackResult::NativeConstruct()
{
	Super::NativeConstruct();

	// 타이머 등록
	GetWorld()->GetTimerManager().SetTimer(_timerHandle, this, &UGsUITraySpaceCrackResult::OnClickBack, 30.f, false);	
}

void UGsUITraySpaceCrackResult::NativeDestruct()
{
	_iconSlotHelper->Clear();
	_iconSelector->RemoveAllChildren();
	_itemList.Empty();
	//타이머 해지
	GetWorld()->GetTimerManager().ClearTimer(_timerHandle);

	Super::NativeDestruct();
}

void UGsUITraySpaceCrackResult::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{	
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UGsUITraySpaceCrackResult::OnRefreshIcon(int32 InIndex, UGsUIIconBase * InIcon)
{
	UGsUIIconItemReward* itemIcon = Cast<UGsUIIconItemReward>(InIcon);
	if (itemIcon)
	{
		itemIcon->SetEnableDetailPopup(true);
		itemIcon->SetCenterPopup(true);

		int currencyNum = _itemList.Num();
		if (InIndex < currencyNum)
		{
			int index = 0;
			for (TMap<uint32, int>::TIterator it = _itemList.CreateIterator(); it; ++it, ++index)
			{
				if (index == InIndex)
				{
					uint32 key = it.Key();
					itemIcon->SetNotOwnItem(key, it.Value());
					break;
				}
			}
		}
	}
}

void UGsUITraySpaceCrackResult::SetData(uint32 inPlayTime, Money inGold, Money inMileage,
	SpaceCrackEnemyKill inZakoCount, SpaceCrackEnemyKill inEliteCount,
	TMap<uint32, int> inItemList)
{
	_itemList.Empty();
	_itemList = inItemList;
	_iconSlotHelper->RefreshAll(_itemList.Num());
	_textBlockTime->SetText(FText::AsNumber(inPlayTime));
	_currencyGoldSlot->SetData(CurrencyType::GOLD, static_cast<Currency>(inGold));
	_currencyMileageSlot->SetData(CurrencyType::MILEAGE, static_cast<Currency>(inMileage));
	_textMonsterCount->SetText(FText::AsNumber(inZakoCount));
	_textEliteMonsterCount->SetText(FText::AsNumber(inEliteCount));

	int minute = inPlayTime / 60;
	int Second = inPlayTime % 60;

	_textBlockTime->SetText(FText::FromString(FString::Format(TEXT("{0} : {1}"), { minute,  Second })));
}

void UGsUITraySpaceCrackResult::OnClickBack()
{
	// 창 닫기
	Close();
}

// 아이콘 생성
void UGsUITraySpaceCrackResult::OnCreateIcon(UGsUIIconBase* InIcon)
{
	UGsUIIconItemReward* itemIcon = Cast<UGsUIIconItemReward>(InIcon);
	if (itemIcon)
	{
		itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUITraySpaceCrackResult::OnLongPressIcon);
	}
}

// 아이콘 상세보기 창
void UGsUITraySpaceCrackResult::OnLongPressIcon(UGsUIIconItem& InIcon)
{
	UGsUIIconItem::DefaultItemDetailOpener(InIcon, true);
}
