#if (WITH_EDITOR) && (WITH_DEV_AUTOMATION_TESTS)

/**
 * Automation Test 가 유효할 때만 빌드에 포함. 에디터/런타임 환경에서 테스트 실행.
 */

#include "Runtime/Core/Public/Misc/AutomationTest.h"
#include "Runtime/Engine/Public/Tests/AutomationCommon.h"

#include "GsSchemaRowBase.h"
#include "GsState.h"
#include "GsTableManager.h"
#include "Item/Collection/GsItemCollectionSlotState.h"
#include "Item/Collection/GsItemCollectionState.h"
#include "Management/ScopeGame/GsItemCollectionManager.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "T1Project.h"

#define NS_TEST_PREFIX "Raon.아이템컬렉션."

void LogAll(FAutomationTestBase& Context, const TArray<const FGsItemCollectionState*>& InItemCollections)
{
	for (int32 i = 0; i < InItemCollections.Num(); ++i)
	{
		const FGsItemCollectionState* ItemCollection = InItemCollections[i];

		Context.TestNotNull(TEXT("ItemCollection"), ItemCollection);
		if (nullptr == ItemCollection)
		{
			continue;
		}

		GSLOG(Log, TEXT("[ItemCollection %d] GetType():%d"), i, ItemCollection->GetType());

		const TArray<StatInfo>& StatInfos = ItemCollection->GetAllStatInfos();
		for (int32 j = 0; j < StatInfos.Num(); ++j)
		{
			const StatInfo& statInfo = StatInfos[j];

			GSLOG(Log, TEXT("[StatInfo %d] mType:%s, mValue:%d"), j, *StatTypeEnumToString(statInfo.mType), statInfo.mValue);
		}

		for (int32 j = 0; j < ItemCollection->GetInnerSlotCount(); ++j)
		{
			const FGsItemCollectionSlotState* SlotState = ItemCollection->GetInnerSlotByIndex(j);

			Context.TestNotNull(TEXT("nullptr == SlotState"), SlotState);
			if (nullptr == SlotState)
			{
				continue;
			}

			TArray<ItemId> innerSlotList;
			/*for (const AvaildCollectionItem& availdItem : SlotState->GetAvaildCollectionItemAll())
			{
				innerSlotList.Emplace(availdItem._itemId);
			}*/

			const FString AvailableItemIds = FString::JoinBy(innerSlotList, TEXT(","), [](int32 Value)
				{
					return FString::Printf(TEXT("%d"), Value);
				});
			GSLOG(Log, TEXT("[Slot %d] AvailableItemIds:%s"), j, *AvailableItemIds);
		}
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGsItemCollectionInitializeTest, TEXT(NS_TEST_PREFIX "01 Item Collection Initialize")
	, EAutomationTestFlags::EditorContext | EAutomationTestFlags::MediumPriorityAndAbove | EAutomationTestFlags::EngineFilter)
bool FGsItemCollectionInitializeTest::RunTest(const FString& parameters)
{
	const FGsItemCollectionManager* Manager = GSItemCollection();
	TestNotNull(TEXT("Manager"), Manager);
	if (nullptr == Manager)
	{
		return false;
	}

	//const TArray<CollectionId> AllItemCollections = Manager->GetValidItemCollections({});
	// LogAll(*this, AllItemCollections);

	return true;
}

#endif
