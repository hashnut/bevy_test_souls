#pragma once
#include "CoreMinimal.h"
#include "../Message/GsMessagePKBook.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UserContext/GsUserContextData.h"
#include "../../Common/UserContext/GsUICommonUserContext.h"
#include "GsUIUserInfo.generated.h"


class UImage;
class UTextBlock;
class UGsButton;
class UWidgetSwitcher;
class UGsSwitcherButton;

enum class UserInfoAnchor : uint8
{
	NONE,

	TOP_RIGHT_SIDE,
	TOP_RIGHT_UP,

	TOP_LEFT_SIDE,
};

UCLASS()
class T1PROJECT_API UGsUIUserInfo : public UUserWidget, public FGsUICommonUserContext
{
	GENERATED_BODY()

protected:
	enum class SWITCHER_BOUNDARYTARGET_INDEX
	{
		Add,
		Remove,	
	};

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imageGuildEmblem;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockGuildName;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockUserLevel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockUserName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnUserProfile;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnGuild;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnAddFriend;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnInviteParty;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnWhisper;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnArena;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnNameCopy;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnReport;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnAddBoundaryTarget;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnRemoveBoundaryTarget;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherBoundaryTarget;
	

protected:
	FGsUserContextInfo _userContextInfo;

	EGsUserContentsOwner _ownerContents = EGsUserContentsOwner::NONE;
	EGsContextContentsMenu _requestContents = EGsContextContentsMenu::NONE;
	FString _chatText;
	bool _isDelayedOpen = false;

protected:
	MsgPKBookHandle _PKBookMessageHandler;

	UWidget* _targetWidget;
	UObject* _parent;
	UWidget* _targetArea;
	bool _isOpened = true;
	UserInfoAnchor _anchor = UserInfoAnchor::NONE;

public:
	void SetData(EGsUserContentsOwner InOwner, FGsUserContextInfo& InUserContextInfo);
	void SetTarget(UWidget* InTarget);
	void SetArea(UWidget* InAreaTarget);
	void SetAnchor(UserInfoAnchor InAnchor) { _anchor = InAnchor; }
	void SetParent(UObject* InParent) { _parent = InParent; }
	void SetIsDelayedOpen(bool InIsDelayedOpen);
	void SetReportContent(const FString& InMessage);

public:
	bool IsWaitingUserExist();
	void ReceiveUserExist(bool InIsExist);

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	void OnClickedUserProfile();
	void OnClickedGuild();
	void OnClickedAddFriend();
	void OnClickedInviteParty();
	void OnClickedWhisper();
	void OnClickedBlock();
	void OnClickedArena();
	void OnClickedCopy();
	void OnClickedReport();
	void OnClickedBoundaryTarget();

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickUserProfile();
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickGuild();
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickAddFriend();
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickInviteParty();
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickWhisper();
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickBlock();
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickArena();
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickCopy();
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickReport();
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickBoundaryTarget();

	void ClickButton(EGsContextContentsMenu InType);

protected:
	void InvalidateBoundaryTarget(const struct IGsMessageParam* InParam);
	void InvalidateButton();
};