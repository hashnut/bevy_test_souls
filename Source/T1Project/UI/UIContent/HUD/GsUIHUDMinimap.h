// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageUI.h"
#include "Message/GsMessageContentHud.h"
#include "Message/GsMessageInvasion.h"
#include "Message/GsMessageBattleArena.h"
#include "UI/UILib/Base/GsUIHUD.h"

#include "Map/Minimap/GsMinimapDefine.h"
#include "GsUIHUDMinimap.generated.h"

/**
 * 상단 미니맵 정보
 */

class UGsMinimapIconBase;
class UGsUIIconBase;
class UGsButton;
class UGsUINPCList;
class UGsHorizontalBoxIconSelector;
class UGsUITooltipTitleDesc;
class UGsUISubNPCList;
class UGsUIHUDFieldBoss;
class UGsUIIconAbnormality;

class UCanvasPanelSlot;
class UCanvasPanel;
class UWidgetSwitcher;
class UTextBlock;
class UImage;

struct IGsMessageParam;
struct FGsSchemaPassivitySet;




UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIHUDMinimap : public UGsUIHUD
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textAreaName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textSafeZone;

	// 이미지 사이즈용 캔버스
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _sizeCanvas;
	// 씬 이미지 로드 해서 붙일곳
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _sceneImageRoot;
	// 아이콘 위젯 로드해서 붙일곳
	// 위젯 계층 구조상 _sceneImageRoot 밑에 있어서
	// _sceneImageRoot를 이동 시키면 icon 들도 따라 움직임
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _iconRoot;
	UPROPERTY()
	UCanvasPanelSlot* _testPanelSlot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnBookmark;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnChannel;
	// 월드맵 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnWorldMap;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherMinimap;

	// 환경 버프/디버프 표시
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsHorizontalBoxIconSelector* _iconSelectorEnv;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsHorizontalBoxIconSelector* _iconSelectorEnv2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI")
	TArray<UGsHorizontalBoxIconSelector*> _arrayIconSelectorEnv;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUITooltipTitleDesc* _tooltipEnv;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUITooltipTitleDesc* _tooltipEnv2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI")
	TArray<UGsUITooltipTitleDesc*> _arrayTooltipEnv;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUINPCList* _npcList;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISubNPCList* _questNpcList;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textCurrentChannel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherSpaceCrack;

	// To be deleted
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _canvasSpaceCrack;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSpaceCrackActive;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSpaceCrackDeactive;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _spaceCrackRedDot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnContribution;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherSideButton;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textInvadeInfo;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIHUDBoss* _bossHud;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIHUDBattleArenaRegister* _battleArenaRegisterHUD;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIHUDBattleArenaEnter* _battleArenaEnterHUD;	

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIHUDBattleArenaScoreHud* _arenaBattleScoreBoardHud;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnInvasion;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnPreInvasion;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnInterPortal;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UWidgetSwitcher* _bgWidgetSwitcher;	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UWidget* _invasionPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _invasionServerTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _userCountTextBlock;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switchInvasionNoti;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI", meta = (BindWidget))
	FText _textInvasionTime;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UPanelWidget* _panelInvasionNoti;
	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//class UTextBlock* _textPreInvasionTime;
	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//class UTextBlock* _textInvasionTime;


protected:
	
	// 채널 리스트 요청을 보낸 상태인가?
	bool _bIsRequestChannelList = false;
	
	// 퀘스트 엔피시 리스트 INDEX
	static const int8 QUEST_NPC_LIST_WIDGET_INDEX = 2;

	// 아이콘 붙이기 메시지 델리게이트
	MsgMinimapHandle _delegateAttachIcon;
	MsgMinimapHandle _delegateAttachIconWidget;
	MsgMinimapHandle _delegateIconPos;
	MsgGameObjHandleArray _gameObjectMsgArray;
	MsgUIHandleArray _uiMsgArray;
	MsgHudHandle _delegateQuestNpcList;

	float _endInvadeTime{ 0 }; //삭제 예정

	bool _isCheckMap{ false };
	bool _isCheckRemainTime{ false };
	//UTextBlock* _useTextBlock{ nullptr };
	FDateTime _InvasionEnableTime;

private:
	int32 _maxUserCount;
	MapId _currentMapId;
	MsgGameObjHandleArray _gameObjectMsg;
	MsgInvasionHandleArray _invasionMsg;
	MsgContentBattleArenaArray _battleArenaMsg;

protected:
	// UUserWidget override.
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void InvalidateAllInternal() override;

public:
	virtual bool UpdateManagerTick(float InDeltaTime) override;
	// UGsUIHUD override : HUD 모드 변경 시
	virtual void SetHUDMode(EGsUIHUDMode InMode) override;

	// 로직 함수
public:

	// debug list 액티브 변경
	void SetActiveDebugList(bool In_isActive);

	// 채널 표시
	void SetChannel(int In_channel);
	// 안전지역 텍스트 표시
	void SetSafeZone(EGsMinimapAreaType In_isSafe);

	// 맵 환경 속성 아이콘 표시
	void SetEnvIcon();	
	void OpenCurrentWorldMap();

	void ClearAllMessage();
	void InvalidContribution(EGsUIHUDMode inMod);

	void ClickedWorldMap();
	void ClickedBookmark();

	// message 이벤트
public:
	// 아이콘 붙이기
	void OnAttachIcon(UGsMinimapIconBase* In_icon);
	void OnAttachIconWidget(UGsUIIconBase* In_iconWidget);
	void OnIconPos(const FVector2D& In_pos);
	
	// 채널 표시
	void OnSetChannel(const IGsMessageParam* In_param);

	void OnRecieveChannelList(const IGsMessageParam*);
	// 시공의틈새 킬수 있는지(버튼 활성화)
	void OnSpaceCrackButtonActive(const IGsMessageParam* In_param);
	//기여도 버튼 활성화
	void OnContributionButtonActive(const IGsMessageParam* In_param);
	// 퀘스트 변화에 따라 questNpcList 아이콘 상태 변경
	void InvalidateQuest();

	// 침공 요건에 따른 현재 맵 갱신
	void OnUpdateInvadeMapState(const IGsMessageParam* In_param);

	// 인터서버 포탈 스폰/디스폰
	void OnEliteDungeonActive(const IGsMessageParam* InParam);
	void OnSpawnInterPortal(const IGsMessageParam*);
	void OnDespawnInterPortal(const IGsMessageParam*);

	void OnInvalidBattleArena(const IGsMessageParam* InParam);
	/*void OnShowArenaEnterButton(const IGsMessageParam* InParam);
	void OnHideArenaEnterButton(const IGsMessageParam*);
	void OnShowArenaTeamInfo(const IGsMessageParam*);
	void OnHideArenaTeamInfo(const IGsMessageParam*);*/

	// ui event
protected:
	// 채널 클릭
	UFUNCTION()
	void OnClickChannel();
	// 기억장소 클릭
	UFUNCTION()
	void OnClickBookmark();
	// 월드맵 클릭
	UFUNCTION()
	void OnClickWorldMap();

	UFUNCTION()
	void OnPressIconEnv(UGsUIIconAbnormality* InIcon);
	UFUNCTION()
	void OnPressIconInvasionInvadeEnv(UGsUIIconAbnormality* InIcon);
	UFUNCTION()
	void OnPressIconInvasionDefenseEnv(UGsUIIconAbnormality* InIcon);
	// 시공의틈새 클릭
	UFUNCTION()
	void OnClickSpaceCrackActive();
	UFUNCTION()
	void OnClickSpaceCrackDeactive();
	UFUNCTION()
	void OnClickContribution();

	UFUNCTION()
	void OnClickGoToInvasionInfo();

	UFUNCTION()
	void OnClickInterPortal();

private:
	void InvalidUserCount();
	void InvalidInvasionServerName();	

private:
	void OnLocalPlayerSpawnComplete(const struct IGsMessageParam*);
	void OnInvalidUserCount(const struct IGsMessageParam*);

private:
	void SetSideSwitcher(EGsMinimapSideButtonMode InMode);

	// get, set
public:
	void SetAreaNameText(const FText& InAreaName);
	// 기본 맵 이름 세팅
	void SetDefaultMapName();

	void SetInvasionMapState(int state);
	void SetInvasionRemainTime(FTimespan remainTime);

	UGsUIIconAbnormality* GetEnvIconByIndex(int In_index);
	const FGsSchemaPassivitySet* GetEnvPassivitySetCurrentMap() const;
	bool GetCurrentMapEnablePK();
	const FGsSchemaPassivitySet* GetInvadeEnvPassivitySetCurrentMap(bool In_isInvade) const;
	UGsUITooltipTitleDesc* GetEnvTooltipByIndex(int In_index);
};
