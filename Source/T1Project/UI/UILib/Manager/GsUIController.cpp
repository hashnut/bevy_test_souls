// Fill out your copyright notice in the Description page of Project Settings.
#include "GsUIController.h"
#include "UI/UILib/Base/GsUIWidgetBase.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "UI/UILib/Base/GsUIHUDFrame.h"
#include "Message/GsMessageUI.h"
#include "Management/GsMessageHolder.h"


void UGsUIController::BeginDestroy()
{
	// 상위 클래스(GsUIManager)에서 Finalize를 통해 해제하므로 해제할 항목은 Finalize에 나열할 것
	Super::BeginDestroy();
}

void UGsUIController::Finalize()
{
	RemoveAll();
}

void UGsUIController::RemoveAll()
{
	bIsRemovingWidgets = true;

	for (UGsUIWidgetBase* widget : _usingWidgetArray)
	{
		if (widget)
		{
			// 모두 파괴시킬 수 있게 전환
			if (widget->_bNotDestroy)
			{
				widget->SetNotDestroy(false);
			}

			widget->SetRemainStack(false);
			widget->SetStackRecoverFlag(false);
			widget->RemoveFromParent();

			widget = nullptr;
		}
	}

	_stackableArray.Empty();
	_systemStackArray.Empty();
	_usingWidgetArray.Empty();
	_cachedWidgetMap.Empty();
	_cachedWidgetArray.Empty();

	bIsRemovingWidgets = false;
}

void UGsUIController::RemoveWhenFlowChanged()
{
	bIsRemovingWidgets = true;

	RemoveUsingWidgets();
	_stackableArray.Empty();

	TArray<UGsUIWidgetBase*> removeList;
	for (UGsUIWidgetBase* widget : _cachedWidgetArray)
	{
		if (widget)
		{
			// 로딩 중 지우지 않을 것들은 배제.
			if (false == widget->IsNotDestroyedDuringLoading())
			{
				removeList.Emplace(widget);
			}
		}
	}

	// 대상 위젯 제거
	for (UGsUIWidgetBase* widget : removeList)
	{
		widget->SetRemainStack(false);
		widget->SetStackRecoverFlag(false);
		_cachedWidgetMap.Remove(widget->GetTableKey());
		_cachedWidgetArray.Remove(widget);
		widget = nullptr;
	}
	removeList.Empty();

	bIsRemovingWidgets = false;
}

void UGsUIController::RemoveWhenLevelChanged()
{
	bIsRemovingWidgets = true;

	RemoveUsingWidgets();	
	_stackableArray.Empty();
	
	// 캐시된 항목 중 사용한지 오래된 위젯 제거
	int64 cacheTimeTick = ETimespan::TicksPerMinute; // 1분(임의의 값)
	int64 currTimeTick = FDateTime::UtcNow().GetTicks();

	TArray<UGsUIWidgetBase*> removeList;
	for (UGsUIWidgetBase* widget : _cachedWidgetArray)
	{
		if (nullptr == widget)
		{
			continue;
		}

		// 로딩 중 지우지 않을 것들은 배제.
		if (false == widget->IsNotDestroyedDuringLoading())
		{
			if (cacheTimeTick < (currTimeTick - widget->_lastCloseTime.GetTicks()))
			{
				removeList.Emplace(widget);
			}
		}
	}

	for (UGsUIWidgetBase* widget : removeList)
	{
		widget->SetRemainStack(false);
		widget->SetStackRecoverFlag(false);
		_cachedWidgetMap.Remove(widget->GetTableKey());
		_cachedWidgetArray.Remove(widget);
		widget = nullptr;
	}
	removeList.Empty();

	bIsRemovingWidgets = false;
}

void UGsUIController::RemoveUsingWidgets()
{
	TArray<UGsUIWidgetBase*> removeList;
	for (UGsUIWidgetBase* widget : _usingWidgetArray)
	{
		if (widget)
		{
			// 로딩 중 지우지 않을 것들은 배제. (SystemStack 등을 유지하기 위함)
			if (false == widget->IsNotDestroyedDuringLoading())
			{
				widget->RemoveFromParent();

				removeList.Emplace(widget);
			}
		}
	}

	// 대상 위젯 제거
	for (UGsUIWidgetBase* widget : removeList)
	{
		widget->SetRemainStack(false);
		widget->SetStackRecoverFlag(false);
		_usingWidgetArray.Remove(widget);
		widget = nullptr;
	}
	removeList.Empty();
}

UGsUIWidgetBase* UGsUIController::CreateAndAdd(
	UGameInstance* InOwner, TSubclassOf<UGsUIWidgetBase> InClass, const FName& InKey)
{
	if (bIsRemovingWidgets)
	{
		FString strKey = InKey.ToString();
		GSLOG(Error, TEXT("UGsUIController::CreateAndAdd while removing widgets!!. key: %s"), *strKey);
	}

	// 캐시 된 위젯인지 체크
	UGsUIWidgetBase* widget = FindCachedWidget(InKey);
	if (widget)
	{
		// 화면에서 사용 중인지 체크
		if (_usingWidgetArray.Contains(widget))
		{
			// 중복 허용일 경우에만 새로 생성
			if (widget->CanMultipleInstance())
			{
				widget = CreateWidget<UGsUIWidgetBase>(InOwner, InClass);
				widget->_tableKey = InKey;
			}
			else
			{
				widget->Reset();
			}
		}
	}
	else
	{
		// 캐시 되지 않은 오브젝트이므로 생성
		widget = CreateWidget<UGsUIWidgetBase>(InOwner, InClass);
		if (widget)
		{
			widget->_tableKey = InKey;
			_cachedWidgetArray.Add(widget);
			_cachedWidgetMap.Add(InKey, widget);
		}
	}

	if (widget)
	{
		// 안에서 TopStack인지 검사를 한번 더 진행한다
		AddWidgetInter(widget);
	}

	return widget;
}

void UGsUIController::AddWidgetInter(UGsUIWidgetBase* InWidget)
{	
	if (InWidget->IsStackable())
	{
		if (InWidget->IsManagedBySystemStack())
		{
			PushSystemStack(InWidget);
		}
		else
		{
			PushStack(InWidget);
		}
	}
	
	AddToViewport(InWidget);
}

void UGsUIController::PushStack(UGsUIWidgetBase* InWidget)
{
	// 중복생성 안되는 위젯의 경우, 스택 맨 위의 오브젝트와 같으면 더 쌓지 않음
	if (false == InWidget->CanMultipleInstance())
	{
		if (IsTopInStack(InWidget, false))
		{
			return;
		}

		// 중복 생성 안되는 위젯이 이미 스택상에 존재하는 경우 제거함
		for (UGsUIWidgetBase* widget : _stackableArray)
		{
			if (widget == InWidget)
			{
				_stackableArray.Remove(widget);
				RemoveWidgetInter(widget);
				break;
			}
		}
	}

	// 화면을 덮는 Window타입 객체면 이전 창들을 모두 Hide
	if (InWidget->IsWindow())
	{
		TArray<UGsUIWidgetBase*> tempArray;
		for (int32 i = _stackableArray.Num() - 1; i >= 0; --i)
		{
			UGsUIWidgetBase* widget = _stackableArray[i];

			if (widget->IsWindow())
			{
				// 복구 대상 Flag를 켜줌.
				widget->SetStackRecoverFlag(true);
			}
			else
			{ 
				// 스택에 남기겠다고 켜둔 것 제외하고는 닫는다
				if (widget->IsRemainStack())
				{					
					widget->SetStackRecoverFlag(true);
				}
				else
				{
					tempArray.Emplace(widget);
				}
			}

			RemoveWidgetInter(widget);
		}

		for (UGsUIWidgetBase* widget : tempArray)
		{
			_stackableArray.Remove(widget);
		}
	}

	_stackableArray.Emplace(InWidget);
}

void UGsUIController::AddToViewport(UGsUIWidgetBase* InWidget)
{
	if (_usingWidgetArray.Contains(InWidget))
	{
		return;
	}

	_usingWidgetArray.AddUnique(InWidget);

	// 주의: UserWidget의 FullScreenWidget이 비워지는 타이밍에 따라 AddToViewport가 false가 될 수 있다.
	// 동일 프레임에 Remove/Add가 일어날 경우 발생한다.
	InWidget->AddToViewportEx();
}

void UGsUIController::RemoveWidget(UGsUIWidgetBase* InWidget)
{
	RemoveWidgetInter(InWidget);
}

void UGsUIController::PopStack(UGsUIWidgetBase* InWidget, bool bInRecoverPrevStack)
{
	if (false == InWidget->IsStackable())
	{
		return;
	}

	if (InWidget->IsManagedBySystemStack())
	{
		// 같은 객체가 스택에 존재할 경우엔 가장 위의 것을 지운다
		for (int32 i = _systemStackArray.Num() - 1; i >= 0; --i)
		{
			UGsUIWidgetBase* widget = _systemStackArray[i];
			if (widget == InWidget)
			{
				_systemStackArray.RemoveAt(i);
				break;
			}
		}
	}
	else
	{
		bool bIsExist = false;
		// 같은 객체가 스택에 존재할 경우엔 가장 위의 것을 지운다
		for (int32 i = _stackableArray.Num() - 1; i >= 0; --i)
		{
			UGsUIWidgetBase* widget = _stackableArray[i];
			if (widget == InWidget)
			{
				bIsExist = true;
				_stackableArray.RemoveAt(i);
				break;
			}
		}

		// 이미 제거 후 중복으로 들어왔을 때, 복구로직을 중복으로 타지 않기 위해 체크
		if (bIsExist)
		{
			if (bInRecoverPrevStack)
			{
				if (InWidget->IsWindow())
				{
					// 이전 스택 복구로직
					RecoverPrevStack();
				}
			}
		}
	}
}

void UGsUIController::CloseAllStack(UGsUIWidgetBase* InCloseWidget, bool bInRecoverDefaultState)
{
	if (InCloseWidget)
	{
		InCloseWidget->Close(false, false);
	}

	for (int32 i = _stackableArray.Num() - 1; i >= 0; --i)
	{
		if (UGsUIWidgetBase* widget = _stackableArray[i])
		{
			widget->SetRemainStack(false);
			widget->SetStackRecoverFlag(false);
			RemoveWidgetInter(widget);
		}
	}
	_stackableArray.Empty();

	if (bInRecoverDefaultState)
	{
		// RECOVER_TO_DEFAULT_STATE 메시지 날림
		RecoverPrevStack();
	}
}

void UGsUIController::RemoveWidgetInter(UGsUIWidgetBase* InWidget)
{
	if (InWidget)
	{
		InWidget->RemoveFromParent();
		_usingWidgetArray.Remove(InWidget);
	}
}

void UGsUIController::RecoverPrevStack()
{
	if (0 == _stackableArray.Num())
	{
		if (FGsMessageHolder* message = GMessage())
		{
			message->GetUI().SendMessage(MessageUI::RECOVER_TO_DEFAULT_STATE, nullptr);
		}
		
		return;
	}
	
	TArray<UGsUIWidgetBase*> recoverList;
	for (int32 i = _stackableArray.Num() - 1; i >= 0; --i)
	{
		UGsUIWidgetBase* widget = _stackableArray[i];
		if (widget->IsWindow())
		{			
			recoverList.Emplace(widget);
			break;
		}

		recoverList.Emplace(widget);
	}

	bool bIsSyncState = false;
	for (int32 i = recoverList.Num() - 1; i >= 0; --i)
	{
		UGsUIWidgetBase* widget = recoverList[i];

		// 주의: 일단 먼저 지우 이유는, SyncContentState가 수행되며 StateEnter 시 열어주기 위해서이다.
		// 미리 지우지 않으면 PushStack의 IsTopInStack 로직에서 리턴된다.
		_stackableArray.Remove(widget);

		if (widget->IsWindow())
		{
			if (UGsUIWindow* window = Cast<UGsUIWindow>(widget))
			{
				bIsSyncState = window->SyncContentState();
				if (false == bIsSyncState)
				{
					// 다시 넣고 바로 켜준다
					_stackableArray.Emplace(widget);
					AddToViewport(widget);
				}
			}
		}
		else
		{
			// 다시 넣고 바로 켜준다
			_stackableArray.Emplace(widget);
			AddToViewport(widget);
		}
	}

	if (false == bIsSyncState)
	{
		bool bNeedDefaultState = true;
		for (int32 i = 0; i < _stackableArray.Num(); ++i)
		{
			UGsUIWidgetBase* widget = _stackableArray[i];
			if (widget->IsWindow())
			{
				bNeedDefaultState = false;
				break;
			}
		}

		if (bNeedDefaultState)
		{
			if (FGsMessageHolder* message = GMessage())
			{
				message->GetUI().SendMessage(MessageUI::RECOVER_TO_DEFAULT_STATE, nullptr);
			}
		}
	}	
}

bool UGsUIController::IsBackDelayed()
{
	return StackPeek(true) != nullptr;
}

bool UGsUIController::Back()
{
	UGsUIWidgetBase* topWidget = StackPeek(true);
	if (nullptr != topWidget)
	{
		return topWidget->OnBack();
	}

	// 퀵메뉴 Back을 체크하기 위한 로직
	for (UGsUIWidgetBase* usingWidget : _usingWidgetArray)
	{
		if (usingWidget->IsA<UGsUIHUDFrame>())
		{
			return usingWidget->OnBack();
		}
	}

	return false;
}

UGsUIWidgetBase* UGsUIController::StackPeek(bool InCheckSystemStack) const
{
	if (InCheckSystemStack)
	{
		// 시스템 스텍을 우선 검사한다
		if (0 < _systemStackArray.Num())
		{
			return _systemStackArray.Last();
		}
	}

	if (0 == _stackableArray.Num())
	{
		return nullptr;
	}

	return _stackableArray.Last();
}

UGsUIWidgetBase* UGsUIController::GetCachedWidgetByName(const FName& InKey, bool InActiveCheck) 
{
	UGsUIWidgetBase* widget = FindCachedWidget(InKey);
	if (nullptr == widget)
	{
		return nullptr;
	}

	if (InActiveCheck)
	{		
		if (false == widget->IsActive())
		{
			return nullptr;
		}
	}

	return widget;
}

UGsUIWidgetBase* UGsUIController::CreateNotAddToViewport(UGameInstance* InOwner, TSubclassOf<UGsUIWidgetBase> InClass, 
	const FName& InKey)
{
	UGsUIWidgetBase* widget = GetCachedWidgetByName(InKey, false);
	if (nullptr == widget)
	{
		widget = CreateWidget<UGsUIWidgetBase>(InOwner, InClass);
		if (widget)
		{
			widget->_tableKey = InKey;
			_cachedWidgetArray.Add(widget);
			_cachedWidgetMap.Add(InKey, widget);
		}		
	}

	return widget;
}

bool UGsUIController::IsActiveWidget(const FName& InKey) const
{
	if (const TWeakObjectPtr<UGsUIWidgetBase>* widget = _cachedWidgetMap.Find(InKey))
	{
		if (widget->IsValid())
		{
			if (widget->Get()->IsActive())
			{
				return true;
			}
		}
	}

	return false;
}

bool UGsUIController::IsTopInStack(const UGsUIWidgetBase* InWidget, bool InCheckSystemStack) const
{
	UGsUIWidgetBase* topWidget = StackPeek(InCheckSystemStack);
	if (InWidget == topWidget)
	{
		return true;
	}

	return false;
}

UGsUIWidgetBase* UGsUIController::FindCachedWidget(const FName& InKey)
{
	if (const TWeakObjectPtr<UGsUIWidgetBase>* widget = _cachedWidgetMap.Find(InKey))
	{
		if (widget->IsValid())
		{
			return widget->Get();
		}
	}

	return nullptr;
}

void UGsUIController::PushSystemStack(UGsUIWidgetBase* InWidget)
{
	_systemStackArray.Add(InWidget);
}

void UGsUIController::PrintLog()
{
	GSLOG(Warning, TEXT("[UIController]***************************************************************************"));
	for (TPair<FName, TWeakObjectPtr<UGsUIWidgetBase>>& iter : _cachedWidgetMap)
	{
		GSLOG(Warning, TEXT("[UIController] cahgedWidget: tableKey: %s, name: %s"), 
			*(iter.Value->GetTableKey().ToString()), *(iter.Value->GetName()));
	}
	GSLOG(Warning, TEXT("[UIController]---------------------------------------------------------------------------"));
	for (UGsUIWidgetBase* widget : _usingWidgetArray)
	{
		GSLOG(Warning, TEXT("[UIController] usingWidget: tableKey: %s, name: %s"), 
			*widget->GetTableKey().ToString(), *widget->GetName());
	}
	GSLOG(Warning, TEXT("[UIController]***************************************************************************"));
}
// active중인 ui중 자동으로 close 되는거 막는 ui가 1개라도 있는지
bool UGsUIController::IsExistWidgetPreventAutoClose()
{
	bool isExist = false;
	for (UGsUIWidgetBase* iter: _usingWidgetArray)
	{
		if (nullptr == iter)
		{
			continue;
		}
		if (iter->IsActive() && iter->GetPreventAutoClose())
		{
			isExist = true;
			break;
		}
	}
	return isExist;
}