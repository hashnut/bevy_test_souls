// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Reward/GsRewardIconDataWrapper.h"
#include "GsUIRewardIconCommonListEntry.generated.h"

class UGsUIIconItemReward;

UCLASS()
class UGsRewardIconCommonListItem : public UObject
{
	GENERATED_BODY()

protected:
	// 주의: 관리 이슈 때문에 포인터를 가능한 사용하지 않음
	FGsRewardIconDataWrapper _iconData;
	bool _bCenterPopup = false;
	bool _bEnableDetailPopup = false;

public:
	void Clear();
	void SetData(FGsRewardIconDataWrapper* InIconData, bool bInCenterPopup, bool bInEnableDetailPopup);
	FGsRewardIconDataWrapper& GetIconData() { return _iconData; }
	bool IsCenterPopup() const { return _bCenterPopup; }
	bool IsEnableDetailPopup() const { return _bEnableDetailPopup; }
};

//----------------------------------------------------------------------------------------------------------------------
/**
 * 보상 ListView 사용하여 표시 시 일반 (UGsUIRewardIconListRandomListEntry 참고)
 * - 나중에 던전쪽 코드 이걸로 수정하자
 */
UCLASS()
class UGsUIRewardIconCommonListEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIIconItemReward* _icon;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

public:
	void RefreshAll();

protected:
	void InvalidateAll(UGsRewardIconCommonListItem* InListItem);

	void OnLongPressIcon(class UGsUIIconItem& InIcon);
};
