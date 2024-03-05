// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG/Public/Blueprint/IUserObjectListEntry.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUICodexMapGroupListEntry.generated.h"

class UImage;
class UTextBlock;
class UProgressBar;
class UPaperSprite;
class UTexture2D;
class UWidgetSwitcher;

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUICodexMapGroupListItem : public UObject
{
	GENERATED_BODY()

private:
	CodexMapGroupId _mapGroupId;
	bool _isMaxLevel;
	FText _level;
	FText _mapGroupName;
	
	UPROPERTY()
	UPaperSprite* _mapGroupSprite;

	UPROPERTY()
	UTexture2D* _mapGroupTexture;
	
	float _progressBarPercent;

	// 선택되었을 때, 추가 이미지 하이라이트를 띄워주기 위함
	bool _isSelected = false;

private:
	void SetDataInternal(CodexMapGroupId InId, bool InIsMaxLevel, FText&& InLevel, FText&& InMapGroupName, UTexture2D* InMapGroupTexture, float InProgressBarPercent);

public:
	void SetData(CodexMapGroupId InId, bool InIsMaxLevel, FText&& InLevel, FText&& InMapGroupName, const FSoftObjectPath& InMapGroupImagePath, float InProgressBarPercent);

	void SetIsSelected(bool InIsSelected);

public:
	CodexMapGroupId GetCodexMapGroupId() { return _mapGroupId; }
	bool GetIsMaxLevel() { return _isMaxLevel; }
	FText GetLevel() { return _level; }
	FText GetMapGroupName() { return _mapGroupName; }
	UPaperSprite* GetPaperSprite() { return _mapGroupSprite; }
	UTexture2D* GetTexture2D() { return _mapGroupTexture; }
	float GetProgressBarPercent() { return _progressBarPercent; }

	bool GetIsSelected() { return _isSelected; }
};

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUICodexMapGroupListEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta=(BindWidget))
	UImage* _imageMaxLevel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta=(BindWidget))
	UTextBlock* _textLevel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta=(BindWidget))
	UTextBlock* _textMapGroupName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta=(BindWidget))
	UImage* _imageMapGroup;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta=(BindWidget))
	UProgressBar* _progressBarlevel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta=(BindWidget))
	UProgressBar* _progressBarlevelVfx;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta=(BindWidget))
	UWidgetSwitcher* _widgetSwitcherSelected;


	/************************************************************************/
	/* Overrides                                                            */
	/************************************************************************/
protected:
	void NativeOnInitialized() final;

	/** 실제 엔트리의 데이터 세팅 */
	void NativeOnListItemObjectSet(UObject* InListItem) final;

public:
	void SetSelectImage(bool IsOn);
};
