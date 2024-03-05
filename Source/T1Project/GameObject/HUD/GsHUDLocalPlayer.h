// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DataSchema/GsSchemaEnums.h"

#include "UI/GsAreaNameHUD.h"
#include "UI/UIContent/HUD/GsUIHUDSideTab.h"

#include "Message/GsMessageContentHud.h"
#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageUI.h"
#include "Message/GsMessageStage.h"
#include "Message/GsMessageUserInfo.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageItem.h"

#include "EventProgress/GsEventProgressInterface.h"
#include "EventProgress/GsEventProgressDefine.h"

#include "Option/GsOptionEnumType.h"

#include "GameFlow/GameContents/GsContentsMode.h"

#include "GsHUDLocalPlayer.generated.h"

class FGsStatInfo;
class FGsTargetHandlerLocalPlayer;
class FGsInputEventMsgBase;
class AGsAreaNameHUD;

struct FGsNetUserData;
struct IGsMessageParam;
struct TargetHUD;

/**
 * HUD 관련 항목들을 처리한다. UI코드에 직접 접근하지 않게 하는 역할.
 */
UCLASS()
class T1PROJECT_API AGsHUDLocalPlayer : public AHUD
	, public IGsEventProgressInterface
{
	GENERATED_BODY()

public:
	using UserInfoHandler = TPair<MessageUserInfo, FDelegateHandle>;
	using MonsterKnowledgeHandler = TPair<MessageContentMonsterKnowledge, FDelegateHandle>;
	
protected:
	TWeakObjectPtr<class UGsUIManager> _uiManager;
	TWeakObjectPtr<class UGsUIHUDFrameMain> _hudWidget;
	TWeakObjectPtr<class UGsGameObjectLocalPlayer> _localPlayer;

	MsgHudHandleArray _hudHandlerList;
	TArray<UserInfoHandler> _userInfoHandlerList;
	MsgGameObjHandleArray _gameObjectHandlerList;
	MsgUIHandleArray _msgUIHandlerList;
	MonsterKnowledgeHandler _monsterKnowledeHandler;
	TArray<MsgStageHandle> _msgStageHandleList;

	UPROPERTY(DuplicateTransient)
	class UInputComponent* _SystemInputComponent;

	UPROPERTY(DuplicateTransient)
	class UInputComponent* _HudInputComponent;
	

private:
	FDelegateHandle _changeStatHandle;
	FDelegateHandle _handleTargetDisplayData;
	TPair<MessageItem, FDelegateHandle>	_itemDelegate;

	// for ui tray Area name
	// call by event progress manager
	FText _backupTextAreaName;
	MsgUIHandle _msgUIHandle;

	FText _invasionTownFormatText;

	bool _isInputEnable{ false };

private:
	int32 _preLevel;
	int32 _currentLevel;

public:
	virtual void BeginPlay() override;
	virtual void PostRender() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void EndPlay(const EEndPlayReason::Type in_type) override;

	virtual void DrawHUD() override;

	// IGsEventProgressInterface
public:	
	virtual void StartEvent(EGsEventProgressType In_startType, EGsEventProgressType In_preType) override;
	virtual void FinishEvent(EGsEventProgressType In_finishType, EGsEventProgressType In_nextType) override;

private:
	void Reset();
	void InitMessageHandler();
	void RemoveMessageHandler();
	void BindingEvent();

public:
	void SetPlayer(class UGsGameObjectLocalPlayer* InPlayer);

private:
	// 메세지 핸들 함수들

	// Invalidate
	void InvalidateAll();
	void InvalidateComsumable();
	void InvalidatePlayerInfo();
	void InvalidateQuest();	
	void InvalidateSkill();
	void InvalidateCamera();
	void InvalidateSystemInfo();
	void InvalidateChat(); //채팅 Hud 갱신
	void InvalidateNPCFunction();
	void InvalidateArena();
	void InvalidateCounterAttack();
	void InvalidateSkillTargetSelect();
	void InvalidatePreset();

	// Target
	void OnEnableTargetHUD(TargetHUD& InTargetInfo);
	void OnDisableTargetHUD(TargetHUD& InTargetInfo);

	// LocalPlayer
	void OnChangeLocalPlayerLevel(const IGsMessageParam*);
	void OnChangePlayerExp(uint64 InExp);
	void OnIncreasePlayerExp(uint64 InExp);
	void OnIncreaseGold(uint64 InAmount);
	void OnIncreaseMonsterBookPoint(uint64 InIncreasePoint);
	void OnIncreaseCodexMapGroupExp(const IGsMessageParam* InParam);
	void OnMaxCurrencyAmount(uint64 InCurrencyType);
#ifdef R3_ARCAS_NOT_USE
	void OnChangeBlessPoint(uint64 InBlessPoint);
#endif
	void OnLocalPlayerSpawnComplete(const IGsMessageParam*);
	void OnLocalPlayerRevive(const IGsMessageParam*);	
	// 스턴
	void OnLocalReaction(struct FGsItemMessageParamBase& InParam);

	// UI
	void OnChangeSafetyMode(const IGsMessageParam* InAreaInfo);		
	void OnShowAreaName( AGsAreaNameHUD& InAreaInfo);
	// 지역명 나감
	void OnAreaNameLeave( AGsAreaNameHUD& InAreaInfo);
	// 0: 부활창 막음, 1: 부활창 막지 않음
	//void OnEnableReviveUI(int32 InParam);
	// 0: auto ui off, 1: auto ui on
	void OnAutoUIChange(int32 In_param);
	void OnChangeHUDMode(const IGsMessageParam* InParam);
	// 파티 HUD sidetab 갱신
	void OnPartyHUDSideTabUpdate(const IGsMessageParam*);

	void OnPartyMemberUpdate(const IGsMessageParam* InParam);

private:
	// 바인딩
	UFUNCTION()
	void OnChangeCamera(EGsCameraMode InMode);
	void OnSetLocalPlayerStat(const FGsStatInfo& InStatInfo);	
	void OnChangeTargetStat(const FGsStatInfo& InStatInfo);
	
private:
	void OpenDisplayAreaUI(const FString& InTopName);

private:
	void OnShowRevive(const IGsMessageParam*);

	//minimap
private:
	// 트리거 들어와서 지역명 출력
	void SetMiniMapAreaName(const FString& InTopName);
	void SetMiniMapDebugUI(bool inIsActive);

private:
	// ai 액티브
	void OnActiveAI(const IGsMessageParam* In_isActive);
	// Auto Quest 액티브
	void OnActiveAutoQuest(const IGsMessageParam* In_isActive);
	// 로컬이가 맞음
	void OnHitLocal(const IGsMessageParam* inParam);
	// 디버그용 예약 리스트 show
	void OnDebugAIReserveListShow(const IGsMessageParam* In_isActive);

private:
	void InvalidTownUI(const struct FGsSchemaMapTerritoryInfo* inTerritory);
	void InvalidChaosUI(const struct FGsSchemaMapTerritoryInfo* inTerritory);

public:
	// 현재맵 기준으로 티커 출력
	void OpenDisplayAreaUICurrentMap();
	// 트리거 나가서 기본 맵이름 출력
	void SetMinimapDefaultMapName();
	// 현재 맵 기준으로 출력
	void ShowCurrentMapInfo();
	// side tab 액티브
	void SetActiveSideTab(UGsUIHUDSideTab::EGsHUDSideTabType In_type);

	void UpdateTargetShowHPBar();

public:
	void SetHudInput();
	void EnableHudInput();
	void DisableHudInput();
	void OnChangeInputProcessEnable(const IGsMessageParam* InParam);
};
