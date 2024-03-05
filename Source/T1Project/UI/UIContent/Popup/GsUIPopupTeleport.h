// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Shared/SharedInclude/SharedPrimitiveTypes.h"
#include "Currency/GsCostPackage.h"
#include "Quest/GsQuestData.h"
#include "GsUIPopupTeleport.generated.h"

class UGsUICurrencySlot;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIPopupTeleport : public UGsUIPopup
{
	GENERATED_BODY()

protected:	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUICurrencySlot* _currencyTeleportGold;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUICurrencySlot* _currencyLocalGold;	
	
	TSharedRef<FGsCostPackage> _costPackage = MakeShared<FGsCostPackage>();
	QuestId _questId;
	QuestKey _questKey;
	QuestObjectiveId _objectiveId;
	bool _isAccpeted = true;

	// 이벤트 종료 호출 할지
	bool _isCallEventProcessFinish = true;

protected:
	virtual void NativeDestruct() override;

protected:
	virtual void OnInputOk() override;
	virtual void OnInputCancel() override;

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickOk();
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickCancel();

public:
	void SetData(TSharedRef<FGsCostPackage> InCostPackage, QuestKey inQuestKey, QuestObjectiveId inObjectiveId, bool inIsAccpeted = true);
};
