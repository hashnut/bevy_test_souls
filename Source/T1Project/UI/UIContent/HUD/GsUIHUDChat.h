#pragma once
#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIHUD.h"
#include "Chat/GsChatItem.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "GsUIHUDChat.generated.h"


/**
 * HUD 채팅 창
 * 4줄 표시(4개의 Chat 표시로 해석) 이므로, 색이나 이름 등등 포멧화된 4개의 문장을 줄 바꿈 있는 한 문장으로 표시
 * Chat Window - ScrollBox 사용
 **/

class UGsButton;
class UGsUIChatItem;
class UScrollBox;
class UGsUIListDataChat;

UCLASS(meta = (DisableNativeTick))
class UGsUIHUDChat : public UGsUIHUD
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnPopupChatWnd;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _chatLog;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIChatItem* _chatItem1;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIChatItem* _chatItem2;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIChatItem* _chatItem3;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIChatItem* _chatItem4;

protected:
	struct FTabTypeName
	{
		EGsExChatType _type;
		FText	 _name;
	};

	int32 _secWndClose = 10; //초
	int64 _lastInputTime = 0;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

public:
	virtual bool UpdateManagerTick(float InDeltaTime) override;

public:
	void OnChatAdded();

	//닫는 처리는 mainFrame에서 하자. 시간만 넘겨주자.
	bool isWindowCloseTime() { return (FGsTimeSyncUtil::GetServerNowSecond() - _lastInputTime) > _secWndClose; }
};

