#include "GsUIRegionTouchMenu.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"

#include "Management/ScopeGame/GsWorldMapManager.h"
#include "Management/ScopeGame/GsAIManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"

#include "Message/GsMessageContents.h"

#include "Runtime/DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"

#include "UTIL/GsGameObjectUtil.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"

#include "UMG/Public/Animation/WidgetAnimation.h"

void UGsUIRegionTouchMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnAutoMove->OnClicked.AddDynamic(this, &UGsUIRegionTouchMenu::OnClickAutoMove);
	
}


void UGsUIRegionTouchMenu::OnClickAutoMove()
{
	int showRegionMapId = GSWorldMap()->GetShowRegionMapId();
	int currentMapId = GLevel()->GetCurrentLevelId();

	if (showRegionMapId != currentMapId)
	{
		if (false == GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::MOVE_TO_OTHER_REGION, true))
		{
			return;
		}
	}


	// 대화연출 응답 대기중이면 return
	if (true == UGsGameObjectUtil::IsInputFlagBlock(EGsKeyMappingInputFlags::INPUT_CONTENTS_WAIT_SERVER_RESPONSE))
	{
		return;
	}

	GSAI()->MoveToPosAuto(EGsMovementAutoContentsType::TouchUIMapMove, showRegionMapId,
		_movePos);


	// main hud로 이동
	GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_HUD);
	// region map window 닫기
	// 특정 ui 만 닫으면 그전에 있던 ui가 뜨므로 다닫기로 한다
	// ex)지역->대륙->지역에서 이동시
	GUI()->CloseAllStack();

	// visibility hidden
	SetVisibility(ESlateVisibility::Hidden);
}

void UGsUIRegionTouchMenu::SetData(FVector In_pos)
{
	
	_movePos = In_pos;

	// clear now
	UGsBlueprintFunctionLibraryUI::StopAllAndClearAnimation(this);
	PlayAnimation(_aniMenu);
	
}

void UGsUIRegionTouchMenu::NativeDestruct()
{

	// clear now
	UGsBlueprintFunctionLibraryUI::StopAllAndClearAnimation(this);
	Super::NativeDestruct();
}