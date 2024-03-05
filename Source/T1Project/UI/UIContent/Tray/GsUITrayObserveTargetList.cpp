#include "GsUITrayObserveTargetList.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGame/GsObserverManager.h"

#include "Message/MessageParam/GsMessageParam.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"

#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"

#include "UI/UIContent/Tray/ObserveTargetList/GsUIObserveTargetListItem.h"

#include "UMG/Public/Components/ScrollBox.h"
#include "UMG/Public/Components/VerticalBox.h"

void UGsUITrayObserveTargetList::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_scrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_scrollBoxHelper->Initialize(_entryWidgetClass, _verticalBox);
	_scrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUITrayObserveTargetList::OnRefreshEntry);
}
void UGsUITrayObserveTargetList::NativeConstruct()
{
	Super::NativeConstruct();

	if (FGsMessageHolder* msg = GMessage())
	{
		_msgContensObserverDelegates.Emplace(
			msg->GetObserver().AddUObject(MessageContentObserver::CHANGE_OBSERVE_TARGET_LIST,
				this, &UGsUITrayObserveTargetList::OnChangeObserveTaretList));
	}
}


void UGsUITrayObserveTargetList::NativeDestruct()
{
	ClearAllMessage();


	Super::NativeDestruct();
}

void UGsUITrayObserveTargetList::ClearAllMessage()
{
	if (FGsMessageHolder* msg = GMessage())
	{
		if (_msgContensObserverDelegates.Num() != 0)
		{
			for (auto iter : _msgContensObserverDelegates)
			{
				msg->GetObserver().Remove(iter);
			}
			_msgContensObserverDelegates.Empty();
		}
	}
}
void UGsUITrayObserveTargetList::OnRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	
	TArray<UGsGameObjectBase*> arrInfo = GSObserver()->GetPlayGuildDungeonMembers();

	if (arrInfo.Num() <= InIndex)
	{
		return;
	}

	UGsGameObjectBase* remoteObj = arrInfo[InIndex];

	if (remoteObj == nullptr)
	{
		return;
	}
	

	// 클라 스폰에게 현재 맵에서 출력할 npc list 데이터 얻어오기
	UGsUIObserveTargetListItem* item = Cast<UGsUIObserveTargetListItem>(InEntry);
	if (item)
	{
		// 아이템에 데이터 세팅
		item->SetObserveTargetListItemData(remoteObj);
	}

}

void UGsUITrayObserveTargetList::BeginDestroy()
{
	if (nullptr != _scrollBoxHelper)
	{
		_scrollBoxHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUITrayObserveTargetList::OnRefreshEntry);
		_scrollBoxHelper = nullptr;
	}

	Super::BeginDestroy();
}

// 데이터 세팅
void UGsUITrayObserveTargetList::SetObserveTargetListData()
{
	TArray<UGsGameObjectBase*> arrInfo = GSObserver()->GetPlayGuildDungeonMembers();

	int remainObserveTargetCount = arrInfo.Num();
	// 클라 스폰에게 현재 맵에서 출력할 npc list 사이즈 얻어오기
	_scrollBoxHelper->RefreshAll(remainObserveTargetCount);

	// 남은 인원수 처리
	_textRemainObserveTargetCount = FText::AsNumber(remainObserveTargetCount);
}


// 관전 대상 변경
void UGsUITrayObserveTargetList::OnChangeObserveTaretList(const IGsMessageParam* In_param)
{
	SetObserveTargetListData();
}