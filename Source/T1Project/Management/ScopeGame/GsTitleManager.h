#pragma once

/**
* file		GsTitleManager.h
* @brief	칭호 시스템 메니져 클래스
* @author	PKT
* @date		2022/01/14
**/


#include "CoreMinimal.h"
#include "Classes/GsManager.h"
#include "RedDot/GsRedDotCheck.h"
#include "Title/GsTitleData.h"
#include "Title/GsTitleCollectionReward.h"
#include "Title/GsTitleEquip.h"
#include "Title/GsTitleFilter.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Message/GsMessageGameObject.h"
#include "Shared/Client/SharedEnums/SharedTitleEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"


class T1PROJECT_API FGsTitleManager final : public IGsManager, public IGsRedDotCheck
{
	const static int32 ALL_CATEGORY_INDEX = 0;

	/************************************************************************/
	/* Member                                                               */
	/************************************************************************/
	// 2022/01/19 PKT - Sub Category Data Holder
	TMap<TitleEquipSlot, TArray<const struct FGsSchemaTitleSubCategory*>> _categoryDataHolder;

	// 2022/01/19 PKT - Title Data Holder
	TArray<FGsTitleData> _titleDataHolder;

	// 2022/01/19 PKT - Title Data By Sub Category ID
	TMap<TitleCategoryId, TArray<FGsTitleData*>> _titleDatasByCategoryId;
	 
	// 2022/01/19 PKT - Colllection Reward Data
	FGsTitleCollectionReward _collectionRewardData;

	// 2022/01/19 PKT - Collection Euqip Data
	FGsTitleEquip _equipData;

	// 2022/01/19 PKT - Title In Stat All
	TArray<StatType> _titleStatAll;

	// 2022/01/19 PKT - current Window View List
	mutable TArray<const FGsTitleData*> _viewList;

	// 2022/01/19 PKT - not Send Message To Window 
	bool isSkipMessage = false;

	// 2022/01/21 PKT - RedDot Collector
	TMap<TitleCategoryId, TArray<TitleId>> _reddotCollector;

	// 2022/02/03 PKT - Spawn Message
	MsgGameObjHandleArray _listGameObjectDelegate;

public:
	virtual void Initialize() final;
	virtual void Finalize() final;

	bool CheckRedDot(bool bInUpdate = true) override;

	void TitleConfirm(TitleEquipSlot InSlot, int32 InIndex);

	void CollectionConfirm();

	/************************************************************************/
	/* Message                                                              */
	/************************************************************************/
private:
	void RegisterMessages();
	void UnegisterMessages();
	void SendToWindowMessage(MessageContentTitle InType, const struct IGsMessageParam* InMessage);

	/************************************************************************/
	/* Set Schema Data                                                      */
	/************************************************************************/
private:	
	void BuildCategoryData();

	void BuildTitle();

	void UpdateBillboardTitle();

	/************************************************************************/
	/* Delegate                                                             */
	/************************************************************************/
private:
	void OnLocalPlayerSpawnComplete(const IGsMessageParam* InParam);

	void OnStateChangedTitleData(const FGsTitleData* InData, ETitleMessageReason InMessage);

	/************************************************************************/
	/* Server -> Client                                                     */
	/************************************************************************/
public:
	// 2022/01/21 PKT - 칭호 관련 초기 세팅값
	void InitTitleData(const TArray<TitleInfo>& InTitleList, const TPair<TitleId, TitleId>& InEquipIds);

	// 2022/01/21 PKT - 칭호 상태(진행도) 변경
	void UpdateTitle(const TitleInfo& InTitleInfo);

	// 2022/01/21 PKT - 칭호 상태(장착 유무) 변경
	void PlayerEquipTitle(TitleId InEquipTitle, TitleId InUnEquipTitle);

	// 2022/02/03 PKT - 
	void RemotePlayerEquipTitle(int64 InGameId, TitleId InFrontId, TitleId InBackId);

	/************************************************************************/
	/* Get / Find                                                           */
	/************************************************************************/
private:
	FGsTitleData* FindTitleDataInHolder(TitleId InId);

public:	
	int32 GetCategoryCount(TitleEquipSlot InSlot) const;

	bool FindCategoryList(TitleEquipSlot InSlot, OUT TArray<const FGsSchemaTitleSubCategory*>& OutList) const;

	bool IsMainCategoryRedDot(TitleEquipSlot InSlot) const;

	bool IsSubCategoryRedDot(TitleEquipSlot InSlot, int32 InIndex) const;

	const TArray<const FGsTitleData*>& MakeViewList(TitleEquipSlot InMainCatetgoryType
		,  const int32 InSubCategoryIndex
		, const TArray <TSharedPtr<IGsTitleFilterConditionBase>>& InFilters) const;
		
	const FGsTitleData* FindTitleDataInViewListByTitleDataId(TitleId InId) const;

	const TArray<StatType>& GetToHaveStatAll() const;

	const FGsTitleCollectionReward* GetCollectionRewardData() const;

	const FGsTitleEquip* GetTitleEquipData() const;
};

#define GTitle() UGsScopeHolder::GetGameManagerFType<FGsTitleManager>(UGsScopeGame::EManagerType::Title) 