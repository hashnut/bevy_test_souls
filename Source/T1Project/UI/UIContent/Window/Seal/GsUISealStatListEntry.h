#pragma once

#include "Runtime/UMG/Public/Blueprint/IUserObjectListEntry.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "GsUISealStatListEntry.generated.h"

class UTextBlock;
class UWidgetSwitcher;

UCLASS()
class UGsUISealStatListItem final : public UObject
{
	GENERATED_BODY()

public:
	bool _activated = false;
	StatType _statType = StatType::NONE;
	StatValue _statValue = 0;
};

UCLASS()
class UGsUISealStatListEntry final : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _activationSwitcher;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _inactivationTxt;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _activationTxt;

protected:
	void NativeOnListItemObjectSet(UObject* InListItemObject) final;

public:
	void Invalidate(const UObject* InListItemObject);
};
