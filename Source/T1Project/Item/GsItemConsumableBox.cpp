// Fill out your copyright notice in the Description page of Project Settings.

#include "GsItemConsumableBox.h"
#include "T1Project.h"
#include "GsItemManager.h"
#include "SharedEnums/SharedItemEnum.h"
#include "Reward/GsRewardHelper.h"
#include "Currency/GsCurrencyHelper.h"
#include "Management/GsScopeHolder.h"
#include "DataSchema/Reward/GsSchemaRewardData.h"
#include "UI/UIContent/Helper/GsUIStringHelper.h"

FGsItemConsumableBox::FGsItemConsumableBox(const int32 In_ItemDetailID)
	: FGsItemConsumable(In_ItemDetailID)
{
}

FText FGsItemConsumableBox::GetUIContentsText() const
{
	int32 rewardDataId = Super::GetRewardId();
	FString rewardDescString;
	SetRewardDescString(rewardDataId, rewardDescString);
	return FText::FromString(rewardDescString);
}

void FGsItemConsumableBox::SetRewardDescString(RewardId rewardId, OUT FString& OutString)
{
	if (rewardId == 0)
		return;

	const UGsTableRewardData* rewardTableData = Cast<UGsTableRewardData>(FGsSchemaRewardData::GetStaticTable());
	if (nullptr == rewardTableData)
		return;

	const FGsSchemaRewardData* rewardData = nullptr;
	if (false == rewardTableData->FindRowById(rewardId, rewardData))
		return;

	// RewardBox 가 없을 경우 : RewardData 만 출력해야 하는데, 해당 경우는 데이터 오류이다.
	if (0 >= rewardData->rewardBoxList.Num())
	{
		GSLOG(Warning, TEXT("RewardBox is not set(RewardBox data missing)!"));
		return;
	}
	
	// 사실 이건 재화 타입에 대한 정보 수집을 위해 사용하는 녀석이다
	SetRewardDescString(rewardData, OutString);
}

void FGsItemConsumableBox::SetRewardDescString(const FGsSchemaRewardData* rewardData, OUT FString& OutString)
{
	if (nullptr == rewardData)
		return;
	
	if (0 < rewardData->expMax || 0 < rewardData->currencyAmountMax)
	{
		FText rewardDataText;

		if (RewardType::ALL == rewardData->rewardType)
		{
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_RewardBoxDesc_GetAll"), rewardDataText);
		}
		else
		{
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_RewardBoxDesc_Get"), rewardDataText);
		}

		OutString.Append(rewardDataText.ToString());
	}
}

void FGsItemConsumableBox::SetRewardDataString(const FGsSchemaRewardData* rewardData, OUT FString& OutString)
{
	// rewardData 에 줄 게 없을 경우 바로 리턴
	if (OutString.IsEmpty())
		return;

	// 구분자 정의
	// ' - '
	FText seperator;
	FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_RewardBoxDesc_Separator"), seperator);

	// ' ~ '
	FText tilde;
	FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_RewardBoxDesc_Tilde"), tilde);

	// 경험치 세팅
	if (0 < rewardData->expMin)
	{
		// 한 줄 띄고 시작
		OutString.Append("\n");

		OutString.Append(seperator.ToString());

		FText expText;
		FText::FindText(TEXT("UICommonText"), TEXT("EXP"), expText);
		OutString.Append(expText.ToString());
		OutString.Append(" ");

		FGsUIStringHelper::GetItemAmountShortText(rewardData->expMin, expText);
		OutString.Append(expText.ToString());

		if (rewardData->expMin != rewardData->expMax)
		{
			OutString.Append(tilde.ToString());

			FGsUIStringHelper::GetItemAmountShortText(rewardData->expMax, expText);
			OutString.Append(expText.ToString());
		}
	}

	// 재화 세팅
	if (CurrencyType::NONE != rewardData->currencyType)
	{
		// 한 줄 띄고 시작
		OutString.Append("\n");

		OutString.Append(seperator.ToString());

		FText currencyText;

		OutString.Append(FGsCurrencyHelper::GetCurrencyLocalizedText(rewardData->currencyType).ToString());
		OutString.Append(" ");

		// [B1] | ejrrb10 | C2URWQ-5332 | 다이아 타입의 경우, 일의 자리까지 표시하도록 Quick Fix
		if (CurrencyType::DIA == rewardData->currencyType ||
			CurrencyType::FREE_DIA == rewardData->currencyType)
		{
			FGsUIStringHelper::GetAmountText(rewardData->currencyAmountMin, UINT64_MAX, currencyText);
		}
		else
		{
			FGsUIStringHelper::GetItemAmountShortText(rewardData->currencyAmountMin, currencyText);
		}

		OutString.Append(currencyText.ToString());

		if (rewardData->currencyAmountMin != rewardData->currencyAmountMax)
		{
			OutString.Append(tilde.ToString());

			if (CurrencyType::DIA == rewardData->currencyType ||
				CurrencyType::FREE_DIA == rewardData->currencyType)
			{
				FGsUIStringHelper::GetAmountText(rewardData->currencyAmountMax, UINT64_MAX, currencyText);
			}
			else
			{
				FGsUIStringHelper::GetItemAmountShortText(rewardData->currencyAmountMax, currencyText);
			}

			OutString.Append(currencyText.ToString());
		}
	}
}

void FGsItemConsumableBox::SetRewardBoxDescString(RewardType rewardType, OUT FString& OutString)
{
	FText rewardBoxDescText;

	switch (rewardType)
	{
	case RewardType::ALL:
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_RewardBoxDesc_GetAll"), rewardBoxDescText);
		break;
	case RewardType::PICK_ONE:
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_RewardBoxDesc_GetPickOne"), rewardBoxDescText);
		break;
	case RewardType::RANDOM_ONE:
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_RewardBoxDesc_GetRandomOne"), rewardBoxDescText);
		break;
	case RewardType::RANDOM_GROUP:
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_RewardBoxDesc_GetRandomGroup"), rewardBoxDescText);
		break;
	}

	// RewardType::ALL 일 경우, 이전에 설명이 없을 경우에만 RewardBox 에 대한 설명을 추가한다.
	if (RewardType::ALL == rewardType)
	{
		if (!OutString.IsEmpty())
			return;
	}
	else
	{
		// 두 줄 띄고 시작 (RewardType::ALL 이 아니고, 이전에 기록된 텍스트가 있을 경우에만)
		if (!OutString.IsEmpty())
		{
			OutString.Append("\n\n");
		}
	}

	OutString.Append(rewardBoxDescText.ToString());
}


void FGsItemConsumableBox::SetRewardBoxDataString(TArray<const FGsSchemaRewardItemBagData*>& itemBagDataList, OUT FString& OutString)
{
	// 구분자 정의
	// ' - '
	FText seperator;
	FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_RewardBoxDesc_Separator"), seperator);

	// ' 및 '
	FText coupler;
	FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_RewardBoxDesc_Coupler"), coupler);

	// ' 개 '
	FText unit;
	FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_RewardBoxDesc_Unit"), unit);

	// ' ~ '
	FText tilde;
	FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_RewardBoxDesc_Tilde"), tilde);

	// ' + '
	FText plus;
	FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_RewardBoxDesc_Plus"), plus);


	// 아이템 정렬 (재화 -> 메인 카테고리 -> 서브 카테고리 -> 등급 -> ID)
	SortBoxItems(itemBagDataList);


	for (const FGsSchemaRewardItemBagData* itemBagData : itemBagDataList)
	{
		// 빈 가방은 건너뛰기
		if (CurrencyType::NONE == itemBagData->currencyType && 0 == itemBagData->itemId)
			continue;

		// 한 줄 띄고 시작
		OutString.Append("\n");

		// 한줄씩 출력 (구분자 유무 결정 필요)
		OutString.Append(seperator.ToString());

		// 재화 타입 스트링 채워넣기
		if (CurrencyType::NONE != itemBagData->currencyType)
		{
			FText currencyAmountText;
			OutString.Append(FGsCurrencyHelper::GetCurrencyLocalizedText(itemBagData->currencyType).ToString());
			OutString.Append(" ");

			// [B1] | ejrrb10 | C2URWQ-5332 | 다이아 타입의 경우, 일의 자리까지 표시하도록 Quick Fix
			if (CurrencyType::DIA == itemBagData->currencyType || 
				CurrencyType::FREE_DIA == itemBagData->currencyType)
			{
				FGsUIStringHelper::GetAmountText(itemBagData->currencyAmountMin, UINT64_MAX ,currencyAmountText);
			}
			else
			{
				FGsUIStringHelper::GetItemAmountShortText(itemBagData->currencyAmountMin, currencyAmountText);
			}
			OutString.Append(currencyAmountText.ToString());

			if (itemBagData->currencyAmountMin != itemBagData->currencyAmountMax)
			{
				OutString.Append(tilde.ToString());

				if (CurrencyType::DIA == itemBagData->currencyType ||
					CurrencyType::FREE_DIA == itemBagData->currencyType)
				{
					FGsUIStringHelper::GetAmountText(itemBagData->currencyAmountMax, UINT64_MAX, currencyAmountText);
				}
				else
				{
					FGsUIStringHelper::GetItemAmountShortText(itemBagData->currencyAmountMax, currencyAmountText);
				}
				OutString.Append(currencyAmountText.ToString());
			}
		}

		// 아이템 타입 스트링 채워넣기
		if (0 != itemBagData->itemId)
		{
			// 만약 재화가 있었다면 coupler 삽입
			if (CurrencyType::NONE != itemBagData->currencyType)
			{
				OutString.Append(coupler.ToString());
			}

			// 아이템 강화단계
			if (0 < itemBagData->itemLevel)
			{
				OutString.Append(plus.ToString());
				OutString.Append(FString::FromInt(itemBagData->itemLevel));
				OutString.Append(" ");
			}

			// 아이템 이름 출력
			OutString.Append(GetItemNameFromItemId(itemBagData->itemId).ToString());
			OutString.Append(" ");

			// 아이템 갯수 출력
			OutString.Append(FString::FromInt(itemBagData->itemCountMin));

			if (itemBagData->itemCountMin != itemBagData->itemCountMax)
			{
				OutString.Append(tilde.ToString());
				OutString.Append(FString::FromInt(itemBagData->itemCountMax));
			}

			OutString.Append(unit.ToString());
		}
	}
}

void FGsItemConsumableBox::SetRewardBoxDataToCurrencyItemIconData(TArray<const FGsSchemaRewardItemBagData*>& itemBagDataList, OUT TArray<FGsCurrencyItemIconData>& currencyItemIconDataList)
{
	currencyItemIconDataList.Empty();

	// 아이템 정렬 (재화 -> 메인 카테고리 -> 서브 카테고리 -> 등급 -> ID)
	SortBoxItems(itemBagDataList);

	for (const FGsSchemaRewardItemBagData* itemBagData : itemBagDataList)
	{
		// 빈 가방은 건너뛰기
		if (CurrencyType::NONE == itemBagData->currencyType && 0 == itemBagData->itemId)
			continue;

		// 재화 타입 채워넣기
		if (CurrencyType::NONE != itemBagData->currencyType)
		{
			// 새로 추가할 재화 정보
			FGsCurrencyItemIconData currencyData;

			currencyData._costType = CostType::CURRENCY;
			currencyData._currencyType = itemBagData->currencyType;

			currencyData._amountMin = itemBagData->currencyAmountMin;
			currencyData._amountMax = itemBagData->currencyAmountMax;
	
			currencyItemIconDataList.Emplace(MoveTemp(currencyData));
		}

		// 아이템 타입 채워넣기
		if (INVALID_ITEM_ID != itemBagData->itemId)
		{
			// 새로 추가할 아이템 정보
			FGsCurrencyItemIconData itemData;

			itemData._costType = CostType::ITEM;
			itemData._itemLevel = itemBagData->itemLevel;
			itemData._itemId = itemBagData->itemId;
			
			itemData._amountMin = itemBagData->itemCountMin;
			itemData._amountMax = itemBagData->itemCountMax;

			currencyItemIconDataList.Emplace(MoveTemp(itemData));
		}
	}
}

FText FGsItemConsumableBox::GetItemNameFromItemId(int32 InItemId)
{
	// 아이템 데이터 테이블 불러오기
	const UGsTableItemCommon* table = Cast<UGsTableItemCommon>(FGsSchemaItemCommon::GetStaticTable());
	if (nullptr == table)
	{
		GSLOG(Error, TEXT("Fail to get Item Table"));
		return FText::GetEmpty();
	}
		

	const FGsSchemaItemCommon* item = nullptr;
	if (false == table->FindRowById(InItemId, item))
	{
		GSLOG(Error, TEXT("Fail to get Item Data. ItemId : %d"), InItemId);
		return FText::GetEmpty();
	}

	return item->name;
}


TArray<TPair<FString, TArray<FGsCurrencyItemIconData>>> FGsItemConsumableBox::GetRewardDescItemsPairList(RewardId InRewardId)
{
	if (InRewardId == 0)
	{
		return TArray<TPair<FString, TArray<FGsCurrencyItemIconData>>>();
	}

	const UGsTableRewardData* rewardTableData = Cast<UGsTableRewardData>(FGsSchemaRewardData::GetStaticTable());
	if (nullptr == rewardTableData)
	{
		return TArray<TPair<FString, TArray<FGsCurrencyItemIconData>>>();
	}
	const FGsSchemaRewardData* rewardData = nullptr;
	if (false == rewardTableData->FindRowById(InRewardId, rewardData))
	{
		return TArray<TPair<FString, TArray<FGsCurrencyItemIconData>>>();
	}

	// RewardBox 가 없을 경우 : RewardData 만 출력해야 하는데, 해당 경우는 데이터 오류이다.
	if (0 >= rewardData->rewardBoxList.Num())
	{
		GSLOG(Warning, TEXT("RewardBox is not set(RewardBox data missing)!"));
		return TArray<TPair<FString, TArray<FGsCurrencyItemIconData>>>();
	}

	// 각 박스의 설명 텍스트 및 박스별 재화/아이템 목록
	TArray<TPair<FString, TArray<FGsCurrencyItemIconData>>> descItemsPairList;

	// RewardBox 의 재화 및 아이템 텍스트 채우기
	// RANDOM_GROUP 일 경우, 모든 아이템을 하나의 박스처럼 취급하여 합쳐준다
	if (RewardType::RANDOM_GROUP == rewardData->rewardType)
	{
		// '내용물' 슬롯의 각 리워드 박스별로 들어갈 텍스트
		FString strContents;
		SetRewardBoxDescString(rewardData->rewardType, strContents);

		TArray<const FGsSchemaRewardItemBagData*> itemBagDataList;
		for (const FGsSchemaRewardBox& rewardBox : rewardData->rewardBoxList)
		{
			if (rewardBox.rewardBoxId == 0 /*|| rewardBox.rate == 0*/)
				continue;
			
			FGsRewardHelper::UnboxingRewardBox(rewardBox.rewardBoxId, itemBagDataList);
		}
		TArray<FGsCurrencyItemIconData> currencyItemIconDataList;
		SetRewardBoxDataToCurrencyItemIconData(itemBagDataList, currencyItemIconDataList);

		descItemsPairList.Emplace(MakeTuple(strContents, currencyItemIconDataList));
	}
	else
	{
		for (const FGsSchemaRewardBox& rewardBox : rewardData->rewardBoxList)
		{
			if (rewardBox.rewardBoxId == 0 /*|| rewardBox.rate == 0*/)
				continue;
	
			// '내용물' 슬롯의 각 리워드 박스별로 들어갈 텍스트
			FString strContents;

			SetRewardBoxDescString(rewardData->rewardType, strContents);

			TArray<const FGsSchemaRewardItemBagData*> itemBagDataList;
			FGsRewardHelper::UnboxingRewardBox(rewardBox.rewardBoxId, itemBagDataList);

			TArray<FGsCurrencyItemIconData> currencyItemIconDataList;
			SetRewardBoxDataToCurrencyItemIconData(itemBagDataList, currencyItemIconDataList);

			descItemsPairList.Emplace(MakeTuple(strContents, currencyItemIconDataList));
		}
	}

	return descItemsPairList;
}

// TArray<TWeakPtr<FGsItem>> 대신 TArray<const FGsSchemaItemCommon*> 을 사용한 이유는 
// Sort 함수가 템플릿 수준에서 TweakPtr 변환을 지원하지 않기 때문이다.
void FGsItemConsumableBox::SortBoxItems(OUT TArray<const FGsSchemaRewardItemBagData*>& OutItemBagDataList)
{
	if (OutItemBagDataList.Num() > 1)
	{
		// 아이템 데이터 테이블 불러오기 (최적화를 위해 람다에서 캡쳐하여 사용
		const UGsTableItemCommon* table = Cast<UGsTableItemCommon>(FGsSchemaItemCommon::GetStaticTable());
		if (nullptr == table)
			return;

		OutItemBagDataList.Sort([&table](const FGsSchemaRewardItemBagData& itemBagA, const FGsSchemaRewardItemBagData& itemBagB)
			{
				// 재화 타입 비교 시작
				int32 diff;

				// 재화 종류
				diff = (uint8)itemBagB.currencyType - (uint8)itemBagA.currencyType;
				if (0 != diff)
				{
					// 재화 타입이 없으면 아이템인데, 재화가 먼저 나와야 한다. 
					// NONE 이 MAX 역할을 해야 하는데, 반대라서 발생한 문제로 예외처리
					if (itemBagA.currencyType == CurrencyType::NONE)
						return false;
					if (itemBagB.currencyType == CurrencyType::NONE)
						return true;

					return (uint8)itemBagA.currencyType < (uint8)itemBagB.currencyType;
				}

				// 재화 수량 MAX
				diff = itemBagB.currencyAmountMax - itemBagA.currencyAmountMax;
				if (0 != diff)
				{
					return itemBagA.currencyAmountMax > itemBagB.currencyAmountMax;
				}

				// 재화 수량 MIN
				diff = itemBagB.currencyAmountMin - itemBagA.currencyAmountMin;
				if (0 != diff)
				{
					return itemBagA.currencyAmountMin > itemBagB.currencyAmountMin;
				}

				// 아이템 포함 여부
				bool itemIncludedA = (0 == itemBagA.itemId) ? false : true;
				bool itemIncludedB = (0 == itemBagB.itemId) ? false : true;
				if (itemIncludedA != itemIncludedB)
				{
					return itemIncludedA ? false : true;
				}

				// 둘 다 아이템 없으면 리턴
				if (!itemIncludedA && !itemIncludedB)
					return true;

				// 아이템 타입 비교 시작
				const FGsSchemaItemCommon* itemA = nullptr;
				if (false == table->FindRowById(itemBagA.itemId, itemA))
				{
					GSLOG(Error, TEXT("Fail to get Item Data. ItemBagId : %d"), itemBagA.itemBagPairId);
					return true;
				}

				const FGsSchemaItemCommon* itemB = nullptr;
				if (false == table->FindRowById(itemBagB.itemId, itemB))
				{
					GSLOG(Error, TEXT("Fail to get Item Data. ItemBagId : %d"), itemBagB.itemBagPairId);
					return true;
				}
			
				// 아이템 타입
				diff = (uint8)itemB->type - (uint8)itemA->type;
				if (0 != diff)
				{
					return (uint8)itemA->type < (uint8)itemB->type;
				}

				// 아이템 카테고리
				diff = (uint8)itemB->categoryMain - (uint8)itemA->categoryMain;
				if (0 != diff)
				{
					return (uint8)itemA->categoryMain < (uint8)itemB->categoryMain;
				}

				// 아이템 세부 카테고리
				diff = (uint8)itemB->categorySub - (uint8)itemA->categorySub;
				if (0 != diff)
				{
					return (uint8)itemA->categorySub < (uint8)itemB->categorySub;
				}

				// 아이템 등급
				diff = (uint8)itemB->grade - (uint8)itemA->grade;
				if (0 != diff)
				{
					return (uint8)itemA->grade > (uint8)itemB->grade;
				}

				// 아이템 TID
				diff = itemB->id - itemA->id;
				if (0 != diff)
				{
					return itemA->id < itemB->id;
				}

				// 아이템 강화 수치
				diff = (uint8)itemBagB.itemLevel - (uint8)itemBagA.itemLevel;
				if (0 != diff)
				{
					return (uint8)itemBagA.itemLevel < (uint8)itemBagB.itemLevel;
				}

				// 아이템 최대 갯수
				diff = itemBagB.itemCountMax - itemBagA.itemCountMax;
				if (0 != diff)
				{
					return itemBagA.itemCountMax > itemBagB.itemCountMax;
				}

				// 아이템 최소 갯수
				diff = itemBagB.itemCountMin - itemBagA.itemCountMin;
				if (0 != diff)
				{
					return itemBagA.itemCountMin > itemBagB.itemCountMin;
				}

				// 유효 기간
				return itemA->expiredTimeId > itemB->expiredTimeId;
			}
		);
	}
}

FText FGsItemConsumableBox::GetRewardDataTextFromId(RewardId InId)
{
	if (InId == 0)
	{
		return FText::GetEmpty();
	}

	const UGsTableRewardData* rewardTableData = Cast<UGsTableRewardData>(FGsSchemaRewardData::GetStaticTable());
	if (nullptr == rewardTableData)
	{
		return FText::GetEmpty();
	}
	const FGsSchemaRewardData* rewardData = nullptr;
	if (false == rewardTableData->FindRowById(InId, rewardData))
	{
		return FText::GetEmpty();
	}

	// RewardBox 가 없을 경우 : RewardData 만 출력해야 하는데, 해당 경우는 데이터 오류이다.
	if (0 >= rewardData->rewardBoxList.Num())
	{
		GSLOG(Warning, TEXT("RewardBox is not set(RewardBox data missing)!"));
		return FText::GetEmpty();
	}

	// '내용물' 슬롯에 들어갈 텍스트
	FString strContents;

	// RewardData 에 대한 타입별 설명 텍스트 채우기
	SetRewardDescString(rewardData, strContents);

	// RewardBox 의 재화 및 아이템 텍스트 채우기
	for (const FGsSchemaRewardBox& rewardBox : rewardData->rewardBoxList)
	{
		if (rewardBox.rewardBoxId == 0 /*|| rewardBox.rate == 0*/)
			continue;

		SetRewardBoxDescString(rewardData->rewardType, strContents);

		TArray<const FGsSchemaRewardItemBagData*> itemBagDataList;
		FGsRewardHelper::UnboxingRewardBox(rewardBox.rewardBoxId, itemBagDataList);

		SetRewardBoxDataString(itemBagDataList, strContents);
	}

	return FText::FromString(strContents);
}
