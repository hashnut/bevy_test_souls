#pragma once
#include "CoreMinimal.h"

#include "UI/UILib/Base/GsUITray.h"

#include "Shared/Shared/SharedInclude/SharedPrimitiveTypes.h"

#include "GsUITrayBookmarkShare.generated.h"

/*
	popup 즐겨찾기 위치 공유
*/

class UGsButton;
class UProgressBar;

UCLASS()
class T1PROJECT_API UGsUITrayBookmarkShare : public UGsUITray
{
	GENERATED_BODY()
protected:
	// 거절 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnReject;
	// 등록 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnEnroll;
	// 이동 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnMove;	
	// 대기 닫기 프로그래스바
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UProgressBar* _progressBarWaitClose;
	// 송신자 정보
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textSenderInfo;
	// 북마크 이름
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textBookmarkName;

private:
	// 대기 시작 시간
	int64 _startTime;
	// 시간 종료
	bool _isTimeover = false;

	// 가상 함수
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	virtual EGsUIHideFlags GetHideFlagType() const override { return EGsUIHideFlags::UI_HIDE_ITEM_DETAIL; }

	// 로직 함수
protected:
	// 거절 버튼
	UFUNCTION()
	void OnClickReject();
	// 등록 버튼
	UFUNCTION()
	void OnClickEnroll();
	// 이동 버튼
	UFUNCTION()
	void OnClickMove();

public:
	// 데이터 세팅
	void SetData();

	// get, set
public:
	// 남은시간 rate 값 구하기
	float GetRemainTimeRate();
};