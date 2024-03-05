// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/SpiritShot/GsUIPopupSpiritShotSumBuff.h"
#include "UI/UIContent/Window/SpiritShot/GsUISpiritShotPlusStatListItem.h"
#include "UI/UIContent/Window/SpiritShot/GsUISpiritShotLevelStatListItem.h"
#include "UI/UIContent/Helper/GsEffectStringHelper.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "GameObject/Stat/GsStatHelper.h"
#include "Runtime/UMG/Public/Components/ListView.h"
#include "Runtime/UMG/Public/Components/CanvasPanel.h"

#include "Management/ScopeGame/GsEffectTextManager.h"
#include "Management/ScopeGame/GsSpiritShotmanager.h"

void UGsUIPopupSpiritShotSumBuff::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	_btnClose->OnClicked.AddUniqueDynamic(this, &UGsUIPopupSpiritShotSumBuff::OnClickClose);
}

void UGsUIPopupSpiritShotSumBuff::SetParameters(Parameters&& InParameters)
{
	_parameters = MoveTemp(InParameters);


	FGsEffectTextManager* effectTextMgr = GSEffectText();
	if (nullptr == effectTextMgr)
	{
		GSLOG(Error, TEXT("nullptr == effectTextMgr"));
		return;
	}

	// 스탯 시간 정보는 출력 안함
	int32 effectTextOption = 0;
	FGsEffectTextOptionFunc::AddOption(EGsEffectTextOption::SKIP_STAT_TIME, effectTextOption);


	// 좌측 '천마석 추가 효과' 파싱 작업
	FGsEffectTextCollection plusTextCollection;

	FGsEffectStringHelper::AddCollectionEffect(&_parameters.plusPassivityCollection, plusTextCollection);


	//// 스탯 이름 - 스탯 값 페어 리스트 생성
	//for (const PassivityId passivityId : _parameters.plusPassivityIdList)
	//{
	//	effectTextMgr->GetEffectTextListPassivity(passivityId, statNameTextPairList, effectTextOption);
	//}	
	
	// 목록별 데이터 리스트 생성
	TArray<UGsSpiritShotPlusStatListData*> plusStatDataList;
	for (const auto& pair : plusTextCollection._statMap)
	{
		UGsSpiritShotPlusStatListData* itemData = NewObject<UGsSpiritShotPlusStatListData>(this);

		TPair<FText, FText> statTextPair;
		FGsStatHelper::GetDesplayStatNameAndValue(pair.Key, pair.Value, statTextPair);
		itemData->SetData(MoveTemp(statTextPair));

		plusStatDataList.Emplace(MoveTemp(itemData));
	}
	_listViewPlusStatSum->SetListItems(plusStatDataList);
	_listViewPlusStatSum->SetScrollbarVisibility(ESlateVisibility::Collapsed);

	// 추가 효과가 비어 있을 경우, "적용중인 효과가 없습니다" 처리
	if (plusStatDataList.Num() > 0)
	{
		_panelNoPlusStatSum->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		_panelNoPlusStatSum->SetVisibility(ESlateVisibility::HitTestInvisible);
	}


	// 우측 '잠식 레벨 효과' 파싱 작업
	TArray<TPair<FText, FText>> statNameTextPairList;

	// 스탯 이름 - 스탯 값 페어 리스트 생성
	for (const auto& pair : _parameters.levelPassivityIdPairList)
	{
		effectTextMgr->GetEffectTextListPassivity(pair.Value, statNameTextPairList, effectTextOption);
	}

	// 목록별 데이터 리스트 생성
	FGsSpiritShotManager* spiritShotMgr = GSSpiritShot();
	if (nullptr == spiritShotMgr)
	{
		GSLOG(Error, TEXT("nullptr == spiritShotMgr"));
		return;
	}

	TArray<UGsUISpiritShotLevelStatListData*> levelStatDataList;
	for (int32 i = 0; i < _parameters.levelPassivityIdPairList.Num(); ++i)
	{
		Level level = _parameters.levelPassivityIdPairList[i].Key;

		UGsUISpiritShotLevelStatListData* itemData = NewObject<UGsUISpiritShotLevelStatListData>(this);

		// 스탯 효과가 비어 있다면 표기 불가
		if (!statNameTextPairList.IsValidIndex(i))
		{
			continue;
		}
		
		itemData->SetData(level, statNameTextPairList[i].Key, statNameTextPairList[i].Value, spiritShotMgr->GetSumLevel() >= level);

		levelStatDataList.Emplace(MoveTemp(itemData));
	}
	_listViewLevelStatSum->SetListItems(levelStatDataList);
	_listViewLevelStatSum->SetScrollbarVisibility(ESlateVisibility::Collapsed);
}

void UGsUIPopupSpiritShotSumBuff::OnClickClose()
{
	Close();
}

void UGsUIPopupSpiritShotSumBuff::OnInputCancel()
{
	Close();
}
