#include "GsUIPopupMonsterCollectionBookGradeRewardInfo.h"
#include "GsUIMonsterCollectionBookDetailRewardEntry.h"
#include "MonsterKnowledgeCollection/GsMonsterKnowledgeLevelInfo.h"
#include "../Management/ScopeGame/GsMonsterCollectionManager.h"
#include "../UI/UIContent/Common/GsUITabButtonCommon.h"
#include "../UI/UIContent/Common/Tooltip/GsUITooltipDesc.h"
#include "../UI/UIControlLib/Define/GsIconDefine.h"
#include "../UI/UIControlLib/Control/GsToggleGroup.h"
#include "../UI/UIControlLib/ContentWidget/GsButton.h"
#include "../UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "../UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UMG/Public/Components/ScrollBox.h"
#include "Shared/Client/SharedEnums/SharedMonsterknowledgeEnum.h"
#include "DataSchema/MonsterKnowledgeCollection/GsSchemaMonsterKnowledgeLevelSet.h"






void UGsUIPopupMonsterCollectionBookGradeRewardInfo::BeginDestroy()
{
	if (_btnClose)
	{
		_btnClose->OnClicked.RemoveDynamic(this, &UGsUIPopupMonsterCollectionBookGradeRewardInfo::OnClose);
		_btnClose = nullptr;
	}	

	if (_btnTooltip)
	{
		_btnTooltip->OnClicked.RemoveDynamic(this, &UGsUIPopupMonsterCollectionBookGradeRewardInfo::OnClickedTooltip);
		_btnTooltip = nullptr;
	}
	

	Super::BeginDestroy();
}

void UGsUIPopupMonsterCollectionBookGradeRewardInfo::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	_slotHelperDetailView = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperDetailView->Initialize(_subClassOfDetailViewEntry, _scrollDetailInfo);
	_slotHelperDetailView->OnRefreshEntry.AddDynamic(this, &UGsUIPopupMonsterCollectionBookGradeRewardInfo::OnRefreshDetailInfo);
		
	_toggleGroupCategoryTab.Clear();
	_toggleGroupCategoryTab.OnSelectChanged.BindUObject(this, &UGsUIPopupMonsterCollectionBookGradeRewardInfo::OnSelectedCategoryTab);
	
	_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupMonsterCollectionBookGradeRewardInfo::OnClose);
	_btnTooltip->OnClicked.AddDynamic(this, &UGsUIPopupMonsterCollectionBookGradeRewardInfo::OnClickedTooltip);
}

void UGsUIPopupMonsterCollectionBookGradeRewardInfo::NativeConstruct()
{
	Super::NativeConstruct();

	FText textTooltip;
	FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("UI_Popup_LevelReward_InfoText"), textTooltip);
	_tooltipGradeReward->SetDescText(textTooltip);
	_tooltipGradeReward->SetLifeTime(_toolTipLifetime);
	
	_toggleGroupCategoryTab.Clear();
	_toggleGroupCategoryTab.AddToggleWidgetByParentPanel(_scrollCategoryBtn);
}

void UGsUIPopupMonsterCollectionBookGradeRewardInfo::NativeDestruct()
{
	_tooltipGradeReward->Close();

	Super::NativeDestruct();
}

void UGsUIPopupMonsterCollectionBookGradeRewardInfo::OnClose()
{
	Close();
}

void UGsUIPopupMonsterCollectionBookGradeRewardInfo::OnClickedTooltip()
{
	_tooltipGradeReward->ToggleOpenClose();
}

void UGsUIPopupMonsterCollectionBookGradeRewardInfo::OnRefreshDetailInfo(int32 InIndex, UWidget* InEntry)
{
	/*TWeakObjectPtr<UGsUIMonsterCollectionBookDetailRewardEntry> item = Cast<UGsUIMonsterCollectionBookDetailRewardEntry>(InEntry);
	if (false == item.IsValid() || nullptr == _data)
	{
		GSLOG(Error, TEXT("item == tabButton.IsValid()"));
		return;
	}
	
	if (false == _data->knowledgeLevelList.IsValidIndex(InIndex))
	{
		return;
	}

	const auto& levelInfo = _data->knowledgeLevelList.FindByPredicate([InIndex](const FGsMonsterKnowledgeLevelInfo& InData)
		{
			return InData.level == (InIndex + 1);
		}
	);

	int32 level = 0;
	int32 requiredKnowledge = 0;
	int32 rewardPoint = 0;

	if (levelInfo)
	{
		level = levelInfo->level;
		requiredKnowledge = levelInfo->levelUpExpRequired;
		rewardPoint = levelInfo->monsterBookPoint;
	}
	item->SetData(level, requiredKnowledge, rewardPoint);*/
}

void UGsUIPopupMonsterCollectionBookGradeRewardInfo::OnSelectedCategoryTab(int32 InIndex)
{
	//// 2023/2/14 PKT - MonsterKnowledgeBookGrade::NONE 제외
	//const int32 maxCount = StaticCast<int32>(MonsterKnowledgeBookGrade::MAX) - 1;
	//MonsterKnowledgeBookGrade bookGrade = StaticCast<MonsterKnowledgeBookGrade>(maxCount - InIndex);
	//const auto& schemaDetailData = GsMonsterCollection()->BookGradeDetailInfo(bookGrade);
	//if (nullptr == schemaDetailData)
	//{
	//	GSLOG(Error, TEXT("nullptr == schemaDetailData"));
	//	return;
	//}

	//const FGsSchemaMonsterKnowledgeLevelSet* levelSet = schemaDetailData->levelSetId.GetRow();
	//if (nullptr == levelSet || 0 == levelSet->knowledgeLevelList.Num())
	//{
	//	GSLOG(Error, TEXT("nullptr != levelSet || 0 == levelSet->knowledgeLevelList.Num()"));
	//	return;
	//}

	//_data = levelSet;

	//const int32 detailCount = _data->knowledgeLevelList.Num();
	//_slotHelperDetailView->RefreshAll(detailCount);
}

void UGsUIPopupMonsterCollectionBookGradeRewardInfo::Open()
{
	_data = nullptr;

	// 2023/2/14 PKT - MonsterKnowledgeBookGrade::NONE 제외
	const int32 maxCount = StaticCast<int32>(MonsterKnowledgeBookGrade::MAX) - 1;
	const int32 selectedIndex = maxCount - StaticCast<int32>(MonsterKnowledgeBookGrade::UNIQUE);
	_toggleGroupCategoryTab.SetSelectedIndex(selectedIndex, true);
}

void UGsUIPopupMonsterCollectionBookGradeRewardInfo::OnInputCancel()
{
	OnClose();
}