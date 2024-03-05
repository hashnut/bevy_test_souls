// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "GsUIAgitRaidBossListEntry.generated.h"

class UImage;
class UTextBlock;
class UWidgetSwitcher;
class UPaperSprite;
class UGsButton;
class UGsToggleButton;
class FGsAgitDungeonRaidBossData;

/**
 * 기사단 영지던전 공략던전의 보스 리스트
 */
UCLASS()
class UGsUIAgitRaidBossListEntry : public UUserWidget,
	public IGsToggleGroupEntry,
	public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSlot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgSelected;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgBoss;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockBossLevel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockBossName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockMap;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockLevelLimit;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsToggleButton* _toggleBtnBookmark;

	// 0: 공략중 1: 공략완료
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherState;	

private:
	bool _bIsSelected = false;
	int32 _raidId = 0;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	// IGsToggleGroupEntry override
public:
	virtual void SetIsSelected(bool bInIsSelected) override;
	virtual bool GetIsSelected() const override { return false; }

public:
	void SetData(TWeakPtr<FGsAgitDungeonRaidBossData> InData);
	void SetIsBookmark(bool bIsBookMark);

	void SetBossImage(int32 InRaidId, UPaperSprite* InSprite);

protected:
	UFUNCTION()
	void OnClickSlot();
	UFUNCTION()
	void OnClickBookmark();
};
