// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineBaseTypes.h"
#include "KeyMapping/GsKeyMappingEnum.h"
#include "../Classes/GsPoolUObject.h"
#include "../UI/UILib/Base/GsUITray.h"
#include "../UI/UILib/Define/GsUIDefine.h"
#include "Message/GsMessageContents.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedGuildEnum.h"
#include "../GameFlow/GameContents/ContentsGame/Hud/BattleArena/GsBattleArenaData.h"
#include "GsUITrayBattleArenaPlayResult.generated.h"

/**
 *
 */
UCLASS()
class T1PROJECT_API UGsUITrayBattleArenaPlayResult : public UGsUITray
{
	GENERATED_BODY()

public:
	enum class EResult { EWin, ELose, EDraw };

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UWidgetSwitcher* _switcherResult;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UImage* _imgGuildEmblem;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _textGuildName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _textNoticeMinimumMember;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UListView* _listViewReward;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _textNoticeRewardCondition;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsButton* _btnConfirm;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsButton* _btnDetailResult;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _textRemainingTime;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsUIRedDotDefault* _redDotDetailScore;

protected:
	FTimerHandle _timerHandle;

protected:
	UPROPERTY()
		UGsPoolUObject* _listViewEntrtyPool;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	UFUNCTION()
		void OnClickedDetailScore();

	UFUNCTION()
		void OnClickedOK();

protected:
	void TryExitDungeon();

	void SetGuildInfo(EResult InResult);
	void SetRewardInfo(const TSet<const struct FGsSchemaRewardItemBagData*>& InData);

protected:
	void OnTimer();
	void ClearTimer();

	/**
	 * https://jira.com2us.com/jira/browse/C2URWQ-7923
	 * 2024/1/4 PKT - 위의 이슈로 기존 Handler에서 SetData를 해줬던 것을 Window에서 해주는 것으로 수정
	 */
	void SetData(EResult InResult, const FGSBattleArenaGuilWarResultData& InResultData);

public:
	// 2024/1/9 PKT - Custom ZOder(layer)
	virtual int32 GetManagedDefaultZOrder() const override { return UI_DEPTH_POPUP - 1; }
};