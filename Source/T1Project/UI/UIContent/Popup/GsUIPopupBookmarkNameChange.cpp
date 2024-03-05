#include "GsUIPopupBookmarkNameChange.h"

#include "Map/Bookmark/GsBookmarkFunc.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Management/ScopeGame/GsBookmarkManager.h"

#include "UMG/Public/Components/EditableTextBox.h"

#include "Containers/StringConv.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "UTIL/GsText.h"
#include "UTIL/GsUIUtil.h"

void UGsUIPopupBookmarkNameChange::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupBookmarkNameChange::OnClickOk);
	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupBookmarkNameChange::OnClickCancel);

	_editUpdateName->OnTextChanged.AddDynamic(this, &UGsUIPopupBookmarkNameChange::OnTextChanged);
}


// ok 클릭
void UGsUIPopupBookmarkNameChange::OnClickOk()
{
	// 글자수 체크 필요
	// 최소 2, 최대 24자(한글은 1, 12자)
	// 한글은 한자에 2
	// 영어 숫자는 한자에 1

	FString sendText = _editUpdateName->GetText().ToString();


	if (false == FGsBookmarkFunc::CheckBookmarkNameLength(sendText))
	{
		return;
	}

	GSBookmark()->SendBookmarkUpdate(_selectIndex, sendText);
	Close();
}
// cancel 클릭
void UGsUIPopupBookmarkNameChange::OnClickCancel()
{
	Close();
}

void UGsUIPopupBookmarkNameChange::NativeConstruct()
{
	Super::NativeConstruct();

	// 클리어
	//_editUpdateName->SetText(FText::GetEmpty());
}

void UGsUIPopupBookmarkNameChange::SetData(int In_selIndex, const FString& In_oldText)
{
	_selectIndex = In_selIndex; 
	_editUpdateName->SetText(FText::FromString(In_oldText));
}

void UGsUIPopupBookmarkNameChange::OnTextChanged(const FText& InText)
{
	int textMaxLen = GData()->GetGlobalData()->_bookmarkNameLenMax;
	UGsUIUtil::CheckEditableTextNameLength(_editUpdateName, textMaxLen, false);
}

void UGsUIPopupBookmarkNameChange::OnInputCancel()
{
	OnClickCancel();
}