// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "Message/GsMessageBoss.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Message/GsMessageUI.h"
#include "GsFieldBossInfoListViewEntry.generated.h"

/**
 *
 */

UCLASS()
class T1PROJECT_API UGsFieldBossInfoListViewEntryData : public UObject
{
	GENERATED_BODY()

public:	
	const struct FGsSchemaFieldBossData* _bossData = nullptr;
	const struct FGsSchemaFieldBossData* _mutantBossData = nullptr;
	TWeakObjectPtr<class UGsUIWindowBossInfo> _parent;	
	int32 _level;
};

UCLASS()
class T1PROJECT_API UGsFieldBossInfoListViewEntry : public UUserWidget, public IUserObjectListEntry
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
		class UGsToggleButton* _bookmarkToggleButton;

private:
	TWeakObjectPtr<class UGsUIWindowBossInfo> _parent;
	const FGsSchemaFieldBossData* _bossData;	
	const FGsSchemaFieldBossData* _mutantBossData;
	TArray<TPair<MessageBoss, FDelegateHandle>> _bossDelegateArray;
	TArray<TPair<MessageUI, FDelegateHandle>> _uiDelegateArray;
	bool _isBookmark;

private:
	FText _levelLimitText;
	FText _levelFormat;
	int32 _level;

public:
	virtual void NativeOnInitialized() override;		
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeOnListItemObjectSet(UObject* inListItemObject) override;

public:
	UFUNCTION()
	void OnClickListViewEntry();

private:
	void InvalidBossInfo();	
	void InvalidBookmark();
	void OnSelectBossInfo(const struct IGsMessageParam* inParam);
	void InvalidSelect(const struct FGsSchemaFieldBossData* inData);
	UFUNCTION()
	void OnClickBookmark();

public:
	const struct FGsSchemaFieldBossData* GetBossData() const;
	const struct FGsSchemaNpcData* GetNpcData() const;
	bool TryGetNewSelectBossId(int32 inBossId, bool inAdd, OUT int32& outBossId);	
	bool IsFullBookmark();
	bool IsEmptyBookmark();
	int32 GetFirstBossId();
	int32 GetLastBossId();

};
