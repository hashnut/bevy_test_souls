
#include "GsUITrayMockeryTicker.h"

#include "Sound/GsSoundPlayer.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Management/ScopeGame/GsPKBookManager.h"
#include "DataSchema/PKBook/GsSchemaPKBookConfig.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/RichTextBlock.h"
#include "UMG/Public/Animation/WidgetAnimation.h"
#include "UMG/Public/Blueprint/WidgetBlueprintGeneratedClass.h"


void UGsUITrayMockeryTicker::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_animationData = nullptr;

	UWidgetBlueprintGeneratedClass* genClass = GetWidgetTreeOwningClass();

	if (nullptr != genClass)
	{
		for (UWidgetAnimation* anim : genClass->Animations)
		{
			_animationData = anim;

			break;
		}
	}

	if (_animationData)
	{
		_finishDelegate.BindDynamic(this, &UGsUITrayMockeryTicker::OnAnimationFinish);
		BindToAnimationFinished(_animationData, _finishDelegate);
	}
}

void UGsUITrayMockeryTicker::BeginDestroy()
{
	_animationData = nullptr;

	Super::BeginDestroy();
}

void UGsUITrayMockeryTicker::AnimationStart()
{
	const static int32 NUM_LOOPS_TO_PLAY = 1;
	PlayAnimation(_animationData, 0.f, NUM_LOOPS_TO_PLAY, EUMGSequencePlayMode::Forward, 1.0f);
}

void UGsUITrayMockeryTicker::OnAnimationFinish()
{
	if (_endCallback)
	{
		_endCallback();
	}
}

void UGsUITrayMockeryTicker::SetData(const FText& InSystemMsg, const FText& InMockery, TFunction<void(void)> InEndCallback /* = nullptr */)
{
	_endCallback = InEndCallback;

	_textSystemMsg->SetText(InSystemMsg);
	_textMockeryMsg->SetText(InMockery);

	AnimationStart();

	if (GsPKBook())
	{
		if (UGsSoundPlayer* palyer = GSoundPlayer())
		{
			palyer->PlaySoundByPath(GsPKBook()->GetConfig()->soundResourceMock);
		}		
	}	
}