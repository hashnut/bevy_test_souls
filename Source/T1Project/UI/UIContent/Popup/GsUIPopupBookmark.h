#pragma once

#include "CoreMinimal.h"

#include "UI/UILib/Base/GsUIPopup.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "Message/GsMessageGameObject.h"

#include "GsUIPopupBookmark.generated.h"

/*
	popup 위치 즐겨찾기
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

struct IGsMessageParam;

UCLASS()
class  T1PROJECT_API UGsUIPopupBookmark : public UGsUIPopup
{
	GENERATED_BODY()

	// 멤버 변수
protected:
	UPROPERTY(EditdefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryWidgetClass;
	// 스크롤
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBox;
	// 아래로 차례차례 추가
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UVerticalBox* _verticalBox;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _scrollBoxHelper;
	// 정렬 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSort;
	// 삭제 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnDel;
	// 수정 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnUpdate;
	// 닫기 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;
	// 지역맵으로 이동 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnMoveToRegionMap;
	// 생성 이름
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UEditableTextBox* _editCreateName;
	// 생성 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCreate;
	// 이름 정렬 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnNameSort;
	// 시간 정렬 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnTimeSort;
	// 버튼 사용 가능 스위치
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherBtnEnable;
	// 패널 sort
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _panelSort;
	// 이름 정렬 상태 스위치
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherNameSortState;
	// 시간 정렬 상태 스위치
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherTimeSortState;
	// 아이템 갯수
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textItemCount;
	// 리스트 갯수
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textListCount;
	// 아이템 없음 표시
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _widgetNoListItem;

	// 토글 그룹 관리
	FGsToggleGroup _toggleGroup;

	// 메시지 관리
	MsgGameObjHandleArray _arrDelegateGameObjectMsg;

	// 공유 메뉴 사용하는 아이템 인덱스
	int _shareMenuItemIndex = -1;

	// 가상 함수
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	virtual void BeginDestroy() override;

	// 로직 함수
private:
	// 데이터 초기화
	void InitData();

	// 리스트 초기화
	void InitList();
	// 정렬 패널 visible 처리
	void SetVisibleSortPanel(bool In_isVisible);
	// 리스트 없음 표시
	void SetVisibleNoListWidget(bool In_isVisible);

protected:
	UFUNCTION()
	void OnRefreshEntry(int32 InIndex, UWidget* InEntry);
	// 정렬 클릭
	UFUNCTION()
	void OnClickSort();
	// 삭제 클릭
	UFUNCTION()
	void OnClickDel();
	// 수정 클릭
	UFUNCTION()
	void OnClickUpdate();
	// 지역맵 이동 클릭
	UFUNCTION()
	void OnClickMoveToRegionMap();
	// 닫기 클릭
	UFUNCTION()
	void OnClickClose();
	// 생성 클릭
	UFUNCTION()
	void OnClickCreate();
	// 인덱스 클릭
	UFUNCTION()
	void OnClickListItem(int32 In_index);
	// 이름 정렬 클릭
	UFUNCTION()
	void OnClickNameSort();
	// 시간 정렬 클릭
	UFUNCTION()
	void OnClickTimeSort();

	UFUNCTION()
	void OnTextChanged(const FText& InText);

public:
	// 모든 리스트 변경
	void OnAllListChange(const IGsMessageParam*);
	// 선택 리스트 변경
	void OnSelListChange(const IGsMessageParam*);

	// get, set
public:
	// 공유 메뉴 아이템 인덱스 세팅
	void SetShareMenuItemIndex(int In_index);

	// 2022/10/20 PKT - KeyBoard Mapping
protected:
	virtual void OnInputCancel() override;
};
