// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/SpiritShot/GsUISpiritShotSceneTabBase.h"

#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/UMG/Public/Animation/UMGSequencePlayer.h"
#include "Runtime/UMG/Public/Animation/WidgetAnimation.h"
#include "Runtime/UMG/Public/Components/CanvasPanel.h"
#include "UI/UILib/Base/GsUIVFX.h"
#include "UI/UILib/Base/GsUIWidgetBase.h"
#include "UI/UIContent/Helper/GsWidgetAnimationPlayCallbackProxy.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Message/MessageParam/GsSpiritShotMessageParam.h"
#include "UI/UIContent/Window/SpiritShot/GsUISpiritShotLine.h"
#include "UI/UIContent/Window/SpiritShot/GsUISpiritShotBlueStone.h"


#include "T1Project.h"


void UGsUISpiritShotSceneTabBase::NativeOnInitialized()
{
	/** 시작 지점 링 */
	_activeRingWidgetList.Empty();
	_activeRingWidgetList.Append(TArray<TWeakObjectPtr<UUserWidget>>({
		MakeWeakObjectPtr(_spiritShotActiveRingA),
		MakeWeakObjectPtr(_spiritShotActiveRingB),
		MakeWeakObjectPtr(_spiritShotActiveRingC),
		MakeWeakObjectPtr(_spiritShotActiveRingD),
		MakeWeakObjectPtr(_spiritShotActiveRingE)
		}));

	/** 성공 효과 VFX */
	_successVfxList.Empty();
	_successVfxList.Append(TArray<TWeakObjectPtr<UGsUIVFX>>({
		MakeWeakObjectPtr(_spiritShotLevelSuccessA),
		MakeWeakObjectPtr(_spiritShotLevelSuccessB),
		MakeWeakObjectPtr(_spiritShotLevelSuccessC),
		MakeWeakObjectPtr(_spiritShotLevelSuccessD),
		MakeWeakObjectPtr(_spiritShotLevelSuccessE)
		}));

	/** 실패 효과 VFX */
	_failVfxList.Empty();
	_failVfxList.Append(TArray<TWeakObjectPtr<UGsUIVFX>>({
		MakeWeakObjectPtr(_spiritShotLevelFailA),
		MakeWeakObjectPtr(_spiritShotLevelFailB),
		MakeWeakObjectPtr(_spiritShotLevelFailC),
		MakeWeakObjectPtr(_spiritShotLevelFailD),
		MakeWeakObjectPtr(_spiritShotLevelFailE)
		}));

	/** 완료 연출 위젯 */
	_completeWidgetList.Empty();
	_completeWidgetList.Append(TArray<TWeakObjectPtr<UUserWidget>>({
		MakeWeakObjectPtr(_spiritShotLevelCompleteA),
		MakeWeakObjectPtr(_spiritShotLevelCompleteB),
		MakeWeakObjectPtr(_spiritShotLevelCompleteC),
		MakeWeakObjectPtr(_spiritShotLevelCompleteD),
		MakeWeakObjectPtr(_spiritShotLevelCompleteE)
		}));

	/** 잠식 단계 표현 라인 위젯 */
	_lineVfxList.Empty();
	_lineVfxList.Append(TArray<TWeakObjectPtr<UGsUISpiritShotLine>>({
		MakeWeakObjectPtr(_spiritShotLineA1),
		MakeWeakObjectPtr(_spiritShotLineA2),
		MakeWeakObjectPtr(_spiritShotLineA3),
		MakeWeakObjectPtr(_spiritShotLineA4),
		MakeWeakObjectPtr(_spiritShotLineA5),
		MakeWeakObjectPtr(_spiritShotLineB1),
		MakeWeakObjectPtr(_spiritShotLineB2),
		MakeWeakObjectPtr(_spiritShotLineB3),
		MakeWeakObjectPtr(_spiritShotLineB4),
		MakeWeakObjectPtr(_spiritShotLineB5),
		MakeWeakObjectPtr(_spiritShotLineC1),
		MakeWeakObjectPtr(_spiritShotLineC2),
		MakeWeakObjectPtr(_spiritShotLineC3),
		MakeWeakObjectPtr(_spiritShotLineC4),
		MakeWeakObjectPtr(_spiritShotLineC5),
		MakeWeakObjectPtr(_spiritShotLineD1),
		MakeWeakObjectPtr(_spiritShotLineD2),
		MakeWeakObjectPtr(_spiritShotLineD3),
		MakeWeakObjectPtr(_spiritShotLineD4),
		MakeWeakObjectPtr(_spiritShotLineD5),
		MakeWeakObjectPtr(_spiritShotLineE1),
		MakeWeakObjectPtr(_spiritShotLineE2),
		MakeWeakObjectPtr(_spiritShotLineE3),
		MakeWeakObjectPtr(_spiritShotLineE4),
		MakeWeakObjectPtr(_spiritShotLineE5)
		}));

	/** 전조 연출 시, 깜빡거릴 패턴 위젯 */
	_patternBlinkVfxList.Empty();
	_patternFailVfxList.Empty();
	_patternNormalVfxList.Empty();
	//_patternVfxList.Append(TArray<TWeakObjectPtr<UGsUIVFX>>({
	//	MakeWeakObjectPtr(_spiritShotPatternA),
	//	MakeWeakObjectPtr(_spiritShotPatternB),
	//	MakeWeakObjectPtr(_spiritShotPatternC),
	//	MakeWeakObjectPtr(_spiritShotPatternD),
	//	MakeWeakObjectPtr(_spiritShotPatternE),
	//	MakeWeakObjectPtr(_spiritShotPatternF),
	//	MakeWeakObjectPtr(_spiritShotPatternG),
	//	MakeWeakObjectPtr(_spiritShotPatternH)
	//	}));
	for (int32 i = 0; i < _patternCanvasPanelBlink->GetChildrenCount(); ++i)
	{
		UWidget* patternWidget = _patternCanvasPanelBlink->GetChildAt(i);
		if (nullptr == patternWidget)
		{
			GSLOG(Error, TEXT("nullptr == patternWidget at Index : %d"), i);
			continue;
		}
		_patternBlinkVfxList.Emplace(MakeWeakObjectPtr(Cast<UGsUIVFX>(patternWidget)));
	}

	for (int32 i = 0; i < _patternCanvasPanelFail->GetChildrenCount(); ++i)
	{
		UWidget* patternWidget = _patternCanvasPanelFail->GetChildAt(i);
		if (nullptr == patternWidget)
		{
			GSLOG(Error, TEXT("nullptr == patternWidget at Index : %d"), i);
			continue;
		}
		_patternFailVfxList.Emplace(MakeWeakObjectPtr(Cast<UGsUIVFX>(patternWidget)));
	}

	for (int32 i = 0; i < _patternCanvasPanelNormal->GetChildrenCount(); ++i)
	{
		UWidget* patternWidget = _patternCanvasPanelNormal->GetChildAt(i);
		if (nullptr == patternWidget)
		{
			GSLOG(Error, TEXT("nullptr == patternWidget at Index : %d"), i);
			continue;
		}
		_patternNormalVfxList.Emplace(MakeWeakObjectPtr(Cast<UUserWidget>(patternWidget)));
	}


	_sequencePlayer = NewObject<UGsUISequencePlayer>();

	for (int32 i = 0; i < _patternBlinkVfxList.Num(); ++i)
	{
		_patternIndexList.Emplace(i);
	}
	_lastPatternIndex = 0;

	GetWorld()->GetTimerManager().ClearTimer(_timerHandle);
	GetWorld()->GetTimerManager().ClearTimer(_timerHandleEffect);
	GetWorld()->GetTimerManager().ClearTimer(_timerHandleTicker);


	_patternGroupCount = 2;
	_patternGroupCountIndex = 0;

	_patternBlinkStrideInternal = 0.025f;
	_patternBlinkStrideExternal = 0.1f;

	_patternNormalDelayAfterEnchant = 0.5f;

	//_patternLinearColorList.Empty();
	//for (int32 i = 0; i < _patternVfxList.Num(); ++i)
	//{
	//	TWeakObjectPtr<UGsUIVFX> patternVfx = _patternVfxList[i];
	//	if (nullptr == patternVfx)
	//	{
	//		GSLOG(Error, TEXT("nullptr == patternVfx"));
	//		_patternLinearColorList.Emplace(FLinearColor());
	//	}
	//	else
	//	{
	//		_patternLinearColorList.Emplace(patternVfx->ColorAndOpacity);
	//	}
	//}
}

void UGsUISpiritShotSceneTabBase::NativeConstruct()
{
	Super::NativeConstruct();

	// 파란돌 효과 끝났을 때 실행될 연출 바인딩
	if (false == _blueStoneFx->OnVFXAnimationFinished.IsBoundToObject(this))
	{
		_blueStoneFx->OnVFXAnimationFinished.BindUObject(this, &UGsUISpiritShotSceneTabBase::OnFinishedPlayBlueStoneVfx);
	}

	// 판 완료 효과 끝났을 때 실행될 연출 바인딩
	if (false == _allCompleteVfx->OnVFXAnimationFinished.IsBoundToObject(this))
	{
		_allCompleteVfx->OnVFXAnimationFinished.BindUObject(this, &UGsUISpiritShotSceneTabBase::OnFinishedPlayAllCompleteVfx);
	}

	// 성공 메인 효과 끝났을 때 실행될 연출 바인딩 (티커 띄워줌 -> 딜레이 주는 것으로 변경)
	if (false == _spiritShotLevelSuccessMain->OnVFXAnimationFinished.IsBoundToObject(this))
	{
		_spiritShotLevelSuccessMain->OnVFXAnimationFinished.BindUObject(this, &UGsUISpiritShotSceneTabBase::OnFinishedPlaySuccessMainVfx);
	}

	// 실패 메인 효과 끝났을 때 실행될 연출 바인딩 (티커 띄워줌 -> 딜레이 주는 것으로 변경)
	if (false == _spiritShotLevelFailMain->OnVFXAnimationFinished.IsBoundToObject(this))
	{
		_spiritShotLevelFailMain->OnVFXAnimationFinished.BindUObject(this, &UGsUISpiritShotSceneTabBase::OnFinishedPlayFailMainVfx);
	}

	_sequencePlayer->Reset();
}

void UGsUISpiritShotSceneTabBase::NativeDestruct()
{
	_sequencePlayer->Reset();

	GetWorld()->GetTimerManager().ClearTimer(_timerHandle);
	GetWorld()->GetTimerManager().ClearTimer(_timerHandleEffect);
	GetWorld()->GetTimerManager().ClearTimer(_timerHandleTicker);

	Super::NativeDestruct();
}

void UGsUISpiritShotSceneTabBase::SetParameters(Parameters&& InParameters)
{
	_parameters = MoveTemp(InParameters);

	// 패턴 연출 및 티커 연출 초기화 (강화 연출 결과 효과는 초기화하면 안된다. 윈도우 버튼 잠금이 묶여 있기 때문)
	GetWorld()->GetTimerManager().ClearTimer(_timerHandle);
	//GetWorld()->GetTimerManager().ClearTimer(_timerHandleEffect);
	GetWorld()->GetTimerManager().ClearTimer(_timerHandleTicker);

	// 위젯 켰을 때, Shake 애니메이션 실행되면 안됨
	StopAnimation(_shakeAni);
	_sequencePlayer->Reset();

	// 단순 카테고리 변경
	if (EGsStateSpiritShotSceneState::CATEGORY_SWITCH == _parameters._sceneState)
	{
		InvalidateRingStep();
		InvalidateCompleteStep();
		ActivatePatternVfxNormal();

		// 만렙을 제외한 5의 배수이면 다 꺼 준다. 그 외의 경우는 켜 준다
		InvalidateLine(true);

		InvalidateBlueStone();
		InvalidateAllCompleteMaintain();
	}
	// 전조 연출 이후, 결과 UI 갱신 (전조 연출이 끝날때, 서버로 패킷을 전송한다)
	else if (EGsStateSpiritShotSceneState::ENCHANT_PREV_VFX == _parameters._sceneState)
	{
		PlayPrevAnimation();
	}
	// 전조 연출 없이, 바로 결과 UI 갱신
	else if (EGsStateSpiritShotSceneState::ENCHANT_RESULT_VFX == _parameters._sceneState)
	{
		PlayEnchantResultVfx();
	}
	else
	{
		GSLOG(Error, TEXT("EGsStateSpiritShotSceneState is Invalid : %d"), StaticCast<int32>(_parameters._sceneState));
	}
}

void UGsUISpiritShotSceneTabBase::PlayPrevAnimation()
{
	if (nullptr == _sequencePlayer || !_sequencePlayer->IsValidLowLevel())
	{
		GSLOG(Error, TEXT("nullptr == _sequencePlayer || !_sequencePlayer->IsValidLowLevel()"));
		return;
	}

	_sequencePlayer->Play(this, _shakeAni, FGsUISequencePlayerFinished::CreateUObject(this, &UGsUISpiritShotSceneTabBase::OnFinishedPlayEnchantPrevAni));

	FGsUIHelper::Play2DSound(SOUND_PREV_ENCHANT);

	// _shakeAni 지속시간만큼, _spiritShotPattern 위젯 깜빡거림 효과 주기 
	PlayPrevPatternBlinkVfx();
}

void UGsUISpiritShotSceneTabBase::PlayPrevPatternBlinkVfx()
{
	// 전조 연출 사용할 인덱스 섞어주기
	const int32 lastIndex = _patternIndexList.Num() - 1;
	for (int32 i = 0; i <= lastIndex; ++i)
	{
		const int32 randIndex = FMath::RandRange(0, lastIndex);
		if (i != randIndex)
		{
			_patternIndexList.Swap(i, randIndex);
		}
	}
	_lastPatternIndex = 0;


	// 전조 연출 지속 시간
	float duration = _shakeAni->GetEndTime() - _shakeAni->GetStartTime();
	
	PlayPatternVfxBlink();
}

void UGsUISpiritShotSceneTabBase::PlayEnchantResultVfx()
{
	// 라인을 먼저 갱신해 준다. 만렙일 경우에 모든 라인을 끈다
	// 단, 5레벨 단위에서 실패를 하게 되면 라인을 켜서는 안된다
	InvalidateLine(true);
	PlaySuccessFailVfx();

	// 만렙 도달시 이펙트 재생
	if (STRIDE * STEP == _parameters._level)
	{
		PlayAllCompleteVfx();

		InvalidateRingStep();
		InvalidateCompleteStep();

		InvalidateBlueStone();
	}

	// 강화 성공하여 5, 10, 15, 20, 25렙 에 도달했을 경우, 파란돌 이펙트 이후에 결과 UI 를 갱신한다
	if (_parameters._levelState == EGsStateSpiritShotLevel::LEVEL_UP &&
		_parameters._level % UGsUISpiritShotSceneTabBase::STRIDE == 0)
	{
		PlayBlueStoneVfx();
	}
	else
	{
		SetEnchantResultEffectFinished();
	}
}

void UGsUISpiritShotSceneTabBase::PlayBlueStoneVfx()
{
	if (nullptr == _blueStoneFx)
	{
		GSLOG(Error, TEXT("nullptr == _blueStoneFx"));
		return;
	}

	_blueStoneFx->ShowVFX();
	FGsUIHelper::Play2DSound(SOUND_ENCHANT_BLUE_STONE);
}

void UGsUISpiritShotSceneTabBase::PlayAllCompleteVfx()
{
	if (nullptr == _allCompleteVfx)
	{
		GSLOG(Error, TEXT("nullptr == _allCompleteVfx"));
		return;
	}

	_allCompleteVfx->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	_allCompleteVfx->HideVFX();
	_allCompleteVfx->ShowVFX();
	FGsUIHelper::Play2DSound(SOUND_ALL_COMPLETE);
}

void UGsUISpiritShotSceneTabBase::PlaySuccessFailVfx()
{
	// 강화 단계에 따른 티커 메시지
	FText tickerMessage;

	// 강화 단계 상승
	if (_parameters._levelState == EGsStateSpiritShotLevel::LEVEL_UP)
	{
		FText::FindText(TEXT("SpiritShotUIText"), TEXT("Alert_Enchant_Result_Success"), tickerMessage);
	}
	// 강화 단계 유지
	else if (_parameters._levelState == EGsStateSpiritShotLevel::LEVEL_SAME)
	{
		FText::FindText(TEXT("SpiritShotUIText"), TEXT("Alert_Enchant_Result_Fail_Keep"), tickerMessage);
	}
	// 강화 단계 하락
	else if (_parameters._levelState == EGsStateSpiritShotLevel::LEVEL_DOWN)
	{
		FText::FindText(TEXT("SpiritShotUIText"), TEXT("Alert_Enchant_Result_Fail_Drop"), tickerMessage);
	}


	// 성공 실패 효과 켜줄 Index
	int32 vfxIndex = FMath::Clamp(_parameters._level / STRIDE, 0, STEP - 1);

	// 성공
	if (_parameters._levelState == EGsStateSpiritShotLevel::LEVEL_UP)
	{
		if (!_successVfxList.IsValidIndex(vfxIndex))
		{
			GSLOG(Error, TEXT("!_successVfxList.IsValidIndex(vfxIndex), vfxIndex : %d"), vfxIndex);
			return;
		}

		TWeakObjectPtr<UGsUIVFX> successVfx = _successVfxList[vfxIndex];
		if (!successVfx.IsValid())
		{
			GSLOG(Error, TEXT("!successVfx.IsValid(), vfxIndex : %d"), vfxIndex);
			return;
		}

		successVfx->ShowVFX();

		// 만렙일 경우에는 만렙 사운드가 따로 켜질 것이므로, 성공 사운드는 켜지 않는다
		// 중앙 성공 연출 효과도 마찬가지다
		if (STRIDE * STEP != _parameters._level)
		{
			FGsUIHelper::Play2DSound(SOUND_ENCHANT_SUCCESS);
			_spiritShotLevelSuccessMain->ShowVFX();
		}

		// 일정 시간 후, 티커 연출
		GetWorld()->GetTimerManager().SetTimer(
			_timerHandleTicker,
			FTimerDelegate::CreateWeakLambda(this, [tickerMessage]() {
				FGsUIHelper::TrayTickerSpiritShotEnchant(tickerMessage, true);
				}),
			FMath::Clamp(_enchantResultTickerDelay, 0.001f, 100.0f),
			false);
	}
	// 실패
	else
	{
		if (!_failVfxList.IsValidIndex(vfxIndex))
		{
			GSLOG(Error, TEXT("!_failVfxList.IsValidIndex(vfxIndex), vfxIndex : %d"), vfxIndex);
			return;
		}

		TWeakObjectPtr<UGsUIVFX> failVfx = _failVfxList[vfxIndex];
		if (!failVfx.IsValid())
		{
			GSLOG(Error, TEXT("!failVfx.IsValid(), vfxIndex : %d"), vfxIndex);
			return;
		}

		failVfx->ShowVFX();
		_spiritShotLevelFailMain->ShowVFX();

		// 레벨 유지와 하락은 사운드 연출이 다르다
		if (_parameters._levelState == EGsStateSpiritShotLevel::LEVEL_SAME)
		{
			FGsUIHelper::Play2DSound(SOUND_ENCHANT_FAIL);
		}
		else /*if (_parameters._levelState == EGsStateSpiritShotLevel::LEVEL_DOWN)*/
		{
			FGsUIHelper::Play2DSound(SOUND_ENCHANT_FAIL_DOWN);
		}

		// 일정 시간 후, 티커 연출
		GetWorld()->GetTimerManager().SetTimer(
			_timerHandleTicker,
			FTimerDelegate::CreateWeakLambda(this, [tickerMessage]() {
				FGsUIHelper::TrayTickerSpiritShotEnchant(tickerMessage, false);
				}),
			FMath::Clamp(_enchantResultTickerDelay, 0.001f, 100.0f),
			false);
	}
}

void UGsUISpiritShotSceneTabBase::PlayPatternVfxBlink()
{
	_lastPatternIndex = (_lastPatternIndex + 1) % _patternIndexList.Num();

	int32 targetPatternIndex = _patternIndexList[_lastPatternIndex];

	if (!_patternBlinkVfxList.IsValidIndex(targetPatternIndex))
	{
		GSLOG(Error, TEXT("!_patternBlinkVfxList.IsValidIndex(targetPatternIndex, Index : %d"), targetPatternIndex);
		return;
	}

	TWeakObjectPtr<UGsUIVFX> patternVfx = _patternBlinkVfxList[targetPatternIndex];
	if (!patternVfx.IsValid())
	{
		GSLOG(Error, TEXT("!patternVfx.IsValid(), Index : %d"), targetPatternIndex);
		return;
	}
	
	patternVfx->ShowVFX();

	// 그룹내 시간으로 재생을 하거나, 그룹별 시간으로 재생을 한다 (동작 원리가 와닿지 않는다면 수정 X)
	float delay = 0.f;
	if (_patternGroupCountIndex == _patternGroupCount)
	{
		_patternGroupCountIndex = 0;
		delay = _patternBlinkStrideExternal;
	}
	else
	{
		++_patternGroupCountIndex;
	}

	// _patternBlinkStrideInternal/_patternBlinkStrideExternal 간격으로 PlayPatternVfxBlink 를 호출
	GetWorld()->GetTimerManager().SetTimer(
		_timerHandle,
		this,
		&UGsUISpiritShotSceneTabBase::PlayPatternVfxBlink,
		_patternBlinkStrideInternal,
		false,
		delay - _patternBlinkStrideInternal);
}

void UGsUISpiritShotSceneTabBase::PlayPatternVfxSuccess()
{
	// 강화 성공 시, 모든 패턴의 빛을 켜 준다
	for (int32 i = 0; i < _patternBlinkVfxList.Num(); ++i)
	{
		TWeakObjectPtr<UGsUIVFX> patternVfx = _patternBlinkVfxList[i];
		if (!patternVfx.IsValid())
		{
			GSLOG(Error, TEXT("!patternVfx.IsValid()"));
			continue;
		}

		patternVfx->ShowVFX();
	}
}

void UGsUISpiritShotSceneTabBase::PlayPatternVfxFail()
{
	// 강화 실패 시, 모든 패턴을 어둡게 만든다
	for (int32 i = 0; i < _patternFailVfxList.Num(); ++i)
	{
		TWeakObjectPtr<UGsUIVFX> patternVfx = _patternFailVfxList[i];
		if (!patternVfx.IsValid())
		{
			GSLOG(Error, TEXT("!patternVfx.IsValid()"));
			continue;
		}

		patternVfx->ShowVFX();
	}
}
 
void UGsUISpiritShotSceneTabBase::ActivatePatternVfxNormal()
{
	// 성공,실패 효과 다 꺼주고, 은은하게 빛나는 것만 켜 준다 
	for (int32 i = 0; i < _patternBlinkVfxList.Num(); ++i)
	{
		TWeakObjectPtr<UGsUIVFX> patternVfx = _patternBlinkVfxList[i];
		if (!patternVfx.IsValid())
		{
			GSLOG(Error, TEXT("!patternVfx.IsValid()"));
			continue;
		}
		patternVfx->HideVFX();
	}

	for (int32 i = 0; i < _patternFailVfxList.Num(); ++i)
	{
		TWeakObjectPtr<UGsUIVFX> patternVfx = _patternFailVfxList[i];
		if (!patternVfx.IsValid())
		{
			GSLOG(Error, TEXT("!patternVfx.IsValid()"));
			continue;
		}
		patternVfx->HideVFX();
	}

	for (int32 i = 0; i < _patternNormalVfxList.Num(); ++i)
	{
		TWeakObjectPtr<UUserWidget> patternVfx = _patternNormalVfxList[i];
		if (!patternVfx.IsValid())
		{
			GSLOG(Error, TEXT("!patternVfx.IsValid()"));
			continue;
		}
		patternVfx->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	// 혹여나 TimerHandle 이 켜져 있다면, 꺼준다
	GetWorld()->GetTimerManager().ClearTimer(_timerHandle);
}	

void UGsUISpiritShotSceneTabBase::DeactivatePatternVfxNormal()
{
	// 은은하게 빛나는 효과 다 꺼준다
	for (int32 i = 0; i < _patternNormalVfxList.Num(); ++i)
	{
		TWeakObjectPtr<UUserWidget> patternVfx = _patternNormalVfxList[i];
		if (!patternVfx.IsValid())
		{
			GSLOG(Error, TEXT("!patternVfx.IsValid()"));
			continue;
		}
		patternVfx->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUISpiritShotSceneTabBase::InvalidateBlueStone()
{
	// 만렙일 경우, 파란돌은 항상 빛나고 있을 것이다
	if (STRIDE * STEP == _parameters._level)
	{
		_blueStoneFx->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		_blueStoneFx->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUISpiritShotSceneTabBase::InvalidateRingStep()
{
	// 만렙이면 다 꺼준다
	if (STRIDE * STEP == _parameters._level)
	{
		for (int32 i = 0; i < _activeRingWidgetList.Num(); ++i)
		{
			TWeakObjectPtr<UUserWidget> uiRing = _activeRingWidgetList[i];
			if (!uiRing.IsValid())
			{
				GSLOG(Error, TEXT("!uiRing.IsValid(), i:%d"), i);
				continue;
			}

			uiRing->SetVisibility(ESlateVisibility::Collapsed);
		}
		return;
	}

	// 현재 레벨에 따라, 다음 링 스톤을 띄워 주어야 한다
	Level currentStep = 0;
	currentStep = _parameters._level / STRIDE;
	for (int32 i = 0; i < _activeRingWidgetList.Num(); ++i)
	{
		TWeakObjectPtr<UUserWidget> uiRing = _activeRingWidgetList[i];
		if (!uiRing.IsValid())
		{
			GSLOG(Error, TEXT("!uiRing.IsValid(), i:%d"), i);
			continue;
		}

		if (i == currentStep)
		{
			uiRing->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			uiRing->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UGsUISpiritShotSceneTabBase::InvalidateCompleteStep()
{
	// 현재 레벨에 따라, 완료된 녀석을 띄워 주어야 한다
	int32 completeStep = 0;
	completeStep = (_parameters._level / STRIDE) - 1;
	for (int32 i = 0; i < _completeWidgetList.Num(); ++i)
	{
		TWeakObjectPtr<UUserWidget> uiComplete = _completeWidgetList[i];
		if (!uiComplete.IsValid())
		{
			GSLOG(Error, TEXT("!uiComplete.IsValid(), i:%d"), i);
			continue;
		}

		if (i <= completeStep)
		{
			uiComplete->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			uiComplete->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UGsUISpiritShotSceneTabBase::InvalidateLine(bool InIsOn /*= true*/)
{
	// 라인을 끄라는 명령이 들어왔을 경우, 이유고하를 막론하고 무조건 끈다
	if (false == InIsOn)
	{
		InvalidateLineInternal(false);
	}
	// 5레벨 간격이면서, 강화에 성공한 시점이 아닐 경우
	// 만렙일 경우에도, 모든 라인을 끈다 (현재는 끄는 쪽으로 결정하며, Maintain 에서 따로 효과를 관리한다)
	else if (_parameters._level % STRIDE == 0 && _parameters._levelState != EGsStateSpiritShotLevel::LEVEL_UP)
	{
		InvalidateLineInternal(false);
	}
	else if (0 < _parameters._level)
	{
		Level startIndex = ((_parameters._level - 1) / STRIDE) * STEP;
		Level endIndex = ((_parameters._level - 1) / STRIDE) * STEP + (_parameters._level - 1) % STRIDE;

		// 만렙을 제외한 5의 배수일 경우, 5개가 켜져 있을 것이다
		InvalidateLineInternal(true, startIndex, endIndex);
	}
	// 레벨이 0 이면, 당연히 전부 꺼 주어야 한다
	else if (0 == _parameters._level)
	{
		InvalidateLineInternal(false);
	}
	// 이곳에 도달한다면, 뭔가 잘못된 것이다.
	else
	{
		GSLOG(Error, TEXT("Something wrong with InvalidateLine, Level : %d"), _parameters._level);
		return;
	}
}

void UGsUISpiritShotSceneTabBase::InvalidateLineInternal(bool InIsOn /*= true*/, Level InStart /* = 0*/, Level InEnd /*= 0*/)
{
	for (int32 i = 0; i < _lineVfxList.Num(); ++i)
	{
		TWeakObjectPtr<UGsUISpiritShotLine> uiLine = _lineVfxList[i];
		if (!uiLine.IsValid())
		{
			GSLOG(Error, TEXT("!uiLine.IsValid(), i:%d"), i);
			continue;
		}

		// InIsOn 이 false 면, 다 끈다
		if (!InIsOn)
		{
			uiLine->SetVisibility(ESlateVisibility::Collapsed);
			continue;
		}
		
		// InIsOn 이 true 면, 선택된 Index 만큼 킨다 (나머지는 꺼 준다)
		if (InIsOn)
		{
			if (i >= InStart && i <= InEnd)
			{
				uiLine->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				
				// 만약 마지막 라인일 경우, Last 이펙트 효과를 켜 준다 (만렙 제외)
				if (i == InEnd && InEnd < STRIDE * STEP - 1)
				{
					uiLine->SetLineEffect(true);
				}
				else
				{
					uiLine->SetLineEffect(false);
				}
			}
			else
			{
				uiLine->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}

void UGsUISpiritShotSceneTabBase::InvalidateAllCompleteMaintain()
{
	if (STRIDE * STEP == _parameters._level)
	{
		_allCompleteMaintain->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		_allCompleteMaintain->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUISpiritShotSceneTabBase::SetEnchantResultEffectFinished()
{
	// 윈도우에게 연출 효과가 끝났다는 것을 알려줌으로써, 버튼 터치를 가능하게 함
	GetWorld()->GetTimerManager().SetTimer(
		_timerHandleEffect,
		FTimerDelegate::CreateWeakLambda(this, []() {
			FGsSpiritShotMessageParamEnchantEffectFinished().Send();
			}),
		_enchantEffectFinishedDelay,
		false);

	// 연출이 끝나고 일정 시간 이후, 패턴을 다시 은은하게 빛나게 한다
	GetWorld()->GetTimerManager().SetTimer(
		_timerHandle,
		this,
		&UGsUISpiritShotSceneTabBase::ActivatePatternVfxNormal,
		_patternNormalDelayAfterEnchant,
		false);
}

void UGsUISpiritShotSceneTabBase::OnFinishedPlayEnchantPrevAni(const UWidgetAnimation* InAnimation)
{
	// 전조 연출이 끝나는 타이밍에 패턴 깜빡임도 멈춰준다
	GetWorld()->GetTimerManager().ClearTimer(_timerHandle);

	for (int32 i = 0; i < _patternBlinkVfxList.Num(); ++i)
	{
		TWeakObjectPtr<UGsUIVFX> patternVfx = _patternBlinkVfxList[i];
		if (!patternVfx.IsValid())
		{
			GSLOG(Error, TEXT("!patternVfx.IsValid()"));
			continue;
		}

		patternVfx->HideVFX();

		// HideVFX 내부에서 Visibility 를 false 로 만들어 주므로 아래 코드 필요 (패턴 이미지가 배경에 따로 들어가 있어 주석 처리) 
		//patternVfx->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		//patternVfx->SetColorAndOpacity(_patternLinearColorList[i]);
	}
}

void UGsUISpiritShotSceneTabBase::OnFinishedWatchPlayEnchantPrevAni()
{
	// 전조 연출을 다 봤다면, 실제 강화 요청 패킷을 전송한다. OnFinishedPlayEnchantPrevAni 에 넣지 않는다.
	// UI 에서 따로 Event Trigger 를 만든 놀라운 이유를 알고 있으나, 주석이 길어져서 여기 적지 않는다.
	FGsSpiritShotMessageParamReqEnchant(_parameters._categoryId, _parameters._levelId, _parameters._rateModifyCount, &_parameters._materialInfos).Send();
}

void UGsUISpiritShotSceneTabBase::OnFinishedPlayBlueStoneVfx()
{
	// 파란돌 이펙트가 왔다는 것은, 5의 배수라는 뜻이므로 라인을 전부 꺼 준다
	InvalidateLine(false);

	// 이 녀석들은 어차피 5레벨 단위에서만 갱신되는 녀석이다
	InvalidateRingStep();
	InvalidateCompleteStep();

	// 파란돌 연출이 끝났다는 것은, 새로운 체크포인트에 불이 켜진 것이다.
	FGsUIHelper::Play2DSound(SOUND_ENCHANT_STEP_CHANGE);

	// 파란돌 관련 부가 연출이 끝나면, 연출이 정말 끝난 것이다
	SetEnchantResultEffectFinished();
}

void UGsUISpiritShotSceneTabBase::OnFinishedPlayAllCompleteVfx()
{
	// 판 완료 특수 효과가 끝나면, 완료 유지 아지랑이 효과를 재생한다
	InvalidateAllCompleteMaintain();
}

void UGsUISpiritShotSceneTabBase::OnFinishedPlaySuccessMainVfx()
{
	//FGsUIHelper::TrayTickerSpiritShotEnchant(true);
}

void UGsUISpiritShotSceneTabBase::OnFinishedPlayFailMainVfx()
{
	//FGsUIHelper::TrayTickerSpiritShotEnchant(false);
}
