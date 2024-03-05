// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupRankingRewardInfo.h"
#include "T1Project.h"
#include "Components/ListView.h"
#include "Components/PanelWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/ScrollBox.h"
#include "DataSchema/Ranking/GsSchemaRankingReward.h"
#include "DataSchema/Ranking/GsSchemaRankingCategory.h"
#include "DataSchema/Ranking/GsSchemaRankingTypeObjective.h"
#include "DataSchema/Ranking/GsSchemaRankingRewardData.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "UI/UIContent/Common/GsUITabButtonCommon.h"
#include "UI/UIContent/Window/Ranking/GsUIRankingRewardListEntry.h"
#include "Management/ScopeGame/GsEffectTextManager.h"
#include "Management/ScopeGame/GsRankingManager.h"


void UGsUIPopupRankingRewardInfo::BeginDestroy()
{
	if (nullptr != _slotHelperMainTabBtn)
	{
		_slotHelperMainTabBtn->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupRankingRewardInfo::MainTabListRefresh);
		_slotHelperMainTabBtn = nullptr;
	}
	
	if (nullptr != _slotHelperReward)
	{
		_slotHelperReward->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupRankingRewardInfo::RewardListRefresh);
		_slotHelperReward = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIPopupRankingRewardInfo::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupRankingRewardInfo::OnClickOk);

	_toggleGroupMainTab.OnSelectChanged.BindUObject(this, &UGsUIPopupRankingRewardInfo::OnSelectChangedMainTab);

	if (nullptr == _slotHelperMainTabBtn)
	{
		_slotHelperMainTabBtn = NewObject<UGsDynamicPanelSlotHelper>(this);
		_slotHelperMainTabBtn->Initialize(_subClassOfMainTab, _rootPanelMainTab);
		_slotHelperMainTabBtn->OnRefreshEntry.AddDynamic(this, &UGsUIPopupRankingRewardInfo::MainTabListRefresh);
	}

	if (nullptr == _slotHelperReward)
	{
		_slotHelperReward = NewObject<UGsDynamicPanelSlotHelper>(this);
		_slotHelperReward->Initialize(_subClassOfReward, _rootPanelReward);
		_slotHelperReward->OnRefreshEntry.AddDynamic(this, &UGsUIPopupRankingRewardInfo::RewardListRefresh);
	}
}

void UGsUIPopupRankingRewardInfo::NativeConstruct()
{
	Super::NativeConstruct();

	InvalidateMainTab();

	_toggleGroupMainTab.SetSelectedIndex(0, true);
}

void UGsUIPopupRankingRewardInfo::NativeDestruct()
{
	_toggleGroupMainTab.Clear();

	_mainTabDataList.Empty();
	_rewardDataList.Empty();

	Super::NativeDestruct();
}

void UGsUIPopupRankingRewardInfo::InvalidateMainTab()
{
	_toggleGroupMainTab.Clear();

	FGsRankingManager* rankingManager = GSRanking();
	if (nullptr == rankingManager)
		return;

	const TArray<const FGsSchemaRankingCategory*>& mainTabDataList = rankingManager->GetMainTabDataList();
	if (0 >= mainTabDataList.Num())
	{
		OnClickOk();
		return;
	}

	_mainTabDataList.Empty();
	for (int32 i = 0; i < mainTabDataList.Num(); ++i)
	{
		if(nullptr == mainTabDataList[i])
			continue;

		// 보상이 존재하는 탭만 추가한다.
		for (int32 subTabIndex = 0; subTabIndex < mainTabDataList[i]->rankingTypeObjective.Num(); ++subTabIndex)
		{
			const FGsSchemaRankingTypeObjective* subTabData = mainTabDataList[i]->rankingTypeObjective[subTabIndex].GetRow();
			if (nullptr == subTabData)
				continue;

			const FGsSchemaRankingReward* rewardData = rankingManager->GetRewardData(subTabData->index);
			if (nullptr == rewardData)
				continue;

			_mainTabDataList.Emplace(mainTabDataList[i]);
			break;
		}
	}

	_slotHelperMainTabBtn->RefreshAll(_mainTabDataList.Num());
}

void UGsUIPopupRankingRewardInfo::InvalidateRewardList(int32 InMainTabIndex)
{
	if (false == _mainTabDataList.IsValidIndex(InMainTabIndex))
		return;

	FGsRankingManager* rankingManager = GSRanking();
	if (nullptr == rankingManager)
		return;

	const FGsSchemaRankingCategory* mainTabData = _mainTabDataList[InMainTabIndex];
	if (nullptr == mainTabData)
		return;
	
	int32 entryCount = 0; // 각 보상은 하위 보상을 가진다. ex) 레벨 랭킹 1위, 2위, 3위, ...
	_rewardDataList.Empty();
	const FGsSchemaRankingReward* rewardData = nullptr;
	for (const FGsSchemaRankingTypeObjectiveRow& subTabRow : mainTabData->rankingTypeObjective)
	{
		const FGsSchemaRankingTypeObjective* subTabData = subTabRow.GetRow();
		if (nullptr == subTabData)
			continue;

		rewardData = rankingManager->GetRewardData(subTabData->index);
		if (nullptr == rewardData)
			continue;

		_rewardDataList.Emplace(rewardData);
		entryCount += rewardData->rewardDataList.Num();
	}

	_slotHelperReward->RefreshAll(entryCount);
}

void UGsUIPopupRankingRewardInfo::OnSelectChangedMainTab(int32 InIndex)
{
	InvalidateRewardList(InIndex);
}

void UGsUIPopupRankingRewardInfo::MainTabListRefresh(int32 InIndex, UWidget* InEntry)
{
	UGsUITabButtonCommon* entry = Cast<UGsUITabButtonCommon>(InEntry);
	if (nullptr == entry)
		return;

	if (false == _mainTabDataList.IsValidIndex(InIndex))
		return;

	entry->SetTitleText(_mainTabDataList[InIndex]->categoryName);
	_toggleGroupMainTab.AddToggleWidget(entry->GetToggleGroupEntry());
}

void UGsUIPopupRankingRewardInfo::RewardListRefresh(int32 InIndex, UWidget* InEntry)
{
	UGsUIRankingRewardListEntry* entry = Cast<UGsUIRankingRewardListEntry>(InEntry);
	if (nullptr == entry)
		return;

	FGsEffectTextManager* effectTextManager = GSEffectText();
	if (nullptr == effectTextManager)
		return;

	int32 rewardIndex = InIndex;
	const FGsSchemaRankingTypeObjective* subTabData = nullptr;
	const FGsSchemaRankingRewardData* rewardGradeData = nullptr;
	// 메인 탭(카테고리)이 가진 보상이 있는 서브 탭들 중에서
	for (const FGsSchemaRankingReward* rewardData : _rewardDataList)
	{
		if (nullptr == rewardData)
			return;

		// 인덱스에 맞는 서브 탭 차례가 되면
		if (rewardIndex < rewardData->rewardDataList.Num())
		{
			subTabData = rewardData->rankingTypeObjectiveID.GetRow();
			if (nullptr == subTabData)
				return;

			// 해당 서브 탭의 세부 보상 정보를 가져와 설정한다.
			rewardGradeData = &(rewardData->rewardDataList[rewardIndex]);
			if (nullptr == rewardGradeData)
				return;

			FText findText;
			FText gradeText;
			// TEXT1
			if (rewardGradeData->rankingRewardRangeMin == rewardGradeData->rankingRewardRangeMax)
			{
				FText::FindText(TEXT("RankingCommonUIText"), TEXT("RankingRewardSingle"), findText);
				gradeText = FText::Format(findText, subTabData->rankingTabName, rewardGradeData->rankingRewardRangeMin);
			}
			else
			{
				FText::FindText(TEXT("RankingCommonUIText"), TEXT("RankingRewardMulti"), findText);
				gradeText = FText::Format(findText, subTabData->rankingTabName, rewardGradeData->rankingRewardRangeMin, rewardGradeData->rankingRewardRangeMax);
			}

			// TEXT2 - Passivity ID로 해당 정보 가져와서 텍스트 설정
			FString strReward;
			TArray<TPair<FText, FText>> statList;
			if (effectTextManager->GetEffectTextListPassivity(rewardGradeData->rankingRewardPassivityID, statList))
			{
				for (int32 i = 0; i < statList.Num() - 1; ++i)
				{
					TPair<FText, FText>& statDetail = statList[i];
					strReward += statDetail.Key.ToString() + statDetail.Value.ToString() + TEXT(", ");
				}
				TPair<FText, FText>& statDetail = statList.Last();
				strReward += statDetail.Key.ToString() + statDetail.Value.ToString();
			}
			
			entry->SetData(gradeText, FText::FromString(strReward), &rewardGradeData->rankingRewardBadge);
			return;
		}
		// 다음 서브 탭으로 넘어간다.
		else
		{
			rewardIndex -= rewardData->rewardDataList.Num();
		}
	}
}

void UGsUIPopupRankingRewardInfo::OnClickOk()
{
	Close();
}

void UGsUIPopupRankingRewardInfo::OnInputCancel()
{
	Close();
}

