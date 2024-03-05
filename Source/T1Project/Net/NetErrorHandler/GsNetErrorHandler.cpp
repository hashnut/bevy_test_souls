#include "GsNetErrorHandler.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Shared/Shared/SharedPackets/PktStringTable.h"
#include "DataSchema/GsSchemaEnums.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Popup/GsUICustomErrorPopup.h"
#include "EnumRange.h"

//FString FGsNetErrorHandler::resultACPacketDelim = TEXT("AC_ACK_");
//FString FGsNetErrorHandler::resultGCPacketDelim = TEXT("GC_ACK_");
//FString FGsNetErrorHandler::resultSCPacketDelim = TEXT("SC_ACK_");

ENUM_RANGE_BY_VALUES(PD::AC::ACPacketId,
	PD::AC::ACPacketId::AC_PONG,
	PD::AC::ACPacketId::AC_ACK_LOGIN,
	PD::AC::ACPacketId::AC_ACK_SELECT_SERVER,
	PD::AC::ACPacketId::AC_KICKED_BY_TIMEOUT,
	PD::AC::ACPacketId::AC_ACK_CHECK_PASSWORD,
	PD::AC::ACPacketId::AC_ACK_CHECK_VERSION,
	PD::AC::ACPacketId::AC_ACK_APP_GUARD_AUTH,
	PD::AC::ACPacketId::AC_ACK_DELETE_ACCOUNT,
	PD::AC::ACPacketId::AC_ACK_PRE_LOGIN,
	PD::AC::ACPacketId::AC_ACK_IDP_CONFLICT_PLYER);

constexpr PD::GC::GCPacketId startGCPacket = static_cast<PD::GC::GCPacketId>(8000);
constexpr PD::GC::GCPacketId lastGCPacket = static_cast<PD::GC::GCPacketId>(static_cast<int>(PD::GC::GCPacketId::PacketIdMax) - 1);
ENUM_RANGE_BY_FIRST_AND_LAST(PD::GC::GCPacketId, startGCPacket, lastGCPacket)

constexpr PD::SC::SCPacketId startSCPacket = static_cast<PD::SC::SCPacketId>(static_cast<int>(PD::GC::GCPacketId::PacketIdMax) + 1);
constexpr PD::SC::SCPacketId lastSCPacket = static_cast<PD::SC::SCPacketId>(static_cast<int>(PD::SC::SCPacketId::PacketIdMax) - 1);
ENUM_RANGE_BY_FIRST_AND_LAST(PD::SC::SCPacketId, startSCPacket, lastSCPacket)

void FGsNetErrorHandler::Initialize(FGsNetManager* inManager)
{
	//LoadTables();
}

void FGsNetErrorHandler::Finalize()
{
}

bool FGsNetErrorHandler::IsACPacket(int32 nPacketId)
{
	return !IsSCPacket(nPacketId) && !IsGCPacket(nPacketId);
}

bool FGsNetErrorHandler::IsGCPacket(int32 nPacketId)
{
	static int lastAC = static_cast<int32>(PD::AC::ACPacketId::PacketIdMax);
	return nPacketId > lastAC && nPacketId <= static_cast<int32>(lastGCPacket);
}

bool FGsNetErrorHandler::IsSCPacket(int32 nPacketId)
{
	static int lastGC = static_cast<int32>(PD::GC::GCPacketId::PacketIdMax);
	return nPacketId > lastGC;
}

void FGsNetErrorHandler::LoadTables()
{
	static TMap<FString, uint16> packetMaps;

	if (packetMaps.Num() <= 0)
	{
		TEnumRange<PD::AC::ACPacketId> acPacketIds;
		for (auto id : acPacketIds)
		{
			auto index = static_cast<uint16>(id);
			packetMaps.Add(GetPdlString(index), index);
		}

		TEnumRange<PD::GC::GCPacketId> gcPacketIds;
		for (auto id : gcPacketIds)
		{
			auto index = static_cast<uint16>(id);
			packetMaps.Add(GetPdlString(index), index);
		}

		TEnumRange<PD::SC::SCPacketId> scPacketIds;
		for (auto id : scPacketIds)
		{
			auto index = static_cast<uint16>(id);
			packetMaps.Add(GetPdlString(index), index);
		}
	}

	if (auto uiTable = Cast<UGsTableNetErrorUI>(FGsSchemaNetErrorUI::GetStaticTable()))
	{
		TArray<const FGsSchemaNetErrorUI*> netErrorUISchemas;
		if (uiTable->GetAllRows<FGsSchemaNetErrorUI>(netErrorUISchemas))
		{
			for (const FGsSchemaNetErrorUI* row : netErrorUISchemas)
			{
				auto index = _resultProcs.Add(MakeShared<FGsNetErrorProc>(row));
				_resultIdMap.Add(row->id, _resultProcs[index]);
				_resultErrorList.FindOrAdd(row->result).Add(_resultProcs[index]);
			}
		}
	}

	if (auto procTable = Cast<UGsTableNetErrorProcess>(FGsSchemaNetErrorProcess::GetStaticTable()))
	{
		TArray<const FGsSchemaNetErrorProcess*> netErrorSchemas;
		if (procTable->GetAllRows<FGsSchemaNetErrorProcess>(netErrorSchemas))
		{
			for (const FGsSchemaNetErrorProcess* row : netErrorSchemas)
			{
				uint16 packetId = packetMaps.Find(row->packetId) != nullptr ? packetMaps[row->packetId] : 0;

				TSharedPtr<FGsNetErrorProc> useProc = nullptr;

				if (row->useId.IsNull() || row->useId.GetRow() == nullptr)
				{
					if (packetId != 0)
					{
						_resultPacketError.FindOrAdd(packetId).Emplace(0, MakeShared<FGsNetErrorProc>(nullptr, row));
					}
				}
				else if (auto iter = _resultIdMap.Find(row->useId.GetRow()->id))
				{
					useProc = iter->Pin();
					useProc->SetPacketData(packetId, row);

					if (packetId == 0)
					{
						useProc->_process = row->process;
					}
					else
					{
						_resultPacketError.FindOrAdd(packetId).Emplace(useProc->_result, useProc);
					}
				}
				else continue;
			}
		}
	}

	for (auto& e : _resultErrorList)
	{
		e.Value.StableSort([](auto pe, auto fe) { 
			return pe.Pin()->_isDefault && !fe.Pin()->_isDefault;
		});
	}
}

int FGsNetErrorHandler::ProcessErrorUI(TWeakPtr<FGsNetErrorProc> weakData)
{
	if (auto data = weakData.Pin())
	{
		switch (data->GetErrorUIType())
		{
		case EGsNetErrorProcUIType::None:
			break;
		case EGsNetErrorProcUIType::SystemTicker: 
			FGsUIHelper::TraySystemTicker(data->GetErrorText());
			break;
		case EGsNetErrorProcUIType::SystemPopup:
			FGsUIHelper::PopupSystemMsg(data->GetErrorText());
			break;
		case EGsNetErrorProcUIType::CodeSystemPopup:
			{
				FString message = data->GetErrorText().ToString();

#if	!UE_BUILD_SHIPPING
				FString strResult = PD::ResultEnumToString(static_cast<PD::Result>(data->GetResult()));
				message = FString::Format(TEXT("Code({0})\n{1}"),
					{ strResult.IsEmpty() ? FString::FromInt(data->GetResult()) : strResult, data->GetErrorText().ToString() });
#endif
				FGsUIHelper::PopupSystemMsg(FText::FromString(message));
			}
			break;
		case EGsNetErrorProcUIType::SectionTicker:
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, data->GetErrorText());
			break;
		case EGsNetErrorProcUIType::CustomPopup:
			{
				FString key = data->GetPopupKey();
				int option = data->GetPopupOption();
				if (auto popup = Cast<UGsUICustomErrorPopup>(GUI()->OpenAndGetWidget(*key).Get()))
				{
					popup->SetErrorHandlerData(data->GetErrorText(), data->GetPopupOption(), data->GetResult());
				}
				else return -1;
			}
			break;
		default:
			break;
		}
	}

	return 0;
}

bool FGsNetErrorHandler::IsCheckEnablePacket(int32 nPacketId, bool defaultHandle)
{
	bool isGCPacket = IsGCPacket(nPacketId);
	bool isSCPacket = IsSCPacket(nPacketId);
	bool isACPacket = !isSCPacket && !isGCPacket;

	return  ((isACPacket && _ackACPackets.Find(nPacketId)) ||
			 (isGCPacket && _ackGCPackets.Find(nPacketId)) ||
			 (isSCPacket && _ackSCPackets.Find(nPacketId)) ||
			 (defaultHandle && nPacketId == 0));
}

bool FGsNetErrorHandler::AddCheckPacket(int32 nPacketId)
{
	bool isGCPacket = IsGCPacket(nPacketId);
	bool isSCPacket = IsSCPacket(nPacketId);
	bool isACPacket = !isSCPacket && !isGCPacket;

	if (isACPacket) _ackACPackets.Emplace(nPacketId);
	else if (isGCPacket) _ackGCPackets.Emplace(nPacketId);
	else if (isSCPacket) _ackSCPackets.Emplace(nPacketId);
	else return false;

	return true;
}

int32 FGsNetErrorHandler::ProcessPacket(uint16 packetId, int32 result, uint8& typeErrorUI, uint8& typeErrorProc, FText& errorText, bool bShow /*= false*/)
{
	typeErrorUI = static_cast<uint8>(EGsNetErrorProcUIType::None);
	typeErrorProc = static_cast<uint8>(EGsNetErrorProcType::None);
	errorText = FText::GetEmpty();

	if (result == PACKET_RESULT_SUCCESS || IsCheckEnablePacket(packetId, true) == false)
	{	
		return 0;
	}

	if (_resultErrorList.Num() <= 0)
	{
		LoadTables();
	}

	if (auto iter = _resultPacketError.Find(packetId))
	{
		if (auto resIter = (*iter).Find(result))
		{
			auto eProcessType = resIter->Pin()->GetProcessType(packetId);
			typeErrorUI		= static_cast<uint8>(resIter->Pin()->GetErrorUIType());
			typeErrorProc	= static_cast<uint8>(eProcessType);
			errorText		= resIter->Pin()->GetErrorText();

			if(!errorText.IsEmpty()) 
			{ 
				switch (eProcessType)
				{
					case EGsNetErrorProcType::NoUI: return 0;
					case EGsNetErrorProcType::Process:
					{
						ProcessErrorUI(*resIter);
						return 0;
					}
					case EGsNetErrorProcType::NoProc:
					{
						FGsUIHelper::HideBlockUI();

						ProcessErrorUI(*resIter);
						return 1;
					}
					break;
					default: break;
				}
			}
		}
	}

	if (auto list = _resultErrorList.Find(result))
	{
		if (list->IsValidIndex(0))
		{
			auto data = (*list)[0];
			auto eProcessType = data.Pin()->GetProcessType(0);

			typeErrorUI = static_cast<uint8>(data.Pin()->GetErrorUIType());
			typeErrorProc = static_cast<uint8>(eProcessType);
			errorText	= data.Pin()->GetErrorText();

			if (!errorText.IsEmpty())
			{
				switch (eProcessType)
				{
					case EGsNetErrorProcType::NoUI: return 0;
					case EGsNetErrorProcType::Process:
					{
						ProcessErrorUI(data);
						return 0;
					}
					case EGsNetErrorProcType::NoProc:
					{
						FGsUIHelper::HideBlockUI();

						ProcessErrorUI(data);
						return 1;
					}
					default: break;
				}
			}
		}
	}

	FString key = PD::ResultEnumToString(static_cast<PD::Result>(result));
	if (errorText.IsEmpty())
	{	
		FText::FindText(TEXT("NetText"), *key, errorText);
	}

	if (errorText.IsEmpty())
	{
		if (CommonMessage.IsEmpty() && CommonMessageKey.IsEmpty() == false)
		{
			FText::FindText(TEXT("LauncherNetText"), *CommonMessageKey, CommonMessage);
		}

		errorText = FText::Format(CommonMessage, key.IsEmpty() ? FText::AsNumber(result) : FText::FromString(key));

		if (errorText.IsEmpty())
		{
			errorText = key.IsEmpty() ? FText::AsNumber(result) : FText::FromString(key);
		}
	}

	if(errorText.IsEmpty() == false)
	{
		FGsUIHelper::HideBlockUI();

		typeErrorUI = static_cast<uint8>(EGsNetErrorProcUIType::CodeSystemPopup);
		typeErrorProc = static_cast<uint8>(EGsNetErrorProcType::DefaultProc);

		FGsUIHelper::PopupSystemMsg(errorText);
		return 1;
	}

	return 0;
}