// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIChatGroupPageInvitation.generated.h"

class UGsScrollBox;
class UGsSwitcherButton;
class UGsButton;
class UWidgetSwitcher;
class UPanelWidget;

struct FGsGuildGroupChatInviteHost;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIChatGroupPageInvitation : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textInvitedCount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _selectList;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsScrollBox* _invitationList;

	TArray<TWeakPtr<FGsGuildGroupChatInviteHost>> _invitations;

protected:
	void OnResponseInvitation(GuildGroupChatRoomId roomId, bool accepted);

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	void InitializeData();
	void RemoveItem(GuildGroupChatRoomId roomId);
	void RefreshUI();
};
