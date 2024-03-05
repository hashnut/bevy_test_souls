#include "GsUIHUDOpenPopupChat.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Popup/GsUIPopupChat.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/GsContentsMode.h"
#include "Runtime/DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"

#include "Option/GsOptionEnumType.h"

void UGsUIHUDOpenPopupChat::OnClickOpenPopupChat()
{
	if (UGsUIManager* uiManager = GUI())
	{
		if (auto popup = Cast<UGsUIPopupChat>(uiManager->GetWidgetByKey(TEXT("PopupChat")).Get()))
		{
			popup->OnClickClose();
		}
		else
		{
			uiManager->OpenAndGetWidget(TEXT("PopupChat"));
		}
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDOpenPopupChat::BeginDestroy()
{
	//if (_inputMessageHandler.Value.IsValid())
	//{
	//	MWindowInput& msgInput = GMessage()->GetWindowInput(); //_msgHandleStageList.Emplace(
	//	msgInput.Remove(_inputMessageHandler);
	//}

	Super::BeginDestroy();
}

void UGsUIHUDOpenPopupChat::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnChat->OnClicked.AddDynamic(this, &UGsUIHUDOpenPopupChat::OnClickOpenPopupChat);

	MWindowInput& msgInput = GMessage()->GetWindowInput(); 
	_inputMessageHandler = msgInput.AddUObject(MessageInput::Window_keyboard_Action, this, &UGsUIHUDOpenPopupChat::OnMessageOpenPopup);
}

void UGsUIHUDOpenPopupChat::OnMessageOpenPopup(const IGsMessageParam* param)
{
	if (const FGsInputEventMsgWindowKey* inputParam = param->Cast<const FGsInputEventMsgWindowKey>())
	{
		if (inputParam->WindowKeyType == static_cast<int>(EGsKeyMappingType::KEY_MAPPING_CHAT) && 
			inputParam->InputEvent == EInputEvent::IE_Released)
		{
			OnClickOpenPopupChat();
			/*if (GMode() && GMode()->GetGameFlow())
			{
				if (auto contentsGame = GMode()->GetGameFlow()->GetContentsManagerGame())
				{
					if (contentsGame->GetCurrentContentsMode() == FGsContentsMode::InGame::ContentsHud)
					{
						if (_btnChat->IsVisible() && _btnChat->GetIsEnabled())
							OnClickOpenPopupChat();
					}
				}
			}*/
		}
	}
}
