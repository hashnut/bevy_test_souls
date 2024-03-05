#pragma once

#include "EventProgress/GsEventProgressInterface.h"
#include "Message/GsMessageContents.h"
#include "UI/UIContent/Common/Icon/GsUIIconContentsEvent.h"
#include "UI/UILib/Base/GsUIHUD.h"
#include "UI/UILib/Define/GsUIDefine.h"

#include "GsUIHUDContentsEventList.generated.h"

class UGsDynamicIconSlotHelper;
class UGsUIContentsEventTooltip;
class UGsUIIconBase;
class UGsWrapBoxIconSelector;
struct IGsMessageParam;

/**
 * HUD 에 컨텐츠이벤트 목록 출력 담당
 */
UCLASS()
class UGsUIHUDContentsEventList final : public UGsUIHUD
{

	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsWrapBoxIconSelector* _iconSelector;

	UPROPERTY()
	UGsDynamicIconSlotHelper* _slotHelper;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIContentsEventTooltip* _tooltip;

private:
	TPair<MessageContentContentsEvent, FDelegateHandle> _contentsEventHandle;

	TArray<UGsUIIconContentsEvent::Parameters> _iconParameters;

	int32 _selectedIndex = -1;

	/**
	 * 오버라이드
	 */

private:
	void NativeOnInitialized() final;
	void NativeConstruct() final;
	void NativeDestruct() final;
	void SetHUDMode(EGsUIHUDMode InMode) final;

	/**
	 * 기능
	 */

private:
	void UpdateIconParameters();
	void InvalidateIcons();
	void InvalidateTooltip();
	void ResetSelectedIndex();

	/**
	 * 이벤트
	 */

private:
	void OnContentsEventGroupStateChanged(const IGsMessageParam&);

	UFUNCTION()
	void OnRefreshIcon(int32 InIndex, UGsUIIconBase* InIcon);

	void OnClickIcon(const int32 InIndex);

};
