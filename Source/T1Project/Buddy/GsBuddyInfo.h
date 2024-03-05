#pragma once
#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UTIL/GsText.h"

UENUM(BlueprintType)
// 커뮤니티 이벤트 타임 타입
enum class EBuddyOnLineStateType : uint8
{
	// 접속
	ONLINE = 0,

	// 비 접속
	OFFLINE = 1,

	// 알수없음
	UNKNOWN = 2,

	MAX = 3,
};

/*
	친구 정보
*/
class FGsBuddyInfo
{
private:
	// 유저 db 아이디
	UserDBId _userDBId;
	// Prefix 규칙을 거치지 않은 이름
	FString _userName;
	// Prefix 규칙을 거친 이름
	FString _userNamePrefix;
	// 마지막 접속 일로 부터의 흐른시간
	time_t _elapsedTime;
	// 접속 서버
	WorldId _worldId;
	// 접속 상태
	EBuddyOnLineStateType _onLineState;
	// 나와 홈월드가 같은가?
	bool _isHomeWorldBuddys = true;

public:
	FGsBuddyInfo() = default;
	 ~FGsBuddyInfo() = default;
	 explicit FGsBuddyInfo(UserDBId InUserDBId, FString InUserName)
		 : _userDBId(InUserDBId), _userName(InUserName), _userNamePrefix(InUserName)
	 {		 
	 }
	explicit FGsBuddyInfo(UserDBId InUserDBId, FString InUserName, WorldId InWorldId, bool inIsHomeWorldBuddys = true)
		 : _userDBId(InUserDBId), _worldId(InWorldId), _isHomeWorldBuddys(inIsHomeWorldBuddys)
	 {
		_userName = InUserName;
		_userNamePrefix = FGsTextUtil::CombineUserAndPrefix(FText::FromString(InUserName), InWorldId);
	 }
	explicit FGsBuddyInfo(UserDBId InUserDBId, FString InUserName, time_t InElapsedTime, WorldId InWorldId, bool inIsHomeWorldBuddys = true)
		: _userDBId(InUserDBId), _elapsedTime(InElapsedTime), _worldId(InWorldId), _isHomeWorldBuddys(inIsHomeWorldBuddys)
	{
		_userName = InUserName;
		_userNamePrefix = FGsTextUtil::CombineUserAndPrefix(FText::FromString(InUserName), InWorldId);
	}

 public:
	 void ApplyData()
	 {
		 if (0 == _elapsedTime)
		 {
			 // 온라인
			 SetOnLineStateType(EBuddyOnLineStateType::ONLINE);
		 }
		 else if (-1 == _elapsedTime)
		 {
			 // 알수없음
			 SetOnLineStateType(EBuddyOnLineStateType::UNKNOWN);
		 }
		 else if (0 < _elapsedTime)
		 {
			 // 오프라인
			 SetOnLineStateType(EBuddyOnLineStateType::OFFLINE);
		 }
		 else
		 {
			 // error
		 }
	 }

	// getter
 	const UserDBId GetUserDBId() const { return _userDBId; }
	const FString& GetBuddyUserName() const { return _userName; }
	const FString& GetBuddyUserNamePrefix() const { return _userNamePrefix; }
	const time_t& GetElapsedTime() const { return _elapsedTime; }
	const WorldId GetWorldId() const { return _worldId; }
	const EBuddyOnLineStateType& GetOnLineStateType() const { return _onLineState; }
	bool IsHomeWorldBuddys() { return _isHomeWorldBuddys; }

	// setter
	void SetOnLineStateType(EBuddyOnLineStateType InStateType) { _onLineState = InStateType; }
	void SetElapsedTime(const time_t& InElapsedTime) { _elapsedTime = InElapsedTime; }
	void SetChangeNickName(const FString& InUserName)
	{
		_userName = InUserName;
		_userNamePrefix = FGsTextUtil::CombineUserAndPrefix(FText::FromString(InUserName), _worldId);
	}
};
