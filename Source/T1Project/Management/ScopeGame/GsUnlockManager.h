#pragma once

#include "CoreMinimal.h"
#include "Classes/GsManager.h"

#include "Management/GsScopeHolder.h"
#include "Management/GsScopeGame.h"

#include "Unlock/GsUnlockDefine.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedUnlockEnum.h"

#include "Message/GsMessageContents.h"
#include "Message/GsMessageUserInfo.h"
#include "Message/GsMessageGameObject.h"


struct IGsMessageParam;
struct FGsSchemaUnlockContents;
namespace PD { namespace SC { struct PKT_SC_CONTENTS_LOCKED_READ; } }
namespace PD { namespace GC 
{
	struct PKT_GC_CONTENTS_LOCKED_READ;
	struct PKT_GC_UNLOCKINFO_READ;
	struct PKT_GC_UNLOCK_CONTENTS_LIST_READ;
	struct PKT_GC_NOTICE_UNLOCK_CONTENTS_UPDATE_READ;
} }
/**
 *  언락 매니저
 *	DTGsUnlockContents 에 존재하는 데이터만 적용할 수 있습니다.
 */
class T1PROJECT_API FGsUnlockManager final : public IGsManager
{
protected:
	TMap<EGsUnlockContentsType, TArray<UnlockContentsId>> _contentsIdListMap; // (클라Type, 서버ID 목록)
	TMap<UnlockContentsId, const FGsSchemaUnlockContents*> _contentsControlDataMap;	// 데이터 캐싱

	TSet<UnlockContentsId> _lockedContents;								// 유형1 : 설정된 값에 의해 잠긴 컨텐츠
	TMap<UnlockCategory, TArray<UnlockContentsId>> _lockedContentsCategory;	// 유형2 : 상황에 연계되어 잠긴 컨텐츠(침공전, 관전 등)
	TSet<UnlockContentsId> _lockedContentsServer;						// 유형3 : 서버에 의해 잠긴 컨텐츠 (운영툴, 버그 등)
	
	TSet<UnlockContentsId> _isNewContents;	// 새로 해제된 컨텐츠
	
protected:
	TArray<TPair<MessageContents, FDelegateHandle>> _listContentsDelegate;
	TArray<TPair<MessageContentQuest, FDelegateHandle>> _questDelegate;
	TPair<MessageUserInfo, FDelegateHandle> _userInfoDelegate;
	MsgGameObjHandle _gameobjectDelegate;

protected:
	int32 _maxSerialNumber = 0;		// 데이터 중 가장 큰 S/N
	int32 _lastSerialNumber = 0;	// 접속 전 마지막으로 동기화한 S/N
	int32 _maxLevel = 0; // 자신이 찍었던 최고 레벨. 레벨 다운 후에도 컨텐츠 언락 유지
	bool _spawnComplete = false;

protected:
	bool _allUnlockCheat = false;

protected:
	TArray<UnlockContentsId> _reservedTicker;

	// 가상 함수
public:
	virtual void Initialize() override;
	virtual void Finalize() override;

protected:
	void RegisterMessages();
	void UnregisterMessages();

public:
	void OnReceiveUnlockInfo(PD::GC::PKT_GC_UNLOCKINFO_READ& InPacket);

public:
	// 관련된 컨텐츠의 잠금 상태를 확인한다.
	bool IsContentsUnlock(UnlockContentsId InContentsId);
	bool IsContentsUnlock(UnlockContentsId InContentsId, bool InShowTicker);
	bool IsContentsUnlock(EGsUnlockContentsType InContentsType);
	bool IsContentsUnlock(EGsUnlockContentsType InContentsType, bool InShowTicker);
	bool IsContentsUnlock(ItemCategoryMain InItemCategoryMain, bool InShowTicker = false);
	bool IsContentsUnlock(ItemCategorySub InItemCategorySub, bool InShowTicker = false);

	UnlockType GetUnlockType(UnlockContentsId InContentsId) const;	// 잠긴 유형 UnlockCondition, UnlockCategory 판단

	UnlockCondition GetLockCondition(UnlockContentsId InContentsId) const;	// 잠긴 원인 Condition 반환
	UnlockCategory GetLockCategory(UnlockContentsId InContentsId) const;	// 잠긴 원인 Category 반환

	// 관련된 컨텐츠의 오픈 여부를 확인한다.
	bool IsNewContents(UnlockContentsId InContentsId);
	bool IsNewContents(EGsUnlockContentsType InContentsType);
	void SetIsNewContents(UnlockContentsId InContentsId, bool InIsShow);
	void SetIsNewContents(EGsUnlockContentsType InContentsType, bool InIsShow);

	// 현재 컨텐츠의 잠긴 원인을 출력한다. (티커)
	bool ShowLockTicker(UnlockContentsId InContentsId, UnlockCondition InCondition);
	bool ShowLockTicker(UnlockContentsId InContentsId, UnlockCategory InCondition);
	bool ShowLockTicker(UnlockContentsId InContentsId);
	bool ShowLockTicker(EGsUnlockContentsType InContentsType);
	bool ShowLockTicker(ItemCategorySub InItemCategorySub);

	// 관련된 컨텐츠의 언락 연출을 출력한다.
	void ShowUnlockTicker(UnlockContentsId InContentsId);
	void ShowUnlockTicker(EGsUnlockContentsType InContentsType);
	const FGsSchemaUnlockContents* PopReservedTickerData();
	
	void Set(PD::GC::PKT_GC_UNLOCK_CONTENTS_LIST_READ& InPacket);
	void Set(PD::GC::PKT_GC_NOTICE_UNLOCK_CONTENTS_UPDATE_READ& InPacket);

	// 서버에 의한 컨텐츠 잠금 및 해제
	void LockContentsByServer(UnlockContentsId InContentsId);
	void UnlockContentsByServer(UnlockContentsId InContentsId);

	// 클라이언트에 의한 컨텐츠 잠금 및 해제 ( 관전모드 등 )
	void LockContentsByClient(UnlockCategory InCategory);
	void UnlockContentsByClient(UnlockCategory InCategory);

	// 컨텐츠의 잠금 해제
	void UnlockContents(UnlockContentsId InContentsId, bool InIsShowUnlockTicker = false, bool InIsNew = false);

	// 레벨 업/퀘스트 완료 후 잠긴 컨텐츠의 상태를 갱신한다.
	void UpdateLockedContentsByLevel(bool InIsShowUnlockTicker = true);
	void UpdateLockedContentsByQuest(bool InIsShowUnlockTicker = true);

	// 2022/05/26 PKT - UnLock사유를 가져온다.
	bool GetUnLockMessage(UnlockContentsId InContentsId, FText& OutMessage) const;
	bool GetUnLockMessage(EGsUnlockContentsType InUnlockContentsType, FText& OutMessage) const;

	// 컨텐츠 이름 반환
	bool GetContentsName(EGsUnlockContentsType InUnlockContentsType, OUT FText& OutText) const;

protected:
	const FGsSchemaUnlockContents* GetUnlockData(UnlockContentsId InContentsId) const;
	const FGsSchemaUnlockContents* GetUnlockData(EGsUnlockContentsType InContentsType) const;

protected:
	void OnMainQuestAccepted(const IGsMessageParam*);
	void OnMainQuestRewarded(const IGsMessageParam*);
	void OnChangeLocalPlayerLevel(uint64 InLevel);
	void OnLocalPlayerSpawnComplete(const IGsMessageParam* InParam);

public:
	void UnlockAllContentsCheat();
	void UnlockResetCheat();

public:
	void FindQuestConditionText(UnlockContentsId InContentsId, OUT FText& OutText) const;

public:
	bool MakeStringUnLockCondition(UnlockContentsId InContentsId, UnlockCondition InCondition, FText& OutMessage) const;
	bool MakeStringUnLockCategory(UnlockContentsId InContentsId, UnlockCategory InCategory, FText& OutMessage) const;

protected:
	EGsUnlockContentsType ConvertToContentsType(ItemCategorySub InItemCategorySub);


protected:
	void OnActiveSkill();
	void OnActiveQuest();
	void OnActiveFairy();
	void OnActiveNpcShop();
	void OnActiveGuild();
	void OnActiveCostume();
	void OnActiveMail();
	void OnActiveItemCollection();
	void OnActiveDungeon();
	void OnActiveNpcDepot();
	void OnActiveMarket();
	void OnActiveCraft();
	void OnActiveMultiLevelRank();
	void OnActiveOfflinePlay();
	void OnActiveBMShop();
	void OnActiveCommunity();
	void OnActiveRanking();
	void OnActiveAchievement();
	void OnActiveMonsterCollection();
	void OnActiveTitle();
	void OnActiveGameStatistics();
	void OnActiveUserProfile();
	void OnActiveBoss();
};

#define GSUnlock() UGsScopeHolder::GetGameManagerFType<FGsUnlockManager>(UGsScopeGame::EManagerType::Unlock)