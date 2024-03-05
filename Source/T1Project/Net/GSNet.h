#pragma once
#include <typeinfo>

#define MAX_BUFFUER = 512

#pragma pack(push, 1)
class FGsNet
{
public:
	enum Mode
	{
		LOBBY,
		GAME,
		RECONNECT,
		MAX,
	};

	using PacketID = unsigned short;

	// 패킷을 묶어주는 역활
	struct Packet
	{
		explicit Packet(unsigned char* inBuffer, int inLength) : Buffer(inBuffer), Length(inLength) {}
		unsigned char*	Buffer;
		int				Length;
	};
};

namespace tempUse
{
	template <typename PKT>
	class ResultGet
	{
	private:
		template <typename U, int(U::*)()> struct ResultCheck;
		template <typename U> static char ResultTest(ResultCheck<U, &U::Result>*);
		template <typename U> static int  ResultTest(...);

	public:
		enum { Value = sizeof(ResultTest<PKT>(nullptr)) == sizeof(char) };
	};
}
template <bool Predicate, typename PacketStruct>
struct FGsErrorHandlePacket;

struct FGsAckPacket : public FGsNet::Packet
{
	using Type = char;
	uint16 _packetId{ 0 };
	int  _result{ 0 };

	uint8 _typeErrorUI{ 0 };
	uint8 _typeErrorProc{ 0 };
	FText _errorText;

	FGsAckPacket() : FGsNet::Packet(nullptr, 0) {}
	virtual ~FGsAckPacket() = default;
	virtual void SetPacket(uint16 id, const FGsNet::Packet& inData)
	{
		_packetId = id;
		Length = inData.Length;
		Buffer = inData.Buffer;
	}

	void Clear()
	{
		Buffer = nullptr;
		Length = 0;
		_typeErrorUI = 0;
		_typeErrorProc = 0;
		_errorText = FText::GetEmpty();
	}

	int  Result() const { return _result; }
	bool IsSuccess() const { return _result == 0; }
	bool IsProcessErrorUI() const { return _typeErrorProc != 0; }

	template<typename T>
	T* PacketCast() const { return  reinterpret_cast<T*>(Buffer); }
};

template <typename PacketStruct>
struct FGsErrorHandlePacket<true, PacketStruct> : public FGsAckPacket
{
	using Type = PacketStruct;

	FGsErrorHandlePacket() : FGsAckPacket() {}
	virtual ~FGsErrorHandlePacket() = default;

	virtual void SetPacket(uint16 id, const FGsNet::Packet& inData) override
	{
		FGsAckPacket::SetPacket(id, inData);
		if (inData.Buffer)
		{
			if (PacketStruct* packet = PacketCast<PacketStruct>())
			{
				_result = tempUse::ResultGet<PacketStruct>::Value ? packet->Result() : 0; //tempUse::ResultGet<PacketStruct>::ResultInvoke<tempUse::ResultGet<tempUse::ResultGet<PacketStruct>::Value, PacketStruct>(packet, &PacketStruct::Result);
			}
		}
	}
};

template <typename PacketStruct>
struct FGsErrorHandlePacket<false, PacketStruct> : public FGsAckPacket
{
	using Type = PacketStruct;

	FGsErrorHandlePacket() : FGsAckPacket() {}
	virtual ~FGsErrorHandlePacket() = default;

	virtual void SetPacket(uint16 id, const FGsNet::Packet& inData) override
	{
		FGsAckPacket::SetPacket(id, inData);
		_result = 0;
	}
};

#pragma pack(pop)