#include "GsUIObserveTargetListItem.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/Data/GsGameObjectData.h"

#include "Management/ScopeGlobal/GsGameFlowManager.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Observer/GsObserverHandler.h"
#include "GameFlow/GameContents/GsContentsMode.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

void UGsUIObserveTargetListItem::NativeOnInitialized()
{
	_btnItem->OnClicked.AddDynamic(this, &UGsUIObserveTargetListItem::OnClickItem);

	Super::NativeOnInitialized();
}

void UGsUIObserveTargetListItem::OnClickItem()
{
	// 타겟 변경 요청
	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	if (nullptr == gameFlow)
	{
		return;
	}

	FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame();
	if (nullptr == contents)
	{
		return;
	}

	FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
	if (nullptr == hud)
	{
		return;
	}

	FGsObserverHandler* observerHandler = hud->GetObserverHandler();
	if (observerHandler == nullptr)
	{
		return;
	}
	observerHandler->OnClickTargetChangeItem(_itemTarget);
}

// 아이템 데이터 세팅
void UGsUIObserveTargetListItem::SetObserveTargetListItemData(UGsGameObjectBase* In_target)
{
	if (In_target == nullptr ||
		In_target->GetData() == nullptr)
	{
		return;
	}

	// 저장
	_itemTarget = In_target;

	int targetLevel = In_target->GetData()->GetLevel();

	// 레벨 세팅
	FString levelFormat = FString::Format(TEXT("Lv.{0}"), { targetLevel });
	_textTargetLevel = FText::FromString(levelFormat);

	// 이름 세팅
	FString targetName = In_target->GetData()->GetName();
	_textTargetName = FText::FromString(targetName);
}