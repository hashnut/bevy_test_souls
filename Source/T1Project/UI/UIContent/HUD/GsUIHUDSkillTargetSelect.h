#pragma once
#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIHUD.h"
#include "UI/UILib/Define/GsUIDefine.h"

#include "Input/GsInputEventMsgBase.h"

#include "Message/GsMessageGameObject.h"

#include "GsUIHUDSkillTargetSelect.generated.h"

class UGsButton;
class UGsUISkillButtonActive;
class UGsUISkillTargetButton;

class UWidget;

struct IGsMessageParam;

UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIHUDSkillTargetSelect : public UGsUIHUD
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnNowTarget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI")
	TArray<UGsUISkillTargetButton*> _arrayPartyMemberTargetButton;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _widgetTargetNotExist;

	// 클릭 시 보일 이펙트 지정(select target skill use)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI")
	FSoftObjectPath _clickSelectTargetUseSkillVFXPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI")
	int32 _clickSelectTargetUseSkillVFXZOrder = 1000;
private:
	FGsInputEventMsgBase _inputMsg;

	// 메시지 관리
	MsgGameObjHandleArray _arrDelegateGameObjectMsg;

	// 가상 함수
protected:
	// 최초 한번 호출
	virtual void NativeOnInitialized() override;	
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	virtual void SetHUDMode(EGsUIHUDMode InMode) override;

	// message
public:
	void OnAllListChange(const IGsMessageParam*);
	void OnOneListChange(const IGsMessageParam* In_param);

	void OnLocalTargetChanged(const IGsMessageParam* In_param);
	void OnLocalTargetClear(const IGsMessageParam*);
	// ui event
public:
	UFUNCTION()
	void OnClickClose();
	UFUNCTION()
	void OnClickNowTarget();

public:
	void InitSkillTargetSelectData();
	void InitOneSkillTargetButton(int In_slotId);
	void UpdateVisible();

	void SetIsTargetExist(bool In_isExist);

	bool GetLastTarget();

	// get
public:
	UGsUISkillTargetButton* GetPartyMemberTargetButton(int In_index);
};