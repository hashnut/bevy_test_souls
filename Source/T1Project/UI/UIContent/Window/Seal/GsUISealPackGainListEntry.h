#pragma once

#include "Runtime/UMG/Public/Blueprint/IUserObjectListEntry.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "GsUISealPackGainListEntry.generated.h"

class UGsUISealStatListItem;
class UListView;
class UObject;
class UTextBlock;
class UWidgetSwitcher;

UCLASS()
class UGsUISealPackGainListItem final : public UObject
{
	GENERATED_BODY()

public:
	bool _activated = false;
	int32 _count = 0;
	TArray<TPair<StatType, StatValue>> _statInfos;
};

UCLASS()
class UGsUISealPackGainListEntry final : public UUserWidget, public IUserObjectListEntry
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
	UListView* _statListView;

	UPROPERTY(Transient)
	TArray<UGsUISealStatListItem*> _statListItems;

protected:
	void NativeOnListItemObjectSet(UObject* InListItemObject) final;

public:
	void Invalidate(const UObject* InListItemObject);
};
