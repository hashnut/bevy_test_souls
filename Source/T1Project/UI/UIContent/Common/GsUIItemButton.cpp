
#include "GsUIItemButton.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "DataSchema/Item/GsSchemaItemCommon.h"

#include "UTIL/GsTableUtil.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Paper2D/Classes/PaperSprite.h"

void UGsUIItemButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnMain->OnClicked.AddDynamic(this, &UGsUIItemButton::OnClickMainButton);
}

void UGsUIItemButton::SetButtonName(const FText& InText)
{
	_textButtonName->SetText(InText);
}

void UGsUIItemButton::OnClickMainButton()
{
	OnClickItemButton.ExecuteIfBound();
}
// 아이템 id 세팅
void UGsUIItemButton::SetItemId(int In_itemId)
{
	const FGsSchemaItemCommon* findTblItemCommon =
		UGsTableUtil::FindRowById<UGsTableItemCommon, FGsSchemaItemCommon>(In_itemId);

	if (findTblItemCommon == nullptr)
	{
		return;
	}

	UPaperSprite* sprite = FGsUIHelper::LoadSpriteFromPath(findTblItemCommon->icon);

	if (sprite)
	{
		_imgIcon->SetVisibility(ESlateVisibility::Visible);
		_imgIcon->SetBrushFromAtlasInterface(sprite);
	}
	else
	{
		_imgIcon->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UGsUIItemButton::SetItemCount(int In_count)
{

	FText itemCount = FText::AsNumber(In_count);
	_textItemCount->SetText(itemCount);
}