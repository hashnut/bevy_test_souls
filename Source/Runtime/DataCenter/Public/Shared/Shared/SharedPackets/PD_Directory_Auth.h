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
namespace DA
{

#pragma warning(push)
#pragma warning(disable:4315)
#pragma warning(disable:5038)
#pragma pack(push, 1)

#ifndef _SERVER
enum class DAPacketId : uint16
#else
enum
#endif
{
	DA_PONG = 4000,
	DA_ACK_REGISTER = 4001,
	DA_ACK_GATEWAY_SERVER_LIST = 4002,
	DA_GATEWAY_SERVER_CONNECTED = 4003,
	DA_GATEWAY_SERVER_DISCONNECTED = 4004,
	DA_GATEWAY_SERVER_CONGESTION_INFO = 4005,
	DA_ACK_PRE_LOGIN = 4006,
	DA_ACK_LOGIN = 4007,
	DA_GATEWAY_SEVER_ADD = 4008,
	DA_GATEWAY_SEVER_UPDATE = 4009,
	PacketIdMax  = 4999
};

#ifdef _SERVER

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// String -> Enum
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline uint16 DAPacketIdStringToEnum(const wchar_t* str) noexcept
{
	if (_wcsicmp(L"DA_PONG", str) == 0)	return DA_PONG;
	if (_wcsicmp(L"DA_ACK_REGISTER", str) == 0)	return DA_ACK_REGISTER;
	if (_wcsicmp(L"DA_ACK_GATEWAY_SERVER_LIST", str) == 0)	return DA_ACK_GATEWAY_SERVER_LIST;
	if (_wcsicmp(L"DA_GATEWAY_SERVER_CONNECTED", str) == 0)	return DA_GATEWAY_SERVER_CONNECTED;
	if (_wcsicmp(L"DA_GATEWAY_SERVER_DISCONNECTED", str) == 0)	return DA_GATEWAY_SERVER_DISCONNECTED;
	if (_wcsicmp(L"DA_GATEWAY_SERVER_CONGESTION_INFO", str) == 0)	return DA_GATEWAY_SERVER_CONGESTION_INFO;
	if (_wcsicmp(L"DA_ACK_PRE_LOGIN", str) == 0)	return DA_ACK_PRE_LOGIN;
	if (_wcsicmp(L"DA_ACK_LOGIN", str) == 0)	return DA_ACK_LOGIN;
	if (_wcsicmp(L"DA_GATEWAY_SEVER_ADD", str) == 0)	return DA_GATEWAY_SEVER_ADD;
	if (_wcsicmp(L"DA_GATEWAY_SEVER_UPDATE", str) == 0)	return DA_GATEWAY_SEVER_UPDATE;
	return PacketIdMax;
}
#endif
#ifdef _SERVER

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> String
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline const wchar_t* DAPacketIdEnumToString(uint16 value) noexcept
{
	switch (value)
	{
		case DA_PONG: return L"DA_PONG";
		case DA_ACK_REGISTER: return L"DA_ACK_REGISTER";
		case DA_ACK_GATEWAY_SERVER_LIST: return L"DA_ACK_GATEWAY_SERVER_LIST";
		case DA_GATEWAY_SERVER_CONNECTED: return L"DA_GATEWAY_SERVER_CONNECTED";
		case DA_GATEWAY_SERVER_DISCONNECTED: return L"DA_GATEWAY_SERVER_DISCONNECTED";
		case DA_GATEWAY_SERVER_CONGESTION_INFO: return L"DA_GATEWAY_SERVER_CONGESTION_INFO";
		case DA_ACK_PRE_LOGIN: return L"DA_ACK_PRE_LOGIN";
		case DA_ACK_LOGIN: return L"DA_ACK_LOGIN";
		case DA_GATEWAY_SEVER_ADD: return L"DA_GATEWAY_SEVER_ADD";
		case DA_GATEWAY_SEVER_UPDATE: return L"DA_GATEWAY_SEVER_UPDATE";
	}
	return nullptr;
}
#endif
// type = DA
struct PKT_DA_PONG_READ
{
public:
	uint16 mPktSize;
	uint16 mPktId;
	uint64 mPong;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"DA_PONG"; }
	const wchar_t* GetRelatedPacketName() const { return L"AD_PONG"; }
#endif

	uint64 Pong() { return mPong; }


#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{

		return Log::format(
			L"[DA_PONG] Pong: {}"
			, mPong
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_DA_PONG_READ);
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

}; // PKT_DA_PONG_READ

// type = DA
#ifdef _SERVER
struct PKT_DA_PONG_WRITE : public Disposable
#else
struct PKT_DA_PONG_WRITE
#endif
{
	PktWriter mPktWriter;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_DA_PONG_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"DA_PONG"; }
	const wchar_t* GetRelatedPacketName() const { return L"AD_PONG"; }
#endif

#ifndef _SERVER
	PKT_DA_PONG_WRITE(uchar* buf, int bufSize, uint64 pong)
		:
		mPktWriter(buf, bufSize)

	{
		mPktWriter << static_cast<uint16>(PD::DA::DAPacketId::DA_PONG);
		mPktWriter << pong;
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_DA_PONG_WRITE(SendBufferHelper& sendBuffer, uint64 pong)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))

	{
		mPktWriter << (uint16)DA_PONG;
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
		auto readPacket = reinterpret_cast<PKT_DA_PONG_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
#else
#endif
}; // PKT_DA_PONG_WRITE

// type = DA
// related packet = PKT_AD_REQ_REGISTER_WRITE
struct PKT_DA_ACK_REGISTER_READ
{
public:
	uint16 mPktSize;
	uint16 mPktId;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"DA_ACK_REGISTER"; }
	const wchar_t* GetRelatedPacketName() const { return L"AD_REQ_REGISTER"; }
#endif



#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{

		return Log::format(
			L"[DA_ACK_REGISTER] "
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_DA_ACK_REGISTER_READ);
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

}; // PKT_DA_ACK_REGISTER_READ

// type = DA
// related packet = PKT_AD_REQ_REGISTER_READ
#ifdef _SERVER
struct PKT_DA_ACK_REGISTER_WRITE : public Disposable
#else
struct PKT_DA_ACK_REGISTER_WRITE
#endif
{
	PktWriter mPktWriter;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_DA_ACK_REGISTER_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"DA_ACK_REGISTER"; }
	const wchar_t* GetRelatedPacketName() const { return L"AD_REQ_REGISTER"; }
#endif

#ifndef _SERVER
	PKT_DA_ACK_REGISTER_WRITE(uchar* buf, int bufSize)
		:
		mPktWriter(buf, bufSize)

	{
		mPktWriter << static_cast<uint16>(PD::DA::DAPacketId::DA_ACK_REGISTER);
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_DA_ACK_REGISTER_WRITE(SendBufferHelper& sendBuffer)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))

	{
		mPktWriter << (uint16)DA_ACK_REGISTER;
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
		auto readPacket = reinterpret_cast<PKT_DA_ACK_REGISTER_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
#else
#endif
}; // PKT_DA_ACK_REGISTER_WRITE

// type = DA
// related packet = PKT_AD_REQ_GATEWAY_SERVER_LIST_WRITE
struct PKT_DA_ACK_GATEWAY_SERVER_LIST_READ
{
	struct ServerList
	{
	protected:
		uint16 mThisOffset;
		uint16 mNextOffset;
		InternalServerElem mInternalServerElem;

	public:
		const InternalServerElem& ConstRefInternalServerElem() { return mInternalServerElem; }
		InternalServerElem InternalServerElem() { return mInternalServerElem; }

		friend PktIterator<ServerList>;
		friend PKT_DA_ACK_GATEWAY_SERVER_LIST_READ;

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
			return (nullptr != pktStart && 0 < pktSize && 0 < dataSize) ? true : false;
		}

	}; // ServerList

public:
	uint16 mPktSize;
	uint16 mPktId;
	uint16 mServerListCount;
	uint16 mFirstServerListOffset;
	bool mCompleted;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"DA_ACK_GATEWAY_SERVER_LIST"; }
	const wchar_t* GetRelatedPacketName() const { return L"AD_REQ_GATEWAY_SERVER_LIST"; }
#endif

	bool Completed() { return mCompleted; }

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
			L"InternalServerElem: {}", 
			iter->InternalServerElem()
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
			L"[DA_ACK_GATEWAY_SERVER_LIST] Completed: {}, ServerList : {}"
			, mCompleted, str1.c_str()
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_DA_ACK_GATEWAY_SERVER_LIST_READ);
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
		return (nullptr != pktStart && 0 < pktSize && 0 < dataSize) ? true : false;
	}

}; // PKT_DA_ACK_GATEWAY_SERVER_LIST_READ

// type = DA
// related packet = PKT_AD_REQ_GATEWAY_SERVER_LIST_READ
#ifdef _SERVER
struct PKT_DA_ACK_GATEWAY_SERVER_LIST_WRITE : public Disposable
#else
struct PKT_DA_ACK_GATEWAY_SERVER_LIST_WRITE
#endif
{
	struct ServerList
	{
		ServerList(uint16 offset, InternalServerElem InternalServerElem)
			:
			mThisOffset(offset)
			, mNextOffset(0)
			, mInternalServerElem(InternalServerElem)
		{
		}

		uint16 mThisOffset;
		uint16 mNextOffset;
		InternalServerElem mInternalServerElem;

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

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_DA_ACK_GATEWAY_SERVER_LIST_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"DA_ACK_GATEWAY_SERVER_LIST"; }
	const wchar_t* GetRelatedPacketName() const { return L"AD_REQ_GATEWAY_SERVER_LIST"; }
#endif

#ifndef _SERVER
	PKT_DA_ACK_GATEWAY_SERVER_LIST_WRITE(uchar* buf, int bufSize, bool completed)
		:
		mPktWriter(buf, bufSize)
		, mNowServerList(0)
	{
		mPktWriter << static_cast<uint16>(PD::DA::DAPacketId::DA_ACK_GATEWAY_SERVER_LIST);
		mServerListCount = (uint16*)mPktWriter.GetBuffer();
		*mServerListCount = 0;
		mPktWriter << (*mServerListCount);
		mFirstServerListOffset = (uint16*)mPktWriter.GetBuffer();
		*mFirstServerListOffset = 0;
		mPktWriter << (*mFirstServerListOffset);
		mPktWriter << completed;
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_DA_ACK_GATEWAY_SERVER_LIST_WRITE(SendBufferHelper& sendBuffer, bool completed)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))
		, mNowServerList(0)
	{
		mPktWriter << (uint16)DA_ACK_GATEWAY_SERVER_LIST;
		mServerListCount = (uint16*)mPktWriter.GetBuffer();
		*mServerListCount = 0;
		mPktWriter << (*mServerListCount);
		mFirstServerListOffset = (uint16*)mPktWriter.GetBuffer();
		*mFirstServerListOffset = 0;
		mPktWriter << (*mFirstServerListOffset);
		mPktWriter << completed;
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
		auto readPacket = reinterpret_cast<PKT_DA_ACK_GATEWAY_SERVER_LIST_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
	ServerListHandle CreateServerList(const InternalServerElem& InternalServerElem)
	{
		uchar* nowBuffer = mPktWriter.GetBuffer();
		uint16 nowWritten = (uint16)mPktWriter.GetPktSize();
		ServerList* newServerList = new ((ServerList*)nowBuffer) ServerList(nowWritten, InternalServerElem);
		++(*mServerListCount);
		if (mNowServerList == 0)
			*mFirstServerListOffset = nowWritten;
		else
			mNowServerList->mNextOffset = nowWritten;
		mNowServerList = newServerList;
		mPktWriter.Skip(sizeof(ServerList));
		return ServerListHandle(mPktWriter, newServerList);
	}

#else
	ServerListHandle CreateServerList(const InternalServerElem& InternalServerElem)
	{
		uchar* nowBuffer = mPktWriter.GetBuffer();
		uint16 nowWritten = (uint16)mPktWriter.GetPktSize();
		ServerList* newServerList = new ((ServerList*)nowBuffer) ServerList(nowWritten, InternalServerElem);
		++(*mServerListCount);
		if (mNowServerList == 0)
			*mFirstServerListOffset = nowWritten;
		else
			mNowServerList->mNextOffset = nowWritten;
		mNowServerList = newServerList;
		mPktWriter.Skip(sizeof(ServerList));
		return ServerListHandle(mPktWriter, newServerList);
	}

#endif
}; // PKT_DA_ACK_GATEWAY_SERVER_LIST_WRITE

// type = DA
struct PKT_DA_GATEWAY_SERVER_CONNECTED_READ
{
public:
	uint16 mPktSize;
	uint16 mPktId;
	InternalServerElem mServerElem;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"DA_GATEWAY_SERVER_CONNECTED"; }
	const wchar_t* GetRelatedPacketName() const { return L"AD_GATEWAY_SERVER_CONNECTED"; }
#endif

	const InternalServerElem& ConstRefServerElem() { return mServerElem; }
	InternalServerElem ServerElem() { return mServerElem; }


#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{

		return Log::format(
			L"[DA_GATEWAY_SERVER_CONNECTED] ServerElem: {}"
			, mServerElem
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_DA_GATEWAY_SERVER_CONNECTED_READ);
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

}; // PKT_DA_GATEWAY_SERVER_CONNECTED_READ

// type = DA
#ifdef _SERVER
struct PKT_DA_GATEWAY_SERVER_CONNECTED_WRITE : public Disposable
#else
struct PKT_DA_GATEWAY_SERVER_CONNECTED_WRITE
#endif
{
	PktWriter mPktWriter;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_DA_GATEWAY_SERVER_CONNECTED_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"DA_GATEWAY_SERVER_CONNECTED"; }
	const wchar_t* GetRelatedPacketName() const { return L"AD_GATEWAY_SERVER_CONNECTED"; }
#endif

#ifndef _SERVER
	PKT_DA_GATEWAY_SERVER_CONNECTED_WRITE(uchar* buf, int bufSize, InternalServerElem serverElem)
		:
		mPktWriter(buf, bufSize)

	{
		mPktWriter << static_cast<uint16>(PD::DA::DAPacketId::DA_GATEWAY_SERVER_CONNECTED);
		mPktWriter << serverElem;
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_DA_GATEWAY_SERVER_CONNECTED_WRITE(SendBufferHelper& sendBuffer, const InternalServerElem& serverElem)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))

	{
		mPktWriter << (uint16)DA_GATEWAY_SERVER_CONNECTED;
		mPktWriter << serverElem;
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
		auto readPacket = reinterpret_cast<PKT_DA_GATEWAY_SERVER_CONNECTED_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
#else
#endif
}; // PKT_DA_GATEWAY_SERVER_CONNECTED_WRITE

// type = DA
struct PKT_DA_GATEWAY_SERVER_DISCONNECTED_READ
{
public:
	uint16 mPktSize;
	uint16 mPktId;
	uint16 mPlanetWorldId;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"DA_GATEWAY_SERVER_DISCONNECTED"; }
	const wchar_t* GetRelatedPacketName() const { return L"AD_GATEWAY_SERVER_DISCONNECTED"; }
#endif

	uint16 PlanetWorldId() { return mPlanetWorldId; }


#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{

		return Log::format(
			L"[DA_GATEWAY_SERVER_DISCONNECTED] PlanetWorldId: {}"
			, mPlanetWorldId
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_DA_GATEWAY_SERVER_DISCONNECTED_READ);
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

}; // PKT_DA_GATEWAY_SERVER_DISCONNECTED_READ

// type = DA
#ifdef _SERVER
struct PKT_DA_GATEWAY_SERVER_DISCONNECTED_WRITE : public Disposable
#else
struct PKT_DA_GATEWAY_SERVER_DISCONNECTED_WRITE
#endif
{
	PktWriter mPktWriter;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_DA_GATEWAY_SERVER_DISCONNECTED_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"DA_GATEWAY_SERVER_DISCONNECTED"; }
	const wchar_t* GetRelatedPacketName() const { return L"AD_GATEWAY_SERVER_DISCONNECTED"; }
#endif

#ifndef _SERVER
	PKT_DA_GATEWAY_SERVER_DISCONNECTED_WRITE(uchar* buf, int bufSize, uint16 planetWorldId)
		:
		mPktWriter(buf, bufSize)

	{
		mPktWriter << static_cast<uint16>(PD::DA::DAPacketId::DA_GATEWAY_SERVER_DISCONNECTED);
		mPktWriter << planetWorldId;
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_DA_GATEWAY_SERVER_DISCONNECTED_WRITE(SendBufferHelper& sendBuffer, uint16 planetWorldId)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))

	{
		mPktWriter << (uint16)DA_GATEWAY_SERVER_DISCONNECTED;
		mPktWriter << planetWorldId;
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
		auto readPacket = reinterpret_cast<PKT_DA_GATEWAY_SERVER_DISCONNECTED_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
#else
#endif
}; // PKT_DA_GATEWAY_SERVER_DISCONNECTED_WRITE

// type = DA
struct PKT_DA_GATEWAY_SERVER_CONGESTION_INFO_READ
{
public:
	uint16 mPktSize;
	uint16 mPktId;
	uint16 mPlanetWorldId;
	uchar mCongestion;
	uint32 mUserCntCreated;
	uint32 mUserCntPlaying;
	uint16 mBestPlanetWorldId;
	ServerStatus mServerStatus;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"DA_GATEWAY_SERVER_CONGESTION_INFO"; }
	const wchar_t* GetRelatedPacketName() const { return L"AD_GATEWAY_SERVER_CONGESTION_INFO"; }
#endif

	uint16 PlanetWorldId() { return mPlanetWorldId; }
	uchar Congestion() { return mCongestion; }
	uint32 UserCntCreated() { return mUserCntCreated; }
	uint32 UserCntPlaying() { return mUserCntPlaying; }
	uint16 BestPlanetWorldId() { return mBestPlanetWorldId; }
	ServerStatus ServerStatus() { return mServerStatus; }


#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{

		return Log::format(
			L"[DA_GATEWAY_SERVER_CONGESTION_INFO] PlanetWorldId: {}, Congestion: {}, UserCntCreated: {}, UserCntPlaying: {}, BestPlanetWorldId: {}, ServerStatus: {}"
			, mPlanetWorldId, mCongestion, mUserCntCreated, mUserCntPlaying, mBestPlanetWorldId, mServerStatus
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_DA_GATEWAY_SERVER_CONGESTION_INFO_READ);
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

}; // PKT_DA_GATEWAY_SERVER_CONGESTION_INFO_READ

// type = DA
#ifdef _SERVER
struct PKT_DA_GATEWAY_SERVER_CONGESTION_INFO_WRITE : public Disposable
#else
struct PKT_DA_GATEWAY_SERVER_CONGESTION_INFO_WRITE
#endif
{
	PktWriter mPktWriter;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_DA_GATEWAY_SERVER_CONGESTION_INFO_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"DA_GATEWAY_SERVER_CONGESTION_INFO"; }
	const wchar_t* GetRelatedPacketName() const { return L"AD_GATEWAY_SERVER_CONGESTION_INFO"; }
#endif

#ifndef _SERVER
	PKT_DA_GATEWAY_SERVER_CONGESTION_INFO_WRITE(uchar* buf, int bufSize, uint16 planetWorldId, uchar congestion, uint32 userCntCreated, uint32 userCntPlaying, uint16 bestPlanetWorldId, ServerStatus serverStatus)
		:
		mPktWriter(buf, bufSize)

	{
		mPktWriter << static_cast<uint16>(PD::DA::DAPacketId::DA_GATEWAY_SERVER_CONGESTION_INFO);
		mPktWriter << planetWorldId;
		mPktWriter << congestion;
		mPktWriter << userCntCreated;
		mPktWriter << userCntPlaying;
		mPktWriter << bestPlanetWorldId;
		mPktWriter << serverStatus;
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_DA_GATEWAY_SERVER_CONGESTION_INFO_WRITE(SendBufferHelper& sendBuffer, uint16 planetWorldId, uchar congestion, uint32 userCntCreated, uint32 userCntPlaying, uint16 bestPlanetWorldId, ServerStatus serverStatus)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))

	{
		mPktWriter << (uint16)DA_GATEWAY_SERVER_CONGESTION_INFO;
		mPktWriter << planetWorldId;
		mPktWriter << congestion;
		mPktWriter << userCntCreated;
		mPktWriter << userCntPlaying;
		mPktWriter << bestPlanetWorldId;
		mPktWriter << serverStatus;
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
		auto readPacket = reinterpret_cast<PKT_DA_GATEWAY_SERVER_CONGESTION_INFO_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
#else
#endif
}; // PKT_DA_GATEWAY_SERVER_CONGESTION_INFO_WRITE

// type = DA
// related packet = PKT_AD_REQ_PRE_LOGIN_WRITE
struct PKT_DA_ACK_PRE_LOGIN_READ
{
public:
	uint16 mPktSize;
	uint16 mPktId;
	uint16 mPayingUserNameOffset;
	AuthTicket mAuthTicket;
	uint64 mClientSession;
	int mResult;
	bool mCanZPay;
	PlanetWorldId mConectionPlanetWorldId;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"DA_ACK_PRE_LOGIN"; }
	const wchar_t* GetRelatedPacketName() const { return L"AD_REQ_PRE_LOGIN"; }
#endif

	AuthTicket AuthTicket() { return mAuthTicket; }
	uint64 ClientSession() { return mClientSession; }
	int Result() { return mResult; }
	bool CanZPay() { return mCanZPay; }
	const PlanetWorldId& ConstRefConectionPlanetWorldId() { return mConectionPlanetWorldId; }
	PlanetWorldId ConectionPlanetWorldId() { return mConectionPlanetWorldId; }
#ifdef _SERVER
	const wchar_t* PayingUserName() const
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = *((uint16*)pktStart);
		if (mPayingUserNameOffset && pktSize > mPayingUserNameOffset)
			return (const wchar_t*)(pktStart + mPayingUserNameOffset);
		else
			return L"";
	}
#else // _SERVER
	const FString PayingUserName()
	{
		const wchar_t* strWchar = L"";
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = *((uint16*)pktStart);
		if (mPayingUserNameOffset && pktSize > mPayingUserNameOffset)
			strWchar = (const wchar_t*)(pktStart + mPayingUserNameOffset);

		return PktWriter::ConvertToFString(strWchar);
	}
#endif // _SERVER


#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{

		return Log::format(
			L"[DA_ACK_PRE_LOGIN] AuthTicket: {}, ClientSession: {}, Result: {}, CanZPay: {}, PayingUserName: {}, ConectionPlanetWorldId: {}"
			, mAuthTicket, mClientSession, mResult, mCanZPay, PayingUserName(), mConectionPlanetWorldId
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_DA_ACK_PRE_LOGIN_READ);
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
		if (false == ValidateString(mPayingUserNameOffset, 0, pktStart, pktSize, dataSize))
			return false;

		return (nullptr != pktStart && 0 < pktSize && 0 < dataSize) ? true : false;
	}

}; // PKT_DA_ACK_PRE_LOGIN_READ

// type = DA
// related packet = PKT_AD_REQ_PRE_LOGIN_READ
#ifdef _SERVER
struct PKT_DA_ACK_PRE_LOGIN_WRITE : public Disposable
#else
struct PKT_DA_ACK_PRE_LOGIN_WRITE
#endif
{
	PktWriter mPktWriter;
	uint16* mPayingUserNameOffset;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_DA_ACK_PRE_LOGIN_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"DA_ACK_PRE_LOGIN"; }
	const wchar_t* GetRelatedPacketName() const { return L"AD_REQ_PRE_LOGIN"; }
#endif

#ifndef _SERVER
	PKT_DA_ACK_PRE_LOGIN_WRITE(uchar* buf, int bufSize, AuthTicket authTicket, uint64 clientSession, int result, bool canZPay, const FString& payingUserName, PlanetWorldId conectionPlanetWorldId)
		:
		mPktWriter(buf, bufSize)

	{
		mPktWriter << static_cast<uint16>(PD::DA::DAPacketId::DA_ACK_PRE_LOGIN);
		mPayingUserNameOffset = (uint16*)mPktWriter.GetBuffer();
		*mPayingUserNameOffset = 0;
		mPktWriter << (*mPayingUserNameOffset);
		mPktWriter << authTicket;
		mPktWriter << clientSession;
		mPktWriter << result;
		mPktWriter << canZPay;
		mPktWriter << conectionPlanetWorldId;
		*mPayingUserNameOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(payingUserName);
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_DA_ACK_PRE_LOGIN_WRITE(SendBufferHelper& sendBuffer, AuthTicket authTicket, uint64 clientSession, int result, bool canZPay, const wchar* payingUserName, const PlanetWorldId& conectionPlanetWorldId)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))

	{
		mPktWriter << (uint16)DA_ACK_PRE_LOGIN;
		mPayingUserNameOffset = (uint16*)mPktWriter.GetBuffer();
		*mPayingUserNameOffset = 0;
		mPktWriter << (*mPayingUserNameOffset);
		mPktWriter << authTicket;
		mPktWriter << clientSession;
		mPktWriter << result;
		mPktWriter << canZPay;
		mPktWriter << conectionPlanetWorldId;
		*mPayingUserNameOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(payingUserName);
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
		auto readPacket = reinterpret_cast<PKT_DA_ACK_PRE_LOGIN_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
#else
#endif
}; // PKT_DA_ACK_PRE_LOGIN_WRITE

// type = DA
// related packet = PKT_AD_REQ_LOGIN_WRITE
struct PKT_DA_ACK_LOGIN_READ
{
	struct PlanetWorldUserCountList
	{
	protected:
		uint16 mThisOffset;
		uint16 mNextOffset;
		uint16 mPlanetWorldId;
		uint16 mUserCount;

	public:
		uint16 PlanetWorldId() { return mPlanetWorldId; }
		uint16 UserCount() { return mUserCount; }

		friend PktIterator<PlanetWorldUserCountList>;
		friend PKT_DA_ACK_LOGIN_READ;

		bool Validate(uint16 offset, uchar * pktStart, uint16 pktSize, uint16& dataSize)
		{
			if (static_cast<size_t>(pktSize) < static_cast<size_t>(offset) + sizeof(PlanetWorldUserCountList))
				return false;

			PlanetWorldUserCountList * node = (PlanetWorldUserCountList*)(pktStart + offset);

			// 자기 자신의 포인터 검증
			if (offset != node->mThisOffset)
				return false;

			// Value 검증
			uchar* pktCurrentNodeStart = (uchar*)this - mThisOffset;
			uint16 pktCurrentNodeSize = *((uint16*)pktStart);
			dataSize += sizeof(PlanetWorldUserCountList);	// skip
			if (false == ValidateValue(pktCurrentNodeStart, pktCurrentNodeSize, dataSize))
				return false;

			return true;
		}

		bool ValidateValue(const uchar* pktStart, uint16 pktSize, uint16& dataSize)
		{
			return (nullptr != pktStart && 0 < pktSize && 0 < dataSize) ? true : false;
		}

	}; // PlanetWorldUserCountList

public:
	uint16 mPktSize;
	uint16 mPktId;
	uint16 mPlanetWorldUserCountListCount;
	uint16 mFirstPlanetWorldUserCountListOffset;
	AccountDBId mAccountId;
	AuthTicket mAuthTicket;
	uint64 mClientSession;
	int mResult;
	uint16 mLastPlanetWorldId;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"DA_ACK_LOGIN"; }
	const wchar_t* GetRelatedPacketName() const { return L"AD_REQ_LOGIN"; }
#endif

	AccountDBId AccountId() { return mAccountId; }
	AuthTicket AuthTicket() { return mAuthTicket; }
	uint64 ClientSession() { return mClientSession; }
	int Result() { return mResult; }
	uint16 LastPlanetWorldId() { return mLastPlanetWorldId; }

	using PlanetWorldUserCountListIterator = PktIterator<PlanetWorldUserCountList>;
	const PlanetWorldUserCountListIterator GetLastPlanetWorldUserCountListIterator() const
	{
		return PlanetWorldUserCountListIterator(0);
	}
	PlanetWorldUserCountListIterator GetFirstPlanetWorldUserCountListIterator() const
	{
		if (mFirstPlanetWorldUserCountListOffset && mPktSize >= mFirstPlanetWorldUserCountListOffset + sizeof(PlanetWorldUserCountList))
		{
			PlanetWorldUserCountList* planetWorldUserCountList = (PlanetWorldUserCountList*)((uchar*)this + mFirstPlanetWorldUserCountListOffset);
			if (mFirstPlanetWorldUserCountListOffset == planetWorldUserCountList->mThisOffset)
				return PlanetWorldUserCountListIterator(planetWorldUserCountList);
			else
				return PlanetWorldUserCountListIterator(0);
		}
		else
			return PlanetWorldUserCountListIterator(0);
	}
	const int GetPlanetWorldUserCountListCount()
	{
		return mPlanetWorldUserCountListCount;
	}
#ifdef _SERVER
	template<typename TFunc>
	void ForEachPlanetWorldUserCountList(TFunc&& func)
	{
		for (auto it = GetFirstPlanetWorldUserCountListIterator(); it != GetLastPlanetWorldUserCountListIterator(); ++it)
			func(it);
	}
#endif


#ifdef _SERVER 
	std::xwstring FuncCreatePlanetWorldUserCountListStr( [[maybe_unused]] PlanetWorldUserCountListIterator& iter) const
	{
		return Log::format(
			L"PlanetWorldId: {}, UserCount: {}", 
			iter->PlanetWorldId(), iter->UserCount()
		).c_str();
	}

	std::xwstring GetLogWString() const
	{
		std::xwstring str1;
		str1.append(L"\n");
		for (PlanetWorldUserCountListIterator iter = this->GetFirstPlanetWorldUserCountListIterator();
			iter != GetLastPlanetWorldUserCountListIterator(); ++iter)
		{
			str1.append(L"[");
			str1.append(FuncCreatePlanetWorldUserCountListStr(iter).c_str());

			str1.append(L"]\n");
		}

		return Log::format(
			L"[DA_ACK_LOGIN] AccountId: {}, AuthTicket: {}, ClientSession: {}, Result: {}, LastPlanetWorldId: {}, PlanetWorldUserCountList : {}"
			, mAccountId, mAuthTicket, mClientSession, mResult, mLastPlanetWorldId, str1.c_str()
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_DA_ACK_LOGIN_READ);
		if (dataSize > mPktSize)
			return false;

		if (false == ValidateValue(pktStart, pktSize, dataSize))
			return false;

		if (false == PlanetWorldUserCountListValidateListNode(mFirstPlanetWorldUserCountListOffset, pktStart, pktSize, dataSize))
			return false;

		if (dataSize != mPktSize)
			return false;

		return true;
	}

	bool PlanetWorldUserCountListValidateListNode(uint16 offset, uchar * pktStart, uint16 pktSize, uint16& dataSize)
	{		for (PlanetWorldUserCountListIterator iter = this->GetFirstPlanetWorldUserCountListIterator();
			iter != GetLastPlanetWorldUserCountListIterator(); ++iter)
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

}; // PKT_DA_ACK_LOGIN_READ

// type = DA
// related packet = PKT_AD_REQ_LOGIN_READ
#ifdef _SERVER
struct PKT_DA_ACK_LOGIN_WRITE : public Disposable
#else
struct PKT_DA_ACK_LOGIN_WRITE
#endif
{
	struct PlanetWorldUserCountList
	{
		PlanetWorldUserCountList(uint16 offset, uint16 planetWorldId, uint16 userCount)
			:
			mThisOffset(offset)
			, mNextOffset(0)
			, mPlanetWorldId(planetWorldId)
			, mUserCount(userCount)
		{
		}

		uint16 mThisOffset;
		uint16 mNextOffset;
		uint16 mPlanetWorldId;
		uint16 mUserCount;

	}; // PlanetWorldUserCountList

	struct PlanetWorldUserCountListHandle
	{
		PlanetWorldUserCountListHandle(PktWriter& pktWriter, PlanetWorldUserCountList* planetWorldUserCountList)
			:
			mPktWriter(pktWriter)
			, mPlanetWorldUserCountList(planetWorldUserCountList)
		{
		}

		PktWriter& mPktWriter;
		PlanetWorldUserCountList* mPlanetWorldUserCountList;

	}; // PlanetWorldUserCountListHandle

	PktWriter mPktWriter;
	uint16* mPlanetWorldUserCountListCount;
	uint16* mFirstPlanetWorldUserCountListOffset;
	PlanetWorldUserCountList* mNowPlanetWorldUserCountList;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_DA_ACK_LOGIN_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"DA_ACK_LOGIN"; }
	const wchar_t* GetRelatedPacketName() const { return L"AD_REQ_LOGIN"; }
#endif

#ifndef _SERVER
	PKT_DA_ACK_LOGIN_WRITE(uchar* buf, int bufSize, AccountDBId accountId, AuthTicket authTicket, uint64 clientSession, int result, uint16 lastPlanetWorldId)
		:
		mPktWriter(buf, bufSize)
		, mNowPlanetWorldUserCountList(0)
	{
		mPktWriter << static_cast<uint16>(PD::DA::DAPacketId::DA_ACK_LOGIN);
		mPlanetWorldUserCountListCount = (uint16*)mPktWriter.GetBuffer();
		*mPlanetWorldUserCountListCount = 0;
		mPktWriter << (*mPlanetWorldUserCountListCount);
		mFirstPlanetWorldUserCountListOffset = (uint16*)mPktWriter.GetBuffer();
		*mFirstPlanetWorldUserCountListOffset = 0;
		mPktWriter << (*mFirstPlanetWorldUserCountListOffset);
		mPktWriter << accountId;
		mPktWriter << authTicket;
		mPktWriter << clientSession;
		mPktWriter << result;
		mPktWriter << lastPlanetWorldId;
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_DA_ACK_LOGIN_WRITE(SendBufferHelper& sendBuffer, AccountDBId accountId, AuthTicket authTicket, uint64 clientSession, int result, uint16 lastPlanetWorldId)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))
		, mNowPlanetWorldUserCountList(0)
	{
		mPktWriter << (uint16)DA_ACK_LOGIN;
		mPlanetWorldUserCountListCount = (uint16*)mPktWriter.GetBuffer();
		*mPlanetWorldUserCountListCount = 0;
		mPktWriter << (*mPlanetWorldUserCountListCount);
		mFirstPlanetWorldUserCountListOffset = (uint16*)mPktWriter.GetBuffer();
		*mFirstPlanetWorldUserCountListOffset = 0;
		mPktWriter << (*mFirstPlanetWorldUserCountListOffset);
		mPktWriter << accountId;
		mPktWriter << authTicket;
		mPktWriter << clientSession;
		mPktWriter << result;
		mPktWriter << lastPlanetWorldId;
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
		auto readPacket = reinterpret_cast<PKT_DA_ACK_LOGIN_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
	PlanetWorldUserCountListHandle CreatePlanetWorldUserCountList(uint16 planetWorldId, uint16 userCount)
	{
		uchar* nowBuffer = mPktWriter.GetBuffer();
		uint16 nowWritten = (uint16)mPktWriter.GetPktSize();
		PlanetWorldUserCountList* newPlanetWorldUserCountList = new ((PlanetWorldUserCountList*)nowBuffer) PlanetWorldUserCountList(nowWritten, planetWorldId, userCount);
		++(*mPlanetWorldUserCountListCount);
		if (mNowPlanetWorldUserCountList == 0)
			*mFirstPlanetWorldUserCountListOffset = nowWritten;
		else
			mNowPlanetWorldUserCountList->mNextOffset = nowWritten;
		mNowPlanetWorldUserCountList = newPlanetWorldUserCountList;
		mPktWriter.Skip(sizeof(PlanetWorldUserCountList));
		return PlanetWorldUserCountListHandle(mPktWriter, newPlanetWorldUserCountList);
	}

#else
	PlanetWorldUserCountListHandle CreatePlanetWorldUserCountList(uint16 planetWorldId, uint16 userCount)
	{
		uchar* nowBuffer = mPktWriter.GetBuffer();
		uint16 nowWritten = (uint16)mPktWriter.GetPktSize();
		PlanetWorldUserCountList* newPlanetWorldUserCountList = new ((PlanetWorldUserCountList*)nowBuffer) PlanetWorldUserCountList(nowWritten, planetWorldId, userCount);
		++(*mPlanetWorldUserCountListCount);
		if (mNowPlanetWorldUserCountList == 0)
			*mFirstPlanetWorldUserCountListOffset = nowWritten;
		else
			mNowPlanetWorldUserCountList->mNextOffset = nowWritten;
		mNowPlanetWorldUserCountList = newPlanetWorldUserCountList;
		mPktWriter.Skip(sizeof(PlanetWorldUserCountList));
		return PlanetWorldUserCountListHandle(mPktWriter, newPlanetWorldUserCountList);
	}

#endif
}; // PKT_DA_ACK_LOGIN_WRITE

// type = DA
struct PKT_DA_GATEWAY_SEVER_ADD_READ
{
public:
	uint16 mPktSize;
	uint16 mPktId;
	InternalServerElem mInternalServerElem;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"DA_GATEWAY_SEVER_ADD"; }
	const wchar_t* GetRelatedPacketName() const { return L"AD_GATEWAY_SEVER_ADD"; }
#endif

	const InternalServerElem& ConstRefInternalServerElem() { return mInternalServerElem; }
	InternalServerElem InternalServerElem() { return mInternalServerElem; }


#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{

		return Log::format(
			L"[DA_GATEWAY_SEVER_ADD] InternalServerElem: {}"
			, mInternalServerElem
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_DA_GATEWAY_SEVER_ADD_READ);
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

}; // PKT_DA_GATEWAY_SEVER_ADD_READ

// type = DA
#ifdef _SERVER
struct PKT_DA_GATEWAY_SEVER_ADD_WRITE : public Disposable
#else
struct PKT_DA_GATEWAY_SEVER_ADD_WRITE
#endif
{
	PktWriter mPktWriter;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_DA_GATEWAY_SEVER_ADD_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"DA_GATEWAY_SEVER_ADD"; }
	const wchar_t* GetRelatedPacketName() const { return L"AD_GATEWAY_SEVER_ADD"; }
#endif

#ifndef _SERVER
	PKT_DA_GATEWAY_SEVER_ADD_WRITE(uchar* buf, int bufSize, InternalServerElem InternalServerElem)
		:
		mPktWriter(buf, bufSize)

	{
		mPktWriter << static_cast<uint16>(PD::DA::DAPacketId::DA_GATEWAY_SEVER_ADD);
		mPktWriter << InternalServerElem;
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_DA_GATEWAY_SEVER_ADD_WRITE(SendBufferHelper& sendBuffer, const InternalServerElem& InternalServerElem)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))

	{
		mPktWriter << (uint16)DA_GATEWAY_SEVER_ADD;
		mPktWriter << InternalServerElem;
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
		auto readPacket = reinterpret_cast<PKT_DA_GATEWAY_SEVER_ADD_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
#else
#endif
}; // PKT_DA_GATEWAY_SEVER_ADD_WRITE

// type = DA
struct PKT_DA_GATEWAY_SEVER_UPDATE_READ
{
public:
	uint16 mPktSize;
	uint16 mPktId;
	uint16 mServerNameOffset;
	uint16 mPublicIPOffset;
	uint16 mBestPlanetWorldId;
	uint16 mPlanetWorldId;
	ServerCongestion mServerCongestion;
	bool mCharCreatable;
	bool mRecommended;
	bool mNewOpened;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"DA_GATEWAY_SEVER_UPDATE"; }
	const wchar_t* GetRelatedPacketName() const { return L"AD_GATEWAY_SEVER_UPDATE"; }
#endif

	uint16 BestPlanetWorldId() { return mBestPlanetWorldId; }
	uint16 PlanetWorldId() { return mPlanetWorldId; }
	ServerCongestion ServerCongestion() { return mServerCongestion; }
	bool CharCreatable() { return mCharCreatable; }
	bool Recommended() { return mRecommended; }
	bool NewOpened() { return mNewOpened; }
#ifdef _SERVER
	const wchar_t* ServerName() const
	{
		uchar* pktStart = (uchar*)this;
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
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = *((uint16*)pktStart);
		if (mServerNameOffset && pktSize > mServerNameOffset)
			strWchar = (const wchar_t*)(pktStart + mServerNameOffset);

		return PktWriter::ConvertToFString(strWchar);
	}
#endif // _SERVER
#ifdef _SERVER
	const wchar_t* PublicIP() const
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = *((uint16*)pktStart);
		if (mPublicIPOffset && pktSize > mPublicIPOffset)
			return (const wchar_t*)(pktStart + mPublicIPOffset);
		else
			return L"";
	}
#else // _SERVER
	const FString PublicIP()
	{
		const wchar_t* strWchar = L"";
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = *((uint16*)pktStart);
		if (mPublicIPOffset && pktSize > mPublicIPOffset)
			strWchar = (const wchar_t*)(pktStart + mPublicIPOffset);

		return PktWriter::ConvertToFString(strWchar);
	}
#endif // _SERVER


#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{

		return Log::format(
			L"[DA_GATEWAY_SEVER_UPDATE] BestPlanetWorldId: {}, PlanetWorldId: {}, ServerName: {}, PublicIP: {}, ServerCongestion: {}, CharCreatable: {}, Recommended: {}, NewOpened: {}"
			, mBestPlanetWorldId, mPlanetWorldId, ServerName(), PublicIP(), mServerCongestion, mCharCreatable, mRecommended, mNewOpened
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_DA_GATEWAY_SEVER_UPDATE_READ);
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
		if (false == ValidateString(mServerNameOffset, 0, pktStart, pktSize, dataSize))
			return false;

		if (false == ValidateString(mPublicIPOffset, 0, pktStart, pktSize, dataSize))
			return false;

		return (nullptr != pktStart && 0 < pktSize && 0 < dataSize) ? true : false;
	}

}; // PKT_DA_GATEWAY_SEVER_UPDATE_READ

// type = DA
#ifdef _SERVER
struct PKT_DA_GATEWAY_SEVER_UPDATE_WRITE : public Disposable
#else
struct PKT_DA_GATEWAY_SEVER_UPDATE_WRITE
#endif
{
	PktWriter mPktWriter;
	uint16* mServerNameOffset;
	uint16* mPublicIPOffset;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_DA_GATEWAY_SEVER_UPDATE_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"DA_GATEWAY_SEVER_UPDATE"; }
	const wchar_t* GetRelatedPacketName() const { return L"AD_GATEWAY_SEVER_UPDATE"; }
#endif

#ifndef _SERVER
	PKT_DA_GATEWAY_SEVER_UPDATE_WRITE(uchar* buf, int bufSize, uint16 bestPlanetWorldId, uint16 planetWorldId, const FString& serverName, const FString& publicIP, ServerCongestion serverCongestion, bool charCreatable, bool recommended, bool newOpened)
		:
		mPktWriter(buf, bufSize)

	{
		mPktWriter << static_cast<uint16>(PD::DA::DAPacketId::DA_GATEWAY_SEVER_UPDATE);
		mServerNameOffset = (uint16*)mPktWriter.GetBuffer();
		*mServerNameOffset = 0;
		mPktWriter << (*mServerNameOffset);
		mPublicIPOffset = (uint16*)mPktWriter.GetBuffer();
		*mPublicIPOffset = 0;
		mPktWriter << (*mPublicIPOffset);
		mPktWriter << bestPlanetWorldId;
		mPktWriter << planetWorldId;
		mPktWriter << serverCongestion;
		mPktWriter << charCreatable;
		mPktWriter << recommended;
		mPktWriter << newOpened;
		*mServerNameOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(serverName);
		*mPublicIPOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(publicIP);
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_DA_GATEWAY_SEVER_UPDATE_WRITE(SendBufferHelper& sendBuffer, uint16 bestPlanetWorldId, uint16 planetWorldId, const wchar* serverName, const wchar* publicIP, ServerCongestion serverCongestion, bool charCreatable, bool recommended, bool newOpened)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))

	{
		mPktWriter << (uint16)DA_GATEWAY_SEVER_UPDATE;
		mServerNameOffset = (uint16*)mPktWriter.GetBuffer();
		*mServerNameOffset = 0;
		mPktWriter << (*mServerNameOffset);
		mPublicIPOffset = (uint16*)mPktWriter.GetBuffer();
		*mPublicIPOffset = 0;
		mPktWriter << (*mPublicIPOffset);
		mPktWriter << bestPlanetWorldId;
		mPktWriter << planetWorldId;
		mPktWriter << serverCongestion;
		mPktWriter << charCreatable;
		mPktWriter << recommended;
		mPktWriter << newOpened;
		*mServerNameOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(serverName);
		*mPublicIPOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(publicIP);
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
		auto readPacket = reinterpret_cast<PKT_DA_GATEWAY_SEVER_UPDATE_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
#else
#endif
}; // PKT_DA_GATEWAY_SEVER_UPDATE_WRITE

#pragma pack(pop)
#pragma warning(pop)
} // namespace DA
} // namespace PD
