#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Input/GsInputEventMsgBase.h"
#include "GameObject/Skill/Checker/GsSkillCheckArray.h"

#include "GsUISkillTargetButton.generated.h"

class UGsButton;
class UGsUISkillButtonActive;

class UWidget;
class UProgressBar;

UCLASS()
class T1PROJECT_API UGsUISkillTargetButton : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClick;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textPartySlotNumber;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _widgetDespawn;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UProgressBar* _progressBarHp;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UProgressBar* _progressBarShieldFront;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UProgressBar* _progressBarShieldBack;

	// 클릭 시 보일 이펙트 지정(select target skill use)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI")
	FSoftObjectPath _clickSelectTargetUseSkillVFXPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI")
	int32 _clickSelectTargetUseSkillVFXZOrder = 1000;

private:
	int _partySlotId = -1;
	FGsInputEventMsgBase _inputMsg;

	int _currHp = 0;
	int _maxHp = 0;
	int _currShield = 0;

private:
	// 스킬 체커 등록
	// https://jira.com2us.com/jira/browse/C2URWQ-4725
	FGsSkillCheckArray _skillChecker;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	virtual bool Initialize() override;
	virtual void BeginDestroy() override;

protected:
	UFUNCTION()
	void OnClickButton();

public:
	void SetPartyTargetButtonData(int In_slotId, bool In_isSpawned, int In_currHp, int In_maxHp, int In_currShield);
	void SetPartySlotId(int In_partySlotId);
	void SetIsSpawned(bool In_isSpawned);
	void SetHP(int32 InCurrent, int32 InMax, bool bInUpdateBar);
	void SetShield(int32 InCurrent, bool bInUpdateBar);
	void UpdateHpShieldBar();
	void UpdateChangedInfo();

	int GetPartySlotId()
	{
		return _partySlotId;
	}
};