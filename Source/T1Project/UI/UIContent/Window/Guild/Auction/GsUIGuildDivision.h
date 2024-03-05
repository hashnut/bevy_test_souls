// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UI/UIContent/Window/Guild/Auction/GsUIGuildStorageBase.h"

#include "GsUIGuildDivision.generated.h"


class UGsSwitcherButton;
class UWidgetSwitcher;
class UListView;
class UGsUIGuildStorageComboBox;
/**
 * 기사단 정산 - 경매 페이지
 */
UCLASS()
class T1PROJECT_API UGsUIGuildDivision : public UGsUIGuildStorageBase
{
	GENERATED_BODY()

protected:
	// 전체 선택 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnCheckAll;

	// 분배 목록 <-> 빈 목록
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherList;

	// 분배 목록
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UListView* _listView;

	// 목록 필터
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIGuildStorageComboBox* _comboParticipant;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIGuildStorageComboBox* _comboItemCategory;

protected:
	class FGsGuildAuctionData* _cachedAuctionData = nullptr;

	TArray<ItemDBId> _divisionItemDBIdList;

	const int32 DIVISION_EXIST = 0;
	const int32 DIVISION_NONE = 1;

	bool _requestList = false;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	virtual void Show(bool InIsStackRecoverFlag) override;
	virtual void Hide() override;

	virtual void InvalidateTick() override;

	void SendRequestPacket();

protected:
	void InvalidateCheckAll();
	void InvalidateList();
	void InvalidateListEntry();
	void OnInvalidateList();

	UFUNCTION()
	void OnClickCheckAll();

	void OnListViewScrolled(float InOffsetInItems, float InDistanceRemaining);

	void OnClickComboMenuParticipant();
	void OnClickComboMenuCategory();
	void OnSelectComboBoxParticipant(int32 InIndex);
	void OnSelectComboBoxCategory(int32 InMainIndex, int32 InSubIndex);

public:
	// 멤버 가입 노티
	void OnNotifyJoinMember();
	void OnReceiveDivisionList();
	void OnChangedDivisionListCheck();
	void OnUpdatedDivisionItem();

	void OnChangedComboBox();

};