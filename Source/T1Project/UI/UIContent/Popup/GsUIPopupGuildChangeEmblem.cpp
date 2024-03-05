// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupGuildChangeEmblem.h"
#include "Components/ScrollBox.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Common/GsUICurrencyButton.h"
#include "UI/UIContent/Popup/Guild/GsUIGuildEmblemEntry.h"
#include "Currency/GsCurrencyHelper.h"
#include "DataSchema/Guild/GsSchemaGuildConfig.h"
#include "DataSchema/Guild/GsSchemaGuildEmblem.h"
#include "Guild/GsGuildHelper.h"


void UGsUIPopupGuildChangeEmblem::BeginDestroy()
{
	if (_slotHelper)
	{
		_slotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupGuildChangeEmblem::OnRefreshEntry);
	}
	_slotHelper = nullptr;
	_toggleGroup.Clear();

	Super::BeginDestroy();
}

void UGsUIPopupGuildChangeEmblem::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_slotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelper->Initialize(_entryWidgetClass, _panelScrollRoot);
	_slotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPopupGuildChangeEmblem::OnRefreshEntry);

	_currencyBtnChangeEmblem->OnClickCurrencyButton.BindUObject(this, &UGsUIPopupGuildChangeEmblem::OnClickChangeEmblem);

	_btnChangeEmblem->OnClicked.AddDynamic(this, &UGsUIPopupGuildChangeEmblem::OnClickChangeEmblem);
	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupGuildChangeEmblem::OnClickCancel);

	_toggleGroup.OnSelectChanged.BindUObject(this, &UGsUIPopupGuildChangeEmblem::OnClickListEntry);

	// 한번만 찾아서 둠
	if (const FGsSchemaGuildConfig* configData = FGsGuildHelper::GetGuildConfig())
	{
		_changeEmblemCost = configData->changeEmblemCost;
	}
}

void UGsUIPopupGuildChangeEmblem::NativeConstruct()
{
	Super::NativeConstruct();

	// 테이블에서 아이콘 리스트 가져오기
	_dataList.Empty();	
	if (const UGsTable* table = FGsSchemaGuildEmblem::GetStaticTable())
	{
		table->GetAllRows<FGsSchemaGuildEmblem>(_dataList);
	}

	// 요구레벨이 적은 순으로 소팅
	_dataList.Sort([](const FGsSchemaGuildEmblem& A, const FGsSchemaGuildEmblem& B)
		{
			return (A.requireLevel <= B.requireLevel) ? true : false;
		});

	_scrollBox->ScrollToStart();
}

void UGsUIPopupGuildChangeEmblem::NativeDestruct()
{
	_toggleGroup.Clear();
	_dataList.Empty();
	_selectCallback = nullptr;
	_bIsGuildExist = false;
	_prevEmblemId = 0;

	Super::NativeDestruct();
}

void UGsUIPopupGuildChangeEmblem::OnInputOk()
{
	if (_selectCallback)
	{
		int32 selectedIndex = _toggleGroup.GetSelectedIndex();
		if (_dataList.IsValidIndex(selectedIndex))
		{
			// 바뀌었을때만 호출해준다
			int32 selectedEmblemId = _dataList[selectedIndex]->id;
			if (selectedEmblemId != _prevEmblemId)
			{
				// 길드 있는 상태에서 변경 시에는 재화량 체크
				if (_bIsGuildExist)
				{
					if (false == FGsCurrencyHelper::CheckCurrency(CurrencyType::GUILD_ASSET, _changeEmblemCost, true))
					{
						return;
					}
				}

				_selectCallback(_dataList[selectedIndex]->id);
			}
		}
	}

	Close();
}

void UGsUIPopupGuildChangeEmblem::OnInputCancel()
{
	Close();
}

void UGsUIPopupGuildChangeEmblem::SetData(bool bInIsGuildExist,  int32 InCurrEmblemId, TFunction<void(int32)> InSelectCallback)
{
	_bIsGuildExist = bInIsGuildExist;
	_selectCallback = InSelectCallback;
	_prevEmblemId = InCurrEmblemId;

	if (_bIsGuildExist)
	{
		// 길드 문장 변경창(재화사용)
		_currencyBtnChangeEmblem->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_btnChangeEmblem->SetVisibility(ESlateVisibility::Collapsed);

		// TEXT: 변경
		FText textBtnName;
		FText::FindText(TEXT("UICommonText"), TEXT("DefaultPopupButtonChange"), textBtnName);
		_currencyBtnChangeEmblem->SetButtonName(textBtnName);

		// GUILD_ASSET 추가되면 바꿀 것
		CurrencyType requireType = CurrencyType::GUILD_ASSET;
		_currencyBtnChangeEmblem->SetData(requireType, _changeEmblemCost);
		_currencyBtnChangeEmblem->UpdateAmountTextColorLocalPlayer();
	}
	else
	{
		// 길드 생성 시 문장 변경창(재화 사용하지 않음)
		_currencyBtnChangeEmblem->SetVisibility(ESlateVisibility::Collapsed);
		_btnChangeEmblem->SetVisibility(ESlateVisibility::Visible);
	}

	_slotHelper->RefreshAll(_dataList.Num());

	_toggleGroup.Clear();
	_toggleGroup.AddToggleWidgetByParentPanel(_panelScrollRoot);
	_toggleGroup.SetSelectedIndex(FindEmblemIndex(InCurrEmblemId));
}

int32 UGsUIPopupGuildChangeEmblem::FindEmblemIndex(int32 InEmblemId) const
{
	for (int32 i = 0, maxCount = _dataList.Num(); i < maxCount; ++i)
	{
		if (InEmblemId == _dataList[i]->id)
		{
			return i;
		}
	}

	return 0;
}

void UGsUIPopupGuildChangeEmblem::OnRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	if (false == _dataList.IsValidIndex(InIndex))
	{
		return;
	}

	if (UGsUIGuildEmblemEntry* entry = Cast<UGsUIGuildEmblemEntry>(InEntry))
	{
		entry->SetData(_dataList[InIndex]);
	}
}

void UGsUIPopupGuildChangeEmblem::OnClickListEntry(int32 InIndex)
{
	_toggleGroup.SetSelectedIndex(InIndex);
}

void UGsUIPopupGuildChangeEmblem::OnClickChangeEmblem()
{
	OnInputOk();
}

void UGsUIPopupGuildChangeEmblem::OnClickCancel()
{
	OnInputCancel();
}
