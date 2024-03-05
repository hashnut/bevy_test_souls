// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUICurrencyList.h"

#include "Components/WrapBox.h"

#include "DataSchema/GsSchemaCurrencyData.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsHiveManager.h"

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"

#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"

#include "UI/UIContent/Common/GsUICurrencySlot.h"
#include "UI/UIContent/Common/GsUICurrencyDetailInfo.h"


void UGsUICurrencyList::BeginDestroy()
{
	if (_currencySlotHelper)
	{
		_currencySlotHelper->OnCreateEntry.RemoveDynamic(this, &UGsUICurrencyList::OnCreateCurrencyEntry);
		_currencySlotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUICurrencyList::OnRefreshCurrencyEntry);
	}
	_currencySlotHelper = nullptr;

	Super::BeginDestroy();
}

void UGsUICurrencyList::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 포커싱 가능한 위젯으로 만든다
	bIsFocusable = true;

	UGsHiveManager* hiveMgr = GHive();
	// 표시할 재화 목록 (_dropDownOrder > 0 인 재화만 표시)
	_currencyList.Empty();
	const UGsTable* currencyTable = FGsSchemaCurrencyData::GetStaticTable();
	if (currencyTable)
	{
		TArray<const FGsSchemaCurrencyData*> dataList;
		if (currencyTable->GetAllRows(dataList))
		{		
			for (const FGsSchemaCurrencyData* currencyData : dataList)
			{
				if (currencyData->dropDownOrder <= 0)
				{
					continue;
				}
				if (CurrencyType::FREE_DIA == currencyData->currencyType &&
					false == hiveMgr->IsDisplayFreeDiaCountry())
				{
					continue;
				}

				_currencyList.Emplace(currencyData);
			}
		}
	}
	_panelWarning->SetVisibility(hiveMgr->IsDisplayFreeDiaCountry() ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	// 오름차순 정렬
	_currencyList.Sort([](const FGsSchemaCurrencyData& A, const FGsSchemaCurrencyData& B)
		{
			return A.dropDownOrder < B.dropDownOrder;
		});

	// 슬롯 헬퍼 생성 및 이벤트 연결
	_currencySlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_currencySlotHelper->Initialize(_currencyEntryClass, _panelCurrency);
	_currencySlotHelper->OnCreateEntry.AddDynamic(this, &UGsUICurrencyList::OnCreateCurrencyEntry);
	_currencySlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUICurrencyList::OnRefreshCurrencyEntry);
}

void UGsUICurrencyList::NativeDestruct()
{
	OnHide.Clear();

	Super::NativeDestruct();
}

FReply UGsUICurrencyList::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	OnHide.Broadcast();

	return FReply::Handled();
}

void UGsUICurrencyList::Show()
{
	SetFocus();

	_openedCurrencyInfoType = 0;
	_bIsCurrencyInfoOpen = false;

	if (_currencyInfo)
	{
		_currencyInfo->SetVisibility(ESlateVisibility::Collapsed);
	}

	InvalidateCurrencyList();


	_handlerUserInfoList.Emplace(GMessage()->GetUserBasicInfo().AddUObject(MessageUserInfo::CURRENCY,
		this, &UGsUICurrencyList::OnInvalidateCurrency));
}

void UGsUICurrencyList::Hide()
{
	_openedCurrencyInfoType = 0;
	_bIsCurrencyInfoOpen = false;
	
	if (_currencyInfo)
	{
		_currencyInfo->SetVisibility(ESlateVisibility::Collapsed);
	}

	for (TPair<MessageUserInfo, FDelegateHandle>& elemContents : _handlerUserInfoList)
	{
		GMessage()->GetUserBasicInfo().Remove(elemContents);
	}
	_handlerUserInfoList.Empty();

}

void UGsUICurrencyList::InvalidateCurrencyList()
{
	if (_currencySlotHelper)
	{
		_currencySlotHelper->RefreshAll(_currencyList.Num());
	}
}

void UGsUICurrencyList::SetCurrencyInfo(UGsUICurrencyDetailInfo* InCurrencyInfo)
{
	_currencyInfo = InCurrencyInfo;
	if (_currencyInfo)
	{
		_currencyInfo->SetCurrencyList(this);
		_currencyInfo->OnHide.AddUObject(this, &UGsUICurrencyList::OnHideCurrencyInfo);
	}
}

void UGsUICurrencyList::OnInvalidateCurrency(uint64 InType)
{
	CurrencyType targetCurrency = static_cast<CurrencyType>(InType);

	// 해당 재화의 인덱스를 찾아서 갱신
	int32 targetIndex = _currencyList.IndexOfByPredicate([targetCurrency](const FGsSchemaCurrencyData* currencyData)
		{
			return currencyData->currencyType == targetCurrency;
		});
	
	if (targetIndex != INDEX_NONE)
	{
		_currencySlotHelper->RefreshByIndex(targetIndex);
	}

	if (_bIsCurrencyInfoOpen && _openedCurrencyInfoType == InType)
	{
		if (_currencyInfo)
		{
			_currencyInfo->SetAmount(targetCurrency);
		}
	}
}

void UGsUICurrencyList::OnCreateCurrencyEntry(UWidget* InEntry)
{
	if (UGsUICurrencySlot* slot = Cast<UGsUICurrencySlot>(InEntry))
	{
		slot->OnClickCurrencySlot.BindUObject(this, &UGsUICurrencyList::OnClickCurrencyEntry);
	}
}

void UGsUICurrencyList::OnRefreshCurrencyEntry(int32 InIndex, UWidget* InEntry)
{
	if (false == _currencyList.IsValidIndex(InIndex))
	{
		return;
	}

	if (UGsUICurrencySlot* slot = Cast<UGsUICurrencySlot>(InEntry))
	{
		slot->SetDataLocalPlayer(_currencyList[InIndex]->currencyType);
	}
}

void UGsUICurrencyList::OnClickCurrencyEntry(class UWidget* InEntry)
{
	if (nullptr == _currencyInfo)
	{
		_openedCurrencyInfoType = 0;
		_bIsCurrencyInfoOpen = false;
		return;
	}

	uint64 prevCurrencyType = _openedCurrencyInfoType;
	if (UGsUICurrencySlot* slot = Cast<UGsUICurrencySlot>(InEntry))
	{
		_openedCurrencyInfoType = static_cast<uint64>(slot->GetCurrencyType());
	}
	
	// 열려있을 때
	if (_bIsCurrencyInfoOpen)
	{
		// 열린 재화와 같은 재화 터치 시 종료
		if (prevCurrencyType == _openedCurrencyInfoType)
		{
			_bIsCurrencyInfoOpen = false;
			_openedCurrencyInfoType = 0; // CurrencyType::None
		}
	}
	// 닫혀있을 때
	else
	{
		_bIsCurrencyInfoOpen = true;
	}
	
	if (_bIsCurrencyInfoOpen)
	{
		_currencyInfo->SetVisibility(ESlateVisibility::Visible);
		_currencyInfo->SetData(static_cast<CurrencyType>(_openedCurrencyInfoType));
	}
	else
	{
		_currencyInfo->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUICurrencyList::OnHideCurrencyInfo()
{
	_bIsCurrencyInfoOpen = false;
	_currencyInfo->SetVisibility(ESlateVisibility::Collapsed);
}
