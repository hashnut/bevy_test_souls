#pragma once
#ifdef _SERVER
#include "ServerCommon/WZPacketDef.h"
#include "TechShared/Shared/SharedInclude/PacketWriter.h"
#include "TechShared/Shared/SharedInclude/SharedTypes.h"
#else
#include "../SharedInclude/PacketWriter.h"
#include "../SharedInclude/SharedTypes.h"
#endif
namespace PD
{
namespace AC
{

#pragma warning(push)
#pragma warning(disable:4315)
#pragma warning(disable:5038)
#pragma pack(push, 1)

#ifndef _SERVER
enum class ACPacketId : uint16
#else
enum
#endif
{
	AC_PONG = 2796,
	AC_ACK_LOGIN = 2817,
	AC_ACK_SELECT_SERVER = 2781,
	AC_KICKED_BY_TIMEOUT = 2960,
	AC_ACK_CHECK_PASSWORD = 2252,
	AC_ACK_CHECK_VERSION = 2811,
	AC_ACK_APP_GUARD_AUTH = 2515,
	AC_ACK_DELETE_ACCOUNT = 2142,
	AC_ACK_PRE_LOGIN = 2936,
	AC_ACK_IDP_CONFLICT_PLYER = 2016,
	PacketIdMax  = 2999
};

#ifdef _SERVER

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// String -> Enum
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline uint16 ACPacketIdStringToEnum(const wchar_t* str) noexcept
{
	if (_wcsicmp(L"AC_PONG", str) == 0)	return AC_PONG;
	if (_wcsicmp(L"AC_ACK_LOGIN", str) == 0)	return AC_ACK_LOGIN;
	if (_wcsicmp(L"AC_ACK_SELECT_SERVER", str) == 0)	return AC_ACK_SELECT_SERVER;
	if (_wcsicmp(L"AC_KICKED_BY_TIMEOUT", str) == 0)	return AC_KICKED_BY_TIMEOUT;
	if (_wcsicmp(L"AC_ACK_CHECK_PASSWORD", str) == 0)	return AC_ACK_CHECK_PASSWORD;
	if (_wcsicmp(L"AC_ACK_CHECK_VERSION", str) == 0)	return AC_ACK_CHECK_VERSION;
	if (_wcsicmp(L"AC_ACK_APP_GUARD_AUTH", str) == 0)	return AC_ACK_APP_GUARD_AUTH;
	if (_wcsicmp(L"AC_ACK_DELETE_ACCOUNT", str) == 0)	return AC_ACK_DELETE_ACCOUNT;
	if (_wcsicmp(L"AC_ACK_PRE_LOGIN", str) == 0)	return AC_ACK_PRE_LOGIN;
	if (_wcsicmp(L"AC_ACK_IDP_CONFLICT_PLYER", str) == 0)	return AC_ACK_IDP_CONFLICT_PLYER;
	return PacketIdMax;
}
#endif
#ifdef _SERVER

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> String
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline const wchar_t* ACPacketIdEnumToString(uint16 value) noexcept
{
	switch (value)
	{
		case AC_PONG: return L"AC_PONG";
		case AC_ACK_LOGIN: return L"AC_ACK_LOGIN";
		case AC_ACK_SELECT_SERVER: return L"AC_ACK_SELECT_SERVER";
		case AC_KICKED_BY_TIMEOUT: return L"AC_KICKED_BY_TIMEOUT";
		case AC_ACK_CHECK_PASSWORD: return L"AC_ACK_CHECK_PASSWORD";
		case AC_ACK_CHECK_VERSION: return L"AC_ACK_CHECK_VERSION";
		case AC_ACK_APP_GUARD_AUTH: return L"AC_ACK_APP_GUARD_AUTH";
		case AC_ACK_DELETE_ACCOUNT: return L"AC_ACK_DELETE_ACCOUNT";
		case AC_ACK_PRE_LOGIN: return L"AC_ACK_PRE_LOGIN";
		case AC_ACK_IDP_CONFLICT_PLYER: return L"AC_ACK_IDP_CONFLICT_PLYER";
	}
	return nullptr;
}
#endif
// type = AC
struct PKT_AC_PONG_READ
{
public:
	uint16 mPktSize;
	uint16 mPktId;
	int16 mPong;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"AC_PONG"; }
	const wchar_t* GetRelatedPacketName() const { return L"CA_PONG"; }
#endif

	int16 Pong() { return mPong; }


#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{

		return Log::format(
			L"[AC_PONG] Pong: {}"
			, mPong
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_AC_PONG_READ);
		if (dataSize > mPktSize)
			return false;

		if (false == ValidateValue(pktStart, pktSize, dataSize))
			return false;

		if (dataSize != mPktSize)
			return false;

		return true;
	}

	bool ValidateValue(const uchar* pktStart, uint16 pktSize, uint16& dataSize)
	{
		return (nullptr != pktStart && 0 < pktSize && 0 < dataSize) ? true : false;
	}

}; // PKT_AC_PONG_READ

// type = AC
#ifdef _SERVER
struct PKT_AC_PONG_WRITE : public Disposable
#else
struct PKT_AC_PONG_WRITE
#endif
{
	PktWriter mPktWriter;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_AC_PONG_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"AC_PONG"; }
	const wchar_t* GetRelatedPacketName() const { return L"CA_PONG"; }
#endif

#ifndef _SERVER
	PKT_AC_PONG_WRITE(uchar* buf, int bufSize, int16 pong)
		:
		mPktWriter(buf, bufSize)

	{
		mPktWriter << static_cast<uint16>(PD::AC::ACPacketId::AC_PONG);
		mPktWriter << pong;
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_AC_PONG_WRITE(SendBufferHelper& sendBuffer, int16 pong)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))

	{
		mPktWriter << (uint16)AC_PONG;
		mPktWriter << pong;
	}
#endif

#ifdef _SERVER 
	void send(IocpObjectSession* session, SendBufferRef sendBuffer, bool autoCloseSendBuffer)
	{
		if (session) 
			session->Send(sendBuffer, mPktWriter.GetPktSize(), autoCloseSendBuffer);
	}
#endif

#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{
		auto readPacket = reinterpret_cast<PKT_AC_PONG_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
#else
#endif
}; // PKT_AC_PONG_WRITE

// type = AC
// related packet = PKT_CA_REQ_LOGIN_WRITE
struct PKT_AC_ACK_LOGIN_READ
{
	struct ServerList
	{
	protected:
		uint16 mThisOffset;
		uint16 mNextOffset;
		uint16 mAppendixNameOffset;
		uint16 mHiveAnalyticsServerIdOffset;
		uint16 mPlanetWorldId;
		uint8 mOrderNum;
		uint16 mUserCount;
		uint8 mCongestion;
		bool mCharCreatable;
		bool mRecommended;
		bool mNewOpened;

	public:
		uint16 PlanetWorldId() { return mPlanetWorldId; }
		uint8 OrderNum() { return mOrderNum; }
		uint16 UserCount() { return mUserCount; }
		uint8 Congestion() { return mCongestion; }
		bool CharCreatable() { return mCharCreatable; }
		bool Recommended() { return mRecommended; }
		bool NewOpened() { return mNewOpened; }

#ifdef _SERVER
		const wchar_t* AppendixName()
		{
			uchar* pktStart = (uchar*)this - mThisOffset;
			uint16 pktSize = *((uint16*)pktStart);
			if (mAppendixNameOffset && pktSize > mAppendixNameOffset)
				return (const wchar_t*)(pktStart + mAppendixNameOffset);
			else
				return L"";
		}
#else // _SERVER
		const FString AppendixName()
		{
			const wchar_t* strWchar = L"";
			uchar* pktStart = (uchar*)this - mThisOffset;
			uint16 pktSize = *((uint16*)pktStart);
			if (mAppendixNameOffset && pktSize > mAppendixNameOffset)
				strWchar = (const wchar_t*)(pktStart + mAppendixNameOffset);
			return PktWriter::ConvertToFString(strWchar);
		}
#endif // _SERVER
#ifdef _SERVER
		const wchar_t* HiveAnalyticsServerId()
		{
			uchar* pktStart = (uchar*)this - mThisOffset;
			uint16 pktSize = *((uint16*)pktStart);
			if (mHiveAnalyticsServerIdOffset && pktSize > mHiveAnalyticsServerIdOffset)
				return (const wchar_t*)(pktStart + mHiveAnalyticsServerIdOffset);
			else
				return L"";
		}
#else // _SERVER
		const FString HiveAnalyticsServerId()
		{
			const wchar_t* strWchar = L"";
			uchar* pktStart = (uchar*)this - mThisOffset;
			uint16 pktSize = *((uint16*)pktStart);
			if (mHiveAnalyticsServerIdOffset && pktSize > mHiveAnalyticsServerIdOffset)
				strWchar = (const wchar_t*)(pktStart + mHiveAnalyticsServerIdOffset);
			return PktWriter::ConvertToFString(strWchar);
		}
#endif // _SERVER
		friend PktIterator<ServerList>;
		friend PKT_AC_ACK_LOGIN_READ;

		bool Validate(uint16 offset, uchar * pktStart, uint16 pktSize, uint16& dataSize)
		{
			if (static_cast<size_t>(pktSize) < static_cast<size_t>(offset) + sizeof(ServerList))
				return false;

			ServerList * node = (ServerList*)(pktStart + offset);

			// 자기 자신의 포인터 검증
			if (offset != node->mThisOffset)
				return false;

			// Value 검증
			uchar* pktCurrentNodeStart = (uchar*)this - mThisOffset;
			uint16 pktCurrentNodeSize = *((uint16*)pktStart);
			dataSize += sizeof(ServerList);	// skip
			if (false == ValidateValue(pktCurrentNodeStart, pktCurrentNodeSize, dataSize))
				return false;

			return true;
		}

		bool ValidateValue(const uchar* pktStart, uint16 pktSize, uint16& dataSize)
		{
			if (false == ValidateString(mAppendixNameOffset, 0, pktStart, pktSize, dataSize))
				return false;

			if (false == ValidateString(mHiveAnalyticsServerIdOffset, 0, pktStart, pktSize, dataSize))
				return false;

			return (nullptr != pktStart && 0 < pktSize && 0 < dataSize) ? true : false;
		}

	}; // ServerList

public:
	uint16 mPktSize;
	uint16 mPktId;
	uint16 mServerListCount;
	uint16 mFirstServerListOffset;
	uint16 mAccountNameOffset;
	uint16 mAppGuardUniqueClientIDOffset;
	int mResult;
	AccountDBId mAccountId;
	uint16 mLastPlanetWorldId;
	uint16 mBestPlanetWorldId;
	int mResultDetail;
	bool mUseSyncPay;
	bool mRegisterDevice;
	uint16 mAdvReservationPlanetWorldId;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"AC_ACK_LOGIN"; }
	const wchar_t* GetRelatedPacketName() const { return L"CA_REQ_LOGIN"; }
#endif

	int Result() { return mResult; }
	AccountDBId AccountId() { return mAccountId; }
	uint16 LastPlanetWorldId() { return mLastPlanetWorldId; }
	uint16 BestPlanetWorldId() { return mBestPlanetWorldId; }
	int ResultDetail() { return mResultDetail; }
	bool UseSyncPay() { return mUseSyncPay; }
	bool RegisterDevice() { return mRegisterDevice; }
	uint16 AdvReservationPlanetWorldId() { return mAdvReservationPlanetWorldId; }
#ifdef _SERVER
	const wchar_t* AccountName() const
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = *((uint16*)pktStart);
		if (mAccountNameOffset && pktSize > mAccountNameOffset)
			return (const wchar_t*)(pktStart + mAccountNameOffset);
		else
			return L"";
	}
#else // _SERVER
	const FString AccountName()
	{
		const wchar_t* strWchar = L"";
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = *((uint16*)pktStart);
		if (mAccountNameOffset && pktSize > mAccountNameOffset)
			strWchar = (const wchar_t*)(pktStart + mAccountNameOffset);

		return PktWriter::ConvertToFString(strWchar);
	}
#endif // _SERVER
#ifdef _SERVER
	const wchar_t* AppGuardUniqueClientID() const
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = *((uint16*)pktStart);
		if (mAppGuardUniqueClientIDOffset && pktSize > mAppGuardUniqueClientIDOffset)
			return (const wchar_t*)(pktStart + mAppGuardUniqueClientIDOffset);
		else
			return L"";
	}
#else // _SERVER
	const FString AppGuardUniqueClientID()
	{
		const wchar_t* strWchar = L"";
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = *((uint16*)pktStart);
		if (mAppGuardUniqueClientIDOffset && pktSize > mAppGuardUniqueClientIDOffset)
			strWchar = (const wchar_t*)(pktStart + mAppGuardUniqueClientIDOffset);

		return PktWriter::ConvertToFString(strWchar);
	}
#endif // _SERVER

	using ServerListIterator = PktIterator<ServerList>;
	const ServerListIterator GetLastServerListIterator() const
	{
		return ServerListIterator(0);
	}
	ServerListIterator GetFirstServerListIterator() const
	{
		if (mFirstServerListOffset && mPktSize >= mFirstServerListOffset + sizeof(ServerList))
		{
			ServerList* serverList = (ServerList*)((uchar*)this + mFirstServerListOffset);
			if (mFirstServerListOffset == serverList->mThisOffset)
				return ServerListIterator(serverList);
			else
				return ServerListIterator(0);
		}
		else
			return ServerListIterator(0);
	}
	const int GetServerListCount()
	{
		return mServerListCount;
	}
#ifdef _SERVER
	template<typename TFunc>
	void ForEachServerList(TFunc&& func)
	{
		for (auto it = GetFirstServerListIterator(); it != GetLastServerListIterator(); ++it)
			func(it);
	}
#endif


#ifdef _SERVER 
	std::xwstring FuncCreateServerListStr( [[maybe_unused]] ServerListIterator& iter) const
	{
		return Log::format(
			L"PlanetWorldId: {}, AppendixName: {}, OrderNum: {}, UserCount: {}, Congestion: {}, CharCreatable: {}, Recommended: {}, NewOpened: {}, HiveAnalyticsServerId: {}", 
			iter->PlanetWorldId(), iter->AppendixName(), iter->OrderNum(), iter->UserCount(), iter->Congestion(), iter->CharCreatable(), iter->Recommended(), iter->NewOpened(), iter->HiveAnalyticsServerId()
		).c_str();
	}

	std::xwstring GetLogWString() const
	{
		std::xwstring str1;
		str1.append(L"\n");
		for (ServerListIterator iter = this->GetFirstServerListIterator();
			iter != GetLastServerListIterator(); ++iter)
		{
			str1.append(L"[");
			str1.append(FuncCreateServerListStr(iter).c_str());

			str1.append(L"]\n");
		}

		return Log::format(
			L"[AC_ACK_LOGIN] Result: {}, AccountId: {}, AccountName: {}, LastPlanetWorldId: {}, BestPlanetWorldId: {}, ResultDetail: {}, AppGuardUniqueClientID: {}, UseSyncPay: {}, RegisterDevice: {}, AdvReservationPlanetWorldId: {}, ServerList : {}"
			, mResult, mAccountId, AccountName(), mLastPlanetWorldId, mBestPlanetWorldId, mResultDetail, AppGuardUniqueClientID(), mUseSyncPay, mRegisterDevice, mAdvReservationPlanetWorldId, str1.c_str()
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_AC_ACK_LOGIN_READ);
		if (dataSize > mPktSize)
			return false;

		if (false == ValidateValue(pktStart, pktSize, dataSize))
			return false;

		if (false == ServerListValidateListNode(mFirstServerListOffset, pktStart, pktSize, dataSize))
			return false;

		if (dataSize != mPktSize)
			return false;

		return true;
	}

	bool ServerListValidateListNode(uint16 offset, uchar * pktStart, uint16 pktSize, uint16& dataSize)
	{		for (ServerListIterator iter = this->GetFirstServerListIterator();
			iter != GetLastServerListIterator(); ++iter)
		{
			if (false == (*iter).Validate(offset, pktStart, pktSize, dataSize))
			{
				return false;
			}
		}
		return true;
	}

	bool ValidateValue(const uchar* pktStart, uint16 pktSize, uint16& dataSize)
	{
		if (false == ValidateString(mAccountNameOffset, 0, pktStart, pktSize, dataSize))
			return false;

		if (false == ValidateString(mAppGuardUniqueClientIDOffset, 0, pktStart, pktSize, dataSize))
			return false;

		return (nullptr != pktStart && 0 < pktSize && 0 < dataSize) ? true : false;
	}

}; // PKT_AC_ACK_LOGIN_READ

// type = AC
// related packet = PKT_CA_REQ_LOGIN_READ
#ifdef _SERVER
struct PKT_AC_ACK_LOGIN_WRITE : public Disposable
#else
struct PKT_AC_ACK_LOGIN_WRITE
#endif
{
	struct ServerList
	{
		ServerList(uint16 offset, uint16 planetWorldId, uint8 orderNum, uint16 userCount, uint8 congestion, bool charCreatable, bool recommended, bool newOpened)
			:
			mThisOffset(offset)
			, mNextOffset(0), mAppendixNameOffset(0), mHiveAnalyticsServerIdOffset(0)
			, mPlanetWorldId(planetWorldId)
			, mOrderNum(orderNum)
			, mUserCount(userCount)
			, mCongestion(congestion)
			, mCharCreatable(charCreatable)
			, mRecommended(recommended)
			, mNewOpened(newOpened)
		{
		}
		void SetAppendixNameOffset(uint16 offset)
		{
			mAppendixNameOffset = offset;
		}
		void SetHiveAnalyticsServerIdOffset(uint16 offset)
		{
			mHiveAnalyticsServerIdOffset = offset;
		}

		uint16 mThisOffset;
		uint16 mNextOffset;
		uint16 mAppendixNameOffset;
		uint16 mHiveAnalyticsServerIdOffset;
		uint16 mPlanetWorldId;
		uint8 mOrderNum;
		uint16 mUserCount;
		uint8 mCongestion;
		bool mCharCreatable;
		bool mRecommended;
		bool mNewOpened;

	}; // ServerList

	struct ServerListHandle
	{
		ServerListHandle(PktWriter& pktWriter, ServerList* serverList)
			:
			mPktWriter(pktWriter)
			, mServerList(serverList)
		{
		}

		PktWriter& mPktWriter;
		ServerList* mServerList;

	}; // ServerListHandle

	PktWriter mPktWriter;
	uint16* mServerListCount;
	uint16* mFirstServerListOffset;
	ServerList* mNowServerList;
	uint16* mAccountNameOffset;
	uint16* mAppGuardUniqueClientIDOffset;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_AC_ACK_LOGIN_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"AC_ACK_LOGIN"; }
	const wchar_t* GetRelatedPacketName() const { return L"CA_REQ_LOGIN"; }
#endif

#ifndef _SERVER
	PKT_AC_ACK_LOGIN_WRITE(uchar* buf, int bufSize, int result, AccountDBId accountId, const FString& accountName, uint16 lastPlanetWorldId, uint16 bestPlanetWorldId, int resultDetail, const FString& appGuardUniqueClientID, bool useSyncPay, bool registerDevice, uint16 advReservationPlanetWorldId)
		:
		mPktWriter(buf, bufSize)
		, mNowServerList(0)
	{
		mPktWriter << static_cast<uint16>(PD::AC::ACPacketId::AC_ACK_LOGIN);
		mServerListCount = (uint16*)mPktWriter.GetBuffer();
		*mServerListCount = 0;
		mPktWriter << (*mServerListCount);
		mFirstServerListOffset = (uint16*)mPktWriter.GetBuffer();
		*mFirstServerListOffset = 0;
		mPktWriter << (*mFirstServerListOffset);
		mAccountNameOffset = (uint16*)mPktWriter.GetBuffer();
		*mAccountNameOffset = 0;
		mPktWriter << (*mAccountNameOffset);
		mAppGuardUniqueClientIDOffset = (uint16*)mPktWriter.GetBuffer();
		*mAppGuardUniqueClientIDOffset = 0;
		mPktWriter << (*mAppGuardUniqueClientIDOffset);
		mPktWriter << result;
		mPktWriter << accountId;
		mPktWriter << lastPlanetWorldId;
		mPktWriter << bestPlanetWorldId;
		mPktWriter << resultDetail;
		mPktWriter << useSyncPay;
		mPktWriter << registerDevice;
		mPktWriter << advReservationPlanetWorldId;
		*mAccountNameOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(accountName);
		*mAppGuardUniqueClientIDOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(appGuardUniqueClientID);
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_AC_ACK_LOGIN_WRITE(SendBufferHelper& sendBuffer, int result, AccountDBId accountId, const wchar* accountName, uint16 lastPlanetWorldId, uint16 bestPlanetWorldId, int resultDetail, const wchar* appGuardUniqueClientID, bool useSyncPay, bool registerDevice, uint16 advReservationPlanetWorldId)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))
		, mNowServerList(0)
	{
		mPktWriter << (uint16)AC_ACK_LOGIN;
		mServerListCount = (uint16*)mPktWriter.GetBuffer();
		*mServerListCount = 0;
		mPktWriter << (*mServerListCount);
		mFirstServerListOffset = (uint16*)mPktWriter.GetBuffer();
		*mFirstServerListOffset = 0;
		mPktWriter << (*mFirstServerListOffset);
		mAccountNameOffset = (uint16*)mPktWriter.GetBuffer();
		*mAccountNameOffset = 0;
		mPktWriter << (*mAccountNameOffset);
		mAppGuardUniqueClientIDOffset = (uint16*)mPktWriter.GetBuffer();
		*mAppGuardUniqueClientIDOffset = 0;
		mPktWriter << (*mAppGuardUniqueClientIDOffset);
		mPktWriter << result;
		mPktWriter << accountId;
		mPktWriter << lastPlanetWorldId;
		mPktWriter << bestPlanetWorldId;
		mPktWriter << resultDetail;
		mPktWriter << useSyncPay;
		mPktWriter << registerDevice;
		mPktWriter << advReservationPlanetWorldId;
		*mAccountNameOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(accountName);
		*mAppGuardUniqueClientIDOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(appGuardUniqueClientID);
	}
#endif

#ifdef _SERVER 
	void send(IocpObjectSession* session, SendBufferRef sendBuffer, bool autoCloseSendBuffer)
	{
		if (session) 
			session->Send(sendBuffer, mPktWriter.GetPktSize(), autoCloseSendBuffer);
	}
#endif

#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{
		auto readPacket = reinterpret_cast<PKT_AC_ACK_LOGIN_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
	ServerListHandle CreateServerList(uint16 planetWorldId, const FString& appendixName, uint8 orderNum, uint16 userCount, uint8 congestion, bool charCreatable, bool recommended, bool newOpened, const FString& hiveAnalyticsServerId)
	{
		uchar* nowBuffer = mPktWriter.GetBuffer();
		uint16 nowWritten = (uint16)mPktWriter.GetPktSize();
		ServerList* newServerList = new ((ServerList*)nowBuffer) ServerList(nowWritten, planetWorldId, orderNum, userCount, congestion, charCreatable, recommended, newOpened);
		++(*mServerListCount);
		if (mNowServerList == 0)
			*mFirstServerListOffset = nowWritten;
		else
			mNowServerList->mNextOffset = nowWritten;
		mNowServerList = newServerList;
		mPktWriter.Skip(sizeof(ServerList));
		mNowServerList->SetAppendixNameOffset((uint16)mPktWriter.GetPktSize());
		mPktWriter.WriteString(appendixName);
		mNowServerList->SetHiveAnalyticsServerIdOffset((uint16)mPktWriter.GetPktSize());
		mPktWriter.WriteString(hiveAnalyticsServerId);
		return ServerListHandle(mPktWriter, newServerList);
	}

#else
	ServerListHandle CreateServerList(uint16 planetWorldId, const wchar* appendixName, uint8 orderNum, uint16 userCount, uint8 congestion, bool charCreatable, bool recommended, bool newOpened, const wchar* hiveAnalyticsServerId)
	{
		uchar* nowBuffer = mPktWriter.GetBuffer();
		uint16 nowWritten = (uint16)mPktWriter.GetPktSize();
		ServerList* newServerList = new ((ServerList*)nowBuffer) ServerList(nowWritten, planetWorldId, orderNum, userCount, congestion, charCreatable, recommended, newOpened);
		++(*mServerListCount);
		if (mNowServerList == 0)
			*mFirstServerListOffset = nowWritten;
		else
			mNowServerList->mNextOffset = nowWritten;
		mNowServerList = newServerList;
		mPktWriter.Skip(sizeof(ServerList));
		mNowServerList->SetAppendixNameOffset((uint16)mPktWriter.GetPktSize());
		mPktWriter.WriteString(appendixName);
		mNowServerList->SetHiveAnalyticsServerIdOffset((uint16)mPktWriter.GetPktSize());
		mPktWriter.WriteString(hiveAnalyticsServerId);
		return ServerListHandle(mPktWriter, newServerList);
	}

#endif
}; // PKT_AC_ACK_LOGIN_WRITE

// type = AC
// related packet = PKT_CA_REQ_SELECT_SERVER_WRITE
struct PKT_AC_ACK_SELECT_SERVER_READ
{
	struct UpdateServerList
	{
	protected:
		uint16 mThisOffset;
		uint16 mNextOffset;
		uint16 mHiveAnalyticsServerIdOffset;
		uint16 mPlanetWorldId;
		uint8 mCongestion;
		bool mCharCreatable;
		bool mRecommended;
		bool mNewOpened;

	public:
		uint16 PlanetWorldId() { return mPlanetWorldId; }
		uint8 Congestion() { return mCongestion; }
		bool CharCreatable() { return mCharCreatable; }
		bool Recommended() { return mRecommended; }
		bool NewOpened() { return mNewOpened; }

#ifdef _SERVER
		const wchar_t* HiveAnalyticsServerId()
		{
			uchar* pktStart = (uchar*)this - mThisOffset;
			uint16 pktSize = *((uint16*)pktStart);
			if (mHiveAnalyticsServerIdOffset && pktSize > mHiveAnalyticsServerIdOffset)
				return (const wchar_t*)(pktStart + mHiveAnalyticsServerIdOffset);
			else
				return L"";
		}
#else // _SERVER
		const FString HiveAnalyticsServerId()
		{
			const wchar_t* strWchar = L"";
			uchar* pktStart = (uchar*)this - mThisOffset;
			uint16 pktSize = *((uint16*)pktStart);
			if (mHiveAnalyticsServerIdOffset && pktSize > mHiveAnalyticsServerIdOffset)
				strWchar = (const wchar_t*)(pktStart + mHiveAnalyticsServerIdOffset);
			return PktWriter::ConvertToFString(strWchar);
		}
#endif // _SERVER
		friend PktIterator<UpdateServerList>;
		friend PKT_AC_ACK_SELECT_SERVER_READ;

		bool Validate(uint16 offset, uchar * pktStart, uint16 pktSize, uint16& dataSize)
		{
			if (static_cast<size_t>(pktSize) < static_cast<size_t>(offset) + sizeof(UpdateServerList))
				return false;

			UpdateServerList * node = (UpdateServerList*)(pktStart + offset);

			// 자기 자신의 포인터 검증
			if (offset != node->mThisOffset)
				return false;

			// Value 검증
			uchar* pktCurrentNodeStart = (uchar*)this - mThisOffset;
			uint16 pktCurrentNodeSize = *((uint16*)pktStart);
			dataSize += sizeof(UpdateServerList);	// skip
			if (false == ValidateValue(pktCurrentNodeStart, pktCurrentNodeSize, dataSize))
				return false;

			return true;
		}

		bool ValidateValue(const uchar* pktStart, uint16 pktSize, uint16& dataSize)
		{
			if (false == ValidateString(mHiveAnalyticsServerIdOffset, 0, pktStart, pktSize, dataSize))
				return false;

			return (nullptr != pktStart && 0 < pktSize && 0 < dataSize) ? true : false;
		}

	}; // UpdateServerList

public:
	uint16 mPktSize;
	uint16 mPktId;
	uint16 mUpdateServerListCount;
	uint16 mFirstUpdateServerListOffset;
	uint16 mIpOffset;
	uint16 mClientIpAddressOffset;
	int mResult;
	AuthTicket mAuthTicket;
	uint16 mPlanetWorldId;
	uint16 mPort;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"AC_ACK_SELECT_SERVER"; }
	const wchar_t* GetRelatedPacketName() const { return L"CA_REQ_SELECT_SERVER"; }
#endif

	int Result() { return mResult; }
	AuthTicket AuthTicket() { return mAuthTicket; }
	uint16 PlanetWorldId() { return mPlanetWorldId; }
	uint16 Port() { return mPort; }
#ifdef _SERVER
	const wchar_t* Ip() const
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = *((uint16*)pktStart);
		if (mIpOffset && pktSize > mIpOffset)
			return (const wchar_t*)(pktStart + mIpOffset);
		else
			return L"";
	}
#else // _SERVER
	const FString Ip()
	{
		const wchar_t* strWchar = L"";
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = *((uint16*)pktStart);
		if (mIpOffset && pktSize > mIpOffset)
			strWchar = (const wchar_t*)(pktStart + mIpOffset);

		return PktWriter::ConvertToFString(strWchar);
	}
#endif // _SERVER
#ifdef _SERVER
	const wchar_t* ClientIpAddress() const
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = *((uint16*)pktStart);
		if (mClientIpAddressOffset && pktSize > mClientIpAddressOffset)
			return (const wchar_t*)(pktStart + mClientIpAddressOffset);
		else
			return L"";
	}
#else // _SERVER
	const FString ClientIpAddress()
	{
		const wchar_t* strWchar = L"";
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = *((uint16*)pktStart);
		if (mClientIpAddressOffset && pktSize > mClientIpAddressOffset)
			strWchar = (const wchar_t*)(pktStart + mClientIpAddressOffset);

		return PktWriter::ConvertToFString(strWchar);
	}
#endif // _SERVER

	using UpdateServerListIterator = PktIterator<UpdateServerList>;
	const UpdateServerListIterator GetLastUpdateServerListIterator() const
	{
		return UpdateServerListIterator(0);
	}
	UpdateServerListIterator GetFirstUpdateServerListIterator() const
	{
		if (mFirstUpdateServerListOffset && mPktSize >= mFirstUpdateServerListOffset + sizeof(UpdateServerList))
		{
			UpdateServerList* updateServerList = (UpdateServerList*)((uchar*)this + mFirstUpdateServerListOffset);
			if (mFirstUpdateServerListOffset == updateServerList->mThisOffset)
				return UpdateServerListIterator(updateServerList);
			else
				return UpdateServerListIterator(0);
		}
		else
			return UpdateServerListIterator(0);
	}
	const int GetUpdateServerListCount()
	{
		return mUpdateServerListCount;
	}
#ifdef _SERVER
	template<typename TFunc>
	void ForEachUpdateServerList(TFunc&& func)
	{
		for (auto it = GetFirstUpdateServerListIterator(); it != GetLastUpdateServerListIterator(); ++it)
			func(it);
	}
#endif


#ifdef _SERVER 
	std::xwstring FuncCreateUpdateServerListStr( [[maybe_unused]] UpdateServerListIterator& iter) const
	{
		return Log::format(
			L"PlanetWorldId: {}, Congestion: {}, CharCreatable: {}, Recommended: {}, NewOpened: {}, HiveAnalyticsServerId: {}", 
			iter->PlanetWorldId(), iter->Congestion(), iter->CharCreatable(), iter->Recommended(), iter->NewOpened(), iter->HiveAnalyticsServerId()
		).c_str();
	}

	std::xwstring GetLogWString() const
	{
		std::xwstring str1;
		str1.append(L"\n");
		for (UpdateServerListIterator iter = this->GetFirstUpdateServerListIterator();
			iter != GetLastUpdateServerListIterator(); ++iter)
		{
			str1.append(L"[");
			str1.append(FuncCreateUpdateServerListStr(iter).c_str());

			str1.append(L"]\n");
		}

		return Log::format(
			L"[AC_ACK_SELECT_SERVER] Result: {}, AuthTicket: {}, PlanetWorldId: {}, Ip: {}, Port: {}, ClientIpAddress: {}, UpdateServerList : {}"
			, mResult, mAuthTicket, mPlanetWorldId, Ip(), mPort, ClientIpAddress(), str1.c_str()
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_AC_ACK_SELECT_SERVER_READ);
		if (dataSize > mPktSize)
			return false;

		if (false == ValidateValue(pktStart, pktSize, dataSize))
			return false;

		if (false == UpdateServerListValidateListNode(mFirstUpdateServerListOffset, pktStart, pktSize, dataSize))
			return false;

		if (dataSize != mPktSize)
			return false;

		return true;
	}

	bool UpdateServerListValidateListNode(uint16 offset, uchar * pktStart, uint16 pktSize, uint16& dataSize)
	{		for (UpdateServerListIterator iter = this->GetFirstUpdateServerListIterator();
			iter != GetLastUpdateServerListIterator(); ++iter)
		{
			if (false == (*iter).Validate(offset, pktStart, pktSize, dataSize))
			{
				return false;
			}
		}
		return true;
	}

	bool ValidateValue(const uchar* pktStart, uint16 pktSize, uint16& dataSize)
	{
		if (false == ValidateString(mIpOffset, 0, pktStart, pktSize, dataSize))
			return false;

		if (false == ValidateString(mClientIpAddressOffset, 0, pktStart, pktSize, dataSize))
			return false;

		return (nullptr != pktStart && 0 < pktSize && 0 < dataSize) ? true : false;
	}

}; // PKT_AC_ACK_SELECT_SERVER_READ

// type = AC
// related packet = PKT_CA_REQ_SELECT_SERVER_READ
#ifdef _SERVER
struct PKT_AC_ACK_SELECT_SERVER_WRITE : public Disposable
#else
struct PKT_AC_ACK_SELECT_SERVER_WRITE
#endif
{
	struct UpdateServerList
	{
		UpdateServerList(uint16 offset, uint16 planetWorldId, uint8 congestion, bool charCreatable, bool recommended, bool newOpened)
			:
			mThisOffset(offset)
			, mNextOffset(0), mHiveAnalyticsServerIdOffset(0)
			, mPlanetWorldId(planetWorldId)
			, mCongestion(congestion)
			, mCharCreatable(charCreatable)
			, mRecommended(recommended)
			, mNewOpened(newOpened)
		{
		}
		void SetHiveAnalyticsServerIdOffset(uint16 offset)
		{
			mHiveAnalyticsServerIdOffset = offset;
		}

		uint16 mThisOffset;
		uint16 mNextOffset;
		uint16 mHiveAnalyticsServerIdOffset;
		uint16 mPlanetWorldId;
		uint8 mCongestion;
		bool mCharCreatable;
		bool mRecommended;
		bool mNewOpened;

	}; // UpdateServerList

	struct UpdateServerListHandle
	{
		UpdateServerListHandle(PktWriter& pktWriter, UpdateServerList* updateServerList)
			:
			mPktWriter(pktWriter)
			, mUpdateServerList(updateServerList)
		{
		}

		PktWriter& mPktWriter;
		UpdateServerList* mUpdateServerList;

	}; // UpdateServerListHandle

	PktWriter mPktWriter;
	uint16* mUpdateServerListCount;
	uint16* mFirstUpdateServerListOffset;
	UpdateServerList* mNowUpdateServerList;
	uint16* mIpOffset;
	uint16* mClientIpAddressOffset;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_AC_ACK_SELECT_SERVER_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"AC_ACK_SELECT_SERVER"; }
	const wchar_t* GetRelatedPacketName() const { return L"CA_REQ_SELECT_SERVER"; }
#endif

#ifndef _SERVER
	PKT_AC_ACK_SELECT_SERVER_WRITE(uchar* buf, int bufSize, int result, AuthTicket authTicket, uint16 planetWorldId, const FString& ip, uint16 port, const FString& clientIpAddress)
		:
		mPktWriter(buf, bufSize)
		, mNowUpdateServerList(0)
	{
		mPktWriter << static_cast<uint16>(PD::AC::ACPacketId::AC_ACK_SELECT_SERVER);
		mUpdateServerListCount = (uint16*)mPktWriter.GetBuffer();
		*mUpdateServerListCount = 0;
		mPktWriter << (*mUpdateServerListCount);
		mFirstUpdateServerListOffset = (uint16*)mPktWriter.GetBuffer();
		*mFirstUpdateServerListOffset = 0;
		mPktWriter << (*mFirstUpdateServerListOffset);
		mIpOffset = (uint16*)mPktWriter.GetBuffer();
		*mIpOffset = 0;
		mPktWriter << (*mIpOffset);
		mClientIpAddressOffset = (uint16*)mPktWriter.GetBuffer();
		*mClientIpAddressOffset = 0;
		mPktWriter << (*mClientIpAddressOffset);
		mPktWriter << result;
		mPktWriter << authTicket;
		mPktWriter << planetWorldId;
		mPktWriter << port;
		*mIpOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(ip);
		*mClientIpAddressOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(clientIpAddress);
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_AC_ACK_SELECT_SERVER_WRITE(SendBufferHelper& sendBuffer, int result, AuthTicket authTicket, uint16 planetWorldId, const wchar* ip, uint16 port, const wchar* clientIpAddress)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))
		, mNowUpdateServerList(0)
	{
		mPktWriter << (uint16)AC_ACK_SELECT_SERVER;
		mUpdateServerListCount = (uint16*)mPktWriter.GetBuffer();
		*mUpdateServerListCount = 0;
		mPktWriter << (*mUpdateServerListCount);
		mFirstUpdateServerListOffset = (uint16*)mPktWriter.GetBuffer();
		*mFirstUpdateServerListOffset = 0;
		mPktWriter << (*mFirstUpdateServerListOffset);
		mIpOffset = (uint16*)mPktWriter.GetBuffer();
		*mIpOffset = 0;
		mPktWriter << (*mIpOffset);
		mClientIpAddressOffset = (uint16*)mPktWriter.GetBuffer();
		*mClientIpAddressOffset = 0;
		mPktWriter << (*mClientIpAddressOffset);
		mPktWriter << result;
		mPktWriter << authTicket;
		mPktWriter << planetWorldId;
		mPktWriter << port;
		*mIpOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(ip);
		*mClientIpAddressOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(clientIpAddress);
	}
#endif

#ifdef _SERVER 
	void send(IocpObjectSession* session, SendBufferRef sendBuffer, bool autoCloseSendBuffer)
	{
		if (session) 
			session->Send(sendBuffer, mPktWriter.GetPktSize(), autoCloseSendBuffer);
	}
#endif

#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{
		auto readPacket = reinterpret_cast<PKT_AC_ACK_SELECT_SERVER_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
	UpdateServerListHandle CreateUpdateServerList(uint16 planetWorldId, uint8 congestion, bool charCreatable, bool recommended, bool newOpened, const FString& hiveAnalyticsServerId)
	{
		uchar* nowBuffer = mPktWriter.GetBuffer();
		uint16 nowWritten = (uint16)mPktWriter.GetPktSize();
		UpdateServerList* newUpdateServerList = new ((UpdateServerList*)nowBuffer) UpdateServerList(nowWritten, planetWorldId, congestion, charCreatable, recommended, newOpened);
		++(*mUpdateServerListCount);
		if (mNowUpdateServerList == 0)
			*mFirstUpdateServerListOffset = nowWritten;
		else
			mNowUpdateServerList->mNextOffset = nowWritten;
		mNowUpdateServerList = newUpdateServerList;
		mPktWriter.Skip(sizeof(UpdateServerList));
		mNowUpdateServerList->SetHiveAnalyticsServerIdOffset((uint16)mPktWriter.GetPktSize());
		mPktWriter.WriteString(hiveAnalyticsServerId);
		return UpdateServerListHandle(mPktWriter, newUpdateServerList);
	}

#else
	UpdateServerListHandle CreateUpdateServerList(uint16 planetWorldId, uint8 congestion, bool charCreatable, bool recommended, bool newOpened, const wchar* hiveAnalyticsServerId)
	{
		uchar* nowBuffer = mPktWriter.GetBuffer();
		uint16 nowWritten = (uint16)mPktWriter.GetPktSize();
		UpdateServerList* newUpdateServerList = new ((UpdateServerList*)nowBuffer) UpdateServerList(nowWritten, planetWorldId, congestion, charCreatable, recommended, newOpened);
		++(*mUpdateServerListCount);
		if (mNowUpdateServerList == 0)
			*mFirstUpdateServerListOffset = nowWritten;
		else
			mNowUpdateServerList->mNextOffset = nowWritten;
		mNowUpdateServerList = newUpdateServerList;
		mPktWriter.Skip(sizeof(UpdateServerList));
		mNowUpdateServerList->SetHiveAnalyticsServerIdOffset((uint16)mPktWriter.GetPktSize());
		mPktWriter.WriteString(hiveAnalyticsServerId);
		return UpdateServerListHandle(mPktWriter, newUpdateServerList);
	}

#endif
}; // PKT_AC_ACK_SELECT_SERVER_WRITE

// type = AC
struct PKT_AC_KICKED_BY_TIMEOUT_READ
{
public:
	uint16 mPktSize;
	uint16 mPktId;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"AC_KICKED_BY_TIMEOUT"; }
	const wchar_t* GetRelatedPacketName() const { return L"CA_KICKED_BY_TIMEOUT"; }
#endif



#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{

		return Log::format(
			L"[AC_KICKED_BY_TIMEOUT] "
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_AC_KICKED_BY_TIMEOUT_READ);
		if (dataSize > mPktSize)
			return false;

		if (false == ValidateValue(pktStart, pktSize, dataSize))
			return false;

		if (dataSize != mPktSize)
			return false;

		return true;
	}

	bool ValidateValue(const uchar* pktStart, uint16 pktSize, uint16& dataSize)
	{
		return (nullptr != pktStart && 0 < pktSize && 0 < dataSize) ? true : false;
	}

}; // PKT_AC_KICKED_BY_TIMEOUT_READ

// type = AC
#ifdef _SERVER
struct PKT_AC_KICKED_BY_TIMEOUT_WRITE : public Disposable
#else
struct PKT_AC_KICKED_BY_TIMEOUT_WRITE
#endif
{
	PktWriter mPktWriter;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_AC_KICKED_BY_TIMEOUT_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"AC_KICKED_BY_TIMEOUT"; }
	const wchar_t* GetRelatedPacketName() const { return L"CA_KICKED_BY_TIMEOUT"; }
#endif

#ifndef _SERVER
	PKT_AC_KICKED_BY_TIMEOUT_WRITE(uchar* buf, int bufSize)
		:
		mPktWriter(buf, bufSize)

	{
		mPktWriter << static_cast<uint16>(PD::AC::ACPacketId::AC_KICKED_BY_TIMEOUT);
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_AC_KICKED_BY_TIMEOUT_WRITE(SendBufferHelper& sendBuffer)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))

	{
		mPktWriter << (uint16)AC_KICKED_BY_TIMEOUT;
	}
#endif

#ifdef _SERVER 
	void send(IocpObjectSession* session, SendBufferRef sendBuffer, bool autoCloseSendBuffer)
	{
		if (session) 
			session->Send(sendBuffer, mPktWriter.GetPktSize(), autoCloseSendBuffer);
	}
#endif

#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{
		auto readPacket = reinterpret_cast<PKT_AC_KICKED_BY_TIMEOUT_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
#else
#endif
}; // PKT_AC_KICKED_BY_TIMEOUT_WRITE

// type = AC
// related packet = PKT_CA_REQ_CHECK_PASSWORD_WRITE
struct PKT_AC_ACK_CHECK_PASSWORD_READ
{
public:
	uint16 mPktSize;
	uint16 mPktId;
	int mResult;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"AC_ACK_CHECK_PASSWORD"; }
	const wchar_t* GetRelatedPacketName() const { return L"CA_REQ_CHECK_PASSWORD"; }
#endif

	int Result() { return mResult; }


#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{

		return Log::format(
			L"[AC_ACK_CHECK_PASSWORD] Result: {}"
			, mResult
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_AC_ACK_CHECK_PASSWORD_READ);
		if (dataSize > mPktSize)
			return false;

		if (false == ValidateValue(pktStart, pktSize, dataSize))
			return false;

		if (dataSize != mPktSize)
			return false;

		return true;
	}

	bool ValidateValue(const uchar* pktStart, uint16 pktSize, uint16& dataSize)
	{
		return (nullptr != pktStart && 0 < pktSize && 0 < dataSize) ? true : false;
	}

}; // PKT_AC_ACK_CHECK_PASSWORD_READ

// type = AC
// related packet = PKT_CA_REQ_CHECK_PASSWORD_READ
#ifdef _SERVER
struct PKT_AC_ACK_CHECK_PASSWORD_WRITE : public Disposable
#else
struct PKT_AC_ACK_CHECK_PASSWORD_WRITE
#endif
{
	PktWriter mPktWriter;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_AC_ACK_CHECK_PASSWORD_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"AC_ACK_CHECK_PASSWORD"; }
	const wchar_t* GetRelatedPacketName() const { return L"CA_REQ_CHECK_PASSWORD"; }
#endif

#ifndef _SERVER
	PKT_AC_ACK_CHECK_PASSWORD_WRITE(uchar* buf, int bufSize, int result)
		:
		mPktWriter(buf, bufSize)

	{
		mPktWriter << static_cast<uint16>(PD::AC::ACPacketId::AC_ACK_CHECK_PASSWORD);
		mPktWriter << result;
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_AC_ACK_CHECK_PASSWORD_WRITE(SendBufferHelper& sendBuffer, int result)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))

	{
		mPktWriter << (uint16)AC_ACK_CHECK_PASSWORD;
		mPktWriter << result;
	}
#endif

#ifdef _SERVER 
	void send(IocpObjectSession* session, SendBufferRef sendBuffer, bool autoCloseSendBuffer)
	{
		if (session) 
			session->Send(sendBuffer, mPktWriter.GetPktSize(), autoCloseSendBuffer);
	}
#endif

#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{
		auto readPacket = reinterpret_cast<PKT_AC_ACK_CHECK_PASSWORD_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
#else
#endif
}; // PKT_AC_ACK_CHECK_PASSWORD_WRITE

// type = AC
// related packet = PKT_CA_REQ_CHECK_VERSION_WRITE
struct PKT_AC_ACK_CHECK_VERSION_READ
{
public:
	uint16 mPktSize;
	uint16 mPktId;
	int mResult;
	uint64 mClientVersion;
	uint64 mServerVersion;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"AC_ACK_CHECK_VERSION"; }
	const wchar_t* GetRelatedPacketName() const { return L"CA_REQ_CHECK_VERSION"; }
#endif

	int Result() { return mResult; }
	uint64 ClientVersion() { return mClientVersion; }
	uint64 ServerVersion() { return mServerVersion; }


#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{

		return Log::format(
			L"[AC_ACK_CHECK_VERSION] Result: {}, ClientVersion: {}, ServerVersion: {}"
			, mResult, mClientVersion, mServerVersion
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_AC_ACK_CHECK_VERSION_READ);
		if (dataSize > mPktSize)
			return false;

		if (false == ValidateValue(pktStart, pktSize, dataSize))
			return false;

		if (dataSize != mPktSize)
			return false;

		return true;
	}

	bool ValidateValue(const uchar* pktStart, uint16 pktSize, uint16& dataSize)
	{
		return (nullptr != pktStart && 0 < pktSize && 0 < dataSize) ? true : false;
	}

}; // PKT_AC_ACK_CHECK_VERSION_READ

// type = AC
// related packet = PKT_CA_REQ_CHECK_VERSION_READ
#ifdef _SERVER
struct PKT_AC_ACK_CHECK_VERSION_WRITE : public Disposable
#else
struct PKT_AC_ACK_CHECK_VERSION_WRITE
#endif
{
	PktWriter mPktWriter;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_AC_ACK_CHECK_VERSION_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"AC_ACK_CHECK_VERSION"; }
	const wchar_t* GetRelatedPacketName() const { return L"CA_REQ_CHECK_VERSION"; }
#endif

#ifndef _SERVER
	PKT_AC_ACK_CHECK_VERSION_WRITE(uchar* buf, int bufSize, int result, uint64 clientVersion, uint64 serverVersion)
		:
		mPktWriter(buf, bufSize)

	{
		mPktWriter << static_cast<uint16>(PD::AC::ACPacketId::AC_ACK_CHECK_VERSION);
		mPktWriter << result;
		mPktWriter << clientVersion;
		mPktWriter << serverVersion;
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_AC_ACK_CHECK_VERSION_WRITE(SendBufferHelper& sendBuffer, int result, uint64 clientVersion, uint64 serverVersion)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))

	{
		mPktWriter << (uint16)AC_ACK_CHECK_VERSION;
		mPktWriter << result;
		mPktWriter << clientVersion;
		mPktWriter << serverVersion;
	}
#endif

#ifdef _SERVER 
	void send(IocpObjectSession* session, SendBufferRef sendBuffer, bool autoCloseSendBuffer)
	{
		if (session) 
			session->Send(sendBuffer, mPktWriter.GetPktSize(), autoCloseSendBuffer);
	}
#endif

#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{
		auto readPacket = reinterpret_cast<PKT_AC_ACK_CHECK_VERSION_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
#else
#endif
}; // PKT_AC_ACK_CHECK_VERSION_WRITE

// type = AC
// related packet = PKT_CA_REQ_APP_GUARD_AUTH_WRITE
struct PKT_AC_ACK_APP_GUARD_AUTH_READ
{
public:
	uint16 mPktSize;
	uint16 mPktId;
	int mResult;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"AC_ACK_APP_GUARD_AUTH"; }
	const wchar_t* GetRelatedPacketName() const { return L"CA_REQ_APP_GUARD_AUTH"; }
#endif

	int Result() { return mResult; }


#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{

		return Log::format(
			L"[AC_ACK_APP_GUARD_AUTH] Result: {}"
			, mResult
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_AC_ACK_APP_GUARD_AUTH_READ);
		if (dataSize > mPktSize)
			return false;

		if (false == ValidateValue(pktStart, pktSize, dataSize))
			return false;

		if (dataSize != mPktSize)
			return false;

		return true;
	}

	bool ValidateValue(const uchar* pktStart, uint16 pktSize, uint16& dataSize)
	{
		return (nullptr != pktStart && 0 < pktSize && 0 < dataSize) ? true : false;
	}

}; // PKT_AC_ACK_APP_GUARD_AUTH_READ

// type = AC
// related packet = PKT_CA_REQ_APP_GUARD_AUTH_READ
#ifdef _SERVER
struct PKT_AC_ACK_APP_GUARD_AUTH_WRITE : public Disposable
#else
struct PKT_AC_ACK_APP_GUARD_AUTH_WRITE
#endif
{
	PktWriter mPktWriter;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_AC_ACK_APP_GUARD_AUTH_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"AC_ACK_APP_GUARD_AUTH"; }
	const wchar_t* GetRelatedPacketName() const { return L"CA_REQ_APP_GUARD_AUTH"; }
#endif

#ifndef _SERVER
	PKT_AC_ACK_APP_GUARD_AUTH_WRITE(uchar* buf, int bufSize, int result)
		:
		mPktWriter(buf, bufSize)

	{
		mPktWriter << static_cast<uint16>(PD::AC::ACPacketId::AC_ACK_APP_GUARD_AUTH);
		mPktWriter << result;
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_AC_ACK_APP_GUARD_AUTH_WRITE(SendBufferHelper& sendBuffer, int result)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))

	{
		mPktWriter << (uint16)AC_ACK_APP_GUARD_AUTH;
		mPktWriter << result;
	}
#endif

#ifdef _SERVER 
	void send(IocpObjectSession* session, SendBufferRef sendBuffer, bool autoCloseSendBuffer)
	{
		if (session) 
			session->Send(sendBuffer, mPktWriter.GetPktSize(), autoCloseSendBuffer);
	}
#endif

#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{
		auto readPacket = reinterpret_cast<PKT_AC_ACK_APP_GUARD_AUTH_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
#else
#endif
}; // PKT_AC_ACK_APP_GUARD_AUTH_WRITE

// type = AC
// related packet = PKT_CA_REQ_DELETE_ACCOUNT_WRITE
struct PKT_AC_ACK_DELETE_ACCOUNT_READ
{
public:
	uint16 mPktSize;
	uint16 mPktId;
	int mResult;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"AC_ACK_DELETE_ACCOUNT"; }
	const wchar_t* GetRelatedPacketName() const { return L"CA_REQ_DELETE_ACCOUNT"; }
#endif

	int Result() { return mResult; }


#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{

		return Log::format(
			L"[AC_ACK_DELETE_ACCOUNT] Result: {}"
			, mResult
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_AC_ACK_DELETE_ACCOUNT_READ);
		if (dataSize > mPktSize)
			return false;

		if (false == ValidateValue(pktStart, pktSize, dataSize))
			return false;

		if (dataSize != mPktSize)
			return false;

		return true;
	}

	bool ValidateValue(const uchar* pktStart, uint16 pktSize, uint16& dataSize)
	{
		return (nullptr != pktStart && 0 < pktSize && 0 < dataSize) ? true : false;
	}

}; // PKT_AC_ACK_DELETE_ACCOUNT_READ

// type = AC
// related packet = PKT_CA_REQ_DELETE_ACCOUNT_READ
#ifdef _SERVER
struct PKT_AC_ACK_DELETE_ACCOUNT_WRITE : public Disposable
#else
struct PKT_AC_ACK_DELETE_ACCOUNT_WRITE
#endif
{
	PktWriter mPktWriter;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_AC_ACK_DELETE_ACCOUNT_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"AC_ACK_DELETE_ACCOUNT"; }
	const wchar_t* GetRelatedPacketName() const { return L"CA_REQ_DELETE_ACCOUNT"; }
#endif

#ifndef _SERVER
	PKT_AC_ACK_DELETE_ACCOUNT_WRITE(uchar* buf, int bufSize, int result)
		:
		mPktWriter(buf, bufSize)

	{
		mPktWriter << static_cast<uint16>(PD::AC::ACPacketId::AC_ACK_DELETE_ACCOUNT);
		mPktWriter << result;
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_AC_ACK_DELETE_ACCOUNT_WRITE(SendBufferHelper& sendBuffer, int result)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))

	{
		mPktWriter << (uint16)AC_ACK_DELETE_ACCOUNT;
		mPktWriter << result;
	}
#endif

#ifdef _SERVER 
	void send(IocpObjectSession* session, SendBufferRef sendBuffer, bool autoCloseSendBuffer)
	{
		if (session) 
			session->Send(sendBuffer, mPktWriter.GetPktSize(), autoCloseSendBuffer);
	}
#endif

#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{
		auto readPacket = reinterpret_cast<PKT_AC_ACK_DELETE_ACCOUNT_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
#else
#endif
}; // PKT_AC_ACK_DELETE_ACCOUNT_WRITE

// type = AC
// related packet = PKT_CA_REQ_PRE_LOGIN_WRITE
struct PKT_AC_ACK_PRE_LOGIN_READ
{
public:
	uint16 mPktSize;
	uint16 mPktId;
	uint16 mPlayingUserNameOffset;
	uint16 mAppendixNameOffset;
	uint16 mHiveHostCategoryOffset;
	int mResult;
	int mResultDetail;
	bool mCanZPay;
	bool mRegisterDevice;
	WorldId mConectionWorldId;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"AC_ACK_PRE_LOGIN"; }
	const wchar_t* GetRelatedPacketName() const { return L"CA_REQ_PRE_LOGIN"; }
#endif

	int Result() { return mResult; }
	int ResultDetail() { return mResultDetail; }
	bool CanZPay() { return mCanZPay; }
	bool RegisterDevice() { return mRegisterDevice; }
	WorldId ConectionWorldId() { return mConectionWorldId; }
#ifdef _SERVER
	const wchar_t* PlayingUserName() const
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = *((uint16*)pktStart);
		if (mPlayingUserNameOffset && pktSize > mPlayingUserNameOffset)
			return (const wchar_t*)(pktStart + mPlayingUserNameOffset);
		else
			return L"";
	}
#else // _SERVER
	const FString PlayingUserName()
	{
		const wchar_t* strWchar = L"";
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = *((uint16*)pktStart);
		if (mPlayingUserNameOffset && pktSize > mPlayingUserNameOffset)
			strWchar = (const wchar_t*)(pktStart + mPlayingUserNameOffset);

		return PktWriter::ConvertToFString(strWchar);
	}
#endif // _SERVER
#ifdef _SERVER
	const wchar_t* AppendixName() const
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = *((uint16*)pktStart);
		if (mAppendixNameOffset && pktSize > mAppendixNameOffset)
			return (const wchar_t*)(pktStart + mAppendixNameOffset);
		else
			return L"";
	}
#else // _SERVER
	const FString AppendixName()
	{
		const wchar_t* strWchar = L"";
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = *((uint16*)pktStart);
		if (mAppendixNameOffset && pktSize > mAppendixNameOffset)
			strWchar = (const wchar_t*)(pktStart + mAppendixNameOffset);

		return PktWriter::ConvertToFString(strWchar);
	}
#endif // _SERVER
#ifdef _SERVER
	const wchar_t* HiveHostCategory() const
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = *((uint16*)pktStart);
		if (mHiveHostCategoryOffset && pktSize > mHiveHostCategoryOffset)
			return (const wchar_t*)(pktStart + mHiveHostCategoryOffset);
		else
			return L"";
	}
#else // _SERVER
	const FString HiveHostCategory()
	{
		const wchar_t* strWchar = L"";
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = *((uint16*)pktStart);
		if (mHiveHostCategoryOffset && pktSize > mHiveHostCategoryOffset)
			strWchar = (const wchar_t*)(pktStart + mHiveHostCategoryOffset);

		return PktWriter::ConvertToFString(strWchar);
	}
#endif // _SERVER


#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{

		return Log::format(
			L"[AC_ACK_PRE_LOGIN] Result: {}, ResultDetail: {}, CanZPay: {}, RegisterDevice: {}, PlayingUserName: {}, ConectionWorldId: {}, AppendixName: {}, HiveHostCategory: {}"
			, mResult, mResultDetail, mCanZPay, mRegisterDevice, PlayingUserName(), mConectionWorldId, AppendixName(), HiveHostCategory()
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_AC_ACK_PRE_LOGIN_READ);
		if (dataSize > mPktSize)
			return false;

		if (false == ValidateValue(pktStart, pktSize, dataSize))
			return false;

		if (dataSize != mPktSize)
			return false;

		return true;
	}

	bool ValidateValue(const uchar* pktStart, uint16 pktSize, uint16& dataSize)
	{
		if (false == ValidateString(mPlayingUserNameOffset, 0, pktStart, pktSize, dataSize))
			return false;

		if (false == ValidateString(mAppendixNameOffset, 0, pktStart, pktSize, dataSize))
			return false;

		if (false == ValidateString(mHiveHostCategoryOffset, 0, pktStart, pktSize, dataSize))
			return false;

		return (nullptr != pktStart && 0 < pktSize && 0 < dataSize) ? true : false;
	}

}; // PKT_AC_ACK_PRE_LOGIN_READ

// type = AC
// related packet = PKT_CA_REQ_PRE_LOGIN_READ
#ifdef _SERVER
struct PKT_AC_ACK_PRE_LOGIN_WRITE : public Disposable
#else
struct PKT_AC_ACK_PRE_LOGIN_WRITE
#endif
{
	PktWriter mPktWriter;
	uint16* mPlayingUserNameOffset;
	uint16* mAppendixNameOffset;
	uint16* mHiveHostCategoryOffset;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_AC_ACK_PRE_LOGIN_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"AC_ACK_PRE_LOGIN"; }
	const wchar_t* GetRelatedPacketName() const { return L"CA_REQ_PRE_LOGIN"; }
#endif

#ifndef _SERVER
	PKT_AC_ACK_PRE_LOGIN_WRITE(uchar* buf, int bufSize, int result, int resultDetail, bool canZPay, bool registerDevice, const FString& playingUserName, WorldId conectionWorldId, const FString& appendixName, const FString& hiveHostCategory)
		:
		mPktWriter(buf, bufSize)

	{
		mPktWriter << static_cast<uint16>(PD::AC::ACPacketId::AC_ACK_PRE_LOGIN);
		mPlayingUserNameOffset = (uint16*)mPktWriter.GetBuffer();
		*mPlayingUserNameOffset = 0;
		mPktWriter << (*mPlayingUserNameOffset);
		mAppendixNameOffset = (uint16*)mPktWriter.GetBuffer();
		*mAppendixNameOffset = 0;
		mPktWriter << (*mAppendixNameOffset);
		mHiveHostCategoryOffset = (uint16*)mPktWriter.GetBuffer();
		*mHiveHostCategoryOffset = 0;
		mPktWriter << (*mHiveHostCategoryOffset);
		mPktWriter << result;
		mPktWriter << resultDetail;
		mPktWriter << canZPay;
		mPktWriter << registerDevice;
		mPktWriter << conectionWorldId;
		*mPlayingUserNameOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(playingUserName);
		*mAppendixNameOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(appendixName);
		*mHiveHostCategoryOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(hiveHostCategory);
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_AC_ACK_PRE_LOGIN_WRITE(SendBufferHelper& sendBuffer, int result, int resultDetail, bool canZPay, bool registerDevice, const wchar* playingUserName, WorldId conectionWorldId, const wchar* appendixName, const wchar* hiveHostCategory)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))

	{
		mPktWriter << (uint16)AC_ACK_PRE_LOGIN;
		mPlayingUserNameOffset = (uint16*)mPktWriter.GetBuffer();
		*mPlayingUserNameOffset = 0;
		mPktWriter << (*mPlayingUserNameOffset);
		mAppendixNameOffset = (uint16*)mPktWriter.GetBuffer();
		*mAppendixNameOffset = 0;
		mPktWriter << (*mAppendixNameOffset);
		mHiveHostCategoryOffset = (uint16*)mPktWriter.GetBuffer();
		*mHiveHostCategoryOffset = 0;
		mPktWriter << (*mHiveHostCategoryOffset);
		mPktWriter << result;
		mPktWriter << resultDetail;
		mPktWriter << canZPay;
		mPktWriter << registerDevice;
		mPktWriter << conectionWorldId;
		*mPlayingUserNameOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(playingUserName);
		*mAppendixNameOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(appendixName);
		*mHiveHostCategoryOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(hiveHostCategory);
	}
#endif

#ifdef _SERVER 
	void send(IocpObjectSession* session, SendBufferRef sendBuffer, bool autoCloseSendBuffer)
	{
		if (session) 
			session->Send(sendBuffer, mPktWriter.GetPktSize(), autoCloseSendBuffer);
	}
#endif

#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{
		auto readPacket = reinterpret_cast<PKT_AC_ACK_PRE_LOGIN_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
#else
#endif
}; // PKT_AC_ACK_PRE_LOGIN_WRITE

// type = AC
// related packet = PKT_CA_REQ_IDP_CONFLICT_PLYER_WRITE
struct PKT_AC_ACK_IDP_CONFLICT_PLYER_READ
{
	struct UserList
	{
	protected:
		uint16 mThisOffset;
		uint16 mNextOffset;
		uint16 mUserNameOffset;
		uint16 mServerNameOffset;
		uint16 mUserInfoType0Offset;
		uint16 mUserInfoValue0Offset;
		uint16 mUserInfoType1Offset;
		uint16 mUserInfoValue1Offset;

	public:

#ifdef _SERVER
		const wchar_t* UserName()
		{
			uchar* pktStart = (uchar*)this - mThisOffset;
			uint16 pktSize = *((uint16*)pktStart);
			if (mUserNameOffset && pktSize > mUserNameOffset)
				return (const wchar_t*)(pktStart + mUserNameOffset);
			else
				return L"";
		}
#else // _SERVER
		const FString UserName()
		{
			const wchar_t* strWchar = L"";
			uchar* pktStart = (uchar*)this - mThisOffset;
			uint16 pktSize = *((uint16*)pktStart);
			if (mUserNameOffset && pktSize > mUserNameOffset)
				strWchar = (const wchar_t*)(pktStart + mUserNameOffset);
			return PktWriter::ConvertToFString(strWchar);
		}
#endif // _SERVER
#ifdef _SERVER
		const wchar_t* ServerName()
		{
			uchar* pktStart = (uchar*)this - mThisOffset;
			uint16 pktSize = *((uint16*)pktStart);
			if (mServerNameOffset && pktSize > mServerNameOffset)
				return (const wchar_t*)(pktStart + mServerNameOffset);
			else
				return L"";
		}
#else // _SERVER
		const FString ServerName()
		{
			const wchar_t* strWchar = L"";
			uchar* pktStart = (uchar*)this - mThisOffset;
			uint16 pktSize = *((uint16*)pktStart);
			if (mServerNameOffset && pktSize > mServerNameOffset)
				strWchar = (const wchar_t*)(pktStart + mServerNameOffset);
			return PktWriter::ConvertToFString(strWchar);
		}
#endif // _SERVER
#ifdef _SERVER
		const wchar_t* UserInfoType0()
		{
			uchar* pktStart = (uchar*)this - mThisOffset;
			uint16 pktSize = *((uint16*)pktStart);
			if (mUserInfoType0Offset && pktSize > mUserInfoType0Offset)
				return (const wchar_t*)(pktStart + mUserInfoType0Offset);
			else
				return L"";
		}
#else // _SERVER
		const FString UserInfoType0()
		{
			const wchar_t* strWchar = L"";
			uchar* pktStart = (uchar*)this - mThisOffset;
			uint16 pktSize = *((uint16*)pktStart);
			if (mUserInfoType0Offset && pktSize > mUserInfoType0Offset)
				strWchar = (const wchar_t*)(pktStart + mUserInfoType0Offset);
			return PktWriter::ConvertToFString(strWchar);
		}
#endif // _SERVER
#ifdef _SERVER
		const wchar_t* UserInfoValue0()
		{
			uchar* pktStart = (uchar*)this - mThisOffset;
			uint16 pktSize = *((uint16*)pktStart);
			if (mUserInfoValue0Offset && pktSize > mUserInfoValue0Offset)
				return (const wchar_t*)(pktStart + mUserInfoValue0Offset);
			else
				return L"";
		}
#else // _SERVER
		const FString UserInfoValue0()
		{
			const wchar_t* strWchar = L"";
			uchar* pktStart = (uchar*)this - mThisOffset;
			uint16 pktSize = *((uint16*)pktStart);
			if (mUserInfoValue0Offset && pktSize > mUserInfoValue0Offset)
				strWchar = (const wchar_t*)(pktStart + mUserInfoValue0Offset);
			return PktWriter::ConvertToFString(strWchar);
		}
#endif // _SERVER
#ifdef _SERVER
		const wchar_t* UserInfoType1()
		{
			uchar* pktStart = (uchar*)this - mThisOffset;
			uint16 pktSize = *((uint16*)pktStart);
			if (mUserInfoType1Offset && pktSize > mUserInfoType1Offset)
				return (const wchar_t*)(pktStart + mUserInfoType1Offset);
			else
				return L"";
		}
#else // _SERVER
		const FString UserInfoType1()
		{
			const wchar_t* strWchar = L"";
			uchar* pktStart = (uchar*)this - mThisOffset;
			uint16 pktSize = *((uint16*)pktStart);
			if (mUserInfoType1Offset && pktSize > mUserInfoType1Offset)
				strWchar = (const wchar_t*)(pktStart + mUserInfoType1Offset);
			return PktWriter::ConvertToFString(strWchar);
		}
#endif // _SERVER
#ifdef _SERVER
		const wchar_t* UserInfoValue1()
		{
			uchar* pktStart = (uchar*)this - mThisOffset;
			uint16 pktSize = *((uint16*)pktStart);
			if (mUserInfoValue1Offset && pktSize > mUserInfoValue1Offset)
				return (const wchar_t*)(pktStart + mUserInfoValue1Offset);
			else
				return L"";
		}
#else // _SERVER
		const FString UserInfoValue1()
		{
			const wchar_t* strWchar = L"";
			uchar* pktStart = (uchar*)this - mThisOffset;
			uint16 pktSize = *((uint16*)pktStart);
			if (mUserInfoValue1Offset && pktSize > mUserInfoValue1Offset)
				strWchar = (const wchar_t*)(pktStart + mUserInfoValue1Offset);
			return PktWriter::ConvertToFString(strWchar);
		}
#endif // _SERVER
		friend PktIterator<UserList>;
		friend PKT_AC_ACK_IDP_CONFLICT_PLYER_READ;

		bool Validate(uint16 offset, uchar * pktStart, uint16 pktSize, uint16& dataSize)
		{
			if (static_cast<size_t>(pktSize) < static_cast<size_t>(offset) + sizeof(UserList))
				return false;

			UserList * node = (UserList*)(pktStart + offset);

			// 자기 자신의 포인터 검증
			if (offset != node->mThisOffset)
				return false;

			// Value 검증
			uchar* pktCurrentNodeStart = (uchar*)this - mThisOffset;
			uint16 pktCurrentNodeSize = *((uint16*)pktStart);
			dataSize += sizeof(UserList);	// skip
			if (false == ValidateValue(pktCurrentNodeStart, pktCurrentNodeSize, dataSize))
				return false;

			return true;
		}

		bool ValidateValue(const uchar* pktStart, uint16 pktSize, uint16& dataSize)
		{
			if (false == ValidateString(mUserNameOffset, 0, pktStart, pktSize, dataSize))
				return false;

			if (false == ValidateString(mServerNameOffset, 0, pktStart, pktSize, dataSize))
				return false;

			if (false == ValidateString(mUserInfoType0Offset, 0, pktStart, pktSize, dataSize))
				return false;

			if (false == ValidateString(mUserInfoValue0Offset, 0, pktStart, pktSize, dataSize))
				return false;

			if (false == ValidateString(mUserInfoType1Offset, 0, pktStart, pktSize, dataSize))
				return false;

			if (false == ValidateString(mUserInfoValue1Offset, 0, pktStart, pktSize, dataSize))
				return false;

			return (nullptr != pktStart && 0 < pktSize && 0 < dataSize) ? true : false;
		}

	}; // UserList

public:
	uint16 mPktSize;
	uint16 mPktId;
	uint16 mUserListCount;
	uint16 mFirstUserListOffset;
	int mResult;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"AC_ACK_IDP_CONFLICT_PLYER"; }
	const wchar_t* GetRelatedPacketName() const { return L"CA_REQ_IDP_CONFLICT_PLYER"; }
#endif

	int Result() { return mResult; }

	using UserListIterator = PktIterator<UserList>;
	const UserListIterator GetLastUserListIterator() const
	{
		return UserListIterator(0);
	}
	UserListIterator GetFirstUserListIterator() const
	{
		if (mFirstUserListOffset && mPktSize >= mFirstUserListOffset + sizeof(UserList))
		{
			UserList* userList = (UserList*)((uchar*)this + mFirstUserListOffset);
			if (mFirstUserListOffset == userList->mThisOffset)
				return UserListIterator(userList);
			else
				return UserListIterator(0);
		}
		else
			return UserListIterator(0);
	}
	const int GetUserListCount()
	{
		return mUserListCount;
	}
#ifdef _SERVER
	template<typename TFunc>
	void ForEachUserList(TFunc&& func)
	{
		for (auto it = GetFirstUserListIterator(); it != GetLastUserListIterator(); ++it)
			func(it);
	}
#endif


#ifdef _SERVER 
	std::xwstring FuncCreateUserListStr( [[maybe_unused]] UserListIterator& iter) const
	{
		return Log::format(
			L"UserName: {}, ServerName: {}, UserInfoType0: {}, UserInfoValue0: {}, UserInfoType1: {}, UserInfoValue1: {}", 
			iter->UserName(), iter->ServerName(), iter->UserInfoType0(), iter->UserInfoValue0(), iter->UserInfoType1(), iter->UserInfoValue1()
		).c_str();
	}

	std::xwstring GetLogWString() const
	{
		std::xwstring str1;
		str1.append(L"\n");
		for (UserListIterator iter = this->GetFirstUserListIterator();
			iter != GetLastUserListIterator(); ++iter)
		{
			str1.append(L"[");
			str1.append(FuncCreateUserListStr(iter).c_str());

			str1.append(L"]\n");
		}

		return Log::format(
			L"[AC_ACK_IDP_CONFLICT_PLYER] Result: {}, UserList : {}"
			, mResult, str1.c_str()
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_AC_ACK_IDP_CONFLICT_PLYER_READ);
		if (dataSize > mPktSize)
			return false;

		if (false == ValidateValue(pktStart, pktSize, dataSize))
			return false;

		if (false == UserListValidateListNode(mFirstUserListOffset, pktStart, pktSize, dataSize))
			return false;

		if (dataSize != mPktSize)
			return false;

		return true;
	}

	bool UserListValidateListNode(uint16 offset, uchar * pktStart, uint16 pktSize, uint16& dataSize)
	{		for (UserListIterator iter = this->GetFirstUserListIterator();
			iter != GetLastUserListIterator(); ++iter)
		{
			if (false == (*iter).Validate(offset, pktStart, pktSize, dataSize))
			{
				return false;
			}
		}
		return true;
	}

	bool ValidateValue(const uchar* pktStart, uint16 pktSize, uint16& dataSize)
	{
		return (nullptr != pktStart && 0 < pktSize && 0 < dataSize) ? true : false;
	}

}; // PKT_AC_ACK_IDP_CONFLICT_PLYER_READ

// type = AC
// related packet = PKT_CA_REQ_IDP_CONFLICT_PLYER_READ
#ifdef _SERVER
struct PKT_AC_ACK_IDP_CONFLICT_PLYER_WRITE : public Disposable
#else
struct PKT_AC_ACK_IDP_CONFLICT_PLYER_WRITE
#endif
{
	struct UserList
	{
		UserList(uint16 offset)
			:
			mThisOffset(offset)
			, mNextOffset(0), mUserNameOffset(0), mServerNameOffset(0), mUserInfoType0Offset(0), mUserInfoValue0Offset(0), mUserInfoType1Offset(0), mUserInfoValue1Offset(0)
		{
		}
		void SetUserNameOffset(uint16 offset)
		{
			mUserNameOffset = offset;
		}
		void SetServerNameOffset(uint16 offset)
		{
			mServerNameOffset = offset;
		}
		void SetUserInfoType0Offset(uint16 offset)
		{
			mUserInfoType0Offset = offset;
		}
		void SetUserInfoValue0Offset(uint16 offset)
		{
			mUserInfoValue0Offset = offset;
		}
		void SetUserInfoType1Offset(uint16 offset)
		{
			mUserInfoType1Offset = offset;
		}
		void SetUserInfoValue1Offset(uint16 offset)
		{
			mUserInfoValue1Offset = offset;
		}

		uint16 mThisOffset;
		uint16 mNextOffset;
		uint16 mUserNameOffset;
		uint16 mServerNameOffset;
		uint16 mUserInfoType0Offset;
		uint16 mUserInfoValue0Offset;
		uint16 mUserInfoType1Offset;
		uint16 mUserInfoValue1Offset;

	}; // UserList

	struct UserListHandle
	{
		UserListHandle(PktWriter& pktWriter, UserList* userList)
			:
			mPktWriter(pktWriter)
			, mUserList(userList)
		{
		}

		PktWriter& mPktWriter;
		UserList* mUserList;

	}; // UserListHandle

	PktWriter mPktWriter;
	uint16* mUserListCount;
	uint16* mFirstUserListOffset;
	UserList* mNowUserList;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_AC_ACK_IDP_CONFLICT_PLYER_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"AC_ACK_IDP_CONFLICT_PLYER"; }
	const wchar_t* GetRelatedPacketName() const { return L"CA_REQ_IDP_CONFLICT_PLYER"; }
#endif

#ifndef _SERVER
	PKT_AC_ACK_IDP_CONFLICT_PLYER_WRITE(uchar* buf, int bufSize, int result)
		:
		mPktWriter(buf, bufSize)
		, mNowUserList(0)
	{
		mPktWriter << static_cast<uint16>(PD::AC::ACPacketId::AC_ACK_IDP_CONFLICT_PLYER);
		mUserListCount = (uint16*)mPktWriter.GetBuffer();
		*mUserListCount = 0;
		mPktWriter << (*mUserListCount);
		mFirstUserListOffset = (uint16*)mPktWriter.GetBuffer();
		*mFirstUserListOffset = 0;
		mPktWriter << (*mFirstUserListOffset);
		mPktWriter << result;
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_AC_ACK_IDP_CONFLICT_PLYER_WRITE(SendBufferHelper& sendBuffer, int result)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))
		, mNowUserList(0)
	{
		mPktWriter << (uint16)AC_ACK_IDP_CONFLICT_PLYER;
		mUserListCount = (uint16*)mPktWriter.GetBuffer();
		*mUserListCount = 0;
		mPktWriter << (*mUserListCount);
		mFirstUserListOffset = (uint16*)mPktWriter.GetBuffer();
		*mFirstUserListOffset = 0;
		mPktWriter << (*mFirstUserListOffset);
		mPktWriter << result;
	}
#endif

#ifdef _SERVER 
	void send(IocpObjectSession* session, SendBufferRef sendBuffer, bool autoCloseSendBuffer)
	{
		if (session) 
			session->Send(sendBuffer, mPktWriter.GetPktSize(), autoCloseSendBuffer);
	}
#endif

#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{
		auto readPacket = reinterpret_cast<PKT_AC_ACK_IDP_CONFLICT_PLYER_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
	UserListHandle CreateUserList(const FString& userName, const FString& serverName, const FString& userInfoType0, const FString& userInfoValue0, const FString& userInfoType1, const FString& userInfoValue1)
	{
		uchar* nowBuffer = mPktWriter.GetBuffer();
		uint16 nowWritten = (uint16)mPktWriter.GetPktSize();
		UserList* newUserList = new ((UserList*)nowBuffer) UserList(nowWritten);
		++(*mUserListCount);
		if (mNowUserList == 0)
			*mFirstUserListOffset = nowWritten;
		else
			mNowUserList->mNextOffset = nowWritten;
		mNowUserList = newUserList;
		mPktWriter.Skip(sizeof(UserList));
		mNowUserList->SetUserNameOffset((uint16)mPktWriter.GetPktSize());
		mPktWriter.WriteString(userName);
		mNowUserList->SetServerNameOffset((uint16)mPktWriter.GetPktSize());
		mPktWriter.WriteString(serverName);
		mNowUserList->SetUserInfoType0Offset((uint16)mPktWriter.GetPktSize());
		mPktWriter.WriteString(userInfoType0);
		mNowUserList->SetUserInfoValue0Offset((uint16)mPktWriter.GetPktSize());
		mPktWriter.WriteString(userInfoValue0);
		mNowUserList->SetUserInfoType1Offset((uint16)mPktWriter.GetPktSize());
		mPktWriter.WriteString(userInfoType1);
		mNowUserList->SetUserInfoValue1Offset((uint16)mPktWriter.GetPktSize());
		mPktWriter.WriteString(userInfoValue1);
		return UserListHandle(mPktWriter, newUserList);
	}

#else
	UserListHandle CreateUserList(const wchar* userName, const wchar* serverName, const wchar* userInfoType0, const wchar* userInfoValue0, const wchar* userInfoType1, const wchar* userInfoValue1)
	{
		uchar* nowBuffer = mPktWriter.GetBuffer();
		uint16 nowWritten = (uint16)mPktWriter.GetPktSize();
		UserList* newUserList = new ((UserList*)nowBuffer) UserList(nowWritten);
		++(*mUserListCount);
		if (mNowUserList == 0)
			*mFirstUserListOffset = nowWritten;
		else
			mNowUserList->mNextOffset = nowWritten;
		mNowUserList = newUserList;
		mPktWriter.Skip(sizeof(UserList));
		mNowUserList->SetUserNameOffset((uint16)mPktWriter.GetPktSize());
		mPktWriter.WriteString(userName);
		mNowUserList->SetServerNameOffset((uint16)mPktWriter.GetPktSize());
		mPktWriter.WriteString(serverName);
		mNowUserList->SetUserInfoType0Offset((uint16)mPktWriter.GetPktSize());
		mPktWriter.WriteString(userInfoType0);
		mNowUserList->SetUserInfoValue0Offset((uint16)mPktWriter.GetPktSize());
		mPktWriter.WriteString(userInfoValue0);
		mNowUserList->SetUserInfoType1Offset((uint16)mPktWriter.GetPktSize());
		mPktWriter.WriteString(userInfoType1);
		mNowUserList->SetUserInfoValue1Offset((uint16)mPktWriter.GetPktSize());
		mPktWriter.WriteString(userInfoValue1);
		return UserListHandle(mPktWriter, newUserList);
	}

#endif
}; // PKT_AC_ACK_IDP_CONFLICT_PLYER_WRITE

#pragma pack(pop)
#pragma warning(pop)
} // namespace AC
} // namespace PD
