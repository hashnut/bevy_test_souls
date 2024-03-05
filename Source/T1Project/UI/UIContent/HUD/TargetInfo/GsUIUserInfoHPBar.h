// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "Message/GsMessageContents.h"
#include "Message/GsMessagePKBook.h"

#include "UserContext/GsUserContextData.h"
#include "../../Common/UserContext/GsUICommonUserContext.h"

#include "GsUIUserInfoHPBar.generated.h"

class UWidgetSwitcher;
class UGsSwitcherButton;
class UGsGameObjectCreature;
class UGsGameObjectRemotePlayer;
struct IGsMessageParam;
/**
 * HUD 상단 타겟 바의 유저 컨텍스트 UI (Simple)
 */
UCLASS()
class T1PROJECT_API UGsUIUserInfoHPBar : public UUserWidget, public FGsUICommonUserContext
{
	GENERATED_BODY()

	enum class SWITCHER_BOUNDARYTARGET_INDEX
	{
		Add,
		Remove,
	};

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnUserProfile;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnInviteParty;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnArena;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnWhisper;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnNameCopy;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnTrade;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnAddBoundaryTarget;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnRemoveBoundaryTarget;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherBoundaryTarget;

protected:
	TArray<TPair<MessageContentUserContext, FDelegateHandle>> _msgHandlerUserContextList;

	MsgPKBookHandle _PKBookMessageHandler;

protected:
	FGsUserContextInfo _userContextInfo;
	int64 _gameId = 0;

	bool _isClickedArena = false;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	void InvalidateButton();

public:
	void SetData(UGsGameObjectCreature* InTarget, const FString& InUserName);

protected:
	UFUNCTION()
	void OnClickUserProfile();
	UFUNCTION()
	void OnClickInviteParty();
	UFUNCTION()
	void OnClickArena();
	UFUNCTION()
	void OnClickWhisper();
	UFUNCTION()
	void OnClickCopy();
	UFUNCTION()
	void OnClickBoundaryTarget();
	UFUNCTION()
	void OnClickPersonalTrading();

	void OnClickedArena();

	void OnReceiveUserInfo(const struct IGsMessageParam* param);
	void InvalidateBoundaryTarget(const struct IGsMessageParam* InParam);

private:
	void ShowTicker(const FTextKey& InNamespace, const FTextKey& InKey);

	const UGsGameObjectRemotePlayer* FindTargetGameObject();
};
