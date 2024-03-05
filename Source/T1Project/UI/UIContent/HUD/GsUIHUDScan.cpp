#include "GSUIHUDScan.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"

#include "Management/ScopeGame/GsGameObjectManager.h"

#include "Scan/GsScanMemberInfo.h"

#include "GameFlow/GameContents/GsContentsMode.h"
#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Scan/GsScanHandler.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UI/UIContent/HUD/Scan/GsUIScanMemberItem.h"

void UGsUIHUDScan::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnClearList->OnClicked.AddDynamic(this, &UGsUIHUDScan::OnClickDelScanList);
	_btnScanOption->OnClicked.AddDynamic(this, &UGsUIHUDScan::OnClickOpenOption);

}
void UGsUIHUDScan::NativeConstruct()
{
	Super::NativeConstruct();
	// 처음에 다 hide시킨다
	HideAllList();

	_tutorialSlotIndex = -1;
	//_isDirtyRefreshUI = false;
}

bool UGsUIHUDScan::UpdateManagerTick(float InDeltaTime)
{
	if (false == Super::UpdateManagerTick(InDeltaTime))
	{
		return false;
	}

	if (_isDirtyRefreshUI == false)
	{
		return false;
	}

	_isDirtyRefreshUI = false;
	RefreshUI();

	return true;
}

// ui 갱신
void UGsUIHUDScan::RefreshUI()
{
	// 전체 끄기
	HideAllList();


	// 스캔 정보 가져오기
	TMap<int64, FGsScanMemberInfo*> mapInfo =	GSGameObject()->GetScanInfo();

	if (IsTutorial())
	{
		if (mapInfo.Num() == 0 ||
			mapInfo.Num() <= _tutorialSlotIndex)
		{
			GMessage()->GetTutorial().SendMessage(MessageContentTutorial::FORCED_END_TUTORIAL, nullptr);
		}
	}

	// 정보 없으면 return
	if (mapInfo.Num() == 0)
	{
		return;
	}

	int i = 0;
	for (auto iter: mapInfo)
	{
		// 실제 아이템갯수를 넘어서면 return
		if (_arrayScanItems.Num() <= i)
		{
			return;
		}

		FGsScanMemberInfo* memberInfo = iter.Value;
		// 정보 없으면 다음 처리
		if (memberInfo == nullptr)
		{
			continue;
		}

		UGsUIScanMemberItem* item = _arrayScanItems[i];
		// ui가 nullptr 이다
		if (item == nullptr)
		{
			// 다음 인덱스
			++i;
			continue;
		}

		item->SetData(memberInfo);
		item->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		// 다음 인덱스
		++i;
	}
}

// 모든 리스트 hide
void UGsUIHUDScan::HideAllList()
{
	if (_arrayScanItems.Num() == 0)
	{
		return;
	}

	for (auto iter: _arrayScanItems)
	{
		if (iter == nullptr)
		{
			continue;
		}
		if (iter->GetVisibility() != ESlateVisibility::Hidden)
		{
			// hidden으로 처리
			iter->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

// 스캔 리스트 삭제 클릭
void UGsUIHUDScan::OnClickDelScanList()
{
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

	FGsScanHandler* scanHandler = hud->GetScanHandler();
	if (scanHandler == nullptr)
	{
		return;
	}

	scanHandler->ClearScanList();
}
// 스캔 옵션 팝업 열기
void UGsUIHUDScan::OnClickOpenOption()
{
	UGsUIManager* uiMgr = GUI();
	if (uiMgr == nullptr)
	{
		return;
	}
	
	uiMgr->Open(TEXT("PopupScanOption"));
}

UGsUIScanMemberItem* UGsUIHUDScan::SetTutorialTarget(int32 InSlotIndex)
{
	_tutorialSlotIndex = InSlotIndex;

	if (_arrayScanItems.IsValidIndex(InSlotIndex))
	{
		return _arrayScanItems[InSlotIndex];
	}

	return nullptr;
}