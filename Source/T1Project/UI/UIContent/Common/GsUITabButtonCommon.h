#pragma once

#include "UMG/Public/Blueprint/UserWidget.h"

#include "UI/UIControlLib/Define/GsIconDefine.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"

#include "GsUITabButtonCommon.generated.h"

class IGsToggleGroupEntry;
class UGsSwitcherButton;
class UGsUIRedDotBase;
class UTextBlock;
class UWidget;

/**
 * 탭버튼 공용 클래스
 * 선택/해제, 텍스트 설정, 잠김 상태, 레드닷 등의 기능을 구현함
 */
UCLASS()
class T1PROJECT_API UGsUITabButtonCommon : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnMain;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtSelected;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtUnselected;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetOptional))
	UWidget* _wgtLocked;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetOptional))
	UTextBlock* _txtLocked;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetOptional))
	UGsUIRedDotBase* _redDot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	UTextBlock* _txtSelectedSub;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	UTextBlock* _txtUnselectedSub;

protected:
	void NativeConstruct() override;

	/************************************************************************/
	/* setter																*/
	/************************************************************************/

public:
	void SetTitleText(const FText& InText);

	void SetSubText(const FText& InText);

	void SetLockedText(const FText& InText);

	void SetIsRedDot(EGsIconRedDotType InType, bool bIsRedDot, int32 InNumber = 0);

	/** 특정 상황에서, 인풋 이벤트가 먹지 않도록 하기 위한 함수 */
	//void SetInputBlock(bool InbShouldBlock);

	/************************************************************************/
	/* getter																*/
	/************************************************************************/

public:
	IGsToggleGroupEntry* GetToggleGroupEntry();

};