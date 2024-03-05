// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Quest/GsQuestInstance.h"
#include "UI/UILib/Base/GsUIHUD.h"

#include "Message/GsMessageUI.h"

#include "EventProgress/GsEventProgressInterface.h"
#include "EventProgress/GsEventProgressDefine.h"


#include "GsUIHUDPenalty.generated.h"

class UTextBlock;
class UWidgetSwitcher;

/**
 * 메인 퀘스트 슬롯
 */
class UGsButton;
class UTextBlock;
class UGsUIRedDotBase;
struct IGsMessageParam;

UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIHUDPenalty : public UGsUIHUD
	, public IGsEventProgressInterface
{
	GENERATED_BODY()

private:
	MsgUIHandle _msgUIHandle;
	EGsUIHUDMode _hudMode;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _penaltyButton;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRedDotBase* _penaltyCountRedDot;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	UFUNCTION(BlueprintCallable, Category = "GsUI")
		void OnClickShowPopupPenalty();
	virtual void SetHUDMode(EGsUIHUDMode InMode) override;

public:
	// 이벤트 시작
	virtual void StartEvent(EGsEventProgressType In_startType, EGsEventProgressType In_preType) override;
	// 이벤트 종료
	virtual void FinishEvent(EGsEventProgressType In_finishType, EGsEventProgressType In_nextType) override;

public:
	void OnChangePenaltyCount(const IGsMessageParam* InParam = nullptr);
	void InvalidPenaltyCount();
	virtual void InvalidateAllInternal() override;
};
