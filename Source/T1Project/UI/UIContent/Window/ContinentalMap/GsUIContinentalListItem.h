#pragma once

#include "CoreMinimal.h"

#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"

#include "UMG/Public/Blueprint/UserWidget.h"

#include "GsUIContinentalListItem.generated.h"

class UWidgetSwitcher;
class UGsButton;
class UGsUIAnimatedWidget;

UCLASS()
class UGsUIContinentalListItem : 
	public UUserWidget ,
	public IGsToggleGroupEntry,
	public IGsEntryInterface
{
	GENERATED_BODY()
protected:
	// 선택, 비선택 스위치
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherType;
	// 아이템 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnItem;
	// 이름(선택)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _textNameSelected;
	// 시간대 (선택)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _textTimeZoneSelected;
	// 이름(선택 안됨)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _textNameUnSelected;
	// 시간대(선택 안됨)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _textTimeZoneUnSelected;
	// 로컬 있는지 표시(0: 있음, 1: 없음)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switchLocal;

	bool _bIsSelected = false;

	// 가상 함수
protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;

public:
	// IGsToggleGroupEntry override
	virtual void SetIsSelected(bool bInIsSelected) override;
	virtual bool GetIsSelected() const override;

	// 로직 함수
public:
	// 텍스트 세팅
	void SetText(const FText& In_name, const FText& In_timeZone);
	// 대륙맵안인가
	void SetIsInContinentalMap(bool In_inMap);
	// 클릭 버튼
	UFUNCTION()
	void OnClickButton();
};