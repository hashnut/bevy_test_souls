#include "GsUITrayTickerBossDespawn.h"

#include "UTIL/GsTableUtil.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "DataSchema/GameObject/GsSchemaShapeData.h"

#include "UMG/Public/Animation/WidgetAnimation.h"
#include "UMG/Public/Components/Image.h"
#include "Paper2D/Classes/PaperSprite.h"

void UGsUITrayTickerBossDespawn::NativeConstruct()
{
	Super::NativeConstruct();

	
	float endTime = _animationTray->GetEndTime();
	PlayAnimation(_animationTray, endTime, 1, EUMGSequencePlayMode::Reverse);

}


// 데이터 세팅
void UGsUITrayTickerBossDespawn::SetDespawnData(int32 In_creatureId)
{
	float endTime = _animationTray->GetEndTime();
	PlayAnimation(_animationTray, endTime, 1, EUMGSequencePlayMode::Reverse);

	// 이미지 갱신
	InvalidBossImage(In_creatureId);


	// 애니 시작
	PlayAnimation(_animationTray);

	FScriptDelegate animationDelegate;
	animationDelegate.BindUFunction(this, "FinishAnimationBP");
	// 애니메이션 종료 콜백 연동(닫기 처리용)
	BindToAnimationFinished(_animationTray, FWidgetAnimationDynamicEvent(animationDelegate));
}

// 연출 끝
void UGsUITrayTickerBossDespawn::FinishAnimationBP()
{
	FScriptDelegate animationDelegate;
	animationDelegate.BindUFunction(this, "FinishAnimationBP");

	UnbindFromAnimationFinished(_animationTray, FWidgetAnimationDynamicEvent(animationDelegate));

	// 닫기
	Close();
}
void UGsUITrayTickerBossDespawn::InvalidBossImage(int32 In_creatrueId)
{
	//_creatureId	
	const FGsSchemaNpcData* npcData = UGsTableUtil::FindRowById<UGsTableNpcData, FGsSchemaNpcData>(In_creatrueId);
	if (nullptr == npcData)
	{
		return;
	}

	const FGsSchemaShapeData* shape = npcData->shapeId.GetRow();
	if (shape)
	{
		UPaperSprite* spriteImg = FGsUIHelper::LoadSpriteFromPath(shape->imageRegionMap);
		_bossImage->SetBrushFromAtlasInterface(spriteImg);
	}

	// 텍스트 세팅

	// 1. 포맷 검색
	FText findText;
	FText::FindText(TEXT("FieldBossText"), TEXT("BossDespawn"), findText);

	// 2. 구성
	FText resultText = FText::Format(findText, npcData->nameText);

	_textDespawn = resultText;
}