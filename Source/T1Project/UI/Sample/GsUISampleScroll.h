// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Control/GsStaticPanelSlotHelper.h"
#include "GsUISampleScroll.generated.h"

class UGsSampleListItem;
class UGsSampleTreeItem;

/**
 * 스크롤 관련 샘플을 모아놓은 클래스
 * <UListView, UTileView, UTreeView>
 * - 스크롤 영역만큼의 Entry를 생성 후 재활용 하는 언리얼 제공 타입. ScrollBox를 사용하지 않음
 * - 실제 생성되는 슬롯을 Entry, 데이터를 Item 이라고 사용되고 있음
 * - 데이터(item)는 UObject 상속 클래스여야 함
 * - 프로퍼티 meta 정보에 EntryClass 명시
 *
 * <FGsStaticPanelSlotHelper>
 * - ScrollBox 내에 이미 배치된 슬롯을 관리하기 위해 만든 커스텀 클래스
 * - 배치된 슬롯을 초과하는 데이터들은 표시되지 않음
 *
 * <FGsDynamicPanelSlotHelper>
 * - ScrollBox 에 슬롯을 동적 생성하기 위해 만든 커스텀 클래스
 * - 한번 생성된 슬롯을 지우지 않고 Visible로 관리함(Panel에 AddSlot 시 발생하는 NewObject 횟수를 최소화 하기 위함)
 */
UCLASS()
class T1PROJECT_API UGsUISampleScroll : public UUserWidget
{
	GENERATED_BODY()

public:
	struct FGsSampleData
	{
		int32 _id = 0;

		explicit FGsSampleData(int32 InId) : _id(InId)
		{
		}
	};

protected:
	//------------------------------------------------------------------------------------------------------------------
	// 몇 개의 Entry를 생성 후 재활용 하는 언리얼 제공 타입
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget, EntryClass = UGsSampleListItem))
	class UListView* _listView;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget, EntryClass = UGsSampleListItem))
	class UTileView* _tileView;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget, EntryClass = UGsSampleTreeItem))
	class UTreeView* _treeView;

	//------------------------------------------------------------------------------------------------------------------
	// ScrollBox 이용한 일반 타입. 커스텀으로 제작된 Helper클래스 예시
	// StaticPanelSlotHelper 용
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UScrollBox* _scrollStatic;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UPanelWidget* _panelStatic;

	// DynamicPanelSlotHelper 용
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUI")
	TSubclassOf<class UUserWidget> _scrollBoxEntryClass;	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UScrollBox* _scrollDynamic;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UPanelWidget* _panelDynamic;	

protected:
	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _dynamicSlotHelper;

	class FGsStaticPanelSlotHelper _staticSlotHelper;

protected:
	// Data
	UPROPERTY()
	TArray<UGsSampleListItem*> _listItemList;
	UPROPERTY()
	TArray<UGsSampleListItem*> _tileItemList;
	UPROPERTY()
	TArray<UGsSampleTreeItem*> _treeRootItemList;

	TArray<FGsSampleData> _dataListStatic;
	TArray<FGsSampleData> _dataListDynamic;	

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	void SetStaticData(int32 InDataCount);
	void SetDynamicData(int32 InDataCount);
	void SetListViewData(int32 InDataCount);
	void SetTileViewData(int32 InDataCount);
	void SetTreeViewData();

protected:
	UFUNCTION()
	void OnRefreshEntryDynamic(int32 InIndex, UWidget* InEntry);
	UFUNCTION()
	void OnRefreshEntryStatic(int32 InIndex, UWidget* InEntry);

public:
	UFUNCTION(BlueprintCallable)
	void SetDataCountStatic(int32 InDataCount);
	UFUNCTION(BlueprintCallable)
	void SetDataCountDynamic(int32 InDataCount);
	UFUNCTION(BlueprintCallable)
	void SetDataCountListView(int32 InDataCount);
	UFUNCTION(BlueprintCallable)
	void SetDataCountTileView(int32 InDataCount);
};
