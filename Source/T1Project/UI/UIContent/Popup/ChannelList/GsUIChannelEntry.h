// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "GsUIChannelEntry.generated.h"

/**
 * 버튼이 하나 있는 팝업.
 */
UCLASS()
class T1PROJECT_API UGsUIChannelEntry : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnSlot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UPanelWidget* _panelMyChannelRoot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textBlockState;

	// 바인딩 변수
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textChannel;

protected:
	FText _textChannelFormat;
	const struct FGsMapChannelData* _data;	
	bool _bIsCurrentChannel = false;

protected:
	virtual void NativeOnInitialized() override;

public:
	virtual void OnShow() override;

public:
	void SetData(const struct FGsMapChannelData& InData, bool bInIsCurrentChannel);

protected:
	UFUNCTION()
	void OnClickSlot();

protected:
	void SetUICongestion(ChannelCongestionStatus InStatus);	
	void SendRequestSelectChannel();
	bool CheckCanChangeChannel();
	bool IsPeaceState() const;
};
