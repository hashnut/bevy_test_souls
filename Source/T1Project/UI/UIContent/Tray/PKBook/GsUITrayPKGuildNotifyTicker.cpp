
#include "GsUITrayPKGuildNotifyTicker.h"

#include "../PKBook/GsPKBookHelper.h"
#include "../../Helper/GsUIHelper.h"

#include "UMG/Public/Components/Image.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "UMG/Public/Components/RichTextBlock.h"
#include "UMG/Public/Components/RichTextBlockImageDecorator.h"
#include "../Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperSprite.h"





void UGsUITrayPKGuildNotifyTicker::NativeOnInitialized()
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
		_finishDelegate.BindDynamic(this, &UGsUITrayPKGuildNotifyTicker::OnAnimationFinish);
		BindToAnimationFinished(_animationData, _finishDelegate);
	}
}

void UGsUITrayPKGuildNotifyTicker::BeginDestroy()
{
	_animationData = nullptr;

	Super::BeginDestroy();
}

void UGsUITrayPKGuildNotifyTicker::AnimationStart()
{
	const static int32 NUM_LOOPS_TO_PLAY = 1;
	PlayAnimation(_animationData, 0.f, NUM_LOOPS_TO_PLAY, EUMGSequencePlayMode::Forward, 1.0f);
}

void UGsUITrayPKGuildNotifyTicker::OnAnimationFinish()
{
	if (_endCallback)
	{
		_endCallback();
	}
}

void UGsUITrayPKGuildNotifyTicker::SetData(GuildEmblemId InKillerGuildEmblemdId, GuildEmblemId InDeadGuildEmblemdId, const FText& InKillerName, const FText& InDeadName , TFunction<void(void)> InEndCallback /* = nullptr */)
{
	_endCallback = InEndCallback;

	FSoftObjectPath killerGuildEmblem;
	FGsPKBookHelper::FindPathGuildIcon(InKillerGuildEmblemdId, true, killerGuildEmblem);

	FSoftObjectPath deadGuildEmblem;
	FGsPKBookHelper::FindPathGuildIcon(InDeadGuildEmblemdId, true, deadGuildEmblem);
	
	if (killerGuildEmblem.IsValid())
	{
		_iconKillGuildEmblem->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_iconKillGuildEmblem->SetBrushFromAtlasInterface(FGsUIHelper::LoadSpriteFromPath(killerGuildEmblem));
	}
	else
	{
		_iconKillGuildEmblem->SetVisibility(ESlateVisibility::Hidden);
	}

	if (deadGuildEmblem.IsValid())
	{
		_iconDeadGuildEmblem->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_iconDeadGuildEmblem->SetBrushFromAtlasInterface(FGsUIHelper::LoadSpriteFromPath(deadGuildEmblem));
	}
	else
	{
		_iconDeadGuildEmblem->SetVisibility(ESlateVisibility::Hidden);
	}

	// 2022/09/22 PKT - "{0}님이 {1}님을 처치하였습니다."
	FText format;
	FGsPKBookHelper::FindText(TEXT("Tray_PK_Guild_Notify"), format);
	FText message = FText::Format(format, InKillerName, InDeadName);

	_textMessage->SetText(message);

	AnimationStart();
}