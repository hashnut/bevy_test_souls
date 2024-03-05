#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "Message/GsMessageGameObject.h"
#include "Buddy/GsBuddyDefine.h"
#include "Message/GsMessageUI.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "GsUIPopupBuddy.generated.h"

/*
	popup 친구
*/

class UWidget;
class UUserWidget;
class UScrollBox;
class UVerticalBox;
class UGsDynamicPanelSlotHelper;
class UEditableTextBox;
class UGsButton;
class UWidgetSwitcher;
class UCanvasPanel;
class UTextBlock;
struct IGsMessageParam;
class FGsBuddyInfo;
class UImage;
class FGsToggleGroup;
class UHorizontalBox;

UCLASS()
class  T1PROJECT_API UGsUIPopupBuddy : public UGsUIPopup
{
	GENERATED_BODY()

	// 멤버 변수
protected:
	UPROPERTY(EditdefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryWidgetClass;

	// 아래로 차례차례 추가
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UVerticalBox* _verticalBox;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UVerticalBox* _verticalBoxApply;

	// 토글버튼의 Root패널
	UPROPERTY(BlueprintReadOnly, Category = "GsAbnormality", meta = (BindWidget))
	class UPanelWidget* _panelTabRoot;

	// sub 토글버튼의 Root패널
	UPROPERTY(BlueprintReadOnly, Category = "GsAbnormality", meta = (BindWidget))
	class UPanelWidget* _panelSubTabRoot;

	UPROPERTY(BlueprintReadOnly, Category = "GsAbnormality", meta = (BindWidget))
	class UHorizontalBox* _subButtonBox;

	// 닫기 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

	// 친구 추가 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnAdd;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UEditableTextBox* _editUserName;

	// 슬롯갯수
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBuddyCount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _buddyRedDot;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _buddyAcceptRedDot;

	// 빈목록일때 연출할 상태
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherNoList;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _scrollBoxHelper;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _panelPopupBuddy;

protected:
	// 토글 그룹 관리 메인
	FGsToggleGroup _toggleGroupTab;
	// 토글 그룹 관리 서브
	FGsToggleGroup _toggleGroupTabSub;
	// 친구 목록
	TArray<TSharedPtr<FGsBuddyInfo>> _buddyList;
	// 친구, 요청, 수락 탭 index
	int _tabIndex = 0;
	int _tabSubIndex = 0;
	// 선택 된 친구 슬롯
	int _slotIndex = 0;
	// 선택 된 친구Id
	uint64 _selectUserDBId = 0;

	MsgUIHandle _updateListHandler;
	
	// 가상 함수
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void InvalidateAllInternal() override;

public:
	virtual void BeginDestroy() override;

protected:
	void InvalidateList(EGsBuddyTabType InIndex, EGsBuddySubTabType InSubIndex);
	void UpdateList(const IGsMessageParam* InParam);
	void OnClickSlot(int inSlotIndex);
	void SetSlotSelect(int inSlotIndex, bool isSelected);
	void UpdateTextCount();
	void SetRedDot();
	void SaveRedDot();

	UFUNCTION()
	void OnRefreshEntry(int32 InIndex, UWidget* InEntry);
	// 닫기 클릭
	UFUNCTION()
	void OnClickClose();
	// 친구 추가 버튼 클릭
	UFUNCTION()
	void OnClickAdd();
	// 탭 메뉴 클릭 시
	UFUNCTION()
	void OnClickTab(int32 InIndex);
	// 탭 서브 메뉴 클릭 시
	UFUNCTION()
	void OnClickTabSub(int32 InIndex);

	// 친구이름 체크
	UFUNCTION()
	void OnTextChangedBuddyName(const FText& InText);
	UFUNCTION()
	void OnTextCommittedBuddyName(const FText& InText, ETextCommit::Type InCommitType);

	// 2022/10/20 PKT - KeyBoard Mapping
protected:
	virtual void OnInputCancel() override;

public:
	UWidget* GetUserInfoTargetWidget() { return _panelPopupBuddy; }
};
