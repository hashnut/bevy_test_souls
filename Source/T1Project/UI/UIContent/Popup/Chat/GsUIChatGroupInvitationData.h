#pragma once

#include "CoreMinimal.h"
#include "Guild/GsGuildGroupChatData.h"
#include "ContentsServer/GsContentsServerHelper.h"
#include "GsUIChatGroupInvitationData.generated.h"

struct FGsGuildGroupChatInviteHost;
/**
 *
 */
UCLASS()
class T1PROJECT_API UGsUIChatGroupInvitationData : public UObject
{
	GENERATED_BODY()

private:
	TWeakPtr<FGsGuildGroupChatInviteHost> _data;

public:
	void SetItemData(TWeakPtr<FGsGuildGroupChatInviteHost> data) { _data = data; }
	TWeakPtr<FGsGuildGroupChatInviteHost> GetItemData() { return _data; }

	bool IsMyWorldGuild() { return _data.IsValid() ? _data.Pin()->GetGuildData()._isHomeWorldGuild : false; }

	FText GetGroupChatRoomName() { return _data.IsValid() ? FText::FromString(_data.Pin()->_roomName) : FText::GetEmpty(); }
	FText GetGuildMasterName() { return _data.IsValid() ? FText::FromString(_data.Pin()->GetGuildData()._masterName) : FText::GetEmpty(); }
	GuildEmblemId GetEmblemId() { return _data.IsValid() ? _data.Pin()->GetGuildData()._emblemId : INVALID_GUILD_EMBLEM_ID; }

	FText GetGuildName() { 
		if (auto data = _data.Pin())
		{
			FText textPrefix = FGsContentsServerHelper::GetWorldIPrefixdWithWorldId(data->GetGuildData()._homePlanetWorldId.st.worldId);
			return FText::FromString(FString::Format(TEXT("{1}[{0}]"), { *textPrefix.ToString(), *(data->GetGuildData()._guildName) }));
		}

		return FText::GetEmpty();
	}

	GuildDBId GetGuildDBId() { return _data.IsValid() ? _data.Pin()->GetGuildData()._guildDBId : INVALID_GUILD_DB_ID; }
	GuildGroupChatRoomId GetGroupChatRoomId() { return _data.IsValid() ? _data.Pin()->_roomId : INVALID_GUILD_GROUP_CHAT_ROOM_ID; }
};
