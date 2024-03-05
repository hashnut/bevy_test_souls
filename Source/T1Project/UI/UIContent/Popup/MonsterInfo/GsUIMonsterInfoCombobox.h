#pragma once

#include "CoreMinimal.h"

#include "UMG/Public/Blueprint/UserWidget.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"

#include "GsUIMonsterInfoCombobox.generated.h"

// 몬스터 정보 팝업 안에 콤보 박스(레벨 표시용)
// https://jira.com2us.com/jira/browse/CHR-24415

class UVerticalBox;
class UCanvasPanel;
class UImage;
class UTextBlock;
class UWidget;
class UWidgetSwitcher;

class UGsDynamicPanelSlotHelper;
class UGsButton;


UCLASS()
class  UGsUIMonsterInfoCombobox : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryWidgetClass;
	// 아래로 차례차례 추가(hide, show도 같이 처리)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UVerticalBox* _verticalBox;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _scrollBoxHelper;

	// 토글 그룹 관리
	FGsToggleGroup _toggleGroup;

	// 버튼: 리스트 보여주기
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnShowList;
	
	// 현재 선택 레벨
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textCurrenLevel;

	// 위, 아래 화살표
	// 0: 아래(리스트 안보일때)
	// 1: 위(리스트 보일때)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherListArrow;

	// 레벨 포맷
	FText _textLevelFormat;

	// 가상 함수
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
public:
	virtual void BeginDestroy() override;

	// 로직함수
public:
	UFUNCTION()
	void OnRefreshEntry(int32 InIndex, UWidget* InEntry);
	// 아이템 클릭
	UFUNCTION()
	void OnClickComboboxItem(int32 In_index);
	UFUNCTION()
	void OnSelectNotChange();
	// 리스트 보여주기 클릭
	UFUNCTION()
	void OnClickShowList();

public:
	// 최신 데이터로 세팅
	void InitData();
	// 리스트 초기화
	void InitList();
	// 리스트 보이게 할지
	void SetVisibleList(bool In_show);
};