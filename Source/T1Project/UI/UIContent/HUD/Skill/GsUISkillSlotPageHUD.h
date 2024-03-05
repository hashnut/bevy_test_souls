// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUISkillSlotPageHUD.generated.h"

class UPanelWidget;
class UGsUISkillButtonBase;

/**
 * HUD 스킬 한 페이지
 */
UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUISkillSlotPageHUD : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TArray<UGsUISkillButtonBase*> _slotList;

	TMap<int32, TWeakObjectPtr<UGsUISkillButtonBase>> _slotMap; // key: SkillSlotId

protected:
	int32 _pageIndex = 0; // 스크롤 리스트상의 index, 0부터 시작하는 값

	// True일 경우, 무한스크롤 연출을위해 추가된 페이지. 메시지를 보내지 않도록 막을 것.
	bool _bIsFakePage = false;

protected:
	virtual void NativeOnInitialized() override;

public:
	void InitializePage(int32 InPageIndex);
	void SetIsFakePage(bool bIsFakePage);
	void UpdateManagerTick(float InDeltaTime);

	UGsUISkillButtonBase* GetSlot(int32 InSlotId) const;
	UGsUISkillButtonBase* GetSlotBySkillId(SkillId InSkillId) const;
};
