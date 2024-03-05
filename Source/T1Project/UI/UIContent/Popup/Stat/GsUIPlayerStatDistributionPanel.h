// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "Message/GsMessageContentHud.h"
#include "GsUIPlayerStatDistributionPanel.generated.h"

UCLASS()
class T1PROJECT_API UGsUIPlayerStatDistributionPanel : public UGsUIPopup
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class UListView* _statDistributionListView;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* _statRemainPointText;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class UGsButton* _statDecisionButton;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class UGsButton* _statDecisionButtonDisable;
	
public:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class UImage* _imageBonusStatItem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class UGsButton* _btnBonusStatItem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* _textBlockBonusStatItem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class UImage* _imageExtend;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* _textBlockItemCount;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class UGsButton* _btnCountExtend;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class UGsButton* _btnStatReset;

private:
	int32 _bonusStatPoint;
	class FGsPlayerStatDistributionController* _playerStatHandler;

private:
	MsgHudHandleArray _hudMessageHandlerArray;
	MsgUIHandleArray _uiMessageHandlerArray;
	FDelegateHandle _longPressDelegate;
	const struct FGsSchemaBonusStatConfig* _bonusStatConfig = nullptr;
	FText _topText;
	FText _bonusStatExtendMaxText;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	void InitEntryList();
	void InvalidateEntryList();

private:
	void BindMessage();
	void UnbindMessage();

public:
	UFUNCTION()
		void OnStatDecision();
	UFUNCTION()
		void OnClickStatReset();
	UFUNCTION()
		void OnClickBonusStatItemCountExtend();

public:
	void InvalidateWidget();
	void InvalidateRemainBonusStatPoint();
	void InvalidateStatDistributionButton();

public:
	void InitializeBonusStatItemInfo();
	void InvalidateBonusStatItemInfo();

public:
	void OnInvalidWidget(const struct IGsMessageParam* inParam);
	void OnInvalidBonusStatItemCount(const struct IGsMessageParam* inParam);

public:
	void OnFinishedLongPress();
};
