
#include "GsUIDungeonSlotEntry.h"
#include "Dungeon/GsDungeonData.h"
#include "Dungeon/GsDungeonHelper.h"
#include "UTIL/GsUIUtil.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "Image.h"
#include "TextBlock.h"
#include "UserWidget.h"

#include "Dungeon/GsSchemaDungeonData.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "T1Project.h"


void UGsUIDungeonSlotEntry::NativeOnInitialized()
{
	_btnClicked->OnClicked.AddDynamic(this, &UGsUIDungeonSlotEntry::OnClickSelected);

	Super::NativeOnInitialized();
}

void UGsUIDungeonSlotEntry::OnClickSelected()
{
	if (_toggleCallback)
	{
		_toggleCallback(_indexInGroup, true);
	}
}

void UGsUIDungeonSlotEntry::SetData(const FGsDungeonData* InDungeonData)
{
	if (nullptr == InDungeonData)
	{
		return;
	}

	_dungeonData = InDungeonData;

	/**
	 * Set Dungeon Name
	 */
	FText format;
	// 2023/2/28 PKT - 현재 모두 층으로 표기
	FGsDungeonHelper::FindText(TEXT("UI_Dungeon_Floor"), format);
	_textName->SetText(FText::Format(format, { _dungeonData->GetSchema()->indexValue }));

	Invalidate();
}

void UGsUIDungeonSlotEntry::Invalidate()
{
	if (nullptr == _dungeonData)
	{
		GSLOG(Error, TEXT("nullptr == _dungeonData"));
		return;
	}

	// 2022/08/17 PKT - 그룹 정보와 던전 정보 모두 언락이 풀려야 딤드를 해제 한다.
	bool isUnLockContents = _dungeonData->IsUnLockState();
	_imgDimmed->SetVisibility(isUnLockContents ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
}

void UGsUIDungeonSlotEntry::SetIsSelected(bool bInIsSelected)
{
	if (_imgSelectedEdge)
	{
		_imgSelectedEdge->SetVisibility(bInIsSelected ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
}

bool UGsUIDungeonSlotEntry::GetIsSelected() const
{
	if (_imgSelectedEdge)
	{
		return ESlateVisibility::SelfHitTestInvisible == _imgSelectedEdge->GetVisibility();
	}

	return false; 
}

const FGsDungeonData* UGsUIDungeonSlotEntry::GetData() const
{
	return _dungeonData;
}