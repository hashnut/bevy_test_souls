#include "GsUITrayTickerMileageSummon.h"

#include "Management/ScopeGame/GsSummonManager.h"

#include "DataSchema/Item/GsSchemaItemCommon.h"

#include "UMG/Public/Animation/WidgetAnimation.h"

void UGsUITrayTickerMileageSummon::NativeConstruct()
{
	Super::NativeConstruct();

	float endTime = _animationTray->GetEndTime();
	PlayAnimation(_animationTray, endTime, 1, EUMGSequencePlayMode::Reverse);

}


// 데이터 세팅 및 애니 시작
void UGsUITrayTickerMileageSummon::StartDataTickerMileageSummon(bool In_isUniqueReward, EGsSummonType In_summonType)
{
	// 애니 맨 앞으로 돌리기
	float endTime = _animationTray->GetEndTime();
	PlayAnimation(_animationTray, endTime, 1, EUMGSequencePlayMode::Reverse);

	// 보상 획득 텍스트 세팅
	// {0}  {1}개 획득
	FText findText;
	FText::FindText(TEXT("SummonText"), TEXT("Ticker_RewardGet"), findText);

	// 아이템 이름
	const FGsSchemaItemCommon* rewardItemTbl = GSSummon()->FindSummonMileageRewardItemTableBySummonType(In_isUniqueReward, In_summonType);
	if (rewardItemTbl == nullptr)
	{
		return;
	}
	// 아이템 갯수
	int itemCount = GSSummon()->GetSummonMileageRewardItemCountSaved();
	// 유니크일땐 1로 고정
	if (In_isUniqueReward == true)
	{
		itemCount = 1;
	}

	FText resultText = FText::Format(findText, rewardItemTbl->name, itemCount);
	_textRewardMessage = resultText;

	// 애니 시작
	PlayAnimation(_animationTray);

	FScriptDelegate animationDelegate;
	animationDelegate.BindUFunction(this, "FinishAnimationBP");
	// 애니메이션 종료 콜백 연동(닫기 처리용)
	BindToAnimationFinished(_animationTray, FWidgetAnimationDynamicEvent(animationDelegate));
}
// 연출 끝
void UGsUITrayTickerMileageSummon::FinishAnimationBP()
{
	FScriptDelegate animationDelegate;
	animationDelegate.BindUFunction(this, "FinishAnimationBP");

	UnbindFromAnimationFinished(_animationTray, FWidgetAnimationDynamicEvent(animationDelegate));

	// 닫기
	Close();
}