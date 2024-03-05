#include "GsUIChatBlockEntry.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "TextBlock.h"

void UGsUIChatBlockEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnUnblock->OnClicked.AddDynamic(this, &UGsUIChatBlockEntry::OnClickUnblock);
}

void UGsUIChatBlockEntry::OnClickUnblock()
{
	OnReqUnblock.ExecuteIfBound(_uid);
}

void UGsUIChatBlockEntry::SetData(FGsChatBlockUser* data)
{
	_uid = 0;
	if (data)
	{
		_uid = data->_uid;
		_name = data->_name;
	}
}

void UGsUIChatBlockEntry::RegreshUI()
{
	_blockName->SetText(_uid > 0 ? FText::FromString(_name) : FText::GetEmpty());
}