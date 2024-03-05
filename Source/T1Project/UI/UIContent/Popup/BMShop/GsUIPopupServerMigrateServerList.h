// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "UI/UIContent/Popup/BMShop/GsUIServerMigrateServerListSortComboBox.h"
#include "GameFlow/GameContents/ContentsGame/Hud/BMShop/ServerMigrate/GsServerMigrateHandler.h"
#include "GsUIPopupServerMigrateServerList.generated.h"

class UGsButton;
class UGsSwitcherButton;
class UGsDynamicPanelSlotHelper;

struct MigrateServer;
struct FGsSharedParam;
/**
 *
 */
UCLASS()
class T1PROJECT_API UGsUIPopupServerMigrateServerList : public UGsUIPopup
{
	GENERATED_BODY()

public:
	enum class EServerListViewSortType
	{
		Normal,
		Recommended,	// 2023/10/31 PKT - 추천
		Spacious,		// 2023/10/31 PKT - 여유
	};

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelServerListRoot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSelected;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;	

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIServerMigrateServerListSortComboBox* _comboBox;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _serverItemEntryWidgetClass;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _serverItemSlotHelper;

protected:
	ServerMigrateType _ownerType = ServerMigrateType::NONE;

	TFunction<void(WorldId)> _callbackFunc = nullptr;

	EServerListViewSortType _sortType = EServerListViewSortType::Normal;

	TArray<TWeakPtr<MigrateServer>> _serverList;
	TWeakPtr<MigrateServer> _selectServer{ nullptr };

	TPair<MessageContentStoreBM, FDelegateHandle> _refreshMsgHandler;

public:
	virtual void BeginDestroy() override;
protected:
	virtual void NativeOnInitialized() override;	
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void OnInputCancel() override;

protected:
	UFUNCTION()
	void OnClose();
	UFUNCTION()
	void OnServerItemRefresh(int32 InIndex, UWidget* InEntry);
	UFUNCTION()
	void OnServerSelected();

	void InvalidateServerMigrate(FGsSharedParam& InMessageParam);

	void OnMenuSelected(UGsUIServerMigrateServerListSortComboBox::MenuType type);
	void OnCallbackSelectedEntry(TWeakPtr<MigrateServer> InSelectedData);

public:
	void SetData(ServerMigrateType InOwnerType, TFunction<void(WorldId)> InCallbackFunc);
	void Invalidate();

private:
	int32 BuildViewServerItemList(EServerListViewSortType InType);
};
