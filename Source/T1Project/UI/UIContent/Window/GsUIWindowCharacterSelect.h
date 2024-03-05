// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "GameFlow/GameContents/ContentsLobby/GsStateLobbyCharacterSelect.h"
#include "Management/ScopeLobby/GsLobbyDataType.h"
#include "CharacterSelect/GsUICharacterSlot.h"
#include "Message/GsMessageContents.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/Define/EGsPartsType.h"
#include "GsUIWindowCharacterSelect.generated.h"

/**
 * UGsUIWindowCharacterSelect
 * 캐릭터 선택 UI
 */
class UTextBlock;
class UGsLobbyDataManager;
class UWidgetSwitcher;
class UGsButton;
class USizeBox;

UCLASS()
class T1PROJECT_API UGsUIWindowCharacterSelect : public UGsUIWindow
{
	GENERATED_BODY()
public:
	DECLARE_DELEGATE_OneParam(FOnClickEvent, const UserDBId);

private:
	TPair<MessageContentsLobby, FDelegateHandle> _delegateUdateUI;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUI")
	TSubclassOf<class UUserWidget> _scrollBoxEntryClass;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UScrollBox* _scrollBox;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UVerticalBox* _verticalBox;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UInvalidationBox* _invalidationBox;

	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//UTextBlock* _textName;

	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//UTextBlock* _textLevel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcher;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _mapName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _weaponName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCharacterDelete;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherBtn;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textExpiringTime;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textCreatedTime;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textLastLoginTime;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textGuildName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnGameStart;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnCustomizing;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textCustomizing;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class USizeBox* _sizeBoxCustomize;

	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _scrollBoxHelper;	

	UPROPERTY()
	uint8 _characterIndex = -1;

	FGsToggleGroup _toggleGroup;
	TArray<LobbyDataType::FGsLobbyUserListData> _listCharacter;
	class FGsCustomizeHandler* _customizeHandler;

	FText _findExpiringTimeText;

public:
	FOnClickEvent OnSelectUser;
	FOnClickEvent OnDeleteUser;
	FOnClickEvent OnSlotClick;
	FOnClickEvent OnDeleteCancel;
	FOnClickEvent OnCustomizingTicketUse;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// UGsUIWidgetBase override
	virtual bool OnBack() override;

private:
	uint32 GetPartsMeshID(EGsPartsType InPartsType, LobbyDataType::FGsLobbyUserListData InLobbyUserData);

public:
	UFUNCTION()
	void IsBeingDeveloped();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickSelectUser();
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickDeleteUser();
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickCharacterDeleteCancel();
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickCustomizingTicketUse();

protected:
	UFUNCTION()
	void OnRefreshEntryScrollBox(int32 InIndex, class UWidget* InEntry);

	UFUNCTION()
	void OnClickSlot(const ECharacterSlotType InType, const uint8 InIndex);

public:
	UFUNCTION()
	void UpdateUserList();

public:
	uint8 GetCurrentSlotIndex();
};
