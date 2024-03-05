

#include "GsUIMonsterCollectionPortraitSet.h"
#include "GSUIMonsterCollectionPortrait.h"
#include "../Management/ScopeGame/GsMonsterCollectionManager.h"
#include "../UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UMG/Public/Components/WrapBox.h"
#include "T1Project.h"

void UGsUIMonsterCollectionPortraitSet::BeginDestroy()
{
	if (_slotHelperPortraitSet)
	{
		_slotHelperPortraitSet->OnRefreshEntry.RemoveDynamic(this, &UGsUIMonsterCollectionPortraitSet::OnRefreshEntryPortraitSet);
	}
	_slotHelperPortraitSet = nullptr;

	Super::BeginDestroy();
}


void UGsUIMonsterCollectionPortraitSet::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_slotHelperPortraitSet = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperPortraitSet->Initialize(_subClassOfPortraitSet, _boxPortraitSet);
	_slotHelperPortraitSet->OnRefreshEntry.AddDynamic(this, &UGsUIMonsterCollectionPortraitSet::OnRefreshEntryPortraitSet);
}

void UGsUIMonsterCollectionPortraitSet::NativeConstruct()
{
	Super::NativeConstruct();

	//
}

void UGsUIMonsterCollectionPortraitSet::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGsUIMonsterCollectionPortraitSet::OnRefreshEntryPortraitSet(int32 InIndex, UWidget* InEntry)
{
	TWeakObjectPtr<UGSUIMonsterCollectionPortrait> item = Cast<UGSUIMonsterCollectionPortrait>(InEntry);
	if (false == item.IsValid())
	{
		GSLOG(Error, TEXT("false == item.IsValid()"));
		return;
	}

	if( false == GsMonsterCollection()->BookViewSet().IsValidIndex(InIndex))
	{
		GSLOG(Error, TEXT("false == GsMonsterCollection()->BookViewSet().IsValidIndex(InIndex) : [%d]"), InIndex);
		return;
	}

	item->SetData(GsMonsterCollection()->BookViewSet()[InIndex]);
}

void UGsUIMonsterCollectionPortraitSet::SetData(int32 InMaxCount)
{
	_slotHelperPortraitSet->RefreshAll(InMaxCount);
}

void UGsUIMonsterCollectionPortraitSet::Invalidate(uint32 InId)
{
	if (INVALID_MONSTER_KNOWLEDGE_ID == InId)
	{	// 2023/1/31 PKT - Id 정보가 없으면 전체 갱신
		int32 maxCount = GsMonsterCollection()->BookViewSet().Num();
		_slotHelperPortraitSet->RefreshAll(maxCount);
	}
	else
	{	// 2023/1/31 PKT - Id 정보가 있으면 찾아서 갱신
		for (int32 idx = 0; idx < GsMonsterCollection()->BookViewSet().Num(); ++idx)
		{
			const FGsMonsterKnowledgeBook* data = GsMonsterCollection()->BookViewSet()[idx];
			if (data && data->Id() == InId)
			{
				TWeakObjectPtr<UGSUIMonsterCollectionPortrait> item = Cast<UGSUIMonsterCollectionPortrait>(_slotHelperPortraitSet->GetEntry(idx));
				if (item.IsValid())
				{	// 2023/2/1 PKT - 단일 갱신
					item->Invalidate();
				}
			}
		}
	}
}