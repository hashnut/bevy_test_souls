// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIHUD.h"
#include "Message/GsMessageUI.h"
#include "Message/GsMessageGameObject.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Runtime/DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"
#include "GsUIHUDSkill.generated.h"

class UGsUISkillSlotSetHUD;
class UGsUISkillButtonNormal;
class UGsUISkillButtonBase;
class UGsUINPCFunctionSlotSetHUD;
class UWidgetAnimation;
class FGsInputEventMsgBase;

/**
 * HUD 우하단 스킬 버튼들 UI 관리
 * - 애니메이션 있음에 유의
 */
UCLASS()
class T1PROJECT_API UGsUIHUDSkill : public UGsUIHUD
{
	GENERATED_BODY()

public:
	using DefSkillSlotList = TArray<UGsUISkillButtonBase*>;
	
protected:
	// 평타 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISkillButtonNormal* _btnNormal;
	// 스킬 리스트
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISkillSlotSetHUD* _skillSlotSet;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUINPCFunctionSlotSetHUD* _npcFunctionSet;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* _aniSkillToNPC;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* _aniSkillIdle;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* _aniNPCToSkill;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* _aniNPCIdle;

protected:
	MsgUIHandleArray _msgUIHandleList;
	MsgGameObjHandleArray _msgGameObjectHandleList;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// IGsInvalidateUIInterface override
	virtual void InvalidateAllInternal() override;

public:
	virtual bool UpdateManagerTick(float InDeltaTime) override;

private:
	void BindMessages();
	void UnbindMessages();

protected:
	void OnReserveSkillSlot(const IGsMessageParam* InParam);
	void OnCancelReserveSkillSlot(const IGsMessageParam* InParam);
	void OnRequestSlotUpdate(const IGsMessageParam* InParam);

	void OnChangeSkillSet(const IGsMessageParam* InParam);
	void OnChangeSkillSlot(const IGsMessageParam* InParam);
	void OnChangeSkillAuto(const IGsMessageParam* InParam);
	void OnChangeChainSkill(const IGsMessageParam* InParam);
	void OnSetStartBuffSkillSlot(const IGsMessageParam* InParam);
	void OnSkillCoolDown(const IGsMessageParam* InParam);
	void OnSkillStart(const IGsMessageParam* InParam);
	void OnAddAbnormality(const IGsMessageParam* InParam);
	void OnRemoveAbnormality(const IGsMessageParam* InParam);

public:
	void OnKeyboardEvent(EGsKeyMappingType InKeyMappingType, EInputEvent InEventType);

protected:
	void FindSkillSlots(int32 InSlotId, OUT DefSkillSlotList& OutList);
	void FindSkillSlotsBySkillId(SkillId InSkillId, OUT DefSkillSlotList& OutList);
	void UpdateAllSkillSlots();
	void UpdateAllNPCFunctionSlots();

public:
	UGsUISkillSlotSetHUD* GetSkillSlotSet() const { return _skillSlotSet; }
	UGsUINPCFunctionSlotSetHUD* GetNpcSlotSet() const { return _npcFunctionSet; }
};
