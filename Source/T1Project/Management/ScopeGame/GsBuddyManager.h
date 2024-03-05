// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsManager.h"
#include "Buddy/GsBuddyDefine.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"


/**
 *
 */
class FGsBuddyHandler;
class FGsBuddyInfo;
class T1PROJECT_API FGsBuddyManager final : public IGsManager
{
private:
	TMap<UserDBId, TSharedPtr<FGsBuddyInfo>> _buddyMap;			// 친구 목록
	TMap<UserDBId, TSharedPtr<FGsBuddyInfo>> _applyBuddyMap;	// 친구 신청 목록
	TMap<UserDBId, TSharedPtr<FGsBuddyInfo>> _acceptBuddyMap;	// 친구 수락 목록	

	bool _isBuddyTabRedDot;
	bool _isBuddyAcceptTabRedDot;

public:
	//IGsManager
	virtual void Finalize() override;

protected:
	void Clear();

public:
	void InitializeUserData();
	// 친구 추가
	void AddBuddy(uint64 inUserDBId, const FString& inUserName, time_t inElapsedTime, WorldId inWorldId = INVALID_WORLD_ID);
	// 맞친구 삭제
	void DeleteBuddy(uint64 inUserDBId);
	// 친구 신청 목록 추가
	void AddApplyBuddy(uint64 inUserDBId, const FString& inUserName, WorldId inWorldId = INVALID_WORLD_ID);
	// 친구 신청 목록 삭제
	void DeleteApplyBuddy(uint64 inUserDBId);
	// 친구 수락 목록 추가
	void AddAcceptBuddy(uint64 inUserDBId, const FString& inUserName, WorldId InWorldId = INVALID_WORLD_ID);
	// 친구 수락 목록 삭제
	void DeleteAcceptBuddy(uint64 inUserDBId);
	// 친구 닉네임 변경
	void ChangeBuddyNickName(uint64 inUserDBId, const FString& inUserName, WorldId inWorldId);

public:
	// 친구 목록 비우기
	void ClearBuddyList();
	// 친구 수락 목록 비우기
	void ClearAcceptBuddyList();
	// 친구 신청 목록 비우기
	void ClearApplyBuddyList();

public:
	// 친구 추가가 가능한지 체크
	bool CheckIsAdded(const FString& inUserName);
	// 친구 수락이 가능한지 체크
	bool CheckIsAccept(const FString& inUserName);
	// 이미 있는 친군지 체크
	bool IsInBuddyName(const FString& inUserName, bool inIsShowTicker = true);
	// 이미 신청한 친군지 체크
	bool IsInApplyBuddyName(const FString& inUserName);
	// 이미 요청받은 친군지 체크
	bool IsInAcceptBuddyName(const FString& inUserName);
	// 친구 리스트가 꽉찼는지 체크
	bool IsBuddyListFull() const;
	// 친구 신청목록 리스트가 꽉찼는지 체크
	bool IsBuddyApplyListFull() const;
	// 추가하려는 대상이 "나" 인가?
	bool IsMySelf(const FString& inUserName);
	// 유효한 이름인치 체크
	bool IsValidName(const FString& inUserName);

public:
	// 맞친구 리스트
	void GetBuddyList(EGsBuddyTabType inTabType, EGsBuddySubTabType inSubTabType, OUT TArray<TSharedPtr<FGsBuddyInfo>>& outList);
	void GetBuddyAllList(OUT TArray<TSharedPtr<FGsBuddyInfo>>& outList);
	void GetBuddyMyList(OUT TArray<TSharedPtr<FGsBuddyInfo>>& outList);
	void GetBuddyOtherList(OUT TArray<TSharedPtr<FGsBuddyInfo>>& outList);

	// 친구 목록 카운트
	int GetCountBuddyList() { return _buddyMap.Num(); }
	// 친구 신청 목록 카운트
	int GetCountApplyBuddyList() { return _applyBuddyMap.Num(); }
	// 친구 수락 목록 카운트
	int GetCountAcceptBuddyList() { return _acceptBuddyMap.Num(); }

public:
	void SetBuddyTabRedDot(bool inVisible);
	void SetBuddyAcceptTabRedDot(bool inVisible);

	// 로그인/로그아웃 처리
	void SetBuddyLogin(IN UserDBId inUserDBId);
	void SetBuddyLogout(IN UserDBId inUserDBId);

	bool GetBuddyTabRedDot() { return _isBuddyTabRedDot; }
	bool GetBuddyAcceptTabRedDot() { return _isBuddyAcceptTabRedDot; }

	// 친구 이름 얻기
	bool GetBuddyName(IN UserDBId inUserDBId, OUT FString& outBuddyName);

	bool IsRedDot() const { return _isBuddyTabRedDot | _isBuddyAcceptTabRedDot; }

	// BuddyHandler
	FGsBuddyHandler* GetBuddyHandler();
};

#define GSBuddy() UGsScopeHolder::GetGameManagerFType<FGsBuddyManager>(UGsScopeGame::EManagerType::Buddy)
