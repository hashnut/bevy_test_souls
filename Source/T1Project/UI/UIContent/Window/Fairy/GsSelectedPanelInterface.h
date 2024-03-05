// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UI/UIContent/Tray/FairyEnchant/GsEnchantUITaskControl.h"
#include "GsSelectedPanelInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGsSelectedPanelInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class T1PROJECT_API IGsSelectedPanelInterface
{
	GENERATED_BODY()

protected:
	enum class EffectSoundResType : uint64
	{
		Sound_None = 0,
		Enchant_NotSafe_Touch,
		Enchant_NotSafe_Success,
		Enchant_NotSafe_Fail,
		Enchant_NotSafe_Guard,
		Sound_Max,
	};
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void InitializeTab(int32 index) { _tabIndex = index; }
	virtual void ReleasedTab() {}
	virtual void RefreshTab() {}

	virtual void OnUpdateItemList(TArray<uint32>& dataList) {}

	void SetSelect() { _isSelectedTab = true; OnSelected(); }
	void SetUnselect() { _isSelectedTab = false;  OnUnselected();  }

	bool IsSelectedTab() { return _isSelectedTab;  }
	void OnInitData(int32 initData) { _initData = initData; };

protected:
	virtual void OnSelected() {}
	virtual void OnUnselected() {};

	virtual void PlayEffectSound(EffectSoundResType InResType) {};
	virtual void StopEffectSound(EffectSoundResType InResType) {};

	FString GetSoundResName(EffectSoundResType InResType)
	{
		FString resultStr = "";
		switch (InResType)
		{
		case EffectSoundResType::Enchant_NotSafe_Touch:
			resultStr = "Enchant_NotSafe_Touch";
			break;
		case EffectSoundResType::Enchant_NotSafe_Success:
			resultStr = "Enchant_NotSafe_Success";
			break;
		case EffectSoundResType::Enchant_NotSafe_Fail:
			resultStr = "Enchant_NotSafe_Fail";
			break;
		case EffectSoundResType::Enchant_NotSafe_Guard:
			resultStr = "Enchant_NotSafe_Guard";
			break;
		}

		return resultStr;
	}

protected:
	int32 _tabIndex = 0;
	int32 _initData = 0;
	bool _isSelectedTab{ false };
};


class FGsTaskStateAct : public FGsTask
{
public:
	using Super = FGsTask;

protected:
	TFunction<void()> _onFnxStarted{ nullptr };
	TFunction<void()> _onFnxFinished{ nullptr };

public:
	FGsTaskStateAct(FString taskName, TFunction<void()> onFnxStarted = nullptr, TFunction<void()> onFnxFinished = nullptr)
		: FGsTask(taskName), _onFnxStarted(onFnxStarted), _onFnxFinished(onFnxFinished)
	{}

	virtual bool cond() { return true; } // Task 종료 조건, default;false

	virtual void onStart() override
	{
		if (_onFnxStarted) _onFnxStarted();
	}

	virtual void onFinish() override
	{
		if (_onFnxFinished) _onFnxFinished();
	}
};