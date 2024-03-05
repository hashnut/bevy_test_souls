// Fill out your copyright notice in the Description page of Project Settings.


#include "GsServerOption.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Net/GsNetSendServiceWorld.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"

#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/GsContentsMode.h"
#include "GameFlow/GsGameFlowGame.h"

#include "Message/MessageParam/GsMessageParam.h"
#include "Message/GsMessageGameObject.h"
#include "GsAlertHelper.h"
#include "Message/MessageParam/GsInvenMessageParam.h"
#include "Message/MessageParam/GsGameObjectMessageParam.h"
#include "Message/MessageParam/GsItemMessageParam.h"
#include "Management/ScopeGame/GsAIManager.h"
#include "../GameObject/ObjectClass/GsGameObjectBase.h"
#include "../GameObject/Stat/GsStatInfo.h"

#include "Item/GsItemManager.h"

#include "GsOptionFunc.h"
#include "Data/GsDataContainManager.h"
#include "GsGameUserSettings.h"
#include "Quest/GsQuestData.h"

void FGsServerOption::Initialize()
{
	// 이벤트 등록
	FGsMessageHolder* message = GMessage();
	MGameObject& objectMessage = message->GetGameObject();
	_gameObjectMessageArray.Emplace(objectMessage.AddRaw(MessageGameObject::UPDATE_USER_DATA, this, &FGsServerOption::OnUserData));
	_gameObjectMessageArray.Emplace(objectMessage.AddRaw(MessageGameObject::LOCAL_HIT, this, &FGsServerOption::OnHitLocal));
	_gameObjectMessageArray.Emplace(objectMessage.AddRaw(MessageGameObject::LOCAL_DEAD, this, &FGsServerOption::OnLocalPlayerDead));
	
	MInventory& inventoryMessage = message->GetInventoryEvent();
	_inventoryMessageArray.Emplace(inventoryMessage.AddRaw(MessageContentInven::UPDATE_ITEM_WEIGHT, this, &FGsServerOption::UpdateInvenWeightCallback));

	MItem& itemMessage = message->GetItem();
	_listItemActionDelegates.Emplace(itemMessage.AddRaw(MessageItem::ADDITEMLIST, this, &FGsServerOption::AddItemListCallback));
	_listItemActionDelegates.Emplace(itemMessage.AddRaw(MessageItem::UPDATEITEM, this, &FGsServerOption::UpdateItemCallback));
	_listItemActionDelegates.Emplace(itemMessage.AddRaw(MessageItem::REMOVEITEM, this, &FGsServerOption::UpdateItemCallback));
	_listItemActionDelegates.Emplace(itemMessage.AddRaw(MessageItem::REMOVEITEMS, this, &FGsServerOption::UpdateItemCallback));

	
	_autoCombatTargetOptionList.Init(1, MAX_COMBAT_OPTION_COUNT);
	_manualCombatTargetOptionList.Init(1, MAX_COMBAT_OPTION_COUNT);

	_notificationAlertDelayInfos.Emplace(EGsOptionNotificationAlert::PVP_ALERT, FDateTime::Now());
	_notificationAlertDelayInfos.Emplace(EGsOptionNotificationAlert::HP_RATE_ALERT, FDateTime::Now());
	_notificationAlertDelayInfos.Emplace(EGsOptionNotificationAlert::WEIGHT_ALERT, FDateTime::Now());
	_notificationAlertDelayInfos.Emplace(EGsOptionNotificationAlert::DEATH_ALERT, FDateTime::Now());
	_notificationAlertDelayInfos.Emplace(EGsOptionNotificationAlert::HP_POTION_ALERT, FDateTime::Now());	
}

void FGsServerOption::Finalize()
{
	FGsMessageHolder* message = GMessage();
	MGameObject& objectMessage = message->GetGameObject();
	for (MsgGameObjHandle handle : _gameObjectMessageArray)
	{
		objectMessage.Remove(handle);
	}
	_gameObjectMessageArray.Empty();

	MInventory& inventoryMessage = message->GetInventoryEvent();
	for (MsgHandleInventory handle : _inventoryMessageArray)
	{
		inventoryMessage.Remove(handle);
	}
	_inventoryMessageArray.Empty();

	MItem& itemMessage = message->GetItem();
	for (MsgItem& handle : _listItemActionDelegates)
	{
		itemMessage.Remove(handle);
	}
	_listItemActionDelegates.Empty();


	_hideUISubQuestIdList.Empty();
	_hideUIRepeatQuestIdList.Empty();
	_guideQuestHudOnList.Empty();
	_guideQuestHudOffList.Empty();
	_autoCombatTargetOptionList.Empty();
	_manualCombatTargetOptionList.Empty();
	_repeatQuestviewIndexList.Empty();

	_notificationAlertDelayInfos.Empty();

	if (UWorld* world = GUI()->GetWorld())
	{
		world->GetTimerManager().ClearTimer(_pvpHUDEffectTimerHandle);
	}
}

void FGsServerOption::OnUserData(const IGsMessageParam* In_data)
{
	const FGsNetUserData* userData = In_data->Cast<const FGsNetUserData>();
	const ClientOptionData* OptionData = &userData->mOptionData;
	ConvertToClientData(OptionData);
}

void FGsServerOption::ConvertToServerData(ClientOptionData* InOptionData)
{
	int i;
	int num = _hideUISubQuestIdList.Num();
	for (i = 0; i < num; ++i)
	{
		if (i >= MAX_ACTIVE_QUEST_SUB_COUNT)
			continue;

		InOptionData->mSubQuestHudShowInfoList[i] = _hideUISubQuestIdList[i];
	}

	num = _hideUIRepeatQuestIdList.Num();
	for (i = 0; i < num; ++i)
	{
		if (i >= MAX_ACTIVE_QUEST_REPEAT_COUNT)
			continue;

		InOptionData->mRepeatQuestHudShowInfoList[i] = _hideUIRepeatQuestIdList[i];
	}

	num = _guideQuestHudOnList.Num();
	for (i = 0; i < num; ++i)
	{
		InOptionData->mGuideQuestHudShowInfoList[i] = _guideQuestHudOnList[i];
	}

	num = _guideQuestHudOffList.Num();
	for (i = 0; i < num; ++i)
	{
		InOptionData->mGuideQuestUserSelectHudHideInfoList[i] = _guideQuestHudOffList[i];
	}

	num = _repeatQuestviewIndexList.Num();
	for (i = 0; i < num; ++i)
	{
		InOptionData->mQuestBoardHudSortInfoArray[i] = _repeatQuestviewIndexList[i];
	}
}

void FGsServerOption::ConvertToClientData(const ClientOptionData* inOptionData)
{
	if (nullptr == inOptionData)
		return;

	_hideUISubQuestIdList.Empty();
	_hideUIRepeatQuestIdList.Empty();
	_guideQuestHudOnList.Empty();
	_guideQuestHudOffList.Empty();
	_repeatQuestviewIndexList.Empty();

	int i;
	for (i = 0; i < MAX_ACTIVE_QUEST_SUB_COUNT; ++i)
	{
		StoryId storyId = inOptionData->mSubQuestHudShowInfoList[i];
		if (0 < storyId)
			_hideUISubQuestIdList.Add(storyId);
	}

	for (i = 0; i < MAX_ACTIVE_QUEST_REPEAT_COUNT; ++i)
	{
		StoryId storyId = inOptionData->mRepeatQuestHudShowInfoList[i];
		if (0 < storyId)
			_hideUIRepeatQuestIdList.Add(storyId);
	}

	for (i = 0; i < MAX_QUEST_GUIDE_HUD_VIEW_COUNT; ++i)
	{
		StoryId storyId = inOptionData->mGuideQuestHudShowInfoList[i];
		if (0 < storyId)
			_guideQuestHudOnList.Add(storyId);
	}

	for (i = 0; i < MAX_QUEST_GUIDE_UI_VIEW_OFF_COUNT; ++i)
	{
		StoryId storyId = inOptionData->mGuideQuestUserSelectHudHideInfoList[i];
		if (0 < storyId)
			_guideQuestHudOffList.Add(storyId);
	}

	for (i = 0; i < MAX_ACTIVE_BOARD_QUEST_COUNT; ++i)
	{
		_repeatQuestviewIndexList.Add(inOptionData->mQuestBoardHudSortInfoArray[i]);
	}
}

void FGsServerOption::ConvertToServerData(GameOption* InOptionData)
{
	InOptionData->mAutoUsePotionInfo.mIsAutoUsePotion = _isAutoUsePotion;
	InOptionData->mAutoUsePotionInfo.mUsePotionHpRate = _usePotionHpRate;

	InOptionData->mViewHelmet = _isVisibleHelmet;

	InOptionData->mOffLineOptionInfo = _offlineOption;

	InOptionData->mCombatOptionInfo.mAutoAttackRange = _autoAttackRange;
	InOptionData->mCombatOptionInfo.mAutoDropItemGrade = _autoDropItemGrade;
	InOptionData->mCombatOptionInfo.mAutoDropItemWeight = (int)_autoDropItemWeight;
	InOptionData->mCombatOptionInfo.mManualCombat.mCounterAttack = _isCounterAttackOn;
	//InOptionData->mCombatOptionInfo.mManualCombat.mAutoSkill = _isAutoSkillOn;

	for (int i = 0; i < MAX_COMBAT_OPTION_COUNT; ++i)
	{
		InOptionData->mCombatOptionInfo.mAutoCombatTargetOptionList[i] = _autoCombatTargetOptionList[i];
		InOptionData->mCombatOptionInfo.mManualCombatTargetOptionList[i] = _manualCombatTargetOptionList[i];
	}

	// 2023/8/7 PKT - Offline Play 관련 셋팅
	if (GOption()->GetGameUserSettingsEx()->GetAutoSettings().Contains(EGsOptionAuto::AUTO_SKILL_MANA_LIMIT_RATE))
	{
		const float rate = GOption()->GetGameUserSettingsEx()->GetAutoSetting(EGsOptionAuto::AUTO_SKILL_MANA_LIMIT_RATE);
		InOptionData->mCombatOptionInfo.mAutoSkillRate = FMath::RoundToInt(rate * 100);
	}

	if (GOption()->GetGameUserSettingsEx()->GetAutoSettings().Contains(EGsOptionAuto::IS_ALWAYS_USE_AUTO_BUFF_SKILL))
	{
		InOptionData->mCombatOptionInfo.mIsMpUseSkillSkill 
			= static_cast<bool>(GOption()->GetGameUserSettingsEx()->GetAutoSetting(EGsOptionAuto::IS_ALWAYS_USE_AUTO_BUFF_SKILL));
	}

	if (GOption()->GetGameUserSettingsEx()->GetAutoSettings().Contains(EGsOptionAuto::AUTO_ITEM_LOOT_OTHER_USERS))
	{
		InOptionData->mCombatOptionInfo.mOtherPlayersDropItem 
			= static_cast<bool>(GOption()->GetGameUserSettingsEx()->GetAutoSetting(EGsOptionAuto::AUTO_ITEM_LOOT_OTHER_USERS));
	}

	if (GOption()->GetGameUserSettingsEx()->GetCombatSettings().Contains(EGsOptionCombat::MANNER_MODE))
	{
		// 2023/8/8 PKT - 매너 타겟 옵션 없음. 이것 추가 해달라고 요청해야 함.
		InOptionData->mCombatOptionInfo.mMannerMode 
			= static_cast<MannerMode>(GOption()->GetGameUserSettingsEx()->GetCombatSetting(EGsOptionCombat::MANNER_MODE));
	}

	InOptionData->mSocialMediaInfo = _socialMediaOption;

	InOptionData->mWorldMapFilterInfo = _worldMapFilter;
		
	InOptionData->mRadarOptionInfo = _scanOption;

	InOptionData->mHivePushNotification = _hivePushNotification;

	InOptionData->mClientNotification = _clientNotification;

	InOptionData->mHiddenNameGetItem = _hiddenNameGetItem;

	InOptionData->mMonitoringTargetNotification = _PKBookOption;

	InOptionData->mPersonalTradingAutoReject = _personalTradingAutoReject;

	InOptionData->mLoginAlram = _loginAlram;

	InOptionData->mIgnoreCinematicRewardPopup = _isIgnoreCinematicRewardPopup;

	InOptionData->mOptionSelectTypeInfo = _itempickupOption;

	InOptionData->mAutoDecompose = _autoDecomposeOption;

	InOptionData->mPcRvo = _pcRvoIndex;

	// 천마석(정령탄) 옵션도 서버에 전송
	// 여기서 세팅 안하면 false로 저장되어서
	// 클라쪽 데이터랑 안맞게 됨
	InOptionData->mSpiritShotOnOff = GGameData()->GetIsSpiritShotOnOff();
}

void FGsServerOption::ConvertToClientData(const GameOption* inOptionData)
{
	if (nullptr == inOptionData)
		return;

	_autoCombatTargetOptionList.Empty();
	_manualCombatTargetOptionList.Empty();

	_usePotionHpRate = inOptionData->mAutoUsePotionInfo.mUsePotionHpRate;
	_isAutoUsePotion = inOptionData->mAutoUsePotionInfo.mIsAutoUsePotion;
	_isVisibleHelmet = inOptionData->mViewHelmet;
	_offlineOption = inOptionData->mOffLineOptionInfo;
	_autoAttackRange = inOptionData->mCombatOptionInfo.mAutoAttackRange;
	_autoDropItemGrade = inOptionData->mCombatOptionInfo.mAutoDropItemGrade;
	_autoDropItemWeight = (EGsOptionValueAutoItemLootWeightLimit)inOptionData->mCombatOptionInfo.mAutoDropItemWeight;
	_socialMediaOption = inOptionData->mSocialMediaInfo;
	_worldMapFilter = inOptionData->mWorldMapFilterInfo;
	_isCounterAttackOn = inOptionData->mCombatOptionInfo.mManualCombat.mCounterAttack;
	//_isAutoSkillOn = inOptionData->mCombatOptionInfo.mManualCombat.mAutoSkill;
	_scanOption = inOptionData->mRadarOptionInfo;
	_hivePushNotification = inOptionData->mHivePushNotification;
	_clientNotification = inOptionData->mClientNotification;
	_hiddenNameGetItem = inOptionData->mHiddenNameGetItem;
	_loginAlram = inOptionData->mLoginAlram;
	_PKBookOption = inOptionData->mMonitoringTargetNotification;
	_personalTradingAutoReject = inOptionData->mPersonalTradingAutoReject;
	_isIgnoreCinematicRewardPopup = inOptionData->mIgnoreCinematicRewardPopup;
	_pcRvoIndex = inOptionData->mPcRvo;

	_itempickupOption = inOptionData->mOptionSelectTypeInfo;
	_autoDecomposeOption = inOptionData->mAutoDecompose;

	for (int i = 0; i < MAX_COMBAT_OPTION_COUNT; ++i)
	{
		_autoCombatTargetOptionList.Emplace(inOptionData->mCombatOptionInfo.mAutoCombatTargetOptionList[i]);
		_manualCombatTargetOptionList.Emplace(inOptionData->mCombatOptionInfo.mManualCombatTargetOptionList[i]);
	}
}

void FGsServerOption::SendToServer()
{
	ClientOptionData optionData;
	ConvertToServerData(&optionData);

	FGsNetSendServiceWorld::SendReqOption(optionData);
}

void FGsServerOption::SendToServerGameOption()
{
	GameOption optionData;
	ConvertToServerData(&optionData);

	FGsNetSendServiceWorld::SendReqGameOption(optionData);
}

void FGsServerOption::SetQuestSubUIVisibility(bool inIsShow, const StoryId inStoryId)
{
	if (inIsShow == false)
	{
		// 없으면 추가
		if (!_hideUISubQuestIdList.Contains(inStoryId))
		{
			_hideUISubQuestIdList.Add(inStoryId);
		}
	}
	else
	{
		// 있으면 삭제
		if (_hideUISubQuestIdList.Contains(inStoryId))
		{
			_hideUISubQuestIdList.Remove(inStoryId);
		}
	}

	SendToServer();
}

void FGsServerOption::SetQuestRepeatUIVisibility(bool inIsShow, const QuestIndex inQuestIndex)
{
	if (inIsShow == false)
	{
		// 없으면 추가
		if (!_hideUIRepeatQuestIdList.Contains(inQuestIndex))
		{
			_hideUIRepeatQuestIdList.Add(inQuestIndex);
		}
	}
	else
	{
		// 있으면 삭제
		if (_hideUIRepeatQuestIdList.Contains(inQuestIndex))
		{
			_hideUIRepeatQuestIdList.Remove(inQuestIndex);
		}
	}

	SendToServer();
}

//void FGsServerOption::SetQuestGuideUIVisibility(bool inIsShow, const StoryId inStoryId)
//{
//	if (inIsShow == false)
//	{
//		// 없으면 추가
//		if (!_hideUIGuideQuestIdList.Contains(inStoryId))
//		{
//			_hideUIGuideQuestIdList.Add(inStoryId);
//		}
//	}
//	else
//	{
//		// 있으면 삭제
//		if (_hideUIGuideQuestIdList.Contains(inStoryId))
//		{
//			_hideUIGuideQuestIdList.Remove(inStoryId);
//			/*if (_hideUIGuideQuestIdList.Num() < MAX_QUEST_GUIDE_UI_VIEW_OFF_COUNT)
//			{
//				_hideUIGuideQuestIdList.Add(inStoryId);
//			}*/
//		}
//	}
//
//	SendToServer();
//}
//
//void FGsServerOption::SetQuestGuideUserSelectHide(bool inIsShow, const StoryId inStoryId)
//{
//	if (inIsShow == false)
//	{
//		// 없으면 추가 (숨기는 경우, 눈동자 On -> Off)
//		if (!_userSelectHideGuideQuestIdList.Contains(inStoryId))
//		{
//			if (_userSelectHideGuideQuestIdList.Num() < MAX_QUEST_GUIDE_UI_VIEW_OFF_COUNT)
//			{
//				_userSelectHideGuideQuestIdList.Add(inStoryId);
//			}
//		}
//	}
//	else
//	{
//		// 있으면 삭제 (보이는 경우, 눈동자 Off -> On)
//		if (_userSelectHideGuideQuestIdList.Contains(inStoryId))
//		{
//			_userSelectHideGuideQuestIdList.Remove(inStoryId);
//		}
//	}
//
//	SendToServer();
//}

void FGsServerOption::SetQuestGuideOnList(bool inIsAdd, StoryId inStoryId)
{
	if (inIsAdd == false)
	{
		// 있으면 삭제
		if (_guideQuestHudOnList.Contains(inStoryId))
		{
			_guideQuestHudOnList.Remove(inStoryId);
		}
	}
	else
	{		
		// 없으면 추가
		if (!_guideQuestHudOnList.Contains(inStoryId))
		{
			_guideQuestHudOnList.Add(inStoryId);
		}
	}

	SendToServer();
}

void FGsServerOption::SetQuestGuideOffList(bool inIsAdd, StoryId inStoryId)
{
	if (inIsAdd == false)
	{	
		if (_guideQuestHudOffList.Contains(inStoryId))
		{
			_guideQuestHudOffList.Remove(inStoryId);
		}		
	}
	else
	{
		if (!_guideQuestHudOffList.Contains(inStoryId))
		{
			if (_guideQuestHudOffList.Num() < MAX_QUEST_GUIDE_UI_VIEW_OFF_COUNT)
			{
				_guideQuestHudOffList.Add(inStoryId);
			}
		}
	}

	SendToServer();
}

void FGsServerOption::SetRemoveQuestRepeatViewIndexList(StoryId inStoryId, QuestKey inQuestKey)
{
	QuestStoryIdPair questId = QuestStoryIdPair(inStoryId, inQuestKey._questId, inQuestKey._questIndex);
	if (_repeatQuestviewIndexList.Contains(questId))
	{
		_repeatQuestviewIndexList.Remove(questId);
		SendToServer();
	}
}

void FGsServerOption::SetRepeatQuestviewIndexList(const TArray<QuestStoryIdPair>& inRepeatQuestviewIndexList)
{
	_repeatQuestviewIndexList = inRepeatQuestviewIndexList;	
}

void FGsServerOption::SaveRepeatQuestviewIndexList()
{
	SendToServer();
}

void FGsServerOption::SetUsePotionHpRate(UsePotionHpRate inUsePotionHpRate)
{
	_usePotionHpRate = inUsePotionHpRate;
}

void FGsServerOption::SetisAutoUsePotion(bool inisAutoUsePotion)
{
	_isAutoUsePotion = inisAutoUsePotion;
}

void FGsServerOption::SetAutoCombatTargetOptionList(const TArray<int>& inCombatTargetOptionList)
{
	_autoCombatTargetOptionList = inCombatTargetOptionList;
}

void FGsServerOption::SetManualCombatTargetOptionList(const TArray<int>& inCombatTargetOptionList)
{
	_manualCombatTargetOptionList = inCombatTargetOptionList;
}

void FGsServerOption::SetAutoCombatTargetOption(uint8 inIndex, int inEnable)
{
	if (inIndex >= MAX_COMBAT_OPTION_COUNT ||
		inIndex >= _autoCombatTargetOptionList.Num())
	{
		return;
	}

	_autoCombatTargetOptionList[inIndex] = inEnable;
}

void FGsServerOption::SetManualCombatTargetOption(uint8 inIndex, int inEnable)
{
	if (inIndex >= MAX_COMBAT_OPTION_COUNT ||
		inIndex >= _manualCombatTargetOptionList.Num())
	{
		return;
	}

	_manualCombatTargetOptionList[inIndex] = inEnable;
}

bool FGsServerOption::IsAutoCombatTargetOption(uint8 inIndex) const
{
	if (inIndex >= MAX_COMBAT_OPTION_COUNT ||
		inIndex >= _autoCombatTargetOptionList.Num())
	{
		return false;
	}

	return _autoCombatTargetOptionList[inIndex] > 0 ? true : false;
}

bool FGsServerOption::IsManualCombatTargetOption(uint8 inIndex) const
{
	if (inIndex >= MAX_COMBAT_OPTION_COUNT ||
		inIndex >= _manualCombatTargetOptionList.Num())
	{
		return false;
	}

	return _manualCombatTargetOptionList[inIndex] > 0 ? true : false;
}

void FGsServerOption::SetIsVisibleHelmet(bool isVisibleHelmet)
{
	_isVisibleHelmet = isVisibleHelmet;
}

void FGsServerOption::SetOfflineOption(const OffLineOptionInfo& inOffline)
{
	_offlineOption = inOffline;
}

void FGsServerOption::SetAutoAttackRange(const AutoAttackRange InAutoAttackRank)
{
	_autoAttackRange = InAutoAttackRank;
}

void FGsServerOption::SetDropItemGrade(const AutoDropItemGrade InAutoDropItemGrade)
{
	_autoDropItemGrade  = InAutoDropItemGrade;
}

void FGsServerOption::SetSocialMediaInfoOption(const SocialMediaInfo& InSocialOption)
{
	_socialMediaOption = InSocialOption;
}
void FGsServerOption::SetWorldMapFilter(const WorldMapFilterInfo& InWorldMapFilter)
{
	_worldMapFilter = InWorldMapFilter;
}
void FGsServerOption::SetIsCounterAttackOn(bool In_isCounterAttackOn)
{
	_isCounterAttackOn = In_isCounterAttackOn;
}
// void FGsServerOption::SetIsAutoSkillOn(bool In_isAutoSkillOn)
// {
// 	_isAutoSkillOn = In_isAutoSkillOn;
// }
void FGsServerOption::SetAutoDropItemWeight(int InAutoDropItemWeight)
{
	_autoDropItemWeight  = (EGsOptionValueAutoItemLootWeightLimit)InAutoDropItemWeight;
}
void FGsServerOption::SetScanOption(const RadarOptionInfo& InScanOption)
{
	_scanOption = InScanOption;
}

void FGsServerOption::SetPKBookOption(const MonitoringTargetNotification& InPKBookOption)
{
	_PKBookOption = InPKBookOption;
}

void FGsServerOption::SetPersonalTradingAutoReject(bool isAutoReject)
{
	_personalTradingAutoReject = isAutoReject;
}

void FGsServerOption::SetHivePushNotification(const HivePushNotification& InHivePushNotification)
{
	_hivePushNotification = InHivePushNotification;
}

void FGsServerOption::SetClientNotification(const ClientNotification& InClientNotification)
{
	_clientNotification = InClientNotification;
}

void FGsServerOption::SetLoginAlram(const LoginAlram& InLoginAlram)
{
	_loginAlram = InLoginAlram;
}

void FGsServerOption::SetHiddenNameGetItem(bool InhiddenNameGetItem)
{
	_hiddenNameGetItem = InhiddenNameGetItem;
}


void FGsServerOption::SetPickupItemOption(const OptionSelectTypeInfo& InOption)
{
	_itempickupOption = InOption;
}


void FGsServerOption::SetOptionAlert(EGsOptionAlert InOptType, int32 InVal)
{
	switch (InOptType)
	{
	case EGsOptionAlert::ALL_ALERT:
		break;
	case EGsOptionAlert::CONSOLE_ALERT:
		break;
	case EGsOptionAlert::CONSOLE_ALERT_NIGHT:
		break;
	case EGsOptionAlert::ITEM_ADD_ALERT:
		break;
	case EGsOptionAlert::ITEMGAIN_GLOBAL_MESSGE_NAME_HIDE:
		_hiddenNameGetItem = static_cast<bool>(InVal);
		break;
	}
}

void FGsServerOption::SetOptionLoginAlert(EGsOptionLoginAlert InOptType, int32 InVal)
{
	switch (InOptType)
	{
	case EGsOptionLoginAlert::CONNECT_ALERT:
		_loginAlram.mLoginAlramOn = static_cast<bool>(InVal);
		break;
	case EGsOptionLoginAlert::GUILD_ALERT:
		_loginAlram.mGuildMemberLoginOn = static_cast<bool>(InVal);
		break;
	case EGsOptionLoginAlert::BUDDY_ALERT:
		_loginAlram.mBuddyLoginOn = static_cast<bool>(InVal);
		break;
	case EGsOptionLoginAlert::MAX:
		break;
	default:
		break;
	}
}

bool FGsServerOption::GetOptionLoginAlert(EGsOptionLoginAlert InOptType)
{
	switch (InOptType)
	{
	case EGsOptionLoginAlert::CONNECT_ALERT:
		return static_cast<bool>(_loginAlram.mLoginAlramOn);
		break;
	case EGsOptionLoginAlert::GUILD_ALERT:
	{
		if (false == static_cast<bool>(_loginAlram.mLoginAlramOn))
			return false;

		return static_cast<bool>(_loginAlram.mGuildMemberLoginOn);
	}
	break;
	case EGsOptionLoginAlert::BUDDY_ALERT:
	{
		if (false == static_cast<bool>(_loginAlram.mLoginAlramOn))
			return false;

		return static_cast<bool>(_loginAlram.mBuddyLoginOn);
	}
	break;
	case EGsOptionLoginAlert::MAX:
		break;
	default:
		break;
	}

	return false;
}

void FGsServerOption::SetOptionNotificationAlert(EGsOptionNotificationAlert InOptType, float InVal)
{
	switch (InOptType)
	{
	case EGsOptionNotificationAlert::PVP_RINGER_ALERT:
		_clientNotification.mAutoKillTakeDamageNotification.mSoundOn = static_cast<bool>(InVal);
		break;
	case EGsOptionNotificationAlert::PVP_VIBRATE_ALERT:
		_clientNotification.mAutoKillTakeDamageNotification.mVibrationOn = static_cast<bool>(InVal);
		break;
	case EGsOptionNotificationAlert::HP_RATE_ALERT:
		_clientNotification.mHPDangerousNotification.mAlertPercent = FMath::RoundToInt(InVal * 100);
		break;
	case EGsOptionNotificationAlert::HP_RATE_RINGER_ALERT:
		_clientNotification.mHPDangerousNotification.mSoundOn = static_cast<bool>(InVal);
		break;
	case EGsOptionNotificationAlert::HP_RATE_VIBRATE_ALERT:
		_clientNotification.mHPDangerousNotification.mVibrationOn = static_cast<bool>(InVal);
		break;
	case EGsOptionNotificationAlert::WEIGHT_ALERT:
		_clientNotification.mWeightNotification.mAlertPercent = FMath::RoundToInt(InVal * 100);
		break;
	case EGsOptionNotificationAlert::WEIGHT_RINGER_ALERT:
		_clientNotification.mWeightNotification.mSoundOn = static_cast<bool>(InVal);
		break;
	case EGsOptionNotificationAlert::WEIGHT_VIBRATE_ALERT:
		_clientNotification.mWeightNotification.mVibrationOn = static_cast<bool>(InVal);
		break;
	case EGsOptionNotificationAlert::DEATH_RINGER_ALERT:
		_clientNotification.mDeadNotification.mSoundOn = static_cast<bool>(InVal);
		break;
	case EGsOptionNotificationAlert::DEATH_VIBRATE_ALERT:
		_clientNotification.mDeadNotification.mVibrationOn = static_cast<bool>(InVal);
		break;
	case EGsOptionNotificationAlert::AUTO_POTION_ALERT:
		_clientNotification.mAutoPotionLowNotification.mAlertCount = InVal;
		break;
	case EGsOptionNotificationAlert::HP_POTION_RINGER_ALERT:
		_clientNotification.mAutoPotionLowNotification.mSoundOn = static_cast<bool>(InVal);
		break;
	case EGsOptionNotificationAlert::HP_POTION_VIBRATE_ALERT:
		_clientNotification.mAutoPotionLowNotification.mVibrationOn = static_cast<bool>(InVal);
		break;
	case EGsOptionNotificationAlert::EQUANIMITY_ALERT:
		_clientNotification.mRepeat = static_cast<bool>(InVal);
		break;
	case EGsOptionNotificationAlert::EQUANIMITY_ALERT_INTERVALTIME:
		_clientNotification.mTimePeriodms = InVal * 1000;
		break;
	case EGsOptionNotificationAlert::COMBAT_STATE_ALERT:
		_clientNotification.mCombatRepeat = static_cast<bool>(InVal);
		break;
	case EGsOptionNotificationAlert::COMBAT_STATE_ALERT_INTERVALTIME:
		_clientNotification.mCombatTimePeriodms = InVal * 1000;
		break;
	case EGsOptionNotificationAlert::MAX:
		break;
	default:
		break;
	}
}

void FGsServerOption::OnRingerPlay(EGsAlertType alertType)
{
	FGsAlertHelper::PlayAlertEffect(alertType);
}

void FGsServerOption::OnVibratePlay(EGsAlertType alertType)
{
	FGsAlertHelper::PlayAlertEffect(alertType);
}

bool FGsServerOption::GetOptionNotificationAlert(EGsOptionNotificationAlert InOptType)
{
	if (false == _clientNotification.mRepeat)
		return false;

	if (!_notificationAlertDelayInfos.Contains(InOptType))
		return false;

	FDateTime nowTime = FDateTime::Now();
	FDateTime endTime = _notificationAlertDelayInfos[InOptType];
	FTimespan waitingTime = nowTime - endTime;

	if (waitingTime.GetTotalMilliseconds() < _clientNotification.mTimePeriodms)
		return false;

	bool isOn = false;
	switch (InOptType)
	{
	case EGsOptionNotificationAlert::DEATH_ALERT:
		{		
			if (_clientNotification.mDeadNotification.mSoundOn)
			{
				OnRingerPlay();
				isOn = true;
			}
			if (_clientNotification.mDeadNotification.mVibrationOn)
			{
				OnVibratePlay();
				isOn = true;
			}
		}
		break;
	case EGsOptionNotificationAlert::HP_POTION_ALERT:
		{
			if (_clientNotification.mAutoPotionLowNotification.mSoundOn)
			{
				OnRingerPlay();
				isOn = true;
			}
			if (_clientNotification.mAutoPotionLowNotification.mVibrationOn)
			{
				OnVibratePlay();
				isOn = true;
			}
		}
		break;
	default:
		break;
	}

	_notificationAlertDelayInfos[InOptType] = nowTime;

	return isOn;
}

void FGsServerOption::SetOptionOfflineAlert(EGsOptionOfflineAlert InOptType, float InVal)
{
	switch (InOptType)
	{
	case EGsOptionOfflineAlert::OFFLINE_PLAY_ALERT:
		_hivePushNotification.mRepeat = static_cast<bool>(InVal);
		break;
	case EGsOptionOfflineAlert::OFFLINE_PLAY_ALERT_INTERVALTIME:
		_hivePushNotification.mTimePeriodms = InVal * 1000;
		break;
	case EGsOptionOfflineAlert::OFFLINE_INVEN_ALERT:
		_hivePushNotification.mFullInventory = static_cast<bool>(InVal);
		break;
	case EGsOptionOfflineAlert::OFFLINE_WEIGHT_ALERT:
		_hivePushNotification.mWeightOver = static_cast<bool>(InVal);
		break;
	case EGsOptionOfflineAlert::OFFLINE_PLAYFINISHED_ALERT:
		_hivePushNotification.mFinishOfflinePlay = static_cast<bool>(InVal);
		break;
	case EGsOptionOfflineAlert::OFFLINE_ATTACKED_ALERT:
		_hivePushNotification.mTakeDamageByPVP = static_cast<bool>(InVal);
		break;
	case EGsOptionOfflineAlert::OFFLINE_DEATHALERT:
		_hivePushNotification.mDead = static_cast<bool>(InVal);
		break;
	case EGsOptionOfflineAlert::OFFLINE_DUNGEON_TIMEOUT_ALERT:
		_hivePushNotification.mFinishDungeon = static_cast<bool>(InVal);
		break;
	case EGsOptionOfflineAlert::OFFLINE_EQUIP_ITEM_ACQUISITION_ALERT:
		_hivePushNotification.mNotiItemGrade = FGsOptionFunc::ConvertAutoItemGainOptionEnumToServerEnum(StaticCast<int32>(InVal));
		break;
	case EGsOptionOfflineAlert::OFFLINE_SKILL_BOOK_ACQUISITION_ALERT:
		_hivePushNotification.mNotiSkillBookGrade = FGsOptionFunc::ConvertAutoItemGainOptionEnumToServerEnum(StaticCast<int32>(InVal));
		break;
	case EGsOptionOfflineAlert::OFFLINE_MARKET_SALE_COMPLETED_ALERT:
		_hivePushNotification.mMarketItemSell = static_cast<bool>(InVal);
		break;
	case EGsOptionOfflineAlert::MAX:
		break;
	default:
		break;
	}
}


void FGsServerOption::SetOptionPKBookAlert(EGsOptionPKBookAlert InOptType, int32 InVal)
{
	switch (InOptType)
	{
	case EGsOptionPKBookAlert::CLOSE_TARGET_DETECTION_ALERT_INTERVAL:			// 2022/09/30 PKT - 경계대상 근접 알림 - 주기
		_PKBookOption.mNotificationCycleSecond = InVal;
		break;
	case EGsOptionPKBookAlert::CLOSE_TARGET_DETECTION_ALERT_DISTANCE:			// 2022/09/30 PKT - 경계대상 근접 알림 - 거리
		_PKBookOption.mNotificationRange = InVal;
		break;
	case EGsOptionPKBookAlert::CLOSE_TARGET_DETECTION_RINGER_ALERT:			// 2022/09/30 PKT - 경계대상 근접 알림 - 소리
		_PKBookOption.mSoundOn = static_cast<bool>(InVal);
		break;
	case EGsOptionPKBookAlert::CLOSE_TARGET_DETECTION_VIBRATE_ALERT:			// 2022/09/30 PKT - 경계대상 근접 알림 - 진동	
		_PKBookOption.mVibrationOn = static_cast<bool>(InVal);
		break;
	default:
		break;
	}
}

void FGsServerOption::SetOptionAuto(EGsOptionAuto InOptType, float InVal)
{
	switch (InOptType)
	{
	case EGsOptionAuto::PERSONAL_TRADING_AUTO_REJECT:
		SetPersonalTradingAutoReject(static_cast<bool>(InVal));
		break;
	default:
		break;
	}
}

void FGsServerOption::CheckHPAlert()
{
	if (false == _clientNotification.mCombatRepeat)
		return;

	if (!_notificationAlertDelayInfos.Contains(EGsOptionNotificationAlert::HP_RATE_ALERT))
		return;

	if (0 >= _clientNotification.mHPDangerousNotification.mAlertPercent)
		return;

	// 소리, 진동 알람 둘 다 꺼져 있으면 return
	if (false == _clientNotification.mHPDangerousNotification.mSoundOn &&
		false == _clientNotification.mHPDangerousNotification.mVibrationOn)
	{
		return;
	}

	FDateTime nowTime = FDateTime::Now();
	FDateTime endTime = _notificationAlertDelayInfos[EGsOptionNotificationAlert::HP_RATE_ALERT];

	FTimespan waitingTime = nowTime - endTime;

	if (waitingTime.GetTotalMilliseconds() > _clientNotification.mCombatTimePeriodms)
	{
		// [B1] | ejrrb10 | '다른 플레이어의 공격 시 알림' 플레이 도중에는 알림 울리지 않도록 설정
		// 다만 사운드와 진동은 독립적으로 작동한다
		if (_clientNotification.mHPDangerousNotification.mSoundOn)
		{
			bool isOn = true;

			if (_clientNotification.mAutoKillTakeDamageNotification.mSoundOn &&
				_notificationAlertDelayInfos.Contains(EGsOptionNotificationAlert::PVP_ALERT))
			{
				endTime = _notificationAlertDelayInfos[EGsOptionNotificationAlert::PVP_ALERT];
				waitingTime = nowTime - endTime;

				// 만약 PVP 소리 알림이 울린 상태라면 체력 부족 소리 알림 재생 안함
				if (waitingTime.GetTotalMilliseconds() < _clientNotification.mCombatTimePeriodms)
					isOn = false;
			}

			if (isOn)
			{
				OnRingerPlay(EGsAlertType::LowHpAlert);
			}
		}
		if (_clientNotification.mHPDangerousNotification.mVibrationOn)
		{
			bool isOn = true;

			if (_clientNotification.mAutoKillTakeDamageNotification.mVibrationOn &&
				_notificationAlertDelayInfos.Contains(EGsOptionNotificationAlert::PVP_ALERT))
			{
				endTime = _notificationAlertDelayInfos[EGsOptionNotificationAlert::PVP_ALERT];
				waitingTime = nowTime - endTime;

				// 만약 PVP 진동 알림이 울린 상태라면 체력 부족 진동 알림 재생 안함
				if (waitingTime.GetTotalMilliseconds() < _clientNotification.mCombatTimePeriodms)
					isOn = false;
			}
			if (isOn)
			{
				OnVibratePlay();
			}
		}

		_notificationAlertDelayInfos[EGsOptionNotificationAlert::HP_RATE_ALERT] = nowTime;
	}
}

void FGsServerOption::UpdateInvenWeightCallback(const struct IGsMessageParam* InParam)
{
	if (false == _clientNotification.mRepeat)
		return;

	if (!_notificationAlertDelayInfos.Contains(EGsOptionNotificationAlert::WEIGHT_ALERT))
		return;	

	if (0 >= _clientNotification.mWeightNotification.mAlertPercent)
		return;

	if (nullptr == InParam)
		return;

	const FGsUpdateInvenWeightInfo* param = InParam->Cast<const FGsUpdateInvenWeightInfo>();
	if (nullptr == param)
		return;

	int32 resultValue = param->_weightPercent;

	if (resultValue < _clientNotification.mWeightNotification.mAlertPercent)
		return;

	FDateTime nowTime = FDateTime::Now();
	FDateTime endTime = _notificationAlertDelayInfos[EGsOptionNotificationAlert::WEIGHT_ALERT];

	FTimespan waitingTime = nowTime - endTime;
	if (waitingTime.GetTotalMilliseconds() > _clientNotification.mTimePeriodms)
	{
		bool isOn = false;
		if (_clientNotification.mWeightNotification.mSoundOn)
		{
			OnRingerPlay();
			isOn = true;
		}
		if (_clientNotification.mWeightNotification.mVibrationOn)
		{
			OnVibratePlay();
			isOn = true;
		}

		_notificationAlertDelayInfos[EGsOptionNotificationAlert::WEIGHT_ALERT] = nowTime;
	}
}

void FGsServerOption::OnHitLocal(const IGsMessageParam* inParam)
{
	if (!_notificationAlertDelayInfos.Contains(EGsOptionNotificationAlert::PVP_ALERT))
		return;

	const FGsGameObjectMessageParamTarget* paramTarget = static_cast<const FGsGameObjectMessageParamTarget*>(inParam);
	if (nullptr == paramTarget)
		return;
		
	// 히트 id 갱신
	// 공격자가 없어도 필요없다.
	UGsGameObjectBase* attacker = paramTarget->_paramTarget;
	if (nullptr == attacker)
		return;

	// 이제 체력이 일정 수치 이하일 경우의 케이스 체크 시작
	UGsGameObjectLocalPlayer* localCreature = Cast<UGsGameObjectLocalPlayer>(GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer));
	if (nullptr == localCreature)
		return;

	const FGsStatInfo* statInfo = localCreature->GetCreatureStatInfoByType(StatType::HP);

	if (nullptr == statInfo)
		return;

	float statValue = (float)statInfo->GetStatValue();
	float maxStatValue = (float)statInfo->GetMaxStatValue();

	float statRatio = statValue / maxStatValue;

	if (statRatio <= _clientNotification.mHPDangerousNotification.mAlertPercent * 0.01f)
	{
		// 공격자가 NPC 이든 플레이어든, 체력 알림은 울릴 수 있다
		CheckHPAlert();
	}

	// 이제 PVP 케이스 체크 시작

	// 오토중이 아니면 필요없다
	if (false == GSAI()->IsAIOn())
		return;

	// only set remote player
	if (false == attacker->IsObjectType(EGsGameObjectType::RemotePlayer))
		return;

	// HUD 피격 효과 출력
	OnHpWarningEffectHUD();

	// 사운드 출력 부
	FDateTime nowTime = FDateTime::Now();
	FDateTime endTime = _notificationAlertDelayInfos[EGsOptionNotificationAlert::PVP_ALERT];
	FTimespan waitingTime = nowTime - endTime;

	if (waitingTime.GetTotalMilliseconds() < _clientNotification.mCombatTimePeriodms)
		return;

	// [B1] | ejrrb10 | '생명력 위험 알림' 사운드 플레이 도중에는 알림 울리지 않도록 설정
	// 체력이 사용자가 설정한 옵션값 이하일 경우, 타 플레이어 피격 사운드를 출력하지 않는다
	// 위에서 유저 체력 수치 일정 이하일 때는 출력 안하도록 설정하였지만, 소리와 진동은 별도로 동작하게 수정
	if (_clientNotification.mAutoKillTakeDamageNotification.mSoundOn)
	{
		bool isOn = true;

		if (_clientNotification.mHPDangerousNotification.mSoundOn &&
			_notificationAlertDelayInfos.Contains(EGsOptionNotificationAlert::HP_RATE_ALERT))
		{
			endTime = _notificationAlertDelayInfos[EGsOptionNotificationAlert::HP_RATE_ALERT];
			waitingTime = nowTime - endTime;
			
			// 만약 체력 부족 소리 알림이 울린 상태라면 PVP 소리 알림 재생 안함
			if (waitingTime.GetTotalMilliseconds() < _clientNotification.mCombatTimePeriodms)
				isOn = false;
		}

		if (isOn)
		{
			OnRingerPlay(EGsAlertType::PvpAlert);
		}
	}
	if (_clientNotification.mAutoKillTakeDamageNotification.mVibrationOn)
	{
		bool isOn = true;

		if (_clientNotification.mHPDangerousNotification.mVibrationOn &&
			_notificationAlertDelayInfos.Contains(EGsOptionNotificationAlert::HP_RATE_ALERT))
		{
			endTime = _notificationAlertDelayInfos[EGsOptionNotificationAlert::HP_RATE_ALERT];
			waitingTime = nowTime - endTime;

			// 만약 체력 부족 진동 알림이 울린 상태라면 PVP 진동 알림 재생 안함
			if (waitingTime.GetTotalMilliseconds() < _clientNotification.mCombatTimePeriodms)
				isOn = false;
		}

		if (isOn)
		{
			OnVibratePlay();
		}
	}

	_notificationAlertDelayInfos[EGsOptionNotificationAlert::PVP_ALERT] = nowTime;
}


void FGsServerOption::OnLocalPlayerDead(const IGsMessageParam* inParam)
{
	if (false == _clientNotification.mRepeat)
		return;

	if (!_notificationAlertDelayInfos.Contains(EGsOptionNotificationAlert::DEATH_ALERT))
		return;

	if (0 >= _clientNotification.mWeightNotification.mAlertPercent)
		return;	

	FDateTime nowTime = FDateTime::Now();
	FDateTime endTime = _notificationAlertDelayInfos[EGsOptionNotificationAlert::DEATH_ALERT];

	FTimespan waitingTime = nowTime - endTime;
	if (waitingTime.GetTotalMilliseconds() > _clientNotification.mTimePeriodms)
	{
		bool isOn = false;
		if (_clientNotification.mDeadNotification.mSoundOn)
		{
			OnRingerPlay();
			isOn = true;
		}
		if (_clientNotification.mDeadNotification.mVibrationOn)
		{
			OnVibratePlay();
			isOn = true;
		}

		_notificationAlertDelayInfos[EGsOptionNotificationAlert::DEATH_ALERT] = nowTime;
	}
}

void FGsServerOption::CheckHPPotionAlert(int inCount)
{
	if (false == _clientNotification.mRepeat)
		return;

	if (!_notificationAlertDelayInfos.Contains(EGsOptionNotificationAlert::HP_POTION_ALERT))
		return;

	// 옵션ui에서 셋팅한 hp포션 갯수
	int alertCount = _clientNotification.mAutoPotionLowNotification.mAlertCount;
	// 셋팅한 갯수가 0보다 클때
	// 내가 가진 갯수가 셋팅한 갯수보다 작을 때
	if (0 >= alertCount || inCount > alertCount)
		return;	

	// 자주자주 불리는걸 방지하기 위해 알람텀을 체크함
	FDateTime nowTime = FDateTime::Now();
	FDateTime endTime = _notificationAlertDelayInfos[EGsOptionNotificationAlert::HP_POTION_ALERT];

	// 울리고 난 후 마지막 시간을 체크해서 흐른시간 비교
	FTimespan waitingTime = nowTime - endTime;
	if (waitingTime.GetTotalMilliseconds() > _clientNotification.mTimePeriodms)
	{
		if (_clientNotification.mAutoPotionLowNotification.mSoundOn)
		{
			OnRingerPlay();			
		}
		if (_clientNotification.mAutoPotionLowNotification.mVibrationOn)
		{
			OnVibratePlay();
		}

		_notificationAlertDelayInfos[EGsOptionNotificationAlert::HP_POTION_ALERT] = nowTime;
	}
}

void FGsServerOption::AddItemListCallback(struct FGsItemMessageParamBase& InParam)
{
	FGsAddItemList* paramData = static_cast<FGsAddItemList*>(&InParam);
	if (nullptr == paramData)
		return;

	if (0 >= paramData->_itemDbidList.Num())
		return;	

	UGsItemManager* itemMgr = GItem();
	if (nullptr == itemMgr)
		return;

	for (const ItemDBId iter : paramData->_itemDbidList)
	{
		TWeakPtr<FGsItem> findItem = itemMgr->FindItem(iter, ItemType::CONSUME);
		if (findItem.IsValid())
		{
			if (findItem.Pin()->IsAutoHpPotionItem())
			{
				const ItemAmount totalCount = itemMgr->GetAllItemAmountByTypes(ItemType::CONSUME, ItemCategoryMain::POTION, ItemCategorySub::HP_POTION);
				CheckHPPotionAlert(totalCount);

				return;
			}
		}
	}
}

void FGsServerOption::UpdateItemCallback(struct FGsItemMessageParamBase& InParam)
{
	if (UGsItemManager* itemMgr = GItem())
	{
		const ItemAmount totalCount = itemMgr->GetAllItemAmountByTypes(ItemType::CONSUME, ItemCategoryMain::POTION, ItemCategorySub::HP_POTION);
		CheckHPPotionAlert(totalCount);
	}
}

void FGsServerOption::OnHpWarningEffectHUD()
{
	if (_isHUDEffectOn)
		return;

	// PVP 시에는 현재 다른 Window 가 화면을 덮고 있는 경우에만 피격 효과 출력
	if (FGsGameFlowGame* gameFlow = GMode()->GetGameFlow())
	{
		if (FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame())
		{
			if (FGsContentsMode::InGame::ContentsHud == contents->GetCurrentContentsMode())
			{
				return;
			}
		}
	}

	if (UGsUIManager* uiMgr = GUI())
	{
		if (false == uiMgr->IsActiveWidget(TEXT("TrayLowHP")))
		{
			uiMgr->Open(TEXT("TrayLowHP"));
		}
	}

	_isHUDEffectOn = true;

	float duration = GData()->GetGlobalData()->PvpHitEffectDuration;

	FTimerDelegate timerCallback;
	timerCallback.BindLambda([this] {
		StopHpWarningEffectHUD();
		});

	if (UWorld* world = GUI()->GetWorld())
	{
		world->GetTimerManager().SetTimer(_pvpHUDEffectTimerHandle, timerCallback, duration, false);
	}
}

void FGsServerOption::StopHpWarningEffectHUD()
{
	if (UGsUIManager* uiMgr = GUI())
	{
		if (uiMgr->IsActiveWidget(TEXT("TrayLowHP")))
		{
			uiMgr->CloseByKeyName(TEXT("TrayLowHP"));
		}
	}

	_isHUDEffectOn = false;
}

void FGsServerOption::SetIsIgnoreCinematicRewardPopup(bool InVal) 
{ 
	_isIgnoreCinematicRewardPopup = InVal; 
	SendToServerGameOption();
}

void FGsServerOption::SetPcRvo(int32 InVal)
{
	_pcRvoIndex = InVal;
}

void FGsServerOption::SetPickupItemOption(EGsPickUpItemOption InOptType, bool InVal)
{
	switch (InOptType)
	{
	case EGsPickUpItemOption::ItemGainCommonConsume:
		_itempickupOption.mItemGainCommonConsume = InVal;
		break;
	case EGsPickUpItemOption::ItemGainSpecialConsume:
		_itempickupOption.mItemGainSpecialConsume = InVal;
		break;
	case EGsPickUpItemOption::ItemGainCommonScroll:		//일반 주문서의 습득을
		_itempickupOption.mItemGainCommonScroll = InVal;
		break;
	case EGsPickUpItemOption::ItemGainSpecialScroll:	//특수 주문서의 습득을
		_itempickupOption.mItemGainSpecialScroll = InVal;
		break;
	case EGsPickUpItemOption::ItemGainEnchantScroll:	//강화 주문서의 습득을
		_itempickupOption.mItemGainEnchantScroll = InVal;
		break;
	case EGsPickUpItemOption::ItemGainOtherNormal:
		_itempickupOption.mItemGainOtherNormal = InVal;
		break;
	case EGsPickUpItemOption::ItemGainOtherSpecial:
		_itempickupOption.mItemGainOtherSpecial = InVal;
		break;
	case EGsPickUpItemOption::ItemGainOtherEtc:
		_itempickupOption.mItemGainOtherEtc = InVal;
		break;
	}
}

void FGsServerOption::SetAutoDecomposeOption(EGsOptionAutoDecompose InOptType, bool InVal)
{
	switch (InOptType)
	{
	case EGsOptionAutoDecompose::FIELD_BOSS:
		_autoDecomposeOption.mIsFieldBossDropItem = InVal;
		break;
	case EGsOptionAutoDecompose::ITEM_COLLECTION:
		_autoDecomposeOption.mIsItemCollectAble = InVal;
		break;
	case EGsOptionAutoDecompose::EQUIP_NORMAL:
		_autoDecomposeOption.mIsEquipNormal = InVal;
		break;
	case EGsOptionAutoDecompose::EQUIP_MAGIC:
		_autoDecomposeOption.mIsEquipMagic = InVal;
		break;
	case EGsOptionAutoDecompose::EQUIP_RARE:
		_autoDecomposeOption.mIsEquipRare = InVal;
		break;
	case EGsOptionAutoDecompose::SKILLBOOK_NORMAL:
		_autoDecomposeOption.mIsSkillBookNormal = InVal;
		break;
	case EGsOptionAutoDecompose::SKILLBOOK_MAGIC:
		_autoDecomposeOption.mIsSkillBookMagic = InVal;
		break;
	case EGsOptionAutoDecompose::SKILLBOOK_RARE:
		_autoDecomposeOption.mIsSkillBookRare = InVal;
		break;
	case EGsOptionAutoDecompose::SLATE_NORMAL:
		_autoDecomposeOption.mIsMonsterKnowwledgeSlateNormal = InVal;
		break;
	case EGsOptionAutoDecompose::SLATE_MAGIC:
		_autoDecomposeOption.mIsMonsterKnowwledgeSlateMagic = InVal;
		break;
	case EGsOptionAutoDecompose::SLATE_RARE:
		_autoDecomposeOption.mIsMonsterKnowwledgeSlateRare = InVal;
		break;
	}
}

void FGsServerOption::SetAutoDecomposeOption(const AutoDecompose& InOption)
{
	_autoDecomposeOption = InOption;
}