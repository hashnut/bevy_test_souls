
#include "GsUITrayTickerCollectionMulti.h"
#include "WidgetAnimation.h"
#include "TextBlock.h"

void UGsUITrayTickerCollectionMulti::NativeConstruct()
{
	Super::NativeConstruct();

	SetAnimationCurrentTime(_animationStart, 0.f);
	StopAnimation(_animationStart);

	_taskManager._taskFinished = [this]() {
		Close();
	};
}

void UGsUITrayTickerCollectionMulti::NativeDestruct()
{	
	_taskManager.clearTask(true);

	Super::NativeDestruct();
}

void UGsUITrayTickerCollectionMulti::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	_taskManager.doTasks(InDeltaTime);
}

void UGsUITrayTickerCollectionMulti::SetData(const FCollectionTickerInitParam& InInitData)
{
	OnInitWork(InInitData);
}

void UGsUITrayTickerCollectionMulti::OnInitWork(const FCollectionTickerInitParam& data)
{
	static float deltaTime = _animationStart->GetEndTime();

	auto task = MakeShared<FGsTaskTrayAct>(TEXT("CollectionTicker"), deltaTime, [this, data]() {
			_name = data._name;
			_desc = data._desc;
			PlayAnimation(_animationStart);
		}, [this]() {

			_name = FText::GetEmpty();
			_desc = FText::GetEmpty();
		});
	_taskManager.pushTask(task);
}