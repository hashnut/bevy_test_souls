#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIHUD.h"
#include "GsUIHUDPlayableEventSkill.generated.h"

/*
* hud 플레이어블 이벤트: 우하단 스킬 버튼들 ui
*/
class UGsButton;
class UImage;

UCLASS()
class UGsUIHUDPlayableEventSkill : public UGsUIHUD
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSkill;

protected:
	virtual void NativeOnInitialized() override;

public:
	UFUNCTION()
	void OnClickedButton();
	// 스킬 메시지 전송
	void SendSkillMessage(int In_slotId);
};