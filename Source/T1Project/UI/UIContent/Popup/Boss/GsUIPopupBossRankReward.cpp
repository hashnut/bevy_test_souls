#include "GsUIPopupBossRankReward.h"
#include "GsBossRewardListViewEntry.h"
#include "DataSchema/Map/FieldBoss/GsSchemaFieldBossData.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "T1Project/UI/UIControlLib/ContentWidget/GsButton.h"
#include "T1Project/UI/UIContent/Common/Tooltip/GsUITooltipDesc.h"
#include "UI/UILib/Define/GsUIDefine.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "DataSchema/Reward/GsSchemaRewardData.h"

const FTextKey UI_TEXT_MAIL_REWARD_TEXT_KEY = TEXT("UI_Text_MailReward");
const FTextKey UI_TEXT_DROP_REWARD_TEXT_KEY = TEXT("UI_Text_DropReward");
const FTextKey UI_TITLE_MAIL_REWARD_TEXT_KEY = TEXT("UI_Title_MailReward");
const FTextKey UI_TITLE_DROP_REWARD_TEXT_KEY = TEXT("UI_Title_DropReward");
const FTextKey RANKING_REWARD_GUID_TEXT_KEY = TEXT("RankingRewardGuide");

void UGsUIPopupBossRankReward::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_closeButton->OnClicked.AddDynamic(this, &UGsUIPopupBossRankReward::OnClose);
	_guidButton->OnClicked.AddDynamic(this, &UGsUIPopupBossRankReward::OnClickGuid);	
}

void UGsUIPopupBossRankReward::NativeConstruct()
{
	Super::NativeConstruct();

	bossRankRewardMap.Empty();

	_guidPanel->Close();	
	_rewardListView->SetScrollbarVisibility(ESlateVisibility::Hidden);

	FText::FindText(FIELD_BOSS_TEXT_KEY, UI_TEXT_MAIL_REWARD_TEXT_KEY, _mailTitleText);
	FText::FindText(FIELD_BOSS_TEXT_KEY, UI_TEXT_DROP_REWARD_TEXT_KEY, _dropTitleText);
	FText::FindText(FIELD_BOSS_TEXT_KEY, UI_TITLE_MAIL_REWARD_TEXT_KEY, _mailTitleFormatText);
	FText::FindText(FIELD_BOSS_TEXT_KEY, UI_TITLE_DROP_REWARD_TEXT_KEY, _dorpTitleFormatText);
	FText::FindText(FIELD_BOSS_TEXT_KEY, RANKING_REWARD_GUID_TEXT_KEY, _guidText);

	_guidPanel->SetDescText(_guidText);
}

/*
* 필드 보스 리워드 데이터의 순위는 기획자가 설정하는 대로 설정되어 있다. 
* 예로 1~5, 6~10, 11~100 이런식으로 설정되어 있다.
* 기획자가 집행해 주는데로 설정되어 있어서 1~5, 1~10, 1~100, 6~10, 6~100 이처럼 집행할 수 도 있는데
* 잘못된 데이터 집행시 기획자에게 문의해야한다. 
*/
void UGsUIPopupBossRankReward::InitRewardList()
{
	const UGsTable* table = FGsSchemaFieldBossRewardData::GetStaticTable();
	if (nullptr == table)
	{
		return;
	}

	//보스 몬스터 리워드 데이터이 포함된 리워드 중 메일 형태의 리워드만 얻어와야한다. 
	TArray<const FGsSchemaFieldBossRewardData*> rewardDataArray;	
	for (const FGsSchemaFieldBossRewardDataRow& rewardData : _selectBossData->bossRewardIdList)
	{
		if (rewardData.GetRow()->giveType != _giveType)
		{
			continue;
		}

		rewardDataArray.Add(rewardData.GetRow());
	}

	//같은 범위의 리워드끼리 묶기
	bossRankRewardMap.Empty();
	for (const FGsSchemaFieldBossRewardData* rewardData : rewardDataArray)
	{
		Add(rewardData->contributionRankMin, rewardData->contributionRankMax, rewardData->rewardId.GetRow());
	}

	//범위 정렬, min rank가 같을 때 max rank 비교
	bossRankRewardMap.KeySort([](const FGsRange& leftRank, const FGsRange& rightRank)->bool {
			
			if (leftRank._min < rightRank._min)
			{
				return true;
			}

			if(leftRank._min == rightRank._min)
			{
				return leftRank._max < rightRank._max;
			}

			return false;
		});

	//item만 수정
		
	TArray<UGsBossRewardListViewEntryData*> iconListViewEntryDataArray;
	UGsBossRewardListViewEntryData* listViewEntryData = nullptr;
	const UGsTableItemCommon* itemTable = Cast<UGsTableItemCommon>(FGsSchemaItemCommon::GetStaticTable());
	const FGsSchemaItemCommon* rightItem = nullptr;
	const FGsSchemaItemCommon* leftItem = nullptr;

	for (TPair<FGsRange, FGsFieldBossRewardArray>& rewardRankPair : bossRankRewardMap)
	{		
		listViewEntryData = NewObject<UGsBossRewardListViewEntryData>();				
		listViewEntryData->_maxRank = rewardRankPair.Key._max; 
		listViewEntryData->_minRank = rewardRankPair.Key._min;

		TArray<TSharedPtr<FGsBossRewardInfo>> itemArray;
		for (const FGsSchemaRewardData* reward : rewardRankPair.Value._rewardDataArray)
		{
			const TArray<FGsSchemaRewardBox>& rewadBoxArray = reward->rewardBoxList;
			for (const FGsSchemaRewardBox& rewardBox : rewadBoxArray)
			{
				const FGsSchemaRewardBoxData* rewardBoxData = rewardBox.rewardBoxDataRow.GetRow();
				if (nullptr == rewardBoxData)
				{
					continue;
				}

				for (const FGsSchemaRewardItemBagData& itemBag : rewardBoxData->itemBagList)
				{
					TSharedPtr<FGsBossRewardInfo>* findReward = itemArray.FindByPredicate([&](TSharedPtr<FGsBossRewardInfo> rewardInfo)->bool {
						return rewardInfo->_itemId == itemBag.itemId;
						});

					if (nullptr == findReward)
					{
						TSharedPtr<FGsBossRewardInfo> rewardInfo = MakeShareable(new FGsBossRewardInfo(itemBag.itemId, itemBag.itemCountMax));
						itemArray.Add(rewardInfo);
					}					
				}
			}
		}

		//item grade, id 별로 정렬
		itemArray.Sort([&](const TSharedPtr<FGsBossRewardInfo>& leftBossRewardInfo, const TSharedPtr<FGsBossRewardInfo>& rightBossRewardInfo)->bool{
			if (itemTable)
			{
				itemTable->FindRowById(rightBossRewardInfo->_itemId, rightItem);
				itemTable->FindRowById(leftBossRewardInfo->_itemId, leftItem);

				if ((int32)leftItem->grade > (int32)rightItem->grade)
				{
					return true;
				}
				else if ((int32)leftItem->grade == (int32)rightItem->grade)
				{
					return leftItem->id > rightItem->id;
				}
			}

			return false;
		});

		listViewEntryData->_rewardArray = itemArray;
		iconListViewEntryDataArray.Add(listViewEntryData);
	}

	_rewardListView->SetListItems(iconListViewEntryDataArray);
}

void UGsUIPopupBossRankReward::OnClose()
{
	Close();	
}

void UGsUIPopupBossRankReward::OnClickGuid()
{
	_guidPanel->ToggleOpenClose();
}

void UGsUIPopupBossRankReward::InitReward(const FGsSchemaFieldBossData* inBossInfo, const FGsSchemaNpcData* inNpcData, BossRewardGiveType inGiveType)
{	
	_selectBossData = inBossInfo;
	_selectNpcData = inNpcData;
	_giveType = inGiveType;

	switch (inGiveType)
	{
	case BossRewardGiveType::MAIL:
	{
		_nameTextBlock->SetText(FText::Format(_mailTitleFormatText, inNpcData->nameText));
		_titleTextBlock->SetText(_mailTitleText);
	}
		break;
	case BossRewardGiveType::FILED_DROP:
	{
		_nameTextBlock->SetText(FText::Format(_dorpTitleFormatText, inNpcData->nameText));
		_titleTextBlock->SetText(_dropTitleText);
	}
		break;
	default:
		break;
	}	

	InitRewardList();
}

void UGsUIPopupBossRankReward::Add(int32 inMinRank, int32 inMaxRank, const FGsSchemaRewardData* inBossInfo)
{
	FGsRange rank(inMinRank, inMaxRank);

	if (false == bossRankRewardMap.Contains(rank))
	{
		bossRankRewardMap.Emplace(rank, FGsFieldBossRewardArray());
	}

	if (false == bossRankRewardMap[rank]._rewardDataArray.Contains(inBossInfo))
	{
		bossRankRewardMap[rank]._rewardDataArray.Emplace(inBossInfo);
	}
}

void UGsUIPopupBossRankReward::OnInputCancel()
{
	OnClose();
}