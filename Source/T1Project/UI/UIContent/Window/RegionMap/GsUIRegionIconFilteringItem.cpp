#include "GsUIRegionIconFilteringItem.h"

#include "Management/GsMessageHolder.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

#include "DataSchema/Icon/GsSchemaRegionMapIconFilteringData.h"

#include "UTIL/GsTableUtil.h"

#include "Option/GsServerOption.h"

#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/GsMessageUI.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Paper2D/Classes/PaperSprite.h"

#include "UMG/Public/Components/WidgetSwitcher.h"

#include "Core/Public/Delegates/Delegate.h"

#include "Components/Image.h"

void UGsUIRegionIconFilteringItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnCheck->OnClicked.AddDynamic(this, &UGsUIRegionIconFilteringItem::OnClickCheck);
}

void UGsUIRegionIconFilteringItem::OnClickCheck()
{
	if (_toggleCallback)
	{
		_toggleCallback(_indexInGroup, !_bIsSelected);
	}
}

void UGsUIRegionIconFilteringItem::SetIsSelected(bool bInIsSelected)
{
	_bIsSelected = bInIsSelected;
	// 0: select
	// 1: unselect
	int tabType = (bInIsSelected == true) ? 0 : 1;
	
	_switcherCheckType->SetActiveWidgetIndex(tabType);
}


bool UGsUIRegionIconFilteringItem::GetIsSelected() const
{
	return _bIsSelected;
}

void UGsUIRegionIconFilteringItem::SetFilteringItemData(EGsRegionMapIconFiltering In_filteringType)
{
	_filteringType = In_filteringType;

	const FGsSchemaRegionMapIconFilteringData* findTbl = 
		UGsTableUtil::FindRowById<UGsTableRegionMapIconFilteringData, FGsSchemaRegionMapIconFilteringData>(In_filteringType);

	if (nullptr == findTbl)
	{
		return;
	}
	// set image
	UPaperSprite* spriteImg = FGsUIHelper::LoadSpriteFromPath(findTbl->_iconImagePath);
	_imgIconType->SetBrushFromAtlasInterface(spriteImg);

	// set text
	_textFilteringName = findTbl->_textType;

	// set sever option check

	WorldMapFilterInfo& optionData = GServerOption()->GetWorldMapFilter();

	bool isChecked = true;
	switch (In_filteringType)
	{
	case EGsRegionMapIconFiltering::Party:
		isChecked = optionData.mParty;
		break;
	case EGsRegionMapIconFiltering::TeleportSpot:
		isChecked = optionData.mTeleportSpot;
		break;
	case EGsRegionMapIconFiltering::Bookmark:
		isChecked = optionData.mBookMark;
		break;
	case EGsRegionMapIconFiltering::SpaceCrackPortal:
		isChecked = optionData.mSpaceCrackPortal;
		break;
	case EGsRegionMapIconFiltering::FieldBoss:
		isChecked = optionData.mFieldBoss;
		break;
	case EGsRegionMapIconFiltering::Sanctum:
		isChecked = optionData.mSanctum;
		break;
	}

	SetIsSelected(isChecked);

	SetToggleCallback([this](int32 In_index, bool In_selected)
		{
			OnSelectCheck( In_selected);
		});

}

void UGsUIRegionIconFilteringItem::OnSelectCheck( bool In_isSelect)
{
	SetIsSelected(In_isSelect);

	// save server option
	WorldMapFilterInfo& optionData = GServerOption()->GetWorldMapFilter();
	
	switch (_filteringType)
	{
	case EGsRegionMapIconFiltering::Party:
		optionData.mParty = In_isSelect;
		break;
	case EGsRegionMapIconFiltering::TeleportSpot:
		optionData.mTeleportSpot = In_isSelect;
		break;
	case EGsRegionMapIconFiltering::Bookmark:
		optionData.mBookMark = In_isSelect;
		break;
	case EGsRegionMapIconFiltering::SpaceCrackPortal:
		optionData.mSpaceCrackPortal = In_isSelect;
		break;
	case EGsRegionMapIconFiltering::FieldBoss:
		optionData.mFieldBoss = In_isSelect;
		break;
	case EGsRegionMapIconFiltering::Sanctum:
		optionData.mSanctum = In_isSelect;
		break;
	}

	// change exist icon visible
	FGsUIMsgParamRegionMapIconFiltering paramFilterType(_filteringType, In_isSelect);
	GMessage()->GetUI().SendMessage(MessageUI::CHANGE_REGION_ICON_FILTERING, &paramFilterType);
}