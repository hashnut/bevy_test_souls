#include "GsUITrayTickerAchievementComplete.h"
#include "WidgetAnimation.h"
#include "TextBlock.h"

void UGsUITrayTickerAchievementComplete::SetData(const FInitData& InInitData)
{
	OnInitWork(InInitData);
}

void UGsUITrayTickerAchievementComplete::OnInitWork(const FInitData& data)
{
	static float deltaTime = _animationStart->GetEndTime();

	auto task = MakeShared<FGsTaskTrayAct>(TEXT("AchievementTicker"), deltaTime, [this, data]() {

			FText titleText;

			if (data._eType == AchievementCompleteType::Achievement)
			{
				FText::FindText(TEXT("AchievementText"), TEXT("completeTicker_achieveTitle_1"), titleText);
			}
			else if(data._eType == AchievementCompleteType::AchievementAll)
			{
				FText::FindText(TEXT("AchievementText"), TEXT("UI_Ticker_AllReceive_Title"), titleText);
			}

			_textTitle->SetVisibility(data._eType == AchievementCompleteType::Theme ?
				ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);

			_textTitle->SetText(titleText);
			_name = data._name;
			_desc = data._desc;
			PlayAnimation(_animationStart);
		}, [this]() {
			
			_textTitle->SetVisibility(ESlateVisibility::Collapsed);
			_name = FText::GetEmpty();
			_desc = FText::GetEmpty();
		});
	_taskManager.pushTask(task);
}

//void UGsUITrayTickerAchievementComplete::OnAnimationEnded()
//{
//	Close();
//}
