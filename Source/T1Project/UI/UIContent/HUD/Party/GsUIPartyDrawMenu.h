#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITooltip.h"

#include "Party/GsPartyDefine.h"
#include "GsUIPartyDrawMenu.generated.h"


class UWidgetSwitcher;

class UGsButton;

UCLASS()
class T1PROJECT_API UGsUIPartyDrawMenu : public UGsUITooltip
{
	GENERATED_BODY()
protected:
	// 드로우 메뉴 스위치(0:member/ im leader, 1: member/ im not leader)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherDrawMenu;

	// btn: 
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnUserInfoImLeader;
	// btn: change leader
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnChangeLeader;
	// btn: kick
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnKick;

	// leader(im not leader)
	// btn: user info context
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnUserInfoLeader;
	// btn: channel move
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnChannelMove;
	// switch
	// 0: moveBtn visible
	// 1: moveBtn not visible, only text show
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherChannelMove;

	// member(im not leader)
	// btn: user info context
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnUserInfoMemberImNotReader;

public:
	TFunction<void()> _callbackChangeLeader;
	TFunction<void()> _callbackKick;
	TFunction<void()> _callbackChannelMove;
	TFunction<void()> _callbackUserInfo;

	// virtual func
protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;

	// ui event
public:
	
	// change leader
	UFUNCTION()
	void OnClickChangeLeader();
	// kick
	UFUNCTION()
	void OnClickKick();
	
	// channel move
	UFUNCTION()
	void OnClickChannelMove();
	// user info
	UFUNCTION()
	void OnClickUserInfo();

	// logic func
public:
	void SetType(EGsUIPartyHUDDrawMenuSwitchType In_type);
};