// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITrayTickerJoinGuild.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Guild/GsGuildHelper.h"
#include "DataSchema/Guild/GsSchemaGuildEmblem.h"
#include "Paper2D/Classes/PaperSprite.h"


void UGsUITrayTickerJoinGuild::NativeDestruct()
{
	_remainTime = 0.f;

	Super::NativeDestruct();
}

void UGsUITrayTickerJoinGuild::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (0 < _remainTime)
	{
		_remainTime -= InDeltaTime;
		if (0 >= _remainTime)
		{
			_remainTime = 0.0f;

			Close();
		}
	}

	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UGsUITrayTickerJoinGuild::SetData(const FString& InGuildName, int32 InEmblemId, bool bIsCreate)
{
	FText textDesc;
	if (bIsCreate)
	{
		// TEXT: 기사단을 창설하였습니다.
		FText::FindText(TEXT("GuildText"), TEXT("Alert_CreateComplete"), textDesc);
	}
	else
	{
		// TEXT: 기사단에 가입되었습니다.
		FText::FindText(TEXT("GuildText"), TEXT("Alert_JoinComplete"), textDesc);
	}

	_textBlockDesc->SetText(textDesc);

	_textBlockGuild->SetText(FText::FromString(InGuildName));

	if (const FGsSchemaGuildEmblem* emblemData = FGsGuildHelper::GetGuildEmblem(InEmblemId))
	{
		_imgEmblem->SetBrushFromAtlasInterface(FGsUIHelper::LoadSpriteFromPath(emblemData->iconPathSmall));
	}

	_remainTime = _playTime;

	StartAnimation();
}
