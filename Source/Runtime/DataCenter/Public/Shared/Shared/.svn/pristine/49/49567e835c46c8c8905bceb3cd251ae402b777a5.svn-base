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
namespace GD
{

#pragma warning(push)
#pragma warning(disable:4315)
#pragma warning(disable:5038)
#pragma pack(push, 1)

#ifndef _SERVER
enum class GDPacketId : uint16
#else
enum
#endif
{
	GD_PING = 7000,
	GD_REQ_REGISTER = 7001,
	GD_REQ_WORLDTIME = 7002,
	GD_REPORT_SERVER_CONGESTION = 7003,
	GD_REQ_LOGIN = 7004,
	GD_REQ_UPDATE_USER_COUNT = 7005,
	GD_NOTIFY_ACCOUNT_DISCONNECT = 7006,
	GD_NOTIFY_SETTIME_BY_COMMAND = 7007,
	GD_NOTIFY_USER_ENTER_WORLD = 7008,
	GD_NOTIFY_USER_LEAVE_WORLD = 7009,
	GD_NOTIFY_BM_PURCHASED = 7010,
	GD_NOTIFY_SERVER_MIGRATE_ACCOUNT = 7011,
	GD_REQ_SERVER_MIGRATE_CHECK_CONDITION = 7012,
	GD_ACK_BYPASS_SERVER_MIGRATE_CHECK_CONDITION = 7013,
	GD_QA_SANCTUM_BATTLE_COMMAND = 7014,
	PacketIdMax  = 7999
};

#ifdef _SERVER

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// String -> Enum
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline uint16 GDPacketIdStringToEnum(const wchar_t* str) noexcept
{
	if (_wcsicmp(L"GD_PING", str) == 0)	return GD_PING;
	if (_wcsicmp(L"GD_REQ_REGISTER", str) == 0)	return GD_REQ_REGISTER;
	if (_wcsicmp(L"GD_REQ_WORLDTIME", str) == 0)	return GD_REQ_WORLDTIME;
	if (_wcsicmp(L"GD_REPORT_SERVER_CONGESTION", str) == 0)	return GD_REPORT_SERVER_CONGESTION;
	if (_wcsicmp(L"GD_REQ_LOGIN", str) == 0)	return GD_REQ_LOGIN;
	if (_wcsicmp(L"GD_REQ_UPDATE_USER_COUNT", str) == 0)	return GD_REQ_UPDATE_USER_COUNT;
	if (_wcsicmp(L"GD_NOTIFY_ACCOUNT_DISCONNECT", str) == 0)	return GD_NOTIFY_ACCOUNT_DISCONNECT;
	if (_wcsicmp(L"GD_NOTIFY_SETTIME_BY_COMMAND", str) == 0)	return GD_NOTIFY_SETTIME_BY_COMMAND;
	if (_wcsicmp(L"GD_NOTIFY_USER_ENTER_WORLD", str) == 0)	return GD_NOTIFY_USER_ENTER_WORLD;
	if (_wcsicmp(L"GD_NOTIFY_USER_LEAVE_WORLD", str) == 0)	return GD_NOTIFY_USER_LEAVE_WORLD;
	if (_wcsicmp(L"GD_NOTIFY_BM_PURCHASED", str) == 0)	return GD_NOTIFY_BM_PURCHASED;
	if (_wcsicmp(L"GD_NOTIFY_SERVER_MIGRATE_ACCOUNT", str) == 0)	return GD_NOTIFY_SERVER_MIGRATE_ACCOUNT;
	if (_wcsicmp(L"GD_REQ_SERVER_MIGRATE_CHECK_CONDITION", str) == 0)	return GD_REQ_SERVER_MIGRATE_CHECK_CONDITION;
	if (_wcsicmp(L"GD_ACK_BYPASS_SERVER_MIGRATE_CHECK_CONDITION", str) == 0)	return GD_ACK_BYPASS_SERVER_MIGRATE_CHECK_CONDITION;
	if (_wcsicmp(L"GD_QA_SANCTUM_BATTLE_COMMAND", str) == 0)	return GD_QA_SANCTUM_BATTLE_COMMAND;
	return PacketIdMax;
}
#endif
#ifdef _SERVER

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> String
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline const wchar_t* GDPacketIdEnumToString(uint16 value) noexcept
{
	switch (value)
	{
		case GD_PING: return L"GD_PING";
		case GD_REQ_REGISTER: return L"GD_REQ_REGISTER";
		case GD_REQ_WORLDTIME: return L"GD_REQ_WORLDTIME";
		case GD_REPORT_SERVER_CONGESTION: return L"GD_REPORT_SERVER_CONGESTION";
		case GD_REQ_LOGIN: return L"GD_REQ_LOGIN";
		case GD_REQ_UPDATE_USER_COUNT: return L"GD_REQ_UPDATE_USER_COUNT";
		case GD_NOTIFY_ACCOUNT_DISCONNECT: return L"GD_NOTIFY_ACCOUNT_DISCONNECT";
		case GD_NOTIFY_SETTIME_BY_COMMAND: return L"GD_NOTIFY_SETTIME_BY_COMMAND";
		case GD_NOTIFY_USER_ENTER_WORLD: return L"GD_NOTIFY_USER_ENTER_WORLD";
		case GD_NOTIFY_USER_LEAVE_WORLD: return L"GD_NOTIFY_USER_LEAVE_WORLD";
		case GD_NOTIFY_BM_PURCHASED: return L"GD_NOTIFY_BM_PURCHASED";
		case GD_NOTIFY_SERVER_MIGRATE_ACCOUNT: return L"GD_NOTIFY_SERVER_MIGRATE_ACCOUNT";
		case GD_REQ_SERVER_MIGRATE_CHECK_CONDITION: return L"GD_REQ_SERVER_MIGRATE_CHECK_CONDITION";
		case GD_ACK_BYPASS_SERVER_MIGRATE_CHECK_CONDITION: return L"GD_ACK_BYPASS_SERVER_MIGRATE_CHECK_CONDITION";
		case GD_QA_SANCTUM_BATTLE_COMMAND: return L"GD_QA_SANCTUM_BATTLE_COMMAND";
	}
	return nullptr;
}
#endif
// type = GD
struct PKT_GD_PING_READ
{
public:
	uint16 mPktSize;
	uint16 mPktId;
	uint64 mPing;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"GD_PING"; }
	const wchar_t* GetRelatedPacketName() const { return L"DG_PING"; }
#endif

	uint64 Ping() { return mPing; }


#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{

		return Log::format(
			L"[GD_PING] Ping: {}"
			, mPing
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_GD_PING_READ);
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

}; // PKT_GD_PING_READ

// type = GD
#ifdef _SERVER
struct PKT_GD_PING_WRITE : public Disposable
#else
struct PKT_GD_PING_WRITE
#endif
{
	PktWriter mPktWriter;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_GD_PING_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"GD_PING"; }
	const wchar_t* GetRelatedPacketName() const { return L"DG_PING"; }
#endif

#ifndef _SERVER
	PKT_GD_PING_WRITE(uchar* buf, int bufSize, uint64 ping)
		:
		mPktWriter(buf, bufSize)

	{
		mPktWriter << static_cast<uint16>(PD::GD::GDPacketId::GD_PING);
		mPktWriter << ping;
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_GD_PING_WRITE(SendBufferHelper& sendBuffer, uint64 ping)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))

	{
		mPktWriter << (uint16)GD_PING;
		mPktWriter << ping;
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
		auto readPacket = reinterpret_cast<PKT_GD_PING_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
#else
#endif
}; // PKT_GD_PING_WRITE

// type = GD
// related packet = PKT_DG_ACK_REGISTER_WRITE
struct PKT_GD_REQ_REGISTER_READ
{
	struct AliveAccountInfoList
	{
	protected:
		uint16 mThisOffset;
		uint16 mNextOffset;
		Vid mVid;
		AccountDBId mAccountId;

	public:
		Vid Vid() { return mVid; }
		AccountDBId AccountId() { return mAccountId; }

		friend PktIterator<AliveAccountInfoList>;
		friend PKT_GD_REQ_REGISTER_READ;

		bool Validate(uint16 offset, uchar * pktStart, uint16 pktSize, uint16& dataSize)
		{
			if (static_cast<size_t>(pktSize) < static_cast<size_t>(offset) + sizeof(AliveAccountInfoList))
				return false;

			AliveAccountInfoList * node = (AliveAccountInfoList*)(pktStart + offset);

			// 자기 자신의 포인터 검증
			if (offset != node->mThisOffset)
				return false;

			// Value 검증
			uchar* pktCurrentNodeStart = (uchar*)this - mThisOffset;
			uint16 pktCurrentNodeSize = *((uint16*)pktStart);
			dataSize += sizeof(AliveAccountInfoList);	// skip
			if (false == ValidateValue(pktCurrentNodeStart, pktCurrentNodeSize, dataSize))
				return false;

			return true;
		}

		bool ValidateValue(const uchar* pktStart, uint16 pktSize, uint16& dataSize)
		{
			return (nullptr != pktStart && 0 < pktSize && 0 < dataSize) ? true : false;
		}

	}; // AliveAccountInfoList

public:
	uint16 mPktSize;
	uint16 mPktId;
	uint16 mAliveAccountInfoListCount;
	uint16 mFirstAliveAccountInfoListOffset;
	uint16 mConnectIpOffset;
	uint16 mPlanetWorldId;
	uint16 mListenPort;
	uint64 mReqTickCount;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"GD_REQ_REGISTER"; }
	const wchar_t* GetRelatedPacketName() const { return L"DG_ACK_REGISTER"; }
#endif

	uint16 PlanetWorldId() { return mPlanetWorldId; }
	uint16 ListenPort() { return mListenPort; }
	uint64 ReqTickCount() { return mReqTickCount; }
#ifdef _SERVER
	const wchar_t* ConnectIp() const
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = *((uint16*)pktStart);
		if (mConnectIpOffset && pktSize > mConnectIpOffset)
			return (const wchar_t*)(pktStart + mConnectIpOffset);
		else
			return L"";
	}
#else // _SERVER
	const FString ConnectIp()
	{
		const wchar_t* strWchar = L"";
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = *((uint16*)pktStart);
		if (mConnectIpOffset && pktSize > mConnectIpOffset)
			strWchar = (const wchar_t*)(pktStart + mConnectIpOffset);

		return PktWriter::ConvertToFString(strWchar);
	}
#endif // _SERVER

	using AliveAccountInfoListIterator = PktIterator<AliveAccountInfoList>;
	const AliveAccountInfoListIterator GetLastAliveAccountInfoListIterator() const
	{
		return AliveAccountInfoListIterator(0);
	}
	AliveAccountInfoListIterator GetFirstAliveAccountInfoListIterator() const
	{
		if (mFirstAliveAccountInfoListOffset && mPktSize >= mFirstAliveAccountInfoListOffset + sizeof(AliveAccountInfoList))
		{
			AliveAccountInfoList* aliveAccountInfoList = (AliveAccountInfoList*)((uchar*)this + mFirstAliveAccountInfoListOffset);
			if (mFirstAliveAccountInfoListOffset == aliveAccountInfoList->mThisOffset)
				return AliveAccountInfoListIterator(aliveAccountInfoList);
			else
				return AliveAccountInfoListIterator(0);
		}
		else
			return AliveAccountInfoListIterator(0);
	}
	const int GetAliveAccountInfoListCount()
	{
		return mAliveAccountInfoListCount;
	}
#ifdef _SERVER
	template<typename TFunc>
	void ForEachAliveAccountInfoList(TFunc&& func)
	{
		for (auto it = GetFirstAliveAccountInfoListIterator(); it != GetLastAliveAccountInfoListIterator(); ++it)
			func(it);
	}
#endif


#ifdef _SERVER 
	std::xwstring FuncCreateAliveAccountInfoListStr( [[maybe_unused]] AliveAccountInfoListIterator& iter) const
	{
		return Log::format(
			L"Vid: {}, AccountId: {}", 
			iter->Vid(), iter->AccountId()
		).c_str();
	}

	std::xwstring GetLogWString() const
	{
		std::xwstring str1;
		str1.append(L"\n");
		for (AliveAccountInfoListIterator iter = this->GetFirstAliveAccountInfoListIterator();
			iter != GetLastAliveAccountInfoListIterator(); ++iter)
		{
			str1.append(L"[");
			str1.append(FuncCreateAliveAccountInfoListStr(iter).c_str());

			str1.append(L"]\n");
		}

		return Log::format(
			L"[GD_REQ_REGISTER] PlanetWorldId: {}, ConnectIp: {}, ListenPort: {}, ReqTickCount: {}, AliveAccountInfoList : {}"
			, mPlanetWorldId, ConnectIp(), mListenPort, mReqTickCount, str1.c_str()
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_GD_REQ_REGISTER_READ);
		if (dataSize > mPktSize)
			return false;

		if (false == ValidateValue(pktStart, pktSize, dataSize))
			return false;

		if (false == AliveAccountInfoListValidateListNode(mFirstAliveAccountInfoListOffset, pktStart, pktSize, dataSize))
			return false;

		if (dataSize != mPktSize)
			return false;

		return true;
	}

	bool AliveAccountInfoListValidateListNode(uint16 offset, uchar * pktStart, uint16 pktSize, uint16& dataSize)
	{		for (AliveAccountInfoListIterator iter = this->GetFirstAliveAccountInfoListIterator();
			iter != GetLastAliveAccountInfoListIterator(); ++iter)
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
		if (false == ValidateString(mConnectIpOffset, 0, pktStart, pktSize, dataSize))
			return false;

		return (nullptr != pktStart && 0 < pktSize && 0 < dataSize) ? true : false;
	}

}; // PKT_GD_REQ_REGISTER_READ

// type = GD
// related packet = PKT_DG_ACK_REGISTER_READ
#ifdef _SERVER
struct PKT_GD_REQ_REGISTER_WRITE : public Disposable
#else
struct PKT_GD_REQ_REGISTER_WRITE
#endif
{
	struct AliveAccountInfoList
	{
		AliveAccountInfoList(uint16 offset, Vid vid, AccountDBId accountId)
			:
			mThisOffset(offset)
			, mNextOffset(0)
			, mVid(vid)
			, mAccountId(accountId)
		{
		}

		uint16 mThisOffset;
		uint16 mNextOffset;
		Vid mVid;
		AccountDBId mAccountId;

	}; // AliveAccountInfoList

	struct AliveAccountInfoListHandle
	{
		AliveAccountInfoListHandle(PktWriter& pktWriter, AliveAccountInfoList* aliveAccountInfoList)
			:
			mPktWriter(pktWriter)
			, mAliveAccountInfoList(aliveAccountInfoList)
		{
		}

		PktWriter& mPktWriter;
		AliveAccountInfoList* mAliveAccountInfoList;

	}; // AliveAccountInfoListHandle

	PktWriter mPktWriter;
	uint16* mAliveAccountInfoListCount;
	uint16* mFirstAliveAccountInfoListOffset;
	AliveAccountInfoList* mNowAliveAccountInfoList;
	uint16* mConnectIpOffset;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_GD_REQ_REGISTER_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"GD_REQ_REGISTER"; }
	const wchar_t* GetRelatedPacketName() const { return L"DG_ACK_REGISTER"; }
#endif

#ifndef _SERVER
	PKT_GD_REQ_REGISTER_WRITE(uchar* buf, int bufSize, uint16 planetWorldId, const FString& connectIp, uint16 listenPort, uint64 reqTickCount)
		:
		mPktWriter(buf, bufSize)
		, mNowAliveAccountInfoList(0)
	{
		mPktWriter << static_cast<uint16>(PD::GD::GDPacketId::GD_REQ_REGISTER);
		mAliveAccountInfoListCount = (uint16*)mPktWriter.GetBuffer();
		*mAliveAccountInfoListCount = 0;
		mPktWriter << (*mAliveAccountInfoListCount);
		mFirstAliveAccountInfoListOffset = (uint16*)mPktWriter.GetBuffer();
		*mFirstAliveAccountInfoListOffset = 0;
		mPktWriter << (*mFirstAliveAccountInfoListOffset);
		mConnectIpOffset = (uint16*)mPktWriter.GetBuffer();
		*mConnectIpOffset = 0;
		mPktWriter << (*mConnectIpOffset);
		mPktWriter << planetWorldId;
		mPktWriter << listenPort;
		mPktWriter << reqTickCount;
		*mConnectIpOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(connectIp);
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_GD_REQ_REGISTER_WRITE(SendBufferHelper& sendBuffer, uint16 planetWorldId, const wchar* connectIp, uint16 listenPort, uint64 reqTickCount)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))
		, mNowAliveAccountInfoList(0)
	{
		mPktWriter << (uint16)GD_REQ_REGISTER;
		mAliveAccountInfoListCount = (uint16*)mPktWriter.GetBuffer();
		*mAliveAccountInfoListCount = 0;
		mPktWriter << (*mAliveAccountInfoListCount);
		mFirstAliveAccountInfoListOffset = (uint16*)mPktWriter.GetBuffer();
		*mFirstAliveAccountInfoListOffset = 0;
		mPktWriter << (*mFirstAliveAccountInfoListOffset);
		mConnectIpOffset = (uint16*)mPktWriter.GetBuffer();
		*mConnectIpOffset = 0;
		mPktWriter << (*mConnectIpOffset);
		mPktWriter << planetWorldId;
		mPktWriter << listenPort;
		mPktWriter << reqTickCount;
		*mConnectIpOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(connectIp);
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
		auto readPacket = reinterpret_cast<PKT_GD_REQ_REGISTER_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
	AliveAccountInfoListHandle CreateAliveAccountInfoList(Vid vid, AccountDBId accountId)
	{
		uchar* nowBuffer = mPktWriter.GetBuffer();
		uint16 nowWritten = (uint16)mPktWriter.GetPktSize();
		AliveAccountInfoList* newAliveAccountInfoList = new ((AliveAccountInfoList*)nowBuffer) AliveAccountInfoList(nowWritten, vid, accountId);
		++(*mAliveAccountInfoListCount);
		if (mNowAliveAccountInfoList == 0)
			*mFirstAliveAccountInfoListOffset = nowWritten;
		else
			mNowAliveAccountInfoList->mNextOffset = nowWritten;
		mNowAliveAccountInfoList = newAliveAccountInfoList;
		mPktWriter.Skip(sizeof(AliveAccountInfoList));
		return AliveAccountInfoListHandle(mPktWriter, newAliveAccountInfoList);
	}

#else
	AliveAccountInfoListHandle CreateAliveAccountInfoList(Vid vid, AccountDBId accountId)
	{
		uchar* nowBuffer = mPktWriter.GetBuffer();
		uint16 nowWritten = (uint16)mPktWriter.GetPktSize();
		AliveAccountInfoList* newAliveAccountInfoList = new ((AliveAccountInfoList*)nowBuffer) AliveAccountInfoList(nowWritten, vid, accountId);
		++(*mAliveAccountInfoListCount);
		if (mNowAliveAccountInfoList == 0)
			*mFirstAliveAccountInfoListOffset = nowWritten;
		else
			mNowAliveAccountInfoList->mNextOffset = nowWritten;
		mNowAliveAccountInfoList = newAliveAccountInfoList;
		mPktWriter.Skip(sizeof(AliveAccountInfoList));
		return AliveAccountInfoListHandle(mPktWriter, newAliveAccountInfoList);
	}

#endif
}; // PKT_GD_REQ_REGISTER_WRITE

// type = GD
// related packet = PKT_DG_ACK_WORLDTIME_WRITE
struct PKT_GD_REQ_WORLDTIME_READ
{
public:
	uint16 mPktSize;
	uint16 mPktId;
	uint64 mReqTickCount;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"GD_REQ_WORLDTIME"; }
	const wchar_t* GetRelatedPacketName() const { return L"DG_ACK_WORLDTIME"; }
#endif

	uint64 ReqTickCount() { return mReqTickCount; }


#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{

		return Log::format(
			L"[GD_REQ_WORLDTIME] ReqTickCount: {}"
			, mReqTickCount
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_GD_REQ_WORLDTIME_READ);
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

}; // PKT_GD_REQ_WORLDTIME_READ

// type = GD
// related packet = PKT_DG_ACK_WORLDTIME_READ
#ifdef _SERVER
struct PKT_GD_REQ_WORLDTIME_WRITE : public Disposable
#else
struct PKT_GD_REQ_WORLDTIME_WRITE
#endif
{
	PktWriter mPktWriter;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_GD_REQ_WORLDTIME_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"GD_REQ_WORLDTIME"; }
	const wchar_t* GetRelatedPacketName() const { return L"DG_ACK_WORLDTIME"; }
#endif

#ifndef _SERVER
	PKT_GD_REQ_WORLDTIME_WRITE(uchar* buf, int bufSize, uint64 reqTickCount)
		:
		mPktWriter(buf, bufSize)

	{
		mPktWriter << static_cast<uint16>(PD::GD::GDPacketId::GD_REQ_WORLDTIME);
		mPktWriter << reqTickCount;
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_GD_REQ_WORLDTIME_WRITE(SendBufferHelper& sendBuffer, uint64 reqTickCount)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))

	{
		mPktWriter << (uint16)GD_REQ_WORLDTIME;
		mPktWriter << reqTickCount;
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
		auto readPacket = reinterpret_cast<PKT_GD_REQ_WORLDTIME_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
#else
#endif
}; // PKT_GD_REQ_WORLDTIME_WRITE

// type = GD
struct PKT_GD_REPORT_SERVER_CONGESTION_READ
{
public:
	uint16 mPktSize;
	uint16 mPktId;
	uint16 mPlanetWorldId;
	int mPlayingCount;
	int mPlayingLimit;
	int mWaitingCount;
	int mWaitingLimit;
	int mCreatedUserCount;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"GD_REPORT_SERVER_CONGESTION"; }
	const wchar_t* GetRelatedPacketName() const { return L"DG_REPORT_SERVER_CONGESTION"; }
#endif

	uint16 PlanetWorldId() { return mPlanetWorldId; }
	int PlayingCount() { return mPlayingCount; }
	int PlayingLimit() { return mPlayingLimit; }
	int WaitingCount() { return mWaitingCount; }
	int WaitingLimit() { return mWaitingLimit; }
	int CreatedUserCount() { return mCreatedUserCount; }


#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{

		return Log::format(
			L"[GD_REPORT_SERVER_CONGESTION] PlanetWorldId: {}, PlayingCount: {}, PlayingLimit: {}, WaitingCount: {}, WaitingLimit: {}, CreatedUserCount: {}"
			, mPlanetWorldId, mPlayingCount, mPlayingLimit, mWaitingCount, mWaitingLimit, mCreatedUserCount
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_GD_REPORT_SERVER_CONGESTION_READ);
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

}; // PKT_GD_REPORT_SERVER_CONGESTION_READ

// type = GD
#ifdef _SERVER
struct PKT_GD_REPORT_SERVER_CONGESTION_WRITE : public Disposable
#else
struct PKT_GD_REPORT_SERVER_CONGESTION_WRITE
#endif
{
	PktWriter mPktWriter;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_GD_REPORT_SERVER_CONGESTION_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"GD_REPORT_SERVER_CONGESTION"; }
	const wchar_t* GetRelatedPacketName() const { return L"DG_REPORT_SERVER_CONGESTION"; }
#endif

#ifndef _SERVER
	PKT_GD_REPORT_SERVER_CONGESTION_WRITE(uchar* buf, int bufSize, uint16 planetWorldId, int playingCount, int playingLimit, int waitingCount, int waitingLimit, int createdUserCount)
		:
		mPktWriter(buf, bufSize)

	{
		mPktWriter << static_cast<uint16>(PD::GD::GDPacketId::GD_REPORT_SERVER_CONGESTION);
		mPktWriter << planetWorldId;
		mPktWriter << playingCount;
		mPktWriter << playingLimit;
		mPktWriter << waitingCount;
		mPktWriter << waitingLimit;
		mPktWriter << createdUserCount;
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_GD_REPORT_SERVER_CONGESTION_WRITE(SendBufferHelper& sendBuffer, uint16 planetWorldId, int playingCount, int playingLimit, int waitingCount, int waitingLimit, int createdUserCount)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))

	{
		mPktWriter << (uint16)GD_REPORT_SERVER_CONGESTION;
		mPktWriter << planetWorldId;
		mPktWriter << playingCount;
		mPktWriter << playingLimit;
		mPktWriter << waitingCount;
		mPktWriter << waitingLimit;
		mPktWriter << createdUserCount;
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
		auto readPacket = reinterpret_cast<PKT_GD_REPORT_SERVER_CONGESTION_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
#else
#endif
}; // PKT_GD_REPORT_SERVER_CONGESTION_WRITE

// type = GD
// related packet = PKT_DG_ACK_LOGIN_WRITE
struct PKT_GD_REQ_LOGIN_READ
{
public:
	uint16 mPktSize;
	uint16 mPktId;
	AccountDBId mAccountId;
	Vid mVid;
	AuthTicket mAuthTicket;
	uint64 mClientSession;
	bool mIsZPaySession;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"GD_REQ_LOGIN"; }
	const wchar_t* GetRelatedPacketName() const { return L"DG_ACK_LOGIN"; }
#endif

	AccountDBId AccountId() { return mAccountId; }
	Vid Vid() { return mVid; }
	AuthTicket AuthTicket() { return mAuthTicket; }
	uint64 ClientSession() { return mClientSession; }
	bool IsZPaySession() { return mIsZPaySession; }


#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{

		return Log::format(
			L"[GD_REQ_LOGIN] AccountId: {}, Vid: {}, AuthTicket: {}, ClientSession: {}, IsZPaySession: {}"
			, mAccountId, mVid, mAuthTicket, mClientSession, mIsZPaySession
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_GD_REQ_LOGIN_READ);
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

}; // PKT_GD_REQ_LOGIN_READ

// type = GD
// related packet = PKT_DG_ACK_LOGIN_READ
#ifdef _SERVER
struct PKT_GD_REQ_LOGIN_WRITE : public Disposable
#else
struct PKT_GD_REQ_LOGIN_WRITE
#endif
{
	PktWriter mPktWriter;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_GD_REQ_LOGIN_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"GD_REQ_LOGIN"; }
	const wchar_t* GetRelatedPacketName() const { return L"DG_ACK_LOGIN"; }
#endif

#ifndef _SERVER
	PKT_GD_REQ_LOGIN_WRITE(uchar* buf, int bufSize, AccountDBId accountId, Vid vid, AuthTicket authTicket, uint64 clientSession, bool isZPaySession)
		:
		mPktWriter(buf, bufSize)

	{
		mPktWriter << static_cast<uint16>(PD::GD::GDPacketId::GD_REQ_LOGIN);
		mPktWriter << accountId;
		mPktWriter << vid;
		mPktWriter << authTicket;
		mPktWriter << clientSession;
		mPktWriter << isZPaySession;
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_GD_REQ_LOGIN_WRITE(SendBufferHelper& sendBuffer, AccountDBId accountId, Vid vid, AuthTicket authTicket, uint64 clientSession, bool isZPaySession)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))

	{
		mPktWriter << (uint16)GD_REQ_LOGIN;
		mPktWriter << accountId;
		mPktWriter << vid;
		mPktWriter << authTicket;
		mPktWriter << clientSession;
		mPktWriter << isZPaySession;
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
		auto readPacket = reinterpret_cast<PKT_GD_REQ_LOGIN_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
#else
#endif
}; // PKT_GD_REQ_LOGIN_WRITE

// type = GD
struct PKT_GD_REQ_UPDATE_USER_COUNT_READ
{
public:
	uint16 mPktSize;
	uint16 mPktId;
	AccountDBId mAccountId;
	uchar mNewCount;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"GD_REQ_UPDATE_USER_COUNT"; }
	const wchar_t* GetRelatedPacketName() const { return L"DG_ACK_UPDATE_USER_COUNT"; }
#endif

	AccountDBId AccountId() { return mAccountId; }
	uchar NewCount() { return mNewCount; }


#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{

		return Log::format(
			L"[GD_REQ_UPDATE_USER_COUNT] AccountId: {}, NewCount: {}"
			, mAccountId, mNewCount
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_GD_REQ_UPDATE_USER_COUNT_READ);
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

}; // PKT_GD_REQ_UPDATE_USER_COUNT_READ

// type = GD
#ifdef _SERVER
struct PKT_GD_REQ_UPDATE_USER_COUNT_WRITE : public Disposable
#else
struct PKT_GD_REQ_UPDATE_USER_COUNT_WRITE
#endif
{
	PktWriter mPktWriter;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_GD_REQ_UPDATE_USER_COUNT_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"GD_REQ_UPDATE_USER_COUNT"; }
	const wchar_t* GetRelatedPacketName() const { return L"DG_ACK_UPDATE_USER_COUNT"; }
#endif

#ifndef _SERVER
	PKT_GD_REQ_UPDATE_USER_COUNT_WRITE(uchar* buf, int bufSize, AccountDBId accountId, uchar newCount)
		:
		mPktWriter(buf, bufSize)

	{
		mPktWriter << static_cast<uint16>(PD::GD::GDPacketId::GD_REQ_UPDATE_USER_COUNT);
		mPktWriter << accountId;
		mPktWriter << newCount;
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_GD_REQ_UPDATE_USER_COUNT_WRITE(SendBufferHelper& sendBuffer, AccountDBId accountId, uchar newCount)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))

	{
		mPktWriter << (uint16)GD_REQ_UPDATE_USER_COUNT;
		mPktWriter << accountId;
		mPktWriter << newCount;
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
		auto readPacket = reinterpret_cast<PKT_GD_REQ_UPDATE_USER_COUNT_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
#else
#endif
}; // PKT_GD_REQ_UPDATE_USER_COUNT_WRITE

// type = GD
struct PKT_GD_NOTIFY_ACCOUNT_DISCONNECT_READ
{
public:
	uint16 mPktSize;
	uint16 mPktId;
	AccountDBId mAccountId;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"GD_NOTIFY_ACCOUNT_DISCONNECT"; }
	const wchar_t* GetRelatedPacketName() const { return L"DG_NOTIFY_ACCOUNT_DISCONNECT"; }
#endif

	AccountDBId AccountId() { return mAccountId; }


#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{

		return Log::format(
			L"[GD_NOTIFY_ACCOUNT_DISCONNECT] AccountId: {}"
			, mAccountId
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_GD_NOTIFY_ACCOUNT_DISCONNECT_READ);
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

}; // PKT_GD_NOTIFY_ACCOUNT_DISCONNECT_READ

// type = GD
#ifdef _SERVER
struct PKT_GD_NOTIFY_ACCOUNT_DISCONNECT_WRITE : public Disposable
#else
struct PKT_GD_NOTIFY_ACCOUNT_DISCONNECT_WRITE
#endif
{
	PktWriter mPktWriter;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_GD_NOTIFY_ACCOUNT_DISCONNECT_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"GD_NOTIFY_ACCOUNT_DISCONNECT"; }
	const wchar_t* GetRelatedPacketName() const { return L"DG_NOTIFY_ACCOUNT_DISCONNECT"; }
#endif

#ifndef _SERVER
	PKT_GD_NOTIFY_ACCOUNT_DISCONNECT_WRITE(uchar* buf, int bufSize, AccountDBId accountId)
		:
		mPktWriter(buf, bufSize)

	{
		mPktWriter << static_cast<uint16>(PD::GD::GDPacketId::GD_NOTIFY_ACCOUNT_DISCONNECT);
		mPktWriter << accountId;
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_GD_NOTIFY_ACCOUNT_DISCONNECT_WRITE(SendBufferHelper& sendBuffer, AccountDBId accountId)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))

	{
		mPktWriter << (uint16)GD_NOTIFY_ACCOUNT_DISCONNECT;
		mPktWriter << accountId;
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
		auto readPacket = reinterpret_cast<PKT_GD_NOTIFY_ACCOUNT_DISCONNECT_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
#else
#endif
}; // PKT_GD_NOTIFY_ACCOUNT_DISCONNECT_WRITE

// type = GD
struct PKT_GD_NOTIFY_SETTIME_BY_COMMAND_READ
{
public:
	uint16 mPktSize;
	uint16 mPktId;
	time_t mChangedTime;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"GD_NOTIFY_SETTIME_BY_COMMAND"; }
	const wchar_t* GetRelatedPacketName() const { return L"DG_NOTIFY_SETTIME_BY_COMMAND"; }
#endif

	time_t ChangedTime() { return mChangedTime; }


#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{

		return Log::format(
			L"[GD_NOTIFY_SETTIME_BY_COMMAND] ChangedTime: {}"
			, mChangedTime
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_GD_NOTIFY_SETTIME_BY_COMMAND_READ);
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

}; // PKT_GD_NOTIFY_SETTIME_BY_COMMAND_READ

// type = GD
#ifdef _SERVER
struct PKT_GD_NOTIFY_SETTIME_BY_COMMAND_WRITE : public Disposable
#else
struct PKT_GD_NOTIFY_SETTIME_BY_COMMAND_WRITE
#endif
{
	PktWriter mPktWriter;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_GD_NOTIFY_SETTIME_BY_COMMAND_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"GD_NOTIFY_SETTIME_BY_COMMAND"; }
	const wchar_t* GetRelatedPacketName() const { return L"DG_NOTIFY_SETTIME_BY_COMMAND"; }
#endif

#ifndef _SERVER
	PKT_GD_NOTIFY_SETTIME_BY_COMMAND_WRITE(uchar* buf, int bufSize, time_t changedTime)
		:
		mPktWriter(buf, bufSize)

	{
		mPktWriter << static_cast<uint16>(PD::GD::GDPacketId::GD_NOTIFY_SETTIME_BY_COMMAND);
		mPktWriter << changedTime;
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_GD_NOTIFY_SETTIME_BY_COMMAND_WRITE(SendBufferHelper& sendBuffer, time_t changedTime)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))

	{
		mPktWriter << (uint16)GD_NOTIFY_SETTIME_BY_COMMAND;
		mPktWriter << changedTime;
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
		auto readPacket = reinterpret_cast<PKT_GD_NOTIFY_SETTIME_BY_COMMAND_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
#else
#endif
}; // PKT_GD_NOTIFY_SETTIME_BY_COMMAND_WRITE

// type = GD
struct PKT_GD_NOTIFY_USER_ENTER_WORLD_READ
{
public:
	uint16 mPktSize;
	uint16 mPktId;
	uint16 mUserNameOffset;
	AccountDBId mAccountId;
	bool mIsOffLinePlay;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"GD_NOTIFY_USER_ENTER_WORLD"; }
	const wchar_t* GetRelatedPacketName() const { return L"DG_NOTIFY_USER_ENTER_WORLD"; }
#endif

	AccountDBId AccountId() { return mAccountId; }
	bool IsOffLinePlay() { return mIsOffLinePlay; }
#ifdef _SERVER
	const wchar_t* UserName() const
	{
		uchar* pktStart = (uchar*)this;
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
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = *((uint16*)pktStart);
		if (mUserNameOffset && pktSize > mUserNameOffset)
			strWchar = (const wchar_t*)(pktStart + mUserNameOffset);

		return PktWriter::ConvertToFString(strWchar);
	}
#endif // _SERVER


#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{

		return Log::format(
			L"[GD_NOTIFY_USER_ENTER_WORLD] AccountId: {}, UserName: {}, IsOffLinePlay: {}"
			, mAccountId, UserName(), mIsOffLinePlay
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_GD_NOTIFY_USER_ENTER_WORLD_READ);
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
		if (false == ValidateString(mUserNameOffset, 0, pktStart, pktSize, dataSize))
			return false;

		return (nullptr != pktStart && 0 < pktSize && 0 < dataSize) ? true : false;
	}

}; // PKT_GD_NOTIFY_USER_ENTER_WORLD_READ

// type = GD
#ifdef _SERVER
struct PKT_GD_NOTIFY_USER_ENTER_WORLD_WRITE : public Disposable
#else
struct PKT_GD_NOTIFY_USER_ENTER_WORLD_WRITE
#endif
{
	PktWriter mPktWriter;
	uint16* mUserNameOffset;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_GD_NOTIFY_USER_ENTER_WORLD_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"GD_NOTIFY_USER_ENTER_WORLD"; }
	const wchar_t* GetRelatedPacketName() const { return L"DG_NOTIFY_USER_ENTER_WORLD"; }
#endif

#ifndef _SERVER
	PKT_GD_NOTIFY_USER_ENTER_WORLD_WRITE(uchar* buf, int bufSize, AccountDBId accountId, const FString& userName, bool isOffLinePlay)
		:
		mPktWriter(buf, bufSize)

	{
		mPktWriter << static_cast<uint16>(PD::GD::GDPacketId::GD_NOTIFY_USER_ENTER_WORLD);
		mUserNameOffset = (uint16*)mPktWriter.GetBuffer();
		*mUserNameOffset = 0;
		mPktWriter << (*mUserNameOffset);
		mPktWriter << accountId;
		mPktWriter << isOffLinePlay;
		*mUserNameOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(userName);
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_GD_NOTIFY_USER_ENTER_WORLD_WRITE(SendBufferHelper& sendBuffer, AccountDBId accountId, const wchar* userName, bool isOffLinePlay)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))

	{
		mPktWriter << (uint16)GD_NOTIFY_USER_ENTER_WORLD;
		mUserNameOffset = (uint16*)mPktWriter.GetBuffer();
		*mUserNameOffset = 0;
		mPktWriter << (*mUserNameOffset);
		mPktWriter << accountId;
		mPktWriter << isOffLinePlay;
		*mUserNameOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(userName);
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
		auto readPacket = reinterpret_cast<PKT_GD_NOTIFY_USER_ENTER_WORLD_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
#else
#endif
}; // PKT_GD_NOTIFY_USER_ENTER_WORLD_WRITE

// type = GD
struct PKT_GD_NOTIFY_USER_LEAVE_WORLD_READ
{
public:
	uint16 mPktSize;
	uint16 mPktId;
	AccountDBId mAccountId;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"GD_NOTIFY_USER_LEAVE_WORLD"; }
	const wchar_t* GetRelatedPacketName() const { return L"DG_NOTIFY_USER_LEAVE_WORLD"; }
#endif

	AccountDBId AccountId() { return mAccountId; }


#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{

		return Log::format(
			L"[GD_NOTIFY_USER_LEAVE_WORLD] AccountId: {}"
			, mAccountId
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_GD_NOTIFY_USER_LEAVE_WORLD_READ);
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

}; // PKT_GD_NOTIFY_USER_LEAVE_WORLD_READ

// type = GD
#ifdef _SERVER
struct PKT_GD_NOTIFY_USER_LEAVE_WORLD_WRITE : public Disposable
#else
struct PKT_GD_NOTIFY_USER_LEAVE_WORLD_WRITE
#endif
{
	PktWriter mPktWriter;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_GD_NOTIFY_USER_LEAVE_WORLD_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"GD_NOTIFY_USER_LEAVE_WORLD"; }
	const wchar_t* GetRelatedPacketName() const { return L"DG_NOTIFY_USER_LEAVE_WORLD"; }
#endif

#ifndef _SERVER
	PKT_GD_NOTIFY_USER_LEAVE_WORLD_WRITE(uchar* buf, int bufSize, AccountDBId accountId)
		:
		mPktWriter(buf, bufSize)

	{
		mPktWriter << static_cast<uint16>(PD::GD::GDPacketId::GD_NOTIFY_USER_LEAVE_WORLD);
		mPktWriter << accountId;
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_GD_NOTIFY_USER_LEAVE_WORLD_WRITE(SendBufferHelper& sendBuffer, AccountDBId accountId)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))

	{
		mPktWriter << (uint16)GD_NOTIFY_USER_LEAVE_WORLD;
		mPktWriter << accountId;
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
		auto readPacket = reinterpret_cast<PKT_GD_NOTIFY_USER_LEAVE_WORLD_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
#else
#endif
}; // PKT_GD_NOTIFY_USER_LEAVE_WORLD_WRITE

// type = GD
struct PKT_GD_NOTIFY_BM_PURCHASED_READ
{
public:
	uint16 mPktSize;
	uint16 mPktId;
	Currency mCurrency;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"GD_NOTIFY_BM_PURCHASED"; }
	const wchar_t* GetRelatedPacketName() const { return L"DG_NOTIFY_BM_PURCHASED"; }
#endif

	Currency Currency() { return mCurrency; }


#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{

		return Log::format(
			L"[GD_NOTIFY_BM_PURCHASED] Currency: {}"
			, mCurrency
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_GD_NOTIFY_BM_PURCHASED_READ);
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

}; // PKT_GD_NOTIFY_BM_PURCHASED_READ

// type = GD
#ifdef _SERVER
struct PKT_GD_NOTIFY_BM_PURCHASED_WRITE : public Disposable
#else
struct PKT_GD_NOTIFY_BM_PURCHASED_WRITE
#endif
{
	PktWriter mPktWriter;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_GD_NOTIFY_BM_PURCHASED_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"GD_NOTIFY_BM_PURCHASED"; }
	const wchar_t* GetRelatedPacketName() const { return L"DG_NOTIFY_BM_PURCHASED"; }
#endif

#ifndef _SERVER
	PKT_GD_NOTIFY_BM_PURCHASED_WRITE(uchar* buf, int bufSize, Currency currency)
		:
		mPktWriter(buf, bufSize)

	{
		mPktWriter << static_cast<uint16>(PD::GD::GDPacketId::GD_NOTIFY_BM_PURCHASED);
		mPktWriter << currency;
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_GD_NOTIFY_BM_PURCHASED_WRITE(SendBufferHelper& sendBuffer, Currency currency)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))

	{
		mPktWriter << (uint16)GD_NOTIFY_BM_PURCHASED;
		mPktWriter << currency;
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
		auto readPacket = reinterpret_cast<PKT_GD_NOTIFY_BM_PURCHASED_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
#else
#endif
}; // PKT_GD_NOTIFY_BM_PURCHASED_WRITE

// type = GD
// related packet = PKT_DG_NOTIFY_SERVER_MIGRATE_ACCOUNT_WRITE
struct PKT_GD_NOTIFY_SERVER_MIGRATE_ACCOUNT_READ
{
public:
	uint16 mPktSize;
	uint16 mPktId;
	PlanetId mTargetPlanetId;
	WorldId mTargetWorldId;
	AccountDBId mAccountDBId;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"GD_NOTIFY_SERVER_MIGRATE_ACCOUNT"; }
	const wchar_t* GetRelatedPacketName() const { return L"DG_NOTIFY_SERVER_MIGRATE_ACCOUNT"; }
#endif

	PlanetId TargetPlanetId() { return mTargetPlanetId; }
	WorldId TargetWorldId() { return mTargetWorldId; }
	AccountDBId AccountDBId() { return mAccountDBId; }


#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{

		return Log::format(
			L"[GD_NOTIFY_SERVER_MIGRATE_ACCOUNT] TargetPlanetId: {}, TargetWorldId: {}, AccountDBId: {}"
			, mTargetPlanetId, mTargetWorldId, mAccountDBId
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_GD_NOTIFY_SERVER_MIGRATE_ACCOUNT_READ);
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

}; // PKT_GD_NOTIFY_SERVER_MIGRATE_ACCOUNT_READ

// type = GD
// related packet = PKT_DG_NOTIFY_SERVER_MIGRATE_ACCOUNT_READ
#ifdef _SERVER
struct PKT_GD_NOTIFY_SERVER_MIGRATE_ACCOUNT_WRITE : public Disposable
#else
struct PKT_GD_NOTIFY_SERVER_MIGRATE_ACCOUNT_WRITE
#endif
{
	PktWriter mPktWriter;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_GD_NOTIFY_SERVER_MIGRATE_ACCOUNT_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"GD_NOTIFY_SERVER_MIGRATE_ACCOUNT"; }
	const wchar_t* GetRelatedPacketName() const { return L"DG_NOTIFY_SERVER_MIGRATE_ACCOUNT"; }
#endif

#ifndef _SERVER
	PKT_GD_NOTIFY_SERVER_MIGRATE_ACCOUNT_WRITE(uchar* buf, int bufSize, PlanetId targetPlanetId, WorldId targetWorldId, AccountDBId accountDBId)
		:
		mPktWriter(buf, bufSize)

	{
		mPktWriter << static_cast<uint16>(PD::GD::GDPacketId::GD_NOTIFY_SERVER_MIGRATE_ACCOUNT);
		mPktWriter << targetPlanetId;
		mPktWriter << targetWorldId;
		mPktWriter << accountDBId;
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_GD_NOTIFY_SERVER_MIGRATE_ACCOUNT_WRITE(SendBufferHelper& sendBuffer, PlanetId targetPlanetId, WorldId targetWorldId, AccountDBId accountDBId)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))

	{
		mPktWriter << (uint16)GD_NOTIFY_SERVER_MIGRATE_ACCOUNT;
		mPktWriter << targetPlanetId;
		mPktWriter << targetWorldId;
		mPktWriter << accountDBId;
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
		auto readPacket = reinterpret_cast<PKT_GD_NOTIFY_SERVER_MIGRATE_ACCOUNT_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
#else
#endif
}; // PKT_GD_NOTIFY_SERVER_MIGRATE_ACCOUNT_WRITE

// type = GD
// related packet = PKT_DG_ACK_SERVER_MIGRATE_CHECK_CONDITION_WRITE
struct PKT_GD_REQ_SERVER_MIGRATE_CHECK_CONDITION_READ
{
public:
	uint16 mPktSize;
	uint16 mPktId;
	ServerMigrateType mMigrateType;
	PlanetWorldId mSourcePlanetWorldId;
	PlanetWorldId mTargetPlanetWorldId;
	AccountDBId mAccountDBId;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"GD_REQ_SERVER_MIGRATE_CHECK_CONDITION"; }
	const wchar_t* GetRelatedPacketName() const { return L"DG_ACK_SERVER_MIGRATE_CHECK_CONDITION"; }
#endif

	const ServerMigrateType& ConstRefMigrateType() { return mMigrateType; }
	ServerMigrateType MigrateType() { return mMigrateType; }
	const PlanetWorldId& ConstRefSourcePlanetWorldId() { return mSourcePlanetWorldId; }
	PlanetWorldId SourcePlanetWorldId() { return mSourcePlanetWorldId; }
	const PlanetWorldId& ConstRefTargetPlanetWorldId() { return mTargetPlanetWorldId; }
	PlanetWorldId TargetPlanetWorldId() { return mTargetPlanetWorldId; }
	AccountDBId AccountDBId() { return mAccountDBId; }


#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{

		return Log::format(
			L"[GD_REQ_SERVER_MIGRATE_CHECK_CONDITION] MigrateType: {}, SourcePlanetWorldId: {}, TargetPlanetWorldId: {}, AccountDBId: {}"
			, mMigrateType, mSourcePlanetWorldId, mTargetPlanetWorldId, mAccountDBId
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_GD_REQ_SERVER_MIGRATE_CHECK_CONDITION_READ);
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

}; // PKT_GD_REQ_SERVER_MIGRATE_CHECK_CONDITION_READ

// type = GD
// related packet = PKT_DG_ACK_SERVER_MIGRATE_CHECK_CONDITION_READ
#ifdef _SERVER
struct PKT_GD_REQ_SERVER_MIGRATE_CHECK_CONDITION_WRITE : public Disposable
#else
struct PKT_GD_REQ_SERVER_MIGRATE_CHECK_CONDITION_WRITE
#endif
{
	PktWriter mPktWriter;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_GD_REQ_SERVER_MIGRATE_CHECK_CONDITION_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"GD_REQ_SERVER_MIGRATE_CHECK_CONDITION"; }
	const wchar_t* GetRelatedPacketName() const { return L"DG_ACK_SERVER_MIGRATE_CHECK_CONDITION"; }
#endif

#ifndef _SERVER
	PKT_GD_REQ_SERVER_MIGRATE_CHECK_CONDITION_WRITE(uchar* buf, int bufSize, ServerMigrateType migrateType, PlanetWorldId sourcePlanetWorldId, PlanetWorldId targetPlanetWorldId, AccountDBId accountDBId)
		:
		mPktWriter(buf, bufSize)

	{
		mPktWriter << static_cast<uint16>(PD::GD::GDPacketId::GD_REQ_SERVER_MIGRATE_CHECK_CONDITION);
		mPktWriter << migrateType;
		mPktWriter << sourcePlanetWorldId;
		mPktWriter << targetPlanetWorldId;
		mPktWriter << accountDBId;
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_GD_REQ_SERVER_MIGRATE_CHECK_CONDITION_WRITE(SendBufferHelper& sendBuffer, const ServerMigrateType& migrateType, const PlanetWorldId& sourcePlanetWorldId, const PlanetWorldId& targetPlanetWorldId, AccountDBId accountDBId)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))

	{
		mPktWriter << (uint16)GD_REQ_SERVER_MIGRATE_CHECK_CONDITION;
		mPktWriter << migrateType;
		mPktWriter << sourcePlanetWorldId;
		mPktWriter << targetPlanetWorldId;
		mPktWriter << accountDBId;
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
		auto readPacket = reinterpret_cast<PKT_GD_REQ_SERVER_MIGRATE_CHECK_CONDITION_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
#else
#endif
}; // PKT_GD_REQ_SERVER_MIGRATE_CHECK_CONDITION_WRITE

// type = GD
// related packet = PKT_DG_REQ_BYPASS_SERVER_MIGRATE_CHECK_CONDITION_WRITE
struct PKT_GD_ACK_BYPASS_SERVER_MIGRATE_CHECK_CONDITION_READ
{
public:
	uint16 mPktSize;
	uint16 mPktId;
	int mResult;
	ServerMigrateType mMigrateType;
	PlanetWorldId mSourcePlanetWorldId;
	PlanetWorldId mTargetPlanetWorldId;
	AccountDBId mAccountDBId;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"GD_ACK_BYPASS_SERVER_MIGRATE_CHECK_CONDITION"; }
	const wchar_t* GetRelatedPacketName() const { return L"DG_REQ_BYPASS_SERVER_MIGRATE_CHECK_CONDITION"; }
#endif

	int Result() { return mResult; }
	const ServerMigrateType& ConstRefMigrateType() { return mMigrateType; }
	ServerMigrateType MigrateType() { return mMigrateType; }
	const PlanetWorldId& ConstRefSourcePlanetWorldId() { return mSourcePlanetWorldId; }
	PlanetWorldId SourcePlanetWorldId() { return mSourcePlanetWorldId; }
	const PlanetWorldId& ConstRefTargetPlanetWorldId() { return mTargetPlanetWorldId; }
	PlanetWorldId TargetPlanetWorldId() { return mTargetPlanetWorldId; }
	AccountDBId AccountDBId() { return mAccountDBId; }


#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{

		return Log::format(
			L"[GD_ACK_BYPASS_SERVER_MIGRATE_CHECK_CONDITION] Result: {}, MigrateType: {}, SourcePlanetWorldId: {}, TargetPlanetWorldId: {}, AccountDBId: {}"
			, mResult, mMigrateType, mSourcePlanetWorldId, mTargetPlanetWorldId, mAccountDBId
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_GD_ACK_BYPASS_SERVER_MIGRATE_CHECK_CONDITION_READ);
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

}; // PKT_GD_ACK_BYPASS_SERVER_MIGRATE_CHECK_CONDITION_READ

// type = GD
// related packet = PKT_DG_REQ_BYPASS_SERVER_MIGRATE_CHECK_CONDITION_READ
#ifdef _SERVER
struct PKT_GD_ACK_BYPASS_SERVER_MIGRATE_CHECK_CONDITION_WRITE : public Disposable
#else
struct PKT_GD_ACK_BYPASS_SERVER_MIGRATE_CHECK_CONDITION_WRITE
#endif
{
	PktWriter mPktWriter;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_GD_ACK_BYPASS_SERVER_MIGRATE_CHECK_CONDITION_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"GD_ACK_BYPASS_SERVER_MIGRATE_CHECK_CONDITION"; }
	const wchar_t* GetRelatedPacketName() const { return L"DG_REQ_BYPASS_SERVER_MIGRATE_CHECK_CONDITION"; }
#endif

#ifndef _SERVER
	PKT_GD_ACK_BYPASS_SERVER_MIGRATE_CHECK_CONDITION_WRITE(uchar* buf, int bufSize, int result, ServerMigrateType migrateType, PlanetWorldId sourcePlanetWorldId, PlanetWorldId targetPlanetWorldId, AccountDBId accountDBId)
		:
		mPktWriter(buf, bufSize)

	{
		mPktWriter << static_cast<uint16>(PD::GD::GDPacketId::GD_ACK_BYPASS_SERVER_MIGRATE_CHECK_CONDITION);
		mPktWriter << result;
		mPktWriter << migrateType;
		mPktWriter << sourcePlanetWorldId;
		mPktWriter << targetPlanetWorldId;
		mPktWriter << accountDBId;
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_GD_ACK_BYPASS_SERVER_MIGRATE_CHECK_CONDITION_WRITE(SendBufferHelper& sendBuffer, int result, const ServerMigrateType& migrateType, const PlanetWorldId& sourcePlanetWorldId, const PlanetWorldId& targetPlanetWorldId, AccountDBId accountDBId)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))

	{
		mPktWriter << (uint16)GD_ACK_BYPASS_SERVER_MIGRATE_CHECK_CONDITION;
		mPktWriter << result;
		mPktWriter << migrateType;
		mPktWriter << sourcePlanetWorldId;
		mPktWriter << targetPlanetWorldId;
		mPktWriter << accountDBId;
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
		auto readPacket = reinterpret_cast<PKT_GD_ACK_BYPASS_SERVER_MIGRATE_CHECK_CONDITION_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
#else
#endif
}; // PKT_GD_ACK_BYPASS_SERVER_MIGRATE_CHECK_CONDITION_WRITE

// type = GD
// related packet = PKT_DG_QA_SANCTUM_BATTLE_COMMAND_WRITE
struct PKT_GD_QA_SANCTUM_BATTLE_COMMAND_READ
{
public:
	uint16 mPktSize;
	uint16 mPktId;
	SanctumBattleCmd mSanctumBattleCmd;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"GD_QA_SANCTUM_BATTLE_COMMAND"; }
	const wchar_t* GetRelatedPacketName() const { return L"DG_QA_SANCTUM_BATTLE_COMMAND"; }
#endif

	const SanctumBattleCmd& ConstRefSanctumBattleCmd() { return mSanctumBattleCmd; }
	SanctumBattleCmd SanctumBattleCmd() { return mSanctumBattleCmd; }


#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{

		return Log::format(
			L"[GD_QA_SANCTUM_BATTLE_COMMAND] SanctumBattleCmd: {}"
			, mSanctumBattleCmd
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_GD_QA_SANCTUM_BATTLE_COMMAND_READ);
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

}; // PKT_GD_QA_SANCTUM_BATTLE_COMMAND_READ

// type = GD
// related packet = PKT_DG_QA_SANCTUM_BATTLE_COMMAND_READ
#ifdef _SERVER
struct PKT_GD_QA_SANCTUM_BATTLE_COMMAND_WRITE : public Disposable
#else
struct PKT_GD_QA_SANCTUM_BATTLE_COMMAND_WRITE
#endif
{
	PktWriter mPktWriter;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_GD_QA_SANCTUM_BATTLE_COMMAND_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"GD_QA_SANCTUM_BATTLE_COMMAND"; }
	const wchar_t* GetRelatedPacketName() const { return L"DG_QA_SANCTUM_BATTLE_COMMAND"; }
#endif

#ifndef _SERVER
	PKT_GD_QA_SANCTUM_BATTLE_COMMAND_WRITE(uchar* buf, int bufSize, SanctumBattleCmd sanctumBattleCmd)
		:
		mPktWriter(buf, bufSize)

	{
		mPktWriter << static_cast<uint16>(PD::GD::GDPacketId::GD_QA_SANCTUM_BATTLE_COMMAND);
		mPktWriter << sanctumBattleCmd;
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_GD_QA_SANCTUM_BATTLE_COMMAND_WRITE(SendBufferHelper& sendBuffer, const SanctumBattleCmd& sanctumBattleCmd)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))

	{
		mPktWriter << (uint16)GD_QA_SANCTUM_BATTLE_COMMAND;
		mPktWriter << sanctumBattleCmd;
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
		auto readPacket = reinterpret_cast<PKT_GD_QA_SANCTUM_BATTLE_COMMAND_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
#else
#endif
}; // PKT_GD_QA_SANCTUM_BATTLE_COMMAND_WRITE

#pragma pack(pop)
#pragma warning(pop)
} // namespace GD
} // namespace PD
