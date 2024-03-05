// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "GsUIWindowMailDetail.generated.h"

class UTextBlock;
class UWidgetSwitcher;
class UGsButton;
class UGsWrapBoxIconSelector;
class UGsDynamicIconSlotHelper;
class UGsUIIconBase;
class FGsMailInfo;
class UGsUIIconItem;

/**
 * 우편 상세창
 */
UCLASS()
class T1PROJECT_API UGsUIWindowMailDetail : public UGsUIWindow
{
	GENERATED_BODY()

protected:
	// 제목
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockTitle;

	// 내용
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget, MultiLine = true))
	UTextBlock* _textContents;

	// 우편 갯수
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textMailCount;

	// 받기/삭제 Switcher
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _btnSwicher;

	// 닫기 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnClose;

	// 받기 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnReceive;

	// 삭제 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnDelete;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsWrapBoxIconSelector* _iconSelector;

	// 동적 아이콘 슬롯 관리
	UPROPERTY()
	class UGsDynamicIconSlotHelper* _listIconSlotHelper;

protected:
	TSharedPtr<FGsMailInfo> _mailInfo;
	uint64 _mailDBId = 0;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual bool SyncContentState() override;

public:
	virtual void Close(bool bImmediate = false, bool bRecoverPrevStack = true) override;

protected:
	void InvalidateIconList();

	// 받기 버튼 눌렀을 때
	UFUNCTION()
	void OnClickReceive();
	// 삭제 버튼 눌렀을 때
	UFUNCTION()
	void OnClickDelete();
	// 아이콘 갱신
	UFUNCTION()
	void OnRefreshIcon(int32 InIndex, class UGsUIIconBase* InIcon);
	UFUNCTION()
	// 아이콘 생성
	void OnCreateIcon(class UGsUIIconBase* InIcon);

protected:
	void OnLongPressIcon(class UGsUIIconItem& InIcon);

public:
	void SetData(uint64 inMailDBId);	
};
