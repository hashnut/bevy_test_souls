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
namespace AD
{

#pragma warning(push)
#pragma warning(disable:4315)
#pragma warning(disable:5038)
#pragma pack(push, 1)

#ifndef _SERVER
enum class ADPacketId : uint16
#else
enum
#endif
{
	AD_PING = 5000,
	AD_REQ_REGISTER = 5001,
	AD_REQ_GATEWAY_SERVER_LIST = 5002,
	AD_REQ_PRE_LOGIN = 5003,
	AD_REQ_LOGIN = 5004,
	AD_NOTIFY_ACCOUNT_NAME_UPDATED = 5005,
	PacketIdMax  = 5999
};

#ifdef _SERVER

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// String -> Enum
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline uint16 ADPacketIdStringToEnum(const wchar_t* str) noexcept
{
	if (_wcsicmp(L"AD_PING", str) == 0)	return AD_PING;
	if (_wcsicmp(L"AD_REQ_REGISTER", str) == 0)	return AD_REQ_REGISTER;
	if (_wcsicmp(L"AD_REQ_GATEWAY_SERVER_LIST", str) == 0)	return AD_REQ_GATEWAY_SERVER_LIST;
	if (_wcsicmp(L"AD_REQ_PRE_LOGIN", str) == 0)	return AD_REQ_PRE_LOGIN;
	if (_wcsicmp(L"AD_REQ_LOGIN", str) == 0)	return AD_REQ_LOGIN;
	if (_wcsicmp(L"AD_NOTIFY_ACCOUNT_NAME_UPDATED", str) == 0)	return AD_NOTIFY_ACCOUNT_NAME_UPDATED;
	return PacketIdMax;
}
#endif
#ifdef _SERVER

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> String
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline const wchar_t* ADPacketIdEnumToString(uint16 value) noexcept
{
	switch (value)
	{
		case AD_PING: return L"AD_PING";
		case AD_REQ_REGISTER: return L"AD_REQ_REGISTER";
		case AD_REQ_GATEWAY_SERVER_LIST: return L"AD_REQ_GATEWAY_SERVER_LIST";
		case AD_REQ_PRE_LOGIN: return L"AD_REQ_PRE_LOGIN";
		case AD_REQ_LOGIN: return L"AD_REQ_LOGIN";
		case AD_NOTIFY_ACCOUNT_NAME_UPDATED: return L"AD_NOTIFY_ACCOUNT_NAME_UPDATED";
	}
	return nullptr;
}
#endif
// type = AD
struct PKT_AD_PING_READ
{
public:
	uint16 mPktSize;
	uint16 mPktId;
	uint64 mPing;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"AD_PING"; }
	const wchar_t* GetRelatedPacketName() const { return L"DA_PING"; }
#endif

	uint64 Ping() { return mPing; }


#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{

		return Log::format(
			L"[AD_PING] Ping: {}"
			, mPing
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_AD_PING_READ);
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

}; // PKT_AD_PING_READ

// type = AD
#ifdef _SERVER
struct PKT_AD_PING_WRITE : public Disposable
#else
struct PKT_AD_PING_WRITE
#endif
{
	PktWriter mPktWriter;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_AD_PING_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"AD_PING"; }
	const wchar_t* GetRelatedPacketName() const { return L"DA_PING"; }
#endif

#ifndef _SERVER
	PKT_AD_PING_WRITE(uchar* buf, int bufSize, uint64 ping)
		:
		mPktWriter(buf, bufSize)

	{
		mPktWriter << static_cast<uint16>(PD::AD::ADPacketId::AD_PING);
		mPktWriter << ping;
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_AD_PING_WRITE(SendBufferHelper& sendBuffer, uint64 ping)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))

	{
		mPktWriter << (uint16)AD_PING;
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
		auto readPacket = reinterpret_cast<PKT_AD_PING_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
#else
#endif
}; // PKT_AD_PING_WRITE

// type = AD
// related packet = PKT_DA_ACK_REGISTER_WRITE
struct PKT_AD_REQ_REGISTER_READ
{
public:
	uint16 mPktSize;
	uint16 mPktId;
	uint16 mIpOffset;
	uchar mAuthServerId;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"AD_REQ_REGISTER"; }
	const wchar_t* GetRelatedPacketName() const { return L"DA_ACK_REGISTER"; }
#endif

	uchar AuthServerId() { return mAuthServerId; }
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
	std::xwstring GetLogWString() const
	{

		return Log::format(
			L"[AD_REQ_REGISTER] AuthServerId: {}, Ip: {}"
			, mAuthServerId, Ip()
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_AD_REQ_REGISTER_READ);
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
		if (false == ValidateString(mIpOffset, 0, pktStart, pktSize, dataSize))
			return false;

		return (nullptr != pktStart && 0 < pktSize && 0 < dataSize) ? true : false;
	}

}; // PKT_AD_REQ_REGISTER_READ

// type = AD
// related packet = PKT_DA_ACK_REGISTER_READ
#ifdef _SERVER
struct PKT_AD_REQ_REGISTER_WRITE : public Disposable
#else
struct PKT_AD_REQ_REGISTER_WRITE
#endif
{
	PktWriter mPktWriter;
	uint16* mIpOffset;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_AD_REQ_REGISTER_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"AD_REQ_REGISTER"; }
	const wchar_t* GetRelatedPacketName() const { return L"DA_ACK_REGISTER"; }
#endif

#ifndef _SERVER
	PKT_AD_REQ_REGISTER_WRITE(uchar* buf, int bufSize, uchar authServerId, const FString& ip)
		:
		mPktWriter(buf, bufSize)

	{
		mPktWriter << static_cast<uint16>(PD::AD::ADPacketId::AD_REQ_REGISTER);
		mIpOffset = (uint16*)mPktWriter.GetBuffer();
		*mIpOffset = 0;
		mPktWriter << (*mIpOffset);
		mPktWriter << authServerId;
		*mIpOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(ip);
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_AD_REQ_REGISTER_WRITE(SendBufferHelper& sendBuffer, uchar authServerId, const wchar* ip)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))

	{
		mPktWriter << (uint16)AD_REQ_REGISTER;
		mIpOffset = (uint16*)mPktWriter.GetBuffer();
		*mIpOffset = 0;
		mPktWriter << (*mIpOffset);
		mPktWriter << authServerId;
		*mIpOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(ip);
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
		auto readPacket = reinterpret_cast<PKT_AD_REQ_REGISTER_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
#else
#endif
}; // PKT_AD_REQ_REGISTER_WRITE

// type = AD
// related packet = PKT_DA_ACK_GATEWAY_SERVER_LIST_WRITE
struct PKT_AD_REQ_GATEWAY_SERVER_LIST_READ
{
public:
	uint16 mPktSize;
	uint16 mPktId;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"AD_REQ_GATEWAY_SERVER_LIST"; }
	const wchar_t* GetRelatedPacketName() const { return L"DA_ACK_GATEWAY_SERVER_LIST"; }
#endif



#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{

		return Log::format(
			L"[AD_REQ_GATEWAY_SERVER_LIST] "
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_AD_REQ_GATEWAY_SERVER_LIST_READ);
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

}; // PKT_AD_REQ_GATEWAY_SERVER_LIST_READ

// type = AD
// related packet = PKT_DA_ACK_GATEWAY_SERVER_LIST_READ
#ifdef _SERVER
struct PKT_AD_REQ_GATEWAY_SERVER_LIST_WRITE : public Disposable
#else
struct PKT_AD_REQ_GATEWAY_SERVER_LIST_WRITE
#endif
{
	PktWriter mPktWriter;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_AD_REQ_GATEWAY_SERVER_LIST_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"AD_REQ_GATEWAY_SERVER_LIST"; }
	const wchar_t* GetRelatedPacketName() const { return L"DA_ACK_GATEWAY_SERVER_LIST"; }
#endif

#ifndef _SERVER
	PKT_AD_REQ_GATEWAY_SERVER_LIST_WRITE(uchar* buf, int bufSize)
		:
		mPktWriter(buf, bufSize)

	{
		mPktWriter << static_cast<uint16>(PD::AD::ADPacketId::AD_REQ_GATEWAY_SERVER_LIST);
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_AD_REQ_GATEWAY_SERVER_LIST_WRITE(SendBufferHelper& sendBuffer)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))

	{
		mPktWriter << (uint16)AD_REQ_GATEWAY_SERVER_LIST;
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
		auto readPacket = reinterpret_cast<PKT_AD_REQ_GATEWAY_SERVER_LIST_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
#else
#endif
}; // PKT_AD_REQ_GATEWAY_SERVER_LIST_WRITE

// type = AD
// related packet = PKT_DA_ACK_PRE_LOGIN_WRITE
struct PKT_AD_REQ_PRE_LOGIN_READ
{
public:
	uint16 mPktSize;
	uint16 mPktId;
	AccountDBId mAccountId;
	AuthTicket mAuthTicket;
	uint64 mClientSession;
	DevicePlatformType mDevicePlatformType;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"AD_REQ_PRE_LOGIN"; }
	const wchar_t* GetRelatedPacketName() const { return L"DA_ACK_PRE_LOGIN"; }
#endif

	AccountDBId AccountId() { return mAccountId; }
	AuthTicket AuthTicket() { return mAuthTicket; }
	uint64 ClientSession() { return mClientSession; }
	DevicePlatformType DevicePlatformType() { return mDevicePlatformType; }


#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{

		return Log::format(
			L"[AD_REQ_PRE_LOGIN] AccountId: {}, AuthTicket: {}, ClientSession: {}, DevicePlatformType: {}"
			, mAccountId, mAuthTicket, mClientSession, mDevicePlatformType
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_AD_REQ_PRE_LOGIN_READ);
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

}; // PKT_AD_REQ_PRE_LOGIN_READ

// type = AD
// related packet = PKT_DA_ACK_PRE_LOGIN_READ
#ifdef _SERVER
struct PKT_AD_REQ_PRE_LOGIN_WRITE : public Disposable
#else
struct PKT_AD_REQ_PRE_LOGIN_WRITE
#endif
{
	PktWriter mPktWriter;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_AD_REQ_PRE_LOGIN_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"AD_REQ_PRE_LOGIN"; }
	const wchar_t* GetRelatedPacketName() const { return L"DA_ACK_PRE_LOGIN"; }
#endif

#ifndef _SERVER
	PKT_AD_REQ_PRE_LOGIN_WRITE(uchar* buf, int bufSize, AccountDBId accountId, AuthTicket authTicket, uint64 clientSession, DevicePlatformType devicePlatformType)
		:
		mPktWriter(buf, bufSize)

	{
		mPktWriter << static_cast<uint16>(PD::AD::ADPacketId::AD_REQ_PRE_LOGIN);
		mPktWriter << accountId;
		mPktWriter << authTicket;
		mPktWriter << clientSession;
		mPktWriter << devicePlatformType;
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_AD_REQ_PRE_LOGIN_WRITE(SendBufferHelper& sendBuffer, AccountDBId accountId, AuthTicket authTicket, uint64 clientSession, DevicePlatformType devicePlatformType)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))

	{
		mPktWriter << (uint16)AD_REQ_PRE_LOGIN;
		mPktWriter << accountId;
		mPktWriter << authTicket;
		mPktWriter << clientSession;
		mPktWriter << devicePlatformType;
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
		auto readPacket = reinterpret_cast<PKT_AD_REQ_PRE_LOGIN_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
#else
#endif
}; // PKT_AD_REQ_PRE_LOGIN_WRITE

// type = AD
// related packet = PKT_DA_ACK_LOGIN_WRITE
struct PKT_AD_REQ_LOGIN_READ
{
public:
	uint16 mPktSize;
	uint16 mPktId;
	Vid mVid;
	AccountDBId mAccountId;
	AccountGrade mAccountGrade;
	AuthTicket mAuthTicket;
	uint64 mClientSession;
	DevicePlatformType mDevicePlatformType;
	bool mUseZPay;
	bool mRegisterDevice;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"AD_REQ_LOGIN"; }
	const wchar_t* GetRelatedPacketName() const { return L"DA_ACK_LOGIN"; }
#endif

	Vid Vid() { return mVid; }
	AccountDBId AccountId() { return mAccountId; }
	AccountGrade AccountGrade() { return mAccountGrade; }
	AuthTicket AuthTicket() { return mAuthTicket; }
	uint64 ClientSession() { return mClientSession; }
	DevicePlatformType DevicePlatformType() { return mDevicePlatformType; }
	bool UseZPay() { return mUseZPay; }
	bool RegisterDevice() { return mRegisterDevice; }


#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{

		return Log::format(
			L"[AD_REQ_LOGIN] Vid: {}, AccountId: {}, AccountGrade: {}, AuthTicket: {}, ClientSession: {}, DevicePlatformType: {}, UseZPay: {}, RegisterDevice: {}"
			, mVid, mAccountId, mAccountGrade, mAuthTicket, mClientSession, mDevicePlatformType, mUseZPay, mRegisterDevice
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_AD_REQ_LOGIN_READ);
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

}; // PKT_AD_REQ_LOGIN_READ

// type = AD
// related packet = PKT_DA_ACK_LOGIN_READ
#ifdef _SERVER
struct PKT_AD_REQ_LOGIN_WRITE : public Disposable
#else
struct PKT_AD_REQ_LOGIN_WRITE
#endif
{
	PktWriter mPktWriter;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_AD_REQ_LOGIN_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"AD_REQ_LOGIN"; }
	const wchar_t* GetRelatedPacketName() const { return L"DA_ACK_LOGIN"; }
#endif

#ifndef _SERVER
	PKT_AD_REQ_LOGIN_WRITE(uchar* buf, int bufSize, Vid vid, AccountDBId accountId, AccountGrade accountGrade, AuthTicket authTicket, uint64 clientSession, DevicePlatformType devicePlatformType, bool useZPay, bool registerDevice)
		:
		mPktWriter(buf, bufSize)

	{
		mPktWriter << static_cast<uint16>(PD::AD::ADPacketId::AD_REQ_LOGIN);
		mPktWriter << vid;
		mPktWriter << accountId;
		mPktWriter << accountGrade;
		mPktWriter << authTicket;
		mPktWriter << clientSession;
		mPktWriter << devicePlatformType;
		mPktWriter << useZPay;
		mPktWriter << registerDevice;
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_AD_REQ_LOGIN_WRITE(SendBufferHelper& sendBuffer, Vid vid, AccountDBId accountId, AccountGrade accountGrade, AuthTicket authTicket, uint64 clientSession, DevicePlatformType devicePlatformType, bool useZPay, bool registerDevice)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))

	{
		mPktWriter << (uint16)AD_REQ_LOGIN;
		mPktWriter << vid;
		mPktWriter << accountId;
		mPktWriter << accountGrade;
		mPktWriter << authTicket;
		mPktWriter << clientSession;
		mPktWriter << devicePlatformType;
		mPktWriter << useZPay;
		mPktWriter << registerDevice;
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
		auto readPacket = reinterpret_cast<PKT_AD_REQ_LOGIN_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
#else
#endif
}; // PKT_AD_REQ_LOGIN_WRITE

// type = AD
struct PKT_AD_NOTIFY_ACCOUNT_NAME_UPDATED_READ
{
public:
	uint16 mPktSize;
	uint16 mPktId;
	uint16 mAccountNameOffset;
	AccountDBId mAccountId;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"AD_NOTIFY_ACCOUNT_NAME_UPDATED"; }
	const wchar_t* GetRelatedPacketName() const { return L"DA_NOTIFY_ACCOUNT_NAME_UPDATED"; }
#endif

	AccountDBId AccountId() { return mAccountId; }
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
	std::xwstring GetLogWString() const
	{

		return Log::format(
			L"[AD_NOTIFY_ACCOUNT_NAME_UPDATED] AccountId: {}, AccountName: {}"
			, mAccountId, AccountName()
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_AD_NOTIFY_ACCOUNT_NAME_UPDATED_READ);
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
		if (false == ValidateString(mAccountNameOffset, 0, pktStart, pktSize, dataSize))
			return false;

		return (nullptr != pktStart && 0 < pktSize && 0 < dataSize) ? true : false;
	}

}; // PKT_AD_NOTIFY_ACCOUNT_NAME_UPDATED_READ

// type = AD
#ifdef _SERVER
struct PKT_AD_NOTIFY_ACCOUNT_NAME_UPDATED_WRITE : public Disposable
#else
struct PKT_AD_NOTIFY_ACCOUNT_NAME_UPDATED_WRITE
#endif
{
	PktWriter mPktWriter;
	uint16* mAccountNameOffset;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_AD_NOTIFY_ACCOUNT_NAME_UPDATED_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"AD_NOTIFY_ACCOUNT_NAME_UPDATED"; }
	const wchar_t* GetRelatedPacketName() const { return L"DA_NOTIFY_ACCOUNT_NAME_UPDATED"; }
#endif

#ifndef _SERVER
	PKT_AD_NOTIFY_ACCOUNT_NAME_UPDATED_WRITE(uchar* buf, int bufSize, AccountDBId accountId, const FString& accountName)
		:
		mPktWriter(buf, bufSize)

	{
		mPktWriter << static_cast<uint16>(PD::AD::ADPacketId::AD_NOTIFY_ACCOUNT_NAME_UPDATED);
		mAccountNameOffset = (uint16*)mPktWriter.GetBuffer();
		*mAccountNameOffset = 0;
		mPktWriter << (*mAccountNameOffset);
		mPktWriter << accountId;
		*mAccountNameOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(accountName);
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_AD_NOTIFY_ACCOUNT_NAME_UPDATED_WRITE(SendBufferHelper& sendBuffer, AccountDBId accountId, const wchar* accountName)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))

	{
		mPktWriter << (uint16)AD_NOTIFY_ACCOUNT_NAME_UPDATED;
		mAccountNameOffset = (uint16*)mPktWriter.GetBuffer();
		*mAccountNameOffset = 0;
		mPktWriter << (*mAccountNameOffset);
		mPktWriter << accountId;
		*mAccountNameOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(accountName);
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
		auto readPacket = reinterpret_cast<PKT_AD_NOTIFY_ACCOUNT_NAME_UPDATED_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
#else
#endif
}; // PKT_AD_NOTIFY_ACCOUNT_NAME_UPDATED_WRITE

#pragma pack(pop)
#pragma warning(pop)
} // namespace AD
} // namespace PD
