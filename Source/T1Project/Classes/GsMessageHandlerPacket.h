#pragma once
#include "GsMessageHandlerOneParam.h"
#include "DataSchema/GsSchemaEnums.h"
#include "Net/GSNet.h"
#include "Net/NetErrorHandler/GsNetErrorHandler.h"
#include "Templates/IsEnumClass.h"
#include "Templates/EnableIf.h"

template <typename TMessageId, typename TParam>
class TGsMessageHandlerPacket : public TGsMessageHandlerOneParam<TMessageId, TParam>
{
	TWeakPtr<class FGsNetErrorHandler> _errorHandler;
	TMap<TMessageId, TSharedPtr<FGsAckPacket>> _erroChecks;

	using Super = TGsMessageHandlerOneParam<TMessageId, TParam>;

public:
	template <typename PacketClass, typename UserClass, typename... VarTypes>
	inline TPair<TMessageId, FDelegateHandle> AddRawPacket(TMessageId MessageId, UserClass* InUserObject,
		typename TMemFunPtrType<false, UserClass, void(TParam, VarTypes...)>::Type InFunc, VarTypes... Vars)
	{
		FDelegateHandle result;
		if (typename Super::MessageDelegator* findFunc = Super::GetRouter().Find(MessageId))
		{
			result = findFunc->AddRaw(InUserObject, InFunc, Forward<VarTypes>(Vars)...);
		}
		else
		{
			typename Super::MessageDelegator delFunc;
			result = delFunc.AddRaw(InUserObject, InFunc, Forward<VarTypes>(Vars)...);
			Super::GetRouter().Add(MessageId, delFunc);
		}

		if (_erroChecks.Find(MessageId) == nullptr)
		{
			if (_errorHandler.IsValid()) _errorHandler.Pin()->AddCheckPacket(static_cast<int32>(MessageId));
			TSharedPtr<FGsAckPacket> packet = MakeShared<FGsErrorHandlePacket<tempUse::ResultGet<PacketClass>::Value, PacketClass>>();

			_erroChecks.Add(MessageId, packet);
		}

		check(result.IsValid());
		return TPairInitializer<TMessageId, FDelegateHandle>(MessageId, result);
	}
	//------------------------------------------------------------------------------------------------
	// 메세지 전송(Sync 용)
	//------------------------------------------------------------------------------------------------
	virtual void SendMessage(TMessageId MessageId, TParam inData, bool inShow = true) override
	{
		bool isAckPacket = _erroChecks.Find(MessageId) != nullptr;

		if (Super::GetRouter().Find(MessageId))
		{
			if (isAckPacket)
			{
				auto messageChecker = _erroChecks.Find(MessageId);
				TSharedRef<FGsAckPacket> exData = (*messageChecker).ToSharedRef();
				exData->SetPacket(static_cast<uint16>(MessageId), inData);

				if (!_errorHandler.IsValid() ||
					 _errorHandler.Pin()->ProcessPacket(exData->_packetId, exData->_result,
						exData->_typeErrorUI, exData->_typeErrorProc, exData->_errorText, inShow) == 0)
				{
					auto data = StaticCastSharedRef<FGsNet::Packet>(exData);
					Super::SendMessage(MessageId, data.Get(), inShow);
				}

				exData->Clear();
				return;
			}
		}
#ifdef LOG_MESSAGE
		else
		{
			GSLOG(Warning, TEXT("[%d] Cant Find Router!!"), (int)MessageId);
		}
#endif

		Super::SendMessage(MessageId, inData, inShow);
	}

	virtual void Remove(const TPair<TMessageId, FDelegateHandle>& Handle) override
	{
		Super::Remove(Handle);

		if (Super::GetRouter().Find(Handle.Key) == nullptr)
		{
			_erroChecks.Remove(Handle.Key);
		}
	}

	virtual void RemoveAll() override
	{
		Super::RemoveAll();
		
		_erroChecks.Reset();
	}

	void SetErrorHandler(TWeakPtr<FGsNetErrorHandler> errorHandler) { _errorHandler = errorHandler; }
};