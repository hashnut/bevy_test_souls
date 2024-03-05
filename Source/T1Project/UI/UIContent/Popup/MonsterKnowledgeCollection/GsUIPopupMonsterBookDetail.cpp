
#include "GsUIPopupMonsterBookDetail.h"
#include "../Item/GsItem.h"
#include "../Item/GsItemManager.h"
#include "../Currency/GsCostPackage.h"
#include "../MonsterKnowledgeCollection/EMonsterKnowledgeEnumData.h"
#include "../MonsterKnowledgeCollection/GsMonsterKnowledgeBook.h"
#include "../Management/ScopeGame/GsMonsterCollectionManager.h"
#include "../Management/ScopeGlobal/GsGameFlowManager.h"
#include "../GameFlow/GsGameFlowGame.h"
#include "../GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "../GameFlow/GameContents/ContentsGame/GsGameStateRegionMap.h"
#include "../GameFlow/GameContents/GsContentsMode.h"
#include "../UTIL/GsGameObjectUtil.h"
#include "../UI/UILib/Base/GsUIVFX.h"
#include "../UI/UIContent/Common/GsUIProgressBar.h"
#include "../UI/UIContent/Common/Tooltip/GsUITooltipDesc.h"
#include "../UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "../UI/UIContent/Common/Icon/GsUIIconItemReward.h"
#include "../UI/UIControlLib/ContentWidget/GsButton.h"
#include "../UI/UIContent/Window/MonsterKnowledge/GSUIMonsterCollectionPortrait.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "UMG/Public/Components/CanvasPanel.h"
#include "UMG/Public/Components/PanelWidget.h"
#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "DataSchema/MonsterKnowledgeCollection/GsSchemaMonsterKnowledgeLevelSet.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "../Net/GsNetSendServiceWorld.h"
#include "T1Project.h"







void UGsUIPopupMonsterBookDetail::BeginDestroy()
{
	if (_btnShotMonsterDetail)
	{
		_btnShotMonsterDetail->OnClicked.RemoveDynamic(this, &UGsUIPopupMonsterBookDetail::OnGoMonsterDetail);
	}
	_btnShotMonsterDetail = nullptr;

	if (_btnTooltip)
	{
		_btnTooltip->OnClicked.RemoveDynamic(this, &UGsUIPopupMonsterBookDetail::OnTooltip);
	}
	_btnTooltip = nullptr;

	if (_btnCancel)
	{
		_btnCancel->OnClicked.RemoveDynamic(this, &UGsUIPopupMonsterBookDetail::OnClose);
	}
	_btnCancel = nullptr;

	if (_btnOK)
	{
		_btnOK->OnClicked.RemoveDynamic(this, &UGsUIPopupMonsterBookDetail::OnClose);
	}
	_btnOK = nullptr;

	if (_btnApply)
	{
		_btnApply->OnClicked.RemoveDynamic(this, &UGsUIPopupMonsterBookDetail::OnApply);
	}
	_btnApply = nullptr;

	Super::BeginDestroy();
}

void UGsUIPopupMonsterBookDetail::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (_btnCancel)
	{
		_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupMonsterBookDetail::OnClose);
	}

	if (_btnOK)
	{
		_btnOK->OnClicked.AddDynamic(this, &UGsUIPopupMonsterBookDetail::OnClose);
	}

	if (_btnApply)
	{
		_btnApply->OnClicked.AddDynamic(this, &UGsUIPopupMonsterBookDetail::OnApply);
	}

	if (_btnTooltip)
	{
		_btnTooltip->OnClicked.AddDynamic(this, &UGsUIPopupMonsterBookDetail::OnTooltip);
	}

	if (_btnShotMonsterDetail)
	{
		_btnShotMonsterDetail->OnClicked.AddDynamic(this, &UGsUIPopupMonsterBookDetail::OnGoMonsterDetail);
	}
	_btnShotMonsterDetail = nullptr;
}

void UGsUIPopupMonsterBookDetail::NativeConstruct()
{
	Super::NativeConstruct();
	
	_data = nullptr;

	_increaseExp = INVALID_MONSTER_KNOWLEDGE_EXP;
	_deltaExp = 0;
	
	_progressBarExp->SetProgressText(FText::GetEmpty());

	_rewardBookPoint->SetText(FText::AsNumber(0));

	_effectAmountChanged->HideVFX();

	FText textToolTip;
	FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("UI_Popup_DetailInfo_InfoText"), textToolTip);
	_tooltipKnowledgeStatus->SetDescText(textToolTip);
	_tooltipKnowledgeStatus->Close();
}

void UGsUIPopupMonsterBookDetail::NativeDestruct()
{
	Super::NativeDestruct();

	_tooltipKnowledgeStatus->Close();
}

void UGsUIPopupMonsterBookDetail::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (nullptr == _data || (double)_increaseExp == _deltaExp)
	{
		return;
	}

	const MonsterKnowledgeExp currentExp = _data->AccumulateExp();

	// 2023/2/2 PKT - 현재 구간에 대한 Gauge Range 값을 얻어온다.
	double scopeValue = 1.0;
	{
		Level deltaLevel = _data->LevelByExp(currentExp + StaticCast<MonsterKnowledgeExp>(_deltaExp));
		if (deltaLevel >= _data->MaxLevel())
		{	// 2023/2/2 PKT - 현재 레벨이 최고 레벨이면 참고 할만한 정보가 없다. 바로 이전 단계의 정보를 참고 하도록.
			double preMaxExp = (double)_data->ExpNPoint(deltaLevel - 2).Key;
			scopeValue = (double)_data->ExpNPoint(deltaLevel - 1).Key - preMaxExp;
		}
		else
		{
			double preMaxExp = (double)_data->ExpNPoint(deltaLevel - 1).Key;
			scopeValue = (double)_data->ExpNPoint(deltaLevel).Key - preMaxExp;
		}
	}

	// 2023/2/2 PKT - Delta 증가량을 계산
	const static double speed = 100;
	if (_deltaExp < _increaseExp)
	{
		_deltaExp += speed * (scopeValue / 100) * InDeltaTime;
		_deltaExp = FMath::Clamp<double>(_deltaExp, 0, StaticCast<double>(_increaseExp));
	}
	else
	{
		_deltaExp -= speed * (scopeValue / 100) * InDeltaTime;
		
		MonsterKnowledgeExp maxTotalExp = _data->ExpNPoint(_data->MaxLevel() - 1).Key;
		_deltaExp = FMath::Clamp<double>(_deltaExp, StaticCast<double>(_increaseExp), StaticCast<double>(maxTotalExp));
	}

	// 2023/2/13 PKT - 현재 경험치로 레벨을 구한다.( 실제 레벨과 다를 수 있음. )
	int32 realLevel = int32(_data->LevelByExp(currentExp));
	// 2023/5/22 PKT - 현재 경험치와 증가할 경험치의 합으로 예상 레벨을 구한다.
	int32 viewLevel = int32(_data->LevelByExp(currentExp + _deltaExp));

	if (realLevel != viewLevel)
	{
		_progressBarExp->SetPercent(0.0f);
		_applyLevelInfo->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		// 2022/04/20 PKT - 적용 후 레벨
		FText textFormatLevel;
		FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("MonsterKnowledgeLevel"), textFormatLevel);
		_applyLevel->SetText(FText::FromString(FString::Format(*textFormatLevel.ToString(), { viewLevel + 1 })));

		_rewardBookPoint->SetText(FText::AsNumber(_data->GainRewardPoint(realLevel, currentExp + _deltaExp)));
	}
	else
	{
		_applyLevelInfo->SetVisibility(ESlateVisibility::Collapsed);

		double preMaxExp = (double)_data->ExpNPoint(realLevel - 1).Key;
		double curExp = (double)_data->AccumulateExp() - preMaxExp;
		double maxExp = (double)_data->ExpNPoint(realLevel).Key - preMaxExp;
		_progressBarExp->SetPercent(FMath::Clamp<float>(float(curExp) / maxExp, 0.f, 1.0f));
	}

	double preMaxExp = (double)_data->ExpNPoint(viewLevel - 1).Key;
	double curExp = (double)_data->AccumulateExp() + _deltaExp - preMaxExp;
	double maxExp = (double)_data->ExpNPoint(viewLevel).Key - preMaxExp;
	if (viewLevel == _data->MaxLevel())
	{
		const auto& pair = _data->ExpNPoint(viewLevel - 2);
		curExp = maxExp = (preMaxExp - pair.Key);
	}

	_progressBarExp->SetProgressText(FText::FromString(FString::Format(TEXT("{0}/{1}"), { int32(curExp), int32(maxExp) })));
	_progressBarExp->SetSecondPercent(FMath::Clamp<float>(float(curExp) / maxExp, 0.f, 1.0f));
}

void UGsUIPopupMonsterBookDetail::OnBtnClickedChangedAmount()
{
	_effectAmountChanged->ShowVFX();
}

void UGsUIPopupMonsterBookDetail::InvalidateAmount(int32 InAmountUsed)
{
	//MonsterKnowledgeId;
	MonsterKnowledgeExp newExp = _data->AccumulateExp();
	
	do
	{
		// 2022/04/20 PKT - 적용 가능한 갯수를 구한다. 최대 경험치를 넘는 아이템 갯수에 대해 조정함.
		if (false == GsMonsterCollection()->BookExpCostSet().IsValidIndex(_selectedIndex) || EMPTY_AMOUNT >= InAmountUsed)
		{
			break;
		}

		const auto& costItem = GsMonsterCollection()->BookExpCostSet()[_selectedIndex];		
		InAmountUsed = InAmountUsed > costItem.GetPlayerCanUseMaxCount() ? costItem.GetPlayerCanUseMaxCount() : InAmountUsed;

		const FGsCostElement* costElement = costItem.GetFirstCostElement();
		if (nullptr == costElement)
		{
			GSLOG(Error, TEXT("nullptr == costElement"));
			break;
		}

		MonsterKnowledgeExp exp = GsMonsterCollection()->AmountIncreasedUsedExpItem(costElement->GetItemId());
		int32 maxAmount = _data->RequiredAmountMaxLevel(exp);
		InAmountUsed = FMath::Min(maxAmount, InAmountUsed);

		_increaseExp = exp * InAmountUsed;

		const MonsterKnowledgeExp totalMaxExp = _data->ExpNPoint(_data->MaxLevel() - 1).Key;
		if (_data->AccumulateExp() + _increaseExp > totalMaxExp)
		{
			_increaseExp = totalMaxExp - _data->AccumulateExp();
		}

	} while (0);

	_btnApply->SetIsEnabled(0 < InAmountUsed);

	Super::InvalidateAmount(InAmountUsed);
}

void UGsUIPopupMonsterBookDetail::OnClose()
{
	if (_data && _OnClickedCloseDelegate.IsBound())
	{
		_OnClickedCloseDelegate.Execute(_data->Id());
	}
	Super::Close();	
}

void UGsUIPopupMonsterBookDetail::OnApply()
{
	if (UNSELECTED_INDEX >= _selectedIndex || EMPTY_AMOUNT >= _amountUsed)
	{
		GSLOG(Error, TEXT("UNSELECTED_INDEX >= _selectedIndex || EMPTY_AMOUNT >= _amountUsed"));
		return;
	}

	if (false == GsMonsterCollection()->BookExpCostSet().IsValidIndex(_selectedIndex) || EMPTY_AMOUNT >= _amountUsed)
	{
		GSLOG(Error, TEXT("Invalidate SelectedIndex or AmountUsed : [%d, %d]"), _selectedIndex, _amountUsed);
		return;
	}

	const auto& costItem = GsMonsterCollection()->BookExpCostSet()[_selectedIndex];
	const FGsCostElement* costElement = costItem.GetFirstCostElement();
	if (nullptr == costElement)
	{
		GSLOG(Error, TEXT("nullptr == costElement"));
		return;
	}

	ItemDBId itemDBId = INVALID_ITEM_DB_ID;
	if (costElement->PlayerHasAmountMax())
	{
		const auto& findSet = GItem()->FindByTID(costElement->GetItemId(), ItemType::INGREDIENT);
		if (0 < findSet.Num() && findSet[0].IsValid())
		{
			itemDBId = findSet[0].Pin().Get()->GetDBID();
		}
	}
	
	if (INVALID_ITEM_DB_ID == itemDBId)
	{
		GSLOG(Error, TEXT("INVALID_ITEM_DB_ID == itemDBId"));
		return;
	}

	FGsNetSendServiceWorld::Send_MonsterKnowledgeExpGainItemUse(itemDBId, _amountUsed, _data->Id());
}

void UGsUIPopupMonsterBookDetail::OnGoMonsterDetail()
{
	if (nullptr == _data || nullptr == _data->SpawnMapData())
	{
		GSLOG(Error, TEXT("nullptr == _data || nullptr == _data->SpawnMapData()"));
		return;
	}

	FGsGameFlowGame* GameFlowGame = GMode()->GetGameFlow();
	if (nullptr == GameFlowGame)
	{
		GSLOG(Warning, TEXT("nullptr == GameFlowGame"));
		return;
	}

	FGsContentsManagerGame* ContentsManagerGame = GameFlowGame->GetContentsManagerGame();
	if (nullptr == ContentsManagerGame)
	{
		GSLOG(Warning, TEXT("nullptr == ContentsManagerGame"));
		return;
	}

	FGsGameStateRegionMap* GameStateRegionMap = ContentsManagerGame->GetContentsStateObject<FGsGameStateRegionMap>(FGsContentsMode::InGame::ContentsRegionMap);
	if (nullptr == GameStateRegionMap)
	{
		GSLOG(Warning, TEXT("nullptr == GameStateRegionMap"));
		return;
	}

	MapId mapId = _data->SpawnMapData()->id;
	CreatureId creatureId = _data->NPCData()->id;
	GameStateRegionMap->ShowCreatureMap(mapId, creatureId);
}

void UGsUIPopupMonsterBookDetail::OnTooltip()
{
	_tooltipKnowledgeStatus->ToggleOpenClose();
}

void UGsUIPopupMonsterBookDetail::Invalidate()
{
	Super::Invalidate();

	if (nullptr == _data)
	{
		GSLOG(Error, TEXT("nullptr == _data"));
		return;
	}

	_portrait->Invalidate();

	// 2022/04/20 PKT - current Level
	FText textFormatLevel;
	FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("MonsterKnowledgeLevel"), textFormatLevel);
	_currentLevel->SetText(FText::FromString(FString::Format(*textFormatLevel.ToString(), { _data->Level() + 1 })));

	const static int32 SWITCHER_USEITEM_INDEX = 0;
	const static int32 SWITCHER_COMPLETED_INDEX = 1;

	bool isCompleted = EMonsterKnowledgeState::FinalLevel == _data->State();

	if (EMonsterKnowledgeState::FinalLevel == _data->State())
	{	// 2023/5/5 PKT - 모든 지식을 완료 한 상태

		// 2023/5/5 PKT - 초기화
		_increaseExp = INVALID_MONSTER_KNOWLEDGE_EXP;
		_deltaExp = 0;

		int32 viewLevel = int32(_data->LevelByExp(_data->AccumulateExp()));

		double preMaxExp = (double)_data->ExpNPoint(viewLevel - 2).Key;
		double maxExp = (double)_data->ExpNPoint(viewLevel - 1).Key - preMaxExp;

		_progressBarExp->SetProgressText(FText::FromString(FString::Format(TEXT("{0}/{1}"), { int32(maxExp), int32(maxExp) })));
		_progressBarExp->SetPercent(1.0f);

		_switcherUseItemPanel->SetActiveWidgetIndex(SWITCHER_COMPLETED_INDEX);
		_switcherUnderButton->SetActiveWidgetIndex(SWITCHER_COMPLETED_INDEX);
	}
	else
	{	
		_deltaExp = 0;
		_increaseExp = 0;
		InvalidateAmount(_amountUsed);

		// 2023/2/13 PKT - 현재 경험치로 레벨을 구한다.( 실제 레벨과 다를 수 있음. )
		int32 viewLevel = int32(_data->LevelByExp(_data->AccumulateExp()));

		double preMaxExp = (double)_data->ExpNPoint(viewLevel - 1).Key;
		double curExp = (double)_data->AccumulateExp() - preMaxExp;
		double maxExp = (double)_data->ExpNPoint(viewLevel).Key - preMaxExp;

		_progressBarExp->SetProgressText(FText::FromString(FString::Format(TEXT("{0}/{1}"), { int32(curExp), int32(maxExp) })));
		_progressBarExp->SetPercent(FMath::Clamp<float>(float(curExp) / maxExp, 0.f, 1.0f));
		_progressBarExp->SetSecondPercent(0.f);

		_switcherUseItemPanel->SetActiveWidgetIndex(SWITCHER_USEITEM_INDEX);
		_switcherUnderButton->SetActiveWidgetIndex(SWITCHER_USEITEM_INDEX);
	}	

	// 2023/5/25 PKT - 다음 레벨과 보상 정보를 숨긴다.
	_applyLevelInfo->SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUIPopupMonsterBookDetail::OnLongPressRewardIcon(UGsUIIconItem& InIcon)
{
	UGsUIIconItem::DefaultItemDetailOpener(InIcon, true);
}

void UGsUIPopupMonsterBookDetail::SetData(const FGsMonsterKnowledgeBook* InData)
{
	if (nullptr == InData)
	{
		GSLOG(Error, TEXT("nullptr == InData"));
		return;
	}

	_data = InData;
	
	const FGsSchemaNpcData* schemaNPCData = _data->NPCData();

	// 2023/2/1 PKT - Title
	_textTitle->SetText(schemaNPCData->nameText);

	// 2023/2/1 PKT - Portrait
	_portrait->SetData(InData, true);

	FText textBuffer;
	
	{	// 2023/2/1 PKT - Rank
		switch (schemaNPCData->grade)
		{
			case CreatureRankType::RAID:
				FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("CreatureRankRaid"), textBuffer);
				break;
			case CreatureRankType::BOSS:
				FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("CreatureRankBoss"), textBuffer);
				break;
			case CreatureRankType::NAMED:
				FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("CreatureRankNamed"), textBuffer);
				break;
			case CreatureRankType::ELITE:
				FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("CreatureRankElite"), textBuffer);
				break;
			case CreatureRankType::NORMAL:
			default:
				FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("CreatureRankNormal"), textBuffer);
				break;
		}
		_textRank->SetText(textBuffer);
	}

	{	// 2023/2/1 PKT - 선공/비선공
		if (schemaNPCData->isOffensive)
		{
			FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("MonOffensive"), textBuffer);
		}
		else
		{
			FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("MonNotOffensive"), textBuffer);
		}

		_textOffensive->SetText(textBuffer);
	}

	{	// 2023/2/1 PKT - 종족 타입
		textBuffer = UGsGameObjectUtil::GetRaceText(schemaNPCData->race);
		_textCreatureRaceType->SetText(textBuffer);
	}

	{	// 2021/12/06 PKT - 약점 / 재질
		textBuffer = UGsGameObjectUtil::GetRaceWeaknessItemMaterialText(schemaNPCData->race); 
		_textDefenseMaterial->SetText(textBuffer);
	}

	{	// 2021/12/06 PKT - 속성
		textBuffer = UGsGameObjectUtil::GetMonsterElementalText(schemaNPCData->element);
		_textElement->SetText(textBuffer);
	}
	
	{
		const auto& mapData = _data->SpawnMapData();
		{
			if (nullptr == mapData)
			{
				FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("UI_Ticker_NoRewardInfoInWorldMap"), textBuffer);
			}
			else
			{
				textBuffer = mapData->MapStringId;
			}
			_textSpawnMap->SetText(textBuffer);
		}		
	}	

	if (schemaNPCData->monsterKnowledgeLevelSetId.GetRow())
	{
		const FGsSchemaMonsterKnowledgeLevelSet* schemaLevelSet = schemaNPCData->monsterKnowledgeLevelSetId.GetRow();
		if (schemaLevelSet && schemaLevelSet->knowledgeSurplusItem.GetRow())
		{
			const FGsSchemaItemCommon* schemaItem = schemaLevelSet->knowledgeSurplusItem.GetRow();
			if (schemaItem)
			{
				_iconRewardItem->SetNotOwnItem(schemaItem->id, schemaLevelSet->knowledgeSurplusItemAmount);
				_iconRewardItem->SetOnChangeDisplayDetailWndType(UGsUIIconItem::LongPressType);

				if (false == _iconRewardItem->OnFinishedLongPressEvent.IsBoundToObject(this))
				{	// 2023/2/20 PKT - Long Press Bind
					_iconRewardItem->OnFinishedLongPressEvent.AddUObject(this, &UGsUIPopupMonsterBookDetail::OnLongPressRewardIcon);
				}				
			}
		}
	}

	_increaseExp = INVALID_MONSTER_KNOWLEDGE_EXP;
	_deltaExp = 0;

	Invalidate();
}

const FGsMonsterKnowledgeBook* UGsUIPopupMonsterBookDetail::GetData() const
{
	return _data;
}

void UGsUIPopupMonsterBookDetail::OnInputCancel()
{
	OnClose();
}