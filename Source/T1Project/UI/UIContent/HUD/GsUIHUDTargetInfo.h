// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIHUD.h"
#include "Blueprint/UserWidget.h"
#include "Message/GsMessageUI.h"
#include "Message/GsMessageBoss.h"
#include "GsUIHUDTargetInfo.generated.h"

class FGsStatInfo;
class UGsUITargetInfoHUDBase;
struct FGsUIMapPartyMemberParam;
struct IGsMessageParam;
/**
 * HUD 상단 적 정보 관리
 */
UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIHUDTargetInfo : public UGsUIHUD
{
	GENERATED_BODY()

public:
	enum ETargetType
	{
		Normal = 0,
		Boss,
		MaxType
	};

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UPanelWidget* _panelRoot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switchTarget;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUITargetInfoHUDBase* _normalInfo;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUITargetInfoHUDBase* _bossInfo;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUITooltipTargetInfo* _tooltipUI;

protected:
	TWeakObjectPtr<class UGsGameObjectCreature> _target;

	TArray<TPair<MessageBoss, FDelegateHandle>> _bossMsgHandlerList;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	bool IsOpened() { return nullptr != _target; }

	void SetData(const struct TargetHUD* InData);	
	void SetStat(const FGsStatInfo& InStatInfo, bool bInUpdateBar);
	void UpdateStat();
	bool IsShowHPBar();
	void UpdateShowHPBar();
	
	void UpdatePartyMember(const FGsUIMapPartyMemberParam* In_param);

protected:
	void SetHP(int32 InCurrent, int32 InMax, bool bInUpdateBar);
	void SetShield(int32 InCurrent, bool bInUpdateBar);
	void UpdateHpShieldBar();

	void SwitchHPBar(ETargetType InType);	
	void SetUITooltipVisibility(bool bInIsVisible);

	UGsUITargetInfoHUDBase* GetCurrInfo() const;
	
protected:	
	UFUNCTION()
	void OnClickTooltip(class UNamedSlot* InSlot);
	
	void OnChangeAggroTarget(const IGsMessageParam* InParam);
	void OnHideAggroTarget(const IGsMessageParam* InParam);
};
