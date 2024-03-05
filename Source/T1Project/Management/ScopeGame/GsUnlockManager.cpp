#include "GsUnlockManager.h"

#include "DataSchema/Unlock/GsSchemaUnlockContents.h"
#include "DataSchema/Unlock/GsSchemaUnlockCategory.h"
#include "DataSchema/Quest/Main/GsSchemaQuestMainStory.h"
#include "DataSchema/Quest/Main/GsSchemaQuestMainChapter.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Tray/GsUITrayTickerUnlock.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGlobal/GsNetManager.h"

#include "Message/MessageParam/GsMessageParam.h"

#include "Net/GsNetSendService.h"

#include "Reward/GsRewardHelper.h"

#include "Quest/Table/GsQuestMainTableCacheSet.h"

#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Shared/Shared/SharedPackets/PD_Gateway_Client.h"



void FGsUnlockManager::Initialize()
{
	const UGsTable* Table = FGsSchemaUnlockContents::GetStaticTable();
	if (nullptr != Table)
	{
		TArray<const FGsSchemaUnlockContents*> Rows;
		if (Table->GetAllRows<FGsSchemaUnlockContents>(Rows))
		{
			for (const FGsSchemaUnlockContents* Row : Rows)
			{
				if (nullptr == Row)
					continue;

				if (_contentsIdListMap.Contains(Row->contentsType))
				{
					_contentsIdListMap[Row->contentsType].Emplace(Row->id);
				}
				else
				{
					_contentsIdListMap.Emplace(Row->contentsType, TArray<UnlockContentsId>{ static_cast<UnlockContentsId>(Row->id) });
				}

				_contentsControlDataMap.Emplace(Row->id, Row);

				if (UnlockCondition::NONE != Row->lockType)
				{
					_lockedContents.Emplace(Row->id);
				}

				if (Row->serialNumber > _maxSerialNumber)
				{
					_maxSerialNumber = Row->serialNumber;
				}
			}
		}
	}

	RegisterMessages();
}

void FGsUnlockManager::Finalize()
{
	_contentsIdListMap.Empty();
	_lockedContents.Empty();
	_lockedContentsCategory.Empty();
	_lockedContentsServer.Empty();
	_contentsControlDataMap.Empty();
	_isNewContents.Empty();
	_reservedTicker.Empty();

	UnregisterMessages();
}

bool FGsUnlockManager::IsContentsUnlock(UnlockContentsId InContentsId)
{
	// 데이터가 없는 컨텐츠는 잠길 수 없다.
	if (false == _contentsControlDataMap.Contains(InContentsId))
	{
		return true;
	}

	// 서버에 의한 잠금 확인 (버그)
	if (_lockedContentsServer.Contains(InContentsId))
	{
		return false;
	}
	
	// 연계 컨텐츠 잠금 확인 (침공, 관전 등)
	for (const auto& lockData : _lockedContentsCategory)
	{
		for (UnlockContentsId lockedContentsId : lockData.Value)
		{
			if (InContentsId == lockedContentsId)
			{
				return false;
			}
		}
	}

	// 치트 : 전부 잠금 해제
	if (true == _allUnlockCheat)
	{
		return true;
	}

	// DTGsUnlockContents : 조건에 의한 잠금
	if (_lockedContents.Contains(InContentsId))
	{
		if (UnlockCondition::NONE != _contentsControlDataMap[InContentsId]->lockType)
			return false;
	}

	return true;
}

bool FGsUnlockManager::IsContentsUnlock(UnlockContentsId InContentsId, bool InShowTicker)
{
	if (IsContentsUnlock(InContentsId))
	{
		return true;
	}

	if (InShowTicker)
	{
		ShowLockTicker(InContentsId, _contentsControlDataMap[InContentsId]->lockType);
	}

	return false;
}

bool FGsUnlockManager::IsContentsUnlock(EGsUnlockContentsType InContentsType)
{
	if (EGsUnlockContentsType::NONE == InContentsType)
		return true;

	if (_contentsIdListMap.Contains(InContentsType))
	{
		for (UnlockContentsId contentsId : _contentsIdListMap[InContentsType])
		{
			if (IsContentsUnlock(contentsId))
			{
				return true;
			}
		}
		return false;
	}

	return true;
}

bool FGsUnlockManager::IsContentsUnlock(EGsUnlockContentsType InContentsType, bool InShowTicker)
{
	if (IsContentsUnlock(InContentsType))
	{
		return true;
	}

	if (InShowTicker)
	{
		ShowLockTicker(InContentsType);
	}

	return false;
}

bool FGsUnlockManager::IsContentsUnlock(ItemCategoryMain InItemCategoryMain, bool InShowTicker)
{
	/*switch (InItemCategoryMain)
	{

	}*/

	// 관련 설정이 없다면 언락과 무관한 분류이다.
	return true;
}

bool FGsUnlockManager::IsContentsUnlock(ItemCategorySub InItemCategorySub, bool InShowTicker)
{
	EGsUnlockContentsType contentsType = ConvertToContentsType(InItemCategorySub);
	return IsContentsUnlock(contentsType, InShowTicker);
}

bool FGsUnlockManager::ShowLockTicker(UnlockContentsId InContentsId, UnlockCondition InLockType)
{
	FText tickerMessage;
	if (MakeStringUnLockCondition(InContentsId, InLockType, tickerMessage))
	{
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, tickerMessage);
		return true;
	}
	return false;
}

bool FGsUnlockManager::ShowLockTicker(UnlockContentsId InContentsId, UnlockCategory InCondition)
{
	FText tickerMessage;
	if (MakeStringUnLockCategory(InContentsId, InCondition, tickerMessage))
	{
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, tickerMessage);
		return true;
	}
	return false;
}

bool FGsUnlockManager::ShowLockTicker(UnlockContentsId InContentsId)
{
	UnlockType unlockType = GetUnlockType(InContentsId);
	switch (unlockType)
	{
	case UnlockType::CONTENTS_CONDITION:
	{
		UnlockCondition lockedCondition = GetLockCondition(InContentsId);
		return ShowLockTicker(InContentsId, lockedCondition);
	}
	case UnlockType::CATEGORY:
	{
		UnlockCategory lockedCategory = GetLockCategory(InContentsId);
		return ShowLockTicker(InContentsId, lockedCategory);
	}
	case UnlockType::MAX:
		break;
	default:
		break;
	}
	return false;
}

bool FGsUnlockManager::ShowLockTicker(EGsUnlockContentsType InContentsType)
{
	if (EGsUnlockContentsType::NONE == InContentsType)
		return false;

	if (_contentsIdListMap.Contains(InContentsType))
	{
		for (UnlockContentsId contentsId : _contentsIdListMap[InContentsType])
		{
			if (ShowLockTicker(contentsId))
			{
				return true;
			}
		}
	}
	return false;
}

bool FGsUnlockManager::ShowLockTicker(ItemCategorySub InItemCategorySub)
{
	return ShowLockTicker(ConvertToContentsType(InItemCategorySub));
}

void FGsUnlockManager::ShowUnlockTicker(UnlockContentsId InContentsId)
{
	const FGsSchemaUnlockContents* unlockData = GetUnlockData(InContentsId);
	if (nullptr == unlockData || false == unlockData->showUnlockTicker)
		return;

	// 잠겨 있는 상태
	if (_lockedContents.Contains(InContentsId))
		return;

	// 언락 티커는 하나의 티커에 연결되어 출력된다.
	if (GUI()->IsExistInQueue(TEXT("TrayTickerUnlock"), 0))
	{
		_reservedTicker.Emplace(InContentsId);
	}
	else
	{
		const FText& name = unlockData->contentsName;
		const FText& desc = unlockData->unlockDesc;
		const FSoftObjectPath& iconPath = unlockData->icon;

		GUI()->PushQueue(TEXT("TrayTickerUnlock"), 0, [name, desc, iconPath](UGsUIWidgetBase* InWidget)
			{
				if (InWidget)
				{
					if (UGsUITrayTickerUnlock* trayTicker = Cast<UGsUITrayTickerUnlock>(InWidget))
					{
						if (desc.IsEmpty())
						{
							trayTicker->SetText(name);
						}
						else
						{
							trayTicker->SetText(name, desc);
						}

						if (iconPath.IsValid())
						{
							trayTicker->SetImage(iconPath);
						}
					}
				}
			});
	}
}

void FGsUnlockManager::ShowUnlockTicker(EGsUnlockContentsType InContentsType)
{
	if (_contentsIdListMap.Contains(InContentsType))
	{
		for (uint8 i = _contentsIdListMap[InContentsType].Num() - 1;i >= 0;--i)
		{
			ShowUnlockTicker(_contentsIdListMap[InContentsType][i]);
		}
	}
}

void FGsUnlockManager::UpdateLockedContentsByLevel(bool InIsShowUnlockTicker)
{
	if (false == _spawnComplete)
		return;

	if (0 >= _lockedContents.Num())
		return;

	const FGsNetUserData* userData = GGameData()->GetUserData();
	if (nullptr == userData)
		return;

	Level pcLevel = userData->mLevel;
	
	TArray<UnlockContentsId> idList = _lockedContents.Array();
	for (UnlockContentsId contentsId : idList)
	{
		const FGsSchemaUnlockContents* unlockData = GetUnlockData(contentsId);
		if (nullptr == unlockData)
			continue;

		if (UnlockCondition::LEVEL != unlockData->lockType)
			continue;

		if (pcLevel >= unlockData->requireLevel)
		{
			// 로그인 후 신규 컨텐츠의 언락 || 오프라인 플레이 후 레벨 업에 의한 언락
			if (_lastSerialNumber < unlockData->serialNumber || _maxLevel < unlockData->requireLevel)
			{
				UnlockContents(contentsId, unlockData->showUnlockTicker, true);
			}
			else
			{
				// InIsShowUnlockTicker	(false) : 로그인 시 기존에 풀려있던 컨텐츠. 
				//						(true) : 조건을 만족하여 새로 풀린 컨텐츠.
				UnlockContents(contentsId, InIsShowUnlockTicker && unlockData->showUnlockTicker, InIsShowUnlockTicker);
			}
		}
		// 레벨 감소로 낮아졌지만 최고 레벨은 요구 레벨 달성
		else
		{
			if (_maxLevel >= unlockData->requireLevel)
			{
				UnlockContents(contentsId, false, false);
			}
		}
	}
}

bool FGsUnlockManager::GetUnLockMessage(UnlockContentsId InContentsId, FText& OutMessage) const
{
	UnlockType unlockType = GetUnlockType(InContentsId);
	switch (unlockType)
	{
	case UnlockType::CONTENTS_CONDITION:
	{
		UnlockCondition lockedCondition = GetLockCondition(InContentsId);
		return MakeStringUnLockCondition(InContentsId, lockedCondition, OutMessage);
	}
	case UnlockType::CATEGORY:
	{
		UnlockCategory lockedCategory = GetLockCategory(InContentsId);
		return MakeStringUnLockCategory(InContentsId, lockedCategory, OutMessage);
	}
	case UnlockType::MAX:
		break;
	default:
		break;
	}
	return false;
}

bool FGsUnlockManager::GetUnLockMessage(EGsUnlockContentsType InUnlockContentsType, FText& OutMessage) const
{
	UnlockContentsId unlockContentsId
		= _contentsIdListMap.Find(InUnlockContentsType) ? _contentsIdListMap[InUnlockContentsType][0] : INVALID_UNLOCK_CONTENTS_ID;

	return GetUnLockMessage(unlockContentsId, OutMessage);
}

void FGsUnlockManager::UpdateLockedContentsByQuest(bool InIsShowUnlockTicker)
{
	if (false == _spawnComplete)
		return;

	if (0 >= _lockedContents.Num())
		return;

	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	TArray<UnlockContentsId> idList = _lockedContents.Array();
	for (UnlockContentsId contentsId : idList)
	{
		const FGsSchemaUnlockContents* unlockData = GetUnlockData(contentsId);
		if (nullptr == unlockData)
			continue;

		if (UnlockCondition::QUEST != unlockData->lockType)
			continue;

		const FGsSchemaQuestMainStory* storyData = unlockData->requireMainStoryId.GetRow();
		if (nullptr == storyData)
			continue;

		// InIsShowUnlockTicker: false && IsStoryRewarded: true --> 인게임 진입 시 퀘스트 상태 부가 확인
		if (questManager->IsStoryCompleted(storyData->id) || 
			(false == InIsShowUnlockTicker && questManager->IsStoryRewarded(QuestType::MAIN, storyData->id)))
		{
			// 로그인 후 신규 컨텐츠의 언락
			if (_lastSerialNumber < unlockData->serialNumber)
			{
				UnlockContents(contentsId, unlockData->showUnlockTicker, true);
			}
			else
			{
				// InIsShowUnlockTicker	(false) : 로그인 시 기존에 풀려있던 컨텐츠. 
				//						(true) : 조건을 만족하여 새로 풀린 컨텐츠.
				UnlockContents(contentsId, InIsShowUnlockTicker && unlockData->showUnlockTicker, InIsShowUnlockTicker);
			}
		}
	}
}

void FGsUnlockManager::UnlockContents(UnlockContentsId InContentsId, bool InIsShowUnlockTicker /*= false*/, bool InIsNew /*= false*/)
{
	if (false == _lockedContents.Contains(InContentsId))
		return;

	_lockedContents.Remove(InContentsId);

	if (InIsShowUnlockTicker && false == _allUnlockCheat)
	{
		ShowUnlockTicker(InContentsId);
	}

	if (InIsNew)
	{
		_isNewContents.Emplace(InContentsId);
	}

	FGsPrimitivePairInt32 param(StaticCast<int32>(_contentsControlDataMap[InContentsId]->contentsType), InContentsId);
	GMessage()->GetUnlock().SendMessage(MessageContentUnlock::NOTIFY_CONTENTS_UNLOCK, &param);
}

UnlockType FGsUnlockManager::GetUnlockType(UnlockContentsId InContentsId) const
{
	// 운영툴 잠금을 먼저 확인한다.
	if (_lockedContentsServer.Contains(InContentsId))
	{
		return UnlockType::CONTENTS_CONDITION;
	}

	// 연계 컨텐츠 잠금을 확인
	for (const auto& lockData : _lockedContentsCategory)
	{
		for (UnlockContentsId lockedContentsId : lockData.Value)
		{
			if (InContentsId == lockedContentsId)
			{
				return UnlockType::CATEGORY;
			}
		}
	}

	// 잠겨 있지 않으면 종료한다.
	if (_lockedContents.Contains(InContentsId))
	{
		return UnlockType::CONTENTS_CONDITION;
	}
	
	return UnlockType::MAX;
}

UnlockCondition FGsUnlockManager::GetLockCondition(UnlockContentsId InContentsId) const
{
	// 운영툴 잠금을 먼저 확인한다.
	if (_lockedContentsServer.Contains(InContentsId))
	{
		return UnlockCondition::BUG_ERROR;
	}

	// 잠겨 있지 않으면 종료한다.
	if (_lockedContents.Contains(InContentsId))
	{
		return _contentsControlDataMap[InContentsId]->lockType;
	}

	return UnlockCondition::NONE;
}

UnlockCategory FGsUnlockManager::GetLockCategory(UnlockContentsId InContentsId) const
{
	// 연계 컨텐츠 잠금을 확인
	for (const auto& lockData : _lockedContentsCategory)
	{
		for (UnlockContentsId lockedContentsId : lockData.Value)
		{
			if (InContentsId == lockedContentsId)
			{
				return lockData.Key;
			}
		}
	}

	return UnlockCategory::NONE;
}

bool FGsUnlockManager::GetContentsName(EGsUnlockContentsType InUnlockContentsType, OUT FText& OutText) const
{
	const FGsSchemaUnlockContents* UnlockContents = GetUnlockData(InUnlockContentsType);
	if (nullptr == UnlockContents)
	{
		return false;
	}

	OutText = UnlockContents->contentsName;
	return true;
}

const FGsSchemaUnlockContents* FGsUnlockManager::GetUnlockData(UnlockContentsId InContentsId) const
{
	if (_contentsControlDataMap.Contains(InContentsId))
	{
		return _contentsControlDataMap[InContentsId];
	}
	return nullptr;
}

const FGsSchemaUnlockContents* FGsUnlockManager::GetUnlockData(EGsUnlockContentsType InContentsType) const
{
	if (_contentsIdListMap.Contains(InContentsType))
	{
		return GetUnlockData(_contentsIdListMap[InContentsType][0]);
	}
	return nullptr;
}

void FGsUnlockManager::Set(PD::GC::PKT_GC_UNLOCK_CONTENTS_LIST_READ& InPacket)
{
	TArray<UnlockContentsId> unlockContentsInfoList;
	InPacket.MakeUnlockContentsIdListVector<TArray<UnlockContentsId>, UnlockContentsId>(unlockContentsInfoList);
	for (const auto& contentsId : unlockContentsInfoList)
	{
		LockContentsByServer(contentsId);
	}
}

void FGsUnlockManager::Set(PD::GC::PKT_GC_NOTICE_UNLOCK_CONTENTS_UPDATE_READ& InPacket)
{
	TArray<UnlockContentsId> addUnlockContentsInfoList;
	InPacket.MakeAddUnlockContentsIdListVector<TArray<UnlockContentsId>, UnlockContentsId>(addUnlockContentsInfoList);
	for (const auto& contentsId : addUnlockContentsInfoList)
	{
		LockContentsByServer(contentsId);
	}

	TArray<UnlockContentsId> deleteUnlockContentsIdList;
	InPacket.MakeDeleteUnlockContentsIdListVector<TArray<UnlockContentsId>, UnlockContentsId>(deleteUnlockContentsIdList);
	for (const auto& contentsId : deleteUnlockContentsIdList)
	{
		UnlockContentsByServer(contentsId);
	}
}

void FGsUnlockManager::LockContentsByServer(UnlockContentsId InContentsId)
{
	if (false == _lockedContentsServer.Contains(InContentsId))
	{
		_lockedContentsServer.Emplace(InContentsId);
	}
	GMessage()->GetUnlock().SendMessage(MessageContentUnlock::INVALIDATE_ALL_UNLOCK, nullptr);
}

void FGsUnlockManager::UnlockContentsByServer(UnlockContentsId InContentsId)
{
	if (_lockedContentsServer.Contains(InContentsId))
	{
		_lockedContentsServer.Remove(InContentsId);
	}
	GMessage()->GetUnlock().SendMessage(MessageContentUnlock::INVALIDATE_ALL_UNLOCK, nullptr);
}

void FGsUnlockManager::RegisterMessages()
{
	FGsMessageHolder* msg = GMessage();
	if (nullptr == msg)
	{
		GSLOG(Error, TEXT("nullptr == MessageManager"));
		return;
	}
	
	_questDelegate.Emplace(msg->GetQuest().AddRaw(MessageContentQuest::QUEST_MAIN_REWARDED,
		this, &FGsUnlockManager::OnMainQuestRewarded));
	_questDelegate.Emplace(msg->GetQuest().AddRaw(MessageContentQuest::QUEST_MAIN_ACCEPTED,
		this, &FGsUnlockManager::OnMainQuestAccepted));

	_userInfoDelegate = msg->GetUserBasicInfo().AddRaw(MessageUserInfo::LEVEL,
		this, &FGsUnlockManager::OnChangeLocalPlayerLevel);

	_gameobjectDelegate = msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_SPAWN_ME_COMPLETE,
		this, &FGsUnlockManager::OnLocalPlayerSpawnComplete);

	auto& contentsHandler = msg->GetContents();
	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_QUEST,
		this, &FGsUnlockManager::OnActiveQuest));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_SKILL,
		this, &FGsUnlockManager::OnActiveSkill));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_FAIRY,
		this, &FGsUnlockManager::OnActiveFairy));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_NPC_SHOP,
		this, &FGsUnlockManager::OnActiveNpcShop));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_GUILD,
		this, &FGsUnlockManager::OnActiveGuild));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_COSTUME,
		this, &FGsUnlockManager::OnActiveCostume));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_MAIL,
		this, &FGsUnlockManager::OnActiveMail));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_ITEM_COLLECTION,
		this, &FGsUnlockManager::OnActiveItemCollection));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_DUNGEON,
		this, &FGsUnlockManager::OnActiveDungeon));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_NPC_DEPOT,
		this, &FGsUnlockManager::OnActiveNpcDepot));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_MARKET,
		this, &FGsUnlockManager::OnActiveMarket));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_CRAFT,
		this, &FGsUnlockManager::OnActiveCraft));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_MULTI_LEVEL_RANK,
		this, &FGsUnlockManager::OnActiveMultiLevelRank));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_OFFLINEPLAY,
		this, &FGsUnlockManager::OnActiveOfflinePlay));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_BMSHOP,
		this, &FGsUnlockManager::OnActiveBMShop));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_COMMUNITY,
		this, &FGsUnlockManager::OnActiveCommunity));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_RANKING,
		this, &FGsUnlockManager::OnActiveRanking));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_ACHIEVEMENT,
		this, &FGsUnlockManager::OnActiveAchievement));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_MONSTER_COLLECTION,
		this, &FGsUnlockManager::OnActiveMonsterCollection));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_TITLE,
		this, &FGsUnlockManager::OnActiveTitle));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_GAME_STATISTICS,
		this, &FGsUnlockManager::OnActiveGameStatistics));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_USER_PROFILE,
		this, &FGsUnlockManager::OnActiveUserProfile));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_BOSS,
		this, &FGsUnlockManager::OnActiveBoss));
}

void FGsUnlockManager::UnregisterMessages()
{
	FGsMessageHolder* msg = GMessage();
	if (nullptr == msg)
	{
		GSLOG(Error, TEXT("nullptr == MessageManager"));
		return;
	}

	for (const TPair<MessageContentQuest, FDelegateHandle>& questDelegate : _questDelegate)
	{
		msg->GetQuest().Remove(questDelegate);
	}
	_questDelegate.Empty();
	
	msg->GetUserBasicInfo().Remove(_userInfoDelegate);
	msg->GetGameObject().Remove(_gameobjectDelegate);

	auto& contentsHandler = msg->GetContents();
	for (TPair<MessageContents, FDelegateHandle>& elemStage : _listContentsDelegate)
	{
		contentsHandler.Remove(elemStage);
	}
	_listContentsDelegate.Empty();
}

void FGsUnlockManager::OnMainQuestAccepted(const IGsMessageParam*)
{
	UpdateLockedContentsByQuest(true);
}

void FGsUnlockManager::OnMainQuestRewarded(const IGsMessageParam*)
{
	UpdateLockedContentsByQuest(true);
}

void FGsUnlockManager::OnChangeLocalPlayerLevel(uint64 InLevel)
{
	if (_maxLevel < InLevel)
	{
		_maxLevel = InLevel;
	}
	
	FGsNetSendService::SendReqUpdateUnlockInfo(_maxSerialNumber, _maxLevel);
}

void FGsUnlockManager::OnReceiveUnlockInfo(PD::GC::PKT_GC_UNLOCKINFO_READ& InPacket)
{
	_maxLevel = InPacket.Level();
	_lastSerialNumber = InPacket.SerialNumber();
}

void FGsUnlockManager::OnLocalPlayerSpawnComplete(const IGsMessageParam* InParam)
{
	_spawnComplete = true;
	UpdateLockedContentsByLevel(false);
	UpdateLockedContentsByQuest(false);

	const FGsNetUserData* userData = GGameData()->GetUserData();
	if (nullptr == userData)
		return;
	Level pcLevel = userData->mLevel;

	if (_maxLevel < pcLevel)
	{
		_maxLevel = pcLevel;
	}
	
	_lastSerialNumber = _maxSerialNumber;

	if (FGsNetManager* netManager = GNet())
	{
		if (false == netManager->GetIsRegisterDevice())
		{
			LockContentsByClient(UnlockCategory::REGISTER_DEVICE);
		}
	}

	GMessage()->GetUnlock().SendMessage(MessageContentUnlock::INVALIDATE_ALL_UNLOCK, nullptr);

	FGsNetSendService::SendReqUpdateUnlockInfo(_maxSerialNumber, _maxLevel);
}

bool FGsUnlockManager::IsNewContents(UnlockContentsId InContentsId)
{
	return _isNewContents.Contains(InContentsId);
}

bool FGsUnlockManager::IsNewContents(EGsUnlockContentsType InContentsType)
{
	if (_contentsIdListMap.Contains(InContentsType))
	{
		for (UnlockContentsId contentsId : _contentsIdListMap[InContentsType])
		{
			if (IsNewContents(contentsId))
			{
				return true;
			}
		}
	}

	return false;
}

void FGsUnlockManager::SetIsNewContents(UnlockContentsId InContentsId, bool InIsNew)
{
	if (InIsNew)
	{
		_isNewContents.Emplace(InContentsId);
	}
	else
	{
		_isNewContents.Remove(InContentsId);
	}
	GMessage()->GetUnlock().SendMessage(MessageContentUnlock::INVALIDATE_ALL_UNLOCK, nullptr);
}

void FGsUnlockManager::SetIsNewContents(EGsUnlockContentsType InContentsType, bool InIsNew)
{
	if (_contentsIdListMap.Contains(InContentsType))
	{
		for (UnlockContentsId contentsId : _contentsIdListMap[InContentsType])
		{
			SetIsNewContents(contentsId, InIsNew);
		}
	}
}

void FGsUnlockManager::FindQuestConditionText(UnlockContentsId InContentsId, OUT FText& OutText) const
{
	const FGsSchemaUnlockContents* unlockData = GetUnlockData(InContentsId);
	if (nullptr == unlockData)
		return;

	const FGsSchemaQuestMainStory* mainStory = unlockData->requireMainStoryId.GetRow();
	int32 mainStoryId = mainStory->id;

	if (const FGsQuestMainTableCacheSet* questMainData = GSQuest()->GetQuestMainTableCache())
	{
		const TArray<TSharedPtr<FGsQuestChapterTableCache>>& chapterDataList = questMainData->GetQuestChapterTableCacheList();
		int32 chapterNum = chapterDataList.Num();
		for (int32 chapterIndex = 0; chapterIndex < chapterNum; ++chapterIndex)
		{
			TSharedPtr<FGsQuestChapterTableCache> chapterTableCache = chapterDataList[chapterIndex];
			if (nullptr == chapterTableCache)
				continue;

			const FGsSchemaQuestMainChapter* schemaQuestChapter = chapterTableCache->GetSchemaQuestChapter();
			if (nullptr == schemaQuestChapter)
				continue;

			int32 storyNum = schemaQuestChapter->questStoryList.Num();
			for (int32 storyIndex = 0; storyIndex < storyNum; ++storyIndex)
			{
				const FGsSchemaQuestMainStory* schemaQuestMainStory = schemaQuestChapter->questStoryList[storyIndex].GetRow();
				if (nullptr == schemaQuestMainStory)
					continue;

				int32 findStoryId = schemaQuestMainStory->id;
				if (mainStoryId == findStoryId)
				{
					// [{0}챕터 {1}. {0}-{2} {3}] 퀘스트를 완료해야 활성화됩니다.
					OutText = FText::Format(unlockData->conditionDesc, 
						(chapterIndex + 1), schemaQuestChapter->chapterTitleText, storyIndex + 1, mainStory->storyTitleText);
					return;
				}
			}
		}
	}
}

bool FGsUnlockManager::MakeStringUnLockCondition(UnlockContentsId InContentsId, UnlockCondition InCondition, FText& OutMessage) const
{
	const FGsSchemaUnlockContents* unlockData = GetUnlockData(InContentsId);
	if (nullptr == unlockData)
	{
		switch (InCondition)
		{
		case UnlockCondition::BUG_ERROR:
		{
			FText::FindText(TEXT("UnlockText"), TEXT("Notice_Buglock"), OutMessage);
			break;
		}
		case UnlockCondition::UPDATE:
		{
			FText::FindText(TEXT("UnlockText"), TEXT("Notice_updateDesc"), OutMessage);
			break;
		}
		}
	}
	else
	{
		OutMessage = FText::GetEmpty();
		switch (InCondition)
		{
		case UnlockCondition::LEVEL:
			OutMessage = FText::Format(unlockData->conditionDesc, FText::AsNumber(unlockData->requireLevel));
			break;
		case UnlockCondition::QUEST:
		{
			OutMessage = unlockData->conditionDesc;
			// [{0}챕터 {1}. {0}-{2} {3}] 퀘스트를 완료해야 활성화됩니다.
			// 예) [2챕터 차원문 넘어. 2-1 물에 잠긴 사원] 퀘스트를 완료해야 활성화됩니다.
			FindQuestConditionText(InContentsId, OutMessage);
			break;
		}
		case UnlockCondition::BUG_ERROR:
			OutMessage = unlockData->bugDesc;
			break;
		case UnlockCondition::UPDATE:
			OutMessage = unlockData->updateDesc;
			break;
		}
	}

	if (OutMessage.IsEmpty())
	{
		GSLOG(Error, TEXT("OutMessage.IsEmpty() ContentsId [%d], InCondition [%d]"), InContentsId, InCondition);
	}

	return (false == OutMessage.IsEmpty());
}

bool FGsUnlockManager::MakeStringUnLockCategory(UnlockContentsId InContentsId, UnlockCategory InCategory, FText& OutMessage) const
{
	const FGsSchemaUnlockContents* unlockData = GetUnlockData(InContentsId);
	if (nullptr == unlockData)
	{
		return false;
	}

	OutMessage = FText::GetEmpty();

	const UGsTableUnlockCategory* Table = Cast<UGsTableUnlockCategory>(FGsSchemaUnlockCategory::GetStaticTable());
	if (nullptr != Table)
	{
		const FGsSchemaUnlockCategory* row = nullptr;
		if (Table->FindRowById(InCategory, row))
		{
			OutMessage = row->lockDesc;
		}
	}

	if (OutMessage.IsEmpty())
	{
		GSLOG(Error, TEXT("OutMessage.IsEmpty() ContentsId [%d], InCategory [%d]"), InContentsId, InCategory);
	}

	return (false == OutMessage.IsEmpty());
}

void FGsUnlockManager::UnlockAllContentsCheat()
{
	_allUnlockCheat = true;
	GMessage()->GetUnlock().SendMessage(MessageContentUnlock::INVALIDATE_ALL_UNLOCK, nullptr);
}

void FGsUnlockManager::UnlockResetCheat()
{
	_allUnlockCheat = false;
	GMessage()->GetUnlock().SendMessage(MessageContentUnlock::INVALIDATE_ALL_UNLOCK, nullptr);
}

void FGsUnlockManager::LockContentsByClient(UnlockCategory InCategory)
{
	if (_lockedContentsCategory.Contains(InCategory))
		return;

	const UGsTableUnlockCategory* Table = Cast<UGsTableUnlockCategory>(FGsSchemaUnlockCategory::GetStaticTable());
	if (nullptr != Table)
	{
		const FGsSchemaUnlockCategory* row = nullptr;
		if (Table->FindRowById(InCategory, row))
		{
			_lockedContentsCategory.Emplace(InCategory, TArray<UnlockContentsId>());

			const TArray<FGsSchemaUnlockContentsRow>& contentsList =  row->subContentsIdList;
			for (const FGsSchemaUnlockContentsRow& content : contentsList)
			{
				if (const FGsSchemaUnlockContents* contentData = content.GetRow())
				{
					_lockedContentsCategory[InCategory].Emplace(contentData->id);
				}
			}
			GMessage()->GetUnlock().SendMessage(MessageContentUnlock::INVALIDATE_ALL_UNLOCK, nullptr);
		}
	}
}

void FGsUnlockManager::UnlockContentsByClient(UnlockCategory InCategory)
{
	if (_lockedContentsCategory.Contains(InCategory))
	{
		_lockedContentsCategory.Remove(InCategory);
		GMessage()->GetUnlock().SendMessage(MessageContentUnlock::INVALIDATE_ALL_UNLOCK, nullptr);
	}
}

EGsUnlockContentsType FGsUnlockManager::ConvertToContentsType(ItemCategorySub InItemCategorySub)
{
	for (const auto& unlockData : _contentsControlDataMap)
	{
		if (nullptr == unlockData.Value)
			continue;

		for (ItemCategorySub itemCategorySub : unlockData.Value->itemCategorySubList)
		{
			if (itemCategorySub == InItemCategorySub)
			{
				return unlockData.Value->contentsType;
			}
		}
	}

	return EGsUnlockContentsType::NONE;
}

void FGsUnlockManager::OnActiveSkill()
{
	SetIsNewContents(EGsUnlockContentsType::SKILL, false);
}

void FGsUnlockManager::OnActiveQuest()
{
	SetIsNewContents(EGsUnlockContentsType::QUEST, false);
}

void FGsUnlockManager::OnActiveFairy()
{
	SetIsNewContents(EGsUnlockContentsType::FAIRY, false);
}

void FGsUnlockManager::OnActiveNpcShop()
{
	SetIsNewContents(EGsUnlockContentsType::NPC_SHOP, false);
}

void FGsUnlockManager::OnActiveGuild()
{
	SetIsNewContents(EGsUnlockContentsType::GUILD, false);
}

void FGsUnlockManager::OnActiveCostume()
{
	SetIsNewContents(EGsUnlockContentsType::COSTUME, false);
}

void FGsUnlockManager::OnActiveMail()
{
	SetIsNewContents(EGsUnlockContentsType::MAIL, false);
}

void FGsUnlockManager::OnActiveItemCollection()
{
	SetIsNewContents(EGsUnlockContentsType::ITEM_COLLECTION, false);
}

void FGsUnlockManager::OnActiveDungeon()
{
	SetIsNewContents(EGsUnlockContentsType::DUNGEON, false);
}

void FGsUnlockManager::OnActiveNpcDepot()
{
	SetIsNewContents(EGsUnlockContentsType::DEPOT, false);
}

void FGsUnlockManager::OnActiveMarket()
{
	SetIsNewContents(EGsUnlockContentsType::MARKET, false);
}

void FGsUnlockManager::OnActiveCraft()
{
	SetIsNewContents(EGsUnlockContentsType::CRAFT, false);
}

void FGsUnlockManager::OnActiveMultiLevelRank()
{
	SetIsNewContents(EGsUnlockContentsType::MULTI_LEVEL_RANK, false);
}

void FGsUnlockManager::OnActiveOfflinePlay()
{
	SetIsNewContents(EGsUnlockContentsType::OFFLINE_PLAY, false);
}

void FGsUnlockManager::OnActiveBMShop()
{
	SetIsNewContents(EGsUnlockContentsType::BMSHOP, false);
}

void FGsUnlockManager::OnActiveCommunity()
{
	SetIsNewContents(EGsUnlockContentsType::COMMUNITY, false);
}

void FGsUnlockManager::OnActiveRanking()
{
	SetIsNewContents(EGsUnlockContentsType::RANKING, false);
}

void FGsUnlockManager::OnActiveAchievement()
{
	SetIsNewContents(EGsUnlockContentsType::ACHIEVEMENT, false);
}

void FGsUnlockManager::OnActiveMonsterCollection()
{
	SetIsNewContents(EGsUnlockContentsType::MONSTER_KNOWLEDGE_COLLECTION, false);
}

void FGsUnlockManager::OnActiveTitle()
{
	SetIsNewContents(EGsUnlockContentsType::TITLE, false);
}

void FGsUnlockManager::OnActiveGameStatistics()
{
	SetIsNewContents(EGsUnlockContentsType::GAME_STATISTICS, false);
}

void FGsUnlockManager::OnActiveUserProfile()
{
	SetIsNewContents(EGsUnlockContentsType::USER_PROFILE, false);
}

void FGsUnlockManager::OnActiveBoss()
{
	SetIsNewContents(EGsUnlockContentsType::BOSS, false);
}

const FGsSchemaUnlockContents* FGsUnlockManager::PopReservedTickerData()
{
	if (_reservedTicker.Num() <= 0)
		return nullptr;

	const FGsSchemaUnlockContents* unlockData = GetUnlockData(_reservedTicker[0]);
	_reservedTicker.RemoveAt(0);
	return unlockData;
}
