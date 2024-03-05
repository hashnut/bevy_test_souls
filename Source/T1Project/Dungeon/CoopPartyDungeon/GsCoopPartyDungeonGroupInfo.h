#pragma once

#include "CoreMinimal.h"
#include "Dungeon/GsDungeonGroupInfo.h"
#include "Currency/GsCostPackage.h"
#include "Unlock/GsUnlockDefine.h"
#include "Shared/Client/SharedEnums/SharedPartyDungeonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Message/GsMessageItem.h"
#include "Message/GsMessageContents.h"


/**
 * 사용하지 않는 것. 삭제 요망
 */


struct FGsItemMessageParamBase;

DECLARE_DELEGATE(FOnRequestRefreshUI);

class FGsCoopPartyDungeonGroupInfo : public FGsDungeonGroupInfo
{
private:
	static const int32 NoneNum = 0;

	// 2022/05/23 PKT - 현재까지 진행 한 난이도(클리어 난이도)
	DungeonDifficulty _clearDifficulty = DungeonDifficulty::NONE;

	ItemDBId _dungeonKeyId = INVALID_ITEM_DB_ID;

	// 2022/05/23 PKT - 보상 받을 수 있는 아이템의 갯수
	TSharedRef<const FGsCostPackage> _costDungeonKey;

	// 2022/05/25 PKT - 주간 보상 획득 갯수
	int32 _weeeklyRewardCount = NoneNum;

	// 2022/05/31 PKT - 던전 제한 시간
	int32 _timeLimitM;

	/************************************************************************/
	/* Message	                                                            */
	/************************************************************************/
private:
	// 2022/05/24 PKT - 아이템 관련 이벤트..
	TArray<TPair<MessageItem, FDelegateHandle>>	_listItemActionDelegates;

	// 2022/05/25 PKT - UnLock Contents
	TPair<MessageContentUnlock, FDelegateHandle> _unlockDelegate;

public:
	FOnRequestRefreshUI _onRequestRefreshUI;

public:
	explicit FGsCoopPartyDungeonGroupInfo(const struct FGsSchemaDungeonGroupData* InDungeonGroupData, DungeonDifficulty InDungeonDifficulty);
	
	virtual void Clear() override;

	/************************************************************************/
	/* Message Delegate                                                     */
	/************************************************************************/
private:
	void RegisterMessage();
	void UnRegisterMessage();

private:
	// 2022/05/24 PKT - Item 상태 변화
	void AddItem(FGsItemMessageParamBase& InParam);
	void UpdateItem(FGsItemMessageParamBase& InParam);
	void RemoveItem(FGsItemMessageParamBase& InParam);
	void ChangedDungeonKey(const ItemId InItemId);
		
	// 2022/05/25 PKT - UnLock Contents
	void OnContentsUnlock(const struct IGsMessageParam* InParam);

	// 2022/05/25 PKT - UI 갱신 요청
	void RequestRefreshUI();

public:
	/************************************************************************/
	/* 2022/05/23 PKT - setter                                              */
	/************************************************************************/
	// 2022/05/25 PKT - 진입 가능한 난이도 설정(서버에서 설정을 내려 줌)
	void SetDifficulty(DungeonDifficulty InType);
	
public:
	/************************************************************************/
	/* 2022/05/23 PKT - getter                                              */
	/************************************************************************/
	// 2022/05/25 PKT - Dungeon Group Open Condition
	//bool IsUnLockContents() const;
	
	// 2022/05/25 PKT - Dungeon Group Open Condition Type
	//EGsUnlockContentsType GetUnLockContensType() const;

	// 2022/06/13 PKT - 입장 가능한 난이도
	//DungeonDifficulty IsPossibleDifficulty() const;
	
	// 2022/05/23 PKT - 현재까지 진행 한 난이도(클리어 난이도)
	DungeonDifficulty GetClearDifficulty() const;

	// 2022/05/25 PKT - 주간 보상 가능 횟수
	int32 GetWeeeklyRewardCount() const;

	// 2022/05/25 PKT - 던전 보상 받을 수 있는 아이템 갯수
	CurrencyDelta GetDungeonKeyAmount() const;
	
public:
	/************************************************************************/
	/* 2022/05/23 PKT - Finder                                              */
	/************************************************************************/
	// 2022/05/25 PKT - 난이도 별 던전 Data
	const FGsSchemaDungeonData* FindDungeonDataByDifficulty(DungeonDifficulty InType) const;

private:
	TSharedRef<const FGsCostPackage> MakeCostPackage(const ItemId InDungeonKeyId);
};
