#include "GsUIIconRegionFieldBoss.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

// 보스 상태 스위칭
void UGsUIIconRegionFieldBoss::SetBossState(EGsRegionFieldBossState In_bossState)
{
	_switcherBossState->SetActiveWidgetIndex((int)In_bossState);
}
// 남은시간 세팅
void UGsUIIconRegionFieldBoss::SetRemainTime(FText In_remainTime)
{
	_textRemainTime = In_remainTime;
}

void UGsUIIconRegionFieldBoss::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnIcon->OnClicked.AddDynamic(this, &UGsUIIconRegionFieldBoss::OnClickButton);
}

void UGsUIIconRegionFieldBoss::OnClickButton()
{
	if (_callbackClick != nullptr)
	{
		_callbackClick();
	}
}