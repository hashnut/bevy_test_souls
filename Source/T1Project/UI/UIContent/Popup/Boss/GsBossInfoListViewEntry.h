// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "Message/GsMessageBoss.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Message/GsMessageUI.h"
#include "GsBossInfoListViewEntry.generated.h"

/**
 *
 */

class UGsButton;
class UImage;
class UGsUIPopupBossInfo;
class UTextBlock;
struct FGsSchemaFieldBossData;
struct FGsSchemaNpcData;

UCLASS()
class T1PROJECT_API UGsBossInfoListViewEntryData : public UObject
{
	GENERATED_BODY()

public:	
	const FGsSchemaFieldBossData* _bossData = nullptr;
	const FGsSchemaFieldBossData* _mutantBossData = nullptr;		
	TWeakObjectPtr<class UGsUIWindowBossInfo> _parent;	
};

UCLASS()
class T1PROJECT_API UGsBossInfoListViewEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UGsButton* _button;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UImage* _bossIcon;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UTextBlock* _areaNameTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UTextBlock* _bossNameTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UImage* _selectImage;
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
	void OnSelectBossInfo(const IGsMessageParam* inParam);
	void InvalidSelect(const struct FGsSchemaFieldBossData* inData);
	UFUNCTION()
	void OnClickBookmark();

public:
	const FGsSchemaFieldBossData* GetBossData() const;
	const FGsSchemaNpcData* GetNpcData() const;

};
