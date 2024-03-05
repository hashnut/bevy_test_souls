#include "GsUITickerHandler.h"
#include "UI/UILib/Base/GsUITray.h"

void FGsUITickerHandler::Finalize()
{
	RemoveAll();
}

void FGsUITickerHandler::RemoveAll()
{
	while (auto iter = _listTicker.Peek())
	{
		if (iter->IsValid() && (*iter)->IsVisible() == false)
		{
			//Invisible 상태에서 삭제되면 Animation Complete가 되지 않고, Sound는 제거되지 않음
			(*iter)->RestoreShowVisibility();
			(*iter)->ClearAllPauseAnimations();
		}
		_listTicker.Pop();
	}

	_listTicker.Empty();
	_stateTicker = TrayWidgetState::None;
}

void FGsUITickerHandler::HideAll()
{
	if (_stateTicker == TrayWidgetState::AllHide) return;

	_stateTicker = TrayWidgetState::AllHide;

	FGsDequeue<TWeakObjectPtr<UGsUITray>> emptyQueue;
	Swap(emptyQueue, _listTicker);

	while (emptyQueue.Peek())
	{
		auto e = emptyQueue.Pop();
		auto condition = e->GetTrayCondition();
		
		if (condition == TrayCondition::TC_Close)
		{
			e->Close();
			continue;
		}
		else if (condition == TrayCondition::TC_Priority)
		{
			//강제 hide시에 TC_Priority는 그냥 두자
		}
		else if (condition != TrayCondition::TC_Private)
		{
			e->SetHideVisibility();
		}
		_listTicker.Push(e);
	}
}

void FGsUITickerHandler::ResetShow()
{
	if (_stateTicker != TrayWidgetState::AllHide) return;

	if (auto prvTray = _listTicker.FindByPredicate([](TWeakObjectPtr<UGsUITray> e) {
		return 	e.IsValid() && e->GetTrayCondition() == TrayCondition::TC_Private;
		}))
	{
		_stateTicker = TrayWidgetState::Private;
	}
	else
	{
		if (auto e = _listTicker.Peek())
		{
			(*e)->RestoreShowVisibility();
		}
		_stateTicker = TrayWidgetState::None;
	}
}

void FGsUITickerHandler::AddWidget(UGsUIWidgetBase* InWidget)
{
	if(InWidget->IsA<UGsUITray>())
	{
		UGsUITray* trayWidget = Cast<UGsUITray>(InWidget);
		TrayCondition condition = trayWidget->GetTrayCondition();

		UGsUITray* onWidget = nullptr;
		switch (condition)
		{
			case TrayCondition::TC_AlwaysShow: return; //아무 처리 안함
			case TrayCondition::TC_Private: //private 이 아닌 것들을 전부 hide 시키고 show
			{
				if (_stateTicker != TrayWidgetState::AllHide)
					_stateTicker = TrayWidgetState::Private;

				if (auto e = _listTicker.Peek())
				{
					if ((*e)->GetTrayCondition() != TrayCondition::TC_Private)
					{
						(*e)->SetHideVisibility();
					}
				}

				_listTicker.PushFront(trayWidget);
			}
			break;
			case TrayCondition::TC_Priority: //가장 앞에 넣고 강제 Hide 나 Private tray가 처리중이 아니라면 show
			{
				trayWidget->SetHideVisibility();
				if (auto e = _listTicker.Peek())
				{
					if ((*e)->GetTrayCondition() != TrayCondition::TC_Private)
						(*e)->SetHideVisibility();
				}
				_listTicker.PushFront(trayWidget);
			}
			break;
			case TrayCondition::TC_Active: //순서에 따라 show
			{
				trayWidget->SetHideVisibility();
				_listTicker.Push(trayWidget);
			}
			break;
			case TrayCondition::TC_Close: //강제 hide 상태에서는 close, 그 상태가 아니면 TC_Active와 동일
			{
				if (_stateTicker == TrayWidgetState::AllHide)
				{
					trayWidget->Close();
					return;
				}
				trayWidget->SetHideVisibility();
				_listTicker.Push(trayWidget);
			}
			break;
			default:
			break;
		}

		if (_stateTicker != TrayWidgetState::AllHide &&
			_stateTicker != TrayWidgetState::Private)
		{
			if (auto e = _listTicker.Peek())
			{
				(*e)->RestoreShowVisibility();
			}
		}
		else if (_stateTicker == TrayWidgetState::AllHide &&
				condition == TrayCondition::TC_Priority) // 강제 Hide시에는 TC_Priority는 켜주자
		{
			trayWidget->RestoreShowVisibility();
		}
	}
}

void FGsUITickerHandler::RemoveWidget(UGsUIWidgetBase* InWidget)
{
	if (InWidget->IsA<UGsUITray>() && (_listTicker.Num() > 0))
	{
		UGsUITray* trayWidget = Cast<UGsUITray>(InWidget);
		auto condition = trayWidget->GetTrayCondition();
		if (condition == TrayCondition::TC_AlwaysShow) return;

		if (auto finder = _listTicker.FindByPredicate([trayWidget](TWeakObjectPtr<UGsUITray> e) {
			return 	e.IsValid() && e.Get() == trayWidget;
			}))
		{
			const auto item = (*finder);
			_listTicker.Remove(item);
			if (_listTicker.IsEmpty())
			{
				if(_stateTicker != TrayWidgetState::AllHide)
					_stateTicker = TrayWidgetState::None;
				return;
			}

			if (_stateTicker == TrayWidgetState::AllHide) return;

			if (auto prvTray = _listTicker.FindByPredicate([](TWeakObjectPtr<UGsUITray> e) {
				return 	e.IsValid() && e->GetTrayCondition() == TrayCondition::TC_Private;
				}))
			{
				_stateTicker = TrayWidgetState::Private;
				return;
			}
			if (auto iter = _listTicker.Peek())
			{
				(*iter)->RestoreShowVisibility();
			}

			_stateTicker = TrayWidgetState::None;
		}
	}
}
