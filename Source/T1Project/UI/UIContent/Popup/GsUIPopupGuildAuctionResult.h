// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UI/UILib/Base/GsUIPopup.h"

#include "Shared/Client/SharedEnums/SharedGuildEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "GsUIPopupGuildAuctionResult.generated.h"


class UTextBlock;
class UGsButton;
class UScrollBox;
class UGsDynamicPanelSlotHelper;
class UUserWidget;
struct FGsGuildDivisionItem;
struct FGsGuildAuctionItem;

/**
 * 기사단 경매 정산 팝업
 */
UCLASS()
class T1PROJECT_API UGsUIPopupGuildAuctionResult : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	enum class EGsAuctionResultOpenType
	{
		NONE,
		DIVISION,
		AUCTION,
		CALCULATE,
	};

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

	// 유저 목록
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBoxUser;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _scrollBoxHelper;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryWidgetClass;

	// 기사단 경매 수수료
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockCommission;

	// 개별 정산금
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockResult;


	/**
	 * 위젯 사용 텍스트
	 * 정산 : GuildAuctionUIText - Auction_Calculate_Charge
	 * 직급 : GuildAuctionUIText - Charge_Popup_position
	 * 대상 : GuildAuctionUIText - Charge_Popup_Character
	 * 기사단 경매 수수료 : GuildAuctionUIText - Charge_Poup_Charge_Description
	 * 개별 정산금 : GuildAuctionUIText - Charge_Popup_Charge_Expectation
	 * 안내 문구 : GuildAuctionUIText - Charge_Popup_Charge_Expectation_Description
	 */

protected:
	TArray<TPair<GuildMemberGradeType, FString>> _userInfoList;

	EGsAuctionResultOpenType _openType = EGsAuctionResultOpenType::NONE;
	ItemDBId _itemDBId = INVALID_ITEM_DB_ID;
	GuildAuctionId _auctionId = INVALID_GUILD_AUCTION_ID;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	
	virtual void OnInputOk() override;
	virtual void OnInputCancel() override;

protected:
	UFUNCTION()
	void OnClickClose();

	UFUNCTION()
	void OnRefreshEntryUser(int32 InIndex, class UWidget* InEntry);

public:
	void SetDivision(const FGsGuildDivisionItem* InDivisionItem);
	void SetAuction(const FGsGuildAuctionItem* InAuctionItem);
	void SetCalculate(const FGsGuildDivisionItem* InCalculateItem);

	void InvalidateList();
};
