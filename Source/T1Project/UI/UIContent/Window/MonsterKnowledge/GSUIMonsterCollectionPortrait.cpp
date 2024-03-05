
#include "GSUIMonsterCollectionPortrait.h"
#include "../NPCPortrait/GsUIMonsterPortrait.h"
#include "../MonsterKnowledgeCollection/EMonsterKnowledgeEnumData.h"
#include "../MonsterKnowledgeCollection/GsMonsterKnowledgeBook.h"
#include "../UI/UILib/Base/GsUIVFX.h"
#include "../UI/UIContent/Common/GsUIProgressBar.h"
#include "../UI/UIContent/Common/RedDot/GsUIRedDotDefault.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/PanelWidget.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "T1Project.h"


void UGsMonsterCollectionPortraitEntryData::SetData(const FGsMonsterKnowledgeBook* InData)
{
	if (nullptr == InData)
	{
		GSLOG(Error, TEXT("nullptr == InData"));
		return;
	}

	_data = InData;
	_isImmediateAnimation = false;
}

void UGsMonsterCollectionPortraitEntryData::SetData(const class FGsMonsterKnowledgeBook* InData, bool IsIsImmediateAnimation)
{
	if (nullptr == InData)
	{
		GSLOG(Error, TEXT("nullptr == InData"));
		return;
	}

	_data = InData;
	_isImmediateAnimation = IsIsImmediateAnimation;
}

bool UGsMonsterCollectionPortraitEntryData::IsImmediateAnimation() const
{
	bool outData = _isImmediateAnimation;
	_isImmediateAnimation = false;
	return outData;
}

const FGsMonsterKnowledgeBook* UGsMonsterCollectionPortraitEntryData::Get() const
{
	return _data;
}






void UGSUIMonsterCollectionPortrait::BeginDestroy()
{
	Super::BeginDestroy();
}

void UGSUIMonsterCollectionPortrait::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UGSUIMonsterCollectionPortrait::NativeConstruct()
{
	_data = nullptr;
	
	// 2023/2/1 PKT - VFX Off
	_effectFinalLevelUp->HideVFX();
	_effectNormalLevelUp->HideVFX();

	Super::NativeConstruct();
}

void UGSUIMonsterCollectionPortrait::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGSUIMonsterCollectionPortrait::NativeOnListItemObjectSet(UObject* inListItemObject)
{
	TWeakObjectPtr<UGsMonsterCollectionPortraitEntryData> item = Cast<UGsMonsterCollectionPortraitEntryData>(inListItemObject);
	if (false == item.IsValid())
	{
		GSLOG(Error, TEXT("false == item.IsValid()"));
		return;
	}

	SetData(item->Get());
	PlayImmediateAnimation(item->IsImmediateAnimation());
}

void UGSUIMonsterCollectionPortrait::PlayImmediateAnimation(bool IsIsImmediateAnimation)
{
	if (nullptr == _data)
	{
		GSLOG(Error, TEXT("nullptr == _data"));
		return;
	}

	if (IsIsImmediateAnimation)
	{
		(EMonsterKnowledgeState::FinalLevel == _data->State()) ? _effectFinalLevelUp->ShowVFX() : _effectNormalLevelUp->ShowVFX();
	}	
}

void UGSUIMonsterCollectionPortrait::SetData(const FGsMonsterKnowledgeBook* InData, bool InIsOnlyDisplay /* = false */)
{
	if (nullptr == InData)
	{
		GSLOG(Error, TEXT("nullptr == InData"));
		return;
	}

	_data = InData;
	_prevLevel = _data->Level();
	_isOnlyDisplay = InIsOnlyDisplay;

	// 2023/2/1 PKT - 포트레이트
	_npcPortrait->SetData(_data->NPCData());

	/**
	 * Draw Edge
	 */
	const static int32 NORMAL_EDGE = 0;
	const static int32 ELITE_EDGE = 1;
	const static int32 BOSS_EDGE = 2;
	switch (_data->NPCData()->grade)
	{
	case CreatureRankType::ELITE:
		_switcherGradeEdge->SetActiveWidgetIndex(ELITE_EDGE);
		break;
	case CreatureRankType::BOSS:
		_switcherGradeEdge->SetActiveWidgetIndex(BOSS_EDGE);
		break;
	case CreatureRankType::NORMAL:
	default:
		_switcherGradeEdge->SetActiveWidgetIndex(NORMAL_EDGE);
		break;
	};
	
	Invalidate();
}

void UGSUIMonsterCollectionPortrait::Invalidate()
{
	if (nullptr == _data)
	{
		GSLOG(Error, TEXT("nullptr == _data"));
		return;
	}

	// 2023/2/1 PKT - 레벨업이 필요 한가?( 디스 플레이 용도로만 사용 할 때는 레벨업을 할 수 있는 기능을 끈다. )
	bool isLevelUpPossible = (false == _isOnlyDisplay && EMonsterKnowledgeState::LevelUpPossible == _data->State() );
	
	// 2023/2/1 PKT - 최종 레벨 인가?
	bool isCompleted = EMonsterKnowledgeState::FinalLevel == _data->State();

	// 2023/2/1 PKT - LevelUp Image
	_panelLevleUp->SetVisibility(isLevelUpPossible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	// 2023/2/1 PKT - 최종 레벨 일경우 
	_panelCompleted->SetVisibility(isCompleted ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	// 2023/2/1 PKT - RedDot
	_redDot->SetRedDot(isLevelUpPossible);

	if (isLevelUpPossible)
	{
		// 2023/2/10 PKT - 현재 경험치로 받을 수 있는 보상 포인트
		int32 gainRewardPoint = _data->GainRewardPoint(_data->Level(), _data->AccumulateExp());
		_textRewardPoint->SetText(FText::AsNumber(gainRewardPoint));		
	}

	if (isCompleted)
	{
		_expProgressBar->SetProgressText(FText::GetEmpty());
		_expProgressBar->SetPercent(1.0f);
	}
	else
	{
		MonsterKnowledgeExp totalMaxExp = _data->ExpNPoint(_data->MaxLevel() - 1).Key;		

		MonsterKnowledgeExp curExp = FMath::Min(_data->AccumulateExp(), totalMaxExp);
		MonsterKnowledgeExp curMaxExp = _data->ExpNPoint(_data->Level()).Key;

		MonsterKnowledgeExp prevMaxExp = _data->ExpNPoint(_data->Level() - 1).Key;

		float percent = float(curExp - prevMaxExp) / (curMaxExp - prevMaxExp);

		_expProgressBar->SetProgressText(FText::FromString(FString::Format(TEXT("{0}/{1}"), { curExp - prevMaxExp, curMaxExp - prevMaxExp })));
		_expProgressBar->SetPercent(FMath::Clamp(percent, 0.f, 1.0f));
	}	

	// 2023/2/1 PKT - Level은 1부터 시작
	FString textLevel = FString::Format(TEXT("{0}"), { _data->Level() + 1});
	_textLevel->SetText(FText::FromString(textLevel));

	if (_prevLevel != _data->Level())
	{
		// 2023/2/6 PKT - 이전 레벨과 다르다면 레벨업을 한 것임.
		if (isCompleted)
		{
			_effectFinalLevelUp->ShowVFX();
		}
		else
		{
			_effectNormalLevelUp->ShowVFX();
		}

		_prevLevel = _data->Level();
	}
}

const FGsMonsterKnowledgeBook* UGSUIMonsterCollectionPortrait::GetData() const
{
	return _data;
}