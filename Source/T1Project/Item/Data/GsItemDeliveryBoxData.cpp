// Fill out your copyright notice in the Description page of Project Settings.

#include "GsItemDeliveryBoxData.h"

#include "DataSchema/ContentsEvent/GsSchemaContentsEventDeliveryBox.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsSoundManager.h"

#include "UI/UIContent/Tray/ItemEnchant/GsUITrayEnchantItemWait.h"
#include "UI/UIContent/Tray/GsUITrayRepairWait.h"
#include "UI/UIContent/Tray/ItemEnchant/GsUITrayEnchantResultSuccess.h"

#include "Item/GsItem.h"
#include "Item/GsItemManager.h"
#include "Item/GsItemConsumable.h"


void FGsItemDeliveryBoxData::Initialize()
{
	Reset();
}

void FGsItemDeliveryBoxData::Reset()
{
	_repairableItemList.Empty();
	_recomposableFairyList.Empty();
	_recomposableCostumeList.Empty();
}

void FGsItemDeliveryBoxData::SetEquipData(const TArray<ItemDataCreated>& InItemList)
{
	Reset();
	_repairableItemList = InItemList;
}

void FGsItemDeliveryBoxData::SetFairyData(const TArray<FairyGrade>& InGradeList)
{
	Reset();
	_recomposableFairyList = InGradeList;
	
}

void FGsItemDeliveryBoxData::SetCostumeData(const TArray<CostumeGrade>& InGradeList)
{
	Reset();
	_recomposableCostumeList = InGradeList;
	
}

void FGsItemDeliveryBoxData::SetUsedBoxItemDBId(ItemDBId InItemDBId)
{
	_usedBoxItemDBId = InItemDBId;

	TWeakPtr<FGsItem> findItem = GItem()->FindItem(_usedBoxItemDBId, ItemType::CONSUME);
	if (findItem.IsValid())
	{
		if (FGsItemConsumable* consumeItem = findItem.Pin()->GetCastItem<FGsItemConsumable>())
		{
			TArray<int32> effectIdList;
			consumeItem->FindEffectIdList(effectIdList);

			const UGsTableContentsEventDeliveryBox* table = Cast<UGsTableContentsEventDeliveryBox>(FGsSchemaContentsEventDeliveryBox::GetStaticTable());
			if (nullptr == table)
				return;

			const FGsSchemaContentsEventDeliveryBox* row = nullptr;
			if (table->FindRowById(effectIdList[0], row))
			{
				_minGrade = row->minGrade;
				_maxGrade = row->maxGrade;

				FDateTime::Parse(row->conditionStartTime, _startTime);
				FDateTime::Parse(row->conditionEndTime, _endTime);

				//_repairCategoryMain = row->category;
				_deliveryBoxType = row->type;
			}
		}
	}
}

const TArray<ItemDataCreated>& FGsItemDeliveryBoxData::GetRepairableItemList()
{
	return _repairableItemList;
}

const TArray<FairyGrade>& FGsItemDeliveryBoxData::GetRecomposableFairyList()
{
	return _recomposableFairyList;
}

const TArray<CostumeGrade>& FGsItemDeliveryBoxData::GetRecomposableCostumeList()
{
	return _recomposableCostumeList;
}

void FGsItemDeliveryBoxData::GetFairyGradeRange(OUT FairyGrade& OutMinGrade, OUT FairyGrade& OutMaxGrade)
{
	OutMinGrade = static_cast<FairyGrade>(_minGrade);
	OutMaxGrade = static_cast<FairyGrade>(_maxGrade);
}

void FGsItemDeliveryBoxData::GetCostumeGradeRange(OUT CostumeGrade& OutMinGrade, OUT CostumeGrade& OutMaxGrade)
{
	OutMinGrade = static_cast<CostumeGrade>(_minGrade);
	OutMaxGrade = static_cast<CostumeGrade>(_maxGrade);
}

FDateTime FGsItemDeliveryBoxData::GetStartTime()
{
	return _startTime;
}

FDateTime FGsItemDeliveryBoxData::GetEndTime()
{
	return _endTime;
}

bool FGsItemDeliveryBoxData::IsActiveFairy()
{
	return _recomposableFairyList.Num() > 0;
}

bool FGsItemDeliveryBoxData::IsActiveCostume()
{
	return _recomposableCostumeList.Num() > 0;
}

ItemDBId FGsItemDeliveryBoxData::GetUsedBoxItemDBId()
{
	return _usedBoxItemDBId;
}

void FGsItemDeliveryBoxData::ShowRepairResult(const ItemDataCreated& InItemData)
{
	ItemDataEquip itemEquipData(InItemData);

	UGsUITrayRepairWait* trayWait = Cast<UGsUITrayRepairWait>(GUI()->OpenAndGetWidget(TEXT("TrayRepairWait")));
	if (trayWait)
	{
		trayWait->SetData(1.0f, [this, itemEquipData]()
			{
				StopEffectSound(true);
				PlayEffectSound(false);
				
				UGsUITrayEnchantResultSuccess* result = Cast<UGsUITrayEnchantResultSuccess>(GUI()->OpenAndGetWidget(TEXT("TrayEnchantResultSuccess")));
				if (result)
				{
					//FGsItem* tempItem = FGsItem::Create(&itemEquipData);
					result->SetDisplayItem(itemEquipData.mItemDBId, 3.f);
					result->SetDestroyCallback([this]()
						{
							GUI()->CloseAllStack();
							StopEffectSound(false);
						});

					FText successText;
					FText::FindText(TEXT("DeliveryBoxText"), TEXT("DeliveryBoxText_Equip_Result_Success"), successText);
					result->SetSuccessText(successText);
				}
				GUI()->CloseByKeyName(TEXT("TrayRepairWait"));
			});


		PlayEffectSound(true);
	}
}

void FGsItemDeliveryBoxData::PlayEffectSound(bool InIsWait)
{
	if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
	{
		FString soundRes = (InIsWait) ? TEXT("Enchant_Start") : TEXT("Enchant_NotSafe_Touch");
		uint64 soundID = soundPlayer->PlaySound2D(soundRes);

		if (InIsWait)
		{
			_waitSoundId = soundID;
		}
		else
		{
			_successSoundId = soundID;
		}
	}
}

void FGsItemDeliveryBoxData::StopEffectSound(bool InIsWait)
{
	if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
	{
		uint64 soundID = (InIsWait) ? _waitSoundId : _successSoundId;
		if (soundPlayer->IsPlaying(soundID))
		{
			soundPlayer->StopSound(soundID);
		}
	}
}