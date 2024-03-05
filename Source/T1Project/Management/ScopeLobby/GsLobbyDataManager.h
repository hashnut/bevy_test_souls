// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsManager.h"
#include "GsLobbyDataType.h"
#include "Shared/Shared/SharedPackets/PD_Auth_Client.h"
#include "Shared/Shared/SharedPackets/PD_Gateway_Client.h"
#include "Management/GsScopeHolder.h"
#include "Lobby/GsSchemaLobbyConfig.h"
#include "Data/GsLocalizationConstant.h"
#include "Data/GsLobbyConstant.h"
#include "GsLobbyDataManager.generated.h"

using namespace LobbyDataType;

struct ServerElem
{
	explicit ServerElem(
		uint16 planetWorldId, uint8 orderNum, uint16 userCount, uint8 congestion, bool charCreatable,
		bool recommended, bool newOpened, const FString& hiveAnalyticsServerId, const FString& appendixName
	)
		:
		mPlanetWorldId(planetWorldId), mOrderNum(orderNum), mUserCount(userCount), mCongestion(congestion),
		mCharCreatable(charCreatable), mRecommended(recommended), mNewOpened(newOpened), mHiveAnalyticsServerId(hiveAnalyticsServerId),
		mAppendixName(appendixName)
	{

	}

	uint16	mPlanetWorldId; // SharedType 에 union 형식으로 정의되어 있다
	uint32	mOrderNum;
	uint16	mUserCount;
	uint8	mCongestion;
	bool	mCharCreatable;
	bool	mRecommended;
	bool	mNewOpened;
	FString mHiveAnalyticsServerId;
	FString mAppendixName;
};

struct T1PROJECT_API FGsNetLobbyData
{
	TArray<ServerElem> ListServer;

	TArray<ServerElem> GetServerList()
	{
		return ListServer;
	}
};
/**
 *
 */
class UGsLobbyConstantData;
class UGsLobbyConstant;
class UGsLocalizationConstant;
struct FGsSchemaLobbyConfig;
UCLASS()
class T1PROJECT_API UGsLobbyDataManager : public UObject, public IGsManager
{
	GENERATED_BODY()

private:
	FGsNetLobbyData _lobbyData;

private:
	TArray<LobbyDataType::FGsLobbyUserListData>	_lobbyUserListData;

private:
	FString _newUserName;
	bool	_isDeleteUser = false;

	uint16 _lastPlanetWorldId = 0;
	uint16 _bestPlanetWorldId = 0;

	const FGsSchemaLobbyConfig* _lobbyConfig = nullptr;

	// 로비 데이타
	UPROPERTY()
	const UGsLobbyConstantData* _lobbyConstantData;

	UPROPERTY()
	const UGsLobbyConstant* _lobbyConstant;

	UPROPERTY()
	const UGsLocalizationConstant* _lobbyLocalizationConstant;

	// 기본 소유 캐릭터 슬롯 개수	
	int _characterFreeSlotCount = 0;
	// 캐릭터 슬롯을 유료로 확장한 개수.
	int	_characterPaidSlotCount = 0;
	

	bool _isCharacterEmpty = false;
	bool _isBackToCharacterSelect = false;

	// 커스터마이징 변경 아이템 개수
	int _customizingTicketCount = 0;
	// 커스터마이징 할 캐릭터
	UserDBId _customizingUserDBId = INVALID_USER_DB_ID;

public:
	UGsLobbyDataManager();
	virtual ~UGsLobbyDataManager();

public:
	//IGsManager
	virtual void Initialize() override;
	virtual void Finalize() override;

private:
	void SortingUserList(int inStartIndex, int inUserCount);
	bool SwapNewUserFirstList(int inUserCount);

	const struct FGsSchemaLobbyConfig* GetLobbyConfig();

public:	
	bool Set(PD::GC::PKT_GC_ACK_USER_LIST_READ& Packet);
	TArray<LobbyDataType::FGsLobbyUserListData>& GetUserList() { return _lobbyUserListData; }

	void SetNewUserName(const FString& inNewUserName) { _newUserName = inNewUserName; }
	const FString& GetNewUserName() { return _newUserName; }
	void ClearNewUserName() { _newUserName.Empty(0); }
	void SetIsDeleteUser(bool inValue) { _isDeleteUser = inValue; }
	const bool GetIsDeleteUser() { return _isDeleteUser; }

	void SetIsBackToCharacterSelect(bool inValue) { _isBackToCharacterSelect = inValue; }
	bool GetIsBackToCharacterSelect();

	void SetCustomizingUserDBId(IN UserDBId inUserDBId) { _customizingUserDBId = inUserDBId; }
	const UserDBId GetCustomizingUserDBId() { return _customizingUserDBId; }
	void ClearCustomizingUserDBId() { _customizingUserDBId = INVALID_USER_DB_ID; }
	bool IsChangeCustomizing() { return (INVALID_USER_DB_ID < _customizingUserDBId) ? true : false; }

public:
	const int GetCharacterFreeSlotCount() { return _characterFreeSlotCount; }
	const int GetCharacterPaidOpenSlotCount() { return _characterPaidSlotCount; }
	const int GetCustomizingTicketCount() { return _customizingTicketCount; }
	const int GetCharacterSlotCount() { return (nullptr == GetLobbyConfig()) ? 0 : _lobbyConfig->characterSlotCount; }
	const int GetCharacterPaidSlotCount() { return (nullptr == GetLobbyConfig()) ? 0 : _lobbyConfig->characterPaidSlotCount; }
	const int GetDeleteUserMinLevel() { return (nullptr == GetLobbyConfig())? 0 : _lobbyConfig->deleteUserMinLevel; }
	const int GetDeletionCooldownHour() { return (nullptr == GetLobbyConfig()) ? 0 : _lobbyConfig->deletionCooldownHour; }
		
	const UGsLobbyConstant* GetLobbyConstantData() const { return _lobbyConstant; }
	const UGsLocalizationConstant* GetLocalizationConstantData() const { return _lobbyLocalizationConstant; }

	const bool GetIsCharacterEmpty() const { return _isCharacterEmpty; }
};

#define LSLobbyData() UGsScopeHolder::GetLobbyManager<UGsLobbyDataManager>(UGsScopeLobby::EManagerType::LobbyData)