#include "GsUIItemDropElement.h"

#include "Runtime/UMG/Public/Blueprint/IUserObjectListEntry.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "T1Project.h"

void UGsUIItemDropElement::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_moveBtn->OnClicked.AddUniqueDynamic(this, &UGsUIItemDropElement::OnClickedBtnMove);
}

void UGsUIItemDropElement::NativeOnListItemObjectSet(UObject* InListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(InListItemObject);

	Invalidate(InListItemObject);
}

void UGsUIItemDropElement::Invalidate(const UObject* InListItemObject /*= nullptr*/)
{
	if (nullptr == InListItemObject)
	{
		InListItemObject = GetListItem();
	}

	const UGsUIItemDropElementListItem* ListItem = Cast<UGsUIItemDropElementListItem>(InListItemObject);
	if (nullptr == ListItem)
	{
		GSLOG(Error, TEXT("nullptr == ListItem"));
		return;
	}

	_mapNameMainTxt->SetText(ListItem->_continentalName);
	_mapNameSubTxt->SetText(ListItem->_mapName);
	_mapNameSubTxt->SetColorAndOpacity(ListItem->_mapNameColor);
	_targetNameTxt->SetText(ListItem->_creatureName);
	_gradeTextBlock->SetText(ListItem->_gradeText);
	_offensiveTextBlock->SetText(ListItem->_offensiveText);
	_levelTextBlock->SetText(ListItem->_levelText);
}

void UGsUIItemDropElement::OnClickedBtnMove()
{
	const UGsUIItemDropElementListItem* ListItem = Cast<UGsUIItemDropElementListItem>(GetListItem());
	if (nullptr == ListItem)
	{
		GSLOG(Error, TEXT("nullptr == ListItem"));
		return;
	}

	ListItem->_onClickMoveBtn.ExecuteIfBound(*ListItem);
}
