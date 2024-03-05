#include "GsSealManager.h"

#include "Currency/GsCostPackage.h"
#include "DataSchema/Seal/GsSchemaSealNode.h"
#include "DataSchema/Seal/GsSchemaSealNodeLevel.h"
#include "DataSchema/Seal/GsSchemaSealNodeLevelGainStat.h"
#include "DataSchema/Seal/GsSchemaSealNodeLevelUpSound.h"
#include "DataSchema/Seal/GsSchemaSealNodeRerollCost.h"
#include "DataSchema/Seal/GsSchemaSealNodeRerollGain.h"
#include "DataSchema/Seal/GsSchemaSealPack.h"
#include "DataSchema/Seal/GsSchemaSealPackGainColor.h"
#include "DataSchema/Seal/GsSchemaSealSlot.h"
#include "DataSchema/Seal/GsSchemaSealSlotRerollGainColorPool.h"
#include "DataSchema/Seal/GsSchemaSealSlotRerollGainStatPool.h"
#include "DataSchema/Sound/GsSchemaSoundResData.h"
#include "GsTable.h"
#include "Item/GsItemHelper.h"
#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsSealMessageParam.h"
#include "Net/MessageHandler/GsNetMessageHandlerSeal.h"
#include "Seal/GsSealNode.h"
#include "Seal/GsSealPack.h"
#include "Seal/GsSealSlot.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "T1Project.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

void FGsSealManager::Initialize()
{
	if (!InitliazeData())
	{
		GSLOG(Error, TEXT("!InitliazeData()"));
		return;
	}

	RegisterMessages();
}

void FGsSealManager::Finalize()
{
	UnegisterMessages();
}

bool FGsSealManager::InitliazeData()
{
	const UGsTable* SealNodeTable = FGsSchemaSealNode::GetStaticTable();
	if (nullptr == SealNodeTable)
	{
		GSLOG(Error, TEXT("nullptr == SealNodeTable"));
	}
	else
	{
		TArray<const FGsSchemaSealNode*> SealNodeDatas;
		if (!SealNodeTable->GetAllRows(SealNodeDatas))
		{
			GSLOG(Error, TEXT("!SealNodeTable->GetAllRows(SealNodeDatas)"));
		}
		else
		{
			for (const FGsSchemaSealNode* SealNodeData : SealNodeDatas)
			{
				if (nullptr == SealNodeData)
				{
					GSLOG(Error, TEXT("nullptr == SealNodeData"));
					continue;
				}

				_sealNodeMap.Emplace(SealNodeData->id, MakeShared<FGsSealNode>(SealNodeData));
			}
		}
	}

	const UGsTable* SealPackTable = FGsSchemaSealPack::GetStaticTable();
	if (nullptr == SealPackTable)
	{
		GSLOG(Error, TEXT("nullptr == SealPackTable"));
		return false;
	}

	TArray<const FGsSchemaSealPack*> SealPacks;
	if (!SealPackTable->GetAllRows(SealPacks))
	{
		GSLOG(Error, TEXT("!SealPackTable->GetAllRows(SealPacks)"));
		return false;
	}

	_sealPackMap.Empty();
	_sealPackList.Empty();
	for (const FGsSchemaSealPack* SealPack : SealPacks)
	{
		if (nullptr == SealPack)
		{
			GSLOG(Error, TEXT("nullptr == SealPack"));
			return false;
		}

		SealPackId PackId = StaticCast<SealPackId>(SealPack->id);
		_sealPackMap.Emplace(PackId, MakeShared<FGsSealPack>(SealPack));
		_sealPackList.Emplace(PackId);

		if (const auto UnlockContentsRow = SealPack->unlockContentsId.GetRow())
			_unlockIdToSealPackId.Emplace(UnlockContentsRow->id, PackId);
	}

	return true;
}

void FGsSealManager::RegisterMessages()
{
	FGsMessageHolder* MessageHolder = GMessage();
	if (nullptr == MessageHolder)
	{
		GSLOG(Error, TEXT("nullptr == MessageHolder"));
		return;
	}

	_sealDelegates.Emplace(MessageHolder->GetSeal().AddRaw(FGsSealMessageParamTrySetRerollSlotUnlock::MESSAGE_ID, this, &FGsSealManager::TrySetRerollSealSlotUnlock));
}

void FGsSealManager::UnegisterMessages()
{
	FGsMessageHolder* MessageHolder = GMessage();
	if (nullptr == MessageHolder)
	{
		GSLOG(Error, TEXT("nullptr == MessageHolder"));
		return;
	}

	for (int32 i = _sealDelegates.Num() - 1; 0 <= i; --i)
	{
		MessageHolder->GetSeal().Remove(_sealDelegates[i]);
	}
	_sealDelegates.Empty();
}

TWeakPtr<const FGsSealPack> FGsSealManager::GetSealPack(const SealPackId InSealPackId) const
{
	const TSharedRef<FGsSealPack>* SealPack = _sealPackMap.Find(InSealPackId);
	if (nullptr == SealPack)
	{
		GSLOG(Error, TEXT("nullptr == SealPack, InSealPackId:%d"), InSealPackId);
		return nullptr;
	}

	return *SealPack;
}

TWeakPtr<const FGsSealNode> FGsSealManager::GetSealNode(const SealId InSealNodeId) const
{
	TWeakPtr<const FGsSealNode> SealNode;

	const TSharedRef<FGsSealNode>* SealNodePtr = _sealNodeMap.Find(InSealNodeId);
	if (nullptr == SealNodePtr)
	{
		GSLOG(Error, TEXT("nullptr == SealNodePtr"));
	}
	else
	{
		SealNode = *SealNodePtr;
	}

	return SealNode;
}

TWeakPtr<const FGsSealPack> FGsSealManager::GetSealPackWithIndex(const uint8 InIndex) const
{
	if (!_sealPackList.IsValidIndex(InIndex))
	{
		GSLOG(Error, TEXT("!_sealPackList.IsValidIndex(%d)"), InIndex);
		return nullptr;
	}

	return GetSealPack(_sealPackList[InIndex]);
}

int32 FGsSealManager::GetSealPackIndexWithId(const SealPackId InSealPackId) const
{
	return _sealPackList.Find(InSealPackId);
}

int32 FGsSealManager::GetTotalSealPackCount() const
{
	return _sealPackList.Num();
}

bool FGsSealManager::CanDisplay(const SealPackId InSealPackId) const
{
	TWeakPtr<const FGsSealPack> SealPack = GetSealPack(InSealPackId);
	if (!SealPack.IsValid())
	{
		GSLOG(Error, TEXT("!SealPack.IsValid(), InSealPackId:%d"), InSealPackId);
		return false;
	}

	return SealPack.Pin()->CanDisplay();
}

bool FGsSealManager::CanSealNow(const SealId InSealNodeId) const
{
	return false;
}

void FGsSealManager::Seal(const IGsMessageParam* InParam)
{
	const FGsSealMessageParamSeal* Param = InParam->Cast<const FGsSealMessageParamSeal>();
	if (nullptr == Param)
	{
		GSLOG(Error, TEXT("nullptr == Param"));
		return;
	}

	for (SealPacketData PacketData : Param->_sealPacketData)
	{
		SealPackId PackId = PacketData._sealPackId;

		if (INVALID_SEAL_PACK_ID == PackId)
		{
			GSLOG(Error, TEXT("INVALID_SEAL_PACK_ID == PackId"));
			return;
		}

		TSharedRef<FGsSealPack>* SealPackPtr = _sealPackMap.Find(PackId);
		if (nullptr == SealPackPtr)
		{
			GSLOG(Error, TEXT("nullptr == SealPackPtr"));
			return;
		}

		TSharedRef<FGsSealPack> SealPack = *SealPackPtr;
		SealPack->SetSealBunusDatas(PacketData._sealBonusDatas);
		SetSealNodeDatas(PacketData._sealDatas);
		SetSealSlotDatas(PacketData._sealSlotDatas);
		SealPack->UpdateSealLines();
	}
}

void FGsSealManager::SealSlotLock(const IGsMessageParam* InParam)
{
	const FGsSealMessageParamSealSlotLock* Param = InParam->Cast<const FGsSealMessageParamSealSlotLock>();
	if (nullptr == Param)
	{
		GSLOG(Error, TEXT("nullptr == Param"));
		return;
	}

	for (const auto SlotLock : Param->_sealSlotLockList)
	{
		SealSlotId Id = SlotLock._sealSlotId;
		_sealSlotMap[Id]->SetRerollUnlock(EGsSealRerollUnlockTarget::Stat, !SlotLock._isStatLock);
		_sealSlotMap[Id]->SetRerollUnlock(EGsSealRerollUnlockTarget::Color, !SlotLock._isStatLock);
	}
}

void FGsSealManager::AckSealLevelUp(const IGsMessageParam* InParam)
{
	const FGsSealMessageParamAckSealLevelUp* Param = InParam->Cast<const FGsSealMessageParamAckSealLevelUp>();
	if (nullptr == Param)
	{
		GSLOG(Error, TEXT("nullptr == Param"));
		return;
	}

	if (!Param->_packetNoError)
	{
		GSLOG(Warning, TEXT("!Param->_packetNoError"));
		return;
	}

	TSharedRef<FGsSealPack>* SealPackPtr = _sealPackMap.Find(Param->_sealPackId);
	if (nullptr == SealPackPtr)
	{
		GSLOG(Error, TEXT("nullptr == SealPackPtr"));
		return;
	}

	TSharedRef<FGsSealPack> SealPack = *SealPackPtr;

	{
		// 경험치 변화량 확인
		const Exp ExpDiff = SealPack->GetSealNodeExpDiff(Param->_sealNode);
		FText TickerTxt;
		if (FText::FindText(TEXT("SealText"), TEXT("UI_Ticker_ExpGain"), TickerTxt))
		{
			FGsUIHelper::TrayMessageTicker(FText::Format(TickerTxt, ExpDiff));

			const UGsTableSealNodeLevelUpSound* SealNodeLevelUpSoundTable = Cast<UGsTableSealNodeLevelUpSound>(FGsSchemaSealNodeLevelUpSound::GetStaticTable());
			TArray<const FGsSchemaSealNodeLevelUpSound*> AllRows;
			if (nullptr != SealNodeLevelUpSoundTable && SealNodeLevelUpSoundTable->GetAllRows(AllRows))
			{
				for (const FGsSchemaSealNodeLevelUpSound* Row : AllRows)
				{
					if (nullptr != Row && ExpDiff == Row->expDiff)
					{
						FGsUIHelper::Play2DSound(Row->soundResDataRow.GetRow());
						break;
					}
				}
			}
		}
	}

	{
		// 레벨 변화량 확인
		const int32 LevelDiff = SealPack->GetSealNodeLevelDiff(Param->_sealNode);
	}

	SetSealNodeData(Param->_sealNode);
}

void FGsSealManager::AckSealSlotChangeAll(const IGsMessageParam* InParam)
{
	const FGsSealMessageParamAckSealSlotChangeAll* Param = InParam->Cast<const FGsSealMessageParamAckSealSlotChangeAll>();
	if (nullptr == Param)
	{
		GSLOG(Error, TEXT("nullptr == Param"));
		return;
	}

	if (!Param->_packetNoError)
	{
		GSLOG(Warning, TEXT("!Param->_packetNoError"));
		return;
	}

	TSharedRef<FGsSealPack>* SealPackPtr = _sealPackMap.Find(Param->_sealPackId);
	if (nullptr == SealPackPtr)
	{
		GSLOG(Error, TEXT("nullptr == SealPackPtr"));
		return;
	}

	TSharedRef<FGsSealPack> SealPack = *SealPackPtr;
	SealPack->SetSealBunusDatas(Param->_sealBonusDatas);
	SetSealSlotDatas(Param->_sealSlotDatas);
	SealPack->UpdateSealLines();
}

void FGsSealManager::AckSealSlotLockSave(const IGsMessageParam* InParam)
{
	// 매니저 단에서 특별히 할 일은 없다. 다만 잠금 상태 저장이 실패하면 클라를 다시 로비로 보낸다.
	// FGsNetMessageHandlerSeal::ACK_SEAL_SLOT_LOCK_SAVE 이후에 필요한 처리가 있으면 여기에서 처리할 것.
}

void FGsSealManager::TrySetRerollSealSlotUnlock(const IGsMessageParam* InParam)
{
	const FGsSealMessageParamTrySetRerollSlotUnlock* Param = InParam->Cast<const FGsSealMessageParamTrySetRerollSlotUnlock>();
	if (nullptr == Param)
	{
		GSLOG(Error, TEXT("nullptr == Param"));
		return;
	}

	TSharedRef<FGsSealNode>* SealNode = _sealNodeMap.Find(Param->_sealNodeId);
	if (nullptr == SealNode)
	{
		GSLOG(Error, TEXT("nullptr == SealNode"));
		return;
	}

	(*SealNode)->SetRerollUnlock(Param->_sealSlotId, Param->_unlockTarget, Param->_sealSlotUnlock);
}

bool FGsSealManager::OnUnlockCategory(UnlockContentsId InContentsId)
{
	if (!_unlockIdToSealPackId.Contains(InContentsId))
	{
		GSLOG(Error, TEXT("!_unlockIdToSealPackId.Contains(InContentsId)"));
		return false;
	}

	SealPackId PackId = _unlockIdToSealPackId[InContentsId];

	bool isChanged = false;

	if (_sealPackMap.Contains(PackId))
	{
		TSharedRef<FGsSealPack> SealPack = _sealPackMap[PackId];
		EGsSealState CurrentState = SealPack->GetSealState();

		// 만약 잠겨 있었다면, 활성 상태로 바꾸어 준다
		if (CurrentState == EGsSealState::Unknown || CurrentState == EGsSealState::Deactivated)
		{
			SealPack->SetSealState(EGsSealState::Activated);
			isChanged = true;
		}
	}

	return isChanged;
}

void FGsSealManager::SetSealNodeDatas(const TArray<SealData>& InSealNodeDatas)
{
	for (const SealData& CurrentSealData : InSealNodeDatas)
	{
		SetSealNodeData(CurrentSealData);
	}
}

void FGsSealManager::SetSealNodeData(const SealData& InSealNodeData)
{
	const SealId SealNodeId = InSealNodeData.mSealId;

	TSharedRef<FGsSealNode>* SealNode = _sealNodeMap.Find(InSealNodeData.mSealId);
	if (nullptr == SealNode)
	{
		GSLOG(Error, TEXT("Failed to find SealNode. id: %d"), InSealNodeData.mSealId);
		return;
	}

	(*SealNode)->SetSealData(InSealNodeData);
}

void FGsSealManager::SetSealSlotDatas(const TArray<SealSlotData>& InSealSlotDatas)
{
	for (const SealSlotData& CurrentSealSlotData : InSealSlotDatas)
	{
		TSharedRef<FGsSealNode>* SealNode = _sealNodeMap.Find(CurrentSealSlotData.mSealId);
		if (nullptr == SealNode)
		{
			GSLOG(Error, TEXT("Failed to find SealNode. id: %d"), CurrentSealSlotData.mSealId);
			return;
		}

		(*SealNode)->SetSealSlotDatas(CurrentSealSlotData);
	}
}

void FGsSealManager::TempResetAllSealNodeSelections()
{
	// 리롤 잠금 서버 저장 기능 개발 전에 임시로 모든 선택 상태를 리셋하는 기능이 필요하여 추가함.
	for (const TPair<SealId, TSharedRef<FGsSealNode>>& SealNodePair : _sealNodeMap)
	{
		TSharedRef<FGsSealNode> SealNode = SealNodePair.Value;

		const int32 SealSlotCount = SealNode->GetSealSlotCount();
		for (int32 i = SealSlotCount - 1; 0 <= i; --i)
		{
			TWeakPtr<const FGsSealSlot> SealSlot = SealNode->GetSealSlotByIndex(i);
			if (!SealSlot.IsValid())
			{
				GSLOG(Error, TEXT("!SealSlot.IsValid()"));
				continue;
			}

			const SealSlotId CurrentSealSlotId = SealSlot.Pin()->GetSealSlotId();
			SealNode->SetRerollUnlock(CurrentSealSlotId, EGsSealRerollUnlockTarget::Stat, true);
			SealNode->SetRerollUnlock(CurrentSealSlotId, EGsSealRerollUnlockTarget::Color, true);
		}
	}
}

SealPackId FGsSealManager::GetDefaultSealPackId()
{
	static SealPackId DefaultSealPackId = -1;

	if (-1 == DefaultSealPackId)
	{
		const UGsTableSealPack* SealPackTable = Cast<UGsTableSealPack>(FGsSchemaSealPack::GetStaticTable());
		TArray<const FGsSchemaSealPack*> SealPacks;
		if (!SealPackTable->GetAllRows(SealPacks) || 0 == SealPacks.Num())
		{
			GSLOG(Error, TEXT("!SealPackTable->GetAllRows(SealPacks) || 0 == SealPacks.Num()(%d)"), SealPacks.Num());
		}
		DefaultSealPackId = StaticCast<SealPackId>((*SealPacks.CreateConstIterator())->id);
	}

	return DefaultSealPackId;
}

FText FGsSealManager::MakeSealGradeText(const SealSlotColor InSealSlotColor)
{
	FString KeyText;
	switch (InSealSlotColor)
	{
	case SealSlotColor::NONE: KeyText = TEXT("UI_Slot_Popup_ColorNone"); break;
	case SealSlotColor::RARE: KeyText = TEXT("UI_Slot_Popup_ColorRare"); break;
	case SealSlotColor::EPIC: KeyText = TEXT("UI_Slot_Popup_ColorEpic"); break;
	case SealSlotColor::UNIQUE: KeyText = TEXT("UI_Slot_Popup_ColorUnique"); break;
	}

	FText SealGradeText;
	FText::FindText(TEXT("SealText"), KeyText, SealGradeText);
	return SealGradeText;
}

bool SortInitSealNodes(const TWeakPtr<FGsSealNode>& InA, const TWeakPtr<FGsSealNode>& InB)
{
	if (!InA.IsValid() || !InB.IsValid())
	{
		return false;
	}

	return InA.Pin()->GetSealNodeId() < InB.Pin()->GetSealNodeId();
}

TArray<TWeakPtr<FGsSealNode>> FGsSealManager::MakeInitSealNodes(const SealPackId InSealPackId) const
{
	TArray<TWeakPtr<FGsSealNode>> SealNodes;
	for (const TPair<SealId, TSharedRef<FGsSealNode>>& SealNodePair : _sealNodeMap)
	{
		const TSharedRef<FGsSealNode>& SealNode = SealNodePair.Value;
		if (nullptr == SealNode->_data)
		{
			GSLOG(Error, TEXT("nullptr == SealNode->_data"));
			continue;
		}

		if (InSealPackId == SealNode->_data->sealPackId)
		{
			SealNodes.Emplace(SealNode);
		}
	}

	SealNodes.Sort(&SortInitSealNodes);
	return MoveTemp(SealNodes);
}

const TMap<FGsSealManager::MatchCount, TArray<TPair<StatType, StatValue>>>* FGsSealManager::GetSealPackAllGainsInColor(FGsSealManager::SealPackGainColorId InSealPackGainColorId, SealSlotColor InLineColor)
{
	if (_collectedSealPackGainStatMap.Num() == 0)
	{
		_collectedSealPackGainStatMap = FGsSealManager::CollectSealPackGainStats();
	}

	const TMap<SealSlotColor, TMap<MatchCount, TArray<TPair<StatType, StatValue>>>>* ColorHas = _collectedSealPackGainStatMap.Find(InSealPackGainColorId);
	if (nullptr == ColorHas)
	{
		return nullptr;
	}

	return ColorHas->Find(InLineColor);
}

const TArray<TPair<StatType, StatValue>>* FGsSealManager::GetSealPackGainStats(FGsSealManager::SealPackGainColorId InSealPackGainColorId, SealSlotColor InLineColor, FGsSealManager::MatchCount InLineCount)
{
	if (_collectedSealPackGainStatMap.Num() == 0)
	{
		_collectedSealPackGainStatMap = FGsSealManager::CollectSealPackGainStats();
	}

	const TMap<SealSlotColor, TMap<MatchCount, TArray<TPair<StatType, StatValue>>>>* SealPackHas = _collectedSealPackGainStatMap.Find(InSealPackGainColorId);
	if (nullptr == SealPackHas)
	{
		GSLOG(Warning, TEXT("nullptr == SealPackHas"));
		return nullptr;
	}

	const TMap<MatchCount, TArray<TPair<StatType, StatValue>>>* SealPackColorHas = SealPackHas->Find(InLineColor);
	if (nullptr == SealPackColorHas)
	{
		GSLOG(Warning, TEXT("nullptr == SealPackColorHas"));
		return nullptr;
	}

	const TArray<TPair<StatType, StatValue>>* SealPackColorCountHas = SealPackColorHas->Find(InLineCount);
	if (nullptr == SealPackColorCountHas)
	{
		GSLOG(Warning, TEXT("nullptr == SealPackColorCountHas"));
		return nullptr;
	}

	return SealPackColorCountHas;
}

void FGsSealManager::SetSealSlotData(const SealSlotId InSlotId, TSharedRef<FGsSealSlot> InSealSlot)
{
	_sealSlotMap.Emplace(InSlotId, InSealSlot);
}

TSharedRef<FGsSealSlot> FGsSealManager::GetSealSlotData(const SealSlotId InSlotId)
{
	if (!_sealSlotMap.Contains(InSlotId))
	{
		GSLOG(Error, TEXT("No valid SealSlot for id : %d!!!"), InSlotId);
		check(false);
	}

	return _sealSlotMap[InSlotId];
}

const FGsSchemaSealNodeLevel* FGsSealManager::FindSealNodeLevel(int32 InSealNodeLevelId, Level InSealNodeLevel)
{
	const UGsTable* SealNodeLevelTable = FGsSchemaSealNodeLevel::GetStaticTable();
	TArray<const FGsSchemaSealNodeLevel*> SealNodeLevelDatas;
	if (nullptr == SealNodeLevelTable || !SealNodeLevelTable->GetAllRows(SealNodeLevelDatas))
	{
		GSLOG(Error, TEXT("nullptr == SealNodeLevelTable || !SealNodeLevelTable->GetAllRows(SealNodeLevelDatas)"));
		return nullptr;
	}

	for (const FGsSchemaSealNodeLevel* SealNodeLevelData : SealNodeLevelDatas)
	{
		if (nullptr == SealNodeLevelData)
		{
			GSLOG(Error, TEXT("nullptr == SealNodeLevelData"));
			continue;
		}

		if (SealNodeLevelData->sealNodeLevelId == InSealNodeLevelId && SealNodeLevelData->sealNodeLevel == InSealNodeLevel)
		{
			return SealNodeLevelData;
		}
	}

	return nullptr;
}

TMap<Level, TArray<TPair<StatType, StatValue>>> FGsSealManager::CollectSealNodeStatInfoAllLevel(int32 InSealNodeLevelId)
{
	TMap<Level, TArray<TPair<StatType, StatValue>>> SealNodeAllLevelStats;

	const UGsTable* SealNodeLevelTable = FGsSchemaSealNodeLevel::GetStaticTable();
	TArray<const FGsSchemaSealNodeLevel*> SealNodeLevelDatas;

	const UGsTable* SealNodeLevelGainStatTable = Cast<UGsTableSealNodeLevelGainStat>(FGsSchemaSealNodeLevelGainStat::GetStaticTable());
	TArray<const FGsSchemaSealNodeLevelGainStat*> SealNodeLevelGainStats;

	if (nullptr == SealNodeLevelTable || !SealNodeLevelTable->GetAllRows(SealNodeLevelDatas) || nullptr == SealNodeLevelGainStatTable || !SealNodeLevelGainStatTable->GetAllRows(SealNodeLevelGainStats))
	{
		GSLOG(Error, TEXT("nullptr == SealNodeLevelTable || !SealNodeLevelTable->GetAllRows(SealNodeLevelDatas) || nullptr == SealNodeLevelGainStatTable || !SealNodeLevelGainStatTable->GetAllRows(SealNodeLevelGainStats)"));
	}
	else
	{
		for (const FGsSchemaSealNodeLevel* SealNodeLevelData : SealNodeLevelDatas)
		{
			if (nullptr == SealNodeLevelData)
			{
				GSLOG(Error, TEXT("nullptr == SealNodeLevelData"));
				continue;
			}

			if (SealNodeLevelData->sealNodeLevelId == InSealNodeLevelId && INVALID_LEVEL != SealNodeLevelData->sealNodeLevel)
			{
				TArray<TPair<StatType, StatValue>>& SealNodeStats = SealNodeAllLevelStats.FindOrAdd(SealNodeLevelData->sealNodeLevel);

				// 캐쉬 필요
				for (const FGsSchemaSealNodeLevelGainStat* SealNodeLevelGainStat : SealNodeLevelGainStats)
				{
					if (nullptr == SealNodeLevelGainStat || SealNodeLevelData->levelGainStatId != SealNodeLevelGainStat->levelGainStatId)
					{
						continue;
					}

					SealNodeStats.Emplace(SealNodeLevelGainStat->statType, StaticCast<StatValue>(SealNodeLevelGainStat->statValue));
				}
			}
		}
	}

	return SealNodeAllLevelStats; //rvo
}

TRange<Level> FGsSealManager::FindSealNodeAllLevelRange(int32 InSealNodeLevelId)
{
	// 최소 레벨 하드캡
	Level MinLevel = TNumericLimits<Level>::Max();
	Level MaxLevel = TNumericLimits<Level>::Min();

	const UGsTable* SealNodeLevelTable = FGsSchemaSealNodeLevel::GetStaticTable();
	TArray<const FGsSchemaSealNodeLevel*> SealNodeLevelDatas;

	// need to cache
	if (nullptr == SealNodeLevelTable || !SealNodeLevelTable->GetAllRows(SealNodeLevelDatas))
	{
		GSLOG(Error, TEXT("nullptr == SealNodeLevelTable || !SealNodeLevelTable->GetAllRows(SealNodeLevelDatas)"));
	}
	else
	{
		for (const FGsSchemaSealNodeLevel* SealNodeLevelData : SealNodeLevelDatas)
		{
			if (nullptr == SealNodeLevelData)
			{
				GSLOG(Error, TEXT("nullptr == SealNodeLevelData"));
				continue;
			}

			const Level CurrentLevel = SealNodeLevelData->sealNodeLevel;
			if (SealNodeLevelData->sealNodeLevelId != InSealNodeLevelId && INVALID_LEVEL == CurrentLevel)
			{
				continue;
			}

			if (MinLevel > CurrentLevel)
			{
				MinLevel = CurrentLevel;
			}

			if (MaxLevel < CurrentLevel)
			{
				MaxLevel = CurrentLevel;
			}
		}
	}

	return TRange<Level>(MinLevel, MaxLevel);
}

TArray<TPair<StatType, StatValue>> FGsSealManager::FindSealNodeLevelGainStats(int32 InSealNodeLevelGainId)
{
	TArray<TPair<StatType, StatValue>> Stats;

	const UGsTable* SealNodeLevelGainStatTable = FGsSchemaSealNodeLevelGainStat::GetStaticTable();
	TArray<const FGsSchemaSealNodeLevelGainStat*> SealNodeLevelGainStatDatas;
	if (nullptr == SealNodeLevelGainStatTable || !SealNodeLevelGainStatTable->GetAllRows(SealNodeLevelGainStatDatas))
	{
		GSLOG(Error, TEXT("nullptr == SealNodeLevelGainStatTable || !SealNodeLevelGainStatTable->GetAllRows(SealNodeLevelGainStatDatas)"));
	}
	else
	{
		for (const FGsSchemaSealNodeLevelGainStat* SealNodeLevelGainStatData : SealNodeLevelGainStatDatas)
		{
			if (nullptr == SealNodeLevelGainStatData)
			{
				GSLOG(Error, TEXT("nullptr == SealNodeRerollCostData"));
				continue;
			}

			if (SealNodeLevelGainStatData->levelGainStatId == InSealNodeLevelGainId)
			{
				Stats.Emplace(SealNodeLevelGainStatData->statType, StaticCast<StatValue>(SealNodeLevelGainStatData->statValue));
			}
		}
	}

	return Stats;
}

//TArray<TPair<SealSlotColor, StatValue>> FGsSealManager::MakeSealNodeRerollColorShare(SealId InSealNodeId, SealSlotId InSealSlotId /*= INVALID_SEAL_SLOT_ID*/, Level InSealNodeLevel /*= INVALID_LEVEL*/)
//{
//	TSet<int32> RerollGainColorPoolIdSet;
//
//	for (const FGsSchemaSealNodeRerollGain* SealNodeRerollGainData : FGsSealManager::CollectSealNodeRerollGainDataSet(InSealNodeId, InSealSlotId, InSealNodeLevel))
//	{
//		if (nullptr == SealNodeRerollGainData)
//		{
//			GSLOG(Error, TEXT("nullptr == SealNodeRerollGainData"));
//			continue;
//		}
//
//		RerollGainColorPoolIdSet.Add(SealNodeRerollGainData->rerollGainColorPoolId);
//	}
//
//	TArray<TPair<SealSlotColor, int32>> SealSlotColorShares;
//	const UGsTable* SealSlotRerollGainColorPoolTable = FGsSchemaSealSlotRerollGainColorPool::GetStaticTable();
//	TArray<const FGsSchemaSealSlotRerollGainColorPool*> SealSlotRerollGainColorPools;
//	if (nullptr == SealSlotRerollGainColorPoolTable || !SealSlotRerollGainColorPoolTable->GetAllRows(SealSlotRerollGainColorPools))
//	{
//		GSLOG(Error, TEXT("nullptr == SealSlotRerollGainColorPoolTable || !SealSlotRerollGainColorPoolTable->GetAllRows(SealSlotRerollGainColorPools)"));
//	}
//	else
//	{
//		for (const FGsSchemaSealSlotRerollGainColorPool* SealSlotRerollGainColorPool : SealSlotRerollGainColorPools)
//		{
//			if (nullptr == SealSlotRerollGainColorPool)
//			{
//				GSLOG(Error, TEXT("nullptr == SealNodeRerollCostData"));
//				continue;
//			}
//
//			if (RerollGainColorPoolIdSet.Contains(SealSlotRerollGainColorPool->rerollGainColorPoolId))
//			{
//				SealSlotColorShares.Emplace(SealSlotRerollGainColorPool->colorType, SealSlotRerollGainColorPool->share);
//			}
//		}
//	}
//	return SealSlotColorShares;
//}
//
//TArray<TTuple<StatType, StatValue, int32>> FGsSealManager::MakeSealNodeRerollStatShare(SealId InSealNodeId, SealSlotId InSealSlotId /*= INVALID_SEAL_SLOT_ID*/, Level InSealNodeLevel /*= INVALID_LEVEL*/)
//{
//	TSet<int32> RerollGainStatPoolIdSet;
//
//	for (const FGsSchemaSealNodeRerollGain* SealNodeRerollGainData : FGsSealManager::CollectSealNodeRerollGainDataSet(InSealNodeId, InSealSlotId, InSealNodeLevel))
//	{
//		if (nullptr == SealNodeRerollGainData)
//		{
//			GSLOG(Error, TEXT("nullptr == SealNodeRerollGainData"));
//			continue;
//		}
//
//		RerollGainStatPoolIdSet.Add(SealNodeRerollGainData->rerollGainStatPoolId);
//	}
//
//	TArray<TTuple<StatType, StatValue, int32>> SealSlotStatShares;
//	const UGsTable* SealSlotRerollGainStatPoolTable = FGsSchemaSealSlotRerollGainStatPool::GetStaticTable();
//	TArray<const FGsSchemaSealSlotRerollGainStatPool*> SealSlotRerollGainStatPools;
//	if (nullptr == SealSlotRerollGainStatPoolTable || !SealSlotRerollGainStatPoolTable->GetAllRows(SealSlotRerollGainStatPools))
//	{
//		GSLOG(Error, TEXT("nullptr == SealSlotRerollGainStatPoolTable || !SealSlotRerollGainStatPoolTable->GetAllRows(SealSlotRerollGainStatPools)"));
//	}
//	else
//	{
//		for (const FGsSchemaSealSlotRerollGainStatPool* SealSlotRerollGainStatPool : SealSlotRerollGainStatPools)
//		{
//			if (nullptr == SealSlotRerollGainStatPool)
//			{
//				GSLOG(Error, TEXT("nullptr == SealNodeRerollCostData"));
//				continue;
//			}
//
//			if (RerollGainStatPoolIdSet.Contains(SealSlotRerollGainStatPool->rerollGainStatPoolId))
//			{
//				SealSlotStatShares.Emplace(SealSlotRerollGainStatPool->statType, StaticCast<StatValue>(SealSlotRerollGainStatPool->statValue), SealSlotRerollGainStatPool->share);
//			}
//		}
//	}
//
//	return SealSlotStatShares; //rvo
//}

TMap<Level, TPair<TArray<TPair<SealSlotColor, int32>>, TArray<TTuple<StatType, StatValue, int32>>>> FGsSealManager::CollectSealRerollGainsSharePerLevel(SealId InSealNodeId)
{
	// 획득풀 수집
	const TMap<int32, TSet<const FGsSchemaSealSlotRerollGainColorPool*>>& RerollGainColorMap = CollectedRerollGainColorMap();
	const TMap<int32, TSet<const FGsSchemaSealSlotRerollGainStatPool*>>& RerollGainStatMap = CollectedRerollGainStatMap();

	TMap<Level, TPair<TArray<TPair<SealSlotColor, int32>>, TArray<TTuple<StatType, StatValue, int32>>>> SealRerollGainsSharePerLevel;

	for (const FGsSchemaSealNodeRerollGain* SealNodeRerollGainData : FGsSealManager::CollectSealNodeRerollGainDataSet(InSealNodeId, INVALID_SEAL_SLOT_ID, INVALID_LEVEL))
	{
		if (nullptr == SealNodeRerollGainData)
		{
			GSLOG(Error, TEXT("nullptr == SealNodeRerollGainData"));
			continue;
		}

		const TSet<const FGsSchemaSealSlotRerollGainColorPool*>* RerollGainColorPools = RerollGainColorMap.Find(SealNodeRerollGainData->rerollGainColorPoolId);
		if (nullptr != RerollGainColorPools)
		{
			for (const FGsSchemaSealSlotRerollGainColorPool* RerollGainColorPool : *RerollGainColorPools)
			{
				check(nullptr != RerollGainColorPool);
				SealRerollGainsSharePerLevel.FindOrAdd(SealNodeRerollGainData->sealNodeLevel).Key.Emplace(RerollGainColorPool->colorType, RerollGainColorPool->share);
			}
		}

		const TSet<const FGsSchemaSealSlotRerollGainStatPool*>* RerollGainStatPools = RerollGainStatMap.Find(SealNodeRerollGainData->rerollGainStatPoolId);
		if (nullptr != RerollGainStatPools)
		{
			for (const FGsSchemaSealSlotRerollGainStatPool* RerollGainStatPool : *RerollGainStatPools)
			{
				check(nullptr != RerollGainStatPool);
				SealRerollGainsSharePerLevel.FindOrAdd(SealNodeRerollGainData->sealNodeLevel).Value.Emplace(MakeTuple(RerollGainStatPool->statType, RerollGainStatPool->statValue, RerollGainStatPool->share));
			}
		}
	}

	return SealRerollGainsSharePerLevel; // rvo
}

TSet<int32> FGsSealManager::CollectAllRerollGainIdSet(SealId InSealNodeId, SealSlotId InSealSlotId /*= INVALID_SEAL_SLOT_ID*/)
{
	// 봉인슬롯ID에 유효한 모든 리롤획득ID 수집
	TSet<int32> RerollGainIdSet;

	const UGsTableSealSlot* SealSlotTable = Cast<UGsTableSealSlot>(FGsSchemaSealSlot::GetStaticTable());
	if (nullptr == SealSlotTable || !SealSlotTable->IsValidLowLevel())
	{
		GSLOG(Error, TEXT("nullptr == SealSlotTable || !SealSlotTable->IsValidLowLevel()"));
	}
	else
	{
		if (InSealSlotId == INVALID_SEAL_SLOT_ID)
		{
			// 봉인슬롯ID를 미지정한 경우, 지정한 봉인노드 내 모든 봉인슬롯의 리롤획득ID를 수집
			TArray<const FGsSchemaSealSlot*> SealSlotDatas;
			if (nullptr == SealSlotTable || !SealSlotTable->GetAllRows(SealSlotDatas))
			{
				GSLOG(Error, TEXT("nullptr == SealSlotTable || !SealSlotTable->GetAllRows(SealSlots)"));
			}
			else
			{
				for (const FGsSchemaSealSlot* SealSlotData : SealSlotDatas)
				{
					if (nullptr == SealSlotData)
					{
						GSLOG(Error, TEXT("nullptr == SealSlotData"));
						continue;
					}

					if (SealSlotData->sealNodeId == InSealNodeId)
					{
						RerollGainIdSet.Add(SealSlotData->rerollGainId);
					}
				}
			}
		}
		else
		{
			// 봉인슬롯ID 를 지정한 경우, 해당 리롤획득ID만 수집
			const FGsSchemaSealSlot* SealSlotData = nullptr;
			if (!SealSlotTable->FindRowById(InSealSlotId, SealSlotData) || nullptr == SealSlotData || SealSlotData->sealNodeId != InSealNodeId)
			{
				GSLOG(Error, TEXT("!SealSlotTable->FindRowById(InSealSlotId) || nullptr == SealSlotData || SealSlotData->sealNodeId != InSealNodeId"));
			}
			else
			{
				RerollGainIdSet.Emplace(SealSlotData->rerollGainId);
			}
		}
	}

	return RerollGainIdSet;
}

TSet<const FGsSchemaSealNodeRerollGain*> FGsSealManager::CollectSealNodeRerollGainDataSet(SealId InSealNodeId, SealSlotId InSealSlotId /*= INVALID_SEAL_SLOT_ID*/, Level InSealNodeLevel /*= INVALID_LEVEL*/)
{
	TSet<const FGsSchemaSealNodeRerollGain*> RerollGainStatPoolIdSet;

	// 봉인슬롯ID에 유효한 모든 리롤획득ID 수집
	const TSet<int32> RerollGainIdSet = FGsSealManager::CollectAllRerollGainIdSet(InSealNodeId, InSealSlotId);

	// 수집한 모든 리롤획득ID에서 봉인노드레벨로 필터링
	const UGsTable* SealNodeRerollGainTable = FGsSchemaSealNodeRerollGain::GetStaticTable();
	TArray<const FGsSchemaSealNodeRerollGain*> SealNodeRerollGainDatas;
	if (nullptr == SealNodeRerollGainTable || !SealNodeRerollGainTable->GetAllRows(SealNodeRerollGainDatas))
	{
		GSLOG(Error, TEXT("nullptr == SealNodeRerollGainTable || !SealNodeRerollGainTable->GetAllRows(SealNodeRerollGainDatas)"));
	}
	else
	{
		const bool IgnoreSealNodeLevel = InSealNodeLevel == INVALID_LEVEL;
		for (const FGsSchemaSealNodeRerollGain* SealNodeRerollGainData : SealNodeRerollGainDatas)
		{
			if (nullptr == SealNodeRerollGainData)
			{
				continue;
			}

			if ((IgnoreSealNodeLevel || SealNodeRerollGainData->sealNodeLevel == InSealNodeLevel) && RerollGainIdSet.Contains(SealNodeRerollGainData->rerollGainId))
			{
				RerollGainStatPoolIdSet.Add(SealNodeRerollGainData);
			}
		}
	}

	return RerollGainStatPoolIdSet; //rvo
}

const TMap<int32, TSet<const FGsSchemaSealSlotRerollGainColorPool*>>& FGsSealManager::CollectedRerollGainColorMap()
{
	if (0 == FGsSealManager::_collectedRerollGainColorMap.Num())
	{
		const UGsTable* SealSlotRerollGainColorPoolTable = FGsSchemaSealSlotRerollGainColorPool::GetStaticTable();
		TArray<const FGsSchemaSealSlotRerollGainColorPool*> SealSlotRerollGainColorPools;
		if (nullptr == SealSlotRerollGainColorPoolTable || !SealSlotRerollGainColorPoolTable->GetAllRows(SealSlotRerollGainColorPools))
		{
			GSLOG(Error, TEXT("nullptr == SealSlotRerollGainColorPoolTable || !SealSlotRerollGainColorPoolTable->GetAllRows(SealSlotRerollGainColorPools)"));
		}
		else
		{
			for (const FGsSchemaSealSlotRerollGainColorPool* SealSlotRerollGainColorPool : SealSlotRerollGainColorPools)
			{
				if (nullptr == SealSlotRerollGainColorPool)
				{
					GSLOG(Error, TEXT("nullptr == SealSlotRerollGainColorPool"));
					continue;
				}

				FGsSealManager::_collectedRerollGainColorMap.FindOrAdd(SealSlotRerollGainColorPool->rerollGainColorPoolId).Add(SealSlotRerollGainColorPool);
			}
		}
	}
	return FGsSealManager::_collectedRerollGainColorMap;
}

const TMap<int32, TSet<const FGsSchemaSealSlotRerollGainStatPool*>>& FGsSealManager::CollectedRerollGainStatMap()
{
	if (0 == FGsSealManager::_collectedRerollGainStatMap.Num())
	{
		const UGsTable* SealSlotRerollGainStatPoolTable = FGsSchemaSealSlotRerollGainStatPool::GetStaticTable();
		TArray<const FGsSchemaSealSlotRerollGainStatPool*> SealSlotRerollGainStatPools;
		if (nullptr == SealSlotRerollGainStatPoolTable || !SealSlotRerollGainStatPoolTable->GetAllRows(SealSlotRerollGainStatPools))
		{
			GSLOG(Error, TEXT("nullptr == SealSlotRerollGainStatPoolTable || !SealSlotRerollGainStatPoolTable->GetAllRows(SealSlotRerollGainStatPools)"));
		}
		else
		{
			for (const FGsSchemaSealSlotRerollGainStatPool* SealSlotRerollGainStatPool : SealSlotRerollGainStatPools)
			{
				if (nullptr == SealSlotRerollGainStatPool)
				{
					GSLOG(Error, TEXT("nullptr == SealSlotRerollGainStatPool"));
					continue;
				}

				FGsSealManager::_collectedRerollGainStatMap.FindOrAdd(SealSlotRerollGainStatPool->rerollGainStatPoolId).Add(SealSlotRerollGainStatPool);
			}
		}
	}
	return FGsSealManager::_collectedRerollGainStatMap;
}

const TMap<FGsSealManager::SealPackGainColorId, TMap<SealSlotColor, TMap<FGsSealManager::MatchCount, TArray<TPair<StatType, StatValue>>>>> FGsSealManager::CollectSealPackGainStats()
{
	TMap<SealPackGainColorId, TMap<SealSlotColor, TMap<MatchCount, TArray<TPair<StatType, StatValue>>>>> GainStats;

	// TPair<StatType, StatValue> GainStat(StatType::NONE, 0);

	const UGsTable* SealPackGainColorTable = FGsSchemaSealPackGainColor::GetStaticTable();
	TArray<const FGsSchemaSealPackGainColor*> SealPackGainColorDatas;
	if (nullptr == SealPackGainColorTable || !SealPackGainColorTable->GetAllRows(SealPackGainColorDatas))
	{
		GSLOG(Error, TEXT("nullptr == SealPackGainColorTable || !SealPackGainColorTable->GetAllRows(SealPackGainColorDatas)"));
	}
	else
	{
		// TODO: 추후 range 로 변경할 것.
		// 테이블 내에 line 갯수를 전부 입력하지 않으므로, 최대 가능한 스탯을 조회하기 위해 마지막으로 유효하게 검색하여 할당된 line count 를 저장해 둠.
		MatchCount FoundLineCount = 0;
		for (const FGsSchemaSealPackGainColor* SealPackGainColorData : SealPackGainColorDatas)
		{
			if (nullptr == SealPackGainColorData)
			{
				GSLOG(Error, TEXT("nullptr == SealPackGainColorData"));
				continue;
			}

			GainStats.FindOrAdd(SealPackGainColorData->sealPackGainColorId).FindOrAdd(SealPackGainColorData->lineColorType).FindOrAdd(SealPackGainColorData->lineColorCount).Emplace(SealPackGainColorData->gainStatType, SealPackGainColorData->gainStatValue);
		}
	}

	return GainStats;
}
