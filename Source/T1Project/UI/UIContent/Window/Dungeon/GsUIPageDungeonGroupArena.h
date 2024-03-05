#pragma once


#include "CoreMinimal.h"
#include "UI/UIContent/Window/Dungeon/GsUIPageDungeonGroupBase.h"
#include "../Classes/GsPoolUObject.h"
#include "GsUIPageDungeonGroupArena.generated.h"


UCLASS()
class T1PROJECT_API UGsUIPageDungeonGroupArena : public UGsUIPageDungeonGroupBase
{
	GENERATED_BODY()

protected:
	// 2023/11/18 PKT - 던전그룹 이름
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsHorizontalBoxIconSelector* _iconSelectorEnv;

	// 2023/11/18 PKT - 던전그룹 설명
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textDesc;

	// 2023/11/18 PKT - 던전 대표 이미지
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgDungeonGroup;

	// 2023/11/23 PKT - 시즌 기간
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textSeasonCount;
	
	// 2023/11/18 PKT - 시즌 기간
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textSeasonDate;

	// 2023/11/18 PKT - 플레이 할 수 있는 시간(정기적인 시간)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textPlayCycle;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textLevelLimit;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textGuildLevelLimit;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textRegisterLimitGuildGrade;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnRewardDetail;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UListView* _listViewRewardSet;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UButton* _btnBattleArenaRank;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UButton* _btnBattleArenaGuide;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnBattleArenaEntered;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textEnteredBtnName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidget* _panelEnterRemainingTime;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textRemainingTime;

protected:
	UPROPERTY()
	UGsPoolUObject* _listViewEntryPool;

	FDateTime _endTime;

protected:
	FTimerHandle _timerHandle;
	
public:
	virtual void BeginDestroy() override;
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	UFUNCTION()
	void OnClickedEnvironmentIcon(class UGsUIIconAbnormality* InIcon);

	UFUNCTION()
	void OnClickedApplyArena();

	UFUNCTION()
	void OnClickedRewardDetail();

	UFUNCTION()
	void OnClickedGuildWarGuide();

	UFUNCTION()
	void OnClickedSeasonRankOpen();

protected:
	void OnTimer();
	void ClearTimer();
	void InvalidateApplyButtonState();

public:
	virtual void Enter(const class FGsDungeonGroupBase* InData, const FGsDungeonUICaptureData& InCaptureData);
	virtual void Invalidate();
	virtual void Capture(FGsDungeonUICaptureData* OutCaptherData) const;
};