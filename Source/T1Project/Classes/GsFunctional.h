#pragma once

//#include "Algo/Accumulate.h"
//#include "Algo/AllOf.h"
//#include "Algo/AnyOf.h"
//#include "Algo/BinarySearch.h"
//#include "Algo/Compare.h"
//#include "Algo/Copy.h"
//#include "Algo/Count.h"
//#include "Algo/Find.h"
//#include "Algo/FindLast.h"
//#include "Algo/FindSequence.h"
//#include "Algo/ForEach.h"
//#include "Algo/Heapify.h"
//#include "Algo/HeapSort.h"
//#include "Algo/IndexOf.h"
//#include "Algo/IntroSort.h"
//#include "Algo/IsHeap.h"
//#include "Algo/IsSorted.h"
//#include "Algo/LegacySort.h"
//#include "Algo/MaxElement.h"
//#include "Algo/MinElement.h"
//#include "Algo/NoneOf.h"
//#include "Algo/Partition.h"
//#include "Algo/RandomShuffle.h"
//#include "Algo/RemoveIf.h"
//#include "Algo/Replace.h"
//#include "Algo/Reverse.h"
//#include "Algo/Rotate.h"
//#include "Algo/SelectRandomWeighted.h"
//#include "Algo/Sort.h"
//#include "Algo/StableSort.h"
//#include "Algo/Transform.h"
//#include "Algo/Unique.h"

//#include "GsSchemaBase.h"
//#include "Util/GsTableUtil.h"
//
//template<typename TableType, typename SchemaType>
//struct TSchemaIdFunctor
//{
//	static_assert(TIsDerivedFrom<SchemaType, FGsSchemaBase>, "SchemaType should be derived from FGsSchemaBase");
//	SchemaType* operator()(const int32 InId)
//	{
//		const SchemaType* schemaPtr = UGsTableUtil::FindRowById<TableType, SchemaType, int32>(InId);
//		return schemaPtr;
//	}
//};

//#pragma region PacketHelper
//
//class GsPacketHelper
//{
//public:
//	template<typename PacketType, typename BufferType = unsigned char>
//	static PacketType* ConvertType(BufferType* InBuffer)
//	{
//		PacketType* packet = reinterpret_cast<PacketType*>(InBuffer);
//		if (nullptr == packet)
//		{
//			GSLOG(Error, TEXT("nullptr == packet"));
//			return nullptr;
//		}
//
//		return packet;
//	}
//
//	template<typename PacketType>
//	static bool IsResultSuccess(PacketType* InPacket)
//	{
//		PD::Result result = StaticCast<PD::Result>(InPacket->Result());
//		if (result != PACKET_RESULT_SUCCESS)
//		{
//			FGsUIHelper::TraySectionMessageTickerNetError(StaticCast<PD::Result>(InPacket->Result()));
//			return false;
//		}
//
//		return true;
//	}
//};
//
//#pragma endregion PacketHelper

namespace Algo
{




}