#pragma once

#include "Runtime/UMG/Public/Blueprint/IUserObjectListEntry.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "GsUISealLevelStatListEntry.generated.h"

class UGsUISealStatListEntry;
class UGsUISealStatListItem;
class UGsUIVFX;
class UObject;
class UTextBlock;
class UWidget;
class UWidgetSwitcher;

UCLASS()
class UGsUISealLevelStatListItem final : public UObject
{
	GENERATED_BODY()

public:
	bool _activated = false;
	Level _level = INVALID_LEVEL;
	TArray<TPair<StatType, StatValue>> _statInfos;
	/** 방금 활성화 됨. 이 값이 true 일 때, 활성화 이펙트를 출력함. */
	mutable bool _justActivated = false;
};

/**
 * 봉인 노드의 모든 레벨의 스탯 출력 리스트
 */
UCLASS()
class UGsUISealLevelStatListEntry final : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _activationSwitcher;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _inactivationTxt;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _activationTxt;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISealStatListEntry* _statListEntry0;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISealStatListEntry* _statListEntry1;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISealStatListEntry* _statListEntry2;

	UPROPERTY(Transient)
	TArray<UGsUISealStatListItem*> _statListitems;

	TArray<UGsUISealStatListEntry*> _statListEntries;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetOptional))
	UWidget* _activationVfx;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetOptional))
	UGsUIVFX* _activationOnceVfx;

protected:
	void NativeOnInitialized() final;
	void NativeDestruct() final;
	void NativeOnListItemObjectSet(UObject* inListItemObject) final;

public:
	void Invalidate(const UObject* InListItemObject);
};
