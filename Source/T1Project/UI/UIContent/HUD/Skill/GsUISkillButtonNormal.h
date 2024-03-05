// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/HUD/Skill/GsUISkillButtonBase.h"
#include "Input/GsInputEventMsgBase.h"
#include "GsUISkillButtonNormal.generated.h"

class UGsButton;
class UImage;

/**
 * 평타 버튼
 */
UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUISkillButtonNormal : public UGsUISkillButtonBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSkill;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgIcon;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnVehicle;

	// 광클방지 로직을 위한 시간 값(초)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUISkillButtonNormal")
	float _clickLockSecond = 0.2f;

protected:
	FGsInputEventMsgBase _inputMsg;
	FDateTime _timer;
	bool _bIsMountVehicle = false; // 속도를 위해 캐싱
	// press skill msg 보냈는가
	// press skill 만 보내고 release skill을 안보냈을때 문제가 되어서 별도 flag 추가
	// https://jira.com2us.com/jira/browse/C2URWQ-5964
	bool _bSendPressKillMsg = false;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

public:
	virtual void SetData(const FGsSkill* InData) override;
	virtual bool UpdateData(const class FGsSkill* InData) override;
	virtual void SetEnableButton(bool InEnable) override;
	virtual void SetEmptySlot() override;
	virtual void OnKeyboardPress() override;
	virtual void OnKeyboardRelease() override;
	virtual void OnMountVehicle(bool bIsMounted) override;

private:
	void UpdateClickTimer();
	bool IsClickLock() const;
	void SetVehicleState(bool bIsMounted);
	bool IsVehicleMounted();

protected:
	UFUNCTION()
	void OnPressed();
	UFUNCTION()
	void OnReleased();
	UFUNCTION()
	void OnClickVehicle();

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "GsUISkillButton")
	void OnPressedEffect();
};
