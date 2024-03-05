#pragma once
#include "CoreMinimal.h"
#include "Chat/GsChatItem.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "GsUIListDataChat.generated.h"

UCLASS()
class T1PROJECT_API UGsUIListDataChat :
    public UObject
{
    GENERATED_BODY()

public:

    void SetItemData(TWeakPtr<FGsChatItem> data) {
        _data = data;
    };

	FString GetMarkupName() {

		if (auto data = _data.Pin())
		{
			return data->GetMarkupName();
		}
		return TEXT("");
	}

	//FString GetMarkupNick() {
	//	return _data.IsValid() ? _data.Pin()->_markupNick : TEXT("");
	//}
	FString GetMarkupText()
	{
		return _data.IsValid() ? _data.Pin()->_markupText : TEXT("");
	}

	time_t GetCheckRemainTime()
	{
		if (auto data = _data.Pin())
		{
			return FGsTimeSyncUtil::GetServerNowSecond() - data->GetInputTime();
		}
		return 0.f;
	}

	FString GetMarkupTime()
	{
		FString markupTime;
		FText textTimes;
		FText::FindText(TEXT("ChatText"), TEXT("ChatTime1"), textTimes);

		if (auto data = _data.Pin())
		{
			FDateTime inputTime = FGsTimeSyncUtil::ConvertServerTimeToUTCDateTime(_data.Pin()->GetInputTime());
			FTimespan passTime = FGsTimeSyncUtil::GetServerNowDateTimeUTC() - inputTime;

			int32 days = passTime.GetTotalDays();
			int32 hours = 0;
			int32 minutes = 0;

			if (days > 0)
			{
				if (FText::FindText(TEXT("ChatText"), TEXT("ChatTime4"), textTimes))
				{
					textTimes = FText::Format(textTimes, days);
				}
				//markupTime = FString::Format(TEXT(" <InputTime>{0}</>"), { *textDay.ToString() });
			}
			else
			{
				hours = static_cast<int32>(passTime.GetTotalHours());
				if (hours > 0)
				{
					if (FText::FindText(TEXT("ChatText"), TEXT("ChatTime3"), textTimes))
					{
						textTimes = FText::Format(textTimes, hours);
					}
				}
				else
				{
					if ((minutes = static_cast<int32>(passTime.GetTotalMinutes())) > 0)
					{
						if (FText::FindText(TEXT("ChatText"), TEXT("ChatTime2"), textTimes))
						{
							textTimes = FText::Format(textTimes, minutes);
						}
					}
				}
			}
		}

		markupTime = FString::Format(TEXT(" <InputTime>{0}</>"), { *textTimes.ToString() });
		return _data.IsValid() ? markupTime : TEXT("");
	}
	
	EGsExChatType GetChatType()
	{
		return _data.IsValid() ? _data.Pin()->_type : EGsExChatType::NORMAL;
	}

	uint64 GetChatterUID()
	{
		if (auto data = _data.Pin())
		{	
			return data->_isSentWhisper ? 0 : _data.Pin()->GetGameId();
		}
		return 0;
	}

	FString GetNickName()
	{
		return _data.IsValid() ? _data.Pin()->GetNickName() : TEXT("");
	}

	FString GetText()
	{
		return _data.IsValid() ? _data.Pin()->GetChatText() : TEXT("");
	}

	bool IsMyWorldPlayer() { return _data.IsValid() ? _data.Pin()->_isMyWorldPlayer : false; }

	uint64 GetChatRank()
	{
		return (_data.IsValid() && _data.Pin()->GetAccountGrade() == AccountGrade::NORMAL) ? _data.Pin()->GetUserRank() : 0;
	}

	bool IsGM() { return _data.IsValid() && _data.Pin()->GetAccountGrade() == AccountGrade::GM; }

	WorldId GetHomeWorldId()
	{
		return _data.IsValid() ? _data.Pin()->GetHomeWorldId() : 0;
	}

	GuildEmblemId GetGuildEmblemId()
	{
		return  _data.IsValid() && _data.Pin()->_type == EGsExChatType::GROUP ? 
				_data.Pin()->GetGuildEmblemId() : INVALID_GUILD_EMBLEM_ID;
	}


protected:
	TWeakPtr<FGsChatItem> _data{ nullptr };
};

