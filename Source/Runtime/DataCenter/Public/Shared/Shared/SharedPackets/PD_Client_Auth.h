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
namespace CA
{

#pragma warning(push)
#pragma warning(disable:4315)
#pragma warning(disable:5038)
#pragma pack(push, 1)

#ifndef _SERVER
enum class CAPacketId : uint16
#else
enum
#endif
{
	CA_PING = 3876,
	CA_REQ_LOGIN = 3826,
	CA_REQ_SELECT_SERVER = 3892,
	CA_REQ_CHECK_PASSWORD = 3670,
	CA_REQ_CHECK_VERSION = 3293,
	CA_REQ_APP_GUARD_AUTH = 3460,
	CA_REQ_DELETE_ACCOUNT = 3224,
	CA_REQ_PRE_LOGIN = 3618,
	CA_REQ_IDP_CONFLICT_PLYER = 3748,
	PacketIdMax  = 3999
};

#ifdef _SERVER

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// String -> Enum
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline uint16 CAPacketIdStringToEnum(const wchar_t* str) noexcept
{
	if (_wcsicmp(L"CA_PING", str) == 0)	return CA_PING;
	if (_wcsicmp(L"CA_REQ_LOGIN", str) == 0)	return CA_REQ_LOGIN;
	if (_wcsicmp(L"CA_REQ_SELECT_SERVER", str) == 0)	return CA_REQ_SELECT_SERVER;
	if (_wcsicmp(L"CA_REQ_CHECK_PASSWORD", str) == 0)	return CA_REQ_CHECK_PASSWORD;
	if (_wcsicmp(L"CA_REQ_CHECK_VERSION", str) == 0)	return CA_REQ_CHECK_VERSION;
	if (_wcsicmp(L"CA_REQ_APP_GUARD_AUTH", str) == 0)	return CA_REQ_APP_GUARD_AUTH;
	if (_wcsicmp(L"CA_REQ_DELETE_ACCOUNT", str) == 0)	return CA_REQ_DELETE_ACCOUNT;
	if (_wcsicmp(L"CA_REQ_PRE_LOGIN", str) == 0)	return CA_REQ_PRE_LOGIN;
	if (_wcsicmp(L"CA_REQ_IDP_CONFLICT_PLYER", str) == 0)	return CA_REQ_IDP_CONFLICT_PLYER;
	return PacketIdMax;
}
#endif
#ifdef _SERVER

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> String
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline const wchar_t* CAPacketIdEnumToString(uint16 value) noexcept
{
	switch (value)
	{
		case CA_PING: return L"CA_PING";
		case CA_REQ_LOGIN: return L"CA_REQ_LOGIN";
		case CA_REQ_SELECT_SERVER: return L"CA_REQ_SELECT_SERVER";
		case CA_REQ_CHECK_PASSWORD: return L"CA_REQ_CHECK_PASSWORD";
		case CA_REQ_CHECK_VERSION: return L"CA_REQ_CHECK_VERSION";
		case CA_REQ_APP_GUARD_AUTH: return L"CA_REQ_APP_GUARD_AUTH";
		case CA_REQ_DELETE_ACCOUNT: return L"CA_REQ_DELETE_ACCOUNT";
		case CA_REQ_PRE_LOGIN: return L"CA_REQ_PRE_LOGIN";
		case CA_REQ_IDP_CONFLICT_PLYER: return L"CA_REQ_IDP_CONFLICT_PLYER";
	}
	return nullptr;
}
#endif
// type = CA
struct PKT_CA_PING_READ
{
public:
	uint16 mPktSize;
	uint16 mPktId;
	int16 mPing;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"CA_PING"; }
	const wchar_t* GetRelatedPacketName() const { return L"AC_PING"; }
#endif

	int16 Ping() { return mPing; }


#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{

		return Log::format(
			L"[CA_PING] Ping: {}"
			, mPing
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_CA_PING_READ);
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

}; // PKT_CA_PING_READ

// type = CA
#ifdef _SERVER
struct PKT_CA_PING_WRITE : public Disposable
#else
struct PKT_CA_PING_WRITE
#endif
{
	PktWriter mPktWriter;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_CA_PING_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"CA_PING"; }
	const wchar_t* GetRelatedPacketName() const { return L"AC_PING"; }
#endif

#ifndef _SERVER
	PKT_CA_PING_WRITE(uchar* buf, int bufSize, int16 ping)
		:
		mPktWriter(buf, bufSize)

	{
		mPktWriter << static_cast<uint16>(PD::CA::CAPacketId::CA_PING);
		mPktWriter << ping;
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_CA_PING_WRITE(SendBufferHelper& sendBuffer, int16 ping)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))

	{
		mPktWriter << (uint16)CA_PING;
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
		auto readPacket = reinterpret_cast<PKT_CA_PING_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
#else
#endif
}; // PKT_CA_PING_WRITE

// type = CA
// related packet = PKT_AC_ACK_LOGIN_WRITE
struct PKT_CA_REQ_LOGIN_READ
{
public:
	uint16 mPktSize;
	uint16 mPktId;
	bool mUseZPay;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"CA_REQ_LOGIN"; }
	const wchar_t* GetRelatedPacketName() const { return L"AC_ACK_LOGIN"; }
#endif

	bool UseZPay() { return mUseZPay; }


#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{

		return Log::format(
			L"[CA_REQ_LOGIN] UseZPay: {}"
			, mUseZPay
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_CA_REQ_LOGIN_READ);
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

}; // PKT_CA_REQ_LOGIN_READ

// type = CA
// related packet = PKT_AC_ACK_LOGIN_READ
#ifdef _SERVER
struct PKT_CA_REQ_LOGIN_WRITE : public Disposable
#else
struct PKT_CA_REQ_LOGIN_WRITE
#endif
{
	PktWriter mPktWriter;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_CA_REQ_LOGIN_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"CA_REQ_LOGIN"; }
	const wchar_t* GetRelatedPacketName() const { return L"AC_ACK_LOGIN"; }
#endif

#ifndef _SERVER
	PKT_CA_REQ_LOGIN_WRITE(uchar* buf, int bufSize, bool useZPay)
		:
		mPktWriter(buf, bufSize)

	{
		mPktWriter << static_cast<uint16>(PD::CA::CAPacketId::CA_REQ_LOGIN);
		mPktWriter << useZPay;
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_CA_REQ_LOGIN_WRITE(SendBufferHelper& sendBuffer, bool useZPay)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))

	{
		mPktWriter << (uint16)CA_REQ_LOGIN;
		mPktWriter << useZPay;
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
		auto readPacket = reinterpret_cast<PKT_CA_REQ_LOGIN_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
#else
#endif
}; // PKT_CA_REQ_LOGIN_WRITE

// type = CA
// related packet = PKT_AC_ACK_SELECT_SERVER_WRITE
struct PKT_CA_REQ_SELECT_SERVER_READ
{
public:
	uint16 mPktSize;
	uint16 mPktId;
	uint16 mPlanetWorldId;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"CA_REQ_SELECT_SERVER"; }
	const wchar_t* GetRelatedPacketName() const { return L"AC_ACK_SELECT_SERVER"; }
#endif

	uint16 PlanetWorldId() { return mPlanetWorldId; }


#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{

		return Log::format(
			L"[CA_REQ_SELECT_SERVER] PlanetWorldId: {}"
			, mPlanetWorldId
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_CA_REQ_SELECT_SERVER_READ);
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

}; // PKT_CA_REQ_SELECT_SERVER_READ

// type = CA
// related packet = PKT_AC_ACK_SELECT_SERVER_READ
#ifdef _SERVER
struct PKT_CA_REQ_SELECT_SERVER_WRITE : public Disposable
#else
struct PKT_CA_REQ_SELECT_SERVER_WRITE
#endif
{
	PktWriter mPktWriter;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_CA_REQ_SELECT_SERVER_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"CA_REQ_SELECT_SERVER"; }
	const wchar_t* GetRelatedPacketName() const { return L"AC_ACK_SELECT_SERVER"; }
#endif

#ifndef _SERVER
	PKT_CA_REQ_SELECT_SERVER_WRITE(uchar* buf, int bufSize, uint16 planetWorldId)
		:
		mPktWriter(buf, bufSize)

	{
		mPktWriter << static_cast<uint16>(PD::CA::CAPacketId::CA_REQ_SELECT_SERVER);
		mPktWriter << planetWorldId;
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_CA_REQ_SELECT_SERVER_WRITE(SendBufferHelper& sendBuffer, uint16 planetWorldId)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))

	{
		mPktWriter << (uint16)CA_REQ_SELECT_SERVER;
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
		auto readPacket = reinterpret_cast<PKT_CA_REQ_SELECT_SERVER_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
#else
#endif
}; // PKT_CA_REQ_SELECT_SERVER_WRITE

// type = CA
// related packet = PKT_AC_ACK_CHECK_PASSWORD_WRITE
struct PKT_CA_REQ_CHECK_PASSWORD_READ
{
public:
	uint16 mPktSize;
	uint16 mPktId;
	uint16 mIdOffset;
	uint16 mPasswordOffset;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"CA_REQ_CHECK_PASSWORD"; }
	const wchar_t* GetRelatedPacketName() const { return L"AC_ACK_CHECK_PASSWORD"; }
#endif

#ifdef _SERVER
	const wchar_t* Id() const
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = *((uint16*)pktStart);
		if (mIdOffset && pktSize > mIdOffset)
			return (const wchar_t*)(pktStart + mIdOffset);
		else
			return L"";
	}
#else // _SERVER
	const FString Id()
	{
		const wchar_t* strWchar = L"";
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = *((uint16*)pktStart);
		if (mIdOffset && pktSize > mIdOffset)
			strWchar = (const wchar_t*)(pktStart + mIdOffset);

		return PktWriter::ConvertToFString(strWchar);
	}
#endif // _SERVER
#ifdef _SERVER
	const wchar_t* Password() const
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = *((uint16*)pktStart);
		if (mPasswordOffset && pktSize > mPasswordOffset)
			return (const wchar_t*)(pktStart + mPasswordOffset);
		else
			return L"";
	}
#else // _SERVER
	const FString Password()
	{
		const wchar_t* strWchar = L"";
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = *((uint16*)pktStart);
		if (mPasswordOffset && pktSize > mPasswordOffset)
			strWchar = (const wchar_t*)(pktStart + mPasswordOffset);

		return PktWriter::ConvertToFString(strWchar);
	}
#endif // _SERVER


#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{

		return Log::format(
			L"[CA_REQ_CHECK_PASSWORD] Id: {}, Password: {}"
			, Id(), Password()
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_CA_REQ_CHECK_PASSWORD_READ);
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
		if (false == ValidateString(mIdOffset, 0, pktStart, pktSize, dataSize))
			return false;

		if (false == ValidateString(mPasswordOffset, 0, pktStart, pktSize, dataSize))
			return false;

		return (nullptr != pktStart && 0 < pktSize && 0 < dataSize) ? true : false;
	}

}; // PKT_CA_REQ_CHECK_PASSWORD_READ

// type = CA
// related packet = PKT_AC_ACK_CHECK_PASSWORD_READ
#ifdef _SERVER
struct PKT_CA_REQ_CHECK_PASSWORD_WRITE : public Disposable
#else
struct PKT_CA_REQ_CHECK_PASSWORD_WRITE
#endif
{
	PktWriter mPktWriter;
	uint16* mIdOffset;
	uint16* mPasswordOffset;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_CA_REQ_CHECK_PASSWORD_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"CA_REQ_CHECK_PASSWORD"; }
	const wchar_t* GetRelatedPacketName() const { return L"AC_ACK_CHECK_PASSWORD"; }
#endif

#ifndef _SERVER
	PKT_CA_REQ_CHECK_PASSWORD_WRITE(uchar* buf, int bufSize, const FString& id, const FString& password)
		:
		mPktWriter(buf, bufSize)

	{
		mPktWriter << static_cast<uint16>(PD::CA::CAPacketId::CA_REQ_CHECK_PASSWORD);
		mIdOffset = (uint16*)mPktWriter.GetBuffer();
		*mIdOffset = 0;
		mPktWriter << (*mIdOffset);
		mPasswordOffset = (uint16*)mPktWriter.GetBuffer();
		*mPasswordOffset = 0;
		mPktWriter << (*mPasswordOffset);
		*mIdOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(id);
		*mPasswordOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(password);
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_CA_REQ_CHECK_PASSWORD_WRITE(SendBufferHelper& sendBuffer, const wchar* id, const wchar* password)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))

	{
		mPktWriter << (uint16)CA_REQ_CHECK_PASSWORD;
		mIdOffset = (uint16*)mPktWriter.GetBuffer();
		*mIdOffset = 0;
		mPktWriter << (*mIdOffset);
		mPasswordOffset = (uint16*)mPktWriter.GetBuffer();
		*mPasswordOffset = 0;
		mPktWriter << (*mPasswordOffset);
		*mIdOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(id);
		*mPasswordOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(password);
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
		auto readPacket = reinterpret_cast<PKT_CA_REQ_CHECK_PASSWORD_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
#else
#endif
}; // PKT_CA_REQ_CHECK_PASSWORD_WRITE

// type = CA
// related packet = PKT_AC_ACK_CHECK_VERSION_WRITE
struct PKT_CA_REQ_CHECK_VERSION_READ
{
public:
	uint16 mPktSize;
	uint16 mPktId;
	uint64 mPdlVersion;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"CA_REQ_CHECK_VERSION"; }
	const wchar_t* GetRelatedPacketName() const { return L"AC_ACK_CHECK_VERSION"; }
#endif

	uint64 PdlVersion() { return mPdlVersion; }


#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{

		return Log::format(
			L"[CA_REQ_CHECK_VERSION] PdlVersion: {}"
			, mPdlVersion
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_CA_REQ_CHECK_VERSION_READ);
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

}; // PKT_CA_REQ_CHECK_VERSION_READ

// type = CA
// related packet = PKT_AC_ACK_CHECK_VERSION_READ
#ifdef _SERVER
struct PKT_CA_REQ_CHECK_VERSION_WRITE : public Disposable
#else
struct PKT_CA_REQ_CHECK_VERSION_WRITE
#endif
{
	PktWriter mPktWriter;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_CA_REQ_CHECK_VERSION_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"CA_REQ_CHECK_VERSION"; }
	const wchar_t* GetRelatedPacketName() const { return L"AC_ACK_CHECK_VERSION"; }
#endif

#ifndef _SERVER
	PKT_CA_REQ_CHECK_VERSION_WRITE(uchar* buf, int bufSize, uint64 pdlVersion)
		:
		mPktWriter(buf, bufSize)

	{
		mPktWriter << static_cast<uint16>(PD::CA::CAPacketId::CA_REQ_CHECK_VERSION);
		mPktWriter << pdlVersion;
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_CA_REQ_CHECK_VERSION_WRITE(SendBufferHelper& sendBuffer, uint64 pdlVersion)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))

	{
		mPktWriter << (uint16)CA_REQ_CHECK_VERSION;
		mPktWriter << pdlVersion;
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
		auto readPacket = reinterpret_cast<PKT_CA_REQ_CHECK_VERSION_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
#else
#endif
}; // PKT_CA_REQ_CHECK_VERSION_WRITE

// type = CA
// related packet = PKT_AC_ACK_APP_GUARD_AUTH_WRITE
struct PKT_CA_REQ_APP_GUARD_AUTH_READ
{
public:
	uint16 mPktSize;
	uint16 mPktId;
	bool mIsSuccessAppGuardCallback;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"CA_REQ_APP_GUARD_AUTH"; }
	const wchar_t* GetRelatedPacketName() const { return L"AC_ACK_APP_GUARD_AUTH"; }
#endif

	bool IsSuccessAppGuardCallback() { return mIsSuccessAppGuardCallback; }


#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{

		return Log::format(
			L"[CA_REQ_APP_GUARD_AUTH] IsSuccessAppGuardCallback: {}"
			, mIsSuccessAppGuardCallback
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_CA_REQ_APP_GUARD_AUTH_READ);
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

}; // PKT_CA_REQ_APP_GUARD_AUTH_READ

// type = CA
// related packet = PKT_AC_ACK_APP_GUARD_AUTH_READ
#ifdef _SERVER
struct PKT_CA_REQ_APP_GUARD_AUTH_WRITE : public Disposable
#else
struct PKT_CA_REQ_APP_GUARD_AUTH_WRITE
#endif
{
	PktWriter mPktWriter;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_CA_REQ_APP_GUARD_AUTH_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"CA_REQ_APP_GUARD_AUTH"; }
	const wchar_t* GetRelatedPacketName() const { return L"AC_ACK_APP_GUARD_AUTH"; }
#endif

#ifndef _SERVER
	PKT_CA_REQ_APP_GUARD_AUTH_WRITE(uchar* buf, int bufSize, bool isSuccessAppGuardCallback)
		:
		mPktWriter(buf, bufSize)

	{
		mPktWriter << static_cast<uint16>(PD::CA::CAPacketId::CA_REQ_APP_GUARD_AUTH);
		mPktWriter << isSuccessAppGuardCallback;
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_CA_REQ_APP_GUARD_AUTH_WRITE(SendBufferHelper& sendBuffer, bool isSuccessAppGuardCallback)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))

	{
		mPktWriter << (uint16)CA_REQ_APP_GUARD_AUTH;
		mPktWriter << isSuccessAppGuardCallback;
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
		auto readPacket = reinterpret_cast<PKT_CA_REQ_APP_GUARD_AUTH_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
#else
#endif
}; // PKT_CA_REQ_APP_GUARD_AUTH_WRITE

// type = CA
// related packet = PKT_AC_ACK_DELETE_ACCOUNT_WRITE
struct PKT_CA_REQ_DELETE_ACCOUNT_READ
{
public:
	uint16 mPktSize;
	uint16 mPktId;
	uint16 mAuthorizationOffset;
	uint16 mAppIdOffset;
	Did mDid;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"CA_REQ_DELETE_ACCOUNT"; }
	const wchar_t* GetRelatedPacketName() const { return L"AC_ACK_DELETE_ACCOUNT"; }
#endif

	Did Did() { return mDid; }
#ifdef _SERVER
	const wchar_t* Authorization() const
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = *((uint16*)pktStart);
		if (mAuthorizationOffset && pktSize > mAuthorizationOffset)
			return (const wchar_t*)(pktStart + mAuthorizationOffset);
		else
			return L"";
	}
#else // _SERVER
	const FString Authorization()
	{
		const wchar_t* strWchar = L"";
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = *((uint16*)pktStart);
		if (mAuthorizationOffset && pktSize > mAuthorizationOffset)
			strWchar = (const wchar_t*)(pktStart + mAuthorizationOffset);

		return PktWriter::ConvertToFString(strWchar);
	}
#endif // _SERVER
#ifdef _SERVER
	const wchar_t* AppId() const
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = *((uint16*)pktStart);
		if (mAppIdOffset && pktSize > mAppIdOffset)
			return (const wchar_t*)(pktStart + mAppIdOffset);
		else
			return L"";
	}
#else // _SERVER
	const FString AppId()
	{
		const wchar_t* strWchar = L"";
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = *((uint16*)pktStart);
		if (mAppIdOffset && pktSize > mAppIdOffset)
			strWchar = (const wchar_t*)(pktStart + mAppIdOffset);

		return PktWriter::ConvertToFString(strWchar);
	}
#endif // _SERVER


#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{

		return Log::format(
			L"[CA_REQ_DELETE_ACCOUNT] Authorization: {}, AppId: {}, Did: {}"
			, Authorization(), AppId(), mDid
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_CA_REQ_DELETE_ACCOUNT_READ);
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
		if (false == ValidateString(mAuthorizationOffset, 0, pktStart, pktSize, dataSize))
			return false;

		if (false == ValidateString(mAppIdOffset, 0, pktStart, pktSize, dataSize))
			return false;

		return (nullptr != pktStart && 0 < pktSize && 0 < dataSize) ? true : false;
	}

}; // PKT_CA_REQ_DELETE_ACCOUNT_READ

// type = CA
// related packet = PKT_AC_ACK_DELETE_ACCOUNT_READ
#ifdef _SERVER
struct PKT_CA_REQ_DELETE_ACCOUNT_WRITE : public Disposable
#else
struct PKT_CA_REQ_DELETE_ACCOUNT_WRITE
#endif
{
	PktWriter mPktWriter;
	uint16* mAuthorizationOffset;
	uint16* mAppIdOffset;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_CA_REQ_DELETE_ACCOUNT_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"CA_REQ_DELETE_ACCOUNT"; }
	const wchar_t* GetRelatedPacketName() const { return L"AC_ACK_DELETE_ACCOUNT"; }
#endif

#ifndef _SERVER
	PKT_CA_REQ_DELETE_ACCOUNT_WRITE(uchar* buf, int bufSize, const FString& authorization, const FString& appId, Did did)
		:
		mPktWriter(buf, bufSize)

	{
		mPktWriter << static_cast<uint16>(PD::CA::CAPacketId::CA_REQ_DELETE_ACCOUNT);
		mAuthorizationOffset = (uint16*)mPktWriter.GetBuffer();
		*mAuthorizationOffset = 0;
		mPktWriter << (*mAuthorizationOffset);
		mAppIdOffset = (uint16*)mPktWriter.GetBuffer();
		*mAppIdOffset = 0;
		mPktWriter << (*mAppIdOffset);
		mPktWriter << did;
		*mAuthorizationOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(authorization);
		*mAppIdOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(appId);
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_CA_REQ_DELETE_ACCOUNT_WRITE(SendBufferHelper& sendBuffer, const wchar* authorization, const wchar* appId, Did did)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))

	{
		mPktWriter << (uint16)CA_REQ_DELETE_ACCOUNT;
		mAuthorizationOffset = (uint16*)mPktWriter.GetBuffer();
		*mAuthorizationOffset = 0;
		mPktWriter << (*mAuthorizationOffset);
		mAppIdOffset = (uint16*)mPktWriter.GetBuffer();
		*mAppIdOffset = 0;
		mPktWriter << (*mAppIdOffset);
		mPktWriter << did;
		*mAuthorizationOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(authorization);
		*mAppIdOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(appId);
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
		auto readPacket = reinterpret_cast<PKT_CA_REQ_DELETE_ACCOUNT_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
#else
#endif
}; // PKT_CA_REQ_DELETE_ACCOUNT_WRITE

// type = CA
// related packet = PKT_AC_ACK_PRE_LOGIN_WRITE
struct PKT_CA_REQ_PRE_LOGIN_READ
{
	struct IdpList
	{
	protected:
		uint16 mThisOffset;
		uint16 mNextOffset;
		uint16 mIdpOffset;

	public:

#ifdef _SERVER
		const wchar_t* Idp()
		{
			uchar* pktStart = (uchar*)this - mThisOffset;
			uint16 pktSize = *((uint16*)pktStart);
			if (mIdpOffset && pktSize > mIdpOffset)
				return (const wchar_t*)(pktStart + mIdpOffset);
			else
				return L"";
		}
#else // _SERVER
		const FString Idp()
		{
			const wchar_t* strWchar = L"";
			uchar* pktStart = (uchar*)this - mThisOffset;
			uint16 pktSize = *((uint16*)pktStart);
			if (mIdpOffset && pktSize > mIdpOffset)
				strWchar = (const wchar_t*)(pktStart + mIdpOffset);
			return PktWriter::ConvertToFString(strWchar);
		}
#endif // _SERVER
		friend PktIterator<IdpList>;
		friend PKT_CA_REQ_PRE_LOGIN_READ;

		bool Validate(uint16 offset, uchar * pktStart, uint16 pktSize, uint16& dataSize)
		{
			if (static_cast<size_t>(pktSize) < static_cast<size_t>(offset) + sizeof(IdpList))
				return false;

			IdpList * node = (IdpList*)(pktStart + offset);

			// 자기 자신의 포인터 검증
			if (offset != node->mThisOffset)
				return false;

			// Value 검증
			uchar* pktCurrentNodeStart = (uchar*)this - mThisOffset;
			uint16 pktCurrentNodeSize = *((uint16*)pktStart);
			dataSize += sizeof(IdpList);	// skip
			if (false == ValidateValue(pktCurrentNodeStart, pktCurrentNodeSize, dataSize))
				return false;

			return true;
		}

		bool ValidateValue(const uchar* pktStart, uint16 pktSize, uint16& dataSize)
		{
			if (false == ValidateString(mIdpOffset, 0, pktStart, pktSize, dataSize))
				return false;

			return (nullptr != pktStart && 0 < pktSize && 0 < dataSize) ? true : false;
		}

	}; // IdpList

public:
	uint16 mPktSize;
	uint16 mPktId;
	uint16 mIdpListCount;
	uint16 mFirstIdpListOffset;
	uint16 mAuthorizationOffset;
	uint16 mAppidOffset;
	uint16 mDidOffset;
	uint16 mDeviceNameOffset;
	uint16 mHiveLanguageCodeOffset;
	uint16 mHiveCountryCodeOffset;
	uint16 mChannelTypeOffset;
	uint16 mMarketTypeOffset;
	Vid mVid;
	uint64 mPdlVersion;
	int mPlatform;
	int mTimeZoneValue;
	bool mIsSandbox;
	AppGuardActiveMode mAppGuardActiveMode;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"CA_REQ_PRE_LOGIN"; }
	const wchar_t* GetRelatedPacketName() const { return L"AC_ACK_PRE_LOGIN"; }
#endif

	Vid Vid() { return mVid; }
	uint64 PdlVersion() { return mPdlVersion; }
	int Platform() { return mPlatform; }
	int TimeZoneValue() { return mTimeZoneValue; }
	bool IsSandbox() { return mIsSandbox; }
	const AppGuardActiveMode& ConstRefAppGuardActiveMode() { return mAppGuardActiveMode; }
	AppGuardActiveMode AppGuardActiveMode() { return mAppGuardActiveMode; }
#ifdef _SERVER
	const wchar_t* Authorization() const
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = *((uint16*)pktStart);
		if (mAuthorizationOffset && pktSize > mAuthorizationOffset)
			return (const wchar_t*)(pktStart + mAuthorizationOffset);
		else
			return L"";
	}
#else // _SERVER
	const FString Authorization()
	{
		const wchar_t* strWchar = L"";
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = *((uint16*)pktStart);
		if (mAuthorizationOffset && pktSize > mAuthorizationOffset)
			strWchar = (const wchar_t*)(pktStart + mAuthorizationOffset);

		return PktWriter::ConvertToFString(strWchar);
	}
#endif // _SERVER
#ifdef _SERVER
	const wchar_t* Appid() const
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = *((uint16*)pktStart);
		if (mAppidOffset && pktSize > mAppidOffset)
			return (const wchar_t*)(pktStart + mAppidOffset);
		else
			return L"";
	}
#else // _SERVER
	const FString Appid()
	{
		const wchar_t* strWchar = L"";
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = *((uint16*)pktStart);
		if (mAppidOffset && pktSize > mAppidOffset)
			strWchar = (const wchar_t*)(pktStart + mAppidOffset);

		return PktWriter::ConvertToFString(strWchar);
	}
#endif // _SERVER
#ifdef _SERVER
	const wchar_t* Did() const
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = *((uint16*)pktStart);
		if (mDidOffset && pktSize > mDidOffset)
			return (const wchar_t*)(pktStart + mDidOffset);
		else
			return L"";
	}
#else // _SERVER
	const FString Did()
	{
		const wchar_t* strWchar = L"";
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = *((uint16*)pktStart);
		if (mDidOffset && pktSize > mDidOffset)
			strWchar = (const wchar_t*)(pktStart + mDidOffset);

		return PktWriter::ConvertToFString(strWchar);
	}
#endif // _SERVER
#ifdef _SERVER
	const wchar_t* DeviceName() const
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = *((uint16*)pktStart);
		if (mDeviceNameOffset && pktSize > mDeviceNameOffset)
			return (const wchar_t*)(pktStart + mDeviceNameOffset);
		else
			return L"";
	}
#else // _SERVER
	const FString DeviceName()
	{
		const wchar_t* strWchar = L"";
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = *((uint16*)pktStart);
		if (mDeviceNameOffset && pktSize > mDeviceNameOffset)
			strWchar = (const wchar_t*)(pktStart + mDeviceNameOffset);

		return PktWriter::ConvertToFString(strWchar);
	}
#endif // _SERVER
#ifdef _SERVER
	const wchar_t* HiveLanguageCode() const
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = *((uint16*)pktStart);
		if (mHiveLanguageCodeOffset && pktSize > mHiveLanguageCodeOffset)
			return (const wchar_t*)(pktStart + mHiveLanguageCodeOffset);
		else
			return L"";
	}
#else // _SERVER
	const FString HiveLanguageCode()
	{
		const wchar_t* strWchar = L"";
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = *((uint16*)pktStart);
		if (mHiveLanguageCodeOffset && pktSize > mHiveLanguageCodeOffset)
			strWchar = (const wchar_t*)(pktStart + mHiveLanguageCodeOffset);

		return PktWriter::ConvertToFString(strWchar);
	}
#endif // _SERVER
#ifdef _SERVER
	const wchar_t* HiveCountryCode() const
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = *((uint16*)pktStart);
		if (mHiveCountryCodeOffset && pktSize > mHiveCountryCodeOffset)
			return (const wchar_t*)(pktStart + mHiveCountryCodeOffset);
		else
			return L"";
	}
#else // _SERVER
	const FString HiveCountryCode()
	{
		const wchar_t* strWchar = L"";
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = *((uint16*)pktStart);
		if (mHiveCountryCodeOffset && pktSize > mHiveCountryCodeOffset)
			strWchar = (const wchar_t*)(pktStart + mHiveCountryCodeOffset);

		return PktWriter::ConvertToFString(strWchar);
	}
#endif // _SERVER
#ifdef _SERVER
	const wchar_t* ChannelType() const
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = *((uint16*)pktStart);
		if (mChannelTypeOffset && pktSize > mChannelTypeOffset)
			return (const wchar_t*)(pktStart + mChannelTypeOffset);
		else
			return L"";
	}
#else // _SERVER
	const FString ChannelType()
	{
		const wchar_t* strWchar = L"";
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = *((uint16*)pktStart);
		if (mChannelTypeOffset && pktSize > mChannelTypeOffset)
			strWchar = (const wchar_t*)(pktStart + mChannelTypeOffset);

		return PktWriter::ConvertToFString(strWchar);
	}
#endif // _SERVER
#ifdef _SERVER
	const wchar_t* MarketType() const
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = *((uint16*)pktStart);
		if (mMarketTypeOffset && pktSize > mMarketTypeOffset)
			return (const wchar_t*)(pktStart + mMarketTypeOffset);
		else
			return L"";
	}
#else // _SERVER
	const FString MarketType()
	{
		const wchar_t* strWchar = L"";
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = *((uint16*)pktStart);
		if (mMarketTypeOffset && pktSize > mMarketTypeOffset)
			strWchar = (const wchar_t*)(pktStart + mMarketTypeOffset);

		return PktWriter::ConvertToFString(strWchar);
	}
#endif // _SERVER

	using IdpListIterator = PktIterator<IdpList>;
	const IdpListIterator GetLastIdpListIterator() const
	{
		return IdpListIterator(0);
	}
	IdpListIterator GetFirstIdpListIterator() const
	{
		if (mFirstIdpListOffset && mPktSize >= mFirstIdpListOffset + sizeof(IdpList))
		{
			IdpList* idpList = (IdpList*)((uchar*)this + mFirstIdpListOffset);
			if (mFirstIdpListOffset == idpList->mThisOffset)
				return IdpListIterator(idpList);
			else
				return IdpListIterator(0);
		}
		else
			return IdpListIterator(0);
	}
	const int GetIdpListCount()
	{
		return mIdpListCount;
	}
#ifdef _SERVER
	template<typename TFunc>
	void ForEachIdpList(TFunc&& func)
	{
		for (auto it = GetFirstIdpListIterator(); it != GetLastIdpListIterator(); ++it)
			func(it);
	}
#endif


#ifdef _SERVER 
	std::xwstring FuncCreateIdpListStr( [[maybe_unused]] IdpListIterator& iter) const
	{
		return Log::format(
			L"Idp: {}", 
			iter->Idp()
		).c_str();
	}

	std::xwstring GetLogWString() const
	{
		std::xwstring str1;
		str1.append(L"\n");
		for (IdpListIterator iter = this->GetFirstIdpListIterator();
			iter != GetLastIdpListIterator(); ++iter)
		{
			str1.append(L"[");
			str1.append(FuncCreateIdpListStr(iter).c_str());

			str1.append(L"]\n");
		}

		return Log::format(
			L"[CA_REQ_PRE_LOGIN] Vid: {}, PdlVersion: {}, Authorization: {}, Platform: {}, Appid: {}, Did: {}, DeviceName: {}, HiveLanguageCode: {}, HiveCountryCode: {}, TimeZoneValue: {}, ChannelType: {}, MarketType: {}, IsSandbox: {}, AppGuardActiveMode: {}, IdpList : {}"
			, mVid, mPdlVersion, Authorization(), mPlatform, Appid(), Did(), DeviceName(), HiveLanguageCode(), HiveCountryCode(), mTimeZoneValue, ChannelType(), MarketType(), mIsSandbox, mAppGuardActiveMode, str1.c_str()
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_CA_REQ_PRE_LOGIN_READ);
		if (dataSize > mPktSize)
			return false;

		if (false == ValidateValue(pktStart, pktSize, dataSize))
			return false;

		if (false == IdpListValidateListNode(mFirstIdpListOffset, pktStart, pktSize, dataSize))
			return false;

		if (dataSize != mPktSize)
			return false;

		return true;
	}

	bool IdpListValidateListNode(uint16 offset, uchar * pktStart, uint16 pktSize, uint16& dataSize)
	{		for (IdpListIterator iter = this->GetFirstIdpListIterator();
			iter != GetLastIdpListIterator(); ++iter)
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
		if (false == ValidateString(mAuthorizationOffset, 0, pktStart, pktSize, dataSize))
			return false;

		if (false == ValidateString(mAppidOffset, 0, pktStart, pktSize, dataSize))
			return false;

		if (false == ValidateString(mDidOffset, 0, pktStart, pktSize, dataSize))
			return false;

		if (false == ValidateString(mDeviceNameOffset, 0, pktStart, pktSize, dataSize))
			return false;

		if (false == ValidateString(mHiveLanguageCodeOffset, 0, pktStart, pktSize, dataSize))
			return false;

		if (false == ValidateString(mHiveCountryCodeOffset, 0, pktStart, pktSize, dataSize))
			return false;

		if (false == ValidateString(mChannelTypeOffset, 0, pktStart, pktSize, dataSize))
			return false;

		if (false == ValidateString(mMarketTypeOffset, 0, pktStart, pktSize, dataSize))
			return false;

		return (nullptr != pktStart && 0 < pktSize && 0 < dataSize) ? true : false;
	}

}; // PKT_CA_REQ_PRE_LOGIN_READ

// type = CA
// related packet = PKT_AC_ACK_PRE_LOGIN_READ
#ifdef _SERVER
struct PKT_CA_REQ_PRE_LOGIN_WRITE : public Disposable
#else
struct PKT_CA_REQ_PRE_LOGIN_WRITE
#endif
{
	struct IdpList
	{
		IdpList(uint16 offset)
			:
			mThisOffset(offset)
			, mNextOffset(0), mIdpOffset(0)
		{
		}
		void SetIdpOffset(uint16 offset)
		{
			mIdpOffset = offset;
		}

		uint16 mThisOffset;
		uint16 mNextOffset;
		uint16 mIdpOffset;

	}; // IdpList

	struct IdpListHandle
	{
		IdpListHandle(PktWriter& pktWriter, IdpList* idpList)
			:
			mPktWriter(pktWriter)
			, mIdpList(idpList)
		{
		}

		PktWriter& mPktWriter;
		IdpList* mIdpList;

	}; // IdpListHandle

	PktWriter mPktWriter;
	uint16* mIdpListCount;
	uint16* mFirstIdpListOffset;
	IdpList* mNowIdpList;
	uint16* mAuthorizationOffset;
	uint16* mAppidOffset;
	uint16* mDidOffset;
	uint16* mDeviceNameOffset;
	uint16* mHiveLanguageCodeOffset;
	uint16* mHiveCountryCodeOffset;
	uint16* mChannelTypeOffset;
	uint16* mMarketTypeOffset;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_CA_REQ_PRE_LOGIN_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"CA_REQ_PRE_LOGIN"; }
	const wchar_t* GetRelatedPacketName() const { return L"AC_ACK_PRE_LOGIN"; }
#endif

#ifndef _SERVER
	PKT_CA_REQ_PRE_LOGIN_WRITE(uchar* buf, int bufSize, Vid vid, uint64 pdlVersion, const FString& authorization, int platform, const FString& appid, const FString& did, const FString& deviceName, const FString& hiveLanguageCode, const FString& hiveCountryCode, int timeZoneValue, const FString& channelType, const FString& marketType, bool isSandbox, AppGuardActiveMode appGuardActiveMode)
		:
		mPktWriter(buf, bufSize)
		, mNowIdpList(0)
	{
		mPktWriter << static_cast<uint16>(PD::CA::CAPacketId::CA_REQ_PRE_LOGIN);
		mIdpListCount = (uint16*)mPktWriter.GetBuffer();
		*mIdpListCount = 0;
		mPktWriter << (*mIdpListCount);
		mFirstIdpListOffset = (uint16*)mPktWriter.GetBuffer();
		*mFirstIdpListOffset = 0;
		mPktWriter << (*mFirstIdpListOffset);
		mAuthorizationOffset = (uint16*)mPktWriter.GetBuffer();
		*mAuthorizationOffset = 0;
		mPktWriter << (*mAuthorizationOffset);
		mAppidOffset = (uint16*)mPktWriter.GetBuffer();
		*mAppidOffset = 0;
		mPktWriter << (*mAppidOffset);
		mDidOffset = (uint16*)mPktWriter.GetBuffer();
		*mDidOffset = 0;
		mPktWriter << (*mDidOffset);
		mDeviceNameOffset = (uint16*)mPktWriter.GetBuffer();
		*mDeviceNameOffset = 0;
		mPktWriter << (*mDeviceNameOffset);
		mHiveLanguageCodeOffset = (uint16*)mPktWriter.GetBuffer();
		*mHiveLanguageCodeOffset = 0;
		mPktWriter << (*mHiveLanguageCodeOffset);
		mHiveCountryCodeOffset = (uint16*)mPktWriter.GetBuffer();
		*mHiveCountryCodeOffset = 0;
		mPktWriter << (*mHiveCountryCodeOffset);
		mChannelTypeOffset = (uint16*)mPktWriter.GetBuffer();
		*mChannelTypeOffset = 0;
		mPktWriter << (*mChannelTypeOffset);
		mMarketTypeOffset = (uint16*)mPktWriter.GetBuffer();
		*mMarketTypeOffset = 0;
		mPktWriter << (*mMarketTypeOffset);
		mPktWriter << vid;
		mPktWriter << pdlVersion;
		mPktWriter << platform;
		mPktWriter << timeZoneValue;
		mPktWriter << isSandbox;
		mPktWriter << appGuardActiveMode;
		*mAuthorizationOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(authorization);
		*mAppidOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(appid);
		*mDidOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(did);
		*mDeviceNameOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(deviceName);
		*mHiveLanguageCodeOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(hiveLanguageCode);
		*mHiveCountryCodeOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(hiveCountryCode);
		*mChannelTypeOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(channelType);
		*mMarketTypeOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(marketType);
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_CA_REQ_PRE_LOGIN_WRITE(SendBufferHelper& sendBuffer, Vid vid, uint64 pdlVersion, const wchar* authorization, int platform, const wchar* appid, const wchar* did, const wchar* deviceName, const wchar* hiveLanguageCode, const wchar* hiveCountryCode, int timeZoneValue, const wchar* channelType, const wchar* marketType, bool isSandbox, const AppGuardActiveMode& appGuardActiveMode)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))
		, mNowIdpList(0)
	{
		mPktWriter << (uint16)CA_REQ_PRE_LOGIN;
		mIdpListCount = (uint16*)mPktWriter.GetBuffer();
		*mIdpListCount = 0;
		mPktWriter << (*mIdpListCount);
		mFirstIdpListOffset = (uint16*)mPktWriter.GetBuffer();
		*mFirstIdpListOffset = 0;
		mPktWriter << (*mFirstIdpListOffset);
		mAuthorizationOffset = (uint16*)mPktWriter.GetBuffer();
		*mAuthorizationOffset = 0;
		mPktWriter << (*mAuthorizationOffset);
		mAppidOffset = (uint16*)mPktWriter.GetBuffer();
		*mAppidOffset = 0;
		mPktWriter << (*mAppidOffset);
		mDidOffset = (uint16*)mPktWriter.GetBuffer();
		*mDidOffset = 0;
		mPktWriter << (*mDidOffset);
		mDeviceNameOffset = (uint16*)mPktWriter.GetBuffer();
		*mDeviceNameOffset = 0;
		mPktWriter << (*mDeviceNameOffset);
		mHiveLanguageCodeOffset = (uint16*)mPktWriter.GetBuffer();
		*mHiveLanguageCodeOffset = 0;
		mPktWriter << (*mHiveLanguageCodeOffset);
		mHiveCountryCodeOffset = (uint16*)mPktWriter.GetBuffer();
		*mHiveCountryCodeOffset = 0;
		mPktWriter << (*mHiveCountryCodeOffset);
		mChannelTypeOffset = (uint16*)mPktWriter.GetBuffer();
		*mChannelTypeOffset = 0;
		mPktWriter << (*mChannelTypeOffset);
		mMarketTypeOffset = (uint16*)mPktWriter.GetBuffer();
		*mMarketTypeOffset = 0;
		mPktWriter << (*mMarketTypeOffset);
		mPktWriter << vid;
		mPktWriter << pdlVersion;
		mPktWriter << platform;
		mPktWriter << timeZoneValue;
		mPktWriter << isSandbox;
		mPktWriter << appGuardActiveMode;
		*mAuthorizationOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(authorization);
		*mAppidOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(appid);
		*mDidOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(did);
		*mDeviceNameOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(deviceName);
		*mHiveLanguageCodeOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(hiveLanguageCode);
		*mHiveCountryCodeOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(hiveCountryCode);
		*mChannelTypeOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(channelType);
		*mMarketTypeOffset = (uint16)mPktWriter.GetPktSize();
		mPktWriter.WriteString(marketType);
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
		auto readPacket = reinterpret_cast<PKT_CA_REQ_PRE_LOGIN_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
	IdpListHandle CreateIdpList(const FString& idp)
	{
		uchar* nowBuffer = mPktWriter.GetBuffer();
		uint16 nowWritten = (uint16)mPktWriter.GetPktSize();
		IdpList* newIdpList = new ((IdpList*)nowBuffer) IdpList(nowWritten);
		++(*mIdpListCount);
		if (mNowIdpList == 0)
			*mFirstIdpListOffset = nowWritten;
		else
			mNowIdpList->mNextOffset = nowWritten;
		mNowIdpList = newIdpList;
		mPktWriter.Skip(sizeof(IdpList));
		mNowIdpList->SetIdpOffset((uint16)mPktWriter.GetPktSize());
		mPktWriter.WriteString(idp);
		return IdpListHandle(mPktWriter, newIdpList);
	}

#else
	IdpListHandle CreateIdpList(const wchar* idp)
	{
		uchar* nowBuffer = mPktWriter.GetBuffer();
		uint16 nowWritten = (uint16)mPktWriter.GetPktSize();
		IdpList* newIdpList = new ((IdpList*)nowBuffer) IdpList(nowWritten);
		++(*mIdpListCount);
		if (mNowIdpList == 0)
			*mFirstIdpListOffset = nowWritten;
		else
			mNowIdpList->mNextOffset = nowWritten;
		mNowIdpList = newIdpList;
		mPktWriter.Skip(sizeof(IdpList));
		mNowIdpList->SetIdpOffset((uint16)mPktWriter.GetPktSize());
		mPktWriter.WriteString(idp);
		return IdpListHandle(mPktWriter, newIdpList);
	}

#endif
}; // PKT_CA_REQ_PRE_LOGIN_WRITE

// type = CA
// related packet = PKT_AC_ACK_IDP_CONFLICT_PLYER_WRITE
struct PKT_CA_REQ_IDP_CONFLICT_PLYER_READ
{
public:
	uint16 mPktSize;
	uint16 mPktId;
	Vid mVid;

public:
#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT [[maybe_unused]] int& serverTaskId) const { return false; }
	const wchar_t* GetPacketName() const { return L"CA_REQ_IDP_CONFLICT_PLYER"; }
	const wchar_t* GetRelatedPacketName() const { return L"AC_ACK_IDP_CONFLICT_PLYER"; }
#endif

	Vid Vid() { return mVid; }


#ifdef _SERVER 
	std::xwstring GetLogWString() const
	{

		return Log::format(
			L"[CA_REQ_IDP_CONFLICT_PLYER] Vid: {}"
			, mVid
		).c_str();
	}
#endif

	bool Validate()
	{
		uchar* pktStart = (uchar*)this;
		uint16 pktSize = mPktSize;

		uint16 dataSize = sizeof(PKT_CA_REQ_IDP_CONFLICT_PLYER_READ);
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

}; // PKT_CA_REQ_IDP_CONFLICT_PLYER_READ

// type = CA
// related packet = PKT_AC_ACK_IDP_CONFLICT_PLYER_READ
#ifdef _SERVER
struct PKT_CA_REQ_IDP_CONFLICT_PLYER_WRITE : public Disposable
#else
struct PKT_CA_REQ_IDP_CONFLICT_PLYER_WRITE
#endif
{
	PktWriter mPktWriter;

#ifdef _SERVER 
	bool GetServerTaskIdRealTime(OUT int& serverTaskId) const { return reinterpret_cast<PKT_CA_REQ_IDP_CONFLICT_PLYER_READ*>(mPktWriter.mBuf)->GetServerTaskIdRealTime(serverTaskId); }
	const wchar_t* GetPacketName() const { return L"CA_REQ_IDP_CONFLICT_PLYER"; }
	const wchar_t* GetRelatedPacketName() const { return L"AC_ACK_IDP_CONFLICT_PLYER"; }
#endif

#ifndef _SERVER
	PKT_CA_REQ_IDP_CONFLICT_PLYER_WRITE(uchar* buf, int bufSize, Vid vid)
		:
		mPktWriter(buf, bufSize)

	{
		mPktWriter << static_cast<uint16>(PD::CA::CAPacketId::CA_REQ_IDP_CONFLICT_PLYER);
		mPktWriter << vid;
	}

#endif // _SERVER
#ifdef _SERVER
	PKT_CA_REQ_IDP_CONFLICT_PLYER_WRITE(SendBufferHelper& sendBuffer, Vid vid)
		:
		mPktWriter(sendBuffer->GetCharBuffer(), (SEND_BUFFER_SIZE - 1))

	{
		mPktWriter << (uint16)CA_REQ_IDP_CONFLICT_PLYER;
		mPktWriter << vid;
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
		auto readPacket = reinterpret_cast<PKT_CA_REQ_IDP_CONFLICT_PLYER_READ*>(mPktWriter.mBuf);
		return readPacket->GetLogWString();
	}
#endif

#ifndef _SERVER
#else
#endif
}; // PKT_CA_REQ_IDP_CONFLICT_PLYER_WRITE

#pragma pack(pop)
#pragma warning(pop)
} // namespace CA
} // namespace PD
