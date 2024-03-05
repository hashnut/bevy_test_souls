#include "GsGameStateStarLegacy.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsUIManager.h"

#include "Management/ScopeGame/GsStarLegacyManager.h"
#include "Management/ScopeGame/GsFairyManager.h"

#include "Item/GsItemManager.h"
#include "Item/GsItem.h"

#include "Message/GsMessageUI.h"
#include "Message/MessageParam/GsUIMessageParam.h"

#include "Fairy/GsFairyData.h"

#include "Currency/GsCostPackage.h"

#include "GameFlow/GameContents/GsContentsMode.h"

#include "UI/UIContent/Window/GsUIWindowStarLegacy.h"

#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedFairyEnum.h"

#include "Runtime/DataCenter/Public/DataSchema/StarLegacy/GsSchemaStarLegacyCategory.h"

FGsGameStateStarLegacy::FGsGameStateStarLegacy() : FGsContentsGameBase(FGsContentsMode::InGame::ContentsStarLegacy)
{

}

// 등급 오픈 처리
void FGsGameStateStarLegacy::OpenMagnitude(int In_slotNum)
{
	// 현재 open 갯수
	int currentOpenCount = GSStarLegacy()->GetCurrentOpenMagnitudeCount();

	// 현재 최고 등급 페어리 찾기
	TArray<TWeakPtr<FGsFairyData>> findList;
	GSFairy()->GetGradeFairyDictionary(FairyGrade::MAX, findList, false);
	// 찾았는데 없으면 return
	if (findList.Num() == 0)
	{
		return;
	}	
	FairyGrade highestGrade = findList[0].Pin()->GetFairyGrade();

	// 현재 최대 열수있는 횟수
	int currentMaxOpenCount = GSStarLegacy()->GetMaxMagnitudeOpenCount(highestGrade);

	// 최대치 도달했다
	if (currentMaxOpenCount == currentOpenCount)
	{
		return;
	}

	// 추가
	GSStarLegacy()->SetCurrentOpenMagnitudeCount( ++currentOpenCount);
	// 해당 슬롯 갱신
	FGsUIMsgParamInt param(In_slotNum);
	GMessage()->GetUI().SendMessage(MessageUI::CHANGED_STAR_LEGACY_MAGNITUDE_OPEN_INFO, &param);
}

// 가상 함수
void FGsGameStateStarLegacy::Enter()
{
	Super::Enter();
	// 초기화
	GSStarLegacy()->InitializeData();
	
	if (UGsUIManager* uiManager = GUI())
	{
		auto widget = uiManager->OpenAndGetWidget(TEXT("WindowStarLegacy"));
		if (widget.IsValid())
		{
			_windowUI = Cast<UGsUIWindowStarLegacy>(widget);
			_windowUI->SetStarLegacyInfo();
		}
	}
	
}
void FGsGameStateStarLegacy::Exit()
{
	_windowUI = nullptr;
	Super::Exit();
}

// 등급 오픈 남은 갯수
int FGsGameStateStarLegacy::GetMagnitudeOpenRemainCount()
{
	// 현재 open 갯수
	int currentOpenCount = GSStarLegacy()->GetCurrentOpenMagnitudeCount();

	// 현재 최고 등급 페어리 찾기
	TArray<TWeakPtr<FGsFairyData>> findList;
	GSFairy()->GetGradeFairyDictionary(FairyGrade::MAX, findList, false);
	// 찾았는데 없으면 return
	if (findList.Num() == 0)
	{
		return -1;
	}
	FairyGrade highestGrade = findList[0].Pin()->GetFairyGrade();

	// 현재 최대 열수있는 횟수
	int currentMaxOpenCount = GSStarLegacy()->GetMaxMagnitudeOpenCount(highestGrade);

	int remainCount = currentMaxOpenCount - currentOpenCount;
	return remainCount;
}

// 슬롯 선택 처리
void FGsGameStateStarLegacy::SelectSlot(int In_slotNum)
{
	// 이미 선택되었으면 해제
	bool isSelected = GSStarLegacy()->IsSelectedId(In_slotNum);
	if (isSelected == true)
	{
		// 삭제
		GSStarLegacy()->DelSelectSlotId(In_slotNum);

		// 갱신 요청
		FGsUIMsgParamInt param(In_slotNum);
		GMessage()->GetUI().SendMessage(MessageUI::CHANGED_STAR_LEGACY_SLOT_RELEASE_INFO, &param);
	}
	else
	{
		// 현재 카테고리에서 최대 선택갯수를 넘었는지 체크
		int currentSlotCount = GSStarLegacy()->GetSelectSlotCount();
		const FGsSchemaStarLegacyCategory* categoryTbl = GSStarLegacy()->GetCurrentCategoryTbl();
		if (categoryTbl == nullptr)
		{
			return;
		}

		int maxCount = categoryTbl->selectMaxCount;

		if (currentSlotCount >= maxCount)
		{
			return;
		}

		// 추가
		GSStarLegacy()->AddSelectSlotId(In_slotNum);

		// 갱신 요청
		FGsUIMsgParamInt param(In_slotNum);
		GMessage()->GetUI().SendMessage(MessageUI::CHANGED_STAR_LEGACY_SLOT_SELECT_INFO, &param);
	}
}
// 별의 유산 소환 시작
void FGsGameStateStarLegacy::StartStarLegacySummon()
{
	// 선택된 슬롯이 없으면 return
	TArray<int> selecteSlots = GSStarLegacy()->GetArraySelectSlotIds();
	int selectCount = selecteSlots.Num();
	if (selectCount == 0)
	{
		return;
	}

	const FGsSchemaStarLegacyCategory* categoryTbl = GSStarLegacy()->GetCurrentCategoryTbl();
	if (categoryTbl == nullptr)
	{
		return;
	}

	// 1개 이상 선택시 bm 아이템 갯수 체크
	if(selectCount >1)
	{ 
		// 추가 선택시 bm 아이템 충분한지 체크

		// 현재 가지고 있는 bm 아이템 갯수
		int currentBMItemCount = FindBMItemCount();

		// bm 필요한 선택 갯수는 1 뺌(1개는 기본 선택)
		int bmNeedSelectCount = selectCount -1;
		// bm 아이템 갯수 선택하나당 
		int bmItemCountPerSelect = categoryTbl->changeItemCount;
		// 필요한 bm 아이템 갯수
		int needBmItemCount = bmNeedSelectCount * bmItemCountPerSelect;
		// bm 아이템 갯수 모자름
		if (currentBMItemCount < needBmItemCount)
		{
			return;
		}
	}

	// 골드 충분한지 체크
	TSharedRef<FGsCostPackage> CostPackage(MakeShared<FGsCostPackage>());

	// 현재 소환 골드 접근
	int summonGold = categoryTbl->rouletteGold;
	CostPackage->AddCost(CurrencyType::GOLD, summonGold);

	const FGsCostElement* firstCostElement = CostPackage->GetFirstCostElement();
	if (firstCostElement == nullptr)
	{
		return;
	}
	// 골드 부족
	if (!firstCostElement->PlayerHasEnough())
	{
		return;
	}

	// 룰렛 연출 시작
	GMessage()->GetUI().SendMessage(MessageUI::START_STAR_LEGACY_ROULETTE, nullptr);
}

// bm 아이템 갯수 구하기
int FGsGameStateStarLegacy::FindBMItemCount()
{
	// 추가 선택시 bm 아이템 충분한지 체크
	const FGsSchemaStarLegacyCategory* categoryTbl = GSStarLegacy()->GetCurrentCategoryTbl();
	if (categoryTbl == nullptr)
	{
		return -1;
	}

	int bmItemId = categoryTbl->chanceItemId;
	TArray<TWeakPtr<FGsItem>> findItems = GItem()->FindByTID(bmItemId);

	int totalCount = 0;
	if (findItems.Num() != 0)
	{
		for (const TWeakPtr<FGsItem> iter : findItems)
		{
			if (false == iter.IsValid())
			{
				continue;
			}
			totalCount += iter.Pin()->GetAmount();
		}
	}

	return totalCount;
}
// 슬롯 갱신 요청
void FGsGameStateStarLegacy::ReqReroll()
{
	const FGsSchemaStarLegacyCategory* categoryTbl = GSStarLegacy()->GetCurrentCategoryTbl();
	if (categoryTbl == nullptr)
	{
		return;
	}

	// 골드 충분한지 체크
	TSharedRef<FGsCostPackage> CostPackage(MakeShared<FGsCostPackage>());

	// 현재 변경 골드 접근
	int rerollGold = categoryTbl->shuffleGold;
	CostPackage->AddCost(CurrencyType::GOLD, rerollGold);

	const FGsCostElement* firstCostElement = CostPackage->GetFirstCostElement();
	if (firstCostElement == nullptr)
	{
		return;
	}
	// 골드 부족
	if (!firstCostElement->PlayerHasEnough())
	{
		return;
	}

	// 슬롯 정보 변경
	GSStarLegacy()->InitializeReroll();

	GMessage()->GetUI().SendMessage(MessageUI::REROLL_STAR_LEGACY, nullptr);
}

// 카테고리 탭 변경
void FGsGameStateStarLegacy::ChangedCategoryTab(int In_tabIndex)
{
	TArray<int> arrayCategoryIds = GSStarLegacy()->GetArrayCategoryIds(); 
	if (arrayCategoryIds.Num() <= In_tabIndex)
	{
		return;
	}
	// 바뀐 카테고리 id 저장
	int categoryId = arrayCategoryIds[In_tabIndex];
	GSStarLegacy()->SetCurrentCategoryId(categoryId);
	// 오픈 정보 초기화
	GSStarLegacy()->InitOpenInfo();

	GMessage()->GetUI().SendMessage(MessageUI::CHANGED_STAR_LEGACY_CATEGORY_TAB, nullptr);
}