#include "GsUITrayCraftResult.h"

#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/UMG/Public/Components/ScrollBox.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Runtime/UMG/Public/Components/Widget.h"
#include "Runtime/UMG/Public/Components/TileView.h"

#include "Craft/GsUICraftResultListItem.h"
#include "T1Project.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UILib/Define/GsUIDefine.h"
#include "TimerManager.h"

void UGsUITrayCraftResult::NativeConstruct()
{
	Super::NativeConstruct();

	if (_txtBtnDefault.IsEmpty())
	{
		_txtBtnDefault = _txtResult->GetText();
	}
}

void UGsUITrayCraftResult::NativeDestruct()
{
	//FreeTileItem();

	Super::NativeDestruct();
}

void UGsUITrayCraftResult::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_tileViewCraftResults->OnItemClicked().AddUObject(this, &UGsUITrayCraftResult::OnClickedListItem);
	//_tileViewCraftResults->OnListViewScrolled().AddUObject(this, &UGsUITrayCraftResult::OnScrolledListView);

	_tileRootSlotList.Reset();
}

EGsUIHideFlags UGsUITrayCraftResult::GetHideFlagType() const
{
	return EGsUIHideFlags::UI_HIDE_TRAY;
}

void UGsUITrayCraftResult::OpenAllResult()
{
	// 이제 다시 연출을 보여줄 준비를 할 것이다. 실제 연출은 NativeOnListItemObjectSet 에서 할 것
	_tileViewCraftResults->SetVisibility(ESlateVisibility::Visible);

	for (int i = 0; i < _tileViewCraftResults->GetNumItems(); ++i)
	{
		UGsCraftResultListItemData* itemData = Cast<UGsCraftResultListItemData>(_tileViewCraftResults->GetItemAt(i));

		if (nullptr == itemData)
		{
			GSLOG(Error, TEXT("nullptr == itemData, i:%d"), i);
			continue;
		}

		// 아이템 데이터는 닫은 상태로 처리했다가, NativeOnListItemObjectSet 열면서 연출을 재생한다
		itemData->SetIsOpened(true);

		// NativeOnListItemObjectSet 에서 애니메이션 재생을 위해 필요한 변수 세팅
		itemData->SetIsSuccess(true);
	}

	InvalidateOpenedResult();
}

void UGsUITrayCraftResult::InvalidateOpenedResult()
{
	TMap<CraftResult, int32> OpenedResultCounts(
	{
		{ CraftResult::NONE, 0 },
		{ CraftResult::CRAFT_SUCCEEDED, 0 },
		{ CraftResult::CRAFT_SUCCEEDED_GREAT, 0 },
		{ CraftResult::CRAFT_FAIL, 0 }
	});

	bool InAllCompleted = true;
	for (int32 i = 0; i < _tileViewCraftResults->GetNumItems(); ++i)
	{
		const UGsCraftResultListItemData* itemData = Cast<UGsCraftResultListItemData>(_tileViewCraftResults->GetItemAt(i));
		if (nullptr == itemData)
		{
			GSLOG(Error, TEXT("nullptr == itemData, i:%d"), i);
			continue;
		}

		if (itemData->IsOpened() || itemData->IsSuccess())
		{
			OpenedResultCounts[itemData->GetCraftResult()]++;
		}
		else
		{
			InAllCompleted = false;
		}
	}

	_txtFail->SetText(FText::AsNumber(OpenedResultCounts[CraftResult::CRAFT_FAIL]));
	_txtSuccess->SetText(FText::AsNumber(OpenedResultCounts[CraftResult::CRAFT_SUCCEEDED]));
	_txtGreatSuccess->SetText(FText::AsNumber(OpenedResultCounts[CraftResult::CRAFT_SUCCEEDED_GREAT]));

	_containerFail->SetVisibility(_parameters._canBeFail ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	_containerSuccess->SetVisibility(_parameters._canBeSuccess ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	_containerGreatSuccess->SetVisibility(_parameters._canBeGreatSuccess ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	SwitchButton(InAllCompleted);
}

bool UGsUITrayCraftResult::OpenCraftResult(UGsCraftResultListItemData* InCraftResultListItemData, int32 InIndex)
{
	if (nullptr == InCraftResultListItemData)
	{
		GSLOG(Error, TEXT("nullptr == InCraftResultListItemData"));
		return false;
	}

	// 스크롤을 먼저 해 줘야 멈춤 현상이 일어나지 않는 듯
	// 그런데 자동 오픈 시, 스크롤되는 오프셋이 위젯 아래 쪽으로 잡히지 않는 문제가 있어, NavigateToIndex 로 처리
	//_tileViewCraftResults->ScrollIndexIntoView(InIndex);
	_tileViewCraftResults->NavigateToIndex(InIndex);
	
	InCraftResultListItemData->SetIsOpened(true);

	InvalidateOpenedResult();

	return InCraftResultListItemData->IsOpened();
}

void UGsUITrayCraftResult::SwitchButton(const bool InAllCompleted)
{
	if (InAllCompleted)
	{
		// 닫기
		FText Text;
		if (!FText::FindText(TEXT("UICommonText"), TEXT("Close"), Text))
		{
			GSLOG(Error, TEXT("!FText::FindText(TEXT('UICommonText'), TEXT('Close'), Text)"));
			Text = FText::FromString(TEXT("Close"));
		}
		_txtResult->SetText(Text);
		
		if (_btnResult->OnClicked.IsAlreadyBound(this, &UGsUITrayCraftResult::OnClickedAutoOpening))
		{
			_btnResult->OnClicked.RemoveDynamic(this, &UGsUITrayCraftResult::OnClickedAutoOpening);
		}

		_btnResult->OnClicked.AddUniqueDynamic(this, &UGsUITrayCraftResult::OnClickedClose);
	}
	else
	{
		// 결과 보기
		_txtResult->SetText(_txtBtnDefault);

		if (_btnResult->OnClicked.IsAlreadyBound(this, &UGsUITrayCraftResult::OnClickedClose))
		{
			_btnResult->OnClicked.RemoveDynamic(this, &UGsUITrayCraftResult::OnClickedClose);
		}

		_btnResult->OnClicked.AddUniqueDynamic(this, &UGsUITrayCraftResult::OnClickedAutoOpening);
	}
}

void UGsUITrayCraftResult::StartAutoOpening()
{
	if (_isAutoOpening)
	{
		return;
	}
	_isAutoOpening = true;

	// 자동 오픈 시에는 스크롤 기능 막아야 함
	_tileViewCraftResults->SetWheelScrollMultiplier(0.f);
	_tileViewCraftResults->SetVisibility(ESlateVisibility::HitTestInvisible);

	_autoOpeningGapSecs = CalcAutoOpeningGapSecs();


	// 스크롤을 내린 상태에서 자동 오픈 시도할 때, 한 번 더 눌러줘야 하는 문제가 있어, 미리 한번 Navigate 시도
	int minIndex = FindClosedCraftResultMinIndex();
	_tileViewCraftResults->NavigateToIndex(minIndex);


	// 0.05 초 뒤 자동 오픈 시작 (NavigateToIndex 는 먹혔으나 아직 실제 Entry 가 그려지지 않는 케이스가 있다)
	FTimerManager* timerManager = GetTimerManager();
	if (nullptr == timerManager)
	{
		GSLOG(Error, TEXT("nullptr == timerManager"));
		return;
	}

	if (_autoOpeningTimerHandle.IsValid())
	{
		timerManager->ClearTimer(_autoOpeningTimerHandle);
	}

	timerManager->SetTimer(_autoOpeningTimerHandle, this, &UGsUITrayCraftResult::AutoOpeningAndReserveNext, _allOpeningScrollOffsetSecs);
	//AutoOpeningAndReserveNext();
}

void UGsUITrayCraftResult::AutoOpeningAndReserveNext()
{
	FTimerManager* TimerManager = GetTimerManager();
	if (nullptr == TimerManager)
	{
		GSLOG(Error, TEXT("nullptr == TimerManager"));
		return;
	}

	if (_autoOpeningTimerHandle.IsValid())
	{
		TimerManager->ClearTimer(_autoOpeningTimerHandle);
	}

	if (0 > AutoOpeningNextAndGetNextClosedCraftResultMinIndex())
	{
		// 자동 오픈 종료 처리
		_isAutoOpening = false;

		// 스크롤 기능 및 재활성화
		_tileViewCraftResults->SetWheelScrollMultiplier(1.f);
		_tileViewCraftResults->SetVisibility(ESlateVisibility::Visible);

		return;
	}

	TimerManager->SetTimer(_autoOpeningTimerHandle, this, &UGsUITrayCraftResult::AutoOpeningAndReserveNext, FMath::Clamp(_autoOpeningGapSecs, 0.f, 5.f));
}

int32 UGsUITrayCraftResult::AutoOpeningNextAndGetNextClosedCraftResultMinIndex()
{
	int32 index = FindClosedCraftResultMinIndex();
	
	// 그냥 순서대로 열면 될 것 같음. 왜냐하면 현재는 자동 오픈 도중에는 새로운 슬롯을 열 수 없기 때문이다
	// 그래서 사실 FindClosedCraftResultMinIndex 도 필요 없고, 0부터 시작하면 된다.
	if (0 <= index)
	{
		UGsCraftResultListItemData* itemData = Cast<UGsCraftResultListItemData>(_tileViewCraftResults->GetItemAt(index));
		if (nullptr == itemData)
		{
			GSLOG(Error, TEXT("nullptr == itemData, index:%d"), index);
			return -1;
		}

		// 아이템 데이터에 대해 오픈처리
		if (!OpenCraftResult(itemData, index))
		{
			// 오픈을 시도했으나 실패한 경우, 다음 자동 오픈 시에 무한 반복할 수 있으므로 에러로 처리함.
			GSLOG(Error, TEXT("!OpenCraftResult(item), index:%d"), index);
		}
		// 아이템 오픈을 성공했으면, 해당 아이템의 엔트리에 대해 오픈 연출 처리 후, 다음 대상의 Index 반환
		else
		{
			UUserWidget* widget = _tileViewCraftResults->GetEntryWidgetFromItem(itemData);
			if (nullptr == widget)
			{
				GSLOG(Error, TEXT("nullptr == widget"));
				return -1;
			}

			UGsUICraftResultListItem* item = Cast<UGsUICraftResultListItem>(widget);
			if (nullptr == item)
			{
				GSLOG(Error, TEXT("nullptr == item"));
				return -1;
			}

			item->OpenCraftResultEntry(true, itemData->GetCraftResult());

			return FindClosedCraftResultMinIndex();
		}
	}

	return -1;
}

float UGsUITrayCraftResult::CalcAutoOpeningGapSecs()
{
	int32 targetCount = 0;

	for (int32 i = 0; i < _tileViewCraftResults->GetNumItems(); ++i)
	{
		UGsCraftResultListItemData* itemData = Cast<UGsCraftResultListItemData>(_tileViewCraftResults->GetItemAt(i));
		if (nullptr == itemData)
		{
			GSLOG(Error, TEXT("nullptr == itemData, i:%d"), i);
			continue;
		}

		if (!itemData->IsOpened())
		{
			++targetCount;
		}
	}

	return FMath::Min(_maxOpeningDurationSec / targetCount, _defaultAutoOpeningGapSecs);
}

int32 UGsUITrayCraftResult::FindClosedCraftResultMinIndex() const
{
	const int32 count = _tileViewCraftResults->GetNumItems();
	for (int32 i = 0; i < count; ++i)
	{
		UGsCraftResultListItemData* itemData = Cast<UGsCraftResultListItemData>(_tileViewCraftResults->GetItemAt(i));
		if (nullptr == itemData)
		{
			GSLOG(Error, TEXT("nullptr == itemData"));
			continue;
		}

		if (!itemData->IsOpened())
		{
			return i;
		}
	}

	return -1;
}

void UGsUITrayCraftResult::SetParameters(Parameters InParameters)
{
	_isAutoOpening = false;

	// 실제 결과는 _parameters 의 _results 에 저장된다
	_parameters = MoveTemp(InParameters);

	// 기존 Item 오브젝트 반납
	//ReleaseAllTileItem();
	_tileRootSlotList.Empty();

	// ListView 에 아이템 리스트 세팅
	for (int32 i = 0; i < _parameters._results.Num(); ++i)
	{
		//UGsCraftResultListItemData* itemData = CreateTileItem();
		UGsCraftResultListItemData* itemData = NewObject<UGsCraftResultListItemData>(this);
		itemData->SetParameters(_parameters._results[i]);

		_tileRootSlotList.Add(itemData);
	}

	_tileViewCraftResults->SetListItems(_tileRootSlotList);

	// 실패가 없으므로 모두 즉시 오픈
	if (!_parameters._canBeFail)
	{
		_isAllSuccess = true;

		// 엔트리를 그려주기 위한 시간을 벌기 위한 hack. 잠시 보이지 않게 처리해 준다
		_tileViewCraftResults->SetVisibility(ESlateVisibility::Hidden);

		// OpenAni 가 어느 정도 끝나면, UIEventWindowFixed 이벤트에 연결된 OnUIEventWindowFixed 가 호출된다
	}
	else
	{
		_isAllSuccess = false;
		InvalidateOpenedResult();
	}
}

FTimerManager* UGsUITrayCraftResult::GetTimerManager() const
{
	const UWorld* World = GetWorld();
	if (nullptr == World)
	{
		return nullptr;
	}

	return &World->GetTimerManager();
}

void UGsUITrayCraftResult::OnClickedListItem(UObject* InCraftResultListItemData)
{
	if (_isAutoOpening)
	{
		return;
	}

	UGsCraftResultListItemData* itemData = Cast<UGsCraftResultListItemData>(InCraftResultListItemData);

	if (nullptr == itemData)
	{
		GSLOG(Error, TEXT("nullptr == itemData"));
		return;
	}

	// 만약 이미 열려 있는 녀석이라면, 오픈 처리를 하지 않고 무시
	if (itemData->IsOpened())
	{
		return;
	}
	
	// 아이템 데이터에 대해 오픈 처리
	OpenCraftResult(itemData, _tileViewCraftResults->GetIndexForItem(itemData));

	// 클릭한 아이템 엔트리에 대해 연출 처리 (스크롤이 일어나, 엔트리는 null 이 아닐 것)
	UUserWidget* widget = _tileViewCraftResults->GetEntryWidgetFromItem(itemData);
	if (nullptr == widget)
	{
		GSLOG(Error, TEXT("nullptr == widget"));
		return;
	}

	UGsUICraftResultListItem* item = Cast<UGsUICraftResultListItem>(widget);
	if (nullptr == item)
	{
		GSLOG(Error, TEXT("nullptr == item"));
		return;
	}

	item->OpenCraftResultEntry(true, itemData->GetCraftResult());
}

void UGsUITrayCraftResult::OnEntryScrolledIntoView(UObject* InCraftResultListItemData, UUserWidget& widget)
{
	// 아이템 데이터 유효성 검증
	UGsCraftResultListItemData* itemData = Cast<UGsCraftResultListItemData>(InCraftResultListItemData);

	if (nullptr == itemData)
	{
		GSLOG(Error, TEXT("nullptr == itemData"));
		return;
	}

	// 엔트리 위젯 유효성 검증
	UGsUICraftResultListItem* entry = Cast<UGsUICraftResultListItem>(&widget);

	if (nullptr == entry)
	{
		GSLOG(Error, TEXT("nullptr == entry"));
		return;
	}

	// ItemData 값에 맞게 엔트리 그려주기 (실질적으로는 컨테이너 커버 가릴지 말지만 결정하면 될듯)
	entry->SetOpened(itemData->IsOpened());
}

void UGsUITrayCraftResult::OnScrolledListView(float currentScrollOffset, float maxScrollOffset)
{
	if (_isAutoOpening)
	{
		_tileViewCraftResults->SetScrollOffset(_desiredScrollOffset);
	}
}

void UGsUITrayCraftResult::OnUIEventWindowFixed()
{
	// 실패가 없어서 한 번에 오픈해야 하는 상황이라면...
	if (_isAllSuccess)
	{
		OpenAllResult();
	}
}

void UGsUITrayCraftResult::OnClickedAutoOpening()
{
	StartAutoOpening();
}

void UGsUITrayCraftResult::OnClickedClose()
{
	if (_isAutoOpening)
	{
		return;
	}

	Close();
}

//void UGsUITrayCraftResult::OnEntryInitializedEvent(UObject* itemData, UUserWidget* widget)
//{
//	UGsCraftResultListItemData* itemData = Cast<UGsCraftResultListItemData>(itemData);
//	if (nullptr == itemData)
//	{
//		GSLOG(Error, TEXT("nullptr == itemData"));
//		return;
//	}
//
//	UGsUICraftResultListItem* item = Cast<UGsUICraftResultListItem>(widget);
//	if (nullptr == item)
//	{
//		GSLOG(Error, TEXT("nullptr == item"));
//		return;
//	}
//
//	if (itemData->IsSuccess())
//	{
//		item->SetVisibility(ESlateVisibility::Hidden);
//	}
//}

//UGsCraftResultListItemData* UGsUITrayCraftResult::CreateTileItem()
//{
//	if (_tileItemDataPool.Num() == 0)
//	{
//		if (UGsCraftResultListItemData* itemData = NewObject<UGsCraftResultListItemData>(this))
//		{
//			_tileItemDataPool.Push(itemData);
//		}
//	}
//
//	return _tileItemDataPool.Pop();
//}
//
//void UGsUITrayCraftResult::ReleaseTileItem(UGsCraftResultListItemData* InItemData)
//{
//	_tileItemDataPool.Push(InItemData);
//}
//
//void UGsUITrayCraftResult::ReleaseAllTileItem()
//{
//	for (UGsCraftResultListItemData* itemData : _tileItemDataPool)
//	{
//		_tileItemDataPool.Push(itemData);
//	}
//}
//
//void UGsUITrayCraftResult::FreeTileItem()
//{
//	for (UGsCraftResultListItemData* itemData : _tileItemDataPool)
//	{
//		itemData->ConditionalBeginDestroy();
//	}
//
//	_tileItemDataPool.Empty();
//}
