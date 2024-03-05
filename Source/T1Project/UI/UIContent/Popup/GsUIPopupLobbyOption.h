// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../UI/UILib/Base/GsUIPopup.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "GsUIPopupLobbyOption.generated.h"

class UHorizontalBox;
struct FGsSchemaOptionCategory;
class UGsUIOptionSubArea;
class UGsUIOptionAccountPage;
class UWidgetSwitcher;
/**
 *  로비 옵션 팝업
 */
UCLASS()
class T1PROJECT_API UGsUIPopupLobbyOption : public UGsUIPopup
{
	GENERATED_BODY()

protected:// 대분류 목록 리스트
	// 리소스
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUI")
	TSubclassOf<class UUserWidget> _scrollBoxEntryClass;

	// 스크롤헬퍼
	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _scrollBoxHelper;

	// 스크롤
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UScrollBox* _scrollBox;

	// 아래로 차례차례 추가
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UHorizontalBox* _horizontalBox;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIOptionAccountPage* _accountPage;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcher;

	// 현재 선택 메뉴
	UPROPERTY()
	int32 _menuIndex;

	// 중분류 이하 영역
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIOptionSubArea* _subArea;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnClose;

	// 선택 on/off 헬퍼
	FGsToggleGroup _toggleGroup;

	// 목록 리스트
	TArray<const FGsSchemaOptionCategory*> _categoryList;


protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void CloseInternal() override;

public:
	// UObject override
	virtual void BeginDestroy() override;

protected:
	// 스크롤 업데이트
	UFUNCTION()
	void OnRefreshEntryScrollBox(int32 InIndex, class UWidget* InEntry);

	// 메뉴 클릭
	UFUNCTION()
	void OnClickSlot(int32 InIndex);

	// 창 닫기
	UFUNCTION()
	void OnClickClose();

	// 대분류 이하 업데이트
	UFUNCTION()
	void UpdateList();

	// 2022/10/20 PKT - KeyBoard Mapping
protected:
	virtual void OnInputCancel() override;
};
