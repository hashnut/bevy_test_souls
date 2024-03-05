#include "GsUIMonsterCollectionSlateNodeDetailPanelBase.h"
#include "../MonsterKnowledgeCollection/GsMonsterKnowledgeSlate.h"
#include "../Management/ScopeGame/GsMonsterCollectionManager.h"
#include "T1Project.h"


void UGsUIMonsterCollectionSlateNodeDetailPanelBase::NativeConstruct()
{
	Super::NativeConstruct();

	_detailType = ENodeDetailType::None;

	_slateId = INVALID_MONSTER_KNOWLEDGE_SLATE_ID;

	_callbackFunc = nullptr;
}

void UGsUIMonsterCollectionSlateNodeDetailPanelBase::NativeDestruct()
{
	_detailType = ENodeDetailType::None;

	_slateId = INVALID_MONSTER_KNOWLEDGE_SLATE_ID;

	_callbackFunc = nullptr;

	Super::NativeDestruct();
}

const FGsMonsterKnowledgeSlate* UGsUIMonsterCollectionSlateNodeDetailPanelBase::FindData() const
{
	if (INVALID_MONSTER_KNOWLEDGE_SLATE_ID != _slateId)
	{
		for (const FGsMonsterKnowledgeSlate* item : GsMonsterCollection()->SlateViewSet())
		{
			if (item->Id() == _slateId)
			{
				return item;
			}
		}
	}
	return nullptr;
}

void UGsUIMonsterCollectionSlateNodeDetailPanelBase::SetData(const class FGsMonsterKnowledgeSlate* InData, TFunction<void(EMonsterCollectionSlateCallbackReason, uint64)> InCallback)
{
	if (nullptr == InData)
	{
		return;
	}

	_slateId = InData->Id();
	_callbackFunc = InCallback;
}