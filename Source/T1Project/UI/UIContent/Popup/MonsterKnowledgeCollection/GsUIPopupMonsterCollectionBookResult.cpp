#include "GsUIPopupMonsterCollectionBookResult.h"
#include "../MonsterKnowledgeCollection/GsMonsterKnowledgeData.h"
#include "../Management/ScopeGame/GsMonsterCollectionManager.h"
#include "../Classes/GsPoolUObject.h"
#include "../UI/UIControlLib/ContentWidget/GsButton.h"
#include "../UI/UIContent/Window/MonsterKnowledge/GSUIMonsterCollectionPortrait.h"
#include "UMG/Public/Components/TileView.h"
#include "UMG/Public/Components/TextBlock.h"


void UGsUIPopupMonsterCollectionBookResult::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (_btnOK)
	{
		_btnOK->OnClicked.AddDynamic(this, &UGsUIPopupMonsterCollectionBookResult::OnClose);
	}
	_btnOK = nullptr;

	_poolBookResultDataSet = NewObject<UGsPoolUObject>(this);
}

void UGsUIPopupMonsterCollectionBookResult::BeginDestroy()
{
	if (_poolBookResultDataSet)
	{
		_poolBookResultDataSet->RemovePool();
		_poolBookResultDataSet = nullptr;
	}

	if (_btnOK)
	{
		_btnOK->OnClicked.RemoveDynamic(this, &UGsUIPopupMonsterCollectionBookResult::OnClose);
	}
	_btnOK = nullptr;

	Super::BeginDestroy();
}

void UGsUIPopupMonsterCollectionBookResult::NativeConstruct()
{
	Super::NativeConstruct();

	_tileListMonsterBook->ClearListItems();
}

void UGsUIPopupMonsterCollectionBookResult::NativeDestruct()
{
	_tileListMonsterBook->ClearListItems();
	_poolBookResultDataSet->RemovePool();
	_currentCount = 0;
	_maxCount = 0;
	_deltaTime = 0.f;

	Super::NativeDestruct();
}

void UGsUIPopupMonsterCollectionBookResult::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (_currentCount >= _maxCount || false == _resultSet.IsValidIndex(_currentCount))
	{
		return;
	}

	_deltaTime -= InDeltaTime;
	if (0.f >= _deltaTime)
	{
		UGsMonsterCollectionPortraitEntryData* entryData = _poolBookResultDataSet->Claim<UGsMonsterCollectionPortraitEntryData>();
		if (entryData)
		{
			entryData->SetData(_resultSet[_currentCount]._data, _resultSet[_currentCount]._isLevelUp);
			
			_tileListMonsterBook->AddItem(entryData);
			//_tileListMonsterBook->ScrollIndexIntoView(_currentCount);
			_tileListMonsterBook->ScrollToBottom();
		}
		_deltaTime = _portraitOpenDelay;
		_currentCount++;
	}
}

void UGsUIPopupMonsterCollectionBookResult::OnClose()
{
	if (OnCloseDelegate.IsBound())
	{
		OnCloseDelegate.Execute();
	}

	Close();
}

void UGsUIPopupMonsterCollectionBookResult::SetData(const TArray<FGsMonsterCollectionBookResult>& InData)
{
	int32 rewardPoint = 0;
	
	_resultSet.Empty(InData.Num());

	/**
	 * 정렬은???
	 */
	for (const auto item : InData)
	{
		const auto& bookData = GsMonsterCollection()->BookDataById(item.id);
		if (nullptr == bookData)
		{
			continue;
		}
		
		FGsBookResultData newData;
		newData._data = bookData;
		newData._isLevelUp = item.isLevelUp;
		
		_resultSet.Emplace(newData);

		rewardPoint += item.rewardPoint;
	}
	// 2023/2/11 PKT - Total Reward Point
	_textRewardPoint->SetText(FText::AsNumber(rewardPoint));

	_tileListMonsterBook->RegenerateAllEntries();
	_tileListMonsterBook->SetScrollOffset(0.f);
	_tileListMonsterBook->SetScrollbarVisibility(ESlateVisibility::Collapsed);

	// 2023/2/11 PKT - 전체 내용 삭제
	_poolBookResultDataSet->RemovePool();
	_poolBookResultDataSet->WarmUp<UGsMonsterCollectionPortraitEntryData>(_resultSet.Num());

	_maxCount = _resultSet.Num();
	_currentCount = 0;
	_deltaTime = 0.f;
}