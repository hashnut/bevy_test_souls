// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "GsUIGuildAuctionChatMacro.generated.h"


class UPanelWidget;
class UGsUIChatMacroEntry;
/**
 * 기사단 경매 - 채팅 매크로
 */
UCLASS()
class T1PROJECT_API UGsUIGuildAuctionChatMacro : public UUserWidget
{
	GENERATED_BODY()

protected:
	DECLARE_DELEGATE_OneParam(FOnClickedMacroEntry, const FText&);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelMacro;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIChatMacroEntry* _macro1;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIChatMacroEntry* _macro2;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIChatMacroEntry* _macro3;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIChatMacroEntry* _macro4;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIChatMacroEntry* _macro5;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIChatMacroEntry* _macro6;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIChatMacroEntry* _macro7;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIChatMacroEntry* _macro8;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIChatMacroEntry* _macro9;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIChatMacroEntry* _macro10;

public:
	FOnClickedMacroEntry OnClickedMacroEntry;

protected:
	TArray<UGsUIChatMacroEntry*> _macroEntryList;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

public:
	void Show();
	void Hide();
};