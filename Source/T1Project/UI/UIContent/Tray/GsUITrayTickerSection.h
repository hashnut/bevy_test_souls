#pragma once

#include "UI/UILib/Base/GsUITicker.h"
#include "UI/UIContent/Tray/NotiMessage/GsUISectionMessageDefine.h"

#include "FairyEnchant/GsUITrayEnchantFairyBase.h"
#include "GsUITrayTickerSection.generated.h"

class UCanvasPanel;
class UPanelWidget;
class UCanvasPanelSlot;
class UGsUISectionMessageBase;

UCLASS()
class T1PROJECT_API UGsUITrayTickerSection : public UGsUITicker
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _panel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _maxRange;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISectionMessageBase* _section1;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISectionMessageBase* _section2;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISectionMessageBase* _section3;

	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//UGsUISectionMessageBase* _section4;

	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//UGsUISectionMessageBase* _alert; //위치만 잡음

	//UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	static TMap<EGsNotiSection, FString> _sectionTag;

	TMap<EGsNotiSection, UCanvasPanelSlot*> _unitSlots;
	TMap<EGsNotiSection, UGsUISectionMessageBase*> _sections;

	float _maxWidth{ 0.f };
	int32 _runningSection{ 0 };

	bool _closed{ false };
	FGsUITickerHandler::TrayWidgetState _prevState{FGsUITickerHandler::TrayWidgetState::None};

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
	virtual void BeginDestroy() override;
	// bInPriority가 true일 경우, 현재 출력 중인 메시지를 끊고 바로 출력한다
	void SetData(EGsNotiSection section, const FText& InText, SectionPriority priority = SectionPriority::None,
		float InPlayTime = 1.0f, TrayCondition condition = TrayCondition::TC_Active);

	void SetDelayData(EGsNotiSection section, const FText& InText, SectionPriority priority = SectionPriority::None,
		float InPlayTime = 1.0f, float DelayTime = 0.f, TrayCondition condition = TrayCondition::TC_Active); //, EGsTickerSectionAnimationType InAnimType = EGsTickerSectionAnimationType::FADE_IN_OUT);

	static FText ParseTrayText(const FText& inText, EGsNotiSection sectionType);

	virtual void SetHideVisibility() override;
	virtual void RestoreShowVisibility() override;
};

