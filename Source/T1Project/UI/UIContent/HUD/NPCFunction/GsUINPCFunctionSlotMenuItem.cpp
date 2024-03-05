#include "GsUINPCFunctionSlotMenuItem.h"

#include "AI/Function/GsAIFunc.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "DataCenter/Public/DataSchema/GameObject/Npc/GsSchemaNpcData.h"

#include "UTIL/GsTableUtil.h"

void UGsUINPCFunctionSlotMenuItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();


	_btnClick->OnClicked.AddDynamic(this, &UGsUINPCFunctionSlotMenuItem::OnClickButton);
}

void UGsUINPCFunctionSlotMenuItem::OnClickButton()
{
	FGsAIFunc::StartMoveNPCFunctionSlot(_npcTblId);
}

void UGsUINPCFunctionSlotMenuItem::SetSlotMenuItemData( int In_tblId, int In_itemNum)
{

	const FGsSchemaNpcData* npcData =
		UGsTableUtil::FindRowById<UGsTableNpcData, FGsSchemaNpcData>(In_tblId);

	if (npcData == nullptr)
	{
		return;
	}

	FString strName = FString::Format(TEXT("{0}.{1}"), 
		{ *(FString::FromInt(In_itemNum)), *(npcData->nameText.ToString()) });

	_npcTblId = In_tblId;
	_textNpcName = FText::FromString(strName);
}