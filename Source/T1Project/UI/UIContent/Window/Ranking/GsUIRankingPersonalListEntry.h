// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Window/Ranking/GsUIRankingListEntryBase.h"
#include "GsUIRankingPersonalListEntry.generated.h"

class UTextBlock;
class UImage;
class UWidgetSwitcher;
class UGsButton;
/**
 * 개인 랭킹 UI 유저 슬롯
 */
UCLASS()
class T1PROJECT_API UGsUIRankingPersonalListEntry : public UGsUIRankingListEntryBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _iconEmblem;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockGuildOrServer;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockValue;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockPercent;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherGuild;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherValue;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnUserName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UUserWidget* _spacerBadge;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UPanelWidget* _boxBadge;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherBadge;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _iconBadge;

protected:
	FString _userName;

	TFunction<void(UWidget*)> OnClickListItem;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

public:
	virtual void SetData(const FGsRankingData* InRankingData) override;
	virtual void SetData(FGsRankingMyData* InMyRanking) override;

protected:
	void SetLocalData(const FGsRankingData* InRankingData);
	void SetWorldData(const FGsRankingData* InRankingData);

	void ShowMainText(const FText& InText);
	void HideMainText();

	void SetLevel(Level InLevel, Exp InExp);
	void SetLevel(Level InLevel, float InPercentExp);

	void SetValue(const FText& InText);

protected:
	UFUNCTION()
	void OnClickUserName();
};