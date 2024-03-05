// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

/**
 * ¼Ò¼È Helper Å¬·¡½º
 */
class T1PROJECT_API FGsSocialHelper
{
public:
	static bool IsVisible(SocialMediaInfo InSocialMediaInfo, FString InUserName, FString InGuildName);

};
