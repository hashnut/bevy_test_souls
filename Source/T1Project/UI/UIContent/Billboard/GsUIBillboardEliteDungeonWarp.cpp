
#include "GsUIBillboardEliteDungeonWarp.h"
#include "../GameObject/Data/GsGameObjectData.h"
#include "../GameObject/ObjectClass/GsGameObjectBase.h"
#include "../GameObject/ObjectClass/GsGameObjectPropInteraction.h"
#include "../GameObject/ObjectClass/GsGameObjectPropInteractionPotal.h"
#include "DataSchema/GameObject/Prop/GsSchemaPropData.h"
#include "UMG/Public/Components/WidgetSwitcher.h"



void UGsUIBillboardEliteDungeonWarp::SetTarget(UGsGameObjectBase* InTarget)
{
	Super::SetTarget(InTarget);

	_gameId = 0;
	_maxLimit = 0;
	_textPropName = FText::GetEmpty();

	if (_target.IsValid() && _target->IsObjectType(EGsGameObjectType::PropInteraction))
	{
		UGsGameObjectPropInteraction* objectProp = Cast<UGsGameObjectPropInteraction>(_target.Get());
		if (objectProp)
		{
			_gameId = objectProp->GetData() ? objectProp->GetData()->GetGameId() : 0;
			_maxLimit = objectProp->GetPropTableData() ? objectProp->GetPropTableData()->multipleInteractionCountMax : 0;
			_textPropName = objectProp->GetPropTableData() ? objectProp->GetPropTableData()->nameString : FText::GetEmpty();
		}
	}

	UpdateCount();
}

void UGsUIBillboardEliteDungeonWarp::SetCount(int32 InCount)
{
	SetVisibility(InCount >= 0 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	_textCount = FText::FromString(FString::Format(TEXT("{0}/{1}"), { InCount, _maxLimit }));
	_switcherPlate->SetActiveWidgetIndex(InCount > 0 && InCount == _maxLimit ? 1 : 0);
}

void UGsUIBillboardEliteDungeonWarp::UpdateCount()
{
	if (_target.IsValid() && _target->IsObjectType(EGsGameObjectType::PropInteraction))
	{
		if (auto item = Cast<UGsGameObjectPropInteractionPotal>(_target.Get()))
		{
			SetCount(item->GetUseCount());
		}
	}
}