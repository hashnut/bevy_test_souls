// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/UILib/Base/GsUITray.h"
#include "../OfflinePlay/GsOfflinePlayData.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUITrayOfflinePlayResults.generated.h"

/**
* file		GsUITrayOfflinePlayResults.h
* @brief	오프라인 플레이 결과 확인 창
* @author	PKT
* @date		2021/06/21
* */
UCLASS()
class T1PROJECT_API UGsUITrayOfflinePlayResults : public UGsUITray
{
	GENERATED_BODY()

protected:
	// 2021/06/21 PKT - 레벨 | 경험치(Percent : 0.0f ~ 100.0f)
	using LevelExpData = TPair<int32, float>;
	

	/************************************************************************/
	/* 2021/06/21 PKT - BP bind                                             */
	/************************************************************************/
protected:
	// 2021/06/18 PKT - 오프라인 플레이 진행 시간
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textRuningTime;

	// 2021/06/21 PKT - 습득한 재화(골드)량
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUICurrencySlot* _pickupCurrency;

	// 2021/06/18 PKT - 오프라인 플레이 진행 전 정보
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textBeforeLevel;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textBeforeExpPercent;

	// 2021/06/18 PKT - 오프라인 플레이 진행 후 정보
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textAfterLevel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textAfterExpPercent;
	
	// 2021/06/18 PKT - 확인 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnOk;

	// 2021/06/18 PKT - 아이템 Icon List
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsWrapBoxIconSelector* _iconSelector;

	UPROPERTY()
	class UGsDynamicIconSlotHelper* _slotHelper;

	/************************************************************************/
	/* 2021/06/21 PKT - 멤버 변수                                           */
	/************************************************************************/
	TArray<ItemDataCreated> _refreshItemList;

	TFunction<void(void)> _closeDelegate = nullptr;


	/************************************************************************/
	/* 2021/06/21 PKT - Construct / Destruct                                */
	/************************************************************************/
public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	/************************************************************************/
	/* // 2021/06/21 PKT - Event Delegate                                   */
	/************************************************************************/
	UFUNCTION()
	void OnCloseTray();
	
	/************************************************************************/
	/* 2021/06/21 PKT - Refresh Entry                                       */
	/************************************************************************/
	UFUNCTION()
	void OnRefreshIcon(int32 InIndex, class UGsUIIconBase* InIcon);
	
	/************************************************************************/
	/* 2021/06/21 PKT - Set...                                              */
	/************************************************************************/
	virtual void SetPlayTime(time_t InPlayTime);
	virtual void SetIncreasedGold(Currency InAmount);
	virtual void SetIncreasedExp(OffLineExp InIncreasedEXP);
	virtual void SetIncreasedItem(const TArray<ItemDataCreated>& InItemList);

	/************************************************************************/
	/* 2022/02/07 PKT - Long Press                                          */
	/************************************************************************/
private:
	void OnLongPressIcon(class UGsUIIconItem& InIcon);

public:
	virtual void SetData(const FGsOfflinePlayResultData& InData, TFunction<void(void)> InCloseEvent);
	// 2021/08/11 PKT - 결과창 위에서 아이템 디테일 팝업창이 가려지는 현상 때문에 예외적으로 ZOrder 조절(조절값 UiManager.h 참고)
	virtual int32 GetManagedDefaultZOrder() const override { return UI_DEPTH_POPUP - 1; }
};