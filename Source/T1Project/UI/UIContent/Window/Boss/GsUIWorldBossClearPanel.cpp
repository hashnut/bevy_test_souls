#include "GsUIWorldBossClearPanel.h"
#include "GameObject/Boss/GsBossHandler.h"
#include "GsUIWorldBossRankListViewEntry.h"
#include "GameObject/Boss/GsWorldBossHandler.h"
#include "Components/ListView.h"

void UGsUIWorldBossClearPanel::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGsUIWorldBossClearPanel::InvalidResult()
{
	TArray<FGsWorldBossRankData> worldBossRankDataArray = FGsBossHandler::GetInstance()->GetWorldBossHandler()->GetWorldBossRankDataArray();
	UGsUIWorldBossRankListViewEntryData* data = nullptr;
	TArray<UGsUIWorldBossRankListViewEntryData*> dataArray;
	for (int32 i = 0; i < worldBossRankDataArray.Num(); ++i)
	{
		data = NewObject<UGsUIWorldBossRankListViewEntryData>(this);
		data->_worldBossData = worldBossRankDataArray[i];
		dataArray.Add(data);
	}
	_rankListView->SetListItems(dataArray);

	FGsWorldBossRankData playerRankData = FGsBossHandler::GetInstance()->GetWorldBossHandler()->GetPlayerRankData();
	_playerRankData->Set(playerRankData, UGsUIWorldBossRankListViewEntry::EViewType::MY_INFO);

	FGsWorldBossRankData lastHitRankData = FGsBossHandler::GetInstance()->GetWorldBossHandler()->GetLastHitRankData();
	_lastHitRankData->Set(lastHitRankData, UGsUIWorldBossRankListViewEntry::EViewType::LAST_HIT);

}
