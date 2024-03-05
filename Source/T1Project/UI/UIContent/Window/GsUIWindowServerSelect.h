// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "ScrollBox.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Management/ScopeLobby/GsLobbyDataManager.h"
#include "GsUIWindowServerSelect.generated.h"

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIWindowServerSelect : public UGsUIWindow
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnClickSlot, const WorldId);
	
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
	TArray<ServerElem> _listServer;
	FOnClickSlot OnClickServer;

protected:
	virtual void NativeOnInitialized() override;

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
