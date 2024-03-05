// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIContent/Tray/FairyEnchant/GsUITrayEnchantFairyBase.h"
#include "UI/UIContent/Tray/GsUITrayTickerSection.h"
#include "Util/GsDequeue.h"
#include "GsUISectionMessageBase.generated.h"

class UDataTable;
class UCanvasPanelSlot;
class UCanvasPanel;
class URichTextBlock;
class UBorder;

UCLASS()
class UGsUISectionMessageBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = Appearance, meta = (RequiredAssetDataTags = "RowStructure=RichTextStyleRow"))
	class UDataTable* TextStyleSet;

	UPROPERTY(EditAnywhere, Category = Appearance)
	bool bOverrideDefaultStyle = false;

	UPROPERTY(EditAnywhere, Category = Appearance, meta = (EditCondition = bOverrideDefaultStyle))
	FTextBlockStyle DefaultTextStyleOverride;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _panel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UBorder* _border;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	URichTextBlock* _richTextBlock;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	EGsNotiSection _section;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	float _animSpeed{ 1.f };

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FSlateColor _bgColor;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	bool _useBGColor{ false };

protected:
	FGsTaskManager _taskManager;
	FGsDequeue<TWeakPtr<FGsTickerMessageDataEx>> _workQueue;
	
	FVector2D _orgPos;
	bool _initSection{ false };

public:
	class FGsTaskTrayAct : public FGsTask
	{
	public:
		using Super = FGsTask;

	protected:
		float _deltaTime = 0.f;

		TFunction<void()> _onFnxStarted{ nullptr };
		TFunction<void()> _onFnxFinished{ nullptr };

	public:
		FGsTaskTrayAct(FString taskName, float endTime, TFunction<void()> onFnxStarted = nullptr, TFunction<void()> onFnxFinished = nullptr)
			: FGsTask(taskName), _deltaTime(endTime), _onFnxStarted(onFnxStarted), _onFnxFinished(onFnxFinished)
		{}

		virtual void onStart() override
		{
			if (_onFnxStarted) _onFnxStarted();
		}

		virtual void onFinish() override
		{
			if (_onFnxFinished) _onFnxFinished();
		}

		virtual bool cond() override
		{
			return _deltaTime <= 0.f;
		}

		virtual void update(float delta)
		{
			_deltaTime -= delta;
		}
	};

public:
	TFunction<void(EGsNotiSection sector, TWeakPtr<FGsTickerMessageDataEx> data)> _onStartProgress{ nullptr };
	TFunction<void(EGsNotiSection sector, TWeakPtr<FGsTickerMessageDataEx> data)> _onCompleteProgress{ nullptr };

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void OnInitWork(TWeakPtr<FGsTickerMessageDataEx> data) {};
	virtual void OnClearWork();

public:
	virtual void InitSection(float maxRange, UCanvasPanelSlot* panelSlot);
	virtual void InitSection();
	virtual void EndSection();
	
	virtual void ForcedConditionHide(FGsUITickerHandler::TrayWidgetState curvState = FGsUITickerHandler::TrayWidgetState::None);
	virtual void ForcedConditionShow(FGsUITickerHandler::TrayWidgetState prevState = FGsUITickerHandler::TrayWidgetState::None);

public:
	EGsNotiSection GetSectionType() { return _section; }
	void SetSectionType(EGsNotiSection section) { _section = section; }
	bool IsRunningSection();
	void SetData(TWeakPtr<FGsTickerMessageDataEx> data, SectionPriority priority = SectionPriority::None);

	friend class FGsTaskTrayAct;
};
