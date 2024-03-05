
#include "GsUIMonsterPortrait.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "DataCenter/Public/DataSchema/GameObject/GsSchemaShapeData.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "UMG/Public/Components/Image.h"
#include "../Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperSprite.h"
#include "../UI/UIContent/Helper/GsUIHelper.h"
#include "T1Project.h"


void UGsUIMonsterPortrait::SetImageSprite(UPaperSprite* InSprite)
{
	if (InSprite)
	{
		_imgPortrait->SetBrushFromAtlasInterface(InSprite);
		_imgPortrait->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		_imgPortrait->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUIMonsterPortrait::LoadImage(const FSoftObjectPath& InPath)
{
	// 2021/11/28 PKT - Image가 로드 되기 전까지 화면에서 가린다.
	SetImageSprite(nullptr);

	_lastSoftObjectPath = InPath;

	// 로드되는 사이 이 객체가 어떻게 될지 모르므로 TWeakObjectPtr 사용
	TWeakObjectPtr<UGsUIMonsterPortrait> weakThis(this);
	FGsUIHelper::AsyncLoadSpriteFromPath(InPath, FStreamableDelegateParam::CreateWeakLambda(this, [weakThis, InPath](UObject* inRes)
		{
			// 로드 끝난후라 LoadSpriteFromPath 시 새로 로드하지 않고, 로드된 항목을 찾음
			if (weakThis.IsValid() && weakThis->_lastSoftObjectPath == InPath)
			{
				weakThis->SetImageSprite(Cast<UPaperSprite>(inRes));
			}
		}));
}

void UGsUIMonsterPortrait::SetData(const FGsSchemaNpcData* InData)
{
	if (nullptr == InData && InData->shapeId.GetRow())
	{
		GSLOG(Error, TEXT("nullptr == InData && InData->shapeId.GetRow()"));
		return;
	}	

	LoadImage(InData->shapeId.GetRow()->imageRegionMap);

	/**
	 * Draw Edge
	 */
	const static int32 NORMAL_EDGE = 0;
	const static int32 ELITE_EDGE = 1;
	const static int32 BOSS_EDGE = 2;
	switch (InData->grade)
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
}