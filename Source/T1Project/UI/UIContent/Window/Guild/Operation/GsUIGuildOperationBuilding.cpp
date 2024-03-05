// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildOperationBuilding.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/PanelWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Window/Guild/Operation/GsUIGuildBuildingListEntry.h"
#include "DataSchema/Guild/GsSchemaGuildBuilding.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Guild/GsGuildHelper.h"
#include "Currency/GsCurrencyHelper.h"


void UGsUIGuildOperationBuilding::BeginDestroy()
{
	if (_slotHelper)
	{
		_slotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIGuildOperationBuilding::OnRefreshEntry);
	}
	_slotHelper = nullptr;

	Super::BeginDestroy();
}

void UGsUIGuildOperationBuilding::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_slotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelper->Initialize(_entryWidgetClass, _panelGrid);
	_slotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIGuildOperationBuilding::OnRefreshEntry);
}

void UGsUIGuildOperationBuilding::NativeDestruct()
{
	_bIsAuthority = false;
	_bIsMyGuild = false;

	Super::NativeDestruct();
}

void UGsUIGuildOperationBuilding::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	IGsInvalidateUIInterface::CheckInvalidate();
	
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UGsUIGuildOperationBuilding::InvalidateAllInternal()
{
	InvalidateList();
}

void UGsUIGuildOperationBuilding::Show(bool bIsMyGuild, bool IsStackRecoverFlag)
{
	_bIsMyGuild = bIsMyGuild;

	_panelGuildAsset->SetVisibility(bIsMyGuild ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	_bIsAuthority = false;
	if (_bIsMyGuild)
	{
		_bIsAuthority = FGsGuildHelper::CanManageBuilding(GGuild()->GetAuthority());

		SetUIGuildAsset();
	}

	InvalidateAll();
}

void UGsUIGuildOperationBuilding::Hide()
{
	if (FGsGuildManager* guildMgr = GGuild())
	{
		guildMgr->GetRedDotCheckData().UpdateBuildingCheckTime();
		guildMgr->CheckRedDotByType(FGsGuildManager::EGsGuildRedDotType::Buildings);
	}
}

void UGsUIGuildOperationBuilding::OnReceiveBuildingList()
{
	InvalidateAll();
}

void UGsUIGuildOperationBuilding::OnChangeGuildLevel()
{
	// 길드 레벨업일 경우는, 개방 체크때문에 전체갱신 해야함
	_slotHelper->RefreshAll(_dataList.Num());
}

void UGsUIGuildOperationBuilding::OnChangeCurrency(CurrencyType InType)
{
	if (CurrencyType::GUILD_ASSET == InType)
	{
		SetUIGuildAsset();

		// 재화량에 따른 버튼 색상 표시를 위해 전체 갱신을 돌린다
		_slotHelper->RefreshAll(_dataList.Num());
	}
}

void UGsUIGuildOperationBuilding::OnUpdateBuilding(GuildBuildingId InBuildingId)
{
	// 주의: 건물 업그레이드 시 재화를 소진하며 OnChangeGuildAsset 가 불리는데,
	// 버튼의 재화량 체크를 위해 전체 갱신을 하기 때문에 다시 갱신할 필요가 없다.
	// 하지만 재화 없이 갱신할 경우를 대비해 아래 갱신을 다시 진행
	for (int32 i = 0, maxCount = _dataList.Num(); i < maxCount; ++i)
	{
		if (InBuildingId == _dataList[i]->id)
		{
			// 해당 슬롯만 갱신
			_slotHelper->RefreshByIndex(i);

			return;
		}
	}
}

void UGsUIGuildOperationBuilding::OnChangeMyAuthority()
{
	// 권한 갱신
	if (_bIsMyGuild)
	{
		_bIsAuthority = FGsGuildHelper::CanManageBuilding(GGuild()->GetAuthority());
	}

	// 권한을 얻거나 잃었을 경우에 대한 UI 표시를 위해 전체 갱신
	_slotHelper->RefreshAll(_dataList.Num());
}

void UGsUIGuildOperationBuilding::InvalidateList()
{
	_dataList.Empty();
	if (const UGsTable* table = FGsSchemaGuildBuilding::GetStaticTable())
	{
		table->GetAllRows(_dataList);
	}

	_slotHelper->RefreshAll(_dataList.Num());

	_scrollBox->ScrollToStart();
}

void UGsUIGuildOperationBuilding::OnRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	if (false == _dataList.IsValidIndex(InIndex))
	{
		return;
	}

	if (UGsUIGuildBuildingListEntry* entry = Cast<UGsUIGuildBuildingListEntry>(InEntry))
	{
		entry->SetData(_dataList[InIndex], _bIsAuthority, _bIsMyGuild);
	}

	// 두줄 배치
	// 0, 1
	// 2, 3,
	// ...
	const TArray<UPanelSlot*>& slots = _panelGrid->GetSlots();
	if (UPanelSlot* slot = slots[InIndex])
	{
		if (UUniformGridSlot* gridSlot = Cast<UUniformGridSlot>(slot))
		{
			gridSlot->SetRow(FMath::FloorToInt(InIndex / 2));
			gridSlot->SetColumn(InIndex % 2);
			gridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
		}
	}
}

void UGsUIGuildOperationBuilding::SetUIGuildAsset()
{
	FText textAmount = FText::AsNumber(FGsCurrencyHelper::GetCurrencyAmount(CurrencyType::GUILD_ASSET));
	_textBlockGuildAsset->SetText(textAmount);
}
