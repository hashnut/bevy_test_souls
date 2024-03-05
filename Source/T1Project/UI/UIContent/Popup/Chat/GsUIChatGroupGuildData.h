#pragma once

#include "CoreMinimal.h"
#include "Guild/GsGuildGroupChatData.h"
#include "ContentsServer/GsContentsServerHelper.h"
#include "GsUIChatGroupGuildData.generated.h"

struct FGsGuildGroupChatMemberGuild;
/**
 *
 */
UCLASS()
class T1PROJECT_API UGsUIChatGroupGuildData : public UObject
{
	GENERATED_BODY()

public:
	enum class MemberState : uint8 { Member = 0, InviteGuest = 1 };

private:
	TWeakPtr<FGsGuildGroupChatMemberGuild> _data;
	MemberState _state{ MemberState::Member };
	bool _controlAuth{ false };

public:
	MemberState GetEntryState() { return _state; }
	void SetEntryState(MemberState state) { _state = state; }
	void SetMasterGuildDiplomat(bool set) { _controlAuth = set; }

	void SetItemData(TWeakPtr<FGsGuildGroupChatMemberGuild> data) { _data = data; }
	TWeakPtr<FGsGuildGroupChatMemberGuild> GetItemData() { return _data; }

	bool IsAuthorizedMember() { return _controlAuth; }

	bool IsMasterGuild() { return _data.IsValid() ? _data.Pin()->_isMaster : false; }
	bool IsMyWorldGuild() { return _data.IsValid() ? _data.Pin()->_isHomeWorldGuild : false; }

	FText GetGuildMasterName() { return _data.IsValid() ? FText::FromString(_data.Pin()->_masterName) : FText::GetEmpty(); }
	GuildEmblemId GetEmblemId() { return _data.IsValid() ? _data.Pin()->_emblemId : INVALID_GUILD_EMBLEM_ID; }

	FText GetGuildName() {
		if (auto data = _data.Pin())
		{
			if (data->_isHomeWorldGuild)
			{
				return FText::FromString(data->_guildName);
			}
				
			FText textPrefix = FGsContentsServerHelper::GetWorldIPrefixdWithWorldId(data->_homePlanetWorldId.st.worldId);
			return FText::FromString(FString::Format(TEXT("{1}[{0}]"), { *textPrefix.ToString(), *(data->_guildName) }));
		}

		return FText::GetEmpty();
	}


	GuildDBId GetGuildDBId() { return _data.IsValid() ? _data.Pin()->_guildDBId : INVALID_GUILD_DB_ID; }
};

using MemberState = UGsUIChatGroupGuildData::MemberState;

