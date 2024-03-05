#pragma once

#include "CoreMinimal.h"

#include "UI/UILib/Base/GsUIHUD.h"

#include "GsUIHUDScan.generated.h"

/*
*	HUD 스캔
*/

class UGsButton;

class UGsUIScanMemberItem;

UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIHUDScan : public UGsUIHUD
{
	GENERATED_BODY()
protected:
	// 리스트 클리어 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClearList;
	// 스캔 옵션 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnScanOption;

	// 스캔 아이템
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI")
	TArray<UGsUIScanMemberItem*> _arrayScanItems;

private:
	// ui 전체 갱신 flag
	bool _isDirtyRefreshUI = false;

	// 가상함수
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

public:
	virtual bool UpdateManagerTick(float InDeltaTime) override;

	// 로직 함수
public:
	// ui 갱신
	void RefreshUI();
	// 모든 리스트 hide
	void HideAllList();
	// 스캔 리스트 삭제 클릭
	UFUNCTION()
	void OnClickDelScanList();
	// 스캔 옵션 팝업 열기
	UFUNCTION()
	void OnClickOpenOption();
public:
	// ui 갱신 요청
	void SetDirtyRefreshUI()
	{
		_isDirtyRefreshUI = true;
	}

//----------------------------------------------------------------------------------------------------------------------
	// Tutorial
protected:
	int32 _tutorialSlotIndex = -1;
	
public:
	UGsUIScanMemberItem* SetTutorialTarget(int32 InSlotIndex);
	bool IsTutorial() const { return (0 <= _tutorialSlotIndex) ? true : false; }
};