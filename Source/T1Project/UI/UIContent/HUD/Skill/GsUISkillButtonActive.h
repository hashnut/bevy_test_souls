// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/HUD/Skill/GsUISkillButtonBase.h"
#include "Tutorial/GsTutorialInteractionInterface.h"
#include "Input/GsInputEventMsgBase.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UI/UIContent/Common/Skill/GsSlotDragDownLogic.h"
#include "GsUISkillButtonActive.generated.h"

class UTextBlock;
class UImage;
class UGsButton;
class UGsUICoolTimeEffect;
class UWidgetSwitcher;
class UGsUIVFX;
class UGsUISkillChainEffect;
class FGsSkill;

UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUISkillButtonActive : public UGsUISkillButtonBase, public IGsTutorialInteractionInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSlot;

	// 체인 아이콘 표시
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRootChain;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherChain0;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherChain1;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherChain2;

	// 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRootButton;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSkill;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgIcon;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISkillChainEffect* _uiChainEffect;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnReserve;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _effectAuto;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICoolTimeEffect* _uiCoolTime;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockRemainTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUI")
	float _grayDarkness = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUI")
	float _grayOpacity = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUIDragDown")
	float _dragDownSensitive = 10.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUIDragDown")
	float _dragDownValue = 15.f;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelSelf;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _effectTargetSelectSlot;

	// 클릭 시 보일 이펙트 지정(select target skill use)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI")
	FSoftObjectPath _clickSelectTargetUseSkillVFXPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI")
	int32 _clickSelectTargetUseSkillVFXZOrder = 1000;

protected:
	UPROPERTY()
	TArray<UWidgetSwitcher*> _switcherChainList;
	
protected:
	FGsInputEventMsgBase _inputMsg;
	FGsSlotDragDownLogic _dragDownLogic;
	bool _bCanDragDown = false;
	bool _bAutoChanged = false;
	bool _bIsCaptureMouse = false;
	FVector2D _pressStartPosAbs;
	SkillId _firstChainSkillId = INVALID_SKILL_ID;	

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

protected:
	virtual void SetEnableButton(bool InEnable) override;
	virtual void SetCooltimeUI(float InRemainSec, float InRate) override;

public:
	virtual void SetData(const FGsSkill* InData) override;
	virtual bool UpdateData(const FGsSkill* InData) override;
	virtual void SetReserveSlot(bool InReserve) override;
	virtual void SetAutoUse(SkillId InSkillId, bool bInIsAuto) override;
	virtual void SetEmptySlot() override;
	virtual void PlayChangeStepEffect(const FGsSkill* InPrevSkill, const FGsSkill* InSkill) override;
	virtual void PlaySkillStartEffect() override;
	virtual void OnKeyboardPress() override;
	virtual void OnKeyboardRelease() override;

public:
	bool IsReserved() const;

protected:
	UFUNCTION()
	virtual void OnClickSlot();

	UFUNCTION()
	void OnPresseSkill();
	UFUNCTION()
	void OnReleaseSkill();
	UFUNCTION()
	void OnClickSkill();
	
	UFUNCTION()
	void OnReleaseAny();

	void OnMouseButtonDownEx(const FGeometry& Geometry, const FPointerEvent& MouseEvent);

	void OnSlotDragDownTranslation(const FVector2D& InTranslation);
	void OnSlotDragDownStateChanged();

	void OnFinishChainAnimation();

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "GsUISkillButton")
	void OnSkillStartEffect();

	UFUNCTION(BlueprintImplementableEvent, Category = "GsUISkillButton")
	void OnSkillChain();

	// 코스튬 스킬 위젯에만 존재
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "GsUISkillButton")
	void PlayCostumeEffect();

protected:
	void SetChainStep(int32 InCurrStep, int32 InMaxStep);
	bool CheckCanDragDown(const FGsSkill* InData) const;
	void SetAutoUseInter(bool bInIsAuto);
	void SetGrayscale(bool bInIsGray);

	void UseSkill();
//----------------------------------------------------------------------------------------------------------------------
	// Tutorial
protected:
	FGsOnTutorialInteraction OnTutorialInteraction;

public:
	virtual void SetIsTutorialInteraction(bool bIsTutorial) { }
	virtual bool IsTutorialInteraction() const { return OnTutorialInteraction.IsBound(); }
	virtual FGsOnTutorialInteraction& GetTutorialInteraction() { return OnTutorialInteraction; }

//----------------------------------------------------------------------------------------------------------------------
	// Name Tooltip
	virtual void GetNameText(OUT FText& OutNameText) override; // 자손 클래스에서 구현
	virtual ItemGrade GetSkillGrade() override; // 자손 클래스에서 구현
};
