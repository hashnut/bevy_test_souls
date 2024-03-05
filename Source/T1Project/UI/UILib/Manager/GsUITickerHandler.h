#pragma once

#include "CoreMinimal.h"
#include "Util/GsDequeue.h"

class UGsUIWidgetBase;
class UGsUITray;
class IGsUIBaseHandler
{
public:
	virtual ~IGsUIBaseHandler() {}

	virtual void Initialize() {};
	virtual void Finalize() {};
	virtual void RemoveAll() = 0;
	
	virtual void HideAll() = 0;
	virtual void ResetShow() = 0;
	virtual void AddWidget(UGsUIWidgetBase* InWidget) = 0;
	virtual void RemoveWidget(UGsUIWidgetBase* InWidget) = 0;
};

class T1PROJECT_API FGsUITickerHandler : public IGsUIBaseHandler
{
public:
	enum class TrayWidgetState : uint8 { None, Private, AllHide };

private:
	//TArray<TWeakObjectPtr<UGsUITray>> _privateTray;
	FGsDequeue<TWeakObjectPtr<UGsUITray>> _listTicker;
	TrayWidgetState _stateTicker{ TrayWidgetState::None };

public:
	void Finalize() override;
	void RemoveAll() override;

	void HideAll() override;
	void ResetShow() override;
	void AddWidget(UGsUIWidgetBase* InWidget) override;
	void RemoveWidget(UGsUIWidgetBase* InWidget) override;

	TrayWidgetState GetCurrentTickerState() { return _stateTicker; }
};

