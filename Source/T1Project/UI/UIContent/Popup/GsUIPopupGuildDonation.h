// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageSystem.h"
#include "Message/GsMessageUserInfo.h"
#include "GsUIPopupGuildDonation.generated.h"

class UTextBlock;
class UGsButton;
class UGsUICurrencyButton;

/**
 * 길드 권한 창. 권한 변경창이 이 클래스를 상속함.
 */
UCLASS()
class T1PROJECT_API UGsUIPopupGuildDonation : public UGsUIPopup
{
	GENERATED_BODY()

public:
	using MsgHandleGuild = TPair<MessageContentGuild, FDelegateHandle>;	
	using MsgHandleUserInfo = TPair<MessageUserInfo, FDelegateHandle>;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockNormalGuildAsset;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockNormalGuildCoin;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockNormalContribution;	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICurrencyButton* _btnNormal;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockAdvancedGuildAsset;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockAdvancedGuildCoin;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockAdvancedContribution;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICurrencyButton* _btnAdvanced;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockRemainCount;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

protected:	
	CurrencyType _normalType;
	Currency _normalAmount = 0;
	int32 _donationMaxCount = 0;
	CurrencyType _advancedType;
	Currency _advancedAmount = 0;

	MsgHandleGuild _msgHandleGuild;
	MsgSystemHandle _msgHandleSystem;
	MsgHandleUserInfo _msgHandleUserInfo;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	virtual void OnInputCancel() override;

protected:
	void InvalidateCount();

	bool CheckCountNormal();
	bool CheckCountAdvanced();

protected:
	void OnReceiveDonationResult(const struct IGsMessageParam*);
	void OnResetTimeContents(const struct IGsMessageParam*);
	void OnChangeCurrency(uint64 InCurrncyType);

	UFUNCTION()
	void OnClickNormal();
	
	UFUNCTION()
	void OnClickAdvanced();

	UFUNCTION()
	void OnClickClose();

protected:
	void GetRewardAmount(const struct FGsSchemaRewardData* InReward, 
		OUT Currency& OutGuildAssetAmount, OUT int32& OutGuildCoinAmount);
};
