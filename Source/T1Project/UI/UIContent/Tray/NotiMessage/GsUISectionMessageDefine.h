#pragma once
#include "UI/UILib/Base/GsUITray.h"
#include "Shared/Client/SharedEnums/SharedNoticeEnum.h"

enum class SectionPriority : uint8 { None = 0, Insert, Instant };

UENUM(BlueprintType)
enum class EGsTickerSectionAnimationType : uint8
{
	NONE = 0,
	FADE_IN,
	FADE_OUT,
	SLIDE,
	ROLLING,
};

UENUM(BlueprintType)
enum class EGsNotiSection : uint8
{
	SYSTEM = 0, CONTENTS, EXTSYSTEM, NOTIFICATION, ALERT
};

struct FGsTickerMessageDataEx
{
	TrayCondition _condition{ TrayCondition::TC_Active };
	FText _text;
	float _playTime;
	float _delayTime{ 0.f };
	bool _rollingEffect{ false };

	NoticeType _type{ NoticeType::MAX };

	FGsTickerMessageDataEx() = default;
	FGsTickerMessageDataEx(const FText& text, float playTime, float delayTime, TrayCondition condition) //EGsTickerSectionAnimationType animationType, 
	{
		_text = text;
		_playTime = playTime;
		_delayTime = delayTime;
		_condition = condition;
	}
};
