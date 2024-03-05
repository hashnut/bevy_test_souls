// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Define/GsUIDefine.h"
#include "GsBlockUIController.generated.h"

class UGsUIWidgetBase;

// BlockUI
enum class EGsHideBlockUIFactor
{
	GAME_END = 0,
	LIFE_TIME_OUT = 1,
	POPUP_NET_ERROR = 2,
	SHOW_LOADING = 3,
	RESTART_GAME = 4,
	ON_BACK_TO_TITLE = 5,
	ON_EXIT_GAME = 6,

	ACK_SELECT_USER = 7,	
	
	SPAWN_ME_COMPLETE = 8,	
	CONTENTS_LOCKED = 9,
	WARP_FAIL = 10, // 각종 워프상황이 있어서 짝을 맞추기 어려움이 있어 이것으로 사용
};

/**
 * BlockUI 관리
 * - 한번 열고난 후에는 Visible로만 관리
 * - 처음 Launcher의 BlockUI 경로를 저장 후 생성하여 활용
 */
UCLASS()
class T1PROJECT_API UGsBlockUIController : public UObject
{
	GENERATED_BODY()	

private:
	// 무조건 2분 뒤에는 꺼줌
	const float BLOCK_UI_LIFE_SECOND = 120.f;
	float _lifeRemainSec = 0.f;
	float _settingLifeRemainSec = 0.f;

	// 로비 정보는 날리기 때문에 저장해둔다
	FName _lobbyUIKey;
	FSoftObjectPath _lobbyUIPath;

	// UIController의 RemoveAll 타이밍에 지워져서 강제로 지우는 처리는 필요가 없음
	TWeakObjectPtr<UGsUIWidgetBase> _blockUI;

	// 짝이 맞는지 추적을 위해 넣은 것. 안맞는 경우가 많아서 일단은 상관없이 여닫히도록 함
	int32 _openCount = 0;

	// 자동 닫힘을 하지 않고 명시적으로 Close 해야 하는 경우에만 사용. 이 리스트에 하나라도 존재하면 안닫힘
	TArray<FName> _openList; 

public:
	void Initialize();
	void Finalize();
	void Update(float InTick);

	void OnLauncherStart(const class UGsLobbyConstant* InData);

public:
	void SetLobbyUIInfo(const FName& InKey, const FSoftObjectPath& InPath);

	void Show();
	void Hide();
	void SetSetting(const FText& InMessage, bool bInHideUI = false, float InLifeSecond = 0);

	void ShowExplicit(const FName& InKey);
	void HideExplicit(const FName& InKey);
	// 강제로 전부 닫기
	void ForcedHide(EGsHideBlockUIFactor InFactor);

private:
	void ShowInter();
	void HideInter();
	//void RemoveInter();

public:
	bool IsOpened() const;

	void PrintLog();
};
