// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSocialHelper.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"


bool FGsSocialHelper::IsVisible(SocialMediaInfo InSocialMediaInfo, FString InUserName, FString InGuildName)
{
	if (false == InSocialMediaInfo.mNormal)
	{
		// 본인의 정보인가?
		if (GGameData()->GetUserData()->mUserName == InUserName)
			return true;

		// 같은 길드원 정보인가?
		if (!InGuildName.IsEmpty() && GGuild()->GetMyGuildData()->_guildName.Equals(InGuildName))
			return true;
	}
	return InSocialMediaInfo.mNormal;
}