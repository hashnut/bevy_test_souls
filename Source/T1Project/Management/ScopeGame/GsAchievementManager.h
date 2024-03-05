#pragma once
#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedAchievementEnum.h"
#include "Classes/GsManager.h"

#include "Management/GsScopeHolder.h"
#include "DataSchema/GsSchemaEnums.h"

#include "GsAchievementManager.generated.h"

//임시
//struct AchievementThemeItem
//{
//	AchievementThemeId id{ INVALID_ACHIEVEMENT_THEME_ID };
//	uint64  achievePoint{ 0 }; //비율로 100 넘을 수도 있음
//	bool  isRewarded{ false };
//};
struct AchievementItem
{
	AchievementId id{ INVALID_ACHIEVEMENT_ID };
	uint64  achievePoint{ 0 };
	bool  isRewarded{ false };
};

struct FGsAchievementData;

UCLASS()
class T1PROJECT_API UGsAchievementManager : public UObject, public IGsManager
{
	GENERATED_BODY()
public:
	using AchievementKey = AchievementGroupId;

	using AchievementList = TArray<TWeakPtr<FGsAchievementData>>;
	using MainAchievementPair = TPair<TWeakPtr<FGsAchievementData>, AchievementList>; //추가

private:
	TMap<AchievementId, TSharedPtr<FGsAchievementData>> _achievements; //모두 가지고 있자
	TMap<AchievementKey, TArray<TWeakPtr<FGsAchievementData>>> _achievementLevelData;		// 같은 그룹의 레벨별 업적 ID -> 업적 데이터 진행되지 않은 업적도 같이 보여주는 기획에 따라
	TMap<int32, AchievementList> _achievementsTabList; //현재 실행중인 achievement

public:
	void Initialize() override;
	void Finalize() override;

public:
	void OnUpdateAchievementList(TArray<AchievementItem>& achievementItems, bool isComplete = false, bool isNewAdd = false); //TArray<AchievementThemeItem>& themesItems, 
	void OnUpdateAchievement(AchievementItem& achievementItem);
	void OnUpdateAchievementProgress(AchievementId achievementId, uint64 achievePoint);
	//theme는 완료로 남아 있을듯 하여 삭제가 필요없을 것 같지만, 하위 업적은 reward 되고 다음 레벨이 없을 때 삭제될 수도 있지 않나..
	void OnRemoveAchievement(AchievementId id); 

	void OnAckReceivedAchievementReward(AchievementId id);
	void OnAckAchievementRewardList(const TArray<CurrencyDeltaPair>& currencyPairList, const TArray<ItemIdAmountPair>& itemAmountPairList);
	void OnAckReceivedRewardAll(const TArray<AchievementId>& achievementIds); //const TArray<AchievementThemeId>& themeIds, 

public:
	bool IsRedDot();
	bool IsTabRedDot(int32 tabIndex);

	int GetGoOnAchievementsList(int32 tabIndex, OUT MainAchievementPair& pairList) const;
	int GetGroupAchievements(AchievementGroupId id, OUT AchievementList& achievements) const;
	TWeakPtr<FGsAchievementData> FindAchievementItem(AchievementId achievementId);

public:
	int32 GetMaxTabIndex();
	TWeakPtr<FGsAchievementData> FindAchievementDataByConditionType(AchievementType condType);

protected:
	void InitAchievementData();
	int UpdateAchievement(AchievementItem& achievementItem, bool isNewAdd = false);

private:
	void SortAchievements(int32 tabIndex);
};

#define GSAchivement() UGsScopeHolder::GetGameManager<UGsAchievementManager>(UGsScopeGame::EManagerType::Achievement)

