#include "GsBattlePassMissionData.h"
#include "Algo/ForEach.h"
#include "Algo/Copy.h"

FGsBattlePassMissionTab::FGsBattlePassMissionTab(const FGsSchemaBattlePassMissionTab* tabData)
	: _data(tabData)
	, _tabDetail(tabData != nullptr ? tabData->tabDetail.GetRow() : nullptr)
{
//	FDateTime::Parse(_data->tabOpenDate, _tabOpenDate);
	_missions.Reset();
}

void FGsBattlePassMissionTab::UpdateMissionTab()
{
	if (_data)
	{
		_isCompleteTab = GetTabCurrentExp() >= _completeExp;
		_missions.StableSort([](TWeakPtr<FGsBattlePassMissionData> le, TWeakPtr<FGsBattlePassMissionData> re) {

			auto leLastState = !le.IsValid() || le.Pin()->IsCompleteMission();
			auto reLastState = !re.IsValid() || re.Pin()->IsCompleteMission();

			return !leLastState && reLastState;
		});
	}
}

uint64 FGsBattlePassMissionTab::GetTabCurrentExp()
{
	uint64 currentExp = _completeExp;
	if (!_isCompleteTab)
	{
		currentExp = 0;
		Algo::ForEach(_missions, [&currentExp](auto e) { currentExp += e.IsValid() ? e.Pin()->GetAchievePoint() : 0; });
	}
	return _completeExp;
}

bool FGsBattlePassMissionTab::IsOpenTab() 
{ 
	return _openPass; // && (FGsTimeSyncUtil::GetServerNowDateTime() >= _tabOpenDate);
}

bool FGsBattlePassMissionTab::IsMissionCompleteTab()
{
	if (!_isCompleteTab) _isCompleteTab = GetTabCurrentExp() >= _completeExp;
	return _isCompleteTab;
}

void FGsBattlePassMissionTab::OnMissionInfoChanged(BattlePassMissionId id, uint64 missionPoint)
{
	if (auto mission = GetMission(id).Pin())
	{
		mission->SetAchieveMissionPoint(missionPoint);
	}
}

int FGsBattlePassMissionTab::GetMissionData(OUT TArray<TWeakPtr<FGsBattlePassMissionData>>& missions)
{
	missions.Append(_missions);

	return missions.Num();
}

TWeakPtr<FGsBattlePassMissionData> FGsBattlePassMissionTab::GetMission(BattlePassMissionId missionId) 
{
	if (auto iter = _missions.FindByPredicate([missionId](auto e) {return e.IsValid() && e.Pin()->GetMissionId() == missionId;}))
		return *iter;
	return nullptr;
}

//FString FGsBattlePassMissionTab::GetTabOpenDate()
//{
//	FDateTime clientDateTime = FGsTimeSyncUtil::ConvertSeverDateTimeToClientDateTime(_tabOpenDate);
//	return clientDateTime.ToString(TEXT("%y.%m.%d %H:%M"));
//}

void FGsBattlePassMissionTab::ResetMissionTab()
{
	Algo::ForEach(_missions, [](auto e) { e.Pin()->ResetMission(); });
}