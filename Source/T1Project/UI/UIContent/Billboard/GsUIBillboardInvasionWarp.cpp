
#include "GsUIBillboardInvasionWarp.h"

#include "DataSchema/GameObject/Prop/GsSchemaPropData.h"

#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsInterServerManager.h"

#include "GameObject/ObjectClass/GsGameObjectPropInteraction.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/Data/GsGameObjectData.h"

#include "UMG/Public/Components/WidgetSwitcher.h"

void UGsUIBillboardInvasionWarp::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGsUIBillboardInvasionWarp::SetTarget(class UGsGameObjectBase* InTarget)
{
	Super::SetTarget(InTarget);

	if (_target.IsValid() && _target->IsObjectType(EGsGameObjectType::PropInteraction))
	{
		_gameId = _target->GetData()? _target->GetData()->GetGameId() : 0;

		auto curLevel = GLevel()->GetCurrentLevelId();
		if (auto data = GSInterServer()->GetInvadeMapData(curLevel).Pin())
		{
			_maxNumInvaders = data->GetEnrollingMaxCountInPortal();
		}
	}

	UpdateInvaderInfo();
}

void UGsUIBillboardInvasionWarp::UpdateInvaderInfo()
{
	if (_target.IsValid() && _target->IsObjectType(EGsGameObjectType::PropInteractInvasion))
	{
		if (auto invasionProp = Cast<UGsGameObjectPropTouchInvasion>(_target.Get()))
		{	
			SetInvaderCount(GGameData()->IsInvadeWorld() ? -1 : invasionProp->GetEnrolledPlayerCountInProp());
		}
		//auto count = GSInterServer()->GetInvaderCountEnrollingInPortal(_gameId);
	}
}

void UGsUIBillboardInvasionWarp::SetInvaderCount(int32 curNum)
{
	SetVisibility(curNum >= 0 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	if (auto target = GetTarget().Get())
	{
		if (target->GetData())
		{
			_textPropName = FText::FromString(target->GetData()->GetName());

			if (_textPropName.IsEmpty())
			{
				if (const auto propData = UGsTableUtil::FindRowById<UGsTablePropData, FGsSchemaPropData>(target->GetData()->GetTableId()))
				{
					_textPropName = propData->nameString;
				}
			}
		}
	}

	FText InvadeUserCount;
	FText::FindText(TEXT("InvadeText"), TEXT("InvadeUserCount"), InvadeUserCount);

	_textInvaderCount = FText::Format(InvadeUserCount, { curNum, _maxNumInvaders });
	_switcherPlate->SetActiveWidgetIndex(curNum > 0 && curNum == _maxNumInvaders ? 1 : 0);
}

