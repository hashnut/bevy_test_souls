#pragma once

#include "Craft/GsUICraftResultListItem.h"
#include "UI/UILib/Base/GsUITray.h"
#include "UI/UILib/Define/GsUIDefine.h"

#include "Blueprint/UserWidget.h"

#include "GsUITrayCraftResult.generated.h"

class FTimerManager;
class UGsButton;
class UGsDynamicPanelSlotHelper;
class UGsUICraftResultListItem;
class UPanelWidget;
class UTileView;
class UScrollBox;
class UTextBlock;
class UWidget;
struct FGeometry;

UCLASS()
class T1PROJECT_API UGsUITrayCraftResult final : public UGsUITray
{

	GENERATED_BODY()

public:
	struct Parameters
	{

		/** 실패 가능성 여부. false 라면(실패가 불가능하다면), 모든 결과를 즉시 오픈하고, "실패" 란을 숨김.
		 *  true 라면(실패가 가능하다면) 모든 결과를 close 상태로 출력하고, "실패" 란을 출력함.
		 */
		bool _canBeFail;

		bool _canBeSuccess;

		bool _canBeGreatSuccess;

		/** 각 결과 */
		TArray<FCraftResultParameters> _results;

	};

	/************************************************************************/
	/* sub class of                                                         */
	/************************************************************************/

protected:
	/** 실패가 없어 한번에 모두 오픈할 때, 지정 인덱스로 이동 후, 엔트리를 그려주기 위해 기다려줄 시간 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	float _allOpeningScrollOffsetSecs = 0.05f;

	/** 자동 오픈 시, 각 개별 오픈마다의 시간 차이(초) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	float _defaultAutoOpeningGapSecs = 0.5f;

	/** 자동 오픈 시, 총 걸리는 시간이 이 시간(초)을 초과할 수 없도록 제한한다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	float _maxOpeningDurationSec = 10.f;

	/************************************************************************/
	/* properties                                                           */
	/************************************************************************/

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTileView* _tileViewCraftResults;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _containerGreatSuccess;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtGreatSuccess;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _containerSuccess;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtSuccess;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _containerFail;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtFail;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnResult;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtResult;

	UPROPERTY()
	TArray<UGsCraftResultListItemData*> _tileRootSlotList;

	UPROPERTY()
	TArray<UGsCraftResultListItemData*> _tileItemDataPool;

private:
	FText _txtBtnDefault;

	Parameters _parameters;

	FTimerHandle _autoOpeningTimerHandle;

	float _autoOpeningGapSecs = 0.5f;

	/** 실패가 없는지 여부. 실패가 없으면 모든 녀석을 한 번에 오픈한다 */
	bool _isAllSuccess = false;
	
	/** 자동 오픈 진행중 여부 */
	bool _isAutoOpening;

	/** 자동 오픈 중, 스크롤을 하길 원하는 Offset */
	float _desiredScrollOffset;

protected:
	void NativeConstruct() final;
	void NativeDestruct() final;

	void NativeOnInitialized() final;

public:
	EGsUIHideFlags GetHideFlagType() const final;

	/************************************************************************/
	/* behavior                                                             */
	/************************************************************************/

private:
	/**
	 * 만약 실패가 없을 경우, 바로 오픈해야 한다. 이때 Entry 가 그려질 시간이 필요하여 약간의 Offset 을 부여한다.
	 */
	void OpenAllResult();

	/**
	 * 열린 모든 슬롯에 대해 대성공/성공/실패 결과를 집계하고 출력한다
	 */
	void InvalidateOpenedResult();

	/**
	 * 슬롯의 결과를 오픈한다. 해당 슬롯 위치로의 자동 스크롤 기능도 지원한다
	 */
	bool OpenCraftResult(UGsCraftResultListItemData* InCraftResultListItem, int32 InIndex);

	void SwitchButton(const bool InAllCompleted);

	/************************************************************************/
	/* auto opening                                                         */
	/************************************************************************/

private:
	void StartAutoOpening();

	void AutoOpeningAndReserveNext();

	int32 AutoOpeningNextAndGetNextClosedCraftResultMinIndex();

	/**
	 * 각 오픈 연출 간격 시간을 계산한다
	 * 오픈 연출이 너무 길어질 경우에 대비하여 오픈 연출 시간을 제한하기 위해 사용.
	 * (최대오픈연출시간 / 오픈할갯수) 와 _defaultAutoOpeningGapSecs 중 작은 값을 반환함.
	 */
	float CalcAutoOpeningGapSecs();

	/* 닫혀 있는 결과 슬롯 중, 가장 Index 가 작은 녀석을 리턴한다  */
	int32 FindClosedCraftResultMinIndex() const;

	/************************************************************************/
	/* setter                                                               */
	/************************************************************************/

public:
	/**
	 * 서버로부터 제작 결과에 대한 정보를 저장한다.
	 * 만약 실패가 없을 경우, 모두 즉시 오픈한다
	 */ 
	void SetParameters(Parameters InParameters);


	/************************************************************************/
	/* getter                                                               */
	/************************************************************************/

private:
	FTimerManager* GetTimerManager() const;

	/************************************************************************/
	/* events                                                               */
	/************************************************************************/

protected:
	/** 리스트 아이템이 클릭되면 호출될 함수 */
	UFUNCTION()
	void OnClickedListItem(UObject* InCraftResultListItemData);

	/** 엔트리가 보이면 호출될 함수 */
	//UFUNCTION() -> UFUNCTION 을 쓰려면 UStruct 가 포인터 타입이어야 함
	void OnEntryScrolledIntoView(UObject* InCraftResultListItemData, UUserWidget& widget);

	/** 스크롤 이벤트가 발생할 때 호출될 함수. 자동 오픈 중에는 오픈되는 엔트리로 스크롤을 해 버린다. */
	void OnScrolledListView(float currentScrollOffset, float maxScrollOffset);

	/** 제작 결과 윈도우가 커졌다 작아지는 것이 완료된 이후에, 제작 결과를 한꺼번에 보여줄 때 사용할 이벤트 */
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnUIEventWindowFixed();


	UFUNCTION()
	void OnClickedAutoOpening();

	UFUNCTION()
	void OnClickedClose();

	/** 엔트리가 처음 생성되었을 때, 튀는 현상을 방지하기 위해 호출한다 */
	//UFUNCTION()
	//void OnEntryInitializedEvent(UObject* itemData, UUserWidget* widget);

//private:
//	/**
//	 * TileView 에 들어갈 ItemData 들에 대해, 풀 관리를 해주는데 사용될 함수들
//	 */
//	UGsCraftResultListItemData* CreateTileItem();
//	void ReleaseTileItem(UGsCraftResultListItemData* InItemData);
//	void ReleaseAllTileItem();
//	void FreeTileItem();
};
