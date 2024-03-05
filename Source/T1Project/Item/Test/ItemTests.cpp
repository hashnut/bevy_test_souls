#include "Runtime/Core/Public/Misc/AutomationTest.h"

#include "Currency/GsSortCost.h"
#include "Item/GsItem.h"
#include "Item/GsItemHelper.h"
#include "Item/GsItemManager.h"
#include "Management/ScopeGame/GsValidatorChainManager.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
/**
 * Automation Test 가 유효할 때만 빌드에 포함. 에디터/런타임 환경에서 테스트 실행.
 */

#if WITH_DEV_AUTOMATION_TESTS

#define NS_TEST "Raon.아이템."

////////////////////////////////////////////////////////////////////////// tests

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGsItemDisposalTest, TEXT(NS_TEST "01 Sort Item Disposal")
	, EAutomationTestFlags::EditorContext
	| EAutomationTestFlags::MediumPriorityAndAbove
	| EAutomationTestFlags::EngineFilter)
bool FGsItemDisposalTest::RunTest(const FString& parameters)
{
	struct FGsItemInfoTest
	{
		ItemId _itemId;
		ItemAmount _amount;
	};

	// 준비
	const TArray<FGsItemInfoTest> ExpiredItemDatas = {
		{800001001, 100}, // 23.12.31
		{800003001, 10}, // 24.12.31
		{102443001, 1000} // 30.04.10
	};
	const TArray<FGsItemInfoTest> TradableItemDatas = { {100111001, 9}, {100111002, 101}, {100111003, 1001} };
	const TArray<FGsItemInfoTest> NonTradableItemDatas = { {100121001, 11}, {100121002, 99}, {100121003, 999} };
	TArray<FGsItemInfoTest> AllItemDatas;
	AllItemDatas.Append(ExpiredItemDatas);
	AllItemDatas.Append(TradableItemDatas);
	AllItemDatas.Append(NonTradableItemDatas);
	
	TArray<TSharedPtr<FGsItem>> ItemContainer;
	TArray<TWeakPtr<FGsItem>> SortTargetItemsA;
	TArray<TWeakPtr<FGsItem>> SortTargetItemsB;
	for (const FGsItemInfoTest& ItemData : AllItemDatas)
	{
		TSharedPtr<FGsItem> Item = MakeShareable(FGsItem::Create(ItemData._itemId, ItemData._amount));
		ItemContainer.Emplace(Item);

		SortTargetItemsA.Emplace(Item);
		SortTargetItemsB.Emplace(Item);
	}

	// 섞기
	SortTargetItemsA.Sort([](const TWeakPtr<FGsItem>& InA, const TWeakPtr<FGsItem>& InB)
	{
		return FMath::RandBool();
	});

	SortTargetItemsB.Sort([](const TWeakPtr<FGsItem>& InA, const TWeakPtr<FGsItem>& InB)
	{
		return FMath::RandBool();
	});

	for (int32 i = 0; i < SortTargetItemsA.Num(); ++i)
	{
		AddInfo(FString::Printf(TEXT("%d A:%d B:%d"), i, SortTargetItemsA[i].Pin()->GetTID(), SortTargetItemsB[i].Pin()->GetTID()));
	}

	// 실행
	SortTargetItemsA.StableSort(FGsComparerItemIdDisposalEfficiency(ECachedValueItemDelegateType::DEFAULT));
	SortTargetItemsB.StableSort(FGsComparerItemIdDisposalEfficiency(ECachedValueItemDelegateType::DEFAULT));

	// 확인
	for (int32 i = 0; i < SortTargetItemsA.Num(); ++i)
	{
		const FGsItem& ItemA = *SortTargetItemsA[i].Pin();
		const FGsItem& ItemB = *SortTargetItemsB[i].Pin();

		FTimespan RemainTimaA(0), RemainTimaB(0);
		ItemA.GetExpiredRemainTime(RemainTimaA);
		ItemB.GetExpiredRemainTime(RemainTimaB);
		AddInfo(FString::Printf(TEXT("%d A:%d(e:%lld, t:%d) B:%d(t:%lld, t:%d)"), i, ItemA.GetTID(), RemainTimaA.GetTicks(), ItemA.IsTradable(), ItemB.GetTID(), RemainTimaB.GetTicks(), ItemB.IsTradable()));
		TestEqual(TEXT("SortTargetItemsA[i].Pin()->GetTID(), SortTargetItemsB[i].Pin()->GetTID()"), SortTargetItemsA[i].Pin()->GetTID(), SortTargetItemsB[i].Pin()->GetTID());
	}

	TestEqual(TEXT("SortTargetItemsA[0].Pin()->GetTID(), ExpiredItemDatas[2]._itemId"), SortTargetItemsA[0].Pin()->GetTID(), ExpiredItemDatas[0]._itemId);
	TestEqual(TEXT("SortTargetItemsA[3].Pin()->GetTID(), NonTradableItemDatas[0]._itemId"), SortTargetItemsA[3].Pin()->GetTID(), NonTradableItemDatas[0]._itemId);
	TestEqual(TEXT("SortTargetItemsA[8].Pin()->GetTID(), TradableItemDatas[2]._itemId"), SortTargetItemsA[8].Pin()->GetTID(), TradableItemDatas[2]._itemId);

	return true;
}

#endif
