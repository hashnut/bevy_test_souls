#pragma once
#include "CoreMinimal.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "Message/GsMessageGameObject.h"
#include "Map/WorldMap/GsWorldMapDefine.h"
#include "GsUIRegionScenePortalPanel.generated.h"

class UWidgetSwitcher;
class UVerticalBox;
class UScrollBox;
class UGsDynamicPanelSlotHelper;
class UWidget;

UCLASS()
class T1PROJECT_API UGsUIRegionScenePortalPanel : public UUserWidget
{
	GENERATED_BODY()

protected:
	// left list
	// 0: list show
	// 1: no show list in dungeon
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _leftPanelWidgetSwitcher;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryWidgetClass;
	// 아래로 차례차례 추가
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UVerticalBox* _verticalBox;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBox;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _scrollBoxHelper;

protected:
	// 아이템 갯수
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textItemCount;

private:
	// 현재 맵 포털 리스트 크기
	int _currentPortalListSize = 0;
	// 현재 맵 즐겨 찾기 리스트 크기
	int _currentBookmarkListSize = 0;

private:
	// 토글 그룹 관리
	FGsToggleGroup _toggleGroup;

	// 가상 함수
	/*
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	virtual void BeginDestroy() override;

public:
	void InitializeWidget();

	// 로직 함수
public:
	void InitializeMessage();
	void FinalizeMessage();

private:
	// 데이터 초기화
	void InitRegionScenePortalPanelData();
	// 리스트 초기화
	void InitList();
	// 통합 인덱스로 type과 data index 찾기
	void FindInfoByTotalIndex(int In_totalIndex, EGsRegionWarpListItemType& Out_itemType,
		int& Out_dataIndex);
	// 인포로 통합 인덱스 구하기
	int FindTotalIndexByInfo(EGsRegionWarpListItemType In_itemType,
		int In_dataIndex);
	// release select list and hide icon menu
	void ReleaseSelectList();

private:
	UFUNCTION()
	void OnRefreshEntry(int32 InIndex, UWidget* InEntry);
	// click left list item
	UFUNCTION()
	void OnClickLeftListItem(int32 In_index);
	*/
};