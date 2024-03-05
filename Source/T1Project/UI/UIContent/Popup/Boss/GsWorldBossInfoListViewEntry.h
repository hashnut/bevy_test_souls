// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "Message/GsMessageBoss.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Message/GsMessageUI.h"
#include "DataSchema/WorldBoss/GsSchemaWorldBossData.h"
#include "UTIL/GsTimeUtil.h"
#include "GameObject/Boss/GsWorldBossHandler.h"
#include "Tutorial/GsTutorialHandler.h"
#include "GsWorldBossInfoListViewEntry.generated.h"

/**
 *
 */

UCLASS()
class T1PROJECT_API UGsWorldBossInfoListViewEntryData : public UObject
{
	GENERATED_BODY()

public:	
	const FGsSchemaWorldBossData* _bossData = nullptr;	
	int32 _level;
};

UCLASS()
class T1PROJECT_API UGsWorldBossInfoListViewEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsButton* _button;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UImage* _bossIcon;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _areaNameTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _bossNameTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _levelTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UImage* _selectImage;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UWidget* _lockPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class URichTextBlock* _lockDescriptionRichTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UWidget* _activeEffect;

private:	
	const struct FGsSchemaWorldBossData* _bossData;	
	TArray<TPair<MessageBoss, FDelegateHandle>> _bossDelegateArray;
	TArray<TPair<MessageUI, FDelegateHandle>> _uiDelegateArray;	
	TArray<FGsTutorialHandler::MsgHandleUnlock> _msgUnlockHandleList;

private:
	FText _levelLimitText;
	FText _levelFormat;
	int32 _level;

private:
	FGsDateTimer _enterTimer;
	FGsWorldBossInfo _bossInfo;

public:
	virtual void NativeOnInitialized() override;		
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeOnListItemObjectSet(class UObject* inListItemObject) override;

public:
	UFUNCTION()
	void OnClickListViewEntry();

private:
	void InvalidBossInfo();	
	void InvalidBookmark();
	void InvalidLock();
	void CheckActiveTimer();
	void InvalidActiveEffect();
	void OnSelectBossInfo(const struct IGsMessageParam* inParam);
	void OnBossRoomReady(const struct IGsMessageParam* inParam);
	void OnInvalidUnlock(const struct IGsMessageParam* inParam);
	void InvalidSelect(const struct FGsSchemaWorldBossData* inData);

public:
	const struct FGsSchemaWorldBossData* GetBossData() const;
	const struct FGsSchemaNpcData* GetNpcData() const;

};
