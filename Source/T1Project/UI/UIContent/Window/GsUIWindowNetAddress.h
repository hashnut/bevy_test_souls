// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "ScrollBox.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "GsUIWindowNetAddress.generated.h"

/**
 *
 */
UCLASS()
class T1PROJECT_API UGsUIWindowNetAddress : public UGsUIWindow
{
	GENERATED_BODY()
	public:
	DECLARE_DELEGATE_OneParam(FOnClickSlot, int);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _entryWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWrapBox* _panelServerlist;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnConnect;

	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _panelSlotHelper;

	UPROPERTY()
	int8 _serverIndex;

public:
	FGsToggleGroup _toggleGroup;
	TArray<FString> _listServer;
	FOnClickSlot OnClickServer;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual bool OnBack() override;

public:
	// UObject override
	virtual void BeginDestroy() override;

protected:
	UFUNCTION()
	void OnRefreshEntry(int32 InIndex, class UWidget* InEntry);

	UFUNCTION()
	void OnClickSlot(int32 InIndex);

	UFUNCTION()
	void OnClickConnect();

public:
	UFUNCTION()
	void SetData(/*const FText& InWorldName*/);
};
