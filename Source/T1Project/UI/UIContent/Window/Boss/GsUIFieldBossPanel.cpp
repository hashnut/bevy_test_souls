#include "GsUIFieldBossPanel.h"
#include "UMG/Public/Components/Image.h"
#include "UMG/Public/Components/TextBlock.h"
#include "DataSchema/Map/FieldBoss/GsSchemaFieldBossData.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateBoss.h"
#include "Engine/AssetManager.h"
#include "GsUIWindowBossInfo.h"
#include "UMG/Public/Components/ListView.h"
#include "GsUIFieldBossDetailInfoPanel.h"
#include "Net/GsNetSendServiceBoss.h"
#include "GameObject/Boss/GsFieldBossHandler.h"
#include "UI/UIContent/Popup/Boss/GsUIPopupBossContribution.h"
#include "Message/MessageParam/GsBossMessageParam.h"
#include "UI/UIContent/Popup/Boss/GsBossInfoListViewEntry.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "UI/UIContent/Popup/Boss/GsFieldBossInfoListViewEntry.h"
#include "UTIL/GsLevelUtil.h"

void UGsUIFieldBossPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_contributionButton->OnClicked.AddDynamic(this, &UGsUIFieldBossPanel::OnReqBossKillConstribution);
	_mutantSwitcherButton->OnClicked.AddDynamic(this, &UGsUIFieldBossPanel::OnSwitchMutant);
}

void UGsUIFieldBossPanel::NativeConstruct()
{
	Super::NativeConstruct();

	//bind message
	if (FGsMessageHolder* msg = GMessage())
	{
		MBoss& bossMessage = msg->GetBoss();
		_bossDelegates.Emplace(bossMessage.AddUObject(MessageBoss::OPEN_BOSS_CONTRIBUTION_POPUP
			, this, &UGsUIFieldBossPanel::OnOpenBossKillContribution));		

		MUI& uiMessage = msg->GetUI();
		_uiDelegates.Emplace(uiMessage.AddUObject(MessageUI::CHANGE_FIELD_BOSS_BOOKMARK
			, this, &UGsUIFieldBossPanel::OnChangeFieldbossBookmark));
	}
	
	_isShowFieldBossMutant = false;
	_bossEntryDataArray.Empty();
	
	_bossImage->SetVisibility(ESlateVisibility::Hidden);
	_bossListView->SetScrollbarVisibility(ESlateVisibility::Hidden);

	FText::FindText(FIELD_BOSS_TEXT_KEY, UI_TEXT_NPERCENT, _ui_text_nPercent);
	FText::FindText(INVADE_TEXT_KEY, UI_TEXT_CAN_USE_FUNCTION, _ui_text_canUseFunction);
	FText::FindText(INVADE_TEXT_KEY, UI_TEXT_CAN_TELEPORT, _ui_text_canTeleport);
	
	Clear();
	InvalidBossList();
}

void UGsUIFieldBossPanel::NativeDestruct()
{
	if (FGsMessageHolder* msg = GMessage())
	{
		if (_bossDelegates.Num() > 0)
		{
			for (auto iter : _bossDelegates)
			{
				msg->GetBoss().Remove(iter);
			}
			_bossDelegates.Empty();
		}

		if (_uiDelegates.Num() > 0)
		{
			for (auto iter : _uiDelegates)
			{
				msg->GetUI().Remove(iter);
			}
			_uiDelegates.Empty();
		}
	}

	_bossEntryDataArray.Empty();
	Clear();

	Super::NativeDestruct();
}

void UGsUIFieldBossPanel::InvalidBossList()
{
	if (false == TryCreateBossArray(_bossEntryDataArray))
	{
		return;
	}

	UGsFieldBossInfoListViewEntryData** bossInfo = _bossEntryDataArray.FindByPredicate([&](UGsFieldBossInfoListViewEntryData* data)->bool {
		if (_selectFieldBossData)
		{
			return data->_bossData->bossId == _selectFieldBossData->bossId;
		}

		return false;
		});

	_bossListView->SetListItems(_bossEntryDataArray);

	if (bossInfo)
	{
		_bossListView->SetSelectedItem(*bossInfo);
	}
}


void UGsUIFieldBossPanel::OnSelectBoss(const struct IGsMessageParam* inParam)
{
	const FGsUIMsgParamSelectBoss* param = inParam->Cast<const FGsUIMsgParamSelectBoss>();
	if (nullptr == param)
	{
		return;
	}

	if (false == IsNeedToInvalid(param))
	{
		return;
	}

	Clear();

	_selectFieldBossData = param->_fieldBoss;
	_selectFieldBossMutantData = param->_mutantFieldBoss;
	_isShowFieldBossMutant = param->_isShowFieldBossMutant;
	_fieldBossLastDeadTime = param->_fieldBossLastDeadTime;

	SelectBoss(_selectFieldBossData, _selectFieldBossMutantData, _isShowFieldBossMutant);

	//버튼 설정

	//Detail Panel 설정
}

bool UGsUIFieldBossPanel::IsNeedToInvalid(const FGsUIMsgParamSelectBoss* inParam)
{
	EGsBossTab bossTab = inParam->_bossType;	

	switch (inParam->_bossType)
	{
	case EGsBossTab::FieldBoss:
	{
		if (inParam->_fieldBoss != _selectFieldBossData
			|| inParam->_mutantFieldBoss != _selectFieldBossMutantData
			|| inParam->_isShowFieldBossMutant != _isShowFieldBossMutant
			|| inParam->_fieldBossLastDeadTime != _fieldBossLastDeadTime)
		{
			return true;
		}
	}
		break;
	}

	return false;
}

void UGsUIFieldBossPanel::OnChangeFieldbossBookmark(const struct IGsMessageParam* inParam)
{
	const FGsUIMsgFieldBossBookmarkParam* param = inParam->Cast<const FGsUIMsgFieldBossBookmarkParam>();
	if (nullptr == param)
	{
		return;
	}

	InvalidBossList();
	FGsGameStateBoss::GetInstance()->SelectedFieldBoss(param->_bossId);
}

bool UGsUIFieldBossPanel::TryCreateBossArray(OUT TArray<class UGsFieldBossInfoListViewEntryData*>& outBossArray)
{	
	outBossArray.Empty();
	TArray<const struct FGsSchemaFieldBossData*> bossArray;
	if (false == FGsGameStateBoss::GetInstance()->TryGetFieldBossArray(bossArray))
	{
		return false;
	}
	
	UGsFieldBossInfoListViewEntryData* newBossInfoParma = nullptr;	
	TArray<const FGsSchemaNpcData*> bossNpcDataArray;
	_bossDataArray.Empty();
	for (const FGsSchemaFieldBossData* bossInfo : bossArray)
	{			
		if (nullptr == bossInfo
			|| nullptr == bossInfo->mapId.GetRow())
		{
			continue;
		}

#if ACTIVE_COOPERATIVE_BOSS
		if (bossInfo->bossType != BossType::COMPETITION
			&& bossInfo->bossType != BossType::COOPERATION)
		{
			continue;
		}
#else		
		if (bossInfo->bossType != BossType::COMPETITION)
		{
			continue;
		}
#endif

		newBossInfoParma = NewObject<UGsFieldBossInfoListViewEntryData>();
		newBossInfoParma->_bossData = bossInfo;
		newBossInfoParma->_mutantBossData = FGsFieldBossHandler::FindFieldBossData(bossInfo->mutantApear.fieldbossDataId);		
		newBossInfoParma->_level = UGsLevelUtil::GetSpawnLevel(bossInfo->bossNpcId.GetRow(), bossInfo->mapId.GetRow()->id);

		_bossDataArray.Add(bossInfo);
		outBossArray.Add(newBossInfoParma);
	}	

	return outBossArray.Num() > 0;
}

void UGsUIFieldBossPanel::OnOpenBossKillContribution(const IGsMessageParam* inParam)
{
	const FGsResultDefeatFieldBossMsgParam* param = static_cast<const FGsResultDefeatFieldBossMsgParam*>(inParam);

	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(POPUP_BOSS_CONTRIBUTION);
	if (false == widget.IsValid())
	{		
		return;
	}

	UGsUIPopupBossContribution* contribution = Cast<UGsUIPopupBossContribution>(widget.Get());
	if (nullptr == contribution)
	{
		return;
	}
	
	const struct FGsSchemaFieldBossData* bossData = FGsFieldBossHandler::FindFieldBossData(param->_bossId);
	contribution->InitContribution(bossData, param->_resultDepeatBoss);
}

void UGsUIFieldBossPanel::OnReqBossKillConstribution()
{
	if (nullptr == _selectFieldBossData)
	{
		return;
	}
	
	if (_isShowFieldBossMutant
	&& _selectFieldBossMutantData)
	{
		FieldBossId bossId = _selectFieldBossMutantData->bossId;
		FGsNetSendServiceBoss::SendBossContribution(bossId);		
	}
	else
	{
		FieldBossId bossId = _selectFieldBossData->bossId;
		FGsNetSendServiceBoss::SendBossContribution(bossId);
	}
}

void UGsUIFieldBossPanel::InvalidateBossInfo()
{
	InvalidNickName();
	InvalidMutantInfo();	
	InvalidBossImage();	
	InvalidDetailPanel();
}

const FGsSchemaFieldBossData* UGsUIFieldBossPanel::GetSelectBossData()
{
	return _selectFieldBossData;
}

void UGsUIFieldBossPanel::SelectBoss(const FGsSchemaFieldBossData* inBossInfo, const FGsSchemaFieldBossData* inMutant, bool inIsShowMutant)
{
	_selectFieldBossData = inBossInfo;
	_selectFieldBossMutantData = inMutant;
	_isShowFieldBossMutant = inIsShowMutant;

	//select npc
	UGsFieldBossInfoListViewEntryData** findEntryData = _bossEntryDataArray.FindByPredicate([&](const UGsFieldBossInfoListViewEntryData* entryData)->bool {
		return entryData->_bossData == inBossInfo;
		});
	if (nullptr == findEntryData)
	{
		return;
	}

	UGsFieldBossInfoListViewEntryData* entryData = (*findEntryData);

	_bossListView->SetItemSelection(entryData, true);
	int32 index = _bossListView->GetIndexForItem(entryData);
	_bossListView->NavigateToIndex(index);

	InvalidateBossInfo();
}

void UGsUIFieldBossPanel::OnSwitchMutant()
{
	FGsGameStateBoss::GetInstance()->SetShowMutant(!_isShowFieldBossMutant);
	FGsGameStateBoss::GetInstance()->InvalidateBoss();
}

void UGsUIFieldBossPanel::InvalidNickName()
{
	if (_isShowFieldBossMutant)
	{
		_nickNameTextBlock->SetText(_selectFieldBossMutantData->nickName);
		_nameTextBlock->SetText(_selectFieldBossMutantData->bossNpcId.GetRow()->nameText);
	}
	else
	{
		_nickNameTextBlock->SetText(_selectFieldBossData->nickName);
		_nameTextBlock->SetText(_selectFieldBossData->bossNpcId.GetRow()->nameText);
	}
}

void UGsUIFieldBossPanel::InvalidMutantInfo()
{
	if (_selectFieldBossMutantData)
	{
		_mutantPanel->SetVisibility(ESlateVisibility::Visible);
		_mutantPercentTextBlock->SetText(FText::Format(_ui_text_nPercent
			, (float)_selectFieldBossData->mutantApear.probability / 100.0f));
		_mutantSwitcherButton->SetSwitcherIndex((int32)_isShowFieldBossMutant);
	}
	else
	{
		_mutantPanel->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UGsUIFieldBossPanel::InvalidBossImage()
{
	if (_isShowFieldBossMutant)
	{
		if (false == _selectFieldBossMutantData->fieldBossImg.IsValid())
		{
			return;
		}

		UTexture2D* image = Cast<UTexture2D>(UAssetManager::GetStreamableManager().LoadSynchronous(_selectFieldBossMutantData->fieldBossImg));
		if (image)
		{
			_bossImage->SetBrushFromTexture(image);
			_bossImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
	else
	{
		if (false == _selectFieldBossData->fieldBossImg.IsValid())
		{
			return;
		}

		UTexture2D* image = Cast<UTexture2D>(UAssetManager::GetStreamableManager().LoadSynchronous(_selectFieldBossData->fieldBossImg));
		if (image)
		{
			_bossImage->SetBrushFromTexture(image);
			_bossImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
}

void UGsUIFieldBossPanel::InvalidDetailPanel()
{
	_fieldBossDetailPanel->SetFieldBossInfo(_selectFieldBossData
		, _selectFieldBossMutantData
		, _isShowFieldBossMutant
		, _fieldBossLastDeadTime);
}

void UGsUIFieldBossPanel::Clear()
{
	_isShowFieldBossMutant = false;
	_selectFieldBossData = nullptr;
	_selectFieldBossMutantData = nullptr;

	_nickNameTextBlock->SetText(FText::GetEmpty());
	_nameTextBlock->SetText(FText::GetEmpty());
	_bossImage->SetBrushFromTexture(nullptr);
	
	InvalidMutantInfo();
}