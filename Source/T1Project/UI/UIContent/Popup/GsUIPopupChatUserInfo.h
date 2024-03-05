#pragma once
#include "CoreMinimal.h"

#include "UI/UILib/Base/GsUIPopup.h"
#include "UI/UIContent/Popup/UserInfo/GsUIUserInfo.h"

#include "Message/GsMessageContents.h"
#include "Message/GsMessageUI.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"

#include "UserContext/GsUserContextData.h"
#include "../Common/UserContext/GsUICommonUserContext.h"

#include "GsUIPopupChatUserInfo.generated.h"

enum class UserInfoContentType : uint8
{
	NONE,
	CHAT,
	BUDDY,
	PARTY,
	RANKING,
	GUILD_MEMBER,
	USER_PROFILE,
};

class UGsButton;
UCLASS()
class T1PROJECT_API UGsUIPopupChatUserInfo : public UGsUIPopup
{
	GENERATED_BODY()

public:
	struct PopupInitData
	{
		uint64 _uid;
		FString _nickName = TEXT("");
		FString _chatText = TEXT("");
		bool _isShowRight = false;
		
		EGsUserContentsOwner _owner = EGsUserContentsOwner::NONE;

		//EGsContextButtonHideFlags _hideFlags = EGsContextButtonHideFlags::NONE;

		WorldId _homeWorldId = INVALID_WORLD_ID;
	};

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIUserInfo* _userInfoVertical;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIUserInfo* _centerUserInfo;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _touchClose;

protected:
	TArray<TPair<MessageContentUserContext, FDelegateHandle>> _msgHandlerUserContextList;
	TPair<MessageContentGuild, FDelegateHandle> _msgHandlerGuild;
	MsgUIHandleArray _msgHandlerUIList;
	FDelegateHandle _handleResize;

protected:
	PopupInitData _data;
	FGsUserContextInfo _userContextInfo;

	bool _isDelayedOpen = false;

	UserInfoContentType _contentType = UserInfoContentType::NONE;
	bool _isHorizontal = true;

public:
	virtual bool CanMultipleInstance() const override { return true; }

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	void SetData(PopupInitData* initParam, bool InIsHorizontal);
	void SetPosition(UWidget* InTarget, UWidget* InArea = nullptr);
	//virtual void InitializeData(PopupInitData* initParam = nullptr);

	//void SetType(UserInfoContentType InContentType, bool InIsHorizontal);
	//void SetPosition(UWidget* InTarget, UWidget* InArea = nullptr);

	UGsUIUserInfo* GetUserInfoUI();

protected:
	// 2023/9/13 PKT - 유저 정보 갱신.
	void OnReceiveUserInfo(const IGsMessageParam* param);
	
	// 2023/9/13 PKT - 유저 정보 요청 이후 Invalid한 상황일때 오는 패킷에 대한 처리
	void OnReceiveInvalidUser(const IGsMessageParam* param);
	void OnReceiveInvalidUserOtherServer(const IGsMessageParam* param);

	// 2023/9/13 PKT - 길드 정보
	void OnReceiveOtherGuildDetailInfo(const IGsMessageParam* param);

	void OnOpenMiddleSmallPopup(const IGsMessageParam* param);
	void OnResizeViewport(class FViewport* InViewport, uint32 InNum);
	
	// 2022/10/20 PKT - KeyBoard Mapping
	virtual void OnInputCancel() override;

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	virtual void OnTouchedExtraRange();
};

