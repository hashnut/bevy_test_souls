#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "GsUIMailSlot.generated.h"

class UGsButton;
class UImage;
class UTextBlock;
class UWidgetSwitcher;
class FGsMailInfo;
class UGsWrapBoxIconSelector;
class UGsDynamicIconSlotHelper;
class UGsUIIconBase;
class UGsUIIconItem;
/**
 *  우편 슬롯
 */
UCLASS()
class UGsUIMailSlot : public UUserWidget, public IGsEntryInterface
	
{
	GENERATED_BODY()	

protected:
	// 제목
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textTitle;

	// 내용
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textContents;

	// 남은 시간
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textTime;

	// 슬롯 (전체)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSlot;

	// 받기/ 삭제
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnRecvDel;

	//// 아이템박스
	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//UImage* _imgItemBox;

	// 받기/삭제 Switcher
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _btnSwicher;

	// 레드닷
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgRedDot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsWrapBoxIconSelector* _iconSelector;

	// 동적 아이콘 슬롯 관리
	UPROPERTY()
	class UGsDynamicIconSlotHelper* _listIconSlotHelper;

protected:
	TSharedPtr<FGsMailInfo> _mailInfo;
	FTimerHandle _timerHandle;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void BeginDestroy() override;

public:
	virtual void OnHide() override;

public:
	// 받기/삭제 버튼 클릭
	UFUNCTION()
	void OnClickRecvDel();
	// 슬롯 클릭
	UFUNCTION()
	void OnClickSlot();
	// 데이터 세팅
	void SetData(TSharedPtr<FGsMailInfo>& inMailInfo);

protected:
	void OnTimer();
	UFUNCTION()
	void OnRefreshIconSlot(int32 InIndex, UGsUIIconBase* InIcon);
	UFUNCTION()
	void OnCreateIcon(UGsUIIconBase* InIcon);

protected:
	void OnLongPressIcon(class UGsUIIconItem& InIcon);
};