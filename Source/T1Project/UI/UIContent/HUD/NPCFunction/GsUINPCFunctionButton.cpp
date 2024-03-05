#include "GsUINPCFunctionButton.h"

#include "Management/ScopeGlobal/GsOptionManager.h"

#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsAIManager.h"
#include "Management/ScopeGame/GsPersonalTradingManager.h"

#include "Option/GsGameUserSettings.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Quest/GsQuestHandler.h"
#include "GameObject/Interaction/GsInteractionHandlerLocalPlayer.h"
#include "GameObject/Interaction/GsInteractionMoveType.h"
#include "GameObject/Movement/LocalPlayer/GsMovementStateAuto.h"
#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"
#include "GameObject/Movement/LocalPlayer/GsMovementAutoDefine.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

#include "DataSchema/GameObject/Npc/GsSchemaNpcFunctionData.h"

#include "UTIL/GsTableUtil.h"

#include "AI/Function/GsAIFunc.h"

#include "Paper2D/Classes/PaperSprite.h"
#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"
#include "Engine/AssetManager.h"
#include "Components/TextBlock.h"

void UGsUINPCFunctionButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// tooltip hide issue
	// click event: 
	// When I check if it is open, it opens again because it is already closed.
	_btnNPCFunction->OnPressed.AddDynamic(this, &UGsUINPCFunctionButton::OnPressedNPCFunctionButton);

	_btnNPCFunction->OnClicked.AddDynamic(this, &UGsUINPCFunctionButton::OnClickNPCFunctionButton);
}

void UGsUINPCFunctionButton::OnClickDimmedButton()
{
}

void UGsUINPCFunctionButton::OnPressedNPCFunctionButton()
{
	if (_bIsFakePage)
	{
		return;
	}

	FGsPersonalTradingManager* tradeManager = GSPersonalTrading();
	if (nullptr == tradeManager || tradeManager->IsTradeOpen())
	{
		return;
	}

	int npcCount = _arrayNpcId.Num();
	if (npcCount == 0)
	{
		return;
	}
	else if( npcCount > 1)
	{
		// show npc list
		if (_callbackClickMultiNpc != nullptr)
		{
			_callbackClickMultiNpc(this, _arrayNpcId);
		}
	}
}
void UGsUINPCFunctionButton::OnClickNPCFunctionButton()
{
	if (_bIsFakePage)
	{
		return;
	}

	FGsPersonalTradingManager* tradeManager = GSPersonalTrading();
	if (nullptr == tradeManager || tradeManager->IsTradeOpen())
	{
		return;
	}

	int npcCount = _arrayNpcId.Num();
	if (npcCount == 0)
	{
		return;
	}
	else if (npcCount == 1)
	{
		int npcId = _arrayNpcId[0];
		FGsAIFunc::StartMoveNPCFunctionSlot(npcId);
	}
}

void UGsUINPCFunctionButton::SetEnableButton(bool InEnable)
{
	int switchIndex = (InEnable == true) ? 0 : 1;
	_switcherBtnState->SetActiveWidgetIndex( switchIndex);
}

void UGsUINPCFunctionButton::SetEmptySlot()
{
	SetEnableButton(true);

	UPaperSprite* spriteImg =  GetEmptyIcon();
	_imgIconDimmed->SetBrushFromAtlasInterface(spriteImg);
	_imgIconNormal->SetBrushFromAtlasInterface(spriteImg);

	UpdateShowSlotNameOption();

	// 일반 마을 -> 그림자 전장 로비 갈때
	// 기존 데이터 남아있어서 클리어 추가
	// https://jira.com2us.com/jira/browse/CHR-24545

	// npc id 클리어
	if (_arrayNpcId.Num() != 0)
	{
		_arrayNpcId.Empty();
	}
	// 슬롯 텍스트 클리어
	_textSlotName = FText::FromString(TEXT(""));
	// 이동 표시 클리어
	SetMoveEffect(false);
	// 콜백함수 클리어
	_callbackClickMultiNpc = nullptr;
}

void UGsUINPCFunctionButton::SetData(TArray<int> In_arrayNpcId, 
	const FGsSchemaNpcFunctionData* In_functionData, 
	bool In_isUseable, 
	bool In_isMoveButton,
	TFunction<void(UUserWidget* In_target, TArray<int> In_arrayNpcTblId)> In_callbackClickMultiNpc)
{
	_arrayNpcId = In_arrayNpcId;

	if (In_functionData)
	{
		UPaperSprite* spriteImg = FGsUIHelper::LoadSpriteFromPath(In_functionData->_slotIconImagePath);
		_imgIconNormal->SetBrushFromAtlasInterface(spriteImg);
		_imgIconDimmed->SetBrushFromAtlasInterface(spriteImg);

		_textSlotName = In_functionData->_slotFunctionName;
	}

	SetEnableButton(In_isUseable);

	_callbackClickMultiNpc = In_callbackClickMultiNpc;

	UpdateShowSlotNameOption();

	SetMoveEffect(In_isMoveButton);
}

UPaperSprite* UGsUINPCFunctionButton::GetEmptyIcon()
{
	if (false == _emptyIconRef.IsNull())
	{
		if (_emptyIconRef.IsPending())
		{
			FStreamableManager& assetMgr = UAssetManager::GetStreamableManager();
			_emptyIconRef = assetMgr.LoadSynchronous(_emptyIconRef, true);
		}

		return _emptyIconRef.Get();
	}

	return nullptr;
}
void UGsUINPCFunctionButton::UpdateShowSlotNameOption()
{
	UGsGameUserSettings* gameUserSettings = GGameUserSettings();
	if (gameUserSettings == nullptr)
	{
		return;
	}
	// 옵션 값 확인
	bool isShowSlotName =
		(bool)gameUserSettings->GetCombatSetting(EGsOptionCombat::NPC_FUNCTION_UI_NAME);

	ESlateVisibility visibility =
		(isShowSlotName == true) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden;

	_textBlockSlotName->SetVisibility(visibility);
	_textBlockSlotNameDimmed->SetVisibility(visibility);
}

void UGsUINPCFunctionButton::SetMoveEffect(bool In_isMoveButton)
{
	ESlateVisibility visibility =
		(In_isMoveButton == true) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden;
	_effectMoveButton->SetVisibility(visibility);
}

void UGsUINPCFunctionButton::OnKeyboardPress()
{
	OnPressedNPCFunctionButton();
}

void UGsUINPCFunctionButton::OnKeyboardRelease()
{
	OnClickNPCFunctionButton();
}