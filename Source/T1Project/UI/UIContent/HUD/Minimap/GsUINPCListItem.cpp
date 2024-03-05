#include "GsUINPCListItem.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Quest/GsQuestHandler.h"
#include "GameObject/Interaction/GsInteractionHandlerLocalPlayer.h"
#include "GameObject/Interaction/GsInteractionMoveType.h"
#include "GameObject/Movement/LocalPlayer/GsMovementStateAuto.h"
#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"
#include "GameObject/Movement/LocalPlayer/GsMovementAutoDefine.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Define/GsIconDefine.h"
#include "UI/UIContent/Common/Icon/GsUIIconBase.h"
#include "UI/UIContent/Factory/GsIconFactory.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsAIManager.h"

#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsInteractionMessageParam.h"

#include "UMG/Public/Components/CanvasPanel.h"
#include "UMG/Public/Components/CanvasPanelSlot.h"

#include "Components/Image.h"
#include "Paper2D/Classes/PaperSprite.h"

#include "DataSchema/GameObject/Npc/GsSchemaNpcFunctionData.h"

#include "Map/Minimap/GsMinimapFunc.h"

#include "T1Project.h"

void UGsUINPCListItem::NativeOnInitialized()
{
	_btnItem->OnClicked.AddDynamic(this, &UGsUINPCListItem::OnClickItem);

	Super::NativeOnInitialized();
}

void UGsUINPCListItem::OnClickItem()
{

}

// 데이터 세팅
void UGsUINPCListItem::SetData( FString In_npcName)
{
	_textNPCName = FText::FromString(In_npcName);
	

}
