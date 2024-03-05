// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWidgetBase.h"
#include "UI/UILib/Base/GsUITray.h"
#include "GsEnchantUITaskControl.h"
#include "GsUITrayEnchantFairyBase.generated.h"

class UGsButton;

UCLASS()
class T1PROJECT_API UGsUITrayEnchantFairyBase : public UGsUITray
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnConfirm;

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	class EffectInitData
	{
	public:
		EffectInitData() = default;
	};

	virtual void InitializeData(EffectInitData* initParam) { _isFinished = false; };
	bool IsFinished() { return _isFinished; }
	bool IsPlaying() { return _isPlaying; }
	//virtual bool IsStackable() const final { return true; }
	virtual TrayCondition GetTrayCondition() const override { return TrayCondition::TC_Priority; }

protected:
	bool _isFinished{ false };
	bool _isPlaying{ true };
};

#pragma region FGsTaskTrayUI
class FGsTaskTrayUI : public FGsTask
{
public:
	FString _trayUIName;

	TWeakObjectPtr<UGsUITrayEnchantFairyBase> _widget{ nullptr };
	TFunction<void()> _onFnxStarted{ nullptr };
	TFunction<void()> _onFnxFinished{ nullptr };

	FGsTaskTrayUI(FString taskName, FString trayUIName, TFunction<void()> onFnxStarted = nullptr, TFunction<void()> onFnxFinished = nullptr)
		: FGsTask(taskName), _trayUIName(trayUIName), _onFnxStarted(onFnxStarted), _onFnxFinished(onFnxFinished) {}

	void onStart() override;
	void onFinish() override;
	bool cond() override;
};

#pragma endregion