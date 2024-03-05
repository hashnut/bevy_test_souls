// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"

#include "GameObject/Stat/GsStatDefine.h"

#include "UI/UILib/Interface/GsInvalidateUIInterface.h"

#include "Message/GsMessageUI.h"

#include "GsStatDistributionListEntry.generated.h"

/**
 * 
 */
 
 UCLASS()
class T1PROJECT_API UGsStatDistributionListEntryData : public UObject
 {
	 GENERATED_BODY()

 public:
	 StatType _statType;
	 TFunction<void(void)> _invalidEvent;
 };

UCLASS()
class T1PROJECT_API UGsStatDistributionListEntry : public UUserWidget, public IUserObjectListEntry, public IGsInvalidateUIInterface
{
	GENERATED_BODY()

	//plus stat
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* _bonusStatNameText;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* _bonusStatPoint;

	//stat button
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class UGsAccelationButton* _decreaseButton;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class UGsAccelationButton* _increaseButton;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TSubclassOf<class UUserWidget> _statSlotTemplate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class UVerticalBox* _statVerticalBox;

private:
	UPROPERTY()
	TArray<class UGsStatListViewEntry*> _statInfoSlotList;

private:
	StatType _statType;
	class FGsPlayerStatDistributionController* _statDistributionHandler;	
	TFunction<void(void)> _invalidEvent;
	MsgUIHandleArray _uiMessageHandlerArray;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeOnListItemObjectSet(class UObject* inListItemObject) override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	void InitStatName();
	void InitSecondaryStatNames();
	void BindMessage();
	void UnbindMessage();

public:
	void OnIncreaseStat();
	void OnLongTouchIncreaseStat(float inDelta);	
	void OnDecreaseStat();
	void OnLongTouchDecreaseStat(float inDelta);

private:
	void IncreaseStat(int32 inStatPoint = 1);
	void DecreaseStat(int32 inStatPoint = 1);
	void OnInvalidWidget(const struct IGsMessageParam* inParam);

public:	
	void InvalidWidget();

protected:
	virtual void InvalidateAllInternal() override;

private:
	void InvalidateParents();
	void InvalidateButtons();	
	void InvalidateStatValue();	
	void InvalidateStatValueColor();
	void InvalidateSecondaryStatValue();
	void InvalidateSecondaryStatValueColor();
};
