#pragma once
#include "CoreMinimal.h"

#include "UI/UILib/Base/GsUITray.h"

#include "GsUIPopupPartyInviteAccept.generated.h"

/*
	popup 파티 초대 수락
*/

class UGsButton;
class UProgressBar;


UCLASS()
class T1PROJECT_API UGsUIPopupPartyInviteAccept : public UGsUITray
{
	GENERATED_BODY()
protected:
	// ok 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOk;
	// cancel 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;
	// 대기 남은시간 프로그래스바
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UProgressBar* _progressBarInviteWait;
	// 초대자 정보
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textInviterInfo;
	// 1. 같은 채널
	// 2. 다른 채널
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textInviteDesc;

private:
	// 대기 시작 시간
	int64 _startTime;
	// 시간 종료
	bool _isTimeover = false;

	// 가상 함수
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// 안드로이드 백키 처리
	// UGsUIWidgetBase override
	virtual bool OnBack() override;

public:
	virtual EGsUIHideFlags GetHideFlagType() const override { return EGsUIHideFlags::UI_HIDE_ITEM_DETAIL; }

	// 로직 함수
protected:
	// ok 클릭
	UFUNCTION()
	void OnClickOk();
	// cancel 클릭
	UFUNCTION()
	void OnClickCancel();

	// get, set
public:
	// 초대 대기 남은시간 rate 값 구하기
	float GetInviteWaitTimeRate();
};