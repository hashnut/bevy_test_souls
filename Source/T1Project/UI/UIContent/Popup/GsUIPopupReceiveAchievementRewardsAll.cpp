#include "GsUIPopupReceiveAchievementRewardsAll.h"

void UGsUIPopupReceiveAchievementRewardsAll::NativeConstruct()
{
	Super::NativeConstruct();

	FText::FindText(TEXT("AchievementText"), TEXT("UI_Popup_AllReceive_Title"), _title);
	FText::FindText(TEXT("AchievementText"), TEXT("UI_Popup_AllReceive_Receive"), _btnName);
}

void UGsUIPopupReceiveAchievementRewardsAll::NativeDestruct()
{
	_delegeteClosed = nullptr;

	Super::NativeDestruct();
}

void UGsUIPopupReceiveAchievementRewardsAll::OnClickOk()
{
	if (_delegeteClosed) _delegeteClosed();
	Super::OnClickOk();
}

void UGsUIPopupReceiveAchievementRewardsAll::SetDetail(FText& title, FText& btnText)
{
	_title = title; 
	_btnName = btnText; 
}

void UGsUIPopupReceiveAchievementRewardsAll::OnInputCancel()
{
	if (_delegeteClosed) _delegeteClosed();
	Super::OnInputCancel();
}