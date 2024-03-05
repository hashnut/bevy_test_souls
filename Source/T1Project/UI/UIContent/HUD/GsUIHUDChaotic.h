// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIHUD.h"
#include "Message/GsMessageUserInfo.h"
#include "GsUIHUDChaotic.generated.h"


/**
 * HUD 로컬플레이어 카오틱 상태 표시
 */

UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIHUDChaotic : public UGsUIHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsButton* _chaoticButton;

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _chaosGradeTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UPanelWidget* _chaoticInfoPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UWidgetSwitcher* _chaoticInfoWidgetSwitcher;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _chaoticPointTextBlock;

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UWidgetSwitcher* _chaoticIconWidgetSwitcher;

private:
	bool _isShowChaoticButton = false;
	const struct FGsSchemaPVPAlignmentData* _maxPvpAlignmentData = nullptr;
	TArray<const struct FGsSchemaPVPAlignmentData*> _pvpAlignmentDataArray;
	TArray<TPair<MessageUserInfo, FDelegateHandle>> _handlerList;
	FText _formatText;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	void BindMessage();
	void UnbindMessage();
	void InvalidChaoticInfo();
	void InvalidPVPAlignmentPoint();
	void InvalidPVPAlignmentGrade();
	UFUNCTION()
		void OnClickChaoticButton();
	UFUNCTION()
		void OnInvalidPVPAlignmentPoint(uint64 inId);
	UFUNCTION()
		void OnInvalidPVPAlignmentGrade(uint64 inId);	
	int32 GetChaosGrade();
	void InitMaxPvpAlignmentPointMax();
};
