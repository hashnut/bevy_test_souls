// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Message/GsMessageItem.h"
#include "Message/GsMessageGameObject.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Option/GsOptionValueDefine.h"
#include "Option/GsAlertHelper.h"
#include "T1Project.h"
#include "GsGameUserSettings.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Message/GsMessageContents.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Quest/GsQuestData.h"

struct T1PROJECT_API FGsAlertDelayInfo
{
	FGsAlertDelayInfo() = default;
	FGsAlertDelayInfo(float inData) : _delay(inData) {}
	virtual ~FGsAlertDelayInfo() = default;

	bool _isLock = false;
	// Delay 시간 설정 (초)
	float _delay = 0.f;

	FTimerHandle _timer;

public:
	void StartTimer()
	{
		if (UWorld* world = GLevel()->GetWorld())
		{
			world->GetTimerManager().SetTimer(_timer, [this]()
				{
					_isLock = false;
				}, _delay, false);

			_isLock = true;
		}
	}
	void SetTimeDelay(float inDelay) { _delay = inDelay; }
	bool IsLock()
	{
		if (!_isLock)
		{
			StartTimer();
		}
		return _isLock;
	}
};

struct IGsMessageParam;
class T1PROJECT_API FGsServerOption
{
public:
	using MsgHandleInventory = TPair<MessageContentInven, FDelegateHandle>;
	using MsgItem = TPair<MessageItem, FDelegateHandle>;

protected:
	TArray<StoryId> _hideUISubQuestIdList;
	TArray<QuestIndex> _hideUIRepeatQuestIdList;
	TArray<StoryId> _guideQuestHudOnList;
	TArray<StoryId> _guideQuestHudOffList;

	TArray<QuestStoryIdPair> _repeatQuestviewIndexList;

	// GameOption
	UsePotionHpRate _usePotionHpRate = 1;
	bool _isAutoUsePotion = false;
	bool _isVisibleHelmet = true;
	OffLineOptionInfo _offlineOption;
	MonitoringTargetNotification _PKBookOption;
	TArray<int> _autoCombatTargetOptionList;
	TArray<int> _manualCombatTargetOptionList;
	AutoAttackRange		_autoAttackRange = AutoAttackRange::METER_50;	// 2021/09/13 PKT - 자동 전투 범위
	AutoDropItemGrade	_autoDropItemGrade = AutoDropItemGrade::ALL;	// 2021/09/13 PKT - 장비 자동 줍기 최소 등급
	SocialMediaInfo _socialMediaOption;
	WorldMapFilterInfo _worldMapFilter;
	bool _personalTradingAutoReject;
	int32 _pcRvoIndex = 0;							// RVO 적용 반경 설정 ( 추후 SharedEnum 정의로 변경 )
	
	OptionSelectTypeInfo _itempickupOption;			// 2023/5/1 PKT - 장비 아이템 제외 그 밖의 아이템들에 대한 줍기 옵션
	AutoDecompose _autoDecomposeOption;				// 자동 분해 옵션

	//message
	TArray<MsgGameObjHandle> _gameObjectMessageArray;
	TArray<MsgHandleInventory> _inventoryMessageArray;
	TArray<MsgItem>	_listItemActionDelegates;

	bool _isCounterAttackOn = false;
	// bool _isAutoSkillOn = false; // 사용 안함

	EGsOptionValueAutoItemLootWeightLimit _autoDropItemWeight = EGsOptionValueAutoItemLootWeightLimit::OFF;


	RadarOptionInfo _scanOption;

	TMap<EGsOptionNotificationAlert, FDateTime>		_notificationAlertDelayInfos;

	HivePushNotification	_hivePushNotification;
	ClientNotification		_clientNotification;
	LoginAlram				_loginAlram;
	bool					_hiddenNameGetItem = true;

	FTimerHandle _pvpHUDEffectTimerHandle;
	bool _isHUDEffectOn = false;

	bool _isIgnoreCinematicRewardPopup = false;

public:
	void Initialize();
	void Finalize();

private:
	void OnUserData(const IGsMessageParam* In_data);
	void SendToServer();

public:
	void SendToServerGameOption();

public:
	void ConvertToServerData(ClientOptionData* InOptionData);
	void ConvertToClientData(const ClientOptionData* inOptionData);
	void ConvertToServerData(GameOption* InOptionData);
	void ConvertToClientData(const GameOption* inOptionData);

	bool IsHideQuestSubUI(StoryId inStoryId) { return _hideUISubQuestIdList.Contains(inStoryId); }
	bool IsHideQuestRepeatUI(QuestIndex inQuestIndex) { return _hideUIRepeatQuestIdList.Contains(inQuestIndex); }
	//bool IsHideQuestGuideUI(StoryId inStoryId) { return _hideUIGuideQuestIdList.Contains(inStoryId); }
	//bool IsUserSelectHideQuestGuide(StoryId inStoryId) { return _userSelectHideGuideQuestIdList.Contains(inStoryId); }

	void SetQuestSubUIVisibility(bool inIsShow, const StoryId inStoryId);
	void SetQuestRepeatUIVisibility(bool inIsShow, const QuestIndex inQuestIndex);
	//void SetQuestGuideUIVisibility(bool inIsShow, const StoryId inStoryId);
	//void SetQuestGuideUserSelectHide(bool inIsShow, const StoryId inStoryId);
	void SetRepeatQuestviewIndexList(const TArray<QuestStoryIdPair>& inRepeatQuestviewIndexList);

	void SetUsePotionHpRate(UsePotionHpRate inUsePotionHpRate);
	void SetisAutoUsePotion(bool inisAutoUsePotion);
	void SetAutoCombatTargetOptionList(const TArray<int>& inCombatTargetOptionList);
	void SetManualCombatTargetOptionList(const TArray<int>& inCombatTargetOptionList);
	void SetAutoCombatTargetOption(uint8 inIndex, int inEnable = 0);
	void SetManualCombatTargetOption(uint8 inIndex, int inEnable = 0);
	void SetIsVisibleHelmet(bool isVisibleHelmet);
	void SetOfflineOption(const OffLineOptionInfo& inOffline);
	void SetAutoAttackRange(const AutoAttackRange InAutoAttackRank);
	void SetDropItemGrade(const AutoDropItemGrade InAutoDropItemGrade);
	void SetSocialMediaInfoOption(const SocialMediaInfo& InSocialOption);
	void SetWorldMapFilter(const WorldMapFilterInfo& InWorldMapFilter);
	void SetIsCounterAttackOn(bool In_isCounterAttackOn);
	// void SetIsAutoSkillOn(bool In_isAutoSkillOn);
	void SetAutoDropItemWeight(int InAutoDropItemWeight);
	void SetScanOption(const RadarOptionInfo& InScanOption);
	void SetPKBookOption(const MonitoringTargetNotification& InPKBookOption);
	void SetPersonalTradingAutoReject(bool isAutoReject);

	void SetHivePushNotification(const HivePushNotification& InHivePushNotification);
	void SetClientNotification(const ClientNotification& InClientNotification);
	void SetLoginAlram(const LoginAlram& InLoginAlram);
	void SetHiddenNameGetItem(bool InhiddenNameGetItem);

	void SetPickupItemOption(const OptionSelectTypeInfo& InOption);
	void SetAutoDecomposeOption(const AutoDecompose& InOption);

	void SetOptionAlert(EGsOptionAlert InOptType, int32 InVal);
	void SetOptionLoginAlert(EGsOptionLoginAlert InOptType, int32 InVal);
	void SetOptionNotificationAlert(EGsOptionNotificationAlert InOptType, float InVal);
	void SetOptionOfflineAlert(EGsOptionOfflineAlert InOptType, float InVal);
	void SetOptionPKBookAlert(EGsOptionPKBookAlert InOptType, int32 InVal);

	void SetOptionAuto(EGsOptionAuto InOptType, float InVal);
	void SetIsIgnoreCinematicRewardPopup(bool InVal);
	void SetPcRvo(int32 InVal);

	void SetPickupItemOption(EGsPickUpItemOption InOptType, bool InVal);
	void SetAutoDecomposeOption(EGsOptionAutoDecompose InOptType, bool InVal);

	const TArray<StoryId>& GetQuestSubUIVisibility() { return _hideUISubQuestIdList; }
	const TArray<QuestIndex>& GetQuestRepeatUIVisibility() { return _hideUIRepeatQuestIdList; }
	//const TArray<StoryId>& GetQuestGuideUIVisibility() { return _hideUIGuideQuestIdList; }
	//int GetQuestGuideUIVisibilityCount() { return _hideUIGuideQuestIdList.Num(); }
	int GetQuestRepeatUIVisibilityCount() { return _hideUIRepeatQuestIdList.Num(); }

	const TArray<StoryId>& GetQuestGuideOnList() { return _guideQuestHudOnList; }
	const TArray<StoryId>& GetQuestGuideOffList() { return _guideQuestHudOffList; }
	const TArray<QuestStoryIdPair>& GetRepeatQuestviewIndexList() const { return _repeatQuestviewIndexList; }

	void SetQuestGuideOnList(bool inIsAdd, StoryId inStoryId);
	void SetQuestGuideOffList(bool inIsAdd, StoryId inStoryId);
	void SetRemoveQuestRepeatViewIndexList(StoryId inStoryId, QuestKey inQuestKey);
	void SaveRepeatQuestviewIndexList();

	bool IsAutoUsePotion() const { return _isAutoUsePotion; }
	bool IsVisibleHelmet() const { return _isVisibleHelmet; }
	bool IsAutoCombatTargetOption(uint8 inIndex) const;
	bool IsManualCombatTargetOption(uint8 inIndex) const;
	UsePotionHpRate GetUsePotionHpRate() const { return _usePotionHpRate; }
	const TArray<int>& GetAutoCombatTargetOptionList() const { return _autoCombatTargetOptionList; }
	const TArray<int>& GetManualCombatTargetOptionList() const { return _manualCombatTargetOptionList; }
	const OffLineOptionInfo& GetOfflineOption() const { return _offlineOption; }
	AutoAttackRange GetAutoAttackRange() const { return _autoAttackRange; }
	AutoDropItemGrade GetDropItemGrade() const { return _autoDropItemGrade; }
	const SocialMediaInfo& GetSocialMediaOption() const { return _socialMediaOption; }
	WorldMapFilterInfo& GetWorldMapFilter() { return _worldMapFilter; }
	bool IsCounterAttackOn() { return _isCounterAttackOn; }
	// bool IsAutoSkillOn() { return _isAutoSkillOn; }
	EGsOptionValueAutoItemLootWeightLimit GetAutoDropItemWeight() const { return _autoDropItemWeight; }
	RadarOptionInfo& GetScanOption() { return _scanOption; }
	const MonitoringTargetNotification& GetPKBookOption() const { return _PKBookOption; }

	const OptionSelectTypeInfo& GetItempickupOption() const { return _itempickupOption; }
	const AutoDecompose& GetAutoDecomposeOption() const { return _autoDecomposeOption; }

	const ClientNotification& GetOptionAlert() const { return _clientNotification; }
	const ClientNotification& GetOptionNotificationAlert() const { return _clientNotification; }
	const LoginAlram& GetOptionLoginAlert() const { return _loginAlram; }
	const HivePushNotification& GetOptionOfflineAlert() { return _hivePushNotification; }

	bool GetHiddenNameGetItem() const { return _hiddenNameGetItem; }
	bool GetOptionLoginAlert(EGsOptionLoginAlert InOptType);
	bool GetOptionNotificationAlert(EGsOptionNotificationAlert InOptType);
	bool GetPersonalTradingAutoReject() const { return _personalTradingAutoReject; }

	bool GetIsIgnoreCinematicRewardPopup() const { return _isIgnoreCinematicRewardPopup; }

	int GetPcRvoIndex() const { return _pcRvoIndex; }

	void CheckHPAlert();
	void CheckHPPotionAlert(int inCount);
	void UpdateInvenWeightCallback(const struct IGsMessageParam* InParam);
	void UpdateItemCallback(struct FGsItemMessageParamBase& InParam);
	void AddItemListCallback(struct FGsItemMessageParamBase& InParam);
	void OnHitLocal(const IGsMessageParam* inParam);
	void OnLocalPlayerDead(const IGsMessageParam* inParam);

	void OnRingerPlay(EGsAlertType alertType = EGsAlertType::RingerAlert);
	void OnVibratePlay(EGsAlertType alertType = EGsAlertType::VibrateAlert);

private:
	void OnHpWarningEffectHUD();
	void StopHpWarningEffectHUD();

public:
	bool GetIsPvpHUDEffectOn() { return _isHUDEffectOn; }
};

#define GServerOption() (nullptr == GOption()? nullptr : GOption()->GetServerOption())
