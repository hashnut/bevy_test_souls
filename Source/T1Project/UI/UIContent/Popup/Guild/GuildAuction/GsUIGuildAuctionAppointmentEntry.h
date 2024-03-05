// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"

#include "GsUIGuildAuctionAppointmentEntry.generated.h"

class UGsButton;
class UTextBlock;
class UImage;
/**
 * 기사단 지정 경매 팝업(GsUIPopupGuildAuctionUser)의 유저 정보
 */
UCLASS()
class T1PROJECT_API UGsUIGuildAuctionAppointmentEntry : public UUserWidget, 
	public IGsToggleGroupEntry,
	public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	DECLARE_DELEGATE_TwoParams(FOnClickUserName, UserDBId, bool);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockUserName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imageSelected;

public:
	FOnClickUserName OnClickUserName;

protected:
	UserDBId _userDBId;
	bool _isOn = false;

public:
	virtual void SetIsSelected(bool bInIsSelected) override;
	virtual bool GetIsSelected() const override;

protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnClickName();

public:
	void SetData(UserDBId InUserDBId, const FText& InUserName);
};