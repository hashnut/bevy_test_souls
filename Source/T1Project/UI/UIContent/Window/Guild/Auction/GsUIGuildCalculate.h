// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UI/UIContent/Window/Guild/Auction/GsUIGuildStorageBase.h"

#include "GsUIGuildCalculate.generated.h"


class UGsUIGuildStorageComboBox;
struct FGsGuildDivisionItem;
/**
 * 기사단 정산 - 결과 페이지
 */
UCLASS()
class T1PROJECT_API UGsUIGuildCalculate : public UGsUIGuildStorageBase
{
	GENERATED_BODY()

protected:
	//  목록 <-> 빈 목록
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherList;

	// 결과 목록
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UListView* _listView;

	// 목록 필터
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIGuildStorageComboBox* _comboParticipant;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIGuildStorageComboBox* _comboItemCategory;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIGuildStorageComboBox* _comboBid;

protected:
	class FGsGuildAuctionData* _cachedAuctionData = nullptr;

	TArray<GuildAuctionId> _calculateIdList;

	bool _requestList = false;

	FText _filterParticipantText;
	FText _filterCategoryText;
	FText _filterBidText;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	virtual void Show(bool InIsStackRecoverFlag) override;
	virtual void Hide() override;

protected:
	void SendRequestPacket();

	void InvalidateList();

	void OnListViewScrolled(float InOffsetInItems, float InDistanceRemaining);

	void OnClickComboMenuParticipant();
	void OnClickComboMenuCategory();
	void OnClickComboMenuBid();

	void OnSelectComboBoxParticipant(int32 InIndex);
	void OnSelectComboBoxCategory(int32 InMainIndex, int32 InSubIndex);
	void OnSelectComboBoxBid(int32 InIndex);

public:
	void OnReceiveCalculateList();
};
