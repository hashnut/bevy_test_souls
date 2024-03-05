#pragma once
#include "UI/UILib/Base/GsUITicker.h"
#include "UI/UIContent/Tray/NotiMessage/GsUISectionMessageDefine.h"
#include "GsUITrayTickerAlertSection.generated.h"

class UCanvasPanel;
class UPanelWidget;
class UGsUISectionMessageBase;

UCLASS()
class T1PROJECT_API UGsUITrayTickerAlertSection : public UGsUITicker
{
    GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _panel;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISectionMessageBase* _alert; 

	int32 _runningSection{ 0 };

	bool _closed{ false };

public:
	TArray<TSharedPtr<FGsTickerMessageDataEx>> _messageData;
	TMap <EGsNotiSection, TWeakPtr<FGsTickerMessageDataEx>> _runMessges;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void Close(bool bImmediate = false, bool bRevcoverPrevStack = true) override;
public:
	// bInPriority가 true일 경우, 현재 출력 중인 메시지를 끊고 바로 출력한다
	void SetData(const FText& InText, NoticeType noticeType, SectionPriority priority = SectionPriority::None, float InPlayTime = 1.0f, TrayCondition condition = TrayCondition::TC_Active);

	virtual void SetHideVisibility() override;
	virtual void RestoreShowVisibility() override;

public:
    EGsUIHideFlags GetHideFlagType() const override { return EGsUIHideFlags::UI_HIDE_SYSTEM_TRAY; }
	TrayCondition GetTrayCondition() const final { return TrayCondition::TC_Priority; }
};

