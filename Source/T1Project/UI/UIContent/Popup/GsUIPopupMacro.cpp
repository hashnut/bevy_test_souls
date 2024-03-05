#include "GsUIPopupMacro.h"
#include "Shared/Client/SharedEnums/SharedChatEnum.h"
#include "DataSchema/SocialEmotion/GsSchemaSocialEmotion.h"

#include "Management/ScopeGame/GsChatManager.h"
#include "Management/ScopeGame/GsPartyManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"

#include "Chat/GsUIChatMacroEntry.h"
#include "Chat/GsUIEmojiChatEntry.h"

#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "Components/WidgetSwitcher.h"
#include "PanelWidget.h"

#include "Management/ScopeGame/GsChatManager.h"
#include "Net/GsNetSendService.h"
#include "ForEach.h"

void UGsUIPopupMacro::NativeOnInitialized()
{
	TArray<UGsUIChatMacroEntry*> macroList = {
		_macro1, _macro2, _macro3, _macro4, _macro5,
		_macro6, _macro7, _macro8, _macro9, _macro10
	};


	int index = 0;
	for (auto macro : macroList)
	{
		macro->SetData(index, GSChat()->GetChatMacro(index));
		macro->OInputText.BindLambda([index](FText text) {
			GSChat()->SetChatMacro(index, text, false);
			});
		macro->OnSendText.BindLambda([index, this]() {
			auto text = GSChat()->GetChatMacro(index);
			if (_cbReqInput) _cbReqInput(text);
			Close();
			});

		index++;
	}

	_touchClose->OnPressed.AddDynamic(this, &UGsUIPopupMacro::OnTouchedExtraRange);

	Super::NativeOnInitialized();
}

void UGsUIPopupMacro::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGsUIPopupMacro::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGsUIPopupMacro::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UGsUIPopupMacro::OnClickClose()
{
	Close();
}

void UGsUIPopupMacro::OnTouchedExtraRange()
{
	Close();
}

void UGsUIPopupMacro::CloseInternal()
{
	OnExit();

	Super::CloseInternal();
}

void UGsUIPopupMacro::InitializeData(PopupInitData* initParam)
{
	_cbReqInput = nullptr;
	_cbCancel = nullptr;
	if (initParam)
	{
		_cbReqInput = initParam->reqInput;
		_cbCancel = initParam->cancel;
	}
}

void UGsUIPopupMacro::OnExit()
{
	_cbReqInput = nullptr;
	_cbCancel = nullptr;

	GSChat()->SetChatMacro(0, GSChat()->GetChatMacro(0), true);
}

void UGsUIPopupMacro::OnInputCancel()
{
	OnTouchedExtraRange();
}