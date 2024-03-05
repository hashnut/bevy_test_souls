// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "GsUIGuildQuestPointListEntry.generated.h"

class UTextBlock;
class UProgressBar;
class UImage;
class UGsButton;
class UGsHorizontalBoxIconSelector;
class UGsUIIconItemReward;
class UGsUIIconItem;
struct FGsSchemaGuildZealPointReward;

/**
 * 기사단 열성 포인트 리스트 아이템 
 */
UCLASS()
class T1PROJECT_API UGsUIGuildQuestPointListEntry : public UUserWidget,
	public IGsEntryInterface
{
	GENERATED_BODY()

public:
	enum class EGsGuildQuestPointState
	{		
		REWARDED = 0, // 보상 받음
		LOCKED = 1, // 잠김
		ACTIVE = 2, // 활성
	};

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsHorizontalBoxIconSelector* _iconSelectorReward;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgLock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockAmount;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UProgressBar* _progressBar;

protected:
	EGsGuildQuestPointState _state = EGsGuildQuestPointState::REWARDED;
	int32 _zealPointLevel = 0;

public:
	void SetData(const FGsSchemaGuildZealPointReward* InData);
	void SetState(EGsGuildQuestPointState InState);
	void SetProgressBar(float InRate);

protected:
	void OnClickSlot(UGsUIIconItem& InIcon);
	void OnLongPressIcon(class UGsUIIconItem& InIcon);

protected:
	UGsUIIconItemReward* GetRewardIcon();
	int32 FindRewardOpenGuildLevel(int32 InZealPointLevel);
};
