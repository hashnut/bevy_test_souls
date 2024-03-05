// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIHUDFrame.h"
#include "UI/UILib/Define/GsUIDefine.h"
#include "Message/GsMessageStage.h"
#include "Message/GsMessageUI.h"
#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageInput.h"
#include "Option/GsOptionEnumType.h"
#include "Runtime/DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"
#include "Message/GsMessageInvasion.h"
#include "GsUIHUDFrameMain.generated.h"

/**
 * 메인 HUD UI 클래스
 */
class UGsUIHUDPropTouchProgress;
class UGsUIHUDPenalty;
class UGsUIHUDSpiritShot;
class UGsUIHUDBless;
class UGsUIHUDDungeonTime;
class UGsUIHUDScan;
class UGsUIHUDScanButton;
class UGsUIHUDOpenPopupChat;
class UGsUIHUDContribution;
class UGsUIHUDSaveBattery;
class UGsUIHUDPartyTargetButton;
class UGsUIHUDArenaTime;
class UGsUIHUDCounterAttackButton;
class UGsUIHUDSkillTargetSelect;
class UGsUIHUDPreset;
class UGsUIPresetSelect;
class UGsUIHUDPartyInfoButton;
class UGsUIHUDGameRating;

UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIHUDFrameMain : public UGsUIHUDFrame
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsUIHUDMainMenu* _mainMenu;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIHUDNavigationBar* _navigationBar;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsUIHUDSkill* _skillUI;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsUIHUDPlayerInfo* _playerInfoUI;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsUIHUDTargetInfo* _targetInfoUI;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsUIHUDCamera* _cameraUI;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsUIHUDQuest* _questUI;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsUIHUDParty* _partyUI;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsUIHUDScan* _scanUI;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsUIHUDChat* _chat;

	// 하단 소모품류 아이템 class
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsUIHUDConsumable* _consumableMenu;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsUIHUDSpiritShot* _spiritShot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsUIHUDMinimap* _minimap;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsUIHUDSystemInfo* _systemInfo;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsUIHUDAuto* _autoUI;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UGsUIHUDScanButton* _scanButtonUI;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsUIHUDAbnormality* _abnormalityUI;
	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//	class UGsUIHUDModeButton* _hudModeButton;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsUIHUDPickUp* _pickUpButton;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsUIHUDChaotic* _chaoticButton;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsUIHUDQuickSlot* _quickSlot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsUIHUDQuestDistance* _questDistance;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsUIHUDPropTouchProgress* _propTouchProgress;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsUIHUDSideTab* _sideTab;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsUIHUDPenalty* _penaltyHud;
	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//	class UGsUIHUDBless* _blessHud;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsUIHUDDungeonTime* _dungeonTimeHud;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsUIHUDGainEffect* _gainExpEffect;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsUIHUDGainEffect* _gainGoldEffect;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsUIHUDGainEffect* _gainMonsterKnowledgeEffect;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsUIHUDOpenPopupChat* _chatButton;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsUIHUDSocialEmojiButton* _emojiButton;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsUIHUDSaveBattery* _hudSaveBattery;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsUIHUDEventActionMessageProgressBar* _hudEventActionMessageProgressBar;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsUIHUDEventActionMessageNotify* _hudEventActionMessageNotify;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsUIHUDArenaTime* _hudArenaTime;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsUIHUDPartyTargetButton* _partyTargetButton;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIHUDCounterAttackButton* _counterAttackButton;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIHUDSkillTargetSelect* _skillTargetSelect;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIHUDPreset* _presetButton;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIPresetSelect* _presetSelect;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIHUDPartyInfoButton* _partyInfoButtonUI;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIHUDEventActionMessageProgressBarEX* _hudEventActionMessageProgressBarEX;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIHUDGameRating* _gameRatingUI;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _textBlockQA;

private:
	MsgStageHandleArray _msgHandleStageList;
	MsgUIHandleArray _msgHandleUIList;
	MsgInputHandle _inputParamDelegate;
	TArray<MsgInvasionHandle> _messageInvasionList;
	MsgGameObjHandleArray _gameObjectHandlerList;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	// UGsUIWidgetBase override
	virtual bool IsEnableManagerTick() const override { return true; }
	virtual void OnManagerTick(float InDeltaTime) override;

	virtual void SetHide(EGsUIHideFlags InHideFlags) override;	

	// Back키가 눌렸을 때 메뉴를 닫음
	virtual bool OnBack() override;

public:
	virtual void SetHUDMode(EGsUIHUDMode InMode) override;

protected:
	void BindMessages();
	void UnbindMessages();

public:
	void InvalidateAllMenu();

	void InvalidSafetyZone();
	void CheckVisibleNavigationBar();

protected:
	UFUNCTION()
		void OnClickSideTab(int32 InButtonIndex, bool bInIsSelected);

public:
	class UGsUIHUDMainMenu* GetMainMenu() const { return _mainMenu; }
	class UGsUIHUDPlayerInfo* GetPlayerInfo() const { return _playerInfoUI; }
	class UGsUIHUDTargetInfo* GetTargetInfo() const { return _targetInfoUI; }
	class UGsUIHUDCamera* GetCamera() const { return _cameraUI; }
	class UGsUIHUDSkill* GetSkill() const { return _skillUI; }
	class UGsUIHUDQuest* GetQuest() const { return _questUI; }
	class UGsUIHUDConsumable* GetConsumableMenu() const { return _consumableMenu; }
	class UGsUIHUDSpiritShot* GetSpiritShot() const { return _spiritShot; }
	class UGsUIHUDSystemInfo* GetSystemInfo() const { return _systemInfo; }
	class UGsUIHUDMinimap* GetMiniMap() const { return _minimap; }
	class UGsUIHUDAuto* GetAuto() const { return _autoUI; }
	class UGsUIHUDSideTab* GetSideTab() const { return _sideTab; }
	class UGsUIHUDChat* GetHudChat() const { return _chat; }
	class UGsUIHUDPropTouchProgress* GetPropInteractionProgress() const { return _propTouchProgress; }
	//class UGsUIHUDBless* GetBlessHud() const { return _blessHud; }
	class UGsUIHUDDungeonTime* GetDungeonTimeHud() const { return _dungeonTimeHud; }	
	class UGsUIHUDGainEffect* GetGainExpEffect() const { return _gainExpEffect; }
	class UGsUIHUDGainEffect* GetGainGoldEffect() const { return _gainGoldEffect; }
	class UGsUIHUDGainEffect* GetGainMonsterKnowledgeEffect() const { return _gainMonsterKnowledgeEffect; }
	class UGsUIHUDScan* GetScan() const { return _scanUI; }
	class UGsUIHUDScanButton* GetScanButton() const { return _scanButtonUI; }
	class UGsUIHUDQuestDistance* GetDistanceHud() const { return _questDistance; }
	class UGsUIHUDEventActionMessageProgressBar* GetEventActionMessageProgressBar() const { return _hudEventActionMessageProgressBar; }
	class UGsUIHUDEventActionMessageNotify* GetEventActionMessageNotify() const { return _hudEventActionMessageNotify; }
	class UGsUIHUDPartyTargetButton* GetPartyTargetButton() const { return _partyTargetButton; }
	class UGsUIHUDArenaTime* GetArenaTime() const { return _hudArenaTime; }
	class UGsUIHUDQuickSlot* GetQuickSlot() const { return _quickSlot; }
	UGsUIHUDCounterAttackButton* GetCounterAttackButton() const { return _counterAttackButton; }
	UGsUIHUDSkillTargetSelect* GetSkillTargetSelect() const { return _skillTargetSelect; }
	UGsUIHUDPreset* GetPreset() const { return _presetButton; }
	class UGsUIHUDParty* GetParty() const { return _partyUI; }
	class UGsUIHUDEventActionMessageProgressBarEX* GetEventActionMessageProgressBarEX() const { return _hudEventActionMessageProgressBarEX; }

public:
	void OnChangeSafetyMode(const struct IGsMessageParam* InParam);
	void OnChangePenalty(const  struct IGsMessageParam* InParam);

	void OnPreventGameOverIndulgence(const IGsMessageParam* InParam);
	//void OnWindowKeyMapping(const struct IGsMessageParam* InParam);
	void OnKeyboardActionOptionKey(EGsKeyMappingType InKeyMappingType, EInputEvent InEventType);
	void OnKeyboardSystemActionKey(EGsKeyMappingType InKeyMappingType, EInputEvent InEventType);
};
