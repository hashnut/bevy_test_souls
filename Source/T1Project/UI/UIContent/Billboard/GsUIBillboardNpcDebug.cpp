#include "GsUIBillboardNpcDebug.h"

#include "UMG/Public/Components/WidgetSwitcher.h"
#include "UMG/Public/Components/SlateWrapperTypes.h"

#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

#include "Paper2D/Classes/PaperSprite.h"

#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Option/GsGameUserSettings.h"

#include "GameObject/ObjectClass/GsGameObjectNonPlayer.h"

#include "T1Project.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Define/GsIconDefine.h"
#include "UI/UIContent/Common/Icon/GsUIIconBase.h"
#include "UI/UIContent/Factory/GsIconFactory.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UILib/Define/GsUIDefine.h"

#include "DataSchema/GsSchemaInteractionData.h"

void UGsUIBillboardNpcDebug::NativeOnInitialized()
{
	Super::NativeOnInitialized();	
}

void UGsUIBillboardNpcDebug::NativeConstruct()
{
	Super::NativeConstruct();
}	

void UGsUIBillboardNpcDebug::SetNpcInfoText(const FString& inNpcInfo)
{
	_npcInfoTextBlock->SetText(FText::FromString(inNpcInfo));
}
