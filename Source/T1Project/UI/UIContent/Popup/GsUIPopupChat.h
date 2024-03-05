#pragma once
#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "Message/GsMessageUI.h"
#include "Message/GsMessageContents.h"
#include "Chat/GsChatItem.h"
#include "Chat/GsUIDropDownMenuChat.h"
#include "GsUIPopupChat.generated.h"

class UGsUIListEntryChat;
class UGsUIListDataChat;
class UGsSwitcherButton;
class UGsUIChatTabUseSubTab;
class UGsScrollBox;
class UGsEditableTextBox;
class UGsDynamicPanelSlotHelper;

class UGsButton;
class UWidgetSwitcher;
class UTextBlock;
class UScrollBox;
class UPanelWidget;
class UScrollBox;
class UUserWidget;
struct IGsMessageParam;
class FGsGuildGroupChatRoom;

struct FGsChatOption : public IMenuData
{
	enum class Method { MenuHidden, PartMenu, FullMenu };
	EGsChatViewType _chatTab;
	uint64 _chatGroupId{ 0 };
	Method _method = Method::MenuHidden;

	FGsChatOption(EGsChatViewType chatTab) : _chatTab(chatTab) {};
};

struct FGsGroupChat : public IMenuData
{
	uint64 _chatGroupId{ 0 };
	FGsGroupChat(uint64 chatGroupId) : _chatGroupId(chatGroupId) {};
};

UCLASS()
class T1PROJECT_API UGsUIPopupChat : public UGsUIPopup
{
	GENERATED_BODY()

	DECLARE_DELEGATE_OneParam(FOnInputChat, const FText&);

	const int REQ_GUILDCHAT_OFFSET{ 2 };
	const int LIST_VIEWCOUNT{ 15 };

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UWidgetSwitcher* _noHistroySelection;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UGsScrollBox* _scroll_Chat;

#pragma region tabButtons
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIChatTabUseSubTab* _btnTabAll;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIChatTabUseSubTab* _btnTabSystem;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIChatTabUseSubTab* _btnTabNormal;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIChatTabUseSubTab* _btnTabWorld;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIChatTabUseSubTab* _btnTabInvasion;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIChatTabUseSubTab* _btnTabParty;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIChatTabUseSubTab* _btnTabAlliance;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIChatTabUseSubTab* _btnTabGuild;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIChatTabUseSubTab* _btnTabWhisper;
#pragma endregion

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _tabCategory;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _tabButtonPanel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsEditableTextBox* _editEnterText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnMacro;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnMngBlock;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnResetWhispererName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIDropDownMenuChat* _inputSelector;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _sizeBoxNormal;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _sizeBoxTrade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI", meta = (BindWidget))
	FSlateColor _editorTextColor;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelPopupChat;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnGroupChat;

public:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickClose();

protected:
	// 출력 타입 챗 Select
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnTabChanged(int32 index);

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	bool OnSelectSubTab(int32 index, bool forced);
	// 입력 타입 Menu Select
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	bool OnInputSelected(int32 index, bool forced = false);

	// EditBox Updated
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnChangedText(const FText& OriginalText);

	// EditBox Commited
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnCommitedText(const FText& inputText, ETextCommit::Type InCommitType);

	// ScrollBox Scrolled
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnScrollOffset(float offset); 

	// MenuBox Creaded
	UFUNCTION()
	void OnCreateMenuEntry(int32 index, struct FMenuData& menuData);
	
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickResetToWhisperName();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnPopupUserInfo(UGsUIListDataChat* data);

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnPopupMacro();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnPopupOptionNUserBlock();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnPopupGroupChat();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void CommitChatInput(const FText& msg);

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void CommitWhisperInput(const FText& usernick, const FText& msg);

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnInputFocusReceived(bool focus);

protected:
	FGsToggleGroup _toggleGroup;
	
	EGsChatViewType _curTab{ EGsChatViewType::ALL };
	uint64 _curSubTabRoomId{ 0 };

	EGsExChatType _inputChatType{ EGsExChatType::NORMAL };
	uint64 _inputGroupRoomId{ 0 };

	bool _isOpenMacro{ false };
	bool _IsOpenOption{ false };
	bool _isOpenGroupChat{ false };

	FText _editHintProhibition;
	FText _editHint;
	 
	FSlateColor _backupTextColor;

private: // whisper 입력 처리
	FOnInputChat InputDelegate;

	FText _editHintWhisperTarget;
	FText _editHintWhisperFormat;

	int32 _curMaxInput{ 0 };
	FText _whisperTarget;
	WorldId _targetWorldId{ 0 };
	
	bool _isUpdate{ false };
	bool _InputEnableFocus{ false };
	
private: // guild chat scroll 처리
	int _guildChatPos{ 0 };
//	int _allyguildChatPos{ 0 };
	int _preChatTop{ 0 };

	int8 _scrollProc{ 0 };

	bool _isUserScrolled{ false };
	bool _toBottomScrolled{ true };

protected:
	TArray<TPair<MessageContentChat, FDelegateHandle>> _chatMessageHandlers;
	TArray<TPair<MessageContentPersoncalTrading, FDelegateHandle>> _tradeMessageHandlers;

	//Tab Index, type
	TMap<int32, EGsChatViewType> _tabIndexPair;
	//DropDown Menu에 사용될 탭 목록
	TArray<TPair<EGsExChatType, UGsUIChatTabUseSubTab*>> _dropDownMenu;
	TArray<TWeakPtr<FGsGuildGroupChatRoom>> _groupRooms;

	// 개인 거래용 처리
protected:
	bool _isTradeMode;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void CloseInternal() override;

public:
	struct PopupInitData
	{
		EGsChatViewType selectTab{ EGsChatViewType::Max };
		EGsExChatType inputChatType{ EGsExChatType::Max };
		FString	   initWhisper;
		WorldId	   worldId{ 0 }; // 같은 서버일 때는 0 여도 된다.
		uint64	selectSubTabRoomId{ 0 };
		uint64  inputGroupRoomId{ 0 };
	};

	void InitializeData(PopupInitData* initParam = nullptr);
	virtual void BeginDestroy() override;

	void SetTradeMode();

	bool IsInputFocused();

protected:
	void OnExit();
	void OnCloseTrade();

	void SetChatListItem(EGsChatViewType listType, uint64 groupId = 0);
	void SelectInputWithTab(EGsChatViewType tab, uint64 subTabRoomId);

	void OnChatListAdded(const IGsMessageParam* param); 
	void OnChatListInserted(const IGsMessageParam* param); 
	void OnChatStateChanged(const IGsMessageParam* param);
	void OnChatListRefresh(const IGsMessageParam* param);
	void OnResultCheckUserExistByName(const IGsMessageParam* param);
	void OnOperateMessageAdded(const IGsMessageParam* param);

	void OnUpdateChatGroupInfo(const IGsMessageParam* param);
	void OnUpdateChatGroupNameChanged(const IGsMessageParam* param);
	void OnUpdateChatGroupAuthGrade(const IGsMessageParam* param);

protected:
	void InitInputDropDownMenu();
	void InitTabAndInput(IN OUT EGsChatViewType& tab, IN OUT uint64& tabGroupId, IN OUT EGsExChatType& input, IN OUT uint64& inputGroupId, IN FString whisperer = TEXT(""));

private:
	//bool IsScrollEndPostion();
	void InitReservedWhisperInfo(UGsUIPopupChat::PopupInitData* initParam);
	FText GetCurrentHintMessage(EGsChatViewType tab, int tabIndex = INDEX_NONE);

	bool CheckEnableInput(EGsExChatType inputType, uint64 groupId = 0);
	bool CheckEnableTab(EGsChatViewType tab);

	// 2022/10/20 PKT - KeyBoard Mapping
protected:
	virtual void OnInputCancel() override;
};

