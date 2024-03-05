#pragma once
#include "CoreMinimal.h"

#include "UMG/Public/Blueprint/UserWidget.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"

#include "GsUIRegionCombobox.generated.h"

/*
	콤보 박스: 지역맵
*/

class UVerticalBox;
class UCanvasPanel;
class UImage;
class UTextBlock;
class UWidget;

class UGsDynamicPanelSlotHelper;
class UGsButton;
class FGsRegionInfo;

UCLASS()
class UGsUIRegionCombobox : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryWidgetClass;
	// 아래로 차례차례 추가
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UVerticalBox* _verticalBox;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _scrollBoxHelper;

	// 토글 그룹 관리
	FGsToggleGroup _toggleGroup;

	// 버튼: 리스트 보여주기
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnShowList;
	// 리스트 패널(hide, show 대상)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _panelList;
	// 현재 선택 맵이름
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textCurrentSelMapName;

	// town image
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgInTown;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _levelTextBlock;

private:
	FText _levelFormat;

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
	void InvalidLevelInfo(int32 inLevelId);

	// get
public:
	FGsRegionInfo* FindRegionInfo(int In_regionId);
	void SetIsInTown(bool In_isInTown);
};