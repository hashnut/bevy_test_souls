
#include "GsUIPKTargetDetectionDataEntry.h"
#include "PKBook/GsPKBookData.h"
#include "../PKBook/GsPKBookHelper.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "../UI/UIControlLib/ContentWidget/GsButton.h"
#include "../Net/GsNetSendServiceWorld.h"
#include "../UTIL/GsText.h"
#include "../T1Project.h"


void UGsUIPKTargetDetectionDataEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnDetectionClear->OnClicked.AddDynamic(this, &UGsUIPKTargetDetectionDataEntry::OnClickedDetectionClear);
	_btnSwitchingState->OnClicked.AddDynamic(this, &UGsUIPKTargetDetectionDataEntry::OnClickedStateSwitch);
}

void UGsUIPKTargetDetectionDataEntry::BeginDestroy()
{
	if (_btnDetectionClear)
	{
		_btnDetectionClear->OnClicked.RemoveDynamic(this, &UGsUIPKTargetDetectionDataEntry::OnClickedDetectionClear);
		_btnDetectionClear = nullptr;
	}
	
	if (_btnSwitchingState)
	{
		_btnSwitchingState->OnClicked.RemoveDynamic(this, &UGsUIPKTargetDetectionDataEntry::OnClickedStateSwitch);
		_btnSwitchingState = nullptr;
	}
	
	Super::BeginDestroy();
}

void UGsUIPKTargetDetectionDataEntry::NativeConstruct()
{
	Super::NativeConstruct();

	_userDBId = INVALID_USER_DB_ID;

	_lastAccess = 0;
}

void UGsUIPKTargetDetectionDataEntry::SetData(const FGsPKDetectionTargetData* InData)
{
	if (nullptr == InData)
	{
		GSLOG(Error, TEXT("nullptr == InData"));
		return;
	}

	_userDBId = InData->_userDBId;
	_lastAccess = InData->_lastAccess;

	_textBlockName->SetText(FText::FromString(FGsTextUtil::CombineUserAndPrefix(InData->_targetName, InData->_worldId)));

	SetLogInState(_lastAccess);
}

void UGsUIPKTargetDetectionDataEntry::SetLogInState(const time_t InLastAccess)
{
	const int64 SWITCHER_CONNECT_INDEX = 0;
	const int64 SWITCHER_DISCONNECT_INDEX = 1;
	const int64 SWITCHER_INVALID_INDEX = 2;

	if (0 == InLastAccess)
	{	// 2022/09/23 PKT - 로그인 중..
		_switcherState->SetActiveWidgetIndex(SWITCHER_CONNECT_INDEX);
	}
	else if (0 > InLastAccess)
	{	// 2022/09/23 PKT - 알 수 없음..
		_switcherState->SetActiveWidgetIndex(SWITCHER_INVALID_INDEX);
	}
	else
	{	// 2022/09/23 PKT - Log Out
		_switcherState->SetActiveWidgetIndex(SWITCHER_DISCONNECT_INDEX);

		FText text;
		FGsPKBookHelper::TextPeriodTime(_lastAccess, text);
		// 2022/09/28 PKT - 
		_textBlockPeriod->SetText(text);
	}
}

void UGsUIPKTargetDetectionDataEntry::OnClickedStateSwitch()
{
	const int64 SWITCHER_DETECTIONTARGET_CLEAR_INDEX = 3;
	if (SWITCHER_DETECTIONTARGET_CLEAR_INDEX == _switcherState->GetActiveWidgetIndex())
	{
		SetLogInState(_lastAccess);
	}
	else
	{
		// 2022/09/23 PKT - 경계 대상 해제 버튼 활성
		_switcherState->SetActiveWidgetIndex(SWITCHER_DETECTIONTARGET_CLEAR_INDEX);
	}
}

void UGsUIPKTargetDetectionDataEntry::OnClickedDetectionClear()
{
	if (INVALID_USER_DB_ID == _userDBId)
	{
		GSLOG(Error, TEXT("INVALID_USER_DB_ID == _userDBId"));
		return;
	}
	// 2022/09/23 PKT - 경계 대상 해제
	FGsNetSendServiceWorld::Send_ReqPKBookDeleteDetectionTarget(_userDBId);
}

void UGsUIPKTargetDetectionDataEntry::Invalidate(const time_t InLastAccess)
{
	SetLogInState(InLastAccess);
}