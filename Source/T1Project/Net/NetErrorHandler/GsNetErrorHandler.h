#pragma once
#include "CoreMinimal.h"
#include "Shared/Shared/SharedPackets/PD_Auth_Client.h"
#include "Shared/Shared/SharedPackets/PD_Gateway_Client.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Message/GsMessageNet.h"
#include "DataSchema/GsSchemaEnums.h"
#include "DataSchema/Net/GsSchemaNetErrorUI.h"
#include "DataSchema/Net/GsSchemaNetErrorProcess.h"
class FGsNetManager;

//----------------------------------------------------------------
// Network Error UI 핸들러
//----------------------------------------------------------------

struct FGsNetErrorProc
{
	int32	_id{ 0 };
	int32	_result{ 0 };
	bool	_isDefault{ false }; //같은 에러 메세제가 여러개가 있을 때, Main

	EGsNetErrorProcType _process{ EGsNetErrorProcType::NoProc }; //default

	struct PacketErrorType
	{
		uint16	packetId{ 0 };
		EGsNetErrorProcType process{ EGsNetErrorProcType::NoProc };
	};

	TArray<PacketErrorType> _errorProc;
	const FGsSchemaNetErrorUI* _errorData;

	FGsNetErrorProc(const FGsSchemaNetErrorUI* data, const FGsSchemaNetErrorProcess* procData = nullptr) 
		: _id(data ? data->id : 0)
		, _result(data ? data->result : 0)
		, _isDefault(data ? data->isDefault : false)
		, _process(EGsNetErrorProcType::NoProc)
		, _errorData(data)
	{
		SetPacketData(0, procData);
	}

	void SetPacketData(uint16 packetId, const FGsSchemaNetErrorProcess* procData = nullptr)
	{
		if (procData)
		{
			if (packetId == 0 || _errorData == nullptr)
			{
				_process = procData->process;
			}
			else if (_errorProc.FindByPredicate([packetId](auto& e) { return e.packetId == packetId; }) == nullptr)
			{
				_errorProc.Add(PacketErrorType{ packetId, procData->process });
			}
		}
	}


	int32 GetResult() { return _result;  }
	EGsNetErrorProcType GetProcessType(uint16 packetId = 0) { 
		auto iter = _errorProc.FindByPredicate([packetId](auto& e) { return e.packetId == packetId; });
		return iter ? (*iter).process : _process;
	}

	EGsNetErrorProcUIType GetErrorUIType() { return _errorData ? _errorData->uiType : EGsNetErrorProcUIType::SystemPopup; }
	FText GetErrorText() { return _errorData ? _errorData->text : FText::AsNumber(_result);}

	FString GetPopupKey() { return _errorData && _errorData->uiType == EGsNetErrorProcUIType::CustomPopup ? _errorData->popupKey : TEXT(""); }
	int32	GetPopupOption() { return _errorData && _errorData->uiType == EGsNetErrorProcUIType::CustomPopup ? _errorData->option : 0; }
};

class FGsNetErrorHandler
{
private:
	TArray<TSharedPtr<FGsNetErrorProc>> _resultProcs;

	TMap<uint16, FString> _ackACPackets;
	TMap<uint16, FString> _ackGCPackets;
	TMap<uint16, FString> _ackSCPackets;

protected:
	TMap<int32, TWeakPtr<FGsNetErrorProc>> _resultIdMap;
	TMap<int32, TArray<TWeakPtr<FGsNetErrorProc>>> _resultErrorList;
	TMap<uint16, TMap<int32, TWeakPtr<FGsNetErrorProc>>> _resultPacketError;

	const FString CommonMessageKey = TEXT("ErrorCommon");

	FText CommonMessage;

private:
	void LoadTables();

protected:
	virtual void Initialize(FGsNetManager* inManager);
	virtual void Finalize();

	virtual int ProcessErrorUI(TWeakPtr<FGsNetErrorProc> weakData);

public:
	FGsNetErrorHandler() = default;
	virtual ~FGsNetErrorHandler() = default;

public:
	static bool IsACPacket(int32 nPacketId);
	static bool IsGCPacket(int32 nPacketId);
	static bool IsSCPacket(int32 nPacketId);

	int32 ProcessPacket(uint16 packetId, int32 result, uint8& typeErrorUI, uint8& typeErrorProc, FText& _errorText, bool bShow = false);

	bool IsCheckEnablePacket(int32 nPacketId, bool defaultHandle = false);
	bool AddCheckPacket(int32 nPacketId);

	friend class FGsNetManager;
};
