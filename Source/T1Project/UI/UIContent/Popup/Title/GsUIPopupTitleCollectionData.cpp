
#include "GsUIPopupTitleCollectionData.h"
#include "Title/ETitleEnum.h"
#include "Title/GsTitleRewardData.h"
#include "Title/GsTitleCollectionReward.h"
#include "Title/GsTitleHelper.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsTitleManager.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Popup/Title/GsUITitleCollectionEntry.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "../Runtime/UMG/Public/Components/Widget.h"
#include "../Runtime/UMG/Public/Components/TextBlock.h"
#include "../Runtime/UMG/Public/Components/ScrollBox.h"
#include "../Runtime/Engine/Classes/Engine/World.h"
#include "../Runtime/Engine/Public/TimerManager.h"


void UGsUIPopupTitleCollectionData::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupTitleCollectionData::OnClickedClose);

	_slotHelperCollection = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperCollection->Initialize(_subClassOfCollection, _scrollBoxCollection);
	_slotHelperCollection->OnRefreshEntry.AddDynamic(this, &UGsUIPopupTitleCollectionData::OnRefreshEntry);

	FText textUI;
	FGsTitleHelper::FindText(TEXT("UI_Popup_Retention_Status_Title"), textUI);
	_textRetentionStatus->SetText(textUI);
}

void UGsUIPopupTitleCollectionData::BeginDestroy()
{
	if (_btnClose)
	{
		_btnClose->OnClicked.RemoveDynamic(this, &UGsUIPopupTitleCollectionData::OnClickedClose);
		_btnClose = nullptr;
	}
	Super::BeginDestroy();
}

void UGsUIPopupTitleCollectionData::NativeConstruct()
{
	Super::NativeConstruct();

	SetRetentionStatus();

	const FGsTitleManager* manager = GTitle();
	if (nullptr == manager)
	{
		GSLOG(Error, TEXT("nullptr == manager"));
		return;
	}

	const FGsTitleCollectionReward* collectionRewardData = manager->GetCollectionRewardData();
	if (nullptr == collectionRewardData)
	{
		GSLOG(Error, TEXT("nullptr == collectionRewardData"));
		return;
	}

	_nextEffectIndex = INDEX_NONE;
	_isPlayingEffect = false;

	_updateEffectList = collectionRewardData->GetUpdateEffectList();

	const int32 maxCount = collectionRewardData->GetCollectionCount();
	_slotHelperCollection->RefreshAll(maxCount);

	SetEffectPlay();

	RegisterMessages();
}


void UGsUIPopupTitleCollectionData::NativeDestruct()
{
	Super::NativeDestruct();

	UWorld* world = GetWorld();
	if (world)
	{		
		world->GetTimerManager().ClearTimer(_timerHandler);
	}

	UnregisterMessages();
}

void UGsUIPopupTitleCollectionData::RegisterMessages()
{
	MTitle& MessageUiHolder = GMessage()->GetTitle();
	_messageUiDelegate 
		= MessageUiHolder.AddUObject(MessageContentTitle::UPDATE_COLLECTION, this, &UGsUIPopupTitleCollectionData::InvalidCollectionList);
}

void UGsUIPopupTitleCollectionData::UnregisterMessages()
{
	MTitle& MessageUiHolder = GMessage()->GetTitle();
	MessageUiHolder.Remove(_messageUiDelegate);
}

void UGsUIPopupTitleCollectionData::SetRetentionStatus()
{
	FGsTitleManager* titleManager = GTitle();
	if (nullptr == titleManager)
	{
		GSLOG(Error, TEXT("nullptr == titleManager"));
		return;
	}

	const FGsTitleCollectionReward* collectionReward = titleManager->GetCollectionRewardData();
	if (nullptr == collectionReward)
	{
		GSLOG(Error, TEXT("nullptr == collectionReward"));
		return;
	}

	const int32 currentAcquire = collectionReward->GetAcquire();
	const int32 maxAcquire = collectionReward->GetMaxAcquire();

	FString textRetention = FString::Format(TEXT("{0}/{1}"), { FString::FormatAsNumber(currentAcquire), FString::FormatAsNumber(maxAcquire) });

	// 2022/01/21 PKT - 보유 현황
	_textRetentionStatusCount->SetText(FText::FromString(textRetention));
}

void UGsUIPopupTitleCollectionData::SetEffectPlay()
{
	do 
	{
		const int32 maxEffectCount = _updateEffectList.Num();
		if (0 >= maxEffectCount || maxEffectCount <= _nextEffectIndex)
		{	// 2022/02/04 PKT - 연출 항목이 없거나 모든 항목을 다 연출 했다면 pass
			break;
		}

		if (true == _isPlayingEffect)
		{	// 2022/02/04 PKT - 현재 연출이 진행 중이라면 pass
			break;
		}

		if (INDEX_NONE == _nextEffectIndex)
		{	// 2022/02/04 PKT - 연출 시작 Index
			static const int32 EFFECT_BEGIN_INDEX = 0;
			_nextEffectIndex = EFFECT_BEGIN_INDEX;
		}

		if (false == _updateEffectList.IsValidIndex(_nextEffectIndex))
		{
			GSLOG(Error, TEXT("false == _updateEffectList.IsValidIndex(_currentEffectIndex)"));
			break;
		}

		const TPair<int32, ETitleRewardState>& target = _updateEffectList[_nextEffectIndex];

		const int32 maxCount = _slotHelperCollection->GetItemCount();
		for (int32 idx = 0; idx < maxCount; ++idx)
		{
			TWeakObjectPtr<UGsUITitleCollectionEntry> item = Cast<UGsUITitleCollectionEntry>(_slotHelperCollection->GetEntry(idx));
			if (false == item.IsValid() || nullptr == item->GetData() || item->GetData()->GetId() != target.Key)
			{
				continue;
			}

			item->PlayEffect(ETitleRewardState::Activate == target.Value);

			_isPlayingEffect = true;

			// 2022/01/26 PKT - Scroll Focus
			_scrollBoxCollection->ScrollWidgetIntoView(item.Get(), false, EDescendantScrollDestination::Center);
		}

	} while (0);

}

void UGsUIPopupTitleCollectionData::OnRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	TWeakObjectPtr<UGsUITitleCollectionEntry> item = Cast<UGsUITitleCollectionEntry>(InEntry);
	if (false == item.IsValid())
	{
		GSLOG(Error, TEXT("false == item.IsValid()"));
		return;
	}

	const FGsTitleManager* manager = GTitle();
	if (nullptr == manager)
	{
		GSLOG(Error, TEXT("nullptr == manager"));
		return;
	}

	const FGsTitleCollectionReward* collectionRewardData = manager->GetCollectionRewardData();
	if (nullptr == collectionRewardData)
	{
		GSLOG(Error, TEXT("nullptr == collectionRewardData"));
		return;
	}

	const FGsTitleRewardData* rewardData = collectionRewardData->FindRewardData(InIndex);

	bool IsConfirm = true;	
	// 2022/01/26 PKT - Effect 목록에 있다면..
	int32 findIndex = _updateEffectList.IndexOfByPredicate([rewardData](const TPair<int32, ETitleRewardState>& InData)
		{
			return (InData.Key == rewardData->GetId());
		}
	);

	if (INDEX_NONE != findIndex)
	{	// 2022/01/26 PKT - 상태를 뒤집어서 넣어 준다.
		IsConfirm = false;
	}

	item->SetData(rewardData, IsConfirm);
	item->OnFinishedVFX().BindUObject(this, &UGsUIPopupTitleCollectionData::OnFinishedVFX);
}

void UGsUIPopupTitleCollectionData::OnClickedClose()
{
	// 2022/01/26 PKT - 
	FGsTitleManager* manager = GTitle();
	if (nullptr == manager)
	{
		GSLOG(Error, TEXT("nullptr == manager"));
		return;
	}
	// 2022/01/26 PKT - 컬렉션 Data 확인 완료
	manager->CollectionConfirm();

	Close();
}


void UGsUIPopupTitleCollectionData::OnFinishedVFX()
{
	_isPlayingEffect = false;

	_nextEffectIndex++;
	
	/**
	 * 2022/02/04 PKT - VFX에서 Delegate 호출 후 Visiblity를 조정하는데 그 때문에 중복된 타이틀에 대한 효과가 하나씩 씹힌다.
	 *					한프레임 늦게 효과를 연출 하자.
	 */
	UWorld* world = GetWorld();
	if (world)
	{
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUObject(this, &UGsUIPopupTitleCollectionData::SetEffectPlay);
		world->GetTimerManager().SetTimerForNextTick(TimerDelegate);
	}
}


void UGsUIPopupTitleCollectionData::InvalidCollectionList(const IGsMessageParam* InParam)
{
	SetRetentionStatus();

	const FGsTitleManager* manager = GTitle();
	if (nullptr == manager)
	{
		GSLOG(Error, TEXT("nullptr == manager"));
		return;
	}

	const FGsTitleCollectionReward* collectionRewardData = manager->GetCollectionRewardData();
	if (nullptr == collectionRewardData)
	{
		GSLOG(Error, TEXT("nullptr == collectionRewardData"));
		return;
	}

	// 2022/01/26 PKT - Effect List에 추가 한다.
	const TArray<TPair<int32, ETitleRewardState>>& updateEffectList = collectionRewardData->GetUpdateEffectList();
	const int32 maxCount = updateEffectList.Num();
	const int32 beginIndex = _updateEffectList.Num();
	for (int32 idx = beginIndex; idx < maxCount; ++idx)
	{	// 2022/02/04 PKT - 현재까지 진행한 뒤에서 부터 추가 한다.
		_updateEffectList.Emplace(updateEffectList[idx]);
	}

	SetEffectPlay();
}

void UGsUIPopupTitleCollectionData::OnInputCancel()
{
	OnClickedClose();
}