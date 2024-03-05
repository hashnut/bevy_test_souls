// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIHUD.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageItem.h"
#include "GsUIHUDQuickSlot.generated.h"

class UGsButton;
class UGsUIReuseScrollHUD;
/**
 * HUD Äü ½½·Ô 
 */
UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIHUDQuickSlot : public UGsUIHUD
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIReuseScrollHUD* _qickSlotList;

private:
	TArray<TPair<MessageContentItem, FDelegateHandle>>	_listQuickSlotDelegate;
	TPair<MessageItem, FDelegateHandle>	_itemDelegate;

public:
	void QuickSlotInvalidateAll();
	void QuickSlotInvalidateBySlotIndex(TArray<QuickSlotId> InSlotIndexList);
	void InvalidateAutoState(TArray<QuickSlotId> InSlotIndexList);
	void OnChangeQuickSlotEditEffectState(bool InIsEditEffect);
	void ResetSelectionInfo(const QuickSlotId InSlotId);
	void ResetAllSelectionInfo();

private:
	void OnChangeQuickSlotDimmedState(struct FGsItemMessageParamBase& InParam);
	//void ResetSelction(const struct IGsMessageParam* InParamData);

private:
	void RegisterMsg();
	void RemoveMsg();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	virtual bool UpdateManagerTick(float InDeltaTime) override;

public:
	UGsUIReuseScrollHUD* GetQuickSlotSet() const { return _qickSlotList; }
};
