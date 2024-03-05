#include "GsUIHUDPlayableEventSkill.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "Input/GsInputEventMsgBase.h"

#include "Management/GsMessageHolder.h"

#include "Message/GsMessageInput.h"

#include "Components/Image.h"

void UGsUIHUDPlayableEventSkill::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnSkill->OnClicked.AddDynamic(this, &UGsUIHUDPlayableEventSkill::OnClickedButton);
}

void UGsUIHUDPlayableEventSkill::OnClickedButton()
{
	SendSkillMessage(0);
}

// 스킬 메시지 전송
void UGsUIHUDPlayableEventSkill::SendSkillMessage(int In_slotId)
{
	FGsInputEventMsgBase inputMsg;
	inputMsg.Id = In_slotId;
	GMessage()->GetInput().SendMessage(MessageInput::ClickSkill, inputMsg);
}