#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITray.h"
#include "Message/GsMessageContents.h"
#include "GsUITrayObserveInfo.generated.h"

/*
* tray 관전 정보
*/

class UGsButton;

struct IGsMessageParam;

UCLASS()
class UGsUITrayObserveInfo : public UGsUITray
{
	GENERATED_BODY()

public:
	// 버틈: 좌측으로 변경
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UGsButton* _btnLeftTargetChange;
	// 버튼: 우측으로 변경
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UGsButton* _btnRightTargetChange;

	// text: 관전 타겟
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textObserveTarget;

	// text: 좌측 타겟
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textLeftObserveTarget;

	// text: 우측 타겟
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textRightObserveTarget;

private:
	// 메시지 델리게이트
	TArray<TPair<MessageContentObserver, FDelegateHandle>> _msgContensObserverDelegates;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// 메시지
public:
	// 관전 대상 변경
	void OnChangeObserveTaret(const IGsMessageParam* In_param);

	// 로직 함수
private:
	void ClearAllMessage();
	void ReqChangeTarget(bool In_isRight);

	// ui 이벤트 바인딩
public:
	// 좌측 변경 클릭
	UFUNCTION()
	void OnClickLeftTargetChange();
	// 우측 변경 클릭
	UFUNCTION()
	void OnClickRightTargetChange();
};