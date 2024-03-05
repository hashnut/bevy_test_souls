
#include "GsUICommonUserContext.h"
#include "Unlock/GsUnlockDefine.h"
#include "../Party/GsPartyFunc.h"
#include "../Arena/GsArenaHandler.h"
#include "../Management/ScopeGame/GsPKBookManager.h"
#include "../Management/ScopeGame/GsUnlockManager.h"
#include "../Management/ScopeGame/GsPersonalTradingManager.h"
#include "../Management/ScopeGame/GsArenaManager.h"
#include "../Management/ScopeGlobal/GsGameDataManager.h"
#include "../Management/ScopeGlobal/GsLevelManager.h"
#include "../Management/ScopeGlobal/GsUIManager.h"
#include "../Management/ScopeGlobal/GsGuildManager.h"
#include "../GameObject/Fence/GsFenceHandler.h"
#include "../UserContext/GsUserContextData.h"
#include "../ContentsServer/GsContentsServerHelper.h"
#include "../GameObject/Fence/GsFence.h"
#include "../GameObject/ObjectClass/GsGameObjectRemotePlayer.h"
#include "../UTIL/GsText.h"
#include "../../Helper/GsUIHelper.h"
#include "../../Popup/GsUIPopupChat.h"
#include "../../Popup/GsUIPopupChatReport.h"
#include "../../Tray/NotiMessage/GsUISectionMessageDefine.h"
#include "DataSchema/GsSchemaEnums.h"
#include "DataSchema/Arena/GsSchemaArenaData.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "HAL/PlatformApplicationMisc.h"
#include "../Net/GsNetSendService.h"
#include "../Net/GsNetSendServiceWorld.h"
#include "../Management/ScopeGame/GsBuddyManager.h"
#include "../Message/MessageParam/GsUIMessageParam.h"



const TMap<EGsContextContentsMenu, bool> FGsUICommonUserContext::GetActivationFeature(const FGsUserContextInfo* InUserInfo, const UGsGameObjectRemotePlayer* InRemotePlayer) const
{
	TMap<EGsContextContentsMenu, bool> OutValue;

	// 2023/9/13 PKT - macro define
#define CHECKED_UNLOCK_ADD_CONTETNS(UnlockType, AddContetns)	\
	if (GSUnlock()->IsContentsUnlock(UnlockType, false))		\
	{															\
		OutValue.FindOrAdd(AddContetns) = false;				\
	}																				

	do 
	{
		if (nullptr == InUserInfo)
		{
			break;
		}

		if (AccountGrade::GM != InUserInfo->GetAccountGrade() && AccountGrade::NORMAL != InUserInfo->GetAccountGrade())
		{
			// 2023/9/11 PKT - 예외 처리. GM / Normal 의 User Info만 제공
			break;
		}

		const WorldId LocalHomeWorldId = GGameData()->GetUserData()->mHomeWorldId;
		const WorldId LocalLocatedWorldId = GGameData()->GetLoadWorldId();
		const WorldId TargetHomeWorldId = InUserInfo->GetHomeWorldId() == INVALID_WORLD_ID ? LocalHomeWorldId : InUserInfo->GetHomeWorldId();

		//const bool isSameWorldUser = InUserInfo->IsSameWorldUser();

		if (AccountGrade::GM == InUserInfo->GetAccountGrade())
		{	// 2023/9/11 PKT - GM 같은 경우 PROFILE 기능만 제공( 다른 서버 GM도 존재 할까? )
			if (LocalHomeWorldId == TargetHomeWorldId)
			{	// 2023/9/11 PKT - 다른 서버 GM까지는 신경 쓰지 말자.~ㅋ
				CHECKED_UNLOCK_ADD_CONTETNS(EGsUnlockContentsType::USER_PROFILE, EGsContextContentsMenu::USER_PROFILE);
			}
			break;
		}

		CHECKED_UNLOCK_ADD_CONTETNS(EGsUnlockContentsType::USER_PROFILE, EGsContextContentsMenu::USER_PROFILE);
		CHECKED_UNLOCK_ADD_CONTETNS(EGsUnlockContentsType::GUILD, EGsContextContentsMenu::GUILD);
		CHECKED_UNLOCK_ADD_CONTETNS(EGsUnlockContentsType::PERSONAL_TRADING, EGsContextContentsMenu::PERSONAL_TRADE);

		if (InRemotePlayer)
		{	// 2023/9/11 PKT - Arena
			if (IsAreanArea(InRemotePlayer->GetLocation()))
				OutValue.FindOrAdd(EGsContextContentsMenu::ARENA) = false;
		}
		else
		{	// 2023/9/13 PKT - HUD가 아닌 다른 경로는 RemovePlayer의 정보가 없으므로 무조건 활성화(예외 사항은 GetActivationFeatureWithOwner 여기에서...) 
			OutValue.FindOrAdd(EGsContextContentsMenu::ARENA) = false;
		}

		// 2023/9/11 PKT - 차단
		OutValue.FindOrAdd(EGsContextContentsMenu::USER_BLOCK) = false;
		// 2023/9/11 PKT - 신고
		OutValue.FindOrAdd(EGsContextContentsMenu::USER_REPORT) = false;
		// 2023/9/13 PKT - 친구 추가
		OutValue.FindOrAdd(EGsContextContentsMenu::ADD_FRIEND) = false;
		// 2023/9/11 PKT - 파티 초대
		CHECKED_UNLOCK_ADD_CONTETNS(EGsUnlockContentsType::PARTY_CREATE_OR_JOIN, EGsContextContentsMenu::INVITE_PARTY);
		// 2023/9/13 PKT - 귓속말
		OutValue.FindOrAdd(EGsContextContentsMenu::WHISPER) = false;
		// 2023/9/1 PKT - 경계 대상( 항상 활성화)
		OutValue.FindOrAdd(EGsContextContentsMenu::USER_BOUNDARYTARGET) = IsBoundaryTargetSwitching(InUserInfo);
		// 2023/9/11 PKT - 이름복사( 기능 대기 - 추후 업데이트 예정)
		// OutValue.FindOrAdd(EGsContextContentsMenu::NAME_COPY) = false;

		// 2023/9/1 PKT - 나와 서버가 다를 경우 막아야 하는 예외 사항
		if (LocalHomeWorldId != TargetHomeWorldId)
		{
			// 2023/9/11 PKT - 유저 프로필
			if (OutValue.Contains(EGsContextContentsMenu::USER_PROFILE))
				OutValue.Remove(EGsContextContentsMenu::USER_PROFILE);

			// 2023/9/11 PKT - 차단 제외
			if (OutValue.Contains(EGsContextContentsMenu::USER_BLOCK))
				OutValue.Remove(EGsContextContentsMenu::USER_BLOCK);

			// 2023/9/11 PKT - 신고 제외
			if (OutValue.Contains(EGsContextContentsMenu::USER_REPORT))
				OutValue.Remove(EGsContextContentsMenu::USER_REPORT);

			// 2023/9/11 PKT - 길드 제외
			if (OutValue.Contains(EGsContextContentsMenu::GUILD))
				OutValue.Remove(EGsContextContentsMenu::GUILD);

			// 2023/9/11 PKT - 개인 거래 제외
			if (OutValue.Contains(EGsContextContentsMenu::PERSONAL_TRADE))
				OutValue.Remove(EGsContextContentsMenu::PERSONAL_TRADE);

			// 2023/9/13 PKT - 근데 이렇게만 막으면 침공서버 관련은 정확하게 막기가 어렵다. 현재 있는 정보로는 여기까지가 한계. 좀더 명확하게는 유저의 현재 위치 WorldId가 필요함.
			//				  ex ) 내가 Inter Server에 와 있고, 타겟 유저가 침공전 유저라면??? or 내가 Inter Field에 와 있고, 타유저가 Inter Dungeon에 와 있고.. 이런 경우데 대한 처리를 할 수가 없음.
			if (false == FGsContentsServerHelper::IsLocatedOnInterServer())
			{
				// 2023/9/11 PKT - 친구 제외
				if (OutValue.Contains(EGsContextContentsMenu::ADD_FRIEND))
					OutValue.Remove(EGsContextContentsMenu::ADD_FRIEND);

				// 2023/10/11 CSH - 파티 추가 제외 - 인터서버외에는 기능을 사용할 수 없다.
				if (OutValue.Contains(EGsContextContentsMenu::INVITE_PARTY))
					OutValue.Remove(EGsContextContentsMenu::INVITE_PARTY);
			}
		}		

	} while (0);
	
#ifdef CHECKED_UNLOCK_ADD_CONTETNS
	#undef CHECKED_UNLOCK_ADD_CONTETNS 
#endif // CHECKED_UNLOCK_ADD_CONTETNS

	return OutValue;
}

const TMap<EGsContextContentsMenu, bool> FGsUICommonUserContext::GetActivationFeatureWithBan(const EGsUserContentsOwner InOwner, const struct FGsUserContextInfo* InUserInfo, const class UGsGameObjectRemotePlayer* InRemotePlayer) const
{
	auto temp = GetActivationFeature(InUserInfo, InRemotePlayer);
	
#define CHECKED_BAN_CONTENTS(RemoveContetns)	if (temp.Contains(RemoveContetns))	{ temp.Remove(RemoveContetns); }


	switch (InOwner)
	{
	case EGsUserContentsOwner::NONE:
		// 2023/9/13 PKT - None
		break;
	case EGsUserContentsOwner::HUD_HPBAR:				// 2023/9/12 PKT - HUD HP Bar에서 요청
	{
		// 2023/9/16 CSH - 상황에 따라 RemotePlayer가 Nullptr가 될 수 있다.
		if (nullptr == InRemotePlayer)
		{
			CHECKED_BAN_CONTENTS(EGsContextContentsMenu::ARENA);
		}
	}	break;
	case EGsUserContentsOwner::GUILD_MEMBER:			// 2023/9/12 PKT - 길드 정보에서 요청
	{	
		CHECKED_BAN_CONTENTS(EGsContextContentsMenu::GUILD);
	}	break;
	case EGsUserContentsOwner::BUDDY_MEMBER:			// 2023/9/12 PKT - 친구 목록에서 요청
	{	
		CHECKED_BAN_CONTENTS(EGsContextContentsMenu::ADD_FRIEND);
	}	break;
	case EGsUserContentsOwner::PARTY_MEMBER:			// 2023/9/12 PKT - 파티 목록에서 요청
	{	
		CHECKED_BAN_CONTENTS(EGsContextContentsMenu::INVITE_PARTY);
	}	break;
	case EGsUserContentsOwner::RANKING:					// 2023/9/12 PKT - 랭킹 정보에서 요청
	{	
		// 2023/9/13 PKT - None
	}	break;
	case EGsUserContentsOwner::USER_PROFILE:			// 2023/9/12 PKT - 프로필에서 요청
	{		
		CHECKED_BAN_CONTENTS(EGsContextContentsMenu::USER_PROFILE);
	}	break;
	case EGsUserContentsOwner::USER_CHEAT:				// 2023/9/12 PKT - 채팅에서 요청
	{
		// 2023/9/13 PKT - 채팅을 통해서 열때는 Arena를 항상 끈다.
		CHECKED_BAN_CONTENTS(EGsContextContentsMenu::ARENA);
	}	break;
	default:
		break;
	}
	
#ifdef CHECKED_BAN_CONTENTS
	#undef CHECKED_BAN_CONTENTS 
#endif // CHECKED_BAN_CONTENTS
	return temp;
}


bool FGsUICommonUserContext::IsAreanArea(const FVector& InTargetLocated) const
{
	bool isArea = false;
	do 
	{
		FGsFenceHandler* fenceHandler = GFence();
		if (nullptr == fenceHandler)
		{
			break;
		}

		// 대상 유저가 대결 준비 구역 안에 있는지 확인 (ArenaReady)
		bool isEnteredArenaReadyTarget = false;
		TArray<TSharedPtr<FGsFence>> fenceList;
		if (false == fenceHandler->TryGetAllOverlapedFence(fenceList, InTargetLocated))
		{
			break;
		}

		for (int32 idx = 0; idx < fenceList.Num(); ++idx)
		{
			if (fenceList[idx]->IsArenaReady())
			{
				// 대결 준비 구역 안에 있는지 확인
				if ((false == fenceHandler->IsEnteredTerritoryType(TerritoryType::ARENA)) && fenceHandler->IsEnteredTerritoryType(TerritoryType::ARENA_READY))
				{
					// 현재 지역의 아레나 정보가 존재하는지 확인
					int32 mapId = GLevel()->GetCurrentLevelId();
					if (const FGsSchemaArenaData* arenaData = UGsTableUtil::FindRowById<UGsTableArenaData, FGsSchemaArenaData>(mapId))
					{
						isArea = true;
					}
					break;
				}
			}
		}

	} while (0);

	return isArea;
}

bool FGsUICommonUserContext::IsBoundaryTargetSwitching(const FGsUserContextInfo* InUserInfo) const
{
	return true == GsPKBook()->CheckDetectionTarget(InUserInfo->GetUserDBId(), InUserInfo->GetHomeWorldId(), FText::FromString(InUserInfo->GetUserName()));
}

void FGsUICommonUserContext::SendUserProfile(UserDBId InUserDBId)
{
	if (FGsUnlockManager* unlockManager = GSUnlock())
	{
		if (unlockManager->IsContentsUnlock(EGsUnlockContentsType::USER_PROFILE, true))
		{
			FGsNetSendService::SendReqUserProfile(InUserDBId);
		}
	}
}

void FGsUICommonUserContext::SendGuildInfo(const FGsUserContextInfo* InUserInfo)
{
	if (nullptr == InUserInfo)
	{
#if WITH_EDITOR
		GSLOG(Error, TEXT("nullptr == InUserInfo"));
#endif
		return;
	}

	if (false == InUserInfo->IsVisible())
	{	// 기사단 정보를 공개하지 않은 유저입니다.
		FText findText;
		FText::FindText(TEXT("SocialText"), TEXT("profileEdit_desc_Guild_NotOpen"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		return;
	}

	if (INVALID_GUILD_DB_ID == InUserInfo->GetGuildDBId())
	{
		// 기사단에 가입되어 있지 않은 유저입니다.
		FText findText;
		FText::FindText(TEXT("ContextUIText"), TEXT("contextUIText2"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		return;
	}

	if (FGsUnlockManager* unlockManager = GSUnlock())
	{
		if (false == unlockManager->IsContentsUnlock(EGsUnlockContentsType::GUILD, true))
		{
			return;
		}
	}

	if (GGuild()->GetGuildDBId() == InUserInfo->GetGuildDBId())
	{
		// 자신이 소속된 기사단의 정보는 이 곳에서 볼 수 없습니다.
		FText findText;
		FText::FindText(TEXT("ContextUIText"), TEXT("contextUIText3"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		return;
	}

	GGuild()->SendRequestGuildDetail(InUserInfo->GetGuildDBId());
}

// 2023/9/11 PKT - 파티 요청
void FGsUICommonUserContext::SendInviteParty(const FString& InUserName, const WorldId InWorldId)
{
	FGsPartyFunc::InviteParty(InUserName, InWorldId);
}

// 2023/9/13 PKT - 친구 추가
void FGsUICommonUserContext::SendAddFriend(UserDBId InUserDBId, const FString& InUserName, WorldId InWorldId)
{
	if (IsTargetSelf(InUserDBId, FText::FromString(InUserName)))
	{
		// 자기 자신을 차단할 수 없습니다.
		FText findText;
		FText::FindText(TEXT("ChatText"), TEXT("ChatTicker10"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		return;
	}

	if (false == InUserName.IsEmpty())
	{
		if (GSBuddy()->CheckIsAdded(InUserName))
		{
			// 2023/9/13 PKT - 현재 친구 추가는 타이핑 기준으로 제작되어 있어, 유저 이름과 Prefix를 붙여서 넣어 줘야 함.
			FString userNameAndPreFix = FGsTextUtil::CombineUserAndPrefix(FText::FromString(InUserName), InWorldId);
			
			FGsUIMsgParamBuddy param = FGsUIMsgParamBuddy(InUserDBId, userNameAndPreFix);
			GMessage()->GetUI().SendMessage(MessageUI::BUDDY_ADD, &param);
		}
	}
}

// 2023/9/11 PKT - 유저 정보 갱신 요청
bool FGsUICommonUserContext::SendUserContextRefresh(const FString& InUserName, WorldId InWorldId)
{
	if (INVALID_WORLD_ID == InWorldId || InWorldId == GGameData()->GetUserData()->mHomeWorldId)
	{
		FGsNetSendService::SendReqUserContext(InUserName);
	}
	else
	{
		FGsNetSendServiceWorld::SendReqUserContextOtherServer(InUserName, InWorldId);
	}
	
	return true;
}

// 2023/9/12 PKT - 대결 신청(아레나)
bool FGsUICommonUserContext::SendDuelInvite(UserDBId InUserDBId, Level InLevel, FString InUserName, bool InIsVisible)
{
	if (FGsArenaManager* arenaManager = GSArena())
	{
		if (FGsArenaHandler* arenaHandler = arenaManager->GetArenaHandler())
		{
			return arenaHandler->TryReqDuelInvite(InUserDBId, InLevel, InUserName, InIsVisible);
		}
	}

	return false;
}

// 2023/9/11 PKT - 귓속말
void FGsUICommonUserContext::SendWhisper(const UserDBId InUserDBId, const FString& InUserName, const WorldId InWorldId)
{
	if (IsTargetSelf(InUserDBId, FText::FromString(InUserName)))
	{
		// 자기 자신을 차단할 수 없습니다.
		FText findText;
		FText::FindText(TEXT("ChatText"), TEXT("ChatTicker10"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		return;
	}

	TWeakObjectPtr<UGsUIPopupChat> chatPopup = Cast<UGsUIPopupChat>(GUI()->OpenAndGetWidget(TEXT("PopupChat")));
	if (chatPopup.IsValid())
	{
		UGsUIPopupChat::PopupInitData initData{ EGsChatViewType::ALL, EGsExChatType::WHISPER,  InUserName, InWorldId };
		chatPopup->InitializeData(&initData);
	}
}

// 2023/9/11 PKT - 이름 카피
bool FGsUICommonUserContext::SendNameCopy(const FText& InUserName, const WorldId InWorldId)
{
	if (false == InUserName.IsEmpty())
	{
		FString userName = FGsTextUtil::CombineUserAndPrefix(InUserName, InWorldId);
		FPlatformApplicationMisc::ClipboardCopy(*userName);

		return true;
	}
	return false;
}

// 2023/9/11 PKT - 경계 대상
void FGsUICommonUserContext::SendBoundaryTarget(bool IsAdd, const UserDBId InUserDBId, const WorldId InWorldId)
{
	if (IsAdd)
	{
		FGsNetSendServiceWorld::Send_ReqPKBookAddDetectionTarget(InWorldId, InUserDBId);
	}
	else
	{
		FGsNetSendServiceWorld::Send_ReqPKBookDeleteDetectionTarget(InUserDBId);
	}
}

// 2023/9/11 PKT - 개인 거래 요청
void FGsUICommonUserContext::SendPersonalTrading(int64 InGameId, const FString& InUserName)
{
	if (FGsPersonalTradingManager* tradeManager = GSPersonalTrading())
	{
		tradeManager->TryReqPersonalTradingTry(InGameId, InUserName);	
	}
}

void FGsUICommonUserContext::SendUserBlock(const UserDBId InUserDBId, const FString& InName)
{
	if (IsTargetSelf(InUserDBId, FText::FromString(InName)))
	{
		// 자기 자신을 차단할 수 없습니다.
		FText findText;
		FText::FindText(TEXT("ChatText"), TEXT("ChatTicker10"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		return;
	}

	const auto blockList = GGameData()->GetChatBlockList();
	if (blockList.FindByPredicate([InName](FGsChatBlockUser& e) {
		return 	e._name.Compare(InName) == 0;
		}))
	{
		// 이미 차단한 유저입니다
		FText findText;
		FText::FindText(TEXT("ChatText"), TEXT("ChatTicker7"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		return;
	}
	
	FGsNetSendService::SendReqBlockChatUser(InName);
}

void FGsUICommonUserContext::SendUserRepot(const FString& InName, const FString& InChatText)
{
	TWeakObjectPtr<UGsUIPopupChatReport> reportPopup = Cast<UGsUIPopupChatReport>(GUI()->OpenAndGetWidget(TEXT("PopupChatReport")));
	if (reportPopup.IsValid())
	{
		UGsUIPopupChatReport::PopupInitData initData{ InName, InChatText };
		reportPopup->InitializeData(&initData);
	}
}

bool FGsUICommonUserContext::IsTargetSelf(UserDBId InUserDBId, const FText& InName)
{
	// 2023/9/13 PKT - 이 부분 추가 해야 함. 이젠 name이 PreFix까지 같이 넘어 올수도 있기 때문에..
	if (const FGsNetUserData* userData = GGameData()->GetUserData())
	{
		if (InUserDBId != INVALID_USER_DB_ID && InUserDBId == userData->mUserDBId)
		{
			return true;
		}

		if (userData->mUserName.Compare(InName.ToString()) == 0)
		{
			return true;
		}
	}
	return false;
}