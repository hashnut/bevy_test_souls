#pragma once
#include "CoreMinimal.h"

#include "UMG/Public/Blueprint/UserWidget.h"
#include "SlateCore/Public/Layout/Geometry.h"

#include "UI/UIControlLib/Control/GsToggleGroup.h"

#include "GsUIRegionMonsterList.generated.h"

/*
	지역맵: 몬스터 리스트
*/

class UGsDynamicPanelSlotHelper;

class UVerticalBox;
class UWidget;
class UScrollBox;
class UWidgetSwitcher;

UCLASS()
class T1PROJECT_API UGsUIRegionMonsterList : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnSelectRegionMonsterList, int32);

public:
	FOnSelectRegionMonsterList OnSelectMonsterList;

protected:
	UPROPERTY(EditdefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryWidgetClass;
	// 아래로 차례차례 추가
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UVerticalBox* _verticalBox;
	// 스크롤 박스
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBox;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcher;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _scrollBoxHelper;		

	// 토글 그룹 관리
	FGsToggleGroup _toggleGroup;

	bool _isDirtyScrollOffsetMove = false;
	int _moveIndex = 0;


	// 가상 함수
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
public:
	virtual void BeginDestroy() override;
	// 로직함수
public:
	UFUNCTION()
	void OnRefreshEntry(int32 InIndex, UWidget* InEntry);
	// 아이템 클릭
	UFUNCTION()
	void OnClickComboboxItem(int32 In_index);
	
	// 최신 데이터로 세팅
	void InitRegionMonsterListData();
	// 리스트 초기화
	void InitList();

	void SelectList(int In_index);

	void CheckDirtyScrollBoxOffsetMove();
	// 몬스터 정보 출력
	void ShowMonsterInfo(int32 In_index);
};