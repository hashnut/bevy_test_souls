#include "GsUIItemCollectionMaterialLevel.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "UMG/Public/Components/Image.h"
#include "UMG/Public/Components/TextBlock.h"





void UGsUIItemCollectionMaterialLevel::SetData(ECollectionMaterialActionType InType, Level InLevel, bool InDrawDelimiter /* = true */)
{
	_imgDelimiter->SetVisibility(InDrawDelimiter ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

	FLinearColor color = FGsUIColorHelper::GetColor(EGsUIColorType::COLLECTION_IMPOSSIBLE);
	if (ECollectionMaterialActionType::ImmediatePossible == InType)
	{
		color = FGsUIColorHelper::GetColor(EGsUIColorType::COLLECTION_IMMEDIATE_POSSIBLE);
	}
	else if (ECollectionMaterialActionType::ConditionalPossible == InType)
	{
		color = FGsUIColorHelper::GetColor(EGsUIColorType::COLLECTION_CONDITIONAL_POSSIBLE);
	}

	FText textLevel = FText::FromString(FString::Printf(TEXT("+%d"), InLevel));
	
	_textLevel->SetText(textLevel);
	_textLevel->SetColorAndOpacity(color);
}