// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "Management/ScopeLobby/GsLobbyDataManager.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "GsUIPopupServerSelect.generated.h"


UENUM()
// 채널 탭 타입
enum class ServerListType : uint8
{
	// 추천
	Server_Recommended = 0,

	// 전체
	Server_All = 1,

	// 캐릭터 보유 서버(내 캐릭터 서버)
	Server_My = 2,

	MAX
};


class FText;
class UTextBlock;
class UGsButton;
class UWrapBox;
class UGsDynamicPanelSlotHelper;
class UPanelWidget;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIPopupServerSelect : public UGsUIPopup
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnClickSlot, const uint16);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _entryWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWrapBox* _panelServerlist;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnClose;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UPanelWidget* _panelTabListRoot;

	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _panelSlotHelper;

protected:
	FGsToggleGroup _toggleGroupTab;
	int8 _serverIndex = 0;

public:
	FGsToggleGroup _toggleGroup;
	TArray<ServerElem> _listServer;
	FOnClickSlot OnClickServer;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	// UObject override
	virtual void BeginDestroy() override;

protected:
	UFUNCTION()
	void OnRefreshEntry(int32 InIndex, class UWidget* InEntry);
	UFUNCTION()
	void OnClickSlot(int32 InIndex);
	UFUNCTION()
	void OnClickClose();

public:
	void SetData(int32 InIndex);

protected:
	void OnSelectTab(int32 InIndex);
};
