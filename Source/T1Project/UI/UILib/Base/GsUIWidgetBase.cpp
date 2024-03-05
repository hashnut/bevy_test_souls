// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIWidgetBase.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsUIMessageParam.h"

#include "Public/TimerManager.h"
#include "UI/UILib/Base/WidgetBase/GsUICurveAnimation.h"


UGsUIWidgetBase::UGsUIWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, _backupVisibility(Visibility)
{

}

void UGsUIWidgetBase::NativeConstruct()
{
	_bIsDestructed = false;

	// 씬로딩 중 RemoveFromParent가 자동으로 불리며 삭제되는 상황 방지
	SetNotDestroy(IsNotDestroyedDuringLoading());

	// 현재 상황을 가져와 반영한다
	if (UGsUIManager* uiMgr = GUI())
	{
		SetHide(uiMgr->GetHideFlagsCurrent());

		if (IsEnableManagerTick())
		{
			_managerTickHandle = uiMgr->OnManagerTick.AddUObject(this, &UGsUIWidgetBase::OnManagerTick);
		}
	}

	_hideMessageHandle = GMessage()->GetUI().AddUObject(MessageUI::HIDE_UI, this, &UGsUIWidgetBase::OnHide);
	_hitTestInvisibleHandle = GMessage()->GetUI().AddUObject(MessageUI::SET_HITTESTINVISIBLE, this, &UGsUIWidgetBase::OnInterectionVisibleFlag);
	
	// 커브 애니메이션이 있으면 바인딩 및 재생
	for (FGsUICurveAnimation& curve : _curveAnimationList)
	{
		if (false == curve.OnCurveAnimationFinished.IsBound())
		{
			curve.OnCurveAnimationFinished.BindUObject(this, &UGsUIWidgetBase::OnCurveAnimationFinished);
		}
	}
	PlayCurveAnimation(EGsUIAnimationTriggerType::OPEN);

	Super::NativeConstruct();
}

void UGsUIWidgetBase::NativeDestruct()
{
	GMessage()->GetUI().Remove(_hideMessageHandle);
	GMessage()->GetUI().Remove(_hitTestInvisibleHandle);

	// 데이터를 정상화 하고 뷰포트에서 제거한다
	if (_bIsHide)
	{
		SetVisibility(_backupVisibility);
		_bIsHide = false;
	}

	// 커브애니메이션의 owner 정보 클리어
	for (FGsUICurveAnimation& curveAnim : _curveAnimationList)
	{
		curveAnim.Clear();
	}

	_lastCloseTime = FDateTime::UtcNow();

	if (IsEnableManagerTick())
	{
		GUI()->OnManagerTick.Remove(_managerTickHandle);
	}

	// 닫기 애니메이션 시작 시 걸었던 HitTestInvisible을 풀어주어 하위 위젯의 인터렉션이 가능하게 함
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	_bIsDestructed = true;

	Super::NativeDestruct();
}

void UGsUIWidgetBase::Reset()
{
	// 이미 사용 중인데 또 부를 경우에 들어온다. 
	// 애니메이션 종료 후 RemoveFromParent 로직을 타는 것을 방지한다.
	for (FGsUICurveAnimation& curveAnim : _curveAnimationList)
	{
		curveAnim.Clear();
	}

	_lastCloseTime = FDateTime::UtcNow();
}

bool UGsUIWidgetBase::AddToViewportEx()
{
	// 일반 AddToViewport 시 ZOrder + 10된 값이 들어간다. UUserWidget::AddToScreen 참고.
	// 같은 ZOrder이면 이미 존재하는 것 다음에 Insert 된다. SOverlay::AddSlot 참고.
	// Window < Popup < Tray 특징별 뎁스 보장을 위해 GetManagedZOrder() 를 통해 타입별 기본값을 달리한다.	
	if (IsInViewport())
	{
		return false;
	}

	AddToViewport(GetManagedZOrder());

	return true;
}

void UGsUIWidgetBase::RemoveFromParent()
{
	if (false == _bNotDestroy)
	{
		Super::RemoveFromParent();
	}
}

void UGsUIWidgetBase::StopAllAnimationsEx()
{
	StopAllAnimations();
	//StoppedSequencePlayers.Empty();
}

void UGsUIWidgetBase::SetNotDestroy(bool bNotDestroy)
{
	_bNotDestroy = bNotDestroy;
}

void UGsUIWidgetBase::Close(bool bImmediate, bool bRecoverPrevStack)
{
	// Close 즉시 Stack에서 제거. Viewport에서는 닫기 애니메이션 재생 후 CloseInternal에서 제거.
	if (UGsUIManager* uimgr = GUI())
	{
		// 내부에서 Stack에 있는지 검사하므로, 여기서는 생략한다.
		uimgr->PopStack(this, bRecoverPrevStack);
	}

	if (bImmediate)
	{
		CloseInternal();
	}
	else
	{
		// 이미 NativeDestruct가 불린 후에 들어와서 Visibility가 꼬이는 문제 방지
		if (false == _bIsDestructed)
		{
			// 종료 애니메이션이 재생 중인데 들어온 경우 끊고 바로 닫기
			if (IsPlayingCurveAnimation(EGsUIAnimationTriggerType::CLOSE))
			{
				Reset(); // 애니메이션 		
				CloseInternal();
			}
			else
			{
				// 종료 커브 애니메이션 호출. 없으면 바로 닫기
				if (PlayCurveAnimation(EGsUIAnimationTriggerType::CLOSE))
				{
					// 자신과 하위 위젯의 모든 인터렉션을 막음
					SetVisibility(ESlateVisibility::HitTestInvisible);
				}
				else
				{
					CloseInternal();
				}
			}
		}
	}
}

void UGsUIWidgetBase::CloseInternal()
{
	if (UGsUIManager* uiManager = GUI())
	{
		// 명시적으로 Close를 통해 닫으면 꺼준다
		if (IsNotDestroyedDuringLoading())
		{
			SetNotDestroy(false);
		}

		SetRemainStack(false);
		SetStackRecoverFlag(false);
		uiManager->RemoveWidget(this);
	}
}

UGsUIManager* UGsUIWidgetBase::GetUIManager()
{
	return GUI();
}

int32 UGsUIWidgetBase::GetManagedZOrder() const
{
	return (_bNotUseManagedZOrder) ? _addZOrder : GetManagedDefaultZOrder() + _addZOrder;
}

void UGsUIWidgetBase::OnHide(const IGsMessageParam* InParam)
{
	const FGsUIMsgParamUInt* param = InParam->Cast<const FGsUIMsgParamUInt>();
	if (param)
	{
		SetHide(static_cast<EGsUIHideFlags>(param->_data));
	}
}

void UGsUIWidgetBase::OnInterectionVisibleFlag(const struct IGsMessageParam* InParam)
{
	const FGsPrimitiveInt32* param = (nullptr != InParam) ? StaticCast<const FGsPrimitiveInt32*>(InParam) : nullptr;
	if (nullptr == param)
	{
		this->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		bool isActive = StaticCast<bool>(param->_data);
		if (isActive)
		{
			this->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}
}

bool UGsUIWidgetBase::IsActive() const
{
	// 위젯 컴포넌트에서 사용되는 위젯의 경우, GetIsVisible로 체크 불가
	if (EGsUIHideFlags::UI_HIDE_WIDGETCOMPONENT == GetHideFlagType())
	{
		return (ESlateVisibility::Hidden != GetVisibility()) ? true : false;
	}

	// 뷰포트에 포함되어 있는가
	return GetIsVisible();
}

void UGsUIWidgetBase::SetHide(EGsUIHideFlags InHideFlags)
{
	if (EnumHasAnyFlags(InHideFlags, GetHideFlagType()))
	{
		// 활성화중인 UI에만 적용
		if (IsActive())
		{
			// 중복 호출 상황 방지
			if (ESlateVisibility::Hidden != Visibility)
			{
				_backupVisibility = Visibility;

				// 커브 애니메이션 중에 들어올 경우에 대한 예외처리 
				if (ESlateVisibility::HitTestInvisible == Visibility)
				{
					if (IsPlayingCurveAnimation(EGsUIAnimationTriggerType::CLOSE))
					{
						_backupVisibility = ESlateVisibility::SelfHitTestInvisible;
					}
				}

				SetVisibility(ESlateVisibility::Hidden);
				_bIsHide = true;
			}
		}
	}
	else
	{
		// 이전에 Hide처리한 오브젝트에만 적용
		if (_bIsHide)
		{
			SetVisibility(_backupVisibility);
			_bIsHide = false;
		}
	}

	//GSLOG(Error, TEXT("---------Name: %s, Backup: %d"), *GetName(), (int)BackupVisibility);
}

void UGsUIWidgetBase::OnCurveAnimationFinished(EGsUIAnimationTriggerType InTriggerType)
{
	if (EGsUIAnimationTriggerType::CLOSE == InTriggerType)
	{
		CloseInternal();
	}
}

void UGsUIWidgetBase::SetCurveAnimationTarget(EGsUIAnimationTriggerType InTriggerType, class UWidget* InWidget)
{
	for (FGsUICurveAnimation& curveAnim : _curveAnimationList)
	{
		if (InTriggerType == curveAnim._triggerType)
		{
			curveAnim.SetTarget(InWidget);
		}
	}
}

void UGsUIWidgetBase::SetCurveAnimationTargetByIndex(int32 InIndex, class UWidget* InWidget)
{
	if (0 > InIndex || InIndex >= _curveAnimationList.Num())
	{
		return;
	}

	FGsUICurveAnimation& curveAnim = _curveAnimationList[InIndex];
	curveAnim.SetTarget(InWidget);
}

bool UGsUIWidgetBase::PlayCurveAnimation(EGsUIAnimationTriggerType InTriggerType)
{
	bool bExist = false;
	for (FGsUICurveAnimation& curveAnim : _curveAnimationList)
	{
		if (InTriggerType == curveAnim._triggerType)
		{
			bExist = true;

			curveAnim.Reset();
			curveAnim.Play(this);
		}
		else
		{
			curveAnim.Reset();
		}
	}

	return bExist;
}

bool UGsUIWidgetBase::PlayCurveAnimationByIndex(int32 InIndex)
{
	if (0 > InIndex || InIndex >= _curveAnimationList.Num())
	{
		return false;
	}

	FGsUICurveAnimation& curveAnim = _curveAnimationList[InIndex];
	curveAnim.Reset();
	curveAnim.Play(this);

	return true;
}

bool UGsUIWidgetBase::IsPlayingCurveAnimation(EGsUIAnimationTriggerType InTriggerType)
{
	for (FGsUICurveAnimation& curveAnim : _curveAnimationList)
	{
		if (InTriggerType == curveAnim._triggerType)
		{
			if (curveAnim.IsPlaying())
			{
				return true;
			}
		}
	}

	return false;
}
