#include "GsUIDaggerBlood.h"
#include "GsUIBloodDot.h"

#include "WidgetSwitcher.h"
#include "Image.h"

void UGsUIDaggerBlood::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_bloodState.Emplace(DotState::None, _baseBloodDot);
	_bloodState.Emplace(DotState::Get, _getBloodDot);
	_bloodState.Emplace(DotState::GetLoop, _getLoopBloodDot);
	_bloodState.Emplace(DotState::CompleteLoop, _completeBloodDot);
	_bloodState.Emplace(DotState::UsePlay, _useBloodDot);

	auto nxtState = DotState::GetLoop;
	_getBloodDot->OnCompleteAnimation().AddLambda([this, nxtState](){
		SetDaggerBloodStat(nxtState, true);
	});

	nxtState = DotState::None;
	_useBloodDot->OnCompleteAnimation().AddLambda([this, nxtState]() {
		SetDaggerBloodStat(nxtState, true);

		OnUsedDaggerBlood.Broadcast();
	});
}

void UGsUIDaggerBlood::NativeConstruct()
{
	Super::NativeConstruct();

	_curState = DotState::None;
	_nxtState = DotState::None;
	_bloodDropSwitcher->SetActiveWidget(_bloodState[DotState::None]);
}

void UGsUIDaggerBlood::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGsUIDaggerBlood::SetDaggerBloodStat(DotState state, bool animEnd)
{
	if (_curState == state) return;

	auto setState = state;
	if (_nxtState != DotState::None && state == DotState::GetLoop && _curState != DotState::Get)
	{
		setState = _curState;
	}

	if (state == DotState::Get) _nxtState = DotState::GetLoop;
	
	if (animEnd) _nxtState = DotState::None;

	_curState = setState;

	_bloodDropSwitcher->SetActiveWidget(_bloodState[setState]);
	if (auto _bloodDot = Cast<UGsUIBloodDot>(_bloodState[setState]))
		_bloodDot->UpdateWidget();
}
