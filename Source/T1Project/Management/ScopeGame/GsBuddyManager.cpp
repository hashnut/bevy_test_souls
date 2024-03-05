// Fill out your copyright notice in the Description page of Project Settings.


#include "GsBuddyManager.h"

#include "Shared/Shared/SharedInclude/SharedDefine.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedBuddyEnum.h"
#include "Net/GsNetSendService.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"

#include "Buddy/GsBuddyInfo.h"
#include "Buddy/GsBuddyDefine.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/GsContentsMode.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UTIL/GsTimeSyncUtil.h"

#include "Option/GsServerOption.h"

#include "T1Project.h"

void FGsBuddyManager::InitializeUserData()
{
	Clear();

	FGsGameDataManager* gameDataManager = GGameData();
	if (nullptr == gameDataManager)
		return;

	if (true == gameDataManager->IsNewDataBuddy())
	{
		const FGsNetUserData* userData = gameDataManager->GetUserData();
		if (nullptr == userData)
			return;

		SetBuddyTabRedDot(userData->mBuddyData.mIsRedDot);
		SetBuddyAcceptTabRedDot(userData->mBuddyData.mIsAcceptRedDot);

		_buddyMap = userData->mBuddyData.mBuddyMap;
		_applyBuddyMap = userData->mBuddyData.mApplyBuddyMap;
		_acceptBuddyMap = userData->mBuddyData.mAcceptBuddyMap;

		// 데이터 사용 끝
		gameDataManager->SetNewDatabuddy(false);
	}
}

void FGsBuddyManager::Finalize()
{
	Clear();
}

void FGsBuddyManager::Clear()
{
	_buddyMap.Empty();
	_applyBuddyMap.Empty();
	_acceptBuddyMap.Empty();
}

// 친구 추가
void FGsBuddyManager::AddBuddy(uint64 inUserDBId, const FString& inUserName, time_t inElapsedTime, WorldId inWorldId)
{
	auto IsSameHomeWorld = [&](WorldId inWorldId)
	{
		const FGsNetUserData* userData = GGameData()->GetUserData();
		if (nullptr == userData)
			return false;

		return userData->mHomeWorldId == inWorldId;
	};

	if (!_buddyMap.Contains(inUserDBId))
	{
		TSharedPtr<FGsBuddyInfo> buddyInfo = _buddyMap.Emplace(inUserDBId, 
			MakeShareable(new FGsBuddyInfo(inUserDBId, inUserName, inElapsedTime, inWorldId, IsSameHomeWorld(inWorldId))));

		if (!buddyInfo.IsValid())
			return;

		buddyInfo->ApplyData();
	}
}

// 친구 삭제
void FGsBuddyManager::DeleteBuddy(uint64 inUserDBId)
{
	_buddyMap.Remove(inUserDBId);

	// 친구가 없으면 레드닷 삭제
	if (0 >= _buddyMap.Num())
	{
		SetBuddyTabRedDot(false);
	}
}

// 친구 신청 목록 추가
void FGsBuddyManager::AddApplyBuddy(uint64 inUserDBId, const FString& inUserName, WorldId inWorldId)
{
	if (!_applyBuddyMap.Contains(inUserDBId))
	{
		_applyBuddyMap.Emplace(inUserDBId, MakeShareable(new FGsBuddyInfo(inUserDBId, inUserName, inWorldId)));
	}
}

// 친구 신청 목록 삭제
void FGsBuddyManager::DeleteApplyBuddy(uint64 inUserDBId)
{
	_applyBuddyMap.Remove(inUserDBId);
}

// 친구 수락 목록 추가
void FGsBuddyManager::AddAcceptBuddy(uint64 inUserDBId, const FString& inUserName, WorldId InWorldId)
{
	if (!_acceptBuddyMap.Contains(inUserDBId))
	{
		_acceptBuddyMap.Emplace(inUserDBId, MakeShareable(new FGsBuddyInfo(inUserDBId, inUserName, InWorldId)));
	}
}

// 친구 수락 목록 삭제
void FGsBuddyManager::DeleteAcceptBuddy(uint64 inUserDBId)
{
	_acceptBuddyMap.Remove(inUserDBId);

	// 더이상 수락 할 목록이 없으면 레드닷 삭제
	if (0 >= _acceptBuddyMap.Num())
	{
		SetBuddyAcceptTabRedDot(false);
	}
}

void FGsBuddyManager::ChangeBuddyNickName(uint64 inUserDBId, const FString& inUserName, WorldId inWorldId)
{
	if (TSharedPtr<FGsBuddyInfo>* buddyPtr = _buddyMap.Find(inUserDBId))
	{
		TSharedPtr<FGsBuddyInfo> buddy = *buddyPtr;
		if (buddy.IsValid())
		{
			if (buddy->GetWorldId() == inWorldId)
			{
				buddy->SetChangeNickName(inUserName);
			}
		}
	}

	if (TSharedPtr<FGsBuddyInfo>* buddyPtr = _applyBuddyMap.Find(inUserDBId))
	{
		TSharedPtr<FGsBuddyInfo> buddy = *buddyPtr;
		if (buddy.IsValid())
		{
			if (buddy->GetWorldId() == inWorldId)
			{
				buddy->SetChangeNickName(inUserName);
			}
		}
	}

	if (TSharedPtr<FGsBuddyInfo>* buddyPtr = _acceptBuddyMap.Find(inUserDBId))
	{
		TSharedPtr<FGsBuddyInfo> buddy = *buddyPtr;
		if (buddy.IsValid())
		{
			if (buddy->GetWorldId() == inWorldId)
			{
				buddy->SetChangeNickName(inUserName);
			}
		}
	}
}

// 친구 목록 비우기
void FGsBuddyManager::ClearBuddyList()
{
	_buddyMap.Empty();
}

// 친구 수락 목록 비우기
void FGsBuddyManager::ClearAcceptBuddyList()
{
	_acceptBuddyMap.Empty();
}

// 친구 신청 목록 비우기
void FGsBuddyManager::ClearApplyBuddyList()
{
	_applyBuddyMap.Empty();
}

// 친구 추가가 가능한지 체크
bool FGsBuddyManager::CheckIsAdded(const FString& inUserName)
{	
	// 유효한 이름인치 체크
	if (false == IsValidName(inUserName))
		return false;

	// 친구 리스트가 꽉찼는지 체크
	if (true == IsBuddyListFull())
		return false;

	// 친구 신청목록 리스트가 꽉찼는지 체크
	if(true == IsBuddyApplyListFull())
		return false;

	// 추가하려는 대상이 "나" 인가?
	if(true == IsMySelf(inUserName))
		return false;

	// 이미 있는 친군지 체크
	if (true == IsInBuddyName(inUserName))
		return false;

	// 이미 신청한 친군지 체크
	if (true == IsInApplyBuddyName(inUserName))
		return false;

	// 이미 친구가 날 초대 했는지?
	if (true == IsInAcceptBuddyName(inUserName))
		return false;

	return true;
}

// 친구 수락이 가능한지 체크
bool FGsBuddyManager::CheckIsAccept(const FString& inUserName)
{
	// 친구 리스트가 꽉찼는지 체크
	if (true == IsBuddyListFull())
		return false;

	// 이미 있는 친군지 체크
	if (true == IsInBuddyName(inUserName))
		return false;

	return true;
}

// 이미 있는 친군지 체크
bool FGsBuddyManager::IsInBuddyName(const FString& inUserName, bool inIsShowTicker)
{
	for (auto& iter : _buddyMap)
	{
		TSharedPtr<FGsBuddyInfo> buddyInfo = iter.Value;
		if (buddyInfo.IsValid())
		{
			if (inUserName.Equals(buddyInfo->GetBuddyUserNamePrefix()))
			{
				if (inIsShowTicker)
				{
					// 이미 친구 관계인 유저에게는 친구 신청을 할 수 없습니다.
					FText findText;
					FText::FindText(TEXT("FriendText"), TEXT("FriendFailureTicker2"), findText);
					FGsUIHelper::TrayMessageTicker(findText);
				}

				return true;
			}
		}
	}

	return false;
}

// 이미 신청한 친군지 체크
bool FGsBuddyManager::IsInApplyBuddyName(const FString& inUserName)
{
	for (auto& iter : _applyBuddyMap)
	{
		TSharedPtr<FGsBuddyInfo> buddyInfo = iter.Value;
		if (buddyInfo.IsValid())
		{
			if (inUserName.Equals(buddyInfo->GetBuddyUserNamePrefix()))
			{
				// 이미 친구 신청을 보낸 유저입니다.
				FText findText;
				FText::FindText(TEXT("FriendText"), TEXT("FriendFailureTicker8"), findText);
				FGsUIHelper::TrayMessageTicker(findText);

				return true;
			}
		}
	}

	return false;
}

// 이미 요청받은 친군지 체크
bool FGsBuddyManager::IsInAcceptBuddyName(const FString& inUserName)
{
	for (auto& iter : _acceptBuddyMap)
	{
		TSharedPtr<FGsBuddyInfo> buddyInfo = iter.Value;
		if (buddyInfo.IsValid())
		{
			if (inUserName.Equals(buddyInfo->GetBuddyUserNamePrefix()))
			{
				// 이미 친구 관계인 유저에게는 친구 신청을 할 수 없습니다.
				FText findText;
				FText::FindText(TEXT("FriendText"), TEXT("FriendFailureTicker9"), findText);
				FGsUIHelper::TrayMessageTicker(findText);

				return true;
			}
		}
	}

	return false;
}

// 추가하려는 대상이 "나" 인가?
bool FGsBuddyManager::IsMySelf(const FString& inUserName)
{
	const FGsNetUserData* userData = GGameData()->GetUserData();
	if (nullptr == userData)
		return true;

	if (inUserName.Equals(userData->mUserName))
	{
		// 자기 자신에게는 친구 신청을 할 수 없습니다.
		FText findText;
		FText::FindText(TEXT("FriendText"), TEXT("FriendFailureTicker1"), findText);
		FGsUIHelper::TrayMessageTicker(findText);

		return true;
	}

	return false;
}

// 친구 리스트가 꽉찼는지 체크
bool FGsBuddyManager::IsBuddyListFull() const
{
	if (MAX_BUDDY_COUNT <= _buddyMap.Num())
	{
		// 더 이상 친구 신청을 보낼 수 없습니다.
		FText findText;
		FText::FindText(TEXT("FriendText"), TEXT("FriendFailureTicker4"), findText);
		FGsUIHelper::TrayMessageTicker(findText);

		return true;
	}

	return false;
}

// 친구 신청목록 리스트가 꽉찼는지 체크
bool FGsBuddyManager::IsBuddyApplyListFull() const
{
	if (MAX_BUDDY_INVITATION_COUNT <= _applyBuddyMap.Num())
	{
		// 내 친구 신청 목록이 가득 차 친구 신청이 불가능합니다.
		FText findText;
		FText::FindText(TEXT("FriendText"), TEXT("FriendFailureTicker5"), findText);
		FGsUIHelper::TrayMessageTicker(findText);

		return true;
	}

	return false;
}

// 유효한 이름인치 체크
bool FGsBuddyManager::IsValidName(const FString& inUserName)
{
	FString outUserName;
	FGsTextUtil::EUserNameValid eValid = FGsTextUtil::CheckedUserNameLengthLimit(inUserName, outUserName);	

	// 최대 자수 넘음
	if (FGsTextUtil::EUserNameValid::InvalidLength == eValid)
	{
		// TEXT: 글자 수 조건이 맞지 않습니다.
		FText textMsg;
		FText::FindText(TEXT("GuildText"), TEXT("Alert_WrongTextLength"), textMsg);
		FGsUIHelper::PopupOK(textMsg);
		return false;
	}
	// 최소 자수 보다 적거나 같음
	else if (FGsTextUtil::EUserNameValid::InvalidLength == eValid)
	{
		// TEXT: 지원하지 않는 문자가 포함 되어있습니다.
		FText textMsg;
		FText::FindText(TEXT("GuildText"), TEXT("Alert_WrongLanguage"), textMsg);
		FGsUIHelper::PopupOK(textMsg);
		return false;
	}

	return true;
}

// 맞친구 리스트
void FGsBuddyManager::GetBuddyList(EGsBuddyTabType inTabType, EGsBuddySubTabType inSubTabType, OUT TArray<TSharedPtr<FGsBuddyInfo>>& outList)
{
	outList.Empty();

	TMap<UserDBId, TSharedPtr<FGsBuddyInfo>> buddyMap;
	switch (inTabType)
	{
	case EGsBuddyTabType::Buddy:
		{
			switch (inSubTabType)
			{
			case EGsBuddySubTabType::All:
				GetBuddyAllList(outList);
				break;
			case EGsBuddySubTabType::My:
				GetBuddyMyList(outList);
				break;
			case EGsBuddySubTabType::Other:
				GetBuddyOtherList(outList);
				break;
			default:
				break;
			}		

			return;
		}		
		break;
	case EGsBuddyTabType::Apply:
		buddyMap = _applyBuddyMap;
		break;
	case EGsBuddyTabType::Accpept:
		buddyMap = _acceptBuddyMap;
		break;
	default:
		break;
	}
	
	for (auto& iter : buddyMap)
	{
		TSharedPtr<FGsBuddyInfo> buddyInfo = iter.Value;
		if (buddyInfo.IsValid())
		{
			outList.Emplace(buddyInfo);
		}
	}
}

void FGsBuddyManager::GetBuddyAllList(OUT TArray<TSharedPtr<FGsBuddyInfo>>& outList)
{
	outList.Empty();

	// 모든 친구 목록 정렬 기준
	// 1. 접속 중인 유저 중 내 서버 이름 오름차순으로 정렬(ㄱ~ㅎ)
	// 2. 접속 중인 유저 중 다른 서버 이름 오름차순으로 정렬(ㄱ~ㅎ)
	// 3. 미접속인 유저 중 내 서버 유저는 미접속 시간이 짧은 순서로 정렬
	// 4. 미접속인 유저 중 다른 서버 유저는 이름 오름차순으로 정렬(ㄱ~ㅎ)

	TArray<TSharedPtr<FGsBuddyInfo>> onLineMyMap, onLineMap;
	TArray<TSharedPtr<FGsBuddyInfo>> offLineMyMap, offLineMap;
	TArray<TSharedPtr<FGsBuddyInfo>> unknownMap;

	for (auto& iter : _buddyMap)
	{
		TSharedPtr<FGsBuddyInfo> info = iter.Value;

		switch (info->GetOnLineStateType())
		{
		case EBuddyOnLineStateType::ONLINE:
		{
			if (info->IsHomeWorldBuddys())
				onLineMyMap.Emplace(info);
			else
				onLineMap.Emplace(info);
		}
		break;
		case EBuddyOnLineStateType::OFFLINE:
		{
			offLineMap.Emplace(info);
		}
		break;
		case EBuddyOnLineStateType::UNKNOWN:
		{
			unknownMap.Emplace(info);
		}
		break;
		case EBuddyOnLineStateType::MAX:
			break;
		default:
			break;
		}
	}

	onLineMyMap.Sort([](const TSharedPtr<FGsBuddyInfo>& A, const TSharedPtr<FGsBuddyInfo>& B)
		{
			return A->GetBuddyUserNamePrefix() < B->GetBuddyUserNamePrefix();
		});

	onLineMap.Sort([](const TSharedPtr<FGsBuddyInfo>& A, const TSharedPtr<FGsBuddyInfo>& B)
		{
			return A->GetBuddyUserNamePrefix() < B->GetBuddyUserNamePrefix();
		});

	unknownMap.Sort([](const TSharedPtr<FGsBuddyInfo>& A, const TSharedPtr<FGsBuddyInfo>& B)
		{
			return A->GetBuddyUserNamePrefix() < B->GetBuddyUserNamePrefix();
		});

	offLineMap.Sort([](const TSharedPtr<FGsBuddyInfo>& A, const TSharedPtr<FGsBuddyInfo>& B)
		{
			return A->GetElapsedTime() > B->GetElapsedTime();
		});	

	outList += onLineMyMap;
	outList += onLineMap;
	outList += offLineMap;
	outList += unknownMap;	
}

void FGsBuddyManager::GetBuddyMyList(OUT TArray<TSharedPtr<FGsBuddyInfo>>& outList)
{
	outList.Empty();

	// 내 서버 친구 목록 정렬 기준
	// 1. 접속 중인 유저 중 오름차순으로 정렬(ㄱ~ㅎ)
	// 2. 미접속인 유저 중 미접속 시간이 짧은 순서로 정렬

	TArray<TSharedPtr<FGsBuddyInfo>> onLineMap;
	TArray<TSharedPtr<FGsBuddyInfo>> offLineMap;

	for (auto& iter : _buddyMap)
	{
		TSharedPtr<FGsBuddyInfo> info = iter.Value;

		switch (info->GetOnLineStateType())
		{
		case EBuddyOnLineStateType::ONLINE:
		{
			if (info->IsHomeWorldBuddys())
				onLineMap.Emplace(info);
		}
		break;
		case EBuddyOnLineStateType::OFFLINE:
		{
			if (info->IsHomeWorldBuddys())
				offLineMap.Emplace(info);
		}
		break;
		case EBuddyOnLineStateType::UNKNOWN:		
			break;
		case EBuddyOnLineStateType::MAX:
			break;
		default:
			break;
		}
	}

	onLineMap.Sort([](const TSharedPtr<FGsBuddyInfo>& A, const TSharedPtr<FGsBuddyInfo>& B)
		{
			return A->GetBuddyUserNamePrefix() < B->GetBuddyUserNamePrefix();
		});

	offLineMap.Sort([](const TSharedPtr<FGsBuddyInfo>& A, const TSharedPtr<FGsBuddyInfo>& B)
		{
			return A->GetElapsedTime() > B->GetElapsedTime();
		});

	outList += onLineMap;
	outList += offLineMap;
}

void FGsBuddyManager::GetBuddyOtherList(OUT TArray<TSharedPtr<FGsBuddyInfo>>& outList)
{
	outList.Empty();

	// 다른 서버 친구 목록 정렬 기준
	// 1. 접속 중인 유저 중 오름차순으로 정렬(ㄱ~ㅎ)
	// 2. 알수없음 유저 중 오름차순으로 정렬(ㄱ~ㅎ)

	TArray<TSharedPtr<FGsBuddyInfo>> onLineMap;
	TArray<TSharedPtr<FGsBuddyInfo>> unknownMap;

	for (auto& iter : _buddyMap)
	{
		TSharedPtr<FGsBuddyInfo> info = iter.Value;

		switch (info->GetOnLineStateType())
		{
		case EBuddyOnLineStateType::ONLINE:
		{
			if (!info->IsHomeWorldBuddys())
				onLineMap.Emplace(info);
		}
		break;
		case EBuddyOnLineStateType::OFFLINE:		
		break;
		case EBuddyOnLineStateType::UNKNOWN:
		{
			if (!info->IsHomeWorldBuddys())
				unknownMap.Emplace(info);
		}
		break;
		case EBuddyOnLineStateType::MAX:
			break;
		default:
			break;
		}
	}

	onLineMap.Sort([](const TSharedPtr<FGsBuddyInfo>& A, const TSharedPtr<FGsBuddyInfo>& B)
		{
			return A->GetWorldId() < B->GetWorldId();
		});

	unknownMap.Sort([](const TSharedPtr<FGsBuddyInfo>& A, const TSharedPtr<FGsBuddyInfo>& B)
		{
			return A->GetWorldId() < B->GetWorldId();
		});

	outList += onLineMap;
	outList += unknownMap;
}

void FGsBuddyManager::SetBuddyTabRedDot(bool inVisible)
{
	if (_isBuddyTabRedDot == inVisible)
		return;

	if (false == inVisible)
	{
		FGsNetSendService::SendBuddyTabRemoveRedDot(BuddyTabType::Buddy);
	}

	_isBuddyTabRedDot = inVisible;

	// 햄버거 메뉴 레드닷 업데이트
	GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::BUDDY, _isBuddyTabRedDot | _isBuddyAcceptTabRedDot);
}

void FGsBuddyManager::SetBuddyAcceptTabRedDot(bool inVisible)
{
	if (_isBuddyAcceptTabRedDot == inVisible)
		return;

	if (false == inVisible)
	{
		FGsNetSendService::SendBuddyTabRemoveRedDot(BuddyTabType::RecvInvitation);
	}

	_isBuddyAcceptTabRedDot = inVisible;

	// 햄버거 메뉴 레드닷 업데이트
	GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::BUDDY, _isBuddyTabRedDot | _isBuddyAcceptTabRedDot);
}

// 친구가 로그인 했을 때
void FGsBuddyManager::SetBuddyLogin(IN UserDBId inUserDBId)
{
	if (!_buddyMap.Contains(inUserDBId))
		return;

	TSharedPtr<FGsBuddyInfo> buddyInfo = _buddyMap.FindRef(inUserDBId);
	if (!buddyInfo.IsValid())
		return;

	buddyInfo->SetOnLineStateType(EBuddyOnLineStateType::ONLINE);
}

// 친구가 로그아웃 했을 때
void FGsBuddyManager::SetBuddyLogout(IN UserDBId inUserDBId)
{
	const FGsNetUserData* userData = GGameData()->GetUserData();
	if (nullptr == userData)
		return;

	if (!_buddyMap.Contains(inUserDBId))
		return;

	TSharedPtr<FGsBuddyInfo> buddyInfo = _buddyMap.FindRef(inUserDBId);
	if (!buddyInfo.IsValid())
		return;

	if (buddyInfo->GetWorldId() != userData->mHomeWorldId)
	{
		buddyInfo->SetOnLineStateType(EBuddyOnLineStateType::UNKNOWN);
	}
	else
	{
		buddyInfo->SetOnLineStateType(EBuddyOnLineStateType::OFFLINE);
	}
	
	buddyInfo->SetElapsedTime(FGsTimeSyncUtil::GetServerNowSecond());
	
}

bool FGsBuddyManager::GetBuddyName(IN UserDBId inUserDBId, OUT FString& outBuddyName)
{
	if (!_buddyMap.Contains(inUserDBId))
		return false;

	TSharedPtr<FGsBuddyInfo> buddyInfo = _buddyMap.FindRef(inUserDBId);
	if (!buddyInfo.IsValid())
		return false;

	outBuddyName = buddyInfo->GetBuddyUserNamePrefix();

	return true;
}

// BuddyHandler
FGsBuddyHandler* FGsBuddyManager::GetBuddyHandler()
{
	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	if (nullptr == gameFlow)
	{
		return nullptr;
	}

	FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame();
	if (nullptr == contents)
	{
		return nullptr;
	}

	FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
	if (nullptr == hud)
	{
		return nullptr;
	}

	return hud->GetBuddyHandler();
}