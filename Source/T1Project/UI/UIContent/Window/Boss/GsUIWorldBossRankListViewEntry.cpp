#include "GsUIWorldBossRankListViewEntry.h"
#include "Components/TextBlock.h"
#include "Reward/GsRewardHelper.h"
#include "UI/UIContent/Common/Entry/GsUIRewardItemIconListViewEntry.h"
#include "Components/ListView.h"
#include "DataSchema/Guild/GsSchemaGuildEmblem.h"
#include "Components/Image.h"
#include "Guild/GsGuildHelper.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "PaperSprite.h"
#include "Components/WidgetSwitcher.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "Shared/Client/SharedEnums/SharedIffEnum.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "UTIL/GsTableUtil.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "UMG/Public/Components/Widget.h"

void UGsUIWorldBossRankListViewEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UGsUIWorldBossRankListViewEntry::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGsUIWorldBossRankListViewEntry::NativeOnListItemObjectSet(UObject* inListItemObject)
{
	UGsUIWorldBossRankListViewEntryData* data = Cast<UGsUIWorldBossRankListViewEntryData>(inListItemObject);
	if (data)
	{
		Set(data->_worldBossData);		
	}
}

void UGsUIWorldBossRankListViewEntry::Set(const FGsWorldBossRankData& inRankData, UGsUIWorldBossRankListViewEntry::EViewType InType /* = EViewType::RANK */)
{
	Clear();

	if (false == inRankData.IsValid())
	{
		return;
	}

	/**
	 * User Name / Damage Text
	 */
	if (PVPAlignmentGrade::CHAOTIC == inRankData._pvpGrade)
	{
		_userNameTextBlock->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::PVP_ALIGNMENT_CHAOS));
	}
	else
	{
		_userNameTextBlock->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::PVP_ALIGNMENT_NEUTRALITY));
	}

	_userNameTextBlock->SetText(FText::FromString(inRankData._userName));

	FText damageText = FText::AsNumber(inRankData._contributePoint);
	_damageTextBlock->SetText(damageText);

	/**
	 * Ranking
	 */
	_rankWidgetSwitcher->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	_rankTextBlock->SetVisibility(ESlateVisibility::Collapsed);

	if (EViewType::LAST_HIT != InType)
	{
		const static int32 SHOW_IMG_LIMIT_RANK = 3;
		if (inRankData._rank <= SHOW_IMG_LIMIT_RANK)
		{	// 2023/11/2 PKT - 1~3등까지는 Image로 보여줌.
			_rankWidgetSwitcher->SetActiveWidgetIndex(inRankData._rank - 1);
		}
		else
		{	// 2023/11/2 PKT - 그외 쩌리's..
			const static int32 SWITCHER_RANK_TEXT_INDEX = 3;
			_rankWidgetSwitcher->SetActiveWidgetIndex(SWITCHER_RANK_TEXT_INDEX);
			_rankTextBlock->SetText(FText::FromString(FString::FromInt(inRankData._rank)));
			_rankTextBlock->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
	else if (EViewType::LAST_HIT == InType)
	{
		const static int32 SWITCHER_LAST_HIT_INDEX = 4;
		_rankWidgetSwitcher->SetActiveWidgetIndex(SWITCHER_LAST_HIT_INDEX);
	}
	

	/**
	 * Guild
	 */
	bool isHideGuildInfo = inRankData._guildName.IsEmpty();
	bool isShowGuildEmblem = false;
	if (false == isHideGuildInfo)
	{
		if(const FGsSchemaGuildEmblem* emblemData = FGsGuildHelper::GetGuildEmblem(inRankData._guildEmblemId))
		{
			UPaperSprite* sprite = FGsUIHelper::LoadSpriteFromPath(emblemData->iconPathSmall);
			_imgGuildEmblem->SetBrushFromAtlasInterface(sprite);
			isShowGuildEmblem = true;			
		}

		FLinearColor color = FLinearColor::White;
		if (GGuild()->IsEnemyGuild(inRankData._guildDBId))
		{
			color = FGsUIColorHelper::GetColor(EGsUIColorType::GUILD_ENEMY_COLOR);
		}
		else if (GGuild()->IsAllyGuild(inRankData._guildDBId))
		{
			color = FGsUIColorHelper::GetColor(EGsUIColorType::GUILD_ALLY_COLOR);
		}
		_guildNameTextBlock->SetColorAndOpacity(color);
		_guildNameTextBlock->SetText(FText::FromString(inRankData._guildName));
	}

	_imgGuildEmblem->SetVisibility(isShowGuildEmblem ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	
	/**
	 * Reward
	 */
	_borderAllReward->SetVisibility(EViewType::MY_INFO == InType ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);

	TArray<UGsUIItemIconListViewEntryData*> iconListViewEntryDataArray;
	TArray<FGsItemIconData> tempSet;

	if (EViewType::RANK == InType)
	{
		tempSet = inRankData._rewardIdList;
	}
	else if (EViewType::LAST_HIT == InType)
	{
		tempSet = inRankData._lastHitRewardIdList;
	}
	else if (EViewType::MY_INFO == InType)
	{
		tempSet = inRankData._rewardIdList;
		tempSet += inRankData._lastHitRewardIdList;
		tempSet += inRankData._basicRewardIdList;
	}
	
	for (int32 i = 0; i < tempSet.Num(); ++i)
	{
		UGsUIItemIconListViewEntryData* listViewEntryData = NewObject<UGsUIItemIconListViewEntryData>();
		FGsItemIconData itemIconData;
		itemIconData.Init(tempSet[i]._itemId, tempSet[i]._itemAmount);
		listViewEntryData->_itemDataArray.Add(itemIconData);

		iconListViewEntryDataArray.Add(listViewEntryData);
	}

	iconListViewEntryDataArray.Sort([](const UGsUIItemIconListViewEntryData& source, const UGsUIItemIconListViewEntryData& dest)->bool {
		const FGsSchemaItemCommon* sourceItem = UGsTableUtil::FindRowById<UGsTableItemCommon, FGsSchemaItemCommon>(source._itemDataArray[0]._itemId);
		const FGsSchemaItemCommon* destItem = UGsTableUtil::FindRowById<UGsTableItemCommon, FGsSchemaItemCommon>(dest._itemDataArray[0]._itemId);

		if (nullptr == sourceItem)
		{
			return false;
		}

		if (nullptr == destItem)
		{
			return false;
		}

		if (sourceItem->grade > destItem->grade)
		{
			return true;
		}
		else if (sourceItem->grade == destItem->grade)
		{
			return false;
		}

		return false; });

	_rewardListView->SetListItems(iconListViewEntryDataArray);
	_rewardListView->SetScrollbarVisibility(ESlateVisibility::Collapsed);
}

void UGsUIWorldBossRankListViewEntry::Clear()
{
	_rankWidgetSwitcher->SetActiveWidgetIndex(3);
	_rankTextBlock->SetText(FText::GetEmpty());
	_userNameTextBlock->SetText(FText::GetEmpty());
	_guildNameTextBlock->SetText(FText::GetEmpty());
	_damageTextBlock->SetText(FText::GetEmpty());
	
	_imgGuildEmblem->SetVisibility(ESlateVisibility::Hidden);
	_rewardListView->ClearListItems();
}
