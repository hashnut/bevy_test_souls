// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIHUD.h"
#include "../GameFlow/GameContents/ContentsGame/Hud/BattleArena/GsBattleArenaData.h"
#include "GsUIHUDBattleArenaScoreHud.generated.h"

UCLASS()
class T1PROJECT_API UGsUIHUDBattleArenaScoreHud : public UGsUIHUD
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textRemainingTime;
	
	/**
	 * Red
	 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgRedGuildEmblem;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textRedGuildName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textRedScore;

	/**
	 * Blue
	 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgBlueGuildEmblem;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textBlueGuildName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textBlueScore;

	/**
	 * Timer
	 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class USizeBox* _panelBoxTimer;	
	
	/**
	 * Coin
	 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UPanelWidget* _sizeBoxArenaCoin;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgCoin;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textCoinCount;

	/**
	 * Detail
	 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UButton* _btnDetailScore;

protected:
	FTimerHandle _timerHandle;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	UFUNCTION()
	void OnClicked();

	void LoadTeamInfo(const class FGsBattleArenaGuildWarHandler* InHandler);
	void LoadCoinInfo(const class FGsBattleArenaGuildWarHandler* InHandler);

	void SetScoreInfo(int32 InRedTeamScore, int32 InBlueTeamScore);
	void SetCoinCount(ItemAmount InCount);

protected:
	void OnTimer();
	void ClearTimer();

public:
	void SetData(MapId InMapId, EBattleArenaGuildWarState InState, uint64 InEndTime, int32 InRedTeamScore, int32 InBlueTeamScore);
};
